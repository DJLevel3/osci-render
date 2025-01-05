/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CommonPluginProcessor.h"
#include "concurrency/AudioBackgroundThread.h"
#include "concurrency/AudioBackgroundThreadManager.h"
#include "audio/SampleRateManager.h"
#include "visualiser/VisualiserSettings.h"
#include "audio/Effect.h"
#include "wav/WavParser.h"

//==============================================================================
/**
*/
class SosciAudioProcessor  : public CommonAudioProcessor
{
public:
    SosciAudioProcessor();
    ~SosciAudioProcessor() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    
    void loadAudioFile(const juce::File& file);
    void stopAudioFile();

    juce::AudioProcessorEditor* createEditor() override;

private:
    juce::CriticalSection wavParserLock;
    std::unique_ptr<WavParser> wavParser;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SosciAudioProcessor)
};
