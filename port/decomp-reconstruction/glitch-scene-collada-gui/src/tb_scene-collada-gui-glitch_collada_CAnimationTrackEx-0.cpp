// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CAnimationTrackEx-0.
// glitch::collada::CAnimationTrackEx::getValue (0x6e2c40), from the Hex-Rays pseudocode and the ARM listing.
// Declaration is in glitch-scene-collada-gui/collada_empty_dtors.hpp.
//
// The control flow is reconstructed. The search is the declared SAnimationAccessor::findKeyFrameNo(int, int,
// int&, float&, int) const (0x66b814, layouts/animation_track.hpp). One callee is still not mapped, so the
// body stays partial:
//   - The virtual slots at byte offsets +0x20 (slot 8) and +0x28 (slot 10) are null in this class's
//     vtable, so the target is an override in a derived class that is not mapped yet.
// Both go through the placeholder seams below, which return zero. Every other step matches the
// pseudocode: the search result and the flag decide the slot, and the key index is written back.

#include "glitch-scene-collada-gui/collada_empty_dtors.hpp"

namespace glitch::collada {

namespace {

// Placeholder for the vtable call at +0x20 (slot 8) on this, taken when a key is found and the flag is
// set. Hex-Rays passes (this, accessor, key, key + 1, fraction, out). The slot is null in this class's
// vtable, so the override is not known.
int keyedValueSlot20(const CAnimationTrackEx* self, const SAnimationAccessor& accessor, int key, int nextKey,
                     float fraction, void* out) {
    (void)self;
    (void)accessor;
    (void)key;
    (void)nextKey;
    (void)fraction;
    (void)out;
    return 0;  // placeholder: vtable +0x20 override not mapped
}

// Placeholder for the vtable call at +0x28 (slot 10) on this, taken otherwise. Hex-Rays passes
// (this, accessor, key, out). The slot is null in this class's vtable, so the override is not known.
int valueSlot28(const CAnimationTrackEx* self, const SAnimationAccessor& accessor, int key, void* out) {
    (void)self;
    (void)accessor;
    (void)key;
    (void)out;
    return 0;  // placeholder: vtable +0x28 override not mapped
}

}  // namespace

// 0x6e2c40 _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvRib
// partial: control flow matches Hex-Rays and the search is the real call to findKeyFrameNo (0x66b814). The
// two virtual slots +0x20 and +0x28 are placeholders, see the file header.
int CAnimationTrackEx::getValue(const SAnimationAccessor& accessor, int time, void* out, int& keyIndex,
                                bool flag) const {
    const int previousKey = keyIndex;  // v6 = *a5 (passed to the search as its last argument)
    int key = 0;                       // v12, set to 0 before the search
    float fraction = 0.0f;             // v11, written by the search (&v11 is the float* argument)

    // Hex-Rays: findKeyFrameNo(accessor, 0, time, &v12, &v11, v6). The result is ANDed with the flag.
    const bool found = accessor.findKeyFrameNo(0, time, key, fraction, previousKey);
    int result;
    if (found && flag) {
        // Hex-Rays: (*(this + 32))(this, accessor, key, key + 1, fraction, out)
        result = keyedValueSlot20(this, accessor, key, key + 1, fraction, out);
    } else {
        // Hex-Rays: (*(this + 40))(this, accessor, key, out)
        result = valueSlot28(this, accessor, key, out);
    }
    keyIndex = key;  // *a5 = v12
    return result;
}

}  // namespace glitch::collada
