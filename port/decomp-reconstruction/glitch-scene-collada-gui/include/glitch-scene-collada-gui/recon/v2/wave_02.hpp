// Recon v2 wave 02: partial-row rerun for glitch::scene::CBatchMesh / CBatchSceneNode and
// the glitch::gui IGUIElement tab, toolbar and table family.
//
// Layout used: layouts/gui.hpp (IGUIElement). Core value types come from deps/core_stand_ins.hpp.
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_02_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_02_HPP

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/layouts/gui.hpp"

namespace glitch::collada::recon_v2_02 {

// The scene half of this wave (CBatchMesh constructors and getMeshBuffer, CBatchSceneNode::getMesh, and the CBatchSceneNodeView
// layout) moved to the canonical classes in scene/CBatchMesh.hpp and scene/CBatchSceneNode.hpp.

// Tail of an extern the wave calls but does not own. Bodies are in other waves or unmapped.
// IGUIElement::updateAbsolutePosition(void), base of CGUIToolBar::updateAbsolutePosition.
int IGUIElement_updateAbsolutePosition(gui::IGUIElement* self);
// IGUIElement::getNextElement(int, bool, IGUIElement*&, IGUIElement*&, bool). Its body is
// partial (vtable slot +0x2c is unmapped), so it is declared only.
int IGUIElement_getNextElement(gui::IGUIElement* self, int tabGroup, gui::IGUIElement*& found,
                               gui::IGUIElement*& other, bool recurse);

// 0x55afcc: glitch::gui::CGUIToolBar::updateAbsolutePosition(void). The receiver is the
// IGUIElement base, as in the pseudocode.
int CGUIToolBar_updateAbsolutePosition(gui::IGUIElement* self);

// 0x55ecd0: glitch::gui::IGUIElement::setTabOrder(int). The int argument is not read by the
// pseudocode, so it is kept but unused. The pseudocode's return value is a clone artifact
// and is not reproduced.
void IGUIElement_setTabOrder(gui::IGUIElement* self, int unusedArgument);

}  // namespace glitch::collada::recon_v2_02

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_02_HPP
