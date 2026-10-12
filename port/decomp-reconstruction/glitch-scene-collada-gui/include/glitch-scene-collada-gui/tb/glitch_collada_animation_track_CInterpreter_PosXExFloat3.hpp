// glitch::collada::animation_track::CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>,
// declared for Track B batch e2-glitch-scene-collada-gui-pack2-52827d-0.
//
// No header in this category declares this instantiation. The class is stateless: each method takes the
// SAnimationAccessor as its first argument and has no this pointer, so the methods are static.
#pragma once

#include <cstdint>

namespace glitch::collada {
struct SAnimationAccessor;  // full layout: glitch-scene-collada-gui/layouts/animation_track.hpp
}  // namespace glitch::collada

namespace glitch::collada::animation_track {

template <typename T>
struct CSceneNodePositionXEx;

template <int Index, typename Value>
struct SUseDefaultValues;

template <typename Mixin, typename Key, int Dim, typename Default>
class CInterpreter;

template <>
class CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>> {
public:
    // 0x61f9a4 getKeyBasedValueEx(SAnimationAccessor const&, int, void*) (static). Writes the x key and, when a
    // default value exists, its y and z words. Returns the default value pointer, or nullptr.
    static const std::uint32_t* getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, float* a3);

    // 0x61fa24 getKeyBasedValueEx(SAnimationAccessor const&, int, int, float, void*) (static). Blends the x key
    // and returns the blended value.
    static float getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, float a4,
                                    float* a5);

    // 0x61faf8 getKeyBasedValueEx(SAnimationAccessor const&, int, int, void*) (static). Writes the x difference of
    // two keys and returns the default value pointer, or nullptr.
    static const std::uint32_t* getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3,
                                                   float* a4);

    // 0x61fb78 getKeyBasedValueEx(SAnimationAccessor const&, int, int, int, float, void*) (static). Blends three
    // keys (a2 base, a3, a4) and returns the blended x value.
    static float getKeyBasedValueEx(const glitch::collada::SAnimationAccessor& a1, int a2, int a3, int a4, float a5,
                                    float* a6);
};

}  // namespace glitch::collada::animation_track
