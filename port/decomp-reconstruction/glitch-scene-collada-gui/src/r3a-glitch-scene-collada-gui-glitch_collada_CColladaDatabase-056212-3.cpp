// Track B redo batch r3a-glitch-scene-collada-gui-glitch_collada_CColladaDatabase-056212-3 (part 4 of 4).
// Owner: glitch::collada::CColladaDatabase. The index overload of constructMaterial.

#include "glitch-scene-collada-gui/collada/CColladaDatabase.hpp"
#include "glitch-scene-collada-gui/collada_factory.hpp"

namespace glitch::collada {

// 0x61cf44 glitch::collada::CColladaDatabase::constructMaterial(glitch::video::IVideoDriver*, int, glitch::collada::CRootSceneNode*) const
// The material is the table entry at `index` (the material getter, materialAt), built by the SMaterial overload (0x61cca8, partial).
glitch::IReferenceCounted** CColladaDatabase::constructMaterial(glitch::IReferenceCounted** out, video::IVideoDriver* driver,
                                                                int index, CRootSceneNode* root) const {
    return constructMaterial(out, driver, materialAt(tables(), index), root);
}

}  // namespace glitch::collada
