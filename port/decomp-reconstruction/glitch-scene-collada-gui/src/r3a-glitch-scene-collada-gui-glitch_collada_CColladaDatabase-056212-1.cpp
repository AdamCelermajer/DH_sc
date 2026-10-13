// Track B redo batch r3a-glitch-scene-collada-gui-glitch_collada_CColladaDatabase-056212-1 (part 2 of 4).
// Owner: glitch::collada::CColladaDatabase. The name overloads of the scene-object builders (reconstructed), and the instance-record
// overloads (partial: the instance layout and the material helpers they call are not declared in the canonical headers).
// The C2 constructor 0x619344 has the same body as 0x6192f0 (r3a batch -056212-0) and is not repeated here.
// Not in this file (see the ledger): the destructors 0x619474 / 0x619784 (no destructor is declared: the canonical class is released
// by dropDatabase, and a declared destructor would release twice), the constructAnimator loaders 0x619510 / 0x6195c8 / 0x61967c
// (static in the original, DefaultFactory not declared), getMaterial(file, name) 0x61ac88 (needs DefaultFactory), and the
// constructController / constructGeometry / constructNode instance overloads, which are completed in the e2 batch file.

#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"
#include "glitch-scene-collada-gui/collada_factory.hpp"

namespace glitch::collada {

// 0x61a5d4 glitch::collada::CColladaDatabase::constructGNPSEmitter(char const*, glitch::video::IVideoDriver*, glitch::res::vector<glitch::res::String>*, glitch::collada::CRootSceneNode*) const
CGlitchNewParticleSystemSceneNode* CColladaDatabase::constructGNPSEmitter(const char* name, video::IVideoDriver* driver,
                                                                          res::vector<res::String>* materials,
                                                                          CRootSceneNode* root) const {
    return constructGNPSEmitter(getGNPSEmitter(tables(), name), driver, materials, root);
}

// 0x61a750 glitch::collada::CColladaDatabase::constructCoronas(char const*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const
CCoronasSceneNode* CColladaDatabase::constructCoronas(const char* name, video::IVideoDriver* driver,
                                                      CRootSceneNode* root) const {
    return constructCoronas(getCoronas(tables(), name), driver, root);
}

// 0x61a854 glitch::collada::CColladaDatabase::constructEmitter(char const*, glitch::video::IVideoDriver*, glitch::res::vector<glitch::res::String>*, glitch::collada::CRootSceneNode*) const
CParticleSystemSceneNode* CColladaDatabase::constructEmitter(const char* name, video::IVideoDriver* driver,
                                                             res::vector<res::String>* materials,
                                                             CRootSceneNode* root) const {
    return constructEmitter(getEmitter(tables(), name), driver, materials, root);
}

// 0x61aa00 glitch::collada::CColladaDatabase::constructController(glitch::video::IVideoDriver*, char const*, glitch::collada::CRootSceneNode*) const
// The result is partial through the SController overload (0x60fa24), which is partial.
glitch::IReferenceCounted** CColladaDatabase::constructController(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                                  const char* name, CRootSceneNode* root) const {
    return constructController(out, driver, getController(tables(), name), root);
}

// 0x61b1b4 glitch::collada::CColladaDatabase::constructImage(char const*, glitch::collada::CRootSceneNode*) const
// The result is partial through the SImage overload (0x60fc70), which is partial.
glitch::IReferenceCounted** CColladaDatabase::constructImage(glitch::IReferenceCounted** out, const char* name,
                                                             CRootSceneNode* root) const {
    return constructImage(out, getImage(tables(), name), root);
}

// 0x61a608 glitch::collada::CColladaDatabase::constructGNPSEmitter(glitch::collada::SInstanceGNPSEmitter*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const
// PARTIAL: the original builds the emitter by the name in the instance (URL word at +0x04, the leading '#' skipped), then adds the materials
// of the instance (count at +0x0c, bindings at +0x10 with stride 0x3c, material name at binding +0x08) through getMaterial and
// IParticleSystemSceneNode::addMaterial. The instance layout is not in the canonical headers and addMaterial is not declared, so the
// result is null here.
CGlitchNewParticleSystemSceneNode* CColladaDatabase::constructGNPSEmitter(SInstanceGNPSEmitter* instance, video::IVideoDriver* driver,
                                                                          CRootSceneNode* root) const {
    (void)instance;
    (void)driver;
    (void)root;
    return nullptr;
}

// 0x61a77c glitch::collada::CColladaDatabase::constructCoronas(glitch::collada::SInstanceCoronas*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const
// PARTIAL: as the GNPS instance overload. The instance layout and CCoronasSceneNode::setMaterial(material, driver) (recon/slice_32.cpp,
// partial) are not usable, so the result is null here.
CCoronasSceneNode* CColladaDatabase::constructCoronas(SInstanceCoronas* instance, video::IVideoDriver* driver,
                                                      CRootSceneNode* root) const {
    (void)instance;
    (void)driver;
    (void)root;
    return nullptr;
}

// 0x61a888 glitch::collada::CColladaDatabase::constructEmitter(glitch::collada::SInstanceEmitter*, glitch::video::IVideoDriver*, glitch::collada::CRootSceneNode*) const
// PARTIAL: as the GNPS instance overload (the material loop needs IParticleSystemSceneNode::addMaterial, which is not declared). Null here.
CParticleSystemSceneNode* CColladaDatabase::constructEmitter(SInstanceEmitter* instance, video::IVideoDriver* driver,
                                                             CRootSceneNode* root) const {
    (void)instance;
    (void)driver;
    (void)root;
    return nullptr;
}

// 0x61aae0 glitch::collada::CColladaDatabase::constructGeometry(glitch::video::IVideoDriver*, char const*, char const*) const
// PARTIAL: the original opens `fileName` next to this database's file (CResFileManager::get with flag 1), builds a database over that record
// with this factory, and constructs `geometryName` from it. On failure it logs "File not found" / "Geometry not found" through
// glitch::os::Printer::log, which is not declared in the port. The out slot is left null.
glitch::IReferenceCounted** CColladaDatabase::constructGeometry(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                                const char* fileName, const char* geometryName) const {
    (void)driver;
    (void)fileName;
    (void)geometryName;
    *out = nullptr;
    return out;
}

// 0x61b8bc glitch::collada::CColladaDatabase::constructVisualScene(glitch::video::IVideoDriver*, glitch::collada::SVisualScene*, glitch::collada::CRootSceneNode*) const
// PARTIAL: the original returns null without a scene, uses the given root or the factory's createScene (slot +0x54), sets the root from
// the scene's reserved word through root slot +0x28, and attaches each top-level node built by constructNode (count +0x08, base +0x0c,
// stride 0x50) through root slot +0x5c, dropping its reference. The root's virtual slots are not named in root_scene_node.hpp, so the
// attach and the set-up are not called and the scene is returned unbuilt.
CRootSceneNode* CColladaDatabase::constructVisualScene(video::IVideoDriver* driver, SVisualScene* scene,
                                                       CRootSceneNode* root) const {
    (void)driver;
    if (scene == nullptr) {
        return nullptr;
    }
    return root;
}

}  // namespace glitch::collada
