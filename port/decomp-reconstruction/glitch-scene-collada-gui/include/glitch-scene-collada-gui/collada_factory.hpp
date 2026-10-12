// glitch::collada::CColladaFactory: the virtual builder that CColladaDatabase
// calls to turn named COLLADA entries into scene objects. The slots below are
// listed in the original vtable order; the byte offset of each slot in the
// original vtable is given in its comment, and the order is what keeps the
// `(*factory + 0xNN)` calls in the database code correct.
//
// The concrete factory (default bodies, particle-force and mesh builders) is
// not part of this header. Return values that the original hands back through
// a hidden intrusive_ptr out-parameter are returned directly here: the caller
// takes over the single owning reference the factory produced.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/collada_database.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch::collada {

class CRootSceneNode;
class CSkinnedMesh;
class CMorphingMesh;
class CModularSkinnedMesh;
class CLightSceneNode;
class CCameraSceneNode;
class CCoronasSceneNode;
class CParticleSystemSceneNode;
class CGlitchNewParticleSystemSceneNode;
class IMesh;
struct SEffectList;
struct SLibraryAnimationClips;
struct SNode;
struct SInstanceModularSkin;
struct SInstanceMaterial;

class CColladaFactory {
public:
    virtual ~CColladaFactory() = default;  // +0x00, +0x04

    virtual void* createAnimator(const CColladaDatabase& db, SLibraryAnimationClips* clips) = 0;  // +0x08
    virtual void* createLight(const CColladaDatabase& db, SLight* light) = 0;                     // +0x0c
    virtual void getAdditionalEffects(const CColladaDatabase& db, SEffect* effect, SEffectList& list) = 0;  // +0x10
    virtual const char* getEffectName(const CColladaDatabase& db, const char* name, const char* fileName) = 0;  // +0x14
    virtual video::CMaterialRenderer* createMaterialRenderer(const CColladaDatabase& db, video::IVideoDriver* driver,
                                                             SEffect* effect, CRootSceneNode* root) = 0;  // +0x18
    virtual video::CMaterialRenderer* createMaterialRenderer(const CColladaDatabase& db, video::IVideoDriver* driver,
                                                             SEffect* effect, const char* name, const char* fileName,
                                                             CRootSceneNode* root) = 0;  // +0x1c
    virtual video::CMaterial* createMaterial(const CColladaDatabase& db, video::IVideoDriver* driver,
                                             SMaterial* material, CRootSceneNode* root) = 0;  // +0x20
    virtual void* createMaterialVertexAttributeMap(const CColladaDatabase& db, SInstanceMaterial* instance,
                                                   const boost::intrusive_ptr<IMesh>& mesh,
                                                   const boost::intrusive_ptr<video::CMaterial>& material,
                                                   unsigned int index, bool flag) = 0;  // +0x24
    virtual std::uint32_t getVertexBufferConfig(const CColladaDatabase& db, SGeometry* geometry) = 0;  // +0x28
    virtual std::uint32_t getIndexBufferConfig(const CColladaDatabase& db, SGeometry* geometry) = 0;   // +0x2c
    virtual bool isSharingMeshBuffers(const CColladaDatabase& db, SGeometry* geometry) = 0;              // +0x30
    virtual IMesh* createGeometry(const CColladaDatabase& db, video::IVideoDriver* driver, SGeometry* geometry) = 0;  // +0x34
    virtual CMorphingMesh* createMorph(const CColladaDatabase& db, video::IVideoDriver* driver, SController* controller,
                                       CRootSceneNode* root) = 0;  // +0x38
    virtual void* createNode(const CColladaDatabase& db, SNode* node) = 0;                // +0x3c
    virtual void* createBillboard(const CColladaDatabase& db, SNode* node) = 0;           // +0x40
    virtual CCameraSceneNode* createCameraNode(const CColladaDatabase& db, SCamera* camera) = 0;  // +0x44
    virtual void* createMeshNode(const CColladaDatabase& db, const boost::intrusive_ptr<IMesh>& mesh, void* instance) = 0;  // +0x48
    virtual void* createSkinNode(const CColladaDatabase& db, const boost::intrusive_ptr<IMesh>& mesh, void* instance) = 0;  // +0x4c
    virtual void* createModularSkinNode(const CColladaDatabase& db, const boost::intrusive_ptr<IMesh>& mesh,
                                        void* instance) = 0;                                       // +0x50
    virtual void* createScene(const CColladaDatabase& db) = 0;                                    // +0x54
    virtual CSkinnedMesh* createSkin(const CColladaDatabase& db, video::IVideoDriver* driver, SController* controller,
                                     CRootSceneNode* root) = 0;                                    // +0x58
    virtual CModularSkinnedMesh* createModularSkin(const CColladaDatabase& db, SInstanceModularSkin* instance,
                                                   CRootSceneNode* root) = 0;                      // +0x5c
    virtual CCoronasSceneNode* createCoronas(const CColladaDatabase& db, video::IVideoDriver* driver, SCoronas* coronas,
                                             CRootSceneNode* root) = 0;                            // +0x60
    virtual CGlitchNewParticleSystemSceneNode* createGlitchNewParticleSystem(
        const CColladaDatabase& db, video::IVideoDriver* driver, SGNPSEmitter* emitter,
        res::vector<res::String>* materials, CRootSceneNode* root) = 0;                            // +0x64
    virtual CParticleSystemSceneNode* createParticleSystem(const CColladaDatabase& db, video::IVideoDriver* driver,
                                                           SEmitter* emitter, res::vector<res::String>* materials,
                                                           CRootSceneNode* root) = 0;              // +0x68
    virtual void* createParticleSystemGravityForce(const CColladaDatabase& db, SForce* force) = 0;  // +0x6c
    virtual void* createParticleSystemWindForce(const CColladaDatabase& db, SForce* force) = 0;     // +0x70
    virtual void* createParticleSystemDeflectorForce(const CColladaDatabase& db, SForce* force) = 0;  // +0x74
    virtual void getExternalLightSceneNode(const CColladaDatabase& db,
                                           const boost::intrusive_ptr<video::CMaterial const>& material,
                                           std::uint16_t a, std::uint32_t b, const char* name) = 0;  // +0x78 (base body in collada_factory.cpp)
    virtual void getExternalLightSceneNode(const CColladaDatabase& db,
                                           const boost::intrusive_ptr<video::CMaterialRenderer const>& renderer,
                                           std::uint16_t a, std::uint32_t b, const char* name) = 0;  // +0x7c (base body in collada_factory.cpp)
};

}  // namespace glitch::collada
