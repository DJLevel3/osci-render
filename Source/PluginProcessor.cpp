/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscirenderAudioProcessor::OscirenderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    {

    permanentEffects.push_back(visualiserParameters.brightnessEffect);
    permanentEffects.push_back(visualiserParameters.intensityEffect);
    permanentEffects.push_back(visualiserParameters.persistenceEffect);
    permanentEffects.push_back(visualiserParameters.hueEffect);
    permanentEffects.push_back(visualiserParameters.saturationEffect);
    permanentEffects.push_back(visualiserParameters.focusEffect);

    allEffects.insert(allEffects.end(), permanentEffects.begin(), permanentEffects.end());

    for (auto effect : allEffects) {
        for (auto effectParameter : effect->parameters) {
            auto parameters = effectParameter->getParameters();
            for (auto parameter : parameters) {
                addParameter(parameter);
            }
        }
    }
    booleanParameters.push_back(visualiserParameters.graticuleEnabled);
    booleanParameters.push_back(visualiserParameters.smudgesEnabled);
    booleanParameters.push_back(visualiserParameters.upsamplingEnabled);
    booleanParameters.push_back(visualiserParameters.legacyVisualiserEnabled);
    booleanParameters.push_back(visualiserParameters.visualiserFullScreen);

    for (auto parameter : booleanParameters) {
        addParameter(parameter);
    }
}

OscirenderAudioProcessor::~OscirenderAudioProcessor() {
}

const juce::String OscirenderAudioProcessor::getName() const {
    return JucePlugin_Name;
}

void OscirenderAudioProcessor::setAudioThreadCallback(std::function<void(const juce::AudioBuffer<float>&)> callback) {
    juce::SpinLock::ScopedLockType lock(audioThreadCallbackLock);
    audioThreadCallback = callback;
}

bool OscirenderAudioProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OscirenderAudioProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OscirenderAudioProcessor::isMidiEffect() const {
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OscirenderAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int OscirenderAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OscirenderAudioProcessor::getCurrentProgram() {
    return 0;
}

void OscirenderAudioProcessor::setCurrentProgram(int index) {
}

const juce::String OscirenderAudioProcessor::getProgramName(int index) {
    return {};
}

void OscirenderAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

void OscirenderAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	currentSampleRate = sampleRate;
    volumeBuffer = std::vector<double>(VOLUME_BUFFER_SECONDS * sampleRate, 0);
}

void OscirenderAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OscirenderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


// effectsLock should be held when calling this
BooleanParameter* OscirenderAudioProcessor::getBooleanParameter(juce::String id) {
    for (auto& parameter : booleanParameters) {
        if (parameter->paramID == id) {
            return parameter;
        }
    }
    return nullptr;
}

void OscirenderAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    // Audio info variables
    int totalNumOutputChannels = getTotalNumOutputChannels();

    const double EPSILON = 0.00001;
    
    auto* channelData = buffer.getArrayOfWritePointers();
    
    
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

		double x = buffer.getSample(0, sample);
		double y = buffer.getSample(1, sample);

        if (totalNumOutputChannels >= 2) {
            channelData[0][sample] = x;
            channelData[1][sample] = y;
        }
        else if (totalNumOutputChannels == 1) {
            channelData[0][sample] = x;
        }

        // clip
        x = juce::jmax(-1.0, juce::jmin(1.0, x));
        y = juce::jmax(-1.0, juce::jmin(1.0, y));
        

        {
            juce::SpinLock::ScopedLockType scope(consumerLock);
            for (auto consumer : consumers) {
                consumer->write(Point(x, y, 1));
                consumer->notifyIfFull();
            }
        }
	}

    // used for any callback that must guarantee all audio is recieved (e.g. when recording to a file)
    juce::SpinLock::ScopedLockType lock(audioThreadCallbackLock);
    if (audioThreadCallback != nullptr) {
        audioThreadCallback(buffer);
    }
}

//==============================================================================
bool OscirenderAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OscirenderAudioProcessor::createEditor() {
    auto editor = new OscirenderAudioProcessorEditor(*this);
    return editor;
}

//==============================================================================
void OscirenderAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::SpinLock::ScopedLockType lock1(parsersLock);
    juce::SpinLock::ScopedLockType lock2(effectsLock);

    std::unique_ptr<juce::XmlElement> xml = std::make_unique<juce::XmlElement>("project");
    xml->setAttribute("version", ProjectInfo::versionString);

    auto booleanParametersXml = xml->createNewChildElement("booleanParameters");
    for (auto parameter : booleanParameters) {
        auto parameterXml = booleanParametersXml->createNewChildElement("parameter");
        parameter->save(parameterXml);
    }
    
    auto visualiserXml = xml->createNewChildElement("visualiser");
    visualiserXml->setAttribute("roughness", visualiserParameters.roughness);
    visualiserXml->setAttribute("intensity", visualiserParameters.intensity);

    copyXmlToBinary(*xml, destData);
}

void OscirenderAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml;

    const uint32_t magicXmlNumber = 0x21324356;
    if (sizeInBytes > 8 && juce::ByteOrder::littleEndianInt(data) == magicXmlNumber) {
        // this is a binary xml format
        xml = getXmlFromBinary(data, sizeInBytes);
    } else {
        // this is a text xml format
        xml = juce::XmlDocument::parse(juce::String((const char*)data, sizeInBytes));
    }

    if (xml.get() != nullptr && xml->hasTagName("project")) {
        auto versionXml = xml->getChildByName("version");
        if (versionXml != nullptr && versionXml->getAllSubText().startsWith("v1.")) {
            return;
        }
        auto version = xml->hasAttribute("version") ? xml->getStringAttribute("version") : "2.0.0";

        juce::SpinLock::ScopedLockType lock1(parsersLock);
        juce::SpinLock::ScopedLockType lock2(effectsLock);

        auto booleanParametersXml = xml->getChildByName("booleanParameters");
        if (booleanParametersXml != nullptr) {
            for (auto parameterXml : booleanParametersXml->getChildIterator()) {
                auto parameter = getBooleanParameter(parameterXml->getStringAttribute("id"));
                if (parameter != nullptr) {
                    parameter->load(parameterXml);
                }
            }
        }
        
        auto visualiserXml = xml->getChildByName("visualiser");
        if (visualiserXml != nullptr) {
            visualiserParameters.roughness = visualiserXml->getIntAttribute("roughness");
            visualiserParameters.intensity = visualiserXml->getDoubleAttribute("intensity");
        }

        broadcaster.sendChangeMessage();
    }
}

void OscirenderAudioProcessor::parameterValueChanged(int parameterIndex, float newValue) {
    
}

void OscirenderAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting) {}

void updateIfApproxEqual(FloatParameter* parameter, float newValue) {
    if (std::abs(parameter->getValueUnnormalised() - newValue) > 0.0001) {
        parameter->setUnnormalisedValueNotifyingHost(newValue);
    }
}

double OscirenderAudioProcessor::getSampleRate() {
    return currentSampleRate;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscirenderAudioProcessor();
}
