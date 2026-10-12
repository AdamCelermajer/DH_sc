// Slice 32: scene-node attribute (de)serializers and light/mesh node constructors in
// glitch::scene, the CSceneManager compile and read entries, the particle-system force
// node attribute (de)serializers, the CColladaDatabase construct* entries that pick a
// factory creator, the CRootSceneNode material and IFL helpers, the mesh and skinned-mesh
// transform and buffer-prepare entries, and the SAnimationAccessor key-frame searches.
//
// Stand-ins below cover code outside this slice (IAttributes slot calls, base-class
// constructors and vtable slots that are not mapped yet, member lookups not declared in
// the existing headers). Rows that depend on a stand-in are ledgered partial with the
// stand-in named in the note; the body is not invented.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/collada_database.hpp"
#include "glitch-scene-collada-gui/collada_factory.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

// Forward declarations of the types this slice names. Full layouts live in the
// headers that own them (or are not mapped yet); the bodies here only pass pointers
// and references through.
namespace glitch::io {
class IAttributes;
struct SAttributeReadWriteOptions;
class IReadFile;
}  // namespace glitch::io

namespace glitch::core {
template <typename T>
class triangle3d;
template <typename T>
class CMatrix4;
class quaternion;
}  // namespace glitch::core

namespace glitch::video {
class ITexture;
}  // namespace glitch::video

namespace glitch::scene {
class ISceneNode;
class CLightSceneNode;
class CMeshSceneNode;
class CSceneManager;
class CSceneManagerRootNode;
class CTriangleSelector;
class CBatchSceneNode;
class CMeshBuffer;
class ISegmentCompileCallback;
class ISplitSegmentChoiceCallback;
class ISceneUserDataSerializer;
}  // namespace glitch::scene

namespace glitch::ps {
template <typename T>
class IParticleSystem;
class PRenderData;
struct GNPSParticle;
struct SParticle;
}  // namespace glitch::ps

namespace glitch::collada {
class CAnimationBlock;
struct SAnimationBlockSearchKey;
class CAnimationStreamingManager;
class CSceneNodeAnimator;
class CModularSkinnedMeshSceneNode;
class CResFile;
class CResFileManager;
class IParticleSystemSceneNode;
class ISkinnedMesh;
class SAnimationAccessor;
struct SInstanceGNPSEmitter;
struct SInstanceCoronas;
struct SInstanceEmitter;
namespace particle_system {
class CDeflectorForceSceneNode;
class CWindForceSceneNode;
class CGravityForceSceneNode;
}  // namespace particle_system
}  // namespace glitch::collada

namespace glitch::collada::recon_32 {

// ---- glitch::scene::CLightSceneNode -------------------------------------------------

// 0x583ddc _ZNK6glitch5scene15CLightSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
int CLightSceneNode_serializeAttributes(const scene::CLightSceneNode& self, io::IAttributes* attributes,
                                        io::SAttributeReadWriteOptions* options);

// 0x584350 _ZN6glitch5scene15CLightSceneNodeC1EiRKNS_4core8vector3dIfEERKNS_5video7SColorfEf
void CLightSceneNode_ctor_C1(scene::CLightSceneNode& self, int id, const core::vector3d<float>& position,
                             const video::SColorf& color, float radius);

// 0x584544 _ZN6glitch5scene15CLightSceneNodeC2EiRKNS_4core8vector3dIfEERKNS_5video7SColorfEf
void CLightSceneNode_ctor_C2(scene::CLightSceneNode& self, int id, const core::vector3d<float>& position,
                             const video::SColorf& color, float radius);

// ---- glitch::scene::CMeshSceneNode --------------------------------------------------

// 0x584b90 _ZNK6glitch5scene14CMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CMeshSceneNode_serializeAttributes(const scene::CMeshSceneNode& self, io::IAttributes* attributes,
                                        io::SAttributeReadWriteOptions* options);

// 0x584ed0 _ZN6glitch5scene14CMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CMeshSceneNode_deserializeAttributes(scene::CMeshSceneNode& self, io::IAttributes* attributes,
                                          io::SAttributeReadWriteOptions* options);

// ---- glitch::scene::CTriangleSelector -----------------------------------------------

// 0x5871c8 _ZNK6glitch5scene17CTriangleSelector12TestWithLineERSt6vectorINS_4core10triangle3dIfEENS3_10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEE
bool CTriangleSelector_TestWithLine(const scene::CTriangleSelector& self,
                                    std::vector<core::triangle3d<float>>& triangles);

// ---- glitch::scene::CSceneManagerRootNode -------------------------------------------

// 0x588f84 _ZNK6glitch5scene21CSceneManagerRootNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CSceneManagerRootNode_serializeAttributes(const scene::CSceneManagerRootNode& self,
                                               io::IAttributes* attributes,
                                               io::SAttributeReadWriteOptions* options);

// 0x588fb0 _ZN6glitch5scene21CSceneManagerRootNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CSceneManagerRootNode_deserializeAttributes(scene::CSceneManagerRootNode& self,
                                                 io::IAttributes* attributes,
                                                 io::SAttributeReadWriteOptions* options);

// ---- glitch::scene::CSceneManager ---------------------------------------------------

// 0x588fe8 _ZNK6glitch5scene13CSceneManager19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CSceneManager_serializeAttributes(const scene::CSceneManager& self, io::IAttributes* attributes,
                                       io::SAttributeReadWriteOptions* options);

// 0x589128 _ZN6glitch5scene13CSceneManager18registerSceneNodesERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
void CSceneManager_registerSceneNodes(scene::CSceneManager& self,
                                      const std::vector<scene::ISceneNode*>& nodes);

// 0x589168 _ZN6glitch5scene13CSceneManager25registerSceneNodesCompileERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
void CSceneManager_registerSceneNodesCompile(scene::CSceneManager& self,
                                             const std::vector<scene::ISceneNode*>& nodes);

// 0x58b6c8 _ZN6glitch5scene13CSceneManager15collectAllNodesERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
void CSceneManager_collectAllNodes(scene::CSceneManager& self, const std::vector<scene::ISceneNode*>& nodes);

// 0x58bbc4 _ZN6glitch5scene13CSceneManager21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CSceneManager_deserializeAttributes(scene::CSceneManager& self, io::IAttributes* attributes,
                                         io::SAttributeReadWriteOptions* options);

// 0x58c4a4 _ZN6glitch5scene13CSceneManager12readUserDataEPNS_2io13IIrrXMLReaderIwNS_17IReferenceCountedEEEPNS0_10ISceneNodeEPNS0_24ISceneUserDataSerializerE
void CSceneManager_readUserData(scene::CSceneManager& self, void* reader, scene::ISceneNode* node,
                                scene::ISceneUserDataSerializer* serializer);

// 0x58c644 _ZN6glitch5scene13CSceneManager13readAnimatorsEPNS_2io13IIrrXMLReaderIwNS_17IReferenceCountedEEEPNS0_10ISceneNodeE
void CSceneManager_readAnimators(scene::CSceneManager& self, void* reader, scene::ISceneNode* node);

// 0x58c8dc _ZN6glitch5scene13CSceneManager13readMaterialsEPNS_2io13IIrrXMLReaderIwNS_17IReferenceCountedEEEPNS0_10ISceneNodeE
void CSceneManager_readMaterials(scene::CSceneManager& self, void* reader, scene::ISceneNode* node);

// 0x58d908 _ZN6glitch5scene13CSceneManagerC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNSC_15IGUIEnvironmentE
void CSceneManager_ctor_C1(scene::CSceneManager& self, video::IVideoDriver* driver, const void* fileSystem,
                           void* cursorControl, void* meshCache, void* guiEnvironment);

// 0x58ddd0 _ZN6glitch5scene13CSceneManagerC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNSC_15IGUIEnvironmentE
void CSceneManager_ctor_C2(scene::CSceneManager& self, video::IVideoDriver* driver, const void* fileSystem,
                           void* cursorControl, void* meshCache, void* guiEnvironment);

// 0x58e1e0 _ZN6glitch5scene13CSceneManager13readSceneNodeEPNS_2io13IIrrXMLReaderIwNS_17IReferenceCountedEEEPNS0_10ISceneNodeEPNS0_24ISceneUserDataSerializerE
scene::ISceneNode* CSceneManager_readSceneNode(scene::CSceneManager& self, void* reader, scene::ISceneNode* parent,
                                               scene::ISceneUserDataSerializer* serializer);

// 0x58fd10 _ZN6glitch5scene13CSceneManager15compileInternalERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEEPNS0_15CBatchSceneNodeEbPNS0_23ISegmentCompileCallbackEPNS0_27ISplitSegmentChoiceCallbackE
scene::CBatchSceneNode* CSceneManager_compileInternal(scene::CSceneManager& self,
                                                      const std::vector<scene::ISceneNode*>& nodes,
                                                      scene::CBatchSceneNode* batch, bool flag,
                                                      scene::ISegmentCompileCallback* segmentCallback,
                                                      scene::ISplitSegmentChoiceCallback* splitCallback);

// 0x58fee0 _ZN6glitch5scene13CSceneManager7compileERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEEPNS0_15CBatchSceneNodeEbPNS0_23ISegmentCompileCallbackEPNS0_27ISplitSegmentChoiceCallbackEj
scene::CBatchSceneNode* CSceneManager_compile(scene::CSceneManager& self,
                                              const std::vector<scene::ISceneNode*>& nodes,
                                              scene::CBatchSceneNode* batch, bool flag,
                                              scene::ISegmentCompileCallback* segmentCallback,
                                              scene::ISplitSegmentChoiceCallback* splitCallback,
                                              std::uint32_t flags);

// 0x58ff10 _ZN6glitch5scene13CSceneManager7compileERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEEPNS0_15CBatchSceneNodeEbPNS0_23ISegmentCompileCallbackEPNS0_27ISplitSegmentChoiceCallbackENS5_8vector3dIfEE
scene::CBatchSceneNode* CSceneManager_compile(scene::CSceneManager& self,
                                              const std::vector<scene::ISceneNode*>& nodes,
                                              scene::CBatchSceneNode* batch, bool flag,
                                              scene::ISegmentCompileCallback* segmentCallback,
                                              scene::ISplitSegmentChoiceCallback* splitCallback,
                                              const core::vector3d<float>& extent);

// ---- glitch::scene::ISceneNode ------------------------------------------------------

// 0x5972a4 _ZNK6glitch5scene10ISceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void ISceneNode_serializeAttributes(const scene::ISceneNode& self, io::IAttributes* attributes,
                                    io::SAttributeReadWriteOptions* options);

// 0x598058 _ZN6glitch5scene10ISceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void ISceneNode_deserializeAttributes(scene::ISceneNode& self, io::IAttributes* attributes,
                                      io::SAttributeReadWriteOptions* options);

// 0x5990c0 _ZN6glitch5scene10ISceneNodeC2EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
void ISceneNode_ctor_C2(scene::ISceneNode& self, int id, const core::vector3d<float>& position,
                        const core::quaternion& rotation, const core::vector3d<float>& scale);

// 0x599268 _ZN6glitch5scene10ISceneNodeC1EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
void ISceneNode_ctor_C1(scene::ISceneNode& self, int id, const core::vector3d<float>& position,
                        const core::quaternion& rotation, const core::vector3d<float>& scale);

// ---- glitch::collada::CCoronasSceneNode -------------------------------------------

// 0x60f8b8 _ZN6glitch7collada17CCoronasSceneNode11setMaterialERNS0_9SMaterialEPNS_5video12IVideoDriverE
void CCoronasSceneNode_setMaterial(CCoronasSceneNode& self, SMaterial& material, video::IVideoDriver* driver);

// ---- glitch::collada::CAnimationStreamingManager ------------------------------------

// 0x60c59c _ZN6glitch7collada26CAnimationStreamingManager17getAnimationBlockERNS0_24SAnimationBlockSearchKeyERN5boost13intrusive_ptrINS0_15CAnimationBlockEEE
void CAnimationStreamingManager_getAnimationBlock(CAnimationStreamingManager& self, SAnimationBlockSearchKey& key,
                                                  boost::intrusive_ptr<CAnimationBlock>& block);

// ---- glitch::collada::CColladaDatabase constructors ---------------------------------
// The factory (the original *(this + 0x04)) is passed explicitly; the database members
// that look entries up by name are not declared in collada_database.hpp yet.

// 0x60e75c _ZNK6glitch7collada16CColladaDatabase16constructCoronasEPNS0_8SCoronasEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
CCoronasSceneNode* CColladaDatabase_constructCoronas(const CColladaDatabase& self, CColladaFactory* factory,
                                                     SCoronas* coronas, video::IVideoDriver* driver,
                                                     CRootSceneNode* root);

// 0x60e79c _ZNK6glitch7collada16CColladaDatabase16constructEmitterEPNS0_8SEmitterEPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
CParticleSystemSceneNode* CColladaDatabase_constructEmitter(const CColladaDatabase& self, CColladaFactory* factory,
                                                            SEmitter* emitter, video::IVideoDriver* driver,
                                                            res::vector<res::String>* materials,
                                                            CRootSceneNode* root);

// 0x60e7e0 _ZNK6glitch7collada16CColladaDatabase16constructEmitterEiPNS_5video12IVideoDriverEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
CParticleSystemSceneNode* CColladaDatabase_constructEmitter(const CColladaDatabase& self, CColladaFactory* factory,
                                                            int index, video::IVideoDriver* driver,
                                                            res::vector<res::String>* materials,
                                                            CRootSceneNode* root);

// 0x60e814 _ZNK6glitch7collada16CColladaDatabase20constructGNPSEmitterEPNS0_12SGNPSEmitterEPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
CGlitchNewParticleSystemSceneNode* CColladaDatabase_constructGNPSEmitter(const CColladaDatabase& self,
                                                                         CColladaFactory* factory,
                                                                         SGNPSEmitter* emitter,
                                                                         video::IVideoDriver* driver,
                                                                         res::vector<res::String>* materials,
                                                                         CRootSceneNode* root);

// 0x60e858 _ZNK6glitch7collada16CColladaDatabase20constructGNPSEmitterEiPNS_5video12IVideoDriverEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
CGlitchNewParticleSystemSceneNode* CColladaDatabase_constructGNPSEmitter(const CColladaDatabase& self,
                                                                         CColladaFactory* factory, int index,
                                                                         video::IVideoDriver* driver,
                                                                         res::vector<res::String>* materials,
                                                                         CRootSceneNode* root);

// 0x61a608 _ZNK6glitch7collada16CColladaDatabase20constructGNPSEmitterEPNS0_20SInstanceGNPSEmitterEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
CGlitchNewParticleSystemSceneNode* CColladaDatabase_constructGNPSEmitter(const CColladaDatabase& self,
                                                                         CColladaFactory* factory,
                                                                         SInstanceGNPSEmitter* instance,
                                                                         video::IVideoDriver* driver,
                                                                         CRootSceneNode* root);

// 0x61a77c _ZNK6glitch7collada16CColladaDatabase16constructCoronasEPNS0_16SInstanceCoronasEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
CCoronasSceneNode* CColladaDatabase_constructCoronas(const CColladaDatabase& self, CColladaFactory* factory,
                                                     SInstanceCoronas* instance, video::IVideoDriver* driver,
                                                     CRootSceneNode* root);

// 0x61a888 _ZNK6glitch7collada16CColladaDatabase16constructEmitterEPNS0_16SInstanceEmitterEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
CParticleSystemSceneNode* CColladaDatabase_constructEmitter(const CColladaDatabase& self, CColladaFactory* factory,
                                                            SInstanceEmitter* instance, video::IVideoDriver* driver,
                                                            CRootSceneNode* root);

// 0x61bc8c _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPNS_2io9IReadFileEbbPNS0_15CColladaFactoryE
int CColladaDatabase_constructScene(CColladaDatabase& self, video::IVideoDriver* driver, io::IReadFile* file,
                                    bool flagA, bool flagB, CColladaFactory* factory);

// 0x61bd54 _ZN6glitch7collada16CColladaDatabase14constructSceneEPNS_5video12IVideoDriverEPNS_2io9IReadFileEbPNS0_15CColladaFactoryE
int CColladaDatabase_constructScene(CColladaDatabase& self, video::IVideoDriver* driver, io::IReadFile* file,
                                    bool flag, CColladaFactory* factory);

// 0x61c71c _ZN6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS_2io9IReadFileEPKcbPNS0_15CColladaFactoryE
CRootSceneNode* CColladaDatabase_constructNode(CColladaDatabase& self, video::IVideoDriver* driver,
                                               io::IReadFile* file, const char* name, bool flag,
                                               CColladaFactory* factory);

// 0x61c7cc _ZN6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS_2io9IReadFileEPKcPNS0_15CColladaFactoryE
CRootSceneNode* CColladaDatabase_constructNode(CColladaDatabase& self, video::IVideoDriver* driver,
                                               io::IReadFile* file, const char* name, CColladaFactory* factory);

// ---- glitch::scene::IParticleSystemSceneNode and particle forces --------------------

// 0x619820 _ZN6glitch7collada24IParticleSystemSceneNode11addMaterialERNS0_9SMaterialEPNS_5video12IVideoDriverE
void IParticleSystemSceneNode_addMaterial(IParticleSystemSceneNode& self, SMaterial& material,
                                          video::IVideoDriver* driver);

// 0x6677d0 _ZNK6glitch7collada24IParticleSystemSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// (thunk to glitch::scene::ISceneNode::serializeAttributes; not reimplemented)

// 0x6677d4 _ZN6glitch7collada24IParticleSystemSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// (thunk to glitch::scene::ISceneNode::deserializeAttributes; not reimplemented)

// 0x6301f8 _ZNK6glitch7collada15particle_system24CDeflectorForceSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
void CDeflectorForceSceneNode_serializeAttributes(const particle_system::CDeflectorForceSceneNode& self,
                                                  io::IAttributes* attributes,
                                                  io::SAttributeReadWriteOptions* options);

// 0x630318 _ZNK6glitch7collada15particle_system19CWindForceSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
void CWindForceSceneNode_serializeAttributes(const particle_system::CWindForceSceneNode& self,
                                             io::IAttributes* attributes, io::SAttributeReadWriteOptions* options);

// 0x6303f0 _ZNK6glitch7collada15particle_system22CGravityForceSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
void CGravityForceSceneNode_serializeAttributes(const particle_system::CGravityForceSceneNode& self,
                                                io::IAttributes* attributes,
                                                io::SAttributeReadWriteOptions* options);

// 0x630480 _ZN6glitch7collada15particle_system24CDeflectorForceSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
void CDeflectorForceSceneNode_deserializeAttributes(particle_system::CDeflectorForceSceneNode& self,
                                                    io::IAttributes* attributes,
                                                    io::SAttributeReadWriteOptions* options);

// 0x630584 _ZN6glitch7collada15particle_system19CWindForceSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
void CWindForceSceneNode_deserializeAttributes(particle_system::CWindForceSceneNode& self,
                                               io::IAttributes* attributes,
                                               io::SAttributeReadWriteOptions* options);

// 0x630648 _ZN6glitch7collada15particle_system22CGravityForceSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
void CGravityForceSceneNode_deserializeAttributes(particle_system::CGravityForceSceneNode& self,
                                                  io::IAttributes* attributes,
                                                  io::SAttributeReadWriteOptions* options);

// ---- glitch::ps particle systems and render data ------------------------------------

// 0x63e82c _ZN6glitch2ps15IParticleSystemINS0_12GNPSParticleEE6updateEfRNS_5video6SColorE
void IParticleSystem_GNPSParticle_update(ps::IParticleSystem<ps::GNPSParticle>& self, float elapsed,
                                         video::SColor& color);

// 0x6532cc _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE6updateEfRNS_5video6SColorE
void IParticleSystem_SParticle_update(ps::IParticleSystem<ps::SParticle>& self, float elapsed,
                                      video::SColor& color);

// 0x63f898 _ZN6glitch2ps11PRenderData17setRenderDataInfoEPNS_5scene11CMeshBufferEPNS_5video12IVideoDriverE
void PRenderData_setRenderDataInfo(ps::PRenderData& self, scene::CMeshBuffer* buffer, video::IVideoDriver* driver);

// ---- glitch::collada mesh, skinned mesh and buffer preparation ----------------------

// 0x647774 _ZN6glitch7collada19CModularSkinnedMesh27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverEj
std::uint32_t CModularSkinnedMesh_onPrepareBufferForRendering(CModularSkinnedMesh& self, int step,
                                                              video::IVideoDriver* driver, std::uint32_t index);

// 0x649474 _ZN6glitch7collada28CModularSkinnedMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CModularSkinnedMeshSceneNode_deserializeAttributes(CModularSkinnedMeshSceneNode& self,
                                                        io::IAttributes* attributes,
                                                        io::SAttributeReadWriteOptions* options);

// 0x6496ac _ZNK6glitch7collada28CModularSkinnedMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CModularSkinnedMeshSceneNode_serializeAttributes(const CModularSkinnedMeshSceneNode& self,
                                                      io::IAttributes* attributes,
                                                      io::SAttributeReadWriteOptions* options);

// 0x64afec _ZN6glitch7collada13CMorphingMesh27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverEj
std::uint32_t CMorphingMesh_onPrepareBufferForRendering(CMorphingMesh& self, int step, video::IVideoDriver* driver,
                                                        std::uint32_t index);

// 0x658564 _ZN6glitch7collada15CResFileManager23updateExternalResourcesEPNS0_8CResFileEPNS_2io9IReadFileE
void CResFileManager_updateExternalResources(CResFileManager& self, CResFile* file, io::IReadFile* readFile);

// 0x65aecc _ZNK6glitch7collada14CRootSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CRootSceneNode_serializeAttributes(const CRootSceneNode& self, io::IAttributes* attributes,
                                        io::SAttributeReadWriteOptions* options);

// 0x65af0c _ZN6glitch7collada14CRootSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// (thunk to glitch::scene::ISceneNode::deserializeAttributes; not reimplemented)

// 0x65c4f8 _ZN6glitch7collada14CRootSceneNode20setIFLAnimationValueEPNS0_10SAnimationEN5boost13intrusive_ptrINS_5video8ITextureEEE
void CRootSceneNode_setIFLAnimationValue(CRootSceneNode& self, SAnimation* animation,
                                         boost::intrusive_ptr<video::ITexture> texture);

// 0x65c5b0 _ZN6glitch7collada14CRootSceneNode15addIFLAnimationEPNS0_10SAnimationEN5boost13intrusive_ptrINS_5video9CMaterialEEEt
void CRootSceneNode_addIFLAnimation(CRootSceneNode& self, SAnimation* animation,
                                    boost::intrusive_ptr<video::CMaterial> material, std::uint16_t frameCount);

// 0x65c760 _ZN6glitch7collada14CRootSceneNode11addMaterialEPNS0_9SMaterialEPNS_5video12IVideoDriverE
video::CMaterial* CRootSceneNode_addMaterial(CRootSceneNode& self, SMaterial* material, video::IVideoDriver* driver);

// 0x65cafc _ZN6glitch7collada14CRootSceneNode11getMaterialEPNS0_9SMaterialEPNS_5video12IVideoDriverE
video::CMaterial* CRootSceneNode_getMaterial(CRootSceneNode& self, SMaterial* material, video::IVideoDriver* driver);

// 0x65d540 _ZN6glitch7collada18CSceneNodeAnimator21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CSceneNodeAnimator_deserializeAttributes(CSceneNodeAnimator& self, io::IAttributes* attributes,
                                              io::SAttributeReadWriteOptions* options);

// 0x65df4c _ZNK6glitch7collada18CSceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CSceneNodeAnimator_serializeAttributes(const CSceneNodeAnimator& self, io::IAttributes* attributes,
                                            io::SAttributeReadWriteOptions* options);

// 0x665004 _ZN6glitch7collada12CSkinnedMesh27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverEj
std::uint32_t CSkinnedMesh_onPrepareBufferForRendering(CSkinnedMesh& self, int step, video::IVideoDriver* driver,
                                                       std::uint32_t index);

// 0x6655ec _ZN6glitch7collada12CSkinnedMesh12setTransformEPNS_5video12IVideoDriverERKNS_4core8CMatrix4IfEE
int CSkinnedMesh_setTransform(CSkinnedMesh& self, video::IVideoDriver* driver, const core::CMatrix4<float>& matrix);

// 0x6676b8 _ZN6glitch7collada5IMesh27onPrepareBufferForRenderingENS0_21E_PREPARE_BUFFER_STEPEPNS_5video12IVideoDriverEj
std::uint32_t IMesh_onPrepareBufferForRendering(IMesh& self, int step, video::IVideoDriver* driver,
                                                std::uint32_t index);

// 0x6676c8 _ZN6glitch7collada5IMesh12setTransformEPNS_5video12IVideoDriverERKNS_4core8CMatrix4IfEE
int IMesh_setTransform(IMesh& self, video::IVideoDriver* driver, const core::CMatrix4<float>& matrix);

// 0x669b70 _ZN6glitch7collada12ISkinnedMesh12setTransformEPNS_5video12IVideoDriverERKNS_4core8CMatrix4IfEE
int ISkinnedMesh_setTransform(ISkinnedMesh& self, video::IVideoDriver* driver, const core::CMatrix4<float>& matrix);

// ---- glitch::collada::SAnimationAccessor key-frame search ---------------------------

// 0x66ad8c _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiRKNS_3res6vectorIiEEiRiRf
int SAnimationAccessor_findKeyFrameNo(const SAnimationAccessor& self, int a, const res::vector<int>& keys, int b,
                                      int& index, float& fraction);

// 0x66af74 _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiRKNS_3res6vectorIiEEiRi
int SAnimationAccessor_findKeyFrameNo(const SAnimationAccessor& self, int a, const res::vector<int>& keys, int b,
                                      int& index);

}  // namespace glitch::collada::recon_32
