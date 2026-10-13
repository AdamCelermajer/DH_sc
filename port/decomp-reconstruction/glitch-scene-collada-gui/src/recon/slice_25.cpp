// Slice 25: glitch::gui (CGUIContextMenu, IGUIElement tab order, CGUIEditBox) and
// glitch::scene (CAppendMeshBuffer, CMeshBuffer, CMesh, CDummyTransformationSceneNode,
// default factories, Collada binary loader). Declarations and layouts are in
// include/glitch-scene-collada-gui/recon/slice_25.hpp and layouts/.

#include "glitch-scene-collada-gui/recon/slice_25.hpp"

namespace glitch::collada::recon_25 {

namespace unattached {

// IGUIElement vtable slot +0x08 (onEvent). Not declared on glitch::gui::IGUIElement, so
// the forwarding call is stubbed and reports the event as not consumed.
int onEvent(glitch::gui::IGUIElement* target, const SEventStand& ev) {
    (void)target;
    (void)ev;
    return 0;
}

// glitch::os::Timer::getTime. Not declared in any layout; the stub returns 0.
std::uint32_t timerNow() { return 0; }

// IReferenceCounted::drop on an object whose layout is not declared. No-op stub.
void dropRef(void* object) { (void)object; }

}  // namespace unattached

namespace {

// Child list node of IGUIElement (the list head is the sentinel at +0x04 and +0x08).
// Only the next link and the element pointer are read here.
struct ChildNode {
    ChildNode* next = nullptr;  // +0x00
    ChildNode* prev = nullptr;  // +0x04
    glitch::gui::IGUIElement* element = nullptr;  // +0x08
};

}  // namespace

// 0x6b0194 _ZNK6glitch3gui11IGUIElement14getNextElementEibbRPS1_S3_b.clone.4
// glitch::gui::IGUIElement::getNextElement (clone of the tab walk). Walks the
// children list of `self` and finds the lowest tab order above the caller's and the first
// candidate. Visibility is read from the visible byte at +0x98.
bool IGUIElement_getNextElement(glitch::gui::IGUIElement* self, bool tabGroup, glitch::gui::IGUIElement*& next,
                                glitch::gui::IGUIElement*& first, bool includeHidden) {
    ChildNode* sentinel = reinterpret_cast<ChildNode*>(&self->childrenNext);  // +0x04 list head
    ChildNode* node = static_cast<ChildNode*>(self->childrenNext);
    if (node == sentinel) {
        return false;
    }
    while (true) {
        glitch::gui::IGUIElement* child = node->element;
        bool skip = !child->visible && !includeHidden;  // +0x98 visible
        if (!skip && !tabGroup && child->tabGroup) {    // +0x13c tabGroup
            skip = true;
        }
        if (!skip) {
            if (!child->tabStop || child->tabGroup != tabGroup) {  // +0x134 tabStop
                if (IGUIElement_getNextElement(child, tabGroup, next, first, false)) {
                    return true;
                }
            } else {
                const std::int32_t order = child->tabOrder;  // +0x138
                if (order == 0x40000000) {
                    first = child;
                    return true;
                }
                if (!first) {
                    if (order < -1) {
                        first = child;
                    }
                } else if (order < -1 && order > first->tabOrder) {
                    first = child;
                }
                if (next && order <= next->tabOrder) {
                    if (IGUIElement_getNextElement(child, tabGroup, next, first, false)) {
                        return true;
                    }
                } else {
                    next = child;
                    if (IGUIElement_getNextElement(child, tabGroup, next, first, false)) {
                        return true;
                    }
                }
            }
        }
        node = node->next;
        if (node == sentinel) {
            return false;
        }
    }
}

// 0x6b02e8 _ZN6glitch3gui11IGUIElement11setTabOrderEi.clone.2
// glitch::gui::IGUIElement::setTabOrder. The decompiled body writes 0 to the tab
// order before it searches; the argument is not read by that body.
void IGUIElement_setTabOrder(glitch::gui::IGUIElement* self, std::int32_t order) {
    (void)order;
    self->tabOrder = 0;  // +0x138
    glitch::gui::IGUIElement* start = nullptr;
    if (self->tabGroup) {  // +0x13c
        start = self;
        if (self->parent) {  // +0x24
            start = self->parent;
            for (glitch::gui::IGUIElement* i = start->parent; i; i = i->parent) {
                start = i;
            }
        }
        glitch::gui::IGUIElement* next = nullptr;
        glitch::gui::IGUIElement* first = nullptr;
        IGUIElement_getNextElement(start, true, next, first, true);
        if (next) {
            self->tabOrder = next->tabOrder + 1;
        }
    } else {
        glitch::gui::IGUIElement* p = self->parent;
        while (p && !p->tabGroup) {
            p = p->parent;
        }
        if (p) {
            glitch::gui::IGUIElement* next = nullptr;
            glitch::gui::IGUIElement* first = nullptr;
            IGUIElement_getNextElement(p, false, next, first, true);
            if (next) {
                self->tabOrder = next->tabOrder + 1;
            }
        }
    }
}

// 0x6ad620 _ZNK6glitch3gui15CGUIContextMenu14hasOpenSubMenuEv
// glitch::gui::CGUIContextMenu::hasOpenSubMenu
bool CGUIContextMenu_hasOpenSubMenu(const glitch::gui::CGUIContextMenu* self) {
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        const SContextMenuItem& item = static_cast<const SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu && item.subMenu->visible) {  // isVisible reads +0x98
            return true;
        }
    }
    return false;
}

// 0x6ad6c4 _ZN6glitch3gui15CGUIContextMenu16closeAllSubMenusEv
// glitch::gui::CGUIContextMenu::closeAllSubMenus. The decompiled body returns the
// items end word as a side effect; that return value is not reproduced.
void CGUIContextMenu_closeAllSubMenus(glitch::gui::CGUIContextMenu* self) {
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu) {
            CGUIContextMenu_setVisible(item.subMenu, false);
        }
    }
}

// 0x6adc3c _ZN6glitch3gui15CGUIContextMenu10setVisibleEb
// glitch::gui::CGUIContextMenu::setVisible. Stamp (+0x170) uses the stubbed timer.
void CGUIContextMenu_setVisible(glitch::gui::CGUIContextMenu* self, bool visible) {
    self->highlightedItem = -1;                                        // +0x158
    self->field_0x170 = unattached::timerNow();                        // +0x170 stamp (timer stubbed)
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu) {
            CGUIContextMenu_setVisible(item.subMenu, false);
        }
    }
    self->visible = visible ? 1 : 0;                                   // +0x98
}

// 0x6ad8cc _ZN6glitch3gui15CGUIContextMenu11setItemTextEjPKw
// glitch::gui::CGUIContextMenu::setItemText. Out-of-range index does nothing; the
// decompiled return value (this on that path) is not reproduced.
void CGUIContextMenu_setItemText(glitch::gui::CGUIContextMenu* self, std::uint32_t index, const wchar_t* text) {
    if (index < itemCount(self)) {
        static_cast<SContextMenuItem*>(self->itemsBegin)[index].text = text;
        CGUIContextMenu_recalculateSize(self);
    }
}

// 0x6ad02c _ZN6glitch3gui15CGUIContextMenu9sendClickERKNS_4core10position2dIiEE
// glitch::gui::CGUIContextMenu::sendClick. The forwarding parts (parent onEvent,
// event parent at +0x174) are stubbed; the item selection path is rebuilt.
std::uint32_t CGUIContextMenu_sendClick(glitch::gui::CGUIContextMenu* self, const glitch::core::position2d<std::int32_t>& p) {
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu && item.subMenu->visible) {
            const std::uint32_t result = CGUIContextMenu_sendClick(item.subMenu, p);
            if (result) {
                return result;
            }
        }
    }
    // isPointInside (IGUIElement::isPointInside): hit test against the absolute rectangle at +0x38.
    const bool inside = p.X >= self->absoluteRect.UpperLeftCorner.X && p.Y >= self->absoluteRect.UpperLeftCorner.Y &&
                        p.X <= self->absoluteRect.LowerRightCorner.X && p.Y <= self->absoluteRect.LowerRightCorner.Y;
    if (!inside) {
        return 0;
    }
    const std::size_t count = itemCount(self);
    const std::size_t index = static_cast<std::size_t>(self->highlightedItem < 0 ? 0 : self->highlightedItem);
    if (self->highlightedItem < 0 || index >= count) {
        return 0;
    }
    const SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[index];
    if (!item.enabled || item.isSeparator || item.subMenu) {
        return 2;
    }
    SEventStand ev;
    ev.type = 0;
    ev.caller = self;
    ev.element = nullptr;
    ev.guiEventType = 18;  // GUI event: menu item selected
    if (self->parent) {
        unattached::onEvent(self->parent, ev);
    }
    return 1;
}

// 0x6ad260 _ZN6glitch3gui15CGUIContextMenu15recalculateSizeEv
// glitch::gui::CGUIContextMenu::recalculateSize. Skin metrics (environment vtable
// slots +0x38 and +0x28) are not declared, so the item layout pass is not rebuilt.
void CGUIContextMenu_recalculateSize(glitch::gui::CGUIContextMenu* self) {
    (void)self;
}

// 0x6ad58c _ZN6glitch3gui15CGUIContextMenu14setEventParentEPNS0_11IGUIElementE
// glitch::gui::CGUIContextMenu::setEventParent. The event parent word at +0x174 is
// declared uint32 in gui.hpp, so the pointer is not stored; submenus are walked.
void CGUIContextMenu_setEventParent(glitch::gui::CGUIContextMenu* self, glitch::gui::IGUIElement* parent) {
    (void)parent;
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu) {
            CGUIContextMenu_setEventParent(item.subMenu, parent);
        }
    }
}

// 0x6ad9e8 _ZN6glitch3gui15CGUIContextMenu9highlightERKNS_4core10position2dIiEEb
// glitch::gui::CGUIContextMenu::highlight. The hit test goes through the item rectangle
// virtual (slot +0xc8) and the submenu open path needs the timer; neither is rebuilt.
bool CGUIContextMenu_highlight(glitch::gui::CGUIContextMenu* self, const glitch::core::position2d<std::int32_t>& p,
                               bool canOpenSubMenu) {
    (void)p;
    (void)canOpenSubMenu;
    self->highlightedItem = -1;
    return false;
}

// 0x6ae480 _ZN6glitch3gui15CGUIContextMenu14removeAllItemsEv
// glitch::gui::CGUIContextMenu::removeAllItems. Submenu references are dropped via the
// stubbed refcount call; the vector erase is not modelled through the void* words.
void CGUIContextMenu_removeAllItems(glitch::gui::CGUIContextMenu* self) {
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu) {
            unattached::dropRef(item.subMenu);
        }
    }
    self->itemsEnd = self->itemsBegin;
    CGUIContextMenu_recalculateSize(self);
}

// 0x6ae7bc _ZN6glitch3gui15CGUIContextMenu10removeItemEj
// glitch::gui::CGUIContextMenu::removeItem. Erase from the layout's item array is not
// modelled through the void* words; the submenu drop is stubbed.
void CGUIContextMenu_removeItem(glitch::gui::CGUIContextMenu* self, std::uint32_t index) {
    if (index < itemCount(self)) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[index];
        if (item.subMenu) {
            unattached::dropRef(item.subMenu);
            item.subMenu = nullptr;
        }
        CGUIContextMenu_recalculateSize(self);
    }
}

// 0x6ae858 _ZN6glitch3gui15CGUIContextMenuD1Ev
// glitch::gui::CGUIContextMenu::~CGUIContextMenu (complete). Vtable resets and base
// destruction are compiler-generated; the item loop drops submenus through the stub.
void CGUIContextMenu_dtor_complete(glitch::gui::CGUIContextMenu* self) {
    for (std::size_t i = 0; i < itemCount(self); ++i) {
        SContextMenuItem& item = static_cast<SContextMenuItem*>(self->itemsBegin)[i];
        if (item.subMenu) {
            unattached::dropRef(item.subMenu);
        }
    }
}

// 0x6aea9c _ZN6glitch3gui15CGUIContextMenu7addItemEPKwibbb
// glitch::gui::CGUIContextMenu::addItem. The item vector growth (push_back) and the
// submenu construction (operator new + ctor) are not rebuilt; returns -1.
std::int32_t CGUIContextMenu_addItem(glitch::gui::CGUIContextMenu* self, const wchar_t* text, std::int32_t commandId,
                                     bool enabled, bool hasSubMenu, bool checked) {
    (void)self;
    (void)text;
    (void)commandId;
    (void)enabled;
    (void)hasSubMenu;
    (void)checked;
    return -1;
}

// 0x6aec08 _ZN6glitch3gui15CGUIContextMenu4drawEv
// glitch::gui::CGUIContextMenu::draw. Skin and font calls (environment vtable) and
// the item drawing loop are not rebuilt.
void CGUIContextMenu_draw(glitch::gui::CGUIContextMenu* self) {
    (void)self;
}

// 0x6ae1f4 _ZN6glitch3gui15CGUIContextMenu7onEventERKNS_6SEventE
// glitch::gui::CGUIContextMenu::onEvent. The event decisions are rebuilt; forwards
// to the parent go through the stub. The environment calls (highlight gate, select gate) are
// not declared, so the gates are taken as open and the close-on-select call is not made.
int CGUIContextMenu_onEvent(glitch::gui::CGUIContextMenu* self, const SEventStand& ev) {
    bool forward = true;
    if (self->enabled) {  // +0x99
        if (ev.type == 0) {  // GUI event
            if (ev.guiEventType == 0) {
                if (ev.caller == self) {
                    bool outside = true;
                    if (ev.element) {
                        glitch::gui::IGUIElement* cur = ev.element;
                        for (glitch::gui::IGUIElement* p = ev.element->parent; p; p = p->parent) {
                            cur = p;
                            if (p == ev.caller || !p->parent) {
                                break;
                            }
                        }
                        outside = ev.caller != cur;
                    }
                    if (outside && self->flag_0x178) {
                        // The original also calls the element remove slot here; not declared.
                        CGUIContextMenu_setEventParent(self, self->parent);
                        return 0;
                    }
                }
            } else if (ev.guiEventType == 1 && ev.caller == self && !self->flag_0x178) {
                return 1;
            }
        } else if (ev.type == 1) {  // mouse event
            if (ev.mouseEventType == 3) {
                const glitch::core::position2d<std::int32_t> pos{ev.mouseX, ev.mouseY};
                CGUIContextMenu_sendClick(self, pos);
                return 1;
            }
            if (ev.mouseEventType == 6) {
                const glitch::core::position2d<std::int32_t> pos{ev.mouseX, ev.mouseY};
                CGUIContextMenu_highlight(self, pos, true);
                return 1;
            }
            if (ev.mouseEventType == 0) {
                return 1;
            }
        }
    }
    if (forward && self->parent) {
        return unattached::onEvent(self->parent, ev);
    }
    return 0;
}

// 0x6adcf0 _ZN6glitch3gui15IGUIContextMenuC2EPNS0_15IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEE
// glitch::gui::IGUIContextMenu::IGUIContextMenu. Base-element state (ids, rects,
// tab fields) is written; the string storage and the environment/parent rectangle pass use stubs.
void IGUIContextMenu_ctor(glitch::gui::IGUIElement* self, glitch::gui::IGUIElement* parent, std::int32_t id,
                          const glitch::core::rect<std::int32_t>& rect) {
    self->parent = parent;
    self->relativeRect = rect;
    self->desiredRect = rect;
    self->id = id;
    self->visible = 1;
    self->enabled = 1;
    self->tabStop = 0;
    self->tabGroup = 0;
    self->tabOrder = -1;
}

// 0x6adfb8 _ZN6glitch3gui15CGUIContextMenuC1EPNS0_15IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEEbb
// glitch::gui::CGUIContextMenu::CGUIContextMenu. The base is constructed, the last
// argument is kept in +0x178 and the default fields are cleared. The environment addChild
// call is stubbed.
void CGUIContextMenu_ctor(glitch::gui::CGUIContextMenu* self, glitch::gui::IGUIElement* parent, std::int32_t id,
                          const glitch::core::rect<std::int32_t>& rect, bool drawBackground, bool moveOnScreen) {
    IGUIContextMenu_ctor(self, parent, id, rect);
    self->highlightedItem = -1;
    self->itemsBegin = nullptr;
    self->itemsEnd = nullptr;
    self->itemsCapacity = nullptr;
    self->flag_0x178 = moveOnScreen ? 1 : 0;
    CGUIContextMenu_recalculateSize(self);
    (void)drawBackground;  // environment addChild (+0x150 vtable) is not declared; not called
    self->field_0x188 = 1;
}

// 0x6ae100 _ZN6glitch3gui15CGUIContextMenuC2EPNS0_15IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEEbb
// glitch::gui::CGUIContextMenu::CGUIContextMenu (second variant, same demangled name).
void CGUIContextMenu_ctor_alt(glitch::gui::CGUIContextMenu* self, glitch::gui::IGUIElement* parent, std::int32_t id,
                              const glitch::core::rect<std::int32_t>& rect, bool drawBackground, bool moveOnScreen) {
    CGUIContextMenu_ctor(self, parent, id, rect, drawBackground, moveOnScreen);
}

// 0x6b0394 _ZN6glitch3gui11IGUIEditBoxC2EPNS0_15IGUIEnvironmentEPNS0_11IGUIElementEiNS_4core4rectIiEE
// glitch::gui::IGUIEditBox::IGUIEditBox. Base state only: ids, rectangles and tab
// fields. The CGUIEditBox member block (text, cursor, line vectors) is not declared, so it is not set.
void IGUIEditBox_ctor(glitch::gui::IGUIElement* self, glitch::gui::IGUIEnvironment* env, glitch::gui::IGUIElement* parent,
                      std::int32_t id, const glitch::core::rect<std::int32_t>& rect) {
    (void)env;
    self->parent = parent;
    self->relativeRect = rect;
    self->desiredRect = rect;
    self->id = id;
    self->visible = 1;
    self->enabled = 1;
    self->tabStop = 0;
    self->tabGroup = 0;
    self->tabOrder = -1;
}

// 0x6b2608 _ZN6glitch3gui11CGUIEditBoxC1EPKwbPNS0_15IGUIEnvironmentEPNS0_11IGUIElementEiRKNS_4core4rectIiEE
// glitch::gui::CGUIEditBox::CGUIEditBox (text, border, environment, parent, id, rect).
// Unattached: the text, border and line members are not declared; only the base is constructed.
void CGUIEditBox_ctor(glitch::gui::IGUIElement* self, const wchar_t* text, bool border, glitch::gui::IGUIEnvironment* env,
                      glitch::gui::IGUIElement* parent, std::int32_t id, const glitch::core::rect<std::int32_t>& rect) {
    (void)text;
    (void)border;
    IGUIEditBox_ctor(self, env, parent, id, rect);
}

// 0x6b28c8 _ZN6glitch3gui11CGUIEditBoxC2EPKwbPNS0_15IGUIEnvironmentEPNS0_11IGUIElementEiRKNS_4core4rectIiEE
// glitch::gui::CGUIEditBox::CGUIEditBox (second variant, same demangled name).
void CGUIEditBox_ctor_alt(glitch::gui::IGUIElement* self, const wchar_t* text, bool border, glitch::gui::IGUIEnvironment* env,
                          glitch::gui::IGUIElement* parent, std::int32_t id, const glitch::core::rect<std::int32_t>& rect) {
    CGUIEditBox_ctor(self, text, border, env, parent, id, rect);
}

// 0x6b0bac _ZN6glitch3gui11CGUIEditBoxD1Ev
// glitch::gui::CGUIEditBox::~CGUIEditBox (complete). Unattached: the text, line and
// refcount members are not declared; the releases are stubbed.
void CGUIEditBox_dtor_complete(glitch::gui::IGUIElement* self) {
    (void)self;
}

// 0x6afafc _ZN6glitch3gui11CGUIEditBox11setTextRectEi
// glitch::gui::CGUIEditBox::setTextRect. CGUIEditBox has no layout in layouts/, so the
// text rectangle state is not modelled.
void CGUIEditBox_setTextRect(glitch::gui::IGUIElement* self, std::int32_t line) {
    (void)self;
    (void)line;
}

// 0x6afd18 _ZN6glitch3gui11CGUIEditBox12getCursorPosEii
// glitch::gui::CGUIEditBox::getCursorPos. Unattached: the text line vector and the
// font width calls are not declared.
std::int32_t CGUIEditBox_getCursorPos(glitch::gui::IGUIElement* self, std::int32_t x, std::int32_t y) {
    (void)self;
    (void)x;
    (void)y;
    return 0;
}

// 0x6afe9c _ZN6glitch3gui11CGUIEditBox16getTextDimensionEv
// glitch::gui::CGUIEditBox::getTextDimension. Unattached: the line rectangles are not declared.
void CGUIEditBox_getTextDimension(glitch::gui::IGUIElement* self, glitch::core::dimension2d<std::int32_t>& out) {
    (void)self;
    out = {};
}

// 0x6affcc _ZN6glitch3gui11CGUIEditBox14getLineFromPosEi
// glitch::gui::CGUIEditBox::getLineFromPos. Unattached: the line-start vector is not declared.
std::int32_t CGUIEditBox_getLineFromPos(glitch::gui::IGUIElement* self, std::int32_t pos) {
    (void)self;
    (void)pos;
    return -1;
}

// 0x6b0844 _ZN6glitch3gui11CGUIEditBox14setPasswordBoxEbw
// glitch::gui::CGUIEditBox::setPasswordBox. Unattached: the password flag and character
// live in CGUIEditBox members that are not declared.
void CGUIEditBox_setPasswordBox(glitch::gui::IGUIElement* self, bool passwordBox, wchar_t passwordChar) {
    (void)self;
    (void)passwordBox;
    (void)passwordChar;
}

// 0x6b0f8c _ZN6glitch3gui11CGUIEditBox9breakTextEv
// glitch::gui::CGUIEditBox::breakText. Unattached: the line vectors and the text
// measuring calls are not declared; the 1.5 KB word-wrap pass is not rebuilt.
void CGUIEditBox_breakText(glitch::gui::IGUIElement* self) {
    (void)self;
}

// 0x6b15c8 _ZN6glitch3gui11CGUIEditBox7setTextEPKw
// glitch::gui::CGUIEditBox::setText. Unattached: the text member is not declared.
void CGUIEditBox_setText(glitch::gui::IGUIElement* self, const wchar_t* text) {
    (void)self;
    (void)text;
}

// 0x6b160c _ZN6glitch3gui11CGUIEditBox22updateAbsolutePositionEv
// glitch::gui::CGUIEditBox::updateAbsolutePosition. The base update is not rebuilt
// here; the word-wrap pass is stubbed.
void CGUIEditBox_updateAbsolutePosition(glitch::gui::IGUIElement* self) {
    CGUIEditBox_breakText(self);
}

// 0x6b162c _ZN6glitch3gui11CGUIEditBox15setOverrideFontEPNS0_8IGUIFontE
// glitch::gui::CGUIEditBox::setOverrideFont. Unattached: the override font member is not declared.
void CGUIEditBox_setOverrideFont(glitch::gui::IGUIElement* self, glitch::gui::IGUIFont* font) {
    (void)font;
    CGUIEditBox_breakText(self);
}

// 0x6b1668 _ZN6glitch3gui11CGUIEditBox9inputCharEw
// glitch::gui::CGUIEditBox::inputChar. Unattached: the text and max-length members are not declared.
void CGUIEditBox_inputChar(glitch::gui::IGUIElement* self, wchar_t ch) {
    (void)ch;
    CGUIEditBox_breakText(self);
}

// 0x6b185c _ZN6glitch3gui11CGUIEditBox6setMaxEj
// glitch::gui::CGUIEditBox::setMax. Unattached: the max-length member is not declared.
void CGUIEditBox_setMax(glitch::gui::IGUIElement* self, std::uint32_t maxChars) {
    (void)self;
    (void)maxChars;
}

// 0x6b18e0 _ZN6glitch3gui11CGUIEditBox4drawEv
// glitch::gui::CGUIEditBox::draw. The 3 KB drawing pass (skin and font calls) is not rebuilt.
void CGUIEditBox_draw(glitch::gui::IGUIElement* self) {
    (void)self;
}

// 0x6b246c _ZN6glitch3gui11CGUIEditBox18calculateScrollPosEv
// glitch::gui::CGUIEditBox::calculateScrollPos. Unattached: scroll members are not declared.
void CGUIEditBox_calculateScrollPos(glitch::gui::IGUIElement* self) {
    (void)self;
}

// 0x6b2b3c _ZN6glitch3gui11CGUIEditBox12processMouseERKNS_6SEventE
// glitch::gui::CGUIEditBox::processMouse. Unattached: cursor and selection members are not declared.
bool CGUIEditBox_processMouse(glitch::gui::IGUIElement* self, const SEventStand& ev) {
    (void)self;
    (void)ev;
    return false;
}

// 0x6b2cc0 _ZN6glitch3gui11CGUIEditBox10processKeyERKNS_6SEventE
// glitch::gui::CGUIEditBox::processKey. The 4 KB key handler is not rebuilt.
bool CGUIEditBox_processKey(glitch::gui::IGUIElement* self, const SEventStand& ev) {
    (void)self;
    (void)ev;
    return false;
}

// 0x6b3d70 _ZN6glitch3gui11CGUIEditBox7onEventERKNS_6SEventE
// glitch::gui::CGUIEditBox::onEvent. Unattached: the key and mouse handlers are stubbed,
// so events fall through to the parent via the stub.
bool CGUIEditBox_onEvent(glitch::gui::IGUIElement* self, const SEventStand& ev) {
    (void)self;
    (void)ev;
    return false;
}

// CMeshBuffer (0x6b88a8 clone, 0x6bc6d8 onDelete, 0x6bca38 D2, 0x6bcb90 clone): canonical members in src/scene/CMeshBuffer.cpp.

// 0x6b93b0 _ZN6glitch5scene24CColladaBinaryFileLoaderD2Ev
// glitch::scene::CColladaBinaryFileLoader::~CColladaBinaryFileLoader (complete).
// Unattached: the loader layout is not declared; the member release is stubbed.
void CColladaBinaryFileLoader_dtor_complete(void* self) {
    (void)self;
}

// 0x6b93f0 _ZN6glitch5scene24CColladaBinaryFileLoaderD1Ev
// glitch::scene::CColladaBinaryFileLoader::~CColladaBinaryFileLoader (second variant).
void CColladaBinaryFileLoader_dtor_complete_alt(void* self) {
    CColladaBinaryFileLoader_dtor_complete(self);
}

// 0x6b9430 _ZN6glitch5scene24CColladaBinaryFileLoader11createSceneEPNS_2io9IReadFileE
// glitch::scene::CColladaBinaryFileLoader::createScene. The decompiled body returns 0.
int CColladaBinaryFileLoader_createScene(void* self, void* file) {
    (void)self;
    (void)file;
    return 0;
}

// 0x6b9458 _ZN6glitch5scene24CColladaBinaryFileLoader10createMeshEPNS_2io9IReadFileE
// glitch::scene::CColladaBinaryFileLoader::createMesh. Unattached: CResFileManager,
// CColladaDatabase and the driver-file words are not declared.
void* CColladaBinaryFileLoader_createMesh(void* self, void* file, void* driverFile) {
    (void)self;
    (void)file;
    (void)driverFile;
    return nullptr;
}

// 0x6b9590 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryC2EPNS0_13CSceneManagerEPNS_3gui14ICursorControlE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::CDefaultSceneNodeAnimatorFactory.
// Unattached: the factory layout is not declared in scene.hpp.
void CDefaultSceneNodeAnimatorFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl) {
    (void)self;
    (void)manager;
    (void)cursorControl;
}

// 0x6b95dc _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryC1EPNS0_13CSceneManagerEPNS_3gui14ICursorControlE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::CDefaultSceneNodeAnimatorFactory (second variant).
void CDefaultSceneNodeAnimatorFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager, void* cursorControl) {
    CDefaultSceneNodeAnimatorFactory_ctor(self, manager, cursorControl);
}

// 0x6b9628 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryD2Ev
// glitch::scene::CDefaultSceneNodeAnimatorFactory::~CDefaultSceneNodeAnimatorFactory (complete).
// Unattached: the factory layout is not declared; the cursor-control release is stubbed.
void CDefaultSceneNodeAnimatorFactory_dtor_complete(void* self) {
    (void)self;
}

// 0x6b9668 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactoryD1Ev
// glitch::scene::CDefaultSceneNodeAnimatorFactory::~CDefaultSceneNodeAnimatorFactory (second variant).
void CDefaultSceneNodeAnimatorFactory_dtor_complete_alt(void* self) {
    CDefaultSceneNodeAnimatorFactory_dtor_complete(self);
}

// 0x6b9720 _ZNK6glitch5scene32CDefaultSceneNodeAnimatorFactory15getTypeFromNameEPKc
// glitch::scene::CDefaultSceneNodeAnimatorFactory::getTypeFromName. Unattached: the
// SceneNodeAnimatorTypeNames table is not declared; returns the not-found index 10.
std::int32_t CDefaultSceneNodeAnimatorFactory_getTypeFromName(void* self, const char* name) {
    (void)self;
    (void)name;
    return 10;
}

// 0x6b9928 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactory23createSceneNodeAnimatorENS0_26E_SCENE_NODE_ANIMATOR_TYPEEPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::createSceneNodeAnimator(type, node).
// The animator classes (flyCircle, flyStraight, ...) are not declared; the switch is not rebuilt.
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator(void* self, std::int32_t type, void* sceneNode) {
    (void)self;
    (void)type;
    (void)sceneNode;
    return nullptr;
}

// 0x6b9778 _ZN6glitch5scene32CDefaultSceneNodeAnimatorFactory23createSceneNodeAnimatorEPKcPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeAnimatorFactory::createSceneNodeAnimator(name, node).
// Looks up the type and delegates to the enum overload through the factory.
void* CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator_byName(void* self, const char* name, void* sceneNode) {
    const std::int32_t type = CDefaultSceneNodeAnimatorFactory_getTypeFromName(self, name);
    return CDefaultSceneNodeAnimatorFactory_createSceneNodeAnimator(self, type, sceneNode);
}

// 0x6b9da0 _ZNK6glitch5scene24CDefaultSceneNodeFactory30getCreateableSceneNodeTypeNameENS0_17E_SCENE_NODE_TYPEE
// glitch::scene::CDefaultSceneNodeFactory::getCreateableSceneNodeTypeName. Unattached: the
// type-name table entries (28-byte stride) are not declared; returns 0.
std::int32_t CDefaultSceneNodeFactory_getCreateableSceneNodeTypeName(void* self, std::int32_t type) {
    (void)self;
    (void)type;
    return 0;
}

// 0x6b9e58 _ZNK6glitch5scene24CDefaultSceneNodeFactory15getTypeFromNameEPKc
// glitch::scene::CDefaultSceneNodeFactory::getTypeFromName. Unattached: the type-name
// table is not declared; returns the not-found value 1852534389.
std::int32_t CDefaultSceneNodeFactory_getTypeFromName(void* self, const char* name) {
    (void)self;
    (void)name;
    return 1852534389;
}

// 0x6b9f04 _ZN6glitch5scene24CDefaultSceneNodeFactory12addSceneNodeEPKcPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeFactory::addSceneNode(name, parent). Looks up the type and
// delegates to the enum overload through the factory.
void* CDefaultSceneNodeFactory_addSceneNode_byName(void* self, const char* name, void* parent) {
    const std::int32_t type = CDefaultSceneNodeFactory_getTypeFromName(self, name);
    return CDefaultSceneNodeFactory_addSceneNode(self, type, parent);
}

// 0x6b9f30 _ZN6glitch5scene24CDefaultSceneNodeFactory12addSceneNodeENS0_17E_SCENE_NODE_TYPEEPNS0_10ISceneNodeE
// glitch::scene::CDefaultSceneNodeFactory::addSceneNode(type, parent). The per-type
// switch (about 2 KB of constructors) is not rebuilt; returns nullptr.
void* CDefaultSceneNodeFactory_addSceneNode(void* self, std::int32_t type, void* parent) {
    (void)self;
    (void)type;
    (void)parent;
    return nullptr;
}

// 0x6ba774 _ZN6glitch5scene24CDefaultSceneNodeFactory18SSceneNodeTypePairC1ENS0_17E_SCENE_NODE_TYPEEPKc
// glitch::scene::CDefaultSceneNodeFactory::SSceneNodeTypePair::SSceneNodeTypePair. The
// name range initialization is not modelled; the type is not stored.
void CDefaultSceneNodeFactory_SSceneNodeTypePair_ctor(void* self, std::int32_t type, const char* name) {
    (void)self;
    (void)type;
    (void)name;
}

// 0x6ba8f8 _ZN6glitch5scene24CDefaultSceneNodeFactoryD1Ev
// glitch::scene::CDefaultSceneNodeFactory::~CDefaultSceneNodeFactory (complete). The
// type-pair vector destruction is stubbed.
void CDefaultSceneNodeFactory_dtor_complete(void* self) {
    (void)self;
}

// 0x6baae8 _ZN6glitch5scene24CDefaultSceneNodeFactoryC2EPNS0_13CSceneManagerEPNS_3gui14ICursorControlERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
// glitch::scene::CDefaultSceneNodeFactory::CDefaultSceneNodeFactory. The 1.2 KB
// constructor (type-name table and file-system reference) is not rebuilt.
void CDefaultSceneNodeFactory_ctor(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                   void* fileSystem) {
    (void)self;
    (void)manager;
    (void)cursorControl;
    (void)fileSystem;
}

// 0x6bb008 _ZN6glitch5scene24CDefaultSceneNodeFactoryC1EPNS0_13CSceneManagerEPNS_3gui14ICursorControlERKN5boost13intrusive_ptrINS_2io11IFileSystemEEE
// glitch::scene::CDefaultSceneNodeFactory::CDefaultSceneNodeFactory (second variant).
void CDefaultSceneNodeFactory_ctor_alt(void* self, glitch::scene::CSceneManager* manager, void* cursorControl,
                                       void* fileSystem) {
    CDefaultSceneNodeFactory_ctor(self, manager, cursorControl, fileSystem);
}

// 0x6bb540 _ZN6glitch5scene29CDummyTransformationSceneNode31getRelativeTransformationMatrixEv
// glitch::scene::CDummyTransformationSceneNode::getRelativeTransformationMatrix. The
// matrix member at +0x130 is not declared in scene.hpp; returns nullptr.
void* CDummyTransformationSceneNode_getRelativeTransformationMatrix(void* self) {
    (void)self;
    return nullptr;
}

// 0x6bb568 _ZN6glitch5scene29IDummyTransformationSceneNodeD1Ev
// glitch::scene::IDummyTransformationSceneNode::~IDummyTransformationSceneNode (complete).
// The base destruction is compiler-generated; nothing else is done.
void IDummyTransformationSceneNode_dtor_complete(void* self) {
    (void)self;
}

// 0x6bb5d4 _ZN6glitch5scene29CDummyTransformationSceneNodeD1Ev
// glitch::scene::CDummyTransformationSceneNode::~CDummyTransformationSceneNode (complete).
void CDummyTransformationSceneNode_dtor_complete(void* self) {
    (void)self;
}

// 0x6bb65c _ZN6glitch5scene29CDummyTransformationSceneNode31setRelativeTransformationMatrixERKNS_4core8CMatrix4IfEE
// glitch::scene::CDummyTransformationSceneNode::setRelativeTransformationMatrix. The matrix
// copy and the virtual translation/rotation/scale setters are not rebuilt.
void CDummyTransformationSceneNode_setRelativeTransformationMatrix(void* self, const float* matrix) {
    (void)self;
    (void)matrix;
}

// 0x6bb6f0 _ZN6glitch5scene29IDummyTransformationSceneNodeC2Ei
// glitch::scene::IDummyTransformationSceneNode::IDummyTransformationSceneNode. The
// ISceneNode base constructor is not declared; the identity defaults are not applied.
void IDummyTransformationSceneNode_ctor(void* self, const void* parent, std::int32_t id) {
    (void)self;
    (void)parent;
    (void)id;
}

// 0x6bb77c _ZN6glitch5scene29CDummyTransformationSceneNodeC1Ei
// glitch::scene::CDummyTransformationSceneNode::CDummyTransformationSceneNode. Depends on
// the base constructor above, so the body is a stub.
void CDummyTransformationSceneNode_ctor(void* self, std::int32_t id) {
    IDummyTransformationSceneNode_ctor(self, nullptr, id);
}

// 0x6bb858 _ZN6glitch5scene29CDummyTransformationSceneNode5cloneEv
// glitch::scene::CDummyTransformationSceneNode::clone. Depends on the base clone-members
// and the matrix copy; returns nullptr.
void* CDummyTransformationSceneNode_clone(void* self) {
    (void)self;
    return nullptr;
}

// 0x6bb940 _ZN6glitch5scene29CDummyTransformationSceneNodeC2Ei
// glitch::scene::CDummyTransformationSceneNode::CDummyTransformationSceneNode (second variant).
void CDummyTransformationSceneNode_ctor_alt(void* self, const void* parent, std::int32_t id) {
    IDummyTransformationSceneNode_ctor(self, parent, id);
}

// CMesh (0x6bbcc4 SBuffer assign, 0x6bc298 D2, 0x6bc2cc recalculateBoundingBox, 0x6bc3e4 clone): canonical members in
// src/scene/CMesh.cpp.

// 0x6be5b0 _ZN6glitch5scene10CMeshCache14getMeshByIndexEj
// glitch::scene::CMeshCache::getMeshByIndex. Unattached: the mesh cache layout is not
// declared; returns nullptr.
void* CMeshCache_getMeshByIndex(void* self, std::uint32_t index) {
    (void)self;
    (void)index;
    return nullptr;
}

// 0x6be6d4 _ZN6glitch5scene10CMeshCache12isMeshLoadedEPKc
// glitch::scene::CMeshCache::isMeshLoaded. Unattached: the lookup goes through the cache
// vtable slot +0x28, which is not declared; returns false.
bool CMeshCache_isMeshLoaded(void* self, const char* name) {
    (void)self;
    (void)name;
    return false;
}

}  // namespace glitch::collada::recon_25
