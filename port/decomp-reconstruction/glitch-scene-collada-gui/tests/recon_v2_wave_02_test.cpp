// Test for the wave 02 (recon v2) reconstruction: the CGUIToolBar updateAbsolutePosition and IGUIElement tab order.
// (The CBatchMesh / CBatchSceneNode checks moved to tests/scene_batch_test.cpp with the canonical classes.)
#include "glitch-scene-collada-gui/recon/v2/wave_02.hpp"


#include <cstdio>
#include <cstdlib>
#include <cstring>
#if defined(__linux__)  // MAP_32BIT guest memory is Linux-only
#include <sys/mman.h>

// Test doubles for the externs the wave declares but does not own.
namespace {
int g_baseUpdateCalls = 0;
int g_nextFoundCalls = 0;
glitch::gui::IGUIElement* g_nextFound = nullptr;  // what getNextElement reports as found
glitch::gui::IGUIElement* g_lastStart = nullptr;  // start element seen by getNextElement
int g_lastTabGroup = -1;
}  // namespace

int glitch::collada::recon_v2_02::IGUIElement_updateAbsolutePosition(glitch::gui::IGUIElement*) {
    ++g_baseUpdateCalls;
    return 7;
}

int glitch::collada::recon_v2_02::IGUIElement_getNextElement(glitch::gui::IGUIElement* self,
                                                             int tabGroup,
                                                             glitch::gui::IGUIElement*& found,
                                                             glitch::gui::IGUIElement*& other,
                                                             bool recurse) {
    (void)recurse;
    ++g_nextFoundCalls;
    g_lastStart = self;
    g_lastTabGroup = tabGroup;
    found = g_nextFound;
    other = nullptr;
    return found != nullptr ? 1 : 0;
}

namespace {

using namespace glitch::collada::recon_v2_02;
namespace gui = glitch::gui;

int failures = 0;

void check(bool cond, const char* what) {
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", what);
    }
}

void testUpdateAbsolutePosition() {
    gui::IGUIElement parent;
    parent.absoluteRect.UpperLeftCorner.X = 10;
    parent.absoluteRect.LowerRightCorner.X = 110;

    gui::IGUIElement toolbar;
    toolbar.parent = &parent;
    toolbar.desiredRect.UpperLeftCorner.X = 55;
    toolbar.desiredRect.LowerRightCorner.X = 66;
    g_baseUpdateCalls = 0;
    int r = CGUIToolBar_updateAbsolutePosition(&toolbar);
    check(toolbar.desiredRect.UpperLeftCorner.X == 0, "updateAbsolutePosition zeroes desired left");
    check(toolbar.desiredRect.LowerRightCorner.X == 100, "updateAbsolutePosition sets desired width");
    check(r == 7 && g_baseUpdateCalls == 1, "updateAbsolutePosition tail-calls the base once");

    gui::IGUIElement orphan;
    orphan.desiredRect.UpperLeftCorner.X = 5;
    CGUIToolBar_updateAbsolutePosition(&orphan);
    check(orphan.desiredRect.UpperLeftCorner.X == 5, "updateAbsolutePosition leaves a parentless element");
}

void testSetTabOrderGroup() {
    gui::IGUIElement root;
    gui::IGUIElement child;
    child.parent = &root;
    child.tabGroup = 1;
    gui::IGUIElement next;
    next.tabOrder = 4;

    g_nextFound = &next;
    g_nextFoundCalls = 0;
    IGUIElement_setTabOrder(&child, 0);
    check(g_nextFoundCalls == 1, "setTabOrder calls getNextElement once in a tab group");
    check(g_lastStart == &root, "setTabOrder starts at the topmost ancestor");
    check(g_lastTabGroup == 1, "setTabOrder passes the group flag");
    check(child.tabOrder == 5, "setTabOrder sets tabOrder to found + 1");

    g_nextFound = nullptr;
    child.tabOrder = 9;
    IGUIElement_setTabOrder(&child, 0);
    check(child.tabOrder == 0, "setTabOrder clears tabOrder when nothing is found");
    g_nextFound = nullptr;
}

void testSetTabOrderNoGroup() {
    gui::IGUIElement top;
    top.tabGroup = 1;
    gui::IGUIElement mid;
    mid.parent = &top;
    gui::IGUIElement leaf;
    leaf.parent = &mid;
    leaf.tabOrder = 3;

    g_nextFoundCalls = 0;
    g_nextFound = nullptr;
    IGUIElement_setTabOrder(&leaf, 0);
    check(g_nextFoundCalls == 1, "setTabOrder walks to the nearest tab-group ancestor");
    check(g_lastStart == &top, "setTabOrder starts at the tab-group ancestor");
    check(g_lastTabGroup == 0, "setTabOrder passes group flag 0 outside a group");
    check(leaf.tabOrder == 0, "setTabOrder zeroes tabOrder before the search");

    gui::IGUIElement lone;
    g_nextFoundCalls = 0;
    IGUIElement_setTabOrder(&lone, 0);
    check(g_nextFoundCalls == 0, "setTabOrder does nothing without a tab-group ancestor");
}

}  // namespace

int main() {
    testUpdateAbsolutePosition();
    testSetTabOrderGroup();
    testSetTabOrderNoGroup();
    if (failures == 0) {
        std::printf("wave_02 tests: all passed\n");
        return 0;
    }
    std::printf("wave_02 tests: %d failure(s)\n", failures);
    return 1;
}

#else
int main() {
    std::printf("skipped: MAP_32BIT guest memory is Linux-only\n");
    return 0;
}
#endif
