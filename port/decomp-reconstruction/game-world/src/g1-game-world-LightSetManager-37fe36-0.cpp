// Batch g1-game-world-LightSetManager-37fe36-0: the LightSetManager class (constructors, destructors, copy assignment,
// light set lookup, light and dummy light slots, the static and active light lists, the filter and the material settings,
// and the per-frame update). Declarations come from dh2/LightSetManager.hpp.
//
// The light set names are the strings the constructor copies from the binary's tables: light_set_names[0] at 0x8c24e8,
// off_999780[0] at 0x8c3e40, off_999784[0] at 0x8c7d80 and off_999788[0] at 0x8c3e30. Their contents are "PlayerLight",
// "SceneLight", "CameraLight" and "MonsterLight".
//
// The deleting destructors (D0) of StaticLight and ActiveLight are not defined here; the compiler generates them from the
// virtual destructors. Their addresses are listed next to the destructors below.

#include "dh2/LightSetManager.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "glitch-video/light.hpp"
#include "glitch-video/material.hpp"
#include "glitch-video/material_renderer.hpp"

// Declared in core-platform/memory.hpp. That header is not included here: it pulls in core-platform's stand-in
// glitch_core.hpp, which clashes with the canonical glitch-shared header that glitch-video/light.hpp needs.
extern int gAssertLevel;

namespace {

// The source file name the original passes to its assert macro.
constexpr const char* kLightSetManagerSource =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Lights\\LightSetManager.cpp";

// The original ASSERT: gAssertLevel 2 writes to address 0 (a crash, here abort), gAssertLevel 1 prints the message to
// stderr and execution continues.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kLightSetManagerSource, line);
    }
}

// The float with the given bit pattern. The constructor stores 0xc9742400 (int -915135488) into three floats.
float FloatFromBits(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

}  // namespace

// 0x3541d4 LightSetManager::operator=(LightSetManager const&)
// Member-wise copy in the original's order. A StaticLight or ActiveLight copies its node (and, for ActiveLight, its
// ObjectHandle) and keeps its own vtable pointer, so only the data members are assigned here.
// unk_00 is copied although the constructor never writes it, as in the original.
LightSetManager& LightSetManager::operator=(const LightSetManager& other) {
    unk_00 = other.unk_00;                                                          // +0x00
    for (int i = 0; i < 4; ++i) {
        m_lightSetNames[i] = other.m_lightSetNames[i];                              // +0x04 .. +0x63
    }
    for (int set = 0; set < 4; ++set) {
        for (int light = 0; light < 5; ++light) {
            m_lightChanged[set][light] = other.m_lightChanged[set][light];          // +0x64 .. +0x77
        }
    }
    for (int set = 0; set < 4; ++set) {
        for (int light = 0; light < 5; ++light) {
            m_lights[set][light] = other.m_lights[set][light];                      // +0x78 .. +0xc7
        }
    }
    for (int i = 0; i < 5; ++i) {
        m_dummyLightOff[i] = other.m_dummyLightOff[i];                              // +0xc8 .. +0xdb
    }
    for (int i = 0; i < 8; ++i) {
        m_staticLights[i].m_node = other.m_staticLights[i].m_node;                  // +0xdc .. +0x11b (+4 of each)
    }
    for (int i = 0; i < 4; ++i) {
        m_activeLights[i].m_node = other.m_activeLights[i].m_node;                  // +0x11c .. +0x16b (+4 of each)
        m_activeLights[i].m_object = other.m_activeLights[i].m_object;              // (+8, 12 bytes of each)
    }
    for (int i = 0; i < 3; ++i) {
        unk_16c[i] = other.unk_16c[i];                                              // +0x16c .. +0x177
    }
    for (int i = 0; i < 4; ++i) {
        unk_178[i] = other.unk_178[i];                                              // +0x178 .. +0x187
    }
    m_countStaticLight = other.m_countStaticLight;                                  // +0x188
    m_countActiveLight = other.m_countActiveLight;                                  // +0x18c
    unk_190 = other.unk_190;                                                        // +0x190 .. +0x19b
    unk_19c = other.unk_19c;                                                        // +0x19c
    unk_19d = other.unk_19d;                                                        // +0x19d
    unk_19e = other.unk_19e;                                                        // +0x19e
    unk_1a0 = other.unk_1a0;                                                        // +0x1a0
    return *this;
}

// 0x40c178 LightSetManager::StaticLight::~StaticLight()   (D1, complete object: empty)
LightSetManager::StaticLight::~StaticLight() {}

// 0x40c17c LightSetManager::ActiveLight::~ActiveLight()   (D1, complete object: empty)
LightSetManager::ActiveLight::~ActiveLight() {}

// 0x40c364 LightSetManager::StaticLight::~StaticLight()   (D0, deleting destructor)
// 0x40c398 LightSetManager::ActiveLight::~ActiveLight()   (D0, deleting destructor)
// Not defined here. The compiler generates the deleting destructors from the virtual destructors above. The original
// resets the vtable pointer and calls CustomFree on the object.

// 0x40c3cc LightSetManager::GetLightSetIdFromName(std::string)
// Index of the first light set whose name equals the argument, or 0 when none matches.
int LightSetManager::GetLightSetIdFromName(std::string name) {
    for (int i = 0; i < 4; ++i) {
        if (std::strcmp(name.c_str(), m_lightSetNames[i].c_str()) == 0) {
            return i;
        }
    }
    return 0;
}

// 0x40c40c LightSetManager::GetLight(int, int)
// Returns the slot m_lights[lightSetId][lightId]. Out-of-range indices only report the assert, as in the original.
boost::intrusive_ptr<glitch::video::CLight>* LightSetManager::GetLight(int lightSetId, int lightId) {
    if (static_cast<unsigned int>(lightSetId) > 3) {
        ReportAssert("lightsetId >= 0 && lightsetId < MAX_NUM_LIGHTSET", 116);
    }
    if (static_cast<unsigned int>(lightId) > 4) {
        ReportAssert("lightId >= 0 && lightId < MAX_NUM_LIGHT_PER_LIGHTSET", 117);
    }
    return &m_lights[lightSetId][lightId];
}

// 0x40c524 LightSetManager::AddStaticLight(glitch::scene::CLightSceneNode*)
void LightSetManager::AddStaticLight(glitch::scene::CLightSceneNode* node) {
    if (m_countStaticLight > 6) {
        ReportAssert("m_countStaticLight < MAX_NUM_STATIC_LIGHT - 1", 107);
    }
    m_staticLights[m_countStaticLight].m_node = node;
    ++m_countStaticLight;
}

// 0x40c5d8 LightSetManager::AddActiveLight(ObjectHandle, glitch::scene::CLightSceneNode*)
void LightSetManager::AddActiveLight(ObjectHandle object, glitch::scene::CLightSceneNode* node) {
    if (m_countActiveLight > 2) {
        ReportAssert("m_countActiveLight < MAX_NUM_ACTIVE_LIGHT - 1", 97);
    }
    ActiveLight& light = m_activeLights[m_countActiveLight];
    light.m_node = node;
    light.m_object = object;
    ++m_countActiveLight;
}

// 0x40c6b0 LightSetManager::GetClosestStaticLight(Point3D<float>)
// partial: the no-light path is bound: the assert "HasStaticLight()" (line 68) and the return of the address
// &m_staticLights[-1] (a1 + 212) that the original gives when no static light is in use. The search over the static
// lights is not bound. It compares the squared-root distance from `position` to each static light's node position,
// which is ISceneNode::getPosition (vtable +0xa0). glitch::scene::CLightSceneNode in the tree has no getPosition, so
// the search returns null here. Layout reference: the search reads m_node at StaticLight +0x04.
LightSetManager::StaticLight* LightSetManager::GetClosestStaticLight(Point3D<float> position) {
    if (m_countStaticLight <= 0) {
        ReportAssert("HasStaticLight()", 68);
        return m_staticLights - 1;
    }
    (void)position;
    // partial: the closest-node search, see the comment above.
    return nullptr;
}

// 0x40c9a8 LightSetManager::ApplySettings(int, boost::intrusive_ptr<glitch::video::CMaterial>&, std::vector<bool>&)
// partial: the count and the names are bound. For each light slot the original names the material parameter "light<n>"
// with n the number of parameters bound so far, looks it up with CMaterialRenderer::getParameterID, and binds the light
// when the parameter exists and the light is set. The slots not in the filter are bound to the dummy light off. The
// binding itself (IMaterialParameters::setParameter<boost::intrusive_ptr<CLight>>, index 0) is not bound: the typed index
// form is not declared on CMaterial in the tree (the parameter mixin is not integrated). The count is kept, so later
// names are right, but the material receives no light values.
void LightSetManager::ApplySettings(int lightSetId, boost::intrusive_ptr<glitch::video::CMaterial>& material,
                                    std::vector<bool>& filter) {
    int bound = 0;
    for (int i = 0; i < 4; ++i) {
        if (!filter[i]) {
            continue;
        }
        boost::intrusive_ptr<glitch::video::CLight>* light = GetLight(lightSetId, i);
        std::string name("light");
        name += static_cast<char>('0' + bound);
        const std::uint16_t parameterId = material->m_renderer->getParameterID(name.c_str(), 0);
        if (parameterId == 0xFFFF || !*light) {
            continue;
        }
        // partial: setParameter<boost::intrusive_ptr<CLight>>(material, parameterId, 0, *light). See above.
        ++bound;
    }
    for (int i = 0; i < 4; ++i) {
        if (filter[i]) {
            continue;
        }
        std::string name("light");
        name += static_cast<char>('0' + bound);
        const std::uint16_t parameterId = material->m_renderer->getParameterID(name.c_str(), 0);
        if (parameterId == 0xFFFF) {
            continue;
        }
        // partial: setParameter<boost::intrusive_ptr<CLight>>(material, parameterId, 0, m_dummyLightOff[i]). See above.
        ++bound;
    }
}

// 0x40cbc0 LightSetManager::ResetLightSets(void)
// Releases every light slot (light index outer, light set inner, as in the original) and every dummy light, and zeroes
// unk_1a0. Light sets and static and active lights are not touched.
void LightSetManager::ResetLightSets() {
    for (int light = 0; light < 5; ++light) {
        for (int set = 0; set < 4; ++set) {
            m_lights[set][light] = nullptr;
        }
    }
    for (int i = 0; i < 5; ++i) {
        m_dummyLightOff[i] = nullptr;
    }
    unk_1a0 = 0;
}

// 0x40ccd0 LightSetManager::~LightSetManager()   (D1, complete object)
// 0x40ce60 LightSetManager::~LightSetManager()   (D2, base object: same body)
// The original's body is the destruction of the members in reverse declaration order, which is what the implicit member
// destructors do here: the light vector buffer, the active lights (each D1), the static lights (each D1), the dummy
// lights, the light slots and the names. The original frees the vector buffer with GlitchFree, and the std::vector here
// frees it itself.
LightSetManager::~LightSetManager() {}

// 0x40cff0 LightSetManager::InitLightFilter(std::vector<bool>&, bool)
// The original's bit-vector code is an erase (when more than 5 bits are in use), a grow (when fewer), or nothing (when
// exactly 5). Each case leaves the filter at 5 entries. Then entries 0..4 are set to value. The result is resize(5) and
// the five entries set.
void LightSetManager::InitLightFilter(std::vector<bool>& filter, bool value) {
    filter.resize(5);
    for (int i = 0; i < 5; ++i) {
        filter[i] = value;
    }
}

// 0x40d368 LightSetManager::SetLight(int, int, boost::intrusive_ptr<glitch::video::CLight> const&)
// The changed flag is set when the stored light is a different object from the new one. Then the slot takes the new
// light, adding a use to it and dropping the old one.
void LightSetManager::SetLight(int lightSetId, int lightId, const boost::intrusive_ptr<glitch::video::CLight>& light) {
    if (static_cast<unsigned int>(lightSetId) > 3) {
        ReportAssert("lightsetId >= 0 && lightsetId < MAX_NUM_LIGHTSET", 123);
    }
    if (static_cast<unsigned int>(lightId) > 4) {
        ReportAssert("lightId >= 0 && lightId < MAX_NUM_LIGHT_PER_LIGHTSET", 124);
    }
    boost::intrusive_ptr<glitch::video::CLight>& slot = m_lights[lightSetId][lightId];
    if (slot.get() != light.get()) {
        m_lightChanged[lightSetId][lightId] = true;
    }
    slot = light;
}

// 0x40d520 LightSetManager::SetDummyLightOff(int, boost::intrusive_ptr<glitch::video::CLight> const&)
void LightSetManager::SetDummyLightOff(int index, const boost::intrusive_ptr<glitch::video::CLight>& light) {
    m_dummyLightOff[index] = light;
}

// 0x40d5a0 LightSetManager::Update(void)
// partial: the gates are bound. Nothing happens unless there is at least one static light and one active light. The
// debug switch "EnablePlayerHeadLight" is read (the original reads it after DebugSwitches::load and treats a switch that
// is off as the enabling case). For each active light the object is resolved, its position is read (GameObject
// +0x160), and GetClosestStaticLight is called with it. The original then moves the active light's node to the closest
// static light's node position plus 20 on each axis (vtable +0xa0 getPosition, +0xa4 setPosition). Neither is declared
// on glitch::scene::CLightSceneNode in the tree, so the move is not bound.
void LightSetManager::Update() {
    if (m_countStaticLight <= 0 || m_countActiveLight <= 0) {
        return;
    }
    DebugSwitches::s_inst.load();
    const std::string switchName("EnablePlayerHeadLight");
    const bool proceed = !DebugSwitches::s_inst.GetSwitch(switchName);
    if (!proceed) {
        return;
    }
    for (int i = 0; i < m_countActiveLight; ++i) {
        ObjectHandle handle = m_activeLights[i].m_object;
        GameObject* object = handle.operator GameObject*();
        if (object == nullptr) {
            continue;
        }
        StaticLight* closest = GetClosestStaticLight(object->m_position);
        (void)closest;
        // partial: the node move (ISceneNode getPosition and setPosition on the nodes, see above).
    }
}

// 0x40d7d4 LightSetManager::LightSetManager()   (C1, complete object)
// 0x40daa8 LightSetManager::LightSetManager()   (C2, base object: same body)
// The original writes the four names from the binary's tables, clears the light slots, the dummy lights, the static and
// active nodes (and their object handles), the three floats (bit pattern 0xc9742400, which is -1000000.0f), unk_178, the counts
// and the vector, and sets unk_19c and unk_19d to 1 and unk_19e and unk_1a0 to 0. unk_00 is not written.
LightSetManager::LightSetManager() {
    m_lightSetNames[0] = "PlayerLight";  // light_set_names[0], 0x8c24e8
    m_lightSetNames[1] = "SceneLight";   // off_999780[0], 0x8c3e40
    m_lightSetNames[2] = "CameraLight";  // off_999784[0], 0x8c7d80
    m_lightSetNames[3] = "MonsterLight"; // off_999788[0], 0x8c3e30

    for (int light = 0; light < 5; ++light) {
        for (int set = 0; set < 4; ++set) {
            m_lights[set][light] = nullptr;
            m_lightChanged[set][light] = false;
        }
    }
    for (int i = 0; i < 5; ++i) {
        m_dummyLightOff[i] = nullptr;
    }
    for (int i = 0; i < 8; ++i) {
        m_staticLights[i].m_node = nullptr;
    }
    for (int i = 0; i < 4; ++i) {
        m_activeLights[i].m_node = nullptr;
        m_activeLights[i].m_object = ObjectHandle();
    }
    unk_16c[0] = FloatFromBits(0xc9742400u);
    unk_16c[1] = FloatFromBits(0xc9742400u);
    unk_16c[2] = FloatFromBits(0xc9742400u);
    for (int i = 0; i < 4; ++i) {
        unk_178[i] = 0;
    }
    m_countStaticLight = 0;
    m_countActiveLight = 0;
    unk_19c = true;
    unk_19d = true;
    unk_19e = false;
    unk_1a0 = 0;
}
