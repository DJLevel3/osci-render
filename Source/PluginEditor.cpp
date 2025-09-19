#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h>

OscirenderAudioProcessorEditor::OscirenderAudioProcessorEditor(OscirenderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p), collapseButton("Collapse", juce::Colours::white, juce::Colours::white, juce::Colours::white)
{
#if JUCE_LINUX
    // use OpenGL on Linux for much better performance. The default on Mac is CoreGraphics, and on Window is Direct2D which is much faster.
    openGlContext.attachTo(*getTopLevelComponent());
#endif

    setLookAndFeel(&lookAndFeel);

#if JUCE_MAC
    if (audioProcessor.wrapperType == juce::AudioProcessor::WrapperType::wrapperType_Standalone) {
        usingNativeMenuBar = true;
        menuBarModel.setMacMainMenu(&menuBarModel);
    }
#endif

    menuBar.setModel(&menuBarModel);
    addAndMakeVisible(menuBar);

    colourScheme = lookAndFeel.getDefaultColourScheme();

    {   
        juce::MessageManagerLock lock;
        audioProcessor.broadcaster.addChangeListener(this);
    }

    if (juce::JUCEApplicationBase::isStandaloneApp()) {
        if (juce::TopLevelWindow::getNumTopLevelWindows() > 0) {
            juce::TopLevelWindow* w = juce::TopLevelWindow::getTopLevelWindow(0);
            juce::DocumentWindow* dw = dynamic_cast<juce::DocumentWindow*>(w);
            if (dw != nullptr) {
                dw->setBackgroundColour(Colours::veryDark);
                dw->setColour(juce::ResizableWindow::backgroundColourId, Colours::veryDark);
                dw->setTitleBarButtonsRequired(juce::DocumentWindow::allButtons, false);
                dw->setUsingNativeTitleBar(true);
            }
        }

        juce::StandalonePluginHolder* standalone = juce::StandalonePluginHolder::getInstance();
        if (standalone != nullptr) {
            standalone->getMuteInputValue().setValue(false);
        }
    }

    setSize(640, 640);
    setResizable(true, true);
    setResizeLimits(500, 500, 2000, 2000);

    layout.setItemLayout(0, -0.3, -1.0, -0.7);
    layout.setItemLayout(1, RESIZER_BAR_SIZE, RESIZER_BAR_SIZE, RESIZER_BAR_SIZE);
    layout.setItemLayout(2, -0.0, -1.0, -0.3);

    addAndMakeVisible(settings);
    addAndMakeVisible(resizerBar);

    addAndMakeVisible(visualiser);

    visualiser.openSettings = [this] {
        visualiserSettingsWindow.setVisible(true);
        visualiserSettingsWindow.toFront(true);
    };

    visualiser.closeSettings = [this] {
        visualiserSettingsWindow.setVisible(false);
    };

    visualiserSettingsWindow.setResizable(true, true);
#if JUCE_WINDOWS
    // if not standalone, use native title bar for compatibility with DAWs
    visualiserSettingsWindow.setUsingNativeTitleBar(processor.wrapperType == juce::AudioProcessor::WrapperType::wrapperType_Standalone);
#elif JUCE_MAC
    visualiserSettingsWindow.setUsingNativeTitleBar(true);
#endif
    visualiserSettings.setLookAndFeel(&getLookAndFeel());
    visualiserSettings.setSize(550, 500);
    visualiserSettingsWindow.setContentNonOwned(&visualiserSettings, true);
    visualiserSettingsWindow.centreWithSize(550, 500);
    
    tooltipDropShadow.setOwner(&tooltipWindow);
}

OscirenderAudioProcessorEditor::~OscirenderAudioProcessorEditor() {
    setLookAndFeel(nullptr);
    juce::Desktop::getInstance().setDefaultLookAndFeel(nullptr);
    juce::MessageManagerLock lock;
    audioProcessor.broadcaster.removeChangeListener(this);
    
#if JUCE_MAC
    if (usingNativeMenuBar) {
        menuBarModel.setMacMainMenu(nullptr);
    }
#endif
}

void OscirenderAudioProcessorEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void OscirenderAudioProcessorEditor::resized() {
    auto area = getLocalBounds();
    menuBar.setBounds(area.removeFromTop(25));

    visualiser.setBounds(area);
    return;
}

void OscirenderAudioProcessorEditor::handleAsyncUpdate() {
    resized();
}

void OscirenderAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* source) {
    
    if (source == &audioProcessor.broadcaster) {
        {
            juce::SpinLock::ScopedLockType lock(audioProcessor.parsersLock);
            settings.update();
        }
        resized();
        repaint();
    }
}

void OscirenderAudioProcessorEditor::toggleLayout(juce::StretchableLayoutManager& layout, double prefSize) {
    double minSize, maxSize, preferredSize;
    double otherMinSize, otherMaxSize, otherPreferredSize;
    layout.getItemLayout(2, minSize, maxSize, preferredSize);
    layout.getItemLayout(0, otherMinSize, otherMaxSize, otherPreferredSize);

    if (preferredSize == CLOSED_PREF_SIZE) {
        double otherPrefSize = -(1 + prefSize);
        if (prefSize > 0) {
            otherPrefSize = -1.0;
        }
        layout.setItemLayout(2, CLOSED_PREF_SIZE, maxSize, prefSize);
        layout.setItemLayout(0, CLOSED_PREF_SIZE, otherMaxSize, otherPrefSize);
    } else {
        layout.setItemLayout(2, CLOSED_PREF_SIZE, maxSize, CLOSED_PREF_SIZE);
        layout.setItemLayout(0, CLOSED_PREF_SIZE, otherMaxSize, -1.0);
    }
}

bool OscirenderAudioProcessorEditor::keyPressed(const juce::KeyPress& key) {
    bool consumeKey = false;

    return consumeKey;
}

void OscirenderAudioProcessorEditor::mouseDown(const juce::MouseEvent& e) {
}

void OscirenderAudioProcessorEditor::mouseMove(const juce::MouseEvent& event) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void OscirenderAudioProcessorEditor::updateTitle() {
    juce::String title = "osci-viewport2";
    getTopLevelComponent()->setName(title);
}

void OscirenderAudioProcessorEditor::openAudioSettings() {
    juce::StandalonePluginHolder* standalone = juce::StandalonePluginHolder::getInstance();
    standalone->showAudioSettingsDialog();
}

void OscirenderAudioProcessorEditor::resetToDefault() {
    juce::StandaloneFilterWindow* window = findParentComponentOfClass<juce::StandaloneFilterWindow>();
    if (window != nullptr) {
        window->resetToDefaultState();
    }
}
