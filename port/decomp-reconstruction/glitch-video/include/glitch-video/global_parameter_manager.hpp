// glitch::video::CGlobalMaterialParameterManager: the driver's table of global shader parameters (DynamicLight<n>,
// FogColor<n>, SceneAmbientLight, ...). Canonical definition (engine unification wave u1-video); the forward
// declarations elsewhere are replaced by this header.
//
// The original derives from IMaterialParameters<CGlobalMaterialParameterManager, SEmptyBase>, whose typed accessors the port
// keeps in detail::SGlobalParameterOwner (material_parameters_bind_g.hpp): that struct owns the entry list and the value
// buffer. The manager is therefore SGlobalParameterOwner plus the driver link and the packing state below.
//
// Layout of the original object (32-bit, 0x3c bytes; constructors 0x5b9e50 C2 / 0x5b9e8c C1 with an IVideoDriver*,
// IVideoDriver::IVideoDriver creates one when none is passed):
//   +0x00..+0x27  SIDedCollection<SShaderParameterDef, ushort, ...>: entries (20 bytes) at +0x18 .. +0x1c, count at +0x26
//   +0x28  IVideoDriver*        +0x2c  value buffer start   +0x30  value buffer end   +0x34  value buffer capacity
//   +0x38  uint8 flags (auto-packing and the packed marker, setAutoPacking 0x5ba8a0, pack 0x5ba820)
// Methods of the queue: grabInternal / dropInternal (0x5b9d2c / 0x5b9d4c, count a parameter's users), addParameter
// (0x5bc374), pack, setAutoPacking, clearParameters, remove / removeAll / removeUnused, (de)serializeAttributes, and the
// destructors 0x5bb560 / 0x5bb588.
#pragma once

#include <cstdint>

#include "glitch-video/material_parameters_bind_g.hpp"
#include "glitch-video/video_types.hpp"

namespace glitch::video {

class CGlobalMaterialParameterManager : public detail::SGlobalParameterOwner {
public:
    // 0x5b9e50 (C2) / 0x5b9e8c (C1). Only the driver link is kept.
    explicit CGlobalMaterialParameterManager(IVideoDriver* driver = nullptr) : m_driver(driver) {}

    // 0x5bb560 (D1) / 0x5bb588 (D2): clears the parameter table and its value buffer; the base releases itself.
    ~CGlobalMaterialParameterManager() = default;

    // 0x5b9d2c / 0x5b9d4c: add or remove a user of parameter `id`. Declared only (the users are counted in the
    // SIDedCollection entry, which the port keeps as part1a::SParamEntry without a user count).
    void grabInternal(uint16_t id);
    void dropInternal(uint16_t id);
    // 0x5bc374: adds a parameter with a type, a value type, a count and an element size, and returns its id. Declared only.
    uint16_t addParameter(const char* name, E_SHADER_PARAMETER_TYPE type, E_SHADER_PARAMETER_VALUE_TYPE valueType,
                          uint32_t count, uint8_t size);
    // 0x5ba820 / 0x5ba8a0: packing of the value buffer. Declared only.
    void pack();
    void setAutoPacking(bool enable);
    // 0x5c0f60 / 0x5c11dc / 0x5c1220 / 0x5c126c: clear, remove the unused, remove all, remove one. Declared only.
    void clearParameters();
    void removeUnused();
    void removeAll();
    void remove(uint16_t id);

    IVideoDriver* m_driver = nullptr;  // +0x28
};

}  // namespace glitch::video
