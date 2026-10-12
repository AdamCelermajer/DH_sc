// Wave 01 (recon v2) source.
#include "glitch-scene-collada-gui/recon/v2/wave_01.hpp"

#include <cstddef>

// Engine allocator. The original requests 12 bytes (the 32-bit ARM size of ChildNode);
// the test file supplies the definitions.
extern "C" void* GlitchAlloc(std::size_t size, int hint);
extern "C" void GlitchFree(void* p);

namespace glitch::collada::recon_v2_01 {
namespace {

// Writes the next (writeNext) or prev word of a list link. The owner is either the
// sentinel (the IGUIElement's own childrenNext / childrenPrev pair) or a ChildNode.
void setLink(glitch::gui::IGUIElement* self, void* owner, bool writeNext, void* value) {
    if (owner == &self->childrenNext) {
        if (writeNext) {
            self->childrenNext = value;
        } else {
            self->childrenPrev = value;
        }
        return;
    }
    ChildNode* node = static_cast<ChildNode*>(owner);
    if (writeNext) {
        node->next = value;
    } else {
        node->prev = value;
    }
}

}  // namespace

// 0x535d70 _ZN6glitch3gui11IGUIElement12bringToFrontEPS1_
int bringToFront(glitch::gui::IGUIElement* self, glitch::gui::IGUIElement* child) {
    void* sentinel = &self->childrenNext;

    // Find the node that holds child.
    void* node = self->childrenNext;
    for (;;) {
        if (node == sentinel) {
            return 0;
        }
        ChildNode* candidate = static_cast<ChildNode*>(node);
        if (candidate->element == child) {
            break;
        }
        node = candidate->next;
    }

    // Unlink it: prev->next = next, next->prev = prev.
    ChildNode* found = static_cast<ChildNode*>(node);
    void* afterFound = found->next;
    void* beforeFound = found->prev;
    setLink(self, beforeFound, true, afterFound);
    setLink(self, afterFound, false, beforeFound);
    GlitchFree(found);

    // Append a fresh node at the tail: new.next = sentinel, new.prev = old tail,
    // old tail.next = new, sentinel.prev = new.
    ChildNode* fresh = static_cast<ChildNode*>(GlitchAlloc(sizeof(ChildNode), 0));
    fresh->element = child;
    fresh->next = sentinel;
    void* tail = self->childrenPrev;
    fresh->prev = tail;
    setLink(self, tail, true, fresh);
    self->childrenPrev = fresh;
    return 1;
}

}  // namespace glitch::collada::recon_v2_01
