// Test for the wave 01 (recon v2) child-list reconstruction.
#include "glitch-scene-collada-gui/recon/v2/wave_01.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <vector>

namespace {

using glitch::collada::recon_v2_01::ChildNode;
using glitch::collada::recon_v2_01::bringToFront;
using glitch::gui::IGUIElement;

std::unique_ptr<IGUIElement> makeParent() {
    auto e = std::make_unique<IGUIElement>();
    e->childrenNext = &e->childrenNext;  // empty list: sentinel links to itself
    e->childrenPrev = &e->childrenNext;
    return e;
}

// Appends a child the way addChild does: a new node at the tail.
void append(IGUIElement* parent, IGUIElement* child) {
    auto* n = static_cast<ChildNode*>(std::malloc(sizeof(ChildNode)));
    n->element = child;
    n->next = &parent->childrenNext;
    n->prev = parent->childrenPrev;
    static_cast<ChildNode*>(parent->childrenPrev)->next = n;  // tail is always a node here
    parent->childrenPrev = n;
}

// Forward walk of the child list, checking the back links on the way.
std::vector<IGUIElement*> forward(IGUIElement* parent, bool& linksOk) {
    std::vector<IGUIElement*> out;
    linksOk = true;
    void* sentinel = &parent->childrenNext;
    void* prev = sentinel;
    void* node = parent->childrenNext;
    while (node != sentinel) {
        auto* n = static_cast<ChildNode*>(node);
        if (n->prev != prev) linksOk = false;
        out.push_back(n->element);
        prev = node;
        node = n->next;
    }
    if (parent->childrenPrev != prev) linksOk = false;
    return out;
}

int failures = 0;

void check(bool cond, const char* what) {
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", what);
    }
}

void clearList(IGUIElement* parent) {
    void* sentinel = &parent->childrenNext;
    void* node = parent->childrenNext;
    while (node != sentinel) {
        auto* n = static_cast<ChildNode*>(node);
        node = n->next;
        std::free(n);
    }
    parent->childrenNext = sentinel;
    parent->childrenPrev = sentinel;
}

}  // namespace

int main() {
    IGUIElement a, b, c, stranger;

    {  // empty list: not found
        auto p = makeParent();
        check(bringToFront(p.get(), &a) == 0, "empty list returns 0");
        bool ok = false;
        check(forward(p.get(), ok).empty() && ok, "empty list unchanged");
    }

    {  // first child moves to the end: [a,b,c] -> [b,c,a]
        auto p = makeParent();
        append(p.get(), &a);
        append(p.get(), &b);
        append(p.get(), &c);
        check(bringToFront(p.get(), &a) == 1, "move head returns 1");
        bool ok = false;
        auto order = forward(p.get(), ok);
        check(ok, "links consistent after moving head");
        check(order == std::vector<IGUIElement*>({&b, &c, &a}), "head moved to end");
        clearList(p.get());
    }

    {  // middle child: [a,b,c] -> [a,c,b]
        auto p = makeParent();
        append(p.get(), &a);
        append(p.get(), &b);
        append(p.get(), &c);
        check(bringToFront(p.get(), &b) == 1, "move middle returns 1");
        bool ok = false;
        auto order = forward(p.get(), ok);
        check(ok, "links consistent after moving middle");
        check(order == std::vector<IGUIElement*>({&a, &c, &b}), "middle moved to end");
        clearList(p.get());
    }

    {  // tail child: [a,b,c] stays [a,b,c]
        auto p = makeParent();
        append(p.get(), &a);
        append(p.get(), &b);
        append(p.get(), &c);
        check(bringToFront(p.get(), &c) == 1, "move tail returns 1");
        bool ok = false;
        auto order = forward(p.get(), ok);
        check(ok, "links consistent after moving tail");
        check(order == std::vector<IGUIElement*>({&a, &b, &c}), "tail stays at end");
        clearList(p.get());
    }

    {  // single child
        auto p = makeParent();
        append(p.get(), &a);
        check(bringToFront(p.get(), &a) == 1, "single child returns 1");
        bool ok = false;
        check(forward(p.get(), ok) == std::vector<IGUIElement*>({&a}) && ok, "single child kept");
        clearList(p.get());
    }

    {  // child not in list: list unchanged
        auto p = makeParent();
        append(p.get(), &a);
        append(p.get(), &b);
        check(bringToFront(p.get(), &stranger) == 0, "stranger returns 0");
        bool ok = false;
        check(forward(p.get(), ok) == std::vector<IGUIElement*>({&a, &b}) && ok, "list unchanged");
        clearList(p.get());
    }

    if (failures == 0) {
        std::printf("recon_v2_wave_01_test: PASS\n");
        return 0;
    }
    std::printf("recon_v2_wave_01_test: %d failure(s)\n", failures);
    return 1;
}
