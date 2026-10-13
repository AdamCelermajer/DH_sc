// Bodies of glitch::collada::CColladaFactory (canonical class: collada/CColladaFactory.hpp). This file replaces collada_factory.cpp, the
// factory bodies of tb_..._CColladaFactory-0.cpp and the factory stand-ins of recon/slice_17.cpp.
//
// A comment `// 0xADDR symbol` precedes each function; PARTIAL in the comment means the ledger row stays partial.
#include "glitch-scene-collada-gui/collada/CColladaFactory.hpp"

#include "glitch-scene-collada-gui/collada/CBillboardSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CCameraSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CLightSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CMeshSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CModularSkinnedMesh.hpp"
#include "glitch-scene-collada-gui/collada/CRootSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CSceneNode.hpp"
#include "glitch-scene-collada-gui/collada/CSceneNodeAnimator.hpp"
#include "glitch-scene-collada-gui/collada/CSkinnedMesh.hpp"
#include "glitch-scene-collada-gui/collada/IMesh.hpp"
#include "glitch-video/material.hpp"
#include "glitch-video/material_vertex_attribute_map.hpp"

namespace glitch::collada {

namespace {

// The mesh nodes of the three kinds are built over a mesh with no node record, id -1 and the identity transform (the stack temporaries
// of 0x631650 / 0x6316d0 / 0x631750: position 0, rotation (0, 0, 0, 1), scale 1).
struct SIdentityPlacement {
    core::vector3d<float> position{0.0f, 0.0f, 0.0f};
    core::quaternion rotation{0.0f, 0.0f, 0.0f, 1.0f};
    core::vector3d<float> scale{1.0f, 1.0f, 1.0f};
};

}  // namespace

// 0x631b28 _ZN6glitch7collada15CColladaFactory14createAnimatorERKNS0_16CColladaDatabaseEPNS0_22SLibraryAnimationClipsE
void* CColladaFactory::createAnimator(const CColladaDatabase& db, SLibraryAnimationClips* clips) {
    return new CSceneNodeAnimator(db, *clips);  // operator new(0x60)
}

// 0x631af8 _ZN6glitch7collada15CColladaFactory11createLightERKNS0_16CColladaDatabaseEPNS0_6SLightE
CLightSceneNode* CColladaFactory::createLight(const CColladaDatabase& db, SLight* light) {
    return new CLightSceneNode(db, *light);  // operator new(0x16c)
}

// 0x62ff04 _ZN6glitch7collada15CColladaFactory19getAdditionalEffectsERKNS0_16CColladaDatabaseEPNS0_7SEffectERNS0_11SEffectListE
void CColladaFactory::getAdditionalEffects(const CColladaDatabase&, SEffect*, SEffectList&) {}

// 0x631ac4 _ZN6glitch7collada15CColladaFactory13getEffectNameERKNS0_16CColladaDatabaseEPKcS6_
// The result is a std::string built from the last argument (the file name).
std::string CColladaFactory::getEffectName(const CColladaDatabase&, const char*, const char* fileName) {
    return std::string(fileName);
}

// 0x3506a4 _ZN6glitch7collada15CColladaFactory22createMaterialRendererERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_7SEffectEPNS0_14CRootSceneNodeE
video::CMaterialRenderer* CColladaFactory::createMaterialRenderer(const CColladaDatabase&, video::IVideoDriver*, SEffect*,
                                                                  CRootSceneNode*) {
    return nullptr;
}

// 0x636c8c _ZN6glitch7collada15CColladaFactory22createMaterialRendererERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_7SEffectEPKcSA_PNS0_14CRootSceneNodeE
// PARTIAL: with an effect, the original asks slot 0x14 (getEffectName) for the renderer's name, looks it up in the driver's renderer
// collection (the driver's material renderer manager, driver +0xdc) and, when it is missing, builds the list of effects through
// slot 0x10 (getAdditionalEffects) and calls the free function createMaterialRenderer, which is not reconstructed. Without an effect
// it logs "Effect could not be loaded, using pink wireframe." and asks the manager for the pink wireframe renderer; neither the
// manager nor the printer's formatted log is reachable from the port's driver. Null is returned.
video::CMaterialRenderer* CColladaFactory::createMaterialRenderer(const CColladaDatabase&, video::IVideoDriver*, SEffect*, const char*,
                                                                  const char*, CRootSceneNode*) {
    return nullptr;
}

// 0x6323d0 _ZN6glitch7collada15CColladaFactory14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_9SMaterialEPNS0_14CRootSceneNodeE
// PARTIAL: with a root, the material the root already built for the record's name is returned (CRootSceneNode::hasMaterial). Otherwise the
// renderer is created through slot 0x1c (the record's effect, file name and URL), and the material is built over it by the free function
// createMaterial(db, driver, renderer, *record, root), which is not reconstructed. Null is returned when the root has none.
video::CMaterial* CColladaFactory::createMaterial(const CColladaDatabase&, video::IVideoDriver*, SMaterial* material,
                                                  CRootSceneNode* root) {
    if (root != nullptr) {
        boost::intrusive_ptr<video::CMaterial> existing = root->hasMaterial(material->name);
        if (existing) {
            return existing.detach();
        }
    }
    return nullptr;
}

// 0x634520 _ZN6glitch7collada15CColladaFactory29createMaterialVertexAttributeMapERKNS0_16CColladaDatabaseEPNS0_17SInstanceMaterialERKN5boost13intrusive_ptrINS0_5IMeshEEERKNS8_INS_5video9CMaterialEEEjb
// PARTIAL: with the instance's map set and `flag` clear the existing map is returned. Otherwise the original asks the material's renderer for
// its technique (CMaterialRenderer slot +0x5c), builds one CVertexAttributeMap per technique over the mesh buffer's streams and links the
// instance (linkInstanceMaterial); the technique table and the vertex-attribute-map builder are not reachable from the port's video
// classes, so null is returned.
boost::intrusive_ptr<video::CMaterialVertexAttributeMap> CColladaFactory::createMaterialVertexAttributeMap(
    const CColladaDatabase&, SInstanceMaterial* instance, const boost::intrusive_ptr<IMesh>&,
    const boost::intrusive_ptr<video::CMaterial>&, unsigned int, bool flag) {
    if (instance != nullptr && instance->vertexAttributeMap != nullptr && !flag) {
        return boost::intrusive_ptr<video::CMaterialVertexAttributeMap>(instance->vertexAttributeMap);
    }
    return boost::intrusive_ptr<video::CMaterialVertexAttributeMap>();
}

// 0x62ff08 _ZN6glitch7collada15CColladaFactory21getVertexBufferConfigERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
SBufferConfig CColladaFactory::getVertexBufferConfig(const CColladaDatabase&, SGeometry*) {
    return SBufferConfig{4, 0, 0};
}

// 0x62ff20 _ZN6glitch7collada15CColladaFactory20getIndexBufferConfigERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
SBufferConfig CColladaFactory::getIndexBufferConfig(const CColladaDatabase&, SGeometry*) {
    return SBufferConfig{4, 0, 0};
}

// 0x62ff38 _ZN6glitch7collada15CColladaFactory20isSharingMeshBuffersERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
bool CColladaFactory::isSharingMeshBuffers(const CColladaDatabase&, SGeometry*) {
    return true;
}

// 0x631924 _ZN6glitch7collada15CColladaFactory14createGeometryERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_9SGeometryE
// The vertex and index configurations come from slots 0x28 and 0x2c and the sharing flag from slot 0x30; the mesh (operator new(0x3c))
// is born with count 0 and the result carries one reference.
IMesh* CColladaFactory::createGeometry(const CColladaDatabase& db, video::IVideoDriver* driver, SGeometry* geometry) {
    const SBufferConfig vertexConfig = getVertexBufferConfig(db, geometry);
    const SBufferConfig indexConfig = getIndexBufferConfig(db, geometry);
    const bool sharing = isSharingMeshBuffers(db, geometry);
    CMesh* mesh = new CMesh(db, driver, *geometry, vertexConfig, indexConfig, sharing);
    ++mesh->referenceCounter;
    return mesh;
}

// 0x6318c8 _ZN6glitch7collada15CColladaFactory11createMorphERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_11SControllerEPNS0_14CRootSceneNodeE
CMorphingMesh* CColladaFactory::createMorph(const CColladaDatabase& db, video::IVideoDriver* driver, SController* controller,
                                            CRootSceneNode* root) {
    CMorphingMesh* morph = new CMorphingMesh(db, driver, *controller, root);  // operator new(0x40)
    ++morph->referenceCounter;
    return morph;
}

// 0x631800 _ZN6glitch7collada15CColladaFactory10createNodeERKNS0_16CColladaDatabaseEPNS0_5SNodeE
CSceneNode* CColladaFactory::createNode(const CColladaDatabase& db, SNode* node) {
    return new CSceneNode(db, node);  // operator new(0x160)
}

// 0x631830 _ZN6glitch7collada15CColladaFactory15createBillboardERKNS0_16CColladaDatabaseEPNS0_5SNodeE
void* CColladaFactory::createBillboard(const CColladaDatabase& db, SNode* node) {
    return static_cast<scene::ISceneNode*>(new CBillboardSceneNode(db, node));  // operator new(0x160)
}

// 0x6317d0 _ZN6glitch7collada15CColladaFactory16createCameraNodeERKNS0_16CColladaDatabaseEPNS0_7SCameraE
CCameraSceneNode* CColladaFactory::createCameraNode(const CColladaDatabase& db, SCamera* camera) {
    return new CCameraSceneNode(db, *camera);  // operator new(0x3a0)
}

// 0x631750 _ZN6glitch7collada15CColladaFactory14createMeshNodeERKNS0_16CColladaDatabaseERKN5boost13intrusive_ptrINS0_5IMeshEEEPv
void* CColladaFactory::createMeshNode(const CColladaDatabase&, const boost::intrusive_ptr<IMesh>& mesh, void*) {
    SIdentityPlacement placement;
    return static_cast<scene::ISceneNode*>(
        new CMeshSceneNode(mesh, nullptr, -1, placement.position, placement.rotation, placement.scale));  // operator new(320)
}

// 0x6316d0 _ZN6glitch7collada15CColladaFactory14createSkinNodeERKNS0_16CColladaDatabaseERKN5boost13intrusive_ptrINS0_5IMeshEEEPv
void* CColladaFactory::createSkinNode(const CColladaDatabase&, const boost::intrusive_ptr<IMesh>& mesh, void*) {
    SIdentityPlacement placement;
    return static_cast<scene::ISceneNode*>(
        new CSkinnedMeshSceneNode(mesh, nullptr, -1, placement.position, placement.rotation, placement.scale));  // operator new(0x184)
}

// 0x631650 _ZN6glitch7collada15CColladaFactory19createModularSkinNodeERKNS0_16CColladaDatabaseERKN5boost13intrusive_ptrINS0_5IMeshEEEPv
void* CColladaFactory::createModularSkinNode(const CColladaDatabase&, const boost::intrusive_ptr<IMesh>& mesh, void*) {
    SIdentityPlacement placement;
    return static_cast<scene::ISceneNode*>(new CModularSkinnedMeshSceneNode(mesh, nullptr, -1, placement.position, placement.rotation,
                                                                            placement.scale));  // operator new(0x184)
}

// 0x631628 _ZN6glitch7collada15CColladaFactory11createSceneERKNS0_16CColladaDatabaseE
void* CColladaFactory::createScene(const CColladaDatabase& db) {
    return static_cast<scene::ISceneNode*>(new CRootSceneNode(db));  // operator new(0x1c4)
}

// 0x6315cc _ZN6glitch7collada15CColladaFactory10createSkinERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_11SControllerEPNS0_14CRootSceneNodeE
CSkinnedMesh* CColladaFactory::createSkin(const CColladaDatabase& db, video::IVideoDriver* driver, SController* controller,
                                          CRootSceneNode* root) {
    CSkinnedMesh* skin = new CSkinnedMesh(db, driver, *controller, root);  // operator new(0x9c)
    ++skin->referenceCounter;
    return skin;
}

// 0x631560 _ZN6glitch7collada15CColladaFactory16createModularSkinERKNS0_16CColladaDatabaseEPNS0_20SInstanceModularSkinEPNS0_14CRootSceneNodeE
CModularSkinnedMesh* CColladaFactory::createModularSkin(const CColladaDatabase& db, SInstanceModularSkin* instance,
                                                        CRootSceneNode* root) {
    CModularSkinnedMesh* skin = new CModularSkinnedMesh(db, instance, root, -1, true);  // operator new(0x5c)
    ++skin->referenceCounter;
    return skin;
}

// 0x631518 _ZN6glitch7collada15CColladaFactory13createCoronasERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_8SCoronasEPNS0_14CRootSceneNodeE
// PARTIAL: the original is operator new(0x20c) and CCoronasSceneNode(db, driver, *coronas, root); CCoronasSceneNode is not canonical.
CCoronasSceneNode* CColladaFactory::createCoronas(const CColladaDatabase&, video::IVideoDriver*, SCoronas*, CRootSceneNode*) {
    return nullptr;
}

// 0x63133c _ZN6glitch7collada15CColladaFactory26createGlitchNewParticleSystemERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_12SGNPSEmitterEPNS_3res6vectorINSB_6StringEEEPNS0_14CRootSceneNodeE
// PARTIAL: operator new(0x198), CGlitchNewParticleSystemSceneNode(driver, materials, root, ...) and the emitter set-up (slot 0xf4 and the
// axis locks of the context); the particle system node is not canonical.
CGlitchNewParticleSystemSceneNode* CColladaFactory::createGlitchNewParticleSystem(const CColladaDatabase&, video::IVideoDriver*,
                                                                                  SGNPSEmitter*, res::vector<res::String>*,
                                                                                  CRootSceneNode*) {
    return nullptr;
}

// 0x634c64 _ZN6glitch7collada15CColladaFactory20createParticleSystemERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_8SEmitterEPNS_3res6vectorINSB_6StringEEEPNS0_14CRootSceneNodeE
// PARTIAL: 2864 bytes that build CParticleSystemSceneNode from the emitter record; the node is not canonical.
CParticleSystemSceneNode* CColladaFactory::createParticleSystem(const CColladaDatabase&, video::IVideoDriver*, SEmitter*,
                                                                res::vector<res::String>*, CRootSceneNode*) {
    return nullptr;
}

// 0x631274, 0x631244, 0x631138. PARTIAL: operator new(0x158 / 0x164 / 0x168) and the force node constructors; the force nodes are not
// canonical.
void* CColladaFactory::createParticleSystemGravityForce(const CColladaDatabase&, SForce*) {
    return nullptr;
}
void* CColladaFactory::createParticleSystemWindForce(const CColladaDatabase&, SForce*) {
    return nullptr;
}
void* CColladaFactory::createParticleSystemDeflectorForce(const CColladaDatabase&, SForce*) {
    return nullptr;
}

// 0x630010 _ZN6glitch7collada15CColladaFactory25getExternalLightSceneNodeERKNS0_16CColladaDatabaseERKN5boost13intrusive_ptrIKNS_5video9CMaterialEEEtjPKc
// 0x630008 (the renderer form). Both are 8-byte tail calls of getExternalLightSceneNodeDefault (0x62ff80): the node named by the URL
// (a leading '#' skipped) in the scene manager (CResFileManager::Inst->device, IDevice +0x1c, slot +0x14), accepted when its type is
// 'lght', else null. PARTIAL: IDevice is not mapped in glitch-core-io (no scene manager member), so null is returned.
scene::CLightSceneNode* CColladaFactory::getExternalLightSceneNode(const CColladaDatabase&,
                                                                   const boost::intrusive_ptr<video::CMaterial const>&, std::uint16_t,
                                                                   std::uint32_t, const char*) {
    return nullptr;
}
scene::CLightSceneNode* CColladaFactory::getExternalLightSceneNode(const CColladaDatabase&,
                                                                   const boost::intrusive_ptr<video::CMaterialRenderer const>&,
                                                                   std::uint16_t, std::uint32_t, const char*) {
    return nullptr;
}

}  // namespace glitch::collada
