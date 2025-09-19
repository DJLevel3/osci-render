#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SettingsComponent.h"
#include "components/MainMenuBarModel.h"
#include "LookAndFeel.h"
#include "components/VisualiserSettings.h"

class OscirenderAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::AsyncUpdater, public juce::ChangeListener {
public:
    OscirenderAudioProcessorEditor(OscirenderAudioProcessor&);
    ~OscirenderAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    void handleAsyncUpdate() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void toggleLayout(juce::StretchableLayoutManager& layout, double prefSize);

    void updateTitle();
    void openAudioSettings();
    void resetToDefault();

private:
    OscirenderAudioProcessor& audioProcessor;
public:

    const double CLOSED_PREF_SIZE = 30.0;
    const double RESIZER_BAR_SIZE = 7.0;

    OscirenderLookAndFeel lookAndFeel;

    VisualiserSettings visualiserSettings = VisualiserSettings(audioProcessor.visualiserParameters);
    SettingsWindow visualiserSettingsWindow = SettingsWindow("Visualiser Settings");
    VisualiserComponent visualiser{audioProcessor, audioProcessor, visualiserSettings, nullptr, audioProcessor.visualiserParameters.legacyVisualiserEnabled->getBoolValue()};

    SettingsComponent settings{audioProcessor, *this};
    
    juce::CodeEditorComponent::ColourScheme colourScheme;
    juce::XmlTokeniser xmlTokeniser;
	juce::ShapeButton collapseButton;

    MainMenuBarModel menuBarModel{audioProcessor, *this};
    juce::MenuBarComponent menuBar;

    juce::StretchableLayoutManager layout;
    juce::StretchableLayoutResizerBar resizerBar{&layout, 1, true};

    juce::StretchableLayoutManager luaLayout;
    juce::StretchableLayoutResizerBar luaResizerBar{&luaLayout, 1, false};

    juce::TooltipWindow tooltipWindow{nullptr, 0};
    juce::DropShadower tooltipDropShadow{juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 6, {0,0})};

    bool usingNativeMenuBar = false;

#if JUCE_LINUX
    juce::OpenGLContext openGlContext;
#endif

    bool keyPressed(const juce::KeyPress& key) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscirenderAudioProcessorEditor)
};
