#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "parser/FileParser.h"
#include "parser/FrameProducer.h"
#include "components/VisualiserComponent.h"
#include "audio/PitchDetector.h"
#include "UGen/ugen_JuceEnvelopeComponent.h"
#include "components/SvgButton.h"
#include "components/AudioRecordingComponent.h"

class OscirenderAudioProcessorEditor;
class MainComponent : public juce::GroupComponent {
public:
	MainComponent(OscirenderAudioProcessor&, OscirenderAudioProcessorEditor&);
	~MainComponent() override;

	void resized() override;
private:
	OscirenderAudioProcessor& audioProcessor;
	OscirenderAudioProcessorEditor& pluginEditor;
    
	AudioRecordingComponent recorder{audioProcessor};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
