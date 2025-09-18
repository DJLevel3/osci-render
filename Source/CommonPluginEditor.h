#pragma once

#include <JuceHeader.h>
#include "CommonPluginProcessor.h"
#include "LookAndFeel.h"
#include "components/SosciMainMenuBarModel.h"
#include "components/SvgButton.h"
#include "components/VolumeComponent.h"
#include "components/DownloaderComponent.h"

#if DEBUG
#if MELATONIN
    #include "melatonin_inspector/melatonin_inspector.h"
#endif
#endif

class CommonPluginEditor : public juce::AudioProcessorEditor {
public:
    CommonPluginEditor(CommonAudioProcessor&, juce::String appName, juce::String projectFileType, int width, int height);
    ~CommonPluginEditor() override;

    void handleCommandLine(const juce::String& commandLine);
    void initialiseMenuBar(juce::MenuBarModel& menuBarModel);
    void openProject(const juce::File& file);
    void openProject();
    void saveProject();
    void saveProjectAs();
    void updateTitle();
    void fileUpdated(juce::String fileName);
    void openAudioSettings();
    void resetToDefault();
    void resized() override;

private:
    CommonAudioProcessor& audioProcessor;
    bool fullScreen = false;
public:
    OscirenderLookAndFeel lookAndFeel;

    juce::String appName;
    juce::String projectFileType;
    juce::String currentFileName;
    
#if OSCI_PREMIUM
    DownloaderComponent ffmpegDownloader;
    SharedTextureManager sharedTextureManager;
#endif

    VolumeComponent volume{audioProcessor};

    std::unique_ptr<juce::FileChooser> chooser;
    juce::MenuBarComponent menuBar;
    juce::SharedResourcePointer<juce::TooltipWindow> tooltipWindow;
    juce::DropShadower tooltipDropShadow{juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 6, {0,0})};

    bool usingNativeMenuBar = false;

#if JUCE_LINUX
    juce::OpenGLContext openGlContext;
#endif

#if DEBUG
#if MELATONIN
    melatonin::Inspector inspector { *this, false };
#endif
#endif

    bool keyPressed(const juce::KeyPress& key) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommonPluginEditor)
};
