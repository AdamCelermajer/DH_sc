// Batch g1-game-world-LightBase-85fa51-0: LightBase update and sync hooks, scene attach and detach hooks, the debug
// draw hook, constructor and destructor, property declaration, the colour and attenuation setters. Declarations come
// from dh2/LightBase.hpp. Bodies are in address order.
//
// The light data is glitch::video::CLight, reached through CLightSceneNode::light. Every body that writes that data
// is partial. glitch-video/light.hpp and glitch-scene-collada-gui/scene_nodes.hpp each define boost::intrusive_ptr
// (two stand-ins with the same name), so one translation unit cannot include both, and CLight is not complete here.
// The LightBase side of each body is reconstructed. The reasons for each partial body are in its comment and ledger row.

#include "dh2/DebugSwitches.hpp"
#include "dh2/LightBase.hpp"

namespace {

// Vec3f_Origin (ELF .bss 0x99f854), the zero Point3D<float> global. Same value as in CameraBase.
constexpr Point3D<float> kVec3fOrigin{0.0f, 0.0f, 0.0f};

}  // namespace

// 0x40a708 LightBase::RefreshAttachment()
void LightBase::RefreshAttachment() {}

// 0x40a70c LightBase::IsUpdatable() const
bool LightBase::IsUpdatable() const {
    return true;
}

// 0x40a714 LightBase::Update()
// The original calls vtable slot +0x5c (SyncData) on this. The call is virtual here too.
void LightBase::Update() {
    SyncData();
}

// 0x40a728 LightBase::TurnOn()
// partial: the original calls vtable slot +0x5c of the scene root with (root, m_irrLight). The root is the object at
// SceneManager +0x04, which is at Application +0x1c. That object has no declared type, and slot +0x5c is not identified
// (the RootSceneNode table lists setID at that slot). The call is not bound.
void LightBase::TurnOn() {}

// 0x40a768 LightBase::TurnOff()
// partial: the original calls vtable slot +0x68 of m_irrLight with the node as its only argument. The table of
// glitch::collada::CLightSceneNode lists addChild, which takes two arguments, at +0x68 and remove() at +0x74. The
// runtime class glitch::scene::CLightSceneNode has no table in the tree, so the slot is not identified. Not bound.
void LightBase::TurnOff() {}

// 0x40a784 LightBase::Draw() const
// partial: the debug switch gate is bound. With IsRenderingLightPosition on and a light node present, the original
// draws a wire box of half-size 10 around the node's absolute position, in colour 0x8500ffff (bytes ff ff 00 85).
// ISceneNode::getAbsolutePosition is not declared on the CLightSceneNode in scene_nodes.hpp, and Draw3DWireFrameBox
// (core-platform's global) has no declaration in the tree. Neither is bound.
void LightBase::Draw() const {
    if (DebugSwitches::GetInstance()->GetSwitch("IsRenderingLightPosition") && m_irrLight != nullptr) {
        // partial: the wire box, see above.
    }
}

// 0x40a8f8 LightBase::SyncData()
// partial: the gate is bound. When m_dae is empty or the TestMultiPlayerLight switch is on, the original takes a
// reference on the node's CLight, writes radius (CLight +0x40, only when radius is above 0.01), attenuation (+0x34,
// +0x38, +0x3c), ambient (+0x04..+0x0c, alpha 1.0 at +0x10), diffuse (+0x14..+0x20) and specular (+0x24..+0x30),
// and drops the reference. The CLight writes are not bound (see the file comment).
void LightBase::SyncData() {
    if (m_dae.empty() || DebugSwitches::GetInstance()->GetSwitch("TestMultiPlayerLight")) {
        // partial: the light data writes, see above.
    }
}

// 0x40aaa0 LightBase::~LightBase()   (D1, complete object)
// 0x40ab34 LightBase::~LightBase()   (D2, base object: same body, compiler-generated)
// 0x40ab18 LightBase::~LightBase()   (D0, deleting destructor: compiler-generated for delete, not defined here)
// partial: the original drops m_irrLight through the light node's IReferenceCounted base. That base is not declared
// by scene_nodes.hpp, so the drop is not bound. The pointer is cleared. The member destructors and ~ObjectBase run as
// in C++.
LightBase::~LightBase() {
    m_irrLight = nullptr;
}

// 0x40aba4 LightBase::LightBase(ObjectBase::GO_IDS)   (C1, complete object)
// 0x40ac58 LightBase::LightBase(ObjectBase::GO_IDS)   (C2, base object: same body, compiler-generated)
LightBase::LightBase(GO_IDS id) : ObjectBase(id) {
    m_irrLight = nullptr;                // +0x120
    m_position = Point3D<float>{};       // +0x124
    // +0x130 (radius) and +0x164 (automatic) are not written by the original constructor, so they are not set here.
    m_attenuation = Point3D<float>{};    // +0x134
    m_ambientColor = Point3D<float>{};   // +0x140
    m_diffuseColor = Point3D<float>{};   // +0x14c
    m_specularColor = Point3D<float>{};  // +0x158
    unk_85 = 1;                          // ObjectBase +0x85, set by LightBase on construction
}

// 0x40adf0 LightBase::DeclareProperties()
// Properties in the original order: position, radius (default 2.0), attenuation, ambientColor, diffuseColor,
// specularColor, automatic (default false), dae (default empty).
void LightBase::DeclareProperties() {
    AddProperty<Point3D<float>>("position", m_position, kVec3fOrigin);
    AddProperty<float>("radius", m_radius, 2.0f);
    AddProperty<Point3D<float>>("attenuation", m_attenuation, Point3D<float>{});
    AddProperty<Point3D<float>>("ambientColor", m_ambientColor, Point3D<float>{});
    AddProperty<Point3D<float>>("diffuseColor", m_diffuseColor, Point3D<float>{});
    AddProperty<Point3D<float>>("specularColor", m_specularColor, Point3D<float>{});
    AddProperty<bool>("automatic", m_automatic, false);
    AddProperty<std::string>("dae", m_dae);
}

// 0x40b0c0 LightBase::InitPost()
// partial: the colour and attenuation scaling and the final SyncData() call are bound. The original also creates or
// loads the light node, which is not bound:
//   - a non-empty m_dae is loaded through SceneManager::LoadScene. The original passes the word at +0x17c (the string's
//     _M_finish, the end pointer, not the buffer) as the file name. Whether that is the original's behaviour or a
//     mistyped decompile is not settled, so the load is not bound.
//   - an empty m_dae creates a glitch::scene::CLightSceneNode(true) (operator new(348)). scene_nodes.hpp declares no
//     such constructor.
//   - the node is passed to the scene root (vtable +0x5c of the root, see TurnOn), and its IReferenceCounted base is
//     adjusted. Neither is bound.
//   - when m_automatic is set, this is appended to the automatic-light vector inside the scene manager's
//     LightSetManager (+0x424 of SceneManager). That vector has no declared type. Not bound.
void LightBase::InitPost() {
    // Point3D<float>::operator/= (0x34d04c) divides each component. The game_types.hpp Point3D used here has no
    // operator/=, so the three colours are divided component by component.
    for (Point3D<float>* color : {&m_ambientColor, &m_diffuseColor, &m_specularColor}) {
        color->X /= 255.0f;
        color->Y /= 255.0f;
        color->Z /= 255.0f;
    }
    m_attenuation.Y = static_cast<float>(m_attenuation.Y / 1000.0);
    // partial: the scene load, node creation and registration, see above.
    m_attenuation.Z = static_cast<float>(m_attenuation.Z / 1000000.0);
    SyncData();
}

// 0x40b3c8 LightBase::SetAttenuation(Point3D<float>)
// partial: the member is set (x as given, y divided by 1e3, z divided by 1e6). The original also writes the attenuation
// into the light data of m_irrLight (CLight +0x34, +0x38, +0x3c) under a reference on the light. Not bound.
void LightBase::SetAttenuation(Point3D<float> attenuation) {
    m_attenuation.X = attenuation.X;
    m_attenuation.Y = static_cast<float>(attenuation.Y / 1000.0);
    m_attenuation.Z = static_cast<float>(attenuation.Z / 1000000.0);
}

// 0x40b498 LightBase::setLightNode(glitch::scene::CLightSceneNode*)
// partial: m_irrLight is set to node. The original also drops the previous node and grabs the new one through the
// light node's IReferenceCounted base, which is not declared. It then copies the light data back into the members
// (attenuation x, y and z as stored, ambient, diffuse, specular and radius). That copy needs CLight, which is not
// complete here.
void LightBase::setLightNode(glitch::scene::CLightSceneNode* node) {
    m_irrLight = node;
}

// 0x40b5c8 LightBase::SetSpecularColor(Point3D<float>)
// partial: m_specularColor is set. The original also writes the colour into the light data (CLight +0x24..+0x30, alpha
// 1.0 at +0x30) under a reference on the light. Not bound.
void LightBase::SetSpecularColor(Point3D<float> color) {
    m_specularColor = color;
}

// 0x40b674 LightBase::SetDiffuseColor(Point3D<float>)
// partial: m_diffuseColor is set. The original also writes the colour into the light data (CLight +0x14..+0x20, alpha
// 1.0 at +0x20) under a reference on the light. Not bound.
void LightBase::SetDiffuseColor(Point3D<float> color) {
    m_diffuseColor = color;
}

// 0x40b720 LightBase::SetAmbientColor(Point3D<float>)
// partial: m_ambientColor is set. The original also writes the colour into the light data (CLight +0x04..+0x0c as float
// bits, alpha 1.0 at +0x10) under a reference on the light. CLight declares those words as uint32_t, so the light side
// needs a layout fix first. Not bound.
void LightBase::SetAmbientColor(Point3D<float> color) {
    m_ambientColor = color;
}
