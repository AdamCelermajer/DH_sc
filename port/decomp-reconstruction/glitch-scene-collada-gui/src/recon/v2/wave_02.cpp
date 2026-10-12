// Wave 02 (recon v2) source.
#include "glitch-scene-collada-gui/recon/v2/wave_02.hpp"

#include <cstring>

namespace glitch::collada::recon_v2_02 {
namespace {

void setUnitBox(core::aabbox3d<float>& box) {
    box.MinEdge = core::vector3d<float>{-1.0f, -1.0f, -1.0f};
    box.MaxEdge = core::vector3d<float>{1.0f, 1.0f, 1.0f};
}

}  // namespace

// 0x578924 / 0x5789e4 / 0x578aa4 / 0x578b60: CBatchMesh constructors. Partial: every store
// except the vptr is written.
scene::CBatchMesh* constructBatchMesh(scene::CBatchMesh* self, std::uint32_t reserved0x70) {
    self->referenceCount = 0;
    std::memset(self->pad_0x08, 0, sizeof self->pad_0x08);  // +0x08..+0x1f zeroed
    self->batchesBegin = 0;
    self->batchesEnd = 0;
    self->batchesCapacity = 0;
    self->reserved0x2c = 0;
    self->reserved0x30 = 0;
    self->reserved0x34 = 0;
    setUnitBox(self->localBox);
    setUnitBox(self->boundingBox);
    self->boundingBoxSource = 0;
    self->boundingBoxStamp = 0xFFFFFFFFu;  // -1
    self->reserved0x70 = reserved0x70;
    self->boundingBoxDirty = 1;
    self->reserved0x78 = 0;
    return self;
}

// 0x578c50: glitch::scene::CBatchMesh::getMeshBuffer(unsigned int) const.
// SBatch is 20 bytes; meshBuffer is its first word; the refcount is CMeshBuffer +0x04.
scene::CMeshBuffer** getMeshBuffer(scene::CMeshBuffer** out, const scene::CBatchMesh* self,
                                   unsigned int index) {
    const auto* batches = targetPtr<const scene::CBatchMesh::SBatch>(self->batchesBegin);
    scene::CMeshBuffer* buffer = targetPtr<scene::CMeshBuffer>(batches[index].meshBuffer);
    *out = buffer;
    if (buffer != nullptr) {
        ++buffer->referenceCount;
    }
    return out;
}

// 0x57e594: glitch::scene::CBatchSceneNode::getMesh(void) const.
scene::IMesh** getMesh(scene::IMesh** out, const CBatchSceneNodeView* self) {
    scene::IMesh* mesh = targetPtr<scene::IMesh>(self->mesh);
    *out = mesh;
    if (mesh != nullptr) {
        ++mesh->referenceCount;
    }
    return out;
}

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
