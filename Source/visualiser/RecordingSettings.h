#pragma once

#include <JuceHeader.h>
#include "../components/EffectComponent.h"
#include "../components/SvgButton.h"
#include "../LookAndFeel.h"
#include "../components/SwitchButton.h"

#define VERSION_HINT 2

// Define codec options
enum class VideoCodec {
    H264,
    H265,
    VP9,
#if JUCE_MAC
    ProRes,
#endif
};