#include "SosciPluginProcessor.h"
#include "SosciPluginEditor.h"
#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>

SosciPluginEditor::SosciPluginEditor(SosciAudioProcessor& p) : CommonPluginEditor(p, "sosci", "sosci", 1180, 750), audioProcessor(p) {
    initialiseMenuBar(model);
    addAndMakeVisible(volume);
    addAndMakeVisible(visualiserSettingsWrapper);

    resized();
}

SosciPluginEditor::~SosciPluginEditor() {
    menuBar.setModel(nullptr);
}

void SosciPluginEditor::paint(juce::Graphics& g) {
    g.fillAll(Colours::veryDark);
}

void SosciPluginEditor::resized() {
    auto area = getLocalBounds();

    menuBar.setBounds(area.removeFromTop(25));

    auto volumeArea = area.removeFromLeft(30);
    volume.setBounds(volumeArea.withSizeKeepingCentre(volumeArea.getWidth(), juce::jmin(volumeArea.getHeight(), 300)));

    auto settingsArea = area.removeFromRight(juce::jmax(juce::jmin(0.4 * getWidth(), 550.0), 350.0));
    visualiserSettings.setSize(settingsArea.getWidth(), 550);
    visualiserSettingsWrapper.setBounds(settingsArea);

    if (area.getWidth() < 10 || area.getHeight() < 10) {
        return;
    }
    visualiser.setBounds(area);
}

bool SosciPluginEditor::isInterestedInFileDrag(const juce::StringArray& files) {
    if (files.size() != 1) {
        return false;
    }
    juce::File file(files[0]);
    return 
        file.hasFileExtension("wav") ||
        file.hasFileExtension("mp3") ||
        file.hasFileExtension("aiff") ||
        file.hasFileExtension("flac") ||
        file.hasFileExtension("ogg");
}

void SosciPluginEditor::filesDropped(const juce::StringArray& files, int x, int y) {
    if (files.size() != 1) {
        return;
    }
    juce::File file(files[0]);
    audioProcessor.loadAudioFile(file);
}
