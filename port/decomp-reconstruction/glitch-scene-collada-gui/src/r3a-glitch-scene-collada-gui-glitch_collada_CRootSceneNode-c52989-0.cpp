// Track B wave r3a, batch r3a-glitch-scene-collada-gui-glitch_collada_CRootSceneNode-c52989-0 (owner glitch::collada::CRootSceneNode), part 1 of 2.
// Only the functions whose bodies need nothing outside the class's own list members are written here. The rest stay
// partial: the class layout in root_scene_node.hpp is a stand-in that does not derive from the canonical scene node
// (layout request: CRootSceneNode : CEmptySceneNode), and the list element types (particle systems, skins, cameras,
// materials, URL records) are void in the stand-in.

#include "glitch-scene-collada-gui/root_scene_node.hpp"

namespace glitch::collada {

// 0x65b000 _ZN6glitch7collada14CRootSceneNode18removeMorphingMeshEPNS0_14CMorphingMeshE
// Walks the morphing-mesh list (sentinel at +0x168); the first node whose value (+0x08) is the mesh is unlinked
// (prev.next = next, next.prev = prev) and freed. Nothing happens when the mesh is not in the list.
void CRootSceneNode::removeMorphingMesh(CMorphingMesh* morphingMesh) {
    SIntrusiveLink<void>* const head = &morphingMeshes.head;
    for (SIntrusiveLink<void>* node = head->next; node != head; node = node->next) {
        if (node->value == static_cast<void*>(morphingMesh)) {
            SIntrusiveLink<void>* next = node->next;
            SIntrusiveLink<void>* prev = node->prev;
            prev->next = next;
            next->prev = prev;
            delete node;
            return;
        }
    }
}

}  // namespace glitch::collada
