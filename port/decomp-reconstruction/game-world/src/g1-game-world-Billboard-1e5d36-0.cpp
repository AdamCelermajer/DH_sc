// Batch g1-game-world-Billboard-1e5d36-0: the eleven Billboard functions. Declarations come from dh2/Billboard.hpp.
//
// Layout used here (original 32-bit offsets, kept in Billboard.hpp): +0x85 ObjectBase::unk_85 (updating flag),
// +0x120 m_tga, +0x138 m_attachTo, +0x150 m_size, +0x158 m_position, +0x164 m_isAdditive, +0x168 m_attachHandle,
// +0x174 m_attachToActiveCamera, +0x178 m_billboardNode.
//
// Partial: Update, InitPost, the destructor (D1 and D2) and DeclareProperties. Each comment names what is missing.
// Skipped: the deleting destructor (D0). Reconstructed: Draw, IsZonable, IsUpdatable and the constructor (C1 and C2).

#include <string>

#include "dh2/Billboard.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/PropertyMap.hpp"

// 0x3876c8 _ZNK9Billboard4DrawEv
void Billboard::Draw() const {
    // Empty in the original.
}

// 0x3876cc _ZNK9Billboard9IsZonableEv
bool Billboard::IsZonable() const {
    return false;
}

// 0x3876d4 _ZNK9Billboard11IsUpdatableEv
bool Billboard::IsUpdatable() const {
    return true;
}

// 0x3876dc _ZN9Billboard6UpdateEv
// PARTIAL: the two guards are reconstructed. The call is not made. The original calls vtable slot +0x5c (ISceneNode::addChild)
// of the active camera, CSceneManager::activeCamera at +0xe4, with the node's ISceneNode subobject (node + 4) as the argument.
// Reaching the scene manager needs glitch::IDevice +0x1c, which the device does not declare (layout request, also asked by
// CameraBase::SetActive). Forming node + 4 as an ISceneNode* needs the CBillboardSceneNode layout, which has no canonical
// header yet.
void Billboard::Update() {
    if (m_billboardNode == nullptr) {  // +0x178
        return;
    }
    if (!m_attachToActiveCamera) {  // +0x174
        return;
    }
    // Not made: activeCamera->addChild(the ISceneNode subobject of m_billboardNode).
}

// 0x38781c _ZN9Billboard8InitPostEv
// PARTIAL: not reconstructed. The original runs these steps in order (the pseudocode and the disassembly agree):
//   1. texture = driver->texture manager->getTexture(m_tga.c_str(), nullptr), where driver is glitch::IDevice +0x10 and the
//      texture manager is IVideoDriver +0xe0. Return when the texture is null.
//   2. node = scene manager->addSceneNode("billBoard", scene manager->rootNode), where the scene manager is IDevice +0x1c.
//      m_billboardNode = node - 4 (the ISceneNode subobject is at +4), and the node's reference count goes up by one.
//   3. material = material renderer manager->createMaterialInstance(m_isAdditive ? 10 : 11), where the manager is
//      IVideoDriver +0xdc. If getParameterID(2, 0) is not 0xffff, the material gets the texture at that parameter.
//   4. m_billboardNode->setMaterial(material); vtable slot +0x08 of the node (setSize) with &m_size; node + 4 gets
//      setPosition(m_position) (vtable +0xa4) and setVisible(false) (vtable +0x48).
//   5. m_attachToActiveCamera = (m_attachTo == "ActiveCamera").
//   6. When m_attachToActiveCamera is false and m_attachTo is not empty, m_attachHandle = ObjectManager::GetObjectByName(
//      m_attachTo.c_str(), -1, false, nullptr).
//   7. When m_attachHandle.GetObject(false) is null, scene manager->rootNode->addChild(node + 4) (vtable +0x5c). When it is a
//      GameObject with m_visualObject (+0x2d8), m_visualObject->m_node (VisualObject +0x08)->addChild(node + 4). Otherwise nothing.
//   8. The material and the texture references are released.
// Blocked by: glitch::IDevice +0x10 and +0x1c, the IVideoDriver +0xdc and +0xe0 members, the CMaterialRendererManager
// createMaterialInstance declaration, the CMaterial parameter setter for an intrusive_ptr<ITexture>, and the
// CBillboardSceneNode layout (setSize, setMaterial and the reference count).
void Billboard::InitPost() {
}

// 0x387ba4 _ZN9BillboardD1Ev
// PARTIAL: the original drops the node first: glitch::IReferenceCounted::drop on the IReferenceCounted virtual base of
// m_billboardNode. That drop is not made, because CBillboardSceneNode is only forward-declared here. The string members
// and the ObjectBase base are destroyed by the compiler in the original order.
Billboard::~Billboard() {
    // Not made: if (m_billboardNode) drop the IReferenceCounted base of m_billboardNode.
}

// 0x387c1c _ZN9BillboardD0Ev
// skipped: the compiler generates the deleting destructor (~Billboard() followed by CustomFree(this)). A delete expression
// produces it, so no source is written for it.

// 0x387c38 _ZN9BillboardD2Ev
// Same body as the D1 destructor at 0x387ba4. The C++ destructor above is the one definition, so the partial note applies here too.

// 0x387d84 _ZN9BillboardC2EN10ObjectBase6GO_IDSE
// 0x38815c _ZN9BillboardC1EN10ObjectBase6GO_IDSE
// Both addresses have the same body. One C++ constructor covers them.
Billboard::Billboard(ObjectBase::GO_IDS id) : ObjectBase(id) {
    unk_85 = 1;                      // +0x85 updating flag, set by the constructor
    m_size = {};                     // +0x150 zero
    m_position = {};                 // +0x158 zero
    m_isAdditive = false;            // +0x164
    m_attachToActiveCamera = false;  // +0x174
    m_billboardNode = nullptr;       // +0x178
    // m_tga and m_attachTo are default constructed (the original allocates their 16-byte buffers, which is not observable).
    // m_attachHandle is default constructed by ObjectHandle().
}

// 0x387fc4 _ZN9Billboard17DeclarePropertiesEv
// PARTIAL: "position" and "size" are not registered. The original builds each one directly as a SimpleTypeProperty:
//   "position": SimpleTypeProperty<glitch::core::vector3d<float>>, offset 0x154 from the PropertyMap at +4, default (0, 0, 0).
//   "size":     SimpleTypeProperty<glitch::core::dimension2d<float>>, offset 0x14c from the PropertyMap at +4, default (0, 0).
// dh2/SimpleTypeProperty.hpp declares the (object, name, field, default) constructor, but no definition exists in the tree
// for these two types. AddProperty<T> is instantiated only for bool, float, int, Point3D<float> and std::string.
// The original returns the map slot of the last property. The virtual is void, so the value is dropped here.
void Billboard::DeclareProperties() {
    AddProperty<std::string>("tga", m_tga);
    AddProperty<std::string>("AttachTo", m_attachTo);
    // Not registered: "position" and "size" (see above).
    AddProperty<bool>("isAdditive", m_isAdditive, false);
}
