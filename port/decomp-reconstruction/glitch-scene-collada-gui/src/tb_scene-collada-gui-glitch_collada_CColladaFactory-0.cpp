// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CColladaFactory-0 (part 1 of 1).
// Owner: glitch::collada::CColladaFactory. The declarations are already in
// glitch-scene-collada-gui/collada_factory.hpp (slots +0x24, +0x20 and +0x3c), so this file adds bodies only.
//
// All three bodies are PARTIAL. Each one needs a callee or a layout that no header in this category
// declares yet, so the bodies return null and the missing pieces are named below.
#include "glitch-scene-collada-gui/collada_factory.hpp"

namespace glitch::collada {

// 0x631800 glitch::collada::CColladaFactory::createNode(glitch::collada::CColladaDatabase const&, glitch::collada::SNode*)
// Original: operator new(0x160) then CSceneNode::CSceneNode(db, node) on the block, and returns the block.
// PARTIAL: the CSceneNode constructor (C1 0x65d150, C2 0x65d2b4) is ledgered partial and is not declared
// in this category, and CSceneNode has no mapped size, so there is nothing to construct. Returns null.
void* CColladaFactory::createNode(const CColladaDatabase& db, SNode* node) {
    (void)db;
    (void)node;
    return nullptr;
}

// 0x6323d0 glitch::collada::CColladaFactory::createMaterial(glitch::collada::CColladaDatabase const&, glitch::video::IVideoDriver*, glitch::collada::SMaterial*, glitch::collada::CRootSceneNode*)
// The original returns its intrusive_ptr<CMaterial> through a hidden out slot (r0 in the asm, with this in r1).
// The caller takes over the single owning reference, so the value is returned directly here.
// Control flow recorded from the asm, not reproduced:
//   1. root (a6) non-null: CRootSceneNode lookup of the material by name. If a material is already found
//      in the out slot, return it. The asm passes root and the material's first word to a hasMaterial call,
//      which does not match the declared member, so this lookup is missing.
//   2. root null: the out slot is set to null (the early store of a6 to *out).
//   3. The factory's vtable +0x1c createMaterialRenderer(name overload) is called with
//      (db, driver, SMaterial+0x18, SMaterial+0x0c + 1, SMaterial+0x08, root). The SMaterial words at
//      +0x08, +0x0c and +0x18 are not in the stand-in SMaterial (collada_database.hpp), so the call cannot be made.
//   4. A non-null renderer goes to the free glitch::collada::createMaterial(out, db, driver, &renderer, material, root),
//      which is not declared in this category.
// PARTIAL: returns null.
video::CMaterial* CColladaFactory::createMaterial(const CColladaDatabase& db, video::IVideoDriver* driver,
                                                  SMaterial* material, CRootSceneNode* root) {
    (void)db;
    (void)driver;
    (void)material;
    (void)root;
    return nullptr;
}

// 0x634520 glitch::collada::CColladaFactory::createMaterialVertexAttributeMap(glitch::collada::CColladaDatabase const&, glitch::collada::SInstanceMaterial*, boost::intrusive_ptr<glitch::collada::IMesh> const&, boost::intrusive_ptr<glitch::video::CMaterial> const&, unsigned int, bool)
// Original (8732 bytes): returns an intrusive_ptr<CMaterialVertexAttributeMap> through the hidden out slot.
// Control flow recorded from the pseudocode, not reproduced:
//   - instance +0x34 holds the existing map. When it is set and flag is false, the original returns it.
//     SInstanceMaterial in collada_database.hpp has no +0x34 member, so this read is missing.
//   - Otherwise it asks the material's renderer object (CMaterial +0x04, then its +0x04 subobject) for a
//     technique id through the virtual slot at +0x5c, then builds CVertexAttributeMap objects per technique and fills the map. These need
//     CMaterialRenderer::getTechniqueID, CVertexAttributeMap, CMaterialVertexAttributeMap::set/allocate and
//     CVertexStreams, none of which are declared in this category.
//   - linkInstanceMaterial (0x60e28c) is declared in collada_database.hpp and is called when the map is first stored.
// PARTIAL: returns null.
void* CColladaFactory::createMaterialVertexAttributeMap(const CColladaDatabase& db, SInstanceMaterial* instance,
                                                        const boost::intrusive_ptr<IMesh>& mesh,
                                                        const boost::intrusive_ptr<video::CMaterial>& material,
                                                        unsigned int index, bool flag) {
    (void)db;
    (void)instance;
    (void)mesh;
    (void)material;
    (void)index;
    (void)flag;
    return nullptr;
}

}  // namespace glitch::collada
