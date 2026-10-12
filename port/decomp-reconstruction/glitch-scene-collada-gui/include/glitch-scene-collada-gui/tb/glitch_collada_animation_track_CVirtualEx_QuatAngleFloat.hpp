// glitch::collada::animation_track::CVirtualEx<CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionAngleMixin<float>>>,
// declared for Track B batch e2-glitch-scene-collada-gui-pack2-52827d-0. Only the two members in that batch
// (getInstance and retrieveValue) are declared here. The primary template has only the D1 destructor for this type.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-shared/glitch_core.hpp"

namespace glitch::collada::animation_track {

template <typename T>
class CSceneNodeQuaternionAngleMixin;

template <>
class CVirtualEx<CApplyValueEx<glitch::core::quaternion, CSceneNodeQuaternionAngleMixin<float>>> {
public:
    // D1 body is empty, as in the primary template.
    ~CVirtualEx() = default;

    // 0x6100dc getInstance(): function-local static of this instantiation. Partial, see the source file.
    static int* getInstance();

    // 0x61cbf4 retrieveValue(void*, void*) const. Partial: calls vtable slot +0x98 of the source object, which is
    // not identified, so the four-word copy cannot be written and the function returns nullptr.
    std::uint32_t* retrieveValue(void* source, std::uint32_t* out) const;
};

}  // namespace glitch::collada::animation_track
