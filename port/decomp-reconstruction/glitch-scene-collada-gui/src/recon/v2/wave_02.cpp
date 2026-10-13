// Wave 02 (recon v2) source.
#include "glitch-scene-collada-gui/recon/v2/wave_02.hpp"

namespace glitch::collada::recon_v2_02 {
// CBatchMesh constructors (0x578924 / 0x5789e4 / 0x578aa4 / 0x578b60), CBatchMesh::getMeshBuffer (0x578c50) and
// CBatchSceneNode::getMesh (0x57e594): canonical members in src/scene/CBatchMesh.cpp and src/scene/CBatchSceneNode.cpp.

// 0x55afcc: glitch::gui::CGUIToolBar::updateAbsolutePosition(void). Partial: the base call
// IGUIElement::updateAbsolutePosition is unmapped (declared only).
// desiredRect +0x58 and +0x60 are the rect's x words, set from the parent's absolute rect
// +0x38 and +0x40.
int CGUIToolBar_updateAbsolutePosition(gui::IGUIElement* self) {
    gui::IGUIElement* parent = self->parent;
    if (parent != nullptr) {
        self->desiredRect.UpperLeftCorner.X = 0;
        self->desiredRect.LowerRightCorner.X =
            parent->absoluteRect.LowerRightCorner.X - parent->absoluteRect.UpperLeftCorner.X;
    }
    return IGUIElement_updateAbsolutePosition(self);
}

// 0x55ecd0: glitch::gui::IGUIElement::setTabOrder(int). Partial: the tab walk calls
// getNextElement, which is itself partial (vtable slot +0x2c unmapped).
void IGUIElement_setTabOrder(gui::IGUIElement* self, int /*unusedArgument*/) {
    const int tabGroup = self->tabGroup;
    self->tabOrder = 0;

    gui::IGUIElement* start = nullptr;
    if (tabGroup != 0) {
        // Topmost ancestor, or the element itself when it has no parent.
        start = self;
        if (self->parent != nullptr) {
            start = self->parent;
            for (gui::IGUIElement* i = start->parent; i != nullptr; i = i->parent) {
                start = i;
            }
        }
    } else {
        // Nearest ancestor that starts a tab group.
        for (gui::IGUIElement* p = self->parent; p != nullptr; p = p->parent) {
            if (p->tabGroup != 0) {
                start = p;
                break;
            }
        }
        if (start == nullptr) {
            return;
        }
    }

    gui::IGUIElement* found = nullptr;
    gui::IGUIElement* other = nullptr;
    IGUIElement_getNextElement(start, tabGroup, found, other, true);
    if (found != nullptr) {
        self->tabOrder = found->tabOrder + 1;
    }
}

}  // namespace glitch::collada::recon_v2_02
