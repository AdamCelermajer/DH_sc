// Slice 25 of the glitch::scene reconstruction: CDummyTransformationSceneNode, the default scene-node factories and the Collada
// binary loader. (The glitch::gui part of the slice, CGUIContextMenu, IGUIElement::setTabOrder and CGUIEditBox, is in gui/*.hpp.)
//
// Member offsets come from the layout headers under layouts/ (32-bit ARM). Members the
// layouts do not declare are not invented: those bodies are marked partial in
// ledger_parts/slice_25.csv and call the unattached helpers below.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::collada::recon_25 {

// glitch::scene. CColladaBinaryFileLoader, the factories and CDummyTransformationSceneNode have no layout in
// layouts/scene.hpp; their bodies are partial. (CAppendMeshBuffer is the canonical class of scene/CAppendMeshBuffer.hpp.)
void CColladaBinaryFileLoader_dtor_complete(void* self);                                                    // 0x6b93b0
void CColladaBinaryFileLoader_dtor_complete_alt(void* self);                                                // 0x6b93f0
int CColladaBinaryFileLoader_createScene(void* self, void* file);                                           // 0x6b9430
void* CColladaBinaryFileLoader_createMesh(void* self, void* file, void* driverFile);                        // 0x6b9458
void CDefaultSceneNodeAnimatorFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl); // 0x6b9590
void CDefaultSceneNodeAnimatorFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager,
                                               void* cursorControl);                                        // 0x6b95dc
void CDefaultSceneNodeAnimatorFactory_dtor_complete(void* self);                                            // 0x6b9628
void CDefaultSceneNodeAnimatorFactory_dtor_complete_alt(void* self);                                        // 0x6b9668
std::int32_t CDefaultSceneNodeAnimatorFactory_getTypeFromName(void* self, const char* name);                // 0x6b9720
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator_byName(void* self, const char* name,
                                                                     void* sceneNode);                      // 0x6b9778
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator(void* self, std::int32_t type,
                                                               void* sceneNode);                            // 0x6b9928
std::int32_t CDefaultSceneNodeFactory_getCreateableSceneNodeTypeName(void* self, std::int32_t type);       // 0x6b9da0
std::int32_t CDefaultSceneNodeFactory_getTypeFromName(void* self, const char* name);                        // 0x6b9e58
void* CDefaultSceneNodeFactory_addSceneNode_byName(void* self, const char* name, void* parent);           // 0x6b9f04
void* CDefaultSceneNodeFactory_addSceneNode(void* self, std::int32_t type, void* parent);                   // 0x6b9f30
void CDefaultSceneNodeFactory_SSceneNodeTypePair_ctor(void* self, std::int32_t type, const char* name);     // 0x6ba774
void CDefaultSceneNodeFactory_dtor_complete(void* self);                                                    // 0x6ba8f8
void CDefaultSceneNodeFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                   void* fileSystem);                                                       // 0x6baae8
void CDefaultSceneNodeFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                       void* fileSystem);                                                   // 0x6bb008
void* CDummyTransformationSceneNode_getRelativeTransformationMatrix(void* self);                             // 0x6bb540
void IDummyTransformationSceneNode_dtor_complete(void* self);                                               // 0x6bb568
void CDummyTransformationSceneNode_dtor_complete(void* self);                                               // 0x6bb5d4
void CDummyTransformationSceneNode_setRelativeTransformationMatrix(void* self, const float* matrix);        // 0x6bb65c
void IDummyTransformationSceneNode_ctor(void* self, const void* parent, std::int32_t id);                   // 0x6bb6f0
void CDummyTransformationSceneNode_ctor(void* self, std::int32_t id);                                       // 0x6bb77c
void* CDummyTransformationSceneNode_clone(void* self);                                                      // 0x6bb858
void CDummyTransformationSceneNode_ctor_alt(void* self, const void* parent, std::int32_t id);               // 0x6bb940
void* CMeshCache_getMeshByIndex(void* self, std::uint32_t index);                                           // 0x6be5b0
bool CMeshCache_isMeshLoaded(void* self, const char* name);                                                 // 0x6be6d4

}  // namespace glitch::collada::recon_25
