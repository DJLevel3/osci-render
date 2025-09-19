#include "SettingsComponent.h"
#include "PluginEditor.h"

SettingsComponent::SettingsComponent(OscirenderAudioProcessor& p, OscirenderAudioProcessorEditor& editor) : audioProcessor(p), pluginEditor(editor) {
    addAndMakeVisible(main);
    addAndMakeVisible(mainResizerBar);

    mainLayout.setItemLayout(0, -0.1, -0.9, -0.4);
    mainLayout.setItemLayout(1, pluginEditor.RESIZER_BAR_SIZE, pluginEditor.RESIZER_BAR_SIZE, pluginEditor.RESIZER_BAR_SIZE);
    mainLayout.setItemLayout(2, -0.1, -0.9, -0.6);
}


void SettingsComponent::resized() {
    auto area = getLocalBounds();
    area.removeFromLeft(5);
    area.removeFromRight(5);
    area.removeFromTop(5);
    area.removeFromBottom(5);

    juce::Component dummy;
    juce::Component dummy2;

    juce::Component* columns[] = { &dummy2, &mainResizerBar, &dummy };
    mainLayout.layOutComponents(columns, 3, dummy.getX(), dummy.getY(), dummy.getWidth(), dummy.getHeight(), false, true);

    auto bounds = dummy2.getBounds();
    main.setBounds(bounds);

    repaint();
}

void SettingsComponent::update() {
}

void SettingsComponent::mouseMove(const juce::MouseEvent& event) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void SettingsComponent::mouseDown(const juce::MouseEvent& event) {
}
