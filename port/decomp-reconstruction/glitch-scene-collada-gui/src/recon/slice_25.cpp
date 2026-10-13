// Slice 25: glitch::scene (CDummyTransformationSceneNode, default factories, Collada binary loader). Declarations and layouts are in
// include/glitch-scene-collada-gui/recon/slice_25.hpp and layouts/.

#include "glitch-scene-collada-gui/recon/slice_25.hpp"

namespace glitch::collada::recon_25 {

// CMeshBuffer (0x6b88a8 clone, 0x6bc6d8 onDelete, 0x6bca38 D2, 0x6bcb90 clone): canonical members in src/scene/CMeshBuffer.cpp.

// 0x6b93b0 _ZN6glitch5scene24CColladaBinaryFileLoaderD2Ev
// glitch::scene::CColladaBinaryFileLoader::~CColladaBinaryFileLoader (complete).
// Unattached: the loader layout is not declared; the member release is stubbed.
void CColladaBinaryFileLoader_dtor_complete(void* self) {
    (void)self;
}

// 0x6b93f0 _ZN6glitch5scene24CColladaBinaryFileLoaderD1Ev
// glitch::scene::CColladaBinaryFileLoader::~CColladaBinaryFileLoader (second variant).
void CColladaBinaryFileLoader_dtor_complete_alt(void* self) {
    CColladaBinaryFileLoader_dtor_complete(self);
}

// 0x6b9430 _ZN6glitch5scene24CColladaBinaryFileLoader11createSceneEPNS_2io9IReadFileE
// glitch::scene::CColladaBinaryFileLoader::createScene. The decompiled body returns 0.
int CColladaBinaryFileLoader_createScene(void* self, void* file) {
    (void)self;
    (void)file;
    return 0;
}

// 0x6b9458 _ZN6glitch5scene24CColladaBinaryFileLoader10createMeshEPNS_2io9IReadFileE
// glitch::scene::CColladaBinaryFileLoader::createMesh. Unattached: CResFileManager,
// CColladaDatabase and the driver-file words are not declared.
void* CColladaBinaryFileLoader_createMesh(void* self, void* file, void* driverFile) {
    (void)self;
    (void)file;
    (void)driverFile;
    return nullptr;
}

// 0x6b9590 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryC2EPNS0_13CSceneManagerEPNS_3gui14ICursorControlE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::CDefaultSceneNodeAnimatorFactory.
// Unattached: the factory layout is not declared in scene.hpp.
void CDefaultSceneNodeAnimatorFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl) {
    (void)self;
    (void)manager;
    (void)cursorControl;
}

// 0x6b95dc _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryC1EPNS0_13CSceneManagerEPNS_3gui14ICursorControlE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::CDefaultSceneNodeAnimatorFactory (second variant).
void CDefaultSceneNodeAnimatorFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager, void* cursorControl) {
    CDefaultSceneNodeAnimatorFactory_ctor(self, manager, cursorControl);
}

// 0x6b9628 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryD2Ev
// glitch::scene::CDefaultSceneNodeAnimatorFactory::~CDefaultSceneNodeAnimatorFactory (complete).
// Unattached: the factory layout is not declared; the cursor-control release is stubbed.
void CDefaultSceneNodeAnimatorFactory_dtor_complete(void* self) {
    (void)self;
}

// 0x6b9668 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryD1Ev
// glitch::scene::CDefaultSceneNodeAnimatorFactory::~CDefaultSceneNodeAnimatorFactory (second variant).
void CDefaultSceneNodeAnimatorFactory_dtor_complete_alt(void* self) {
    CDefaultSceneNodeAnimatorFactory_dtor_complete(self);
}

// 0x6b9720 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory15getTypeFromNameEPKc
// glitch::scene::CDefaultSceneNodeAnimatorFactory::getTypeFromName. Unattached: the
// SceneNodeAnimatorTypeNames table is not declared; returns the not-found index 10.
std::int32_t CDefaultSceneNodeAnimatorFactory_getTypeFromName(void* self, const char* name) {
    (void)self;
    (void)name;
    return 10;
}

// 0x6b9928 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactory23createSceneNodeAnimatorENS0_26E_SCENE_NODE_ANIMATOR_TYPEEPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::createSceneNodeAnimator(type, node).
// The animator classes (flyCircle, flyStraight, ...) are not declared; the switch is not rebuilt.
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator(void* self, std::int32_t type, void* sceneNode) {
    (void)self;
    (void)type;
    (void)sceneNode;
    return nullptr;
}

// 0x6b9778 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactory23createSceneNodeAnimatorEPKcPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::createSceneNodeAnimator(name, node).
// Looks up the type and delegates to the enum overload through the factory.
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator_byName(void* self, const char* name, void* sceneNode) {
    const std::int32_t type = CDefaultSceneNodeAnimatorFactory_getTypeFromName(self, name);
    return CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator(self, type, sceneNode);
}

// 0x6b9da0 _ZNK6glitch5scene24CDefaultSceneNodeFactory30getCreateableSceneNodeTypeNameENS0_17E_SCENE_NODE_TYPEE
// glitch::scene::CDefaultSceneNodeFactory::getCreateableSceneNodeTypeName. Unattached: the
// type-name table entries (28-byte stride) are not declared; returns 0.
std::int32_t CDefaultSceneNodeFactory_getCreateableSceneNodeTypeName(void* self, std::int32_t type) {
    (void)self;
    (void)type;
    return 0;
}

// 0x6b9e58 _ZNK6glitch5scene24CDefaultSceneNodeFactory15getTypeFromNameEPKc
// glitch::scene::CDefaultSceneNodeFactory::getTypeFromName. Unattached: the type-name
// table is not declared; returns the not-found value 1852534389.
std::int32_t CDefaultSceneNodeFactory_getTypeFromName(void* self, const char* name) {
    (void)self;
    (void)name;
    return 1852534389;
}

// 0x6b9f04 _ZN6glitch5scene24CDefaultSceneNodeFactory12addSceneNodeEPKcPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeFactory::addSceneNode(name, parent). Looks up the type and
// delegates to the enum overload through the factory.
void* CDefaultSceneNodeFactory_addSceneNode_byName(void* self, const char* name, void* parent) {
    const std::int32_t type = CDefaultSceneNodeFactory_getTypeFromName(self, name);
    return CDefaultSceneNodeFactory_addSceneNode(self, type, parent);
}

// 0x6b9f30 _ZN6glitch5scene24CDefaultSceneNodeFactory12addSceneNodeENS0_17E_SCENE_NODE_TYPEEPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeFactory::addSceneNode(type, parent). The per-type
// switch (about 2 KB of constructors) is not rebuilt; returns nullptr.
void* CDefaultSceneNodeFactory_addSceneNode(void* self, std::int32_t type, void* parent) {
    (void)self;
    (void)type;
    (void)parent;
    return nullptr;
}

// 0x6ba774 _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
// glitch::scene::CDefaultSceneNodeFactory::SSceneNodeTypePair::SSceneNodeTypePair. The
// name range initialization is not modelled; the type is not stored.
void CDefaultSceneNodeFactory_SSceneNodeTypePair_ctor(void* self, std::int32_t type, const char* name) {
    (void)self;
    (void)type;
    (void)name;
}

// 0x6ba8f8 _ZN6glitch5scene24CDefaultSceneNodeFactoryD1Ev
// glitch::scene::CDefaultSceneNodeFactory::~CDefaultSceneNodeFactory (complete). The
// type-pair vector destruction is stubbed.
void CDefaultSceneNodeFactory_dtor_complete(void* self) {
    (void)self;
}

// 0x6baae8 _ZN6glitch5scene24CDefaultSceneNodeFactoryC2EPNS0_13CSceneManagerEPNS_3gui14ICursorControlERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
// glitch::scene::CDefaultSceneNodeFactory::CDefaultSceneNodeFactory. The 1.2 KB
// constructor (type-name table and file-system reference) is not rebuilt.
void CDefaultSceneNodeFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                   void* fileSystem) {
    (void)self;
    (void)manager;
    (void)cursorControl;
    (void)fileSystem;
}

// 0x6bb008 _ZN6glitch5scene24CDefaultSceneNodeFactoryC1EPNS0_13CSceneManagerEPNS_3gui14ICursorControlERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
// glitch::scene::CDefaultSceneNodeFactory::CDefaultSceneNodeFactory (second variant).
void CDefaultSceneNodeFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                       void* fileSystem) {
    CDefaultSceneNodeFactory_ctor(self, manager, cursorControl, fileSystem);
}

// 0x6bb540 _ZN6glitch5scene29CDummyTransformationSceneNode31getRelativeTransformationMatrixEv
// glitch::scene::CDummyTransformationSceneNode::getRelativeTransformationMatrix. The
// matrix member at +0x130 is not declared in scene.hpp; returns nullptr.
void* CDummyTransformationSceneNode_getRelativeTransformationMatrix(void* self) {
    (void)self;
    return nullptr;
}

// 0x6bb568 _ZN6glitch5scene29IDummyTransformationSceneNodeD1Ev
// glitch::scene::IDummyTransformationSceneNode::~IDummyTransformationSceneNode (complete).
// The base destruction is compiler-generated; nothing else is done.
void IDummyTransformationSceneNode_dtor_complete(void* self) {
    (void)self;
}

// 0x6bb5d4 _ZN6glitch5scene29CDummyTransformationSceneNodeD1Ev
// glitch::scene::CDummyTransformationSceneNode::~CDummyTransformationSceneNode (complete).
void CDummyTransformationSceneNode_dtor_complete(void* self) {
    (void)self;
}

// 0x6bb65c _ZN6glitch5scene29CDummyTransformationSceneNode31setRelativeTransformationMatrixERKNS_4core8CMatrix4IfEE
// glitch::scene::CDummyTransformationSceneNode::setRelativeTransformationMatrix. The matrix
// copy and the virtual translation/rotation/scale setters are not rebuilt.
void CDummyTransformationSceneNode_setRelativeTransformationMatrix(void* self, const float* matrix) {
    (void)self;
    (void)matrix;
}

// 0x6bb6f0 _ZN6glitch5scene29IDummyTransformationSceneNodeC2Ei
// glitch::scene::IDummyTransformationSceneNode::IDummyTransformationSceneNode. The
// ISceneNode base constructor is not declared; the identity defaults are not applied.
void IDummyTransformationSceneNode_ctor(void* self, const void* parent, std::int32_t id) {
    (void)self;
    (void)parent;
    (void)id;
}

// 0x6bb77c _ZN6glitch5scene29CDummyTransformationSceneNodeC1Ei
// glitch::scene::CDummyTransformationSceneNode::CDummyTransformationSceneNode. Depends on
// the base constructor above, so the body is a stub.
void CDummyTransformationSceneNode_ctor(void* self, std::int32_t id) {
    IDummyTransformationSceneNode_ctor(self, nullptr, id);
}

// 0x6bb858 _ZN6glitch5scene29CDummyTransformationSceneNode5cloneEv
// glitch::scene::CDummyTransformationSceneNode::clone. Depends on the base clone-members
// and the matrix copy; returns nullptr.
void* CDummyTransformationSceneNode_clone(void* self) {
    (void)self;
    return nullptr;
}

// 0x6bb940 _ZN6glitch5scene29CDummyTransformationSceneNodeC2Ei
// glitch::scene::CDummyTransformationSceneNode::CDummyTransformationSceneNode (second variant).
void CDummyTransformationSceneNode_ctor_alt(void* self, const void* parent, std::int32_t id) {
    IDummyTransformationSceneNode_ctor(self, parent, id);
}

// CMesh (0x6bbcc4 SBuffer assign, 0x6bc298 D2, 0x6bc2cc recalculateBoundingBox, 0x6bc3e4 clone): canonical members in
// src/scene/CMesh.cpp.

// 0x6be5b0 _ZN6glitch5scene10CMeshCache14getMeshByIndexEj
// glitch::scene::CMeshCache::getMeshByIndex. Unattached: the mesh cache layout is not
// declared; returns nullptr.
void* CMeshCache_getMeshByIndex(void* self, std::uint32_t index) {
    (void)self;
    (void)index;
    return nullptr;
}

// 0x6be6d4 _ZN6glitch5scene10CMeshCache12isMeshLoadedEPKc
// glitch::scene::CMeshCache::isMeshLoaded. Unattached: the lookup goes through the cache
// vtable slot +0x28, which is not declared; returns false.
bool CMeshCache_isMeshLoaded(void* self, const char* name) {
    (void)self;
    (void)name;
    return false;
}

}  // namespace glitch::collada::recon_25
