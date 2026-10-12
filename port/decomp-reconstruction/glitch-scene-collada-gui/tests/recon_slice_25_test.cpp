// Tests for slice 25 (glitch::gui tab walk, context-menu item helpers). Plain asserts; returns 1 on failure.
#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/recon/slice_25.hpp"

namespace rc = glitch::collada::recon_25;
namespace gg = glitch::gui;

namespace {

// Same layout as the list node that IGUIElement_getNextElement reads (next, prev, element).
struct TestNode {
    void* next;
    void* prev;
    gg::IGUIElement* element;
};

int failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        ++failures;
        (void)what;
    }
}

}  // namespace

int main() {
    // IGUIElement_getNextElement on a parent with no children returns false.
    {
        gg::IGUIElement parent;
        parent.childrenNext = &parent.childrenNext;  // empty list is self-linked
        parent.childrenPrev = &parent.childrenPrev;
        gg::IGUIElement* next = nullptr;
        gg::IGUIElement* first = nullptr;
        check(!rc::IGUIElement_getNextElement(&parent, false, next, first, true), "empty children list");
        check(next == nullptr && first == nullptr, "empty children list leaves outputs null");
    }

    // A child with tab order at the 0x40000000 sentinel is returned as the first element.
    {
        gg::IGUIElement parent;
        gg::IGUIElement child;
        child.tabStop = 1;
        child.tabGroup = 0;
        child.tabOrder = 0x40000000;
        child.visible = 1;

        TestNode node{nullptr, nullptr, &child};
        parent.childrenNext = &node;
        parent.childrenPrev = &node;
        node.next = &parent.childrenNext;  // the list head is the sentinel at +0x04
        node.prev = &parent.childrenNext;

        gg::IGUIElement* next = nullptr;
        gg::IGUIElement* first = nullptr;
        check(rc::IGUIElement_getNextElement(&parent, false, next, first, true), "sentinel child found");
        check(first == &child, "sentinel child is the first element");
    }

    // setTabOrder with no parent and no tab group leaves the order at 0 (the decompiled body
    // writes 0 before the search).
    {
        gg::IGUIElement lone;
        lone.tabOrder = -1;
        rc::IGUIElement_setTabOrder(&lone, 5);
        check(lone.tabOrder == 0, "setTabOrder writes 0 for a root element");
    }

    // hasOpenSubMenu reports a visible submenu and ignores hidden ones.
    {
        gg::CGUIContextMenu menu;
        gg::CGUIContextMenu sub;
        std::vector<rc::SContextMenuItem> items(2);
        items[1].subMenu = &sub;
        menu.itemsBegin = items.data();
        menu.itemsEnd = items.data() + items.size();

        sub.visible = 0;
        check(!rc::CGUIContextMenu_hasOpenSubMenu(&menu), "hidden submenu is not open");
        sub.visible = 1;
        check(rc::CGUIContextMenu_hasOpenSubMenu(&menu), "visible submenu is open");
    }

    // setItemText assigns the text of an in-range item.
    {
        gg::CGUIContextMenu menu;
        std::vector<rc::SContextMenuItem> items(1);
        menu.itemsBegin = items.data();
        menu.itemsEnd = items.data() + items.size();
        rc::CGUIContextMenu_setItemText(&menu, 0, L"Open");
        check(items[0].text == L"Open", "setItemText assigns text");
        rc::CGUIContextMenu_setItemText(&menu, 3, L"Out of range");
        check(items[0].text == L"Open", "out-of-range setItemText is ignored");
    }

    // sendClick outside the menu rectangle returns 0.
    {
        gg::CGUIContextMenu menu;
        menu.absoluteRect.UpperLeftCorner = {0, 0};
        menu.absoluteRect.LowerRightCorner = {10, 10};
        glitch::core::position2d<std::int32_t> outside{50, 50};
        check(rc::CGUIContextMenu_sendClick(&menu, outside) == 0, "click outside returns 0");
    }

    if (failures != 0) {
        return 1;
    }
    return 0;
}
