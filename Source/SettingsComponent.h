#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainComponent.h"

class OscirenderAudioProcessorEditor;
class SettingsComponent : public juce::Component {
public:
	SettingsComponent(OscirenderAudioProcessor&, OscirenderAudioProcessorEditor&);

	void resized() override;
	void update();
	void mouseMove(const juce::MouseEvent& event) override;
	void mouseDown(const juce::MouseEvent& event) override;

private:
	OscirenderAudioProcessor& audioProcessor;
	OscirenderAudioProcessorEditor& pluginEditor;

	MainComponent main{audioProcessor, pluginEditor};

	juce::StretchableLayoutManager mainLayout;
	juce::StretchableLayoutResizerBar mainResizerBar{&mainLayout, 1, true};

	double prefSizes[1] = { 300 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsComponent)
};
