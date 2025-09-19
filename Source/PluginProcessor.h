/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#define VERSION_HINT 2

#include <JuceHeader.h>
#include "shape/Shape.h"
#include "concurrency/ConsumerManager.h"
#include "components/VisualiserSettings.h"
#include "audio/SampleRateManager.h"
#include <numbers>

//==============================================================================
/**
*/
class OscirenderAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorParameter::Listener, public ConsumerManager, public SampleRateManager
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    OscirenderAudioProcessor();
    ~OscirenderAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    void setAudioThreadCallback(std::function<void(const juce::AudioBuffer<float>&)> callback);

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;
    double getSampleRate() override;

    std::atomic<double> currentSampleRate = 0.0;

    juce::SpinLock parsersLock;
    juce::SpinLock effectsLock;

    VisualiserParameters visualiserParameters;

    juce::ChangeBroadcaster broadcaster;
private:

    juce::SpinLock audioThreadCallbackLock;
    std::function<void(const juce::AudioBuffer<float>&)> audioThreadCallback;

    std::vector<BooleanParameter*> booleanParameters;
    std::vector<std::shared_ptr<Effect>> allEffects;
    std::vector<std::shared_ptr<Effect>> permanentEffects;

    const double VOLUME_BUFFER_SECONDS = 0.1;

    std::vector<double> volumeBuffer;
    int volumeBufferIndex = 0;
    double squaredVolume = 0;
    double currentVolume = 0;

    BooleanParameter* getBooleanParameter(juce::String id);
    FloatParameter* getFloatParameter(juce::String id);
    IntParameter* getIntParameter(juce::String id);

    void parseVersion(int result[3], const juce::String& input) {
        std::istringstream parser(input.toStdString());
        parser >> result[0];
        for (int idx = 1; idx < 3; idx++) {
            parser.get(); //Skip period
            parser >> result[idx];
        }
    }

    bool lessThanVersion(const juce::String& a, const juce::String& b) {
        int parsedA[3], parsedB[3];
        parseVersion(parsedA, a);
        parseVersion(parsedB, b);
        return std::lexicographical_compare(parsedA, parsedA + 3, parsedB, parsedB + 3);
    }

    const double MIN_LENGTH_INCREMENT = 0.000001;

    juce::AudioPlayHead* playHead;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscirenderAudioProcessor)
};
