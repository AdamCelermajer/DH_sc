// Batch e1-glitch-scene-collada-gui-glitch_collada_CTimelineController-0.
// Bodies for glitch::collada::CTimelineController, rebuilt from the Hex-Rays
// pseudocode. The class declaration is in glitch-scene-collada-gui/collada_misc.hpp.

#include "glitch-scene-collada-gui/collada_misc.hpp"

#include <cmath>

namespace glitch::collada {

namespace {

// The original converts an int to float, divides by the double literal 1000.0 and
// stores the result as float: (float)ms / 1000.0.
float msToSeconds(std::int32_t ms) {
    return static_cast<float>(static_cast<float>(ms) / 1000.0);
}

}  // namespace

// 0x666c20 _ZN6glitch7collada19CTimelineController8setScaleEf
// The original returns this in r0; C++ returns void.
void CTimelineController::setScale(float value) {
    scale = value;  // +0x30
}

// 0x666c28 _ZN6glitch7collada19CTimelineController7setLoopEb
void CTimelineController::setLoop(bool enabled) {
    loop = enabled;  // +0x18
}

// 0x666c30 _ZNK6glitch7collada19CTimelineController7getLoopEv
bool CTimelineController::getLoop() const {
    return loop;  // +0x18
}

// 0x666c38 _ZN6glitch7collada19CTimelineController8setRangeEiib
// The original returns r0 here: a float from the jumpTo call when apply is set,
// otherwise the range length or this (depending on clipTable). C++ returns void.
void CTimelineController::setRange(std::int32_t startMs, std::int32_t endMs, bool apply) {
    if (clipTable == nullptr) {  // +0x34 is zero
        rangeStartMs = startMs;                      // +0x10
        rangeEndMs = endMs;                          // +0x14
        rangeStartSec = msToSeconds(startMs);        // +0x20
        rangeLengthSec = msToSeconds(endMs - startMs);  // +0x24
    }
    if (apply) {
        jumpTo(rangeStartMs);  // vtable +0x0c, the float result is not used here
    }
}

// 0x666d3c _ZNK6glitch7collada19CTimelineController19getCurrentClipStartEv
std::uint32_t CTimelineController::getCurrentClipStart() const {
    return getClipStart(currentClipIndex);  // vtable +0x20
}

// 0x666d54 _ZNK6glitch7collada19CTimelineController17getCurrentClipEndEv
std::uint32_t CTimelineController::getCurrentClipEnd() const {
    return getClipEnd(currentClipIndex);  // vtable +0x24
}

// 0x666d9c _ZNK6glitch7collada19CTimelineController19getCurrentClipIndexEv
std::int32_t CTimelineController::getCurrentClipIndex() const {
    return currentClipIndex;  // +0x38
}

// 0x666db8 _ZN6glitch7collada19CTimelineControllerC2Ev (partial, body not written)
// This is the VTT form (C2). It takes the VTT as its second argument and writes the
// base subobjects through the vbase offsets stored at vtable -12. The class has no
// virtual bases in collada_misc.hpp, so those writes cannot be expressed. Missing:
// the virtual-base layout and the VTT's three vtable pointers. The non-base field
// writes match the C1 constructor below.

// 0x666e40 _ZN6glitch7collada19CTimelineControllerC1Ev (partial)
// Field values are the member default initializers in collada_misc.hpp:
// +0x04 0, +0x08 0, +0x18 1 (loop), +0x2c 0.0, +0x30 1.0 (scale), +0x34 0, +0x38 0,
// +0x3c 0, +0x3d 0. The original also sets the vtable of the secondary base at +0x40
// (not modelled) and writes an int 1 at +0x44 (inside the secondary base, not
// mapped). The +0x44 write is missing, so the row stays partial.
CTimelineController::CTimelineController() {}

// 0x666f04 _ZN6glitch7collada19CTimelineController4initEii
void CTimelineController::init(std::int32_t startMs, std::int32_t endMs) {
    rangeStartMs = startMs;  // +0x10
    rangeEndMs = endMs;      // +0x14
}

// 0x666f10 _ZN6glitch7collada19CTimelineController6jumpToEi
float CTimelineController::jumpTo(std::int32_t timeMs) {
    currentTimeMs = timeMs;                 // +0x04
    const float seconds = msToSeconds(timeMs);
    started = false;                        // +0x3d
    currentSec = seconds;                   // +0x2c
    endReported = false;                    // +0x3c
    return seconds;
}

// 0x666f7c _ZN6glitch7collada19CTimelineController7setClipEi
float CTimelineController::setClip(std::int32_t clipIndex) {
    currentClipIndex = clipIndex;           // +0x38
    started = false;                        // +0x3d
    endReported = false;                    // +0x3c
    rangeStartMs = static_cast<std::int32_t>(getCurrentClipStart());  // vtable +0x2c
    rangeEndMs = static_cast<std::int32_t>(getCurrentClipEnd());      // vtable +0x30
    rangeStartSec = msToSeconds(rangeStartMs);
    const float length = msToSeconds(rangeEndMs - rangeStartMs);
    currentTimeMs = rangeStartMs;           // +0x04
    rangeLengthSec = length;                // +0x24
    currentSec = msToSeconds(rangeStartMs); // +0x2c
    return length;
}

// 0x667104 _ZN6glitch7collada19CTimelineController6updateEi
std::int32_t CTimelineController::update(std::int32_t timeMs) {
    const float v3 = msToSeconds(timeMs);
    float v5 = 0.0f;       // step of this update (seconds)
    float v6 = 0.0f;       // new position (seconds)
    float v8 = 0.0f;       // end reference for the wrap test (seconds)
    float v10 = 0.0f;      // wrap base (seconds)
    bool reverse = false;  // the original's early exit for a negative step
    bool wrap = false;     // the original's jump to LABEL_5

    if (started) {
        v5 = (v3 - prevTimeSec) * scale;   // v13
        v6 = v5 + currentSec;              // v14
        prevTimeSec = v3;
        currentSec = v6;
        if (v5 < 0.0f) {
            const float startSec = msToSeconds(rangeStartMs);
            v8 = startSec;
            v10 = rangeStartSec + rangeLengthSec;
            lastStep = std::fabs(v5);      // the original stores the sign-cleared bit pattern
            reverse = true;
            wrap = !(v6 >= startSec);      // goto LABEL_14 when v6 >= startSec
        }
    } else {
        v6 = currentSec;                   // v4
        started = true;
        v5 = 0.0f;
        prevTimeSec = v3;
        currentSec = v6;
    }

    if (!reverse) {
        const float endSec = msToSeconds(rangeEndMs);
        v8 = endSec;
        lastStep = v5;
        v10 = rangeStartSec;
        wrap = v6 > endSec;
    }

    if (wrap) {
        const auto callbackFn = callback;
        if (loop) {
            const float v11 = (rangeLengthSec == 0.0f) ? 0.0f : std::fmod(currentSec - v8, rangeLengthSec);
            currentSec = v10 + v11;
            if (callbackFn != nullptr) {
                callbackFn(this, callbackData);
            }
        } else {
            currentSec = v8;
            if (!endReported) {
                endReported = true;
                if (callbackFn != nullptr) {
                    callbackFn(this, callbackData);
                }
            }
        }
    }

    const std::int32_t result = static_cast<std::int32_t>(static_cast<float>(currentSec * 1000.0));
    currentTimeMs = result;  // +0x04
    return result;
}

}  // namespace glitch::collada
