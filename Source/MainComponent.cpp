#include "MainComponent.h"
#include "parser/FileParser.h"
#include "parser/FrameProducer.h"
#include "PluginEditor.h"

MainComponent::MainComponent(OscirenderAudioProcessor& p, OscirenderAudioProcessorEditor& editor) : audioProcessor(p), pluginEditor(editor) {
	setText("Main Settings");

	BooleanParameter* visualiserFullScreen = audioProcessor.visualiserParameters.visualiserFullScreen;

	if (!visualiserFullScreen->getBoolValue()) {
		addAndMakeVisible(pluginEditor.visualiser);
	}
	pluginEditor.visualiser.setFullScreenCallback([this, visualiserFullScreen](FullScreenMode mode) {
		if (mode == FullScreenMode::TOGGLE) {
			visualiserFullScreen->setBoolValueNotifyingHost(!visualiserFullScreen->getBoolValue());
		} else if (mode == FullScreenMode::FULL_SCREEN) {
			visualiserFullScreen->setBoolValueNotifyingHost(true);
		} else if (mode == FullScreenMode::MAIN_COMPONENT) {
			visualiserFullScreen->setBoolValueNotifyingHost(false);
        }
        
		pluginEditor.visualiser.setFullScreen(visualiserFullScreen->getBoolValue());
		
		pluginEditor.resized();
		pluginEditor.repaint();
		resized();
		repaint();
    });

	addAndMakeVisible(recorder);
}

MainComponent::~MainComponent() {}

void MainComponent::resized() {
    juce::Rectangle<int> bounds = getLocalBounds().withTrimmedTop(20).reduced(20);
	auto buttonWidth = 120;
	auto buttonHeight = 30;
	auto padding = 10;
	auto rowPadding = 10;

	recorder.setBounds(bounds.removeFromBottom(30));
	bounds.removeFromBottom(padding);

	bounds.removeFromTop(padding);
	if (!audioProcessor.visualiserParameters.visualiserFullScreen->getBoolValue()) {
		auto minDim = juce::jmin(bounds.getWidth(), bounds.getHeight());
        juce::Point<int> localTopLeft = {bounds.getX(), bounds.getY()};
        juce::Point<int> topLeft = pluginEditor.getLocalPoint(this, localTopLeft);
        auto shiftedBounds = bounds;
        shiftedBounds.setX(topLeft.getX());
        shiftedBounds.setY(topLeft.getY());
		pluginEditor.visualiser.setBounds(shiftedBounds.withSizeKeepingCentre(minDim, minDim));
	}
}
