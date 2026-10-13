// Batch g1-game-world-LightBase-85fa51-0 (redo pass r2a-game-world-LightBase-85fa51-0 completed the bodies in place):
// LightBase update and sync hooks, scene attach and detach hooks, the debug draw hook, constructor and destructor,
// property declaration, the colour and attenuation setters. Declarations come from dh2/LightBase.hpp. Bodies are in
// address order.
//
// The light data is glitch::video::CLight, reached through CLightSceneNode::light (the node's light is at +0x134).
// CLight is declared canonically in glitch-video/light.hpp (float colours, attenuation, count at +0x00), but that header
// cannot share a translation unit with scene_nodes.hpp or any scene/ header: glitch-core-io's deps/glitch_core_objects.hpp
// still defines a stub glitch::video::CLight (`class CLight : public glitch::IReferenceCounted {};`), and io/interfaces.hpp
// pulls it in through ISceneNode.hpp. Until the stub is removed (glitch-core-io owner), the CLight writes and the copy
// back from the light stay unbound, and every body that needs them is partial. The LightBase side of each body is
// reconstructed. The reasons for each partial body are in its comment and ledger row.

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/LightBase.hpp"
#include "dh2/SceneManager.hpp"

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
// partial: the original calls vtable slot +0x5c (ISceneNode::addChild, Hex-Rays call offset) of the scene root with
// (root, m_irrLight). The root is SceneManager::rootNode (CSceneManager +0x04, typed ISceneNode*) of the scene manager at
// Application +0x1c. The argument is a CLightSceneNode*, and scene_nodes.hpp does not derive CLightSceneNode from
// ISceneNode, so the call is not bound. The original's return value (the callee's register) is not returned here.
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
// partial: the original drops m_irrLight through the light node's IReferenceCounted virtual base (the base offset is
// read from the node's vtable at -12, and the count is at +4 of the base). CLightSceneNode in scene_nodes.hpp has no
// IReferenceCounted base, so the drop is not bound. The pointer is cleared. The member destructors and ~ObjectBase run
// as in C++.
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
// partial. Bound: the colour divisions by 255 (ambient, diffuse, specular, component by component), attenuation y by
// 1e3 and z by 1e6 (single-precision divisions, __aeabi_fdiv in the binary), the scene load for a non-empty m_dae
// (SceneManager::LoadScene(file, "", false, false) on the scene manager at Application +0x1c, the file name being the
// string's character pointer at +0x17c), and the final SyncData() call (a virtual call on this, vtable +0x5c).
// Not bound, so m_irrLight is not set by this body:
//   - m_irrLight from the loaded root: the first element of the root's lights list (CRootSceneNode +0x188, an
//     SIntrusiveList<void>). The loaded root is an ISceneNode, and CRootSceneNode is not derived from ISceneNode in the
//     tree, so the list cannot be reached. The original also releases the loaded root's reference after this step; the
//     release is not bound, so the reference is kept here (the loaded root is not dropped).
//   - the root call rootNode->addChild(m_irrLight) (rootNode at CSceneManager +0x04, vtable +0x5c, ISceneNode::addChild).
//     The light is a CLightSceneNode, which scene_nodes.hpp does not derive from ISceneNode.
//   - when m_irrLight is still null: operator new(0x15c), glitch::scene::CLightSceneNode(true) (no such constructor in
//     scene_nodes.hpp), the addChild on the root, and the release of the creation reference.
//   - the counted reference the original takes on m_irrLight (the count of its IReferenceCounted base).
//   - when m_automatic (+0x164) is set, this is pushed onto LightSetManager::unk_190 (a LightBase* vector at
//     LightSetManager +0x190, SceneManager +0x424). dh2/SceneManager.hpp keeps LightSetManager as a byte array.
void LightBase::InitPost() {
    // Point3D<float>::operator/= (0x34d04c) divides each component. The three colours are divided component by component.
    for (Point3D<float>* color : {&m_ambientColor, &m_diffuseColor, &m_specularColor}) {
        color->X /= 255.0f;
        color->Y /= 255.0f;
        color->Z /= 255.0f;
    }
    m_attenuation.Y /= 1000.0f;     // +0x138
    m_attenuation.Z /= 1000000.0f;  // +0x13c

    if (!m_dae.empty()) {
        // partial: the loaded root is returned here and never released (see above). m_irrLight is not taken from it.
        ApplicationGetInstance().unk_1c->LoadScene(m_dae.c_str(), "", false, false);
    }
    // partial: the node creation when m_irrLight is null, the counted reference on m_irrLight, and the automatic-light
    // registration. See above.
    SyncData();
}

// 0x40b3c8 LightBase::SetAttenuation(Point3D<float>)
// partial: the member is set (x as given, y divided by 1e3, z divided by 1e6, single precision). The original also writes
// the attenuation into the light data of m_irrLight (CLight +0x34, +0x38, +0x3c) under a counted reference on the light.
// Not bound: CLight cannot be included here (see the file comment).
void LightBase::SetAttenuation(Point3D<float> attenuation) {
    m_attenuation.X = attenuation.X;
    m_attenuation.Y = attenuation.Y / 1000.0f;
    m_attenuation.Z = attenuation.Z / 1000000.0f;
}

// 0x40b498 LightBase::setLightNode(glitch::scene::CLightSceneNode*)
// partial: m_irrLight is set to node. The original also drops the previous node and grabs the new one through the
// light node's IReferenceCounted base, which is not declared. It then copies the light data back into the members
// (attenuation as stored, ambient, diffuse, specular and radius, from CLight +0x34, +0x04..+0x0c, +0x14..+0x1c,
// +0x24..+0x2c and +0x40). That copy needs CLight, which cannot be included here.
void LightBase::setLightNode(glitch::scene::CLightSceneNode* node) {
    m_irrLight = node;
}

// 0x40b5c8 LightBase::SetSpecularColor(Point3D<float>)
// partial: m_specularColor is set. The original also writes the colour into the light data (CLight +0x24..+0x2c, alpha
// 1.0 at +0x30) under a counted reference on the light. Not bound (see the file comment).
void LightBase::SetSpecularColor(Point3D<float> color) {
    m_specularColor = color;
}

// 0x40b674 LightBase::SetDiffuseColor(Point3D<float>)
// partial: m_diffuseColor is set. The original also writes the colour into the light data (CLight +0x14..+0x1c, alpha
// 1.0 at +0x20) under a counted reference on the light. Not bound (see the file comment).
void LightBase::SetDiffuseColor(Point3D<float> color) {
    m_diffuseColor = color;
}

// 0x40b720 LightBase::SetAmbientColor(Point3D<float>)
// partial: m_ambientColor is set. The original also writes the colour into the light data (CLight +0x04..+0x0c, alpha
// 1.0 at +0x10) under a counted reference on the light. Not bound (see the file comment).
void LightBase::SetAmbientColor(Point3D<float> color) {
    m_ambientColor = color;
}
