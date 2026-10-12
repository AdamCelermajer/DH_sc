// glitch::collada::animation_track::CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float> >
//
// Declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CInterpreter_glit-0
// (the owner class of this batch, selected by its owner field; the batch id is shared by 165 batches).
// The class is not declared anywhere else in glitch-scene-collada-gui, so this header holds only the
// explicit specialization that the batch needs. The primary template has the same declaration as the
// other CInterpreter headers in this category (Mixin, Key, int Dim, Default).
//
// Offsets are the original 32-bit ARM offsets, kept as comments only.
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::collada {

struct SAnimationAccessor;  // full layout in layouts/animation_track.hpp; not included here

namespace animation_track {

template <typename T>
class CSceneNodeScaleXEx;

template <int Index, typename T>
struct SUseDefaultValues;

// Primary template: layout and other specializations are declared by their own batches.
template <typename Mixin, typename Key, int Dim, typename Default>
class CInterpreter;

template <>
class CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>> {
public:
    // Static: the symbols take the accessor as the first argument and have no this pointer.
    // 0x61dc4c ...getKeyBasedValueEx(SAnimationAccessor const&, int, void*): one key, copied.
    // Returns the default-value pointer (partial, see the .cpp).
    static int getKeyBasedValueEx(const SAnimationAccessor& a1, int key, void* out);

    // 0x61dccc ...getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*): linear blend.
    // Returns the blended X component reinterpreted from the default pointer bits (partial, see the .cpp).
    static float getKeyBasedValueEx(const SAnimationAccessor& a1, int from, int to, float t, void* out);

    // 0x61dda0 ...getKeyBasedValueEx(SAnimationAccessor const&, int, int, void*): key difference.
    // Returns the default-value pointer (partial, see the .cpp).
    static int getKeyBasedValueEx(const SAnimationAccessor& a1, int from, int to, void* out);

    // 0x61de20 ...getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*): blend of two
    // key deltas measured from the base key.
    static float getKeyBasedValueEx(const SAnimationAccessor& a1, int base, int from, int to, float t,
                                    void* out);
};

}  // namespace animation_track

namespace tb_ext {

// SAnimationAccessor members used by the bodies above, defined outside this batch (accessor layout not
// mapped here). The key data pointer is the word at +0x04 of SAnimationAccessor::getOutput(0), as read
// by the pseudocode. The default-value pointer is the result of getDefaultValue().
const void* accessorKeyData(const SAnimationAccessor& accessor);
bool accessorHasDefaultValue(const SAnimationAccessor& accessor);
const void* accessorDefaultValue(const SAnimationAccessor& accessor);

}  // namespace tb_ext

}  // namespace glitch::collada
