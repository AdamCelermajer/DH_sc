// Recon v2 wave 02: partial-row rerun for glitch::scene::CBatchMesh / CBatchSceneNode and
// the glitch::gui IGUIElement tab, toolbar and table family.
//
// Layouts used: layouts/scene.hpp (CBatchMesh, IMesh, CMeshBuffer) and layouts/gui.hpp
// (IGUIElement). Core value types come from deps/core_stand_ins.hpp (no shared header
// exists yet under port/decomp-reconstruction/shared/).
//
// Pointer words in the scene layouts are 32-bit target words. targetPtr() turns one
// into a host pointer; the test allocates such targets below 4 GiB.
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_02_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_02_HPP

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/layouts/gui.hpp"
#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::collada::recon_v2_02 {

template <typename T>
inline T* targetPtr(std::uint32_t word) {
    return reinterpret_cast<T*>(static_cast<std::uintptr_t>(word));
}

// glitch::scene::CBatchSceneNode. The layout header declares no members for it. The only
// word this wave reads is the mesh target at +0x130 (getMesh loads it, getMaterial calls
// through it), so that word is the only one declared here.
struct CBatchSceneNodeView {
    std::uint8_t pad_0x000[0x130];  // +0x000..+0x12f not confirmed
    std::uint32_t mesh;             // +0x130 IMesh target pointer (refcounted at IMesh +0x04)
};
static_assert(offsetof(CBatchSceneNodeView, mesh) == 0x130, "CBatchSceneNodeView::mesh");

// Tail of an extern the wave calls but does not own. Bodies are in other waves or unmapped.
// IGUIElement::updateAbsolutePosition(void), base of CGUIToolBar::updateAbsolutePosition.
int IGUIElement_updateAbsolutePosition(gui::IGUIElement* self);
// IGUIElement::getNextElement(int, bool, IGUIElement*&, IGUIElement*&, bool). Its body is
// partial (vtable slot +0x2c is unmapped), so it is declared only.
int IGUIElement_getNextElement(gui::IGUIElement* self, int tabGroup, gui::IGUIElement*& found,
                               gui::IGUIElement*& other, bool recurse);

// 0x578924 and 0x5789e4: glitch::scene::CBatchMesh::CBatchMesh(void), with reserved0x70 = 44.
// 0x578aa4 and 0x578b60: glitch::scene::CBatchMesh::CBatchMesh(unsigned int), with reserved0x70
// set to the argument. The vptr store is not written here: the vtable (off_974818) is unmapped.
scene::CBatchMesh* constructBatchMesh(scene::CBatchMesh* self, std::uint32_t reserved0x70);

// 0x578c50: glitch::scene::CBatchMesh::getMeshBuffer(unsigned int) const.
// Output is the intrusive_ptr slot the original writes, returned as in the pseudocode.
scene::CMeshBuffer** getMeshBuffer(scene::CMeshBuffer** out, const scene::CBatchMesh* self,
                                   unsigned int index);

// 0x57e594: glitch::scene::CBatchSceneNode::getMesh(void) const.
scene::IMesh** getMesh(scene::IMesh** out, const CBatchSceneNodeView* self);

// 0x55afcc: glitch::gui::CGUIToolBar::updateAbsolutePosition(void). The receiver is the
// IGUIElement base, as in the pseudocode.
int CGUIToolBar_updateAbsolutePosition(gui::IGUIElement* self);

// 0x55ecd0: glitch::gui::IGUIElement::setTabOrder(int). The int argument is not read by the
// pseudocode, so it is kept but unused. The pseudocode's return value is a clone artifact
// and is not reproduced.
void IGUIElement_setTabOrder(gui::IGUIElement* self, int unusedArgument);

}  // namespace glitch::collada::recon_v2_02

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_02_HPP
