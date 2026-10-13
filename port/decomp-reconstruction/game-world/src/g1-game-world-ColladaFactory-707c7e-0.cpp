// Batch g1-game-world-ColladaFactory-707c7e-0 (part 1 of 1): the thirteen ColladaFactory functions. Declarations come from
// dh2/ColladaFactory.hpp; every override is already declared there in vtable order, so the header needed no change.
//
// Forwarders. createMaterial, createSkin, createMorph, createGeometry and createParticleSystem call the engine base
// glitch::collada::CColladaFactory with the same arguments and return the base result. In the binary the hidden result
// pointer (r0) is passed through unchanged and handed back, so the forwarded value equals the base value.
//
// Node builders. Each allocates its object with operator new and runs its constructor. This tree allocates with new, as the
// other game-world sources do, so the operator new(N) sizes of the original are noted in comments only.

#include "dh2/Animator.hpp"
#include "dh2/ColladaFactory.hpp"
#include "dh2/MeshSceneNode.hpp"
#include "dh2/ModularSkinnedMeshSceneNode.hpp"
#include "dh2/RootSceneNode.hpp"

// 0x3506ac _ZN14ColladaFactoryD1Ev
// Complete-object destructor (D1; D2 is the same code). The vptr reset to the ColladaFactory table and the call of
// glitch::collada::CColladaFactory::~CColladaFactory are made by the compiler from the virtual destructor.
ColladaFactory::~ColladaFactory() {}

// 0x3506e0 _ZN14ColladaFactory14createMaterialERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_9SMaterialEPNS1_14CRootSceneNodeE
// Forward to glitch::collada::CColladaFactory::createMaterial, with the arguments and the returned value passed through.
glitch::video::CMaterial* ColladaFactory::createMaterial(const glitch::collada::CColladaDatabase& db,
                                                         glitch::video::IVideoDriver* driver,
                                                         glitch::collada::SMaterial* material,
                                                         glitch::collada::CRootSceneNode* root) {
    return glitch::collada::CColladaFactory::createMaterial(db, driver, material, root);
}

// 0x35070c _ZN14ColladaFactory14createAnimatorERKN6glitch7collada16CColladaDatabaseEPNS1_22SLibraryAnimationClipsE
// operator new(156) and the Animator constructor (db, clips), then the object is returned.
void* ColladaFactory::createAnimator(const glitch::collada::CColladaDatabase& db,
                                     glitch::collada::SLibraryAnimationClips* clips) {
    return new Animator(db, *clips);  // operator new(156) in the original
}

// 0x35073c _ZN14ColladaFactory10createSkinERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_11SControllerEPNS1_14CRootSceneNodeE
// Forward to glitch::collada::CColladaFactory::createSkin, with the arguments and the returned value passed through.
glitch::collada::CSkinnedMesh* ColladaFactory::createSkin(const glitch::collada::CColladaDatabase& db,
                                                          glitch::video::IVideoDriver* driver,
                                                          glitch::collada::SController* controller,
                                                          glitch::collada::CRootSceneNode* root) {
    return glitch::collada::CColladaFactory::createSkin(db, driver, controller, root);
}

// 0x350768 _ZN14ColladaFactory11createMorphERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_11SControllerEPNS1_14CRootSceneNodeE
// Forward to glitch::collada::CColladaFactory::createMorph, with the arguments and the returned value passed through.
glitch::collada::CMorphingMesh* ColladaFactory::createMorph(const glitch::collada::CColladaDatabase& db,
                                                            glitch::video::IVideoDriver* driver,
                                                            glitch::collada::SController* controller,
                                                            glitch::collada::CRootSceneNode* root) {
    return glitch::collada::CColladaFactory::createMorph(db, driver, controller, root);
}

// 0x350794 _ZN14ColladaFactory14createGeometryERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_9SGeometryE
// Forward to glitch::collada::CColladaFactory::createGeometry, with the arguments and the returned value passed through.
glitch::collada::IMesh* ColladaFactory::createGeometry(const glitch::collada::CColladaDatabase& db,
                                                       glitch::video::IVideoDriver* driver,
                                                       glitch::collada::SGeometry* geometry) {
    return glitch::collada::CColladaFactory::createGeometry(db, driver, geometry);
}

// 0x3507b8 _ZN14ColladaFactory20createParticleSystemERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_8SEmitterEPNS0_3res6vectorINSA_6StringEEEPNS1_14CRootSceneNodeE
// Thunk: a single branch to glitch::collada::CColladaFactory::createParticleSystem. Forwarded, not reimplemented.
glitch::collada::CParticleSystemSceneNode* ColladaFactory::createParticleSystem(
    const glitch::collada::CColladaDatabase& db, glitch::video::IVideoDriver* driver,
    glitch::collada::SEmitter* emitter, glitch::res::vector<glitch::res::String>* materials,
    glitch::collada::CRootSceneNode* root) {
    return glitch::collada::CColladaFactory::createParticleSystem(db, driver, emitter, materials, root);
}

// 0x3507bc _ZN14ColladaFactory15createBillboardERKN6glitch7collada16CColladaDatabaseEPNS1_5SNodeE
// PARTIAL: not rebuilt, returns null. The original allocates 352 bytes (operator new(0x160)) and builds a
// glitch::collada::CBillboardSceneNode in place. It sets the vptrs of the construction tables, sets the count of the
// IReferenceCounted subobject at +0x158 to 1 (+0x15c), calls glitch::collada::CSceneNode::CSceneNode(db, node) on the
// object, and stores the final vptrs (primary at +0x00, secondary at +0x158). A virtual base
// glitch::io::IAttributeExchangingObject is placed through an offset read from the vtable. This tree has no declaration
// of glitch::collada::CBillboardSceneNode: the only CBillboardSceneNode header (glitch-scene-collada-gui/scene_nodes.hpp)
// is glitch::scene::CBillboardSceneNode, a different class. Waits on that layout (layout request below).
void* ColladaFactory::createBillboard(const glitch::collada::CColladaDatabase& db, glitch::collada::SNode* node) {
    (void)db;
    (void)node;
    return nullptr;
}

// 0x350854 _ZN14ColladaFactory21createModularSkinNodeERKN6glitch7collada16CColladaDatabaseERKN5boost13intrusive_ptrINS1_5IMeshEEEPv
// operator new(396) and the ModularSkinnedMeshSceneNode constructor on the mesh, then the object is returned. The db and
// instance arguments are not read by the original.
void* ColladaFactory::createModularSkinNode(const glitch::collada::CColladaDatabase& db,
                                            const boost::intrusive_ptr<glitch::collada::IMesh>& mesh,
                                            void* instance) {
    (void)db;
    (void)instance;
    return new ModularSkinnedMeshSceneNode(mesh);  // operator new(396) in the original
}

// 0x35087c _ZN14ColladaFactory14createSkinNodeERKN6glitch7collada16CColladaDatabaseERKN5boost13intrusive_ptrINS1_5IMeshEEEPv
// PARTIAL: not rebuilt, returns null. The original allocates 396 bytes (operator new(396)) and runs the
// SkinnedMeshSceneNode constructor on the mesh. dh2/SkinnedMeshSceneNode.hpp does not compile on its own:
// BaseMeshSceneNode<glitch::collada::CSkinnedMeshSceneNode> needs the complete engine class, which exists only as recon
// stand-ins (glitch-scene-collada-gui/recon/slice_22.hpp), and the two override slots then have nothing to override.
// The header is not touched here and not included, so this file compiles. Waits on dh2/SkinnedMeshSceneNode.hpp.
void* ColladaFactory::createSkinNode(const glitch::collada::CColladaDatabase& db,
                                     const boost::intrusive_ptr<glitch::collada::IMesh>& mesh,
                                     void* instance) {
    (void)db;
    (void)mesh;
    (void)instance;
    return nullptr;
}

// 0x3508a4 _ZN14ColladaFactory14createMeshNodeERKN6glitch7collada16CColladaDatabaseERKN5boost13intrusive_ptrINS1_5IMeshEEEPv
// operator new(324) and the MeshSceneNode constructor on the mesh, then the object is returned. The db and instance
// arguments are not read by the original.
void* ColladaFactory::createMeshNode(const glitch::collada::CColladaDatabase& db,
                                     const boost::intrusive_ptr<glitch::collada::IMesh>& mesh,
                                     void* instance) {
    (void)db;
    (void)instance;
    return new MeshSceneNode(mesh);  // operator new(324) in the original
}

// 0x3508cc _ZN14ColladaFactory11createSceneERKN6glitch7collada16CColladaDatabaseE
// operator new(532) and the RootSceneNode constructor on the database, then the object is returned.
void* ColladaFactory::createScene(const glitch::collada::CColladaDatabase& db) {
    return new RootSceneNode(db);  // operator new(532) in the original
}

// 0x350a10 _ZN14ColladaFactoryD0Ev
// skipped: the compiler generates the deleting destructor from the virtual ~ColladaFactory() in dh2/ColladaFactory.hpp.
// The original is the D1 body above (vptr reset, base destructor), then CustomFree(this), which is operator delete here.
