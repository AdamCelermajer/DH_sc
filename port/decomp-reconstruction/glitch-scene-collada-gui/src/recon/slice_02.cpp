// Slice 02: glitch::gui list box, context menu, mesh viewer, message box, modal screen,
// scroll bar, skin and spin box. Types come from include/.../recon/slice_02.hpp.
// Each function starts with its original address and mangled symbol.

#include "glitch-scene-collada-gui/recon/slice_02.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <utility>

namespace glitch::collada::recon_02 {

std::uint32_t getTimeMs() {
    using namespace std::chrono;
    return static_cast<std::uint32_t>(
        duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

// ---------------------------------------------------------------------------
// IGUIElement stand-in
// ---------------------------------------------------------------------------

// 0x534e48-era base behaviour: forward to the parent.
bool IGUIElement::onEvent(const SEvent& event) {
    if (parent) {
        return parent->onEvent(event);
    }
    return false;
}

void IGUIElement::addChild(IGUIElement* child) {
    if (!child) {
        return;
    }
    children.push_back(child);
    child->parent = this;
}

void IGUIElement::removeChild(IGUIElement* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->parent = nullptr;
    }
}

// 0x542840 _ZNK6glitch3gui11IGUIElement14getNextElementEibbRPS1_S3_b
// Depth-first search for the next element in tab order. The pseudocode walks the
// child list and keeps two candidates: `best` (smallest order above the sentinel)
// and `current` (the last element visited).
bool IGUIElement::getNextElement(bool groupFlag, IGUIElement*& current, IGUIElement*& best,
                                 bool recursive) const {
    for (IGUIElement* child : children) {
        if (!child->isEnabled() && !recursive) {
            continue;
        }
        if (!groupFlag && child->tabGroup) {
            continue;
        }
        if (!child->tabStop || child->tabGroup != groupFlag) {
            if (child->getNextElement(groupFlag, current, best, false)) {
                return true;
            }
            continue;
        }
        const int order = child->tabOrder;
        if (order == 0x40000000) {
            best = child;
            return true;
        }
        if (!best) {
            if (order < -1) {
                best = child;
            }
        } else {
            const bool better = order > best->tabOrder && order < -1;
            if (better) {
                best = child;
            }
        }
        if (current && order <= current->tabOrder) {
            if (child->getNextElement(groupFlag, current, best, false)) {
                return true;
            }
            continue;
        }
        current = child;
        if (child->getNextElement(groupFlag, current, best, false)) {
            return true;
        }
    }
    return false;
}

// 0x542994 _ZN6glitch3gui11IGUIElement10setTabOrderEi
void IGUIElement::setTabOrder(int unusedArgument) {
    (void)unusedArgument;
    tabOrder = 0;
    IGUIElement* root = this;
    if (tabGroup) {
        if (parent) {
            root = parent;
            for (IGUIElement* i = parent->parent; i; i = i->parent) {
                root = i;
            }
        }
    } else {
        // Nearest ancestor that is a tab group; without one the order stays 0.
        IGUIElement* group = nullptr;
        for (IGUIElement* i = parent; i; i = i->parent) {
            if (i->tabGroup) {
                group = i;
                break;
            }
        }
        if (!group) {
            return;
        }
        root = group;
    }
    IGUIElement* current = nullptr;
    IGUIElement* best = nullptr;
    root->getNextElement(tabGroup, current, best, true);
    if (current) {
        tabOrder = current->tabOrder + 1;
    }
}

// ---------------------------------------------------------------------------
// CGUIScrollBar
// ---------------------------------------------------------------------------

// 0x548c34 _ZNK6glitch3gui12CGUIScrollBar18getPosFromMousePosEii
int CGUIScrollBar::getPosFromMousePos(int mouseX, int mouseY) const {
    if (horizontal) {
        const float breadth = static_cast<float>(relativeRect.y1 - relativeRect.y0);
        const float numerator = static_cast<float>(mouseX - absoluteRect.x0) + breadth * -1.5f;
        const float denominator =
            static_cast<float>(relativeRect.x1 - relativeRect.x0) + breadth * -3.0f;
        return static_cast<int>((numerator / denominator) * static_cast<float>(max));
    }
    const float breadth = static_cast<float>(relativeRect.x1 - relativeRect.x0);
    const float numerator = static_cast<float>(mouseY - absoluteRect.y0) + breadth * -1.5f;
    const float denominator =
        static_cast<float>(relativeRect.y1 - relativeRect.y0) + breadth * -3.0f;
    return static_cast<int>((numerator / denominator) * static_cast<float>(max));
}

// 0x548d90 _ZN6glitch3gui12CGUIScrollBar6setPosEi
void CGUIScrollBar::setPos(int value) {
    int clamped = value;
    if (value < 0) {
        clamped = 0;
        pos = 0;
    } else if (value <= max) {
        pos = value;
    } else {
        pos = max;
        clamped = max;
    }
    if (horizontal) {
        const int length = relativeRect.y1 - relativeRect.y0;
        const float span = static_cast<float>(relativeRect.x1 - relativeRect.x0) +
                           static_cast<float>(length) * -3.0f;
        const float step = span / static_cast<float>(max);
        thumbPos = static_cast<int>(static_cast<float>(clamped) * step +
                                    static_cast<float>(length) * 0.5f);
        thumbSize = length;
    } else {
        const int width = relativeRect.x1 - relativeRect.x0;
        const float span = static_cast<float>(relativeRect.y1 - relativeRect.y0) +
                           static_cast<float>(width) * -3.0f;
        const float step = max ? span / static_cast<float>(max) : 0.0f;
        thumbPos = static_cast<int>(static_cast<float>(clamped) * step +
                                    static_cast<float>(width) * 0.5f);
        thumbSize = width;
    }
}

// 0x548f6c _ZN6glitch3gui12CGUIScrollBar6setMaxEi
void CGUIScrollBar::setMax(int value) {
    const bool positive = value > 0;
    max = positive ? value : 0;
    if (upButton) {
        upButton->setEnabled(positive);
    }
    if (downButton) {
        downButton->setEnabled(positive);
    }
    setPos(pos);
}

// 0x5490c0 _ZN6glitch3gui12CGUIScrollBar14refreshControlsEv
// Partial: the two arrow buttons are positioned here. Their creation
// (operator new of the CGUIButton record, 0x1e4 bytes) and the skin sprite
// assignment are not ported, so the caller must supply the buttons.
void CGUIScrollBar::refreshControls() {
    if (horizontal) {
        const int size = relativeRect.y1 - relativeRect.y0;
        if (upButton) {
            upButton->setRelativePosition(SRect{0, 0, size, size});
        }
        if (downButton) {
            const int width = relativeRect.x1 - relativeRect.x0;
            downButton->setRelativePosition(SRect{width - size, 0, width, size});
        }
    } else {
        const int size = relativeRect.x1 - relativeRect.x0;
        if (upButton) {
            upButton->setRelativePosition(SRect{0, 0, size, size});
        }
        if (downButton) {
            const int height = relativeRect.y1 - relativeRect.y0;
            downButton->setRelativePosition(SRect{0, height - size, size, height});
        }
    }
}

// 0x5496b4 _ZN6glitch3gui12CGUIScrollBar20updateAbsolutePositionEv
// Partial: the base update and the control refresh are ported. The thumb
// recomputation that follows in the original is not ported here.
void CGUIScrollBar::updateAbsolutePosition() {
    IGUIElement::updateAbsolutePosition();
    refreshControls();
}

// 0x54a228 _ZN6glitch3gui12CGUIScrollBar7onEventERKNS_6SEventE
// Stub: the mouse drag and button dispatch are not ported in this pass.
bool CGUIScrollBar::onEvent(const SEvent& event) {
    return IGUIElement::onEvent(event);
}

// 0x54a758 _ZN6glitch3gui12CGUIScrollBar4drawEv
// Stub: the track and thumb drawing through the skin is not ported in this pass.
void CGUIScrollBar::draw() {
    if (!visible) {
        return;
    }
    for (IGUIElement* child : children) {
        child->draw();
    }
}

// ---------------------------------------------------------------------------
// CGUIListBox
// ---------------------------------------------------------------------------

// 0x541e18 _ZN6glitch3gui11CGUIListBox20recalculateScrollPosEv
void CGUIListBox::recalculateScrollPos() {
    if (!scrollEnabled || !scrollBar) {
        return;
    }
    const int span = (selected == -1) ? defaultItemHeight : itemHeight * selected;
    const int current = scrollBar->getPos();
    const int delta = span - current;
    if (delta < 0) {
        scrollBar->setPos(current + delta);
    } else if (delta > absoluteRect.height() - itemHeight) {
        scrollBar->setPos(itemHeight - absoluteRect.height() + delta + current);
    }
}

// 0x541ef4 _ZN6glitch3gui11CGUIListBox20recalculateItemWidthEj
// Partial: only the sprite-bank bounds check and the max-width update are ported.
// The original reads the item's sprite rectangle through the bank vtable.
void CGUIListBox::recalculateItemWidth(std::uint32_t index) {
    if (!selectIcons || index >= static_cast<std::uint32_t>(items.size())) {
        return;
    }
    if (static_cast<std::uint32_t>(index) >= static_cast<std::uint32_t>(selectIcons->spriteCount())) {
        return;
    }
    const int width = selectIcons->spriteWidth(index);
    if (maxItemWidth < width) {
        maxItemWidth = width;
    }
}

// 0x542050 _ZN6glitch3gui11CGUIListBox19setItemOverrideColorEjNS0_17EGUI_LISTBOX_COLORERKNS_5video6SColorE
void CGUIListBox::setItemOverrideColor(std::uint32_t index, int colorIndex, std::uint32_t color) {
    if (index >= items.size() || colorIndex < 0 || colorIndex > 3) {
        return;
    }
    SListItem& item = items[index];
    item.overrideSet[colorIndex] = true;
    item.overrideColor[colorIndex] = color;
}

// 0x542258 _ZNK6glitch3gui11CGUIListBox19getItemDefaultColorENS0_17EGUI_LISTBOX_COLORE
// Partial: the original reads the same skin sub-object for all four colour cases and
// ignores the index. The stand-in forwards the index to IGUISkin::getColor.
std::uint32_t CGUIListBox::getItemDefaultColor(int colorIndex) const {
    if (!environment || !environment->skin) {
        return 0;
    }
    if (colorIndex < 0 || colorIndex > 3) {
        return 0;
    }
    return environment->skin->getColor(colorIndex);
}

// 0x542400 _ZN6glitch3gui11CGUIListBox9selectNewEib
// The first argument is a vertical position in list space (a mouse y), which the
// original converts to an item index.
void CGUIListBox::selectNew(int position, bool setBySecond) {
    const std::uint32_t now = getTimeMs();
    const int previous = selected;
    if (itemHeight != 0) {
        const int scroll = scrollBar ? scrollBar->getPos() : 0;
        selected = (position - absoluteRect.y0 - 1 + scroll) / itemHeight;
    }
    const int count = static_cast<int>(items.size());
    if (selected >= 0) {
        if (selected >= count) {
            selected = count - 1;
        }
    } else {
        selected = 0;
    }
    recalculateScrollPos();
    if (parent && !setBySecond) {
        SEvent event;
        event.category = 0;
        event.element = this;
        event.eventCode = (selected == previous && now < lastSelectTime + 500) ? 9 : 8;
        parent->onEvent(event);
    }
    lastSelectTime = now;
}

// 0x542518 _ZN6glitch3gui11CGUIListBox11setSelectedEj
void CGUIListBox::setSelected(std::uint32_t index) {
    if (index < items.size()) {
        selected = static_cast<int>(index);
    } else {
        selected = -1;
    }
    lastSelectTime = getTimeMs();
    recalculateScrollPos();
}

// 0x542f54 _ZN6glitch3gui11CGUIListBox7onEventERKNS_6SEventE
// Partial: the GUI sub-events (mouse arm, select, scroll), the arrow, page, home and
// end keys, and the forward-to-parent fallback are ported. Not ported: the mouse
// category (0) click and hover branches, which forward to the parent here, and the
// type-ahead search over item text (a typed character is forwarded).
bool CGUIListBox::onEvent(const SEvent& event) {
    if (!enabled) {
        return parent ? parent->onEvent(event) : false;
    }
    if (event.category == 1) {
        switch (event.guiCode) {
            case 0:
                mouseSelectArmed = true;
                return true;
            case 3: {
                mouseSelectArmed = false;
                if (!canSelect(event)) {
                    return true;
                }
                selectNew(event.keyOrRelated, false);
                return true;
            }
            case 6:
                if ((!mouseSelectArmed && !allowKeySelect) || !canSelect(event)) {
                    return parent ? parent->onEvent(event) : false;
                }
                selectNew(event.keyOrRelated, true);
                return true;
            case 7:
                if (scrollBar) {
                    scrollBar->setPos(scrollBar->getPos() - static_cast<int>(10.0f * event.amount));
                }
                return true;
            default:
                return parent ? parent->onEvent(event) : false;
        }
    }
    if (event.category == 2) {
        if (!event.keyDown) {
            if (event.keyOrRelated == kKeyReturn || event.keyOrRelated == kKeySpace) {
                if (parent) {
                    SEvent sent;
                    sent.category = 0;
                    sent.element = this;
                    sent.eventCode = 9;
                    parent->onEvent(sent);
                    return true;
                }
                return true;
            }
            return parent ? parent->onEvent(event) : false;
        }
        const int count = static_cast<int>(items.size());
        const int before = selected;
        const int pageSpan = (itemHeight != 0)
                                 ? (absoluteRect.height() - itemHeight) / itemHeight
                                 : 0;
        int next = selected;
        switch (event.keyOrRelated) {
            case kKeyHome:
                next = 0;
                break;
            case kKeyEnd:
                next = count - 1;
                break;
            case kKeyPageDown:
                next = selected + pageSpan;
                break;
            case kKeyPageUp:
                next = selected - pageSpan;
                break;
            case kKeyUp:
                next = selected - 1;
                break;
            case kKeyDown:
                next = selected + 1;
                break;
            default:
                return parent ? parent->onEvent(event) : false;
        }
        if (next < count) {
            if (next < 0) {
                selected = 0;
            } else {
                selected = next;
            }
        } else {
            selected = count - 1;
        }
        recalculateScrollPos();
        if (selected != before && parent && !mouseSelectArmed && !allowKeySelect) {
            SEvent sent;
            sent.category = 0;
            sent.element = this;
            sent.eventCode = 8;
            parent->onEvent(sent);
        }
        return true;
    }
    return parent ? parent->onEvent(event) : false;
}

// 0x5439b4 _ZN6glitch3gui11CGUIListBox4drawEv
// Stub: the row and icon drawing through the skin is not ported in this pass; the
// child loop is ported.
void CGUIListBox::draw() {
    if (!visible) {
        return;
    }
    for (IGUIElement* child : children) {
        child->draw();
    }
}

// 0x544280 _ZN6glitch3gui11CGUIListBox5clearEv
void CGUIListBox::clear() {
    items.clear();
    selected = -1;
    maxItemWidth = 0;
    if (scrollBar) {
        // The original calls a one-argument slot (+152) here; mapped to refreshControls.
        scrollBar->refreshControls();
    }
    recalculateItemHeight();
}

// 0x544390 _ZN6glitch3gui11CGUIListBox10removeItemEj
void CGUIListBox::removeItem(std::uint32_t index) {
    if (index >= items.size()) {
        return;
    }
    if (selected == static_cast<int>(index)) {
        selected = -1;
    } else if (static_cast<int>(index) < selected) {
        selected = selected - 1;
        lastSelectTime = getTimeMs();
    }
    items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
    recalculateItemHeight();
}

// 0x5446dc _ZN6glitch3gui11CGUIListBox9swapItemsEjj
// Partial: the records are exchanged with std::swap. The original copies the
// fields one by one and this pass did not check the copy order against it.
void CGUIListBox::swapItems(std::uint32_t a, std::uint32_t b) {
    if (a >= items.size() || b >= items.size()) {
        return;
    }
    std::swap(items[a], items[b]);
}

// 0x544828 _ZN6glitch3gui11CGUIListBox7setItemEjPKwi
void CGUIListBox::setItem(std::uint32_t index, const wchar_t* value, int data) {
    if (index >= items.size()) {
        return;
    }
    items[index].text = value ? value : L"";
    items[index].data = data;
    recalculateItemHeight();
    recalculateItemWidth(static_cast<std::uint32_t>(data));
}

// 0x5448ac _ZNK6glitch3gui11CGUIListBox22getSerializationLabelsENS0_17EGUI_LISTBOX_COLORERNSt...
bool CGUIListBox::getSerializationLabels(int colorIndex, std::string& useLabel,
                                         std::string& colorLabel) const {
    switch (colorIndex) {
        case 0:
            useLabel = "UseColText";
            colorLabel = "ColText";
            return true;
        case 1:
            useLabel = "UseColTextHl";
            colorLabel = "ColTextHl";
            return true;
        case 2:
            useLabel = "UseColIcon";
            colorLabel = "ColIcon";
            return true;
        case 3:
            useLabel = "UseColIconHl";
            colorLabel = "ColIconHl";
            return true;
        default:
            return false;
    }
}

// 0x545304 _ZN6glitch3gui11CGUIListBox10insertItemEjPKwi
std::uint32_t CGUIListBox::insertItem(std::uint32_t index, const wchar_t* value, int data) {
    SListItem item;
    item.text = value ? value : L"";
    item.data = data;
    const std::uint32_t position = std::min<std::uint32_t>(index, static_cast<std::uint32_t>(items.size()));
    items.insert(items.begin() + static_cast<std::ptrdiff_t>(position), item);
    recalculateItemHeight();
    recalculateItemWidth(static_cast<std::uint32_t>(data));
    return index;
}

// 0x545480 _ZN6glitch3gui11CGUIListBox7addItemEPKwi
std::uint32_t CGUIListBox::addItem(const wchar_t* value, int data) {
    SListItem item;
    item.text = value ? value : L"";
    item.data = data;
    items.push_back(item);
    recalculateItemHeight();
    recalculateItemWidth(static_cast<std::uint32_t>(data));
    return static_cast<std::uint32_t>(items.size() - 1);
}

// ---------------------------------------------------------------------------
// CGUIMenu
// ---------------------------------------------------------------------------

// 0x5455b8 _ZN6glitch3gui8CGUIMenu13recalculateSizeEv
// Partial: the item height (font height plus 5) and the stacking of the item
// records are ported. The per-item text measurement and the submenu padding are
// not ported.
void CGUIMenu::recalculateSize() {
    if (!environment || !environment->skin) {
        return;
    }
    IGUIFont* font = environment->skin->getFont(3);
    if (!font) {
        return;
    }
    const int rowHeight = font->getHeight() + 5;
    int y = 0;
    int widest = 0;
    for (SMenuItem& item : menuItems) {
        item.y = y;
        if (item.separator) {
            item.width = 0;
            item.height = rowHeight;
        } else {
            item.height = rowHeight;
        }
        y += item.height;
        widest = std::max(widest, item.width);
    }
    setRelativePosition(SRect{relativeRect.x0, relativeRect.y0, relativeRect.x0 + widest, relativeRect.y0 + y});
}

// 0x54585c _ZNK6glitch3gui8CGUIMenu7getRectERKNS0_16CGUIContextMenu5SItemERKNS_4core4rectIiEE
// Stub: the original calls a virtual on the item (vtable +200) that is not mapped.
SRect CGUIMenu::getRect(const SRect& base) const {
    return base;
}

// 0x545878 _ZN6glitch3gui8CGUIMenu20updateAbsolutePositionEv
void CGUIMenu::updateAbsolutePosition() {
    if (parent) {
        parentWidth = parent->absoluteRect.x1 - parent->absoluteRect.x0;
    }
    IGUIElement::updateAbsolutePosition();
}

// 0x545c18 _ZN6glitch3gui8CGUIMenu7onEventERKNS_6SEventE
// Stub: the mouse-hover and submenu open/close paths are not ported in this pass.
bool CGUIMenu::onEvent(const SEvent& event) {
    return IGUIElement::onEvent(event);
}

// 0x545fc4 _ZN6glitch3gui8CGUIMenu4drawEv
// Stub: item row drawing is not ported; the child loop is.
void CGUIMenu::draw() {
    if (!visible) {
        return;
    }
    for (IGUIElement* child : children) {
        child->draw();
    }
}

// ---------------------------------------------------------------------------
// CGUIMeshViewer
// ---------------------------------------------------------------------------

// 0x5462c4 _ZN6glitch3gui14CGUIMeshViewer7setMeshERKN5boost13intrusive_ptrINS_5scene14IAnimatedMeshEEE
void CGUIMeshViewer::setMesh(void* value) {
    mesh = value;
}

// 0x5462f0 _ZNK6glitch3gui14CGUIMeshViewer7getMeshEv
void* CGUIMeshViewer::getMesh() const {
    return mesh;
}

// 0x54630c _ZNK6glitch3gui14CGUIMeshViewer11getMaterialEv
void* CGUIMeshViewer::getMaterial() const {
    return material;
}

// 0x546348 _ZN6glitch3gui14CGUIMeshViewer11setMaterialERKN5boost13intrusive_ptrINS_5video9CMaterialEEE
void CGUIMeshViewer::setMaterial(void* value) {
    material = value;
}

// 0x5467ac _ZN6glitch3gui14CGUIMeshViewer7onEventERKNS_6SEventE
bool CGUIMeshViewer::onEvent(const SEvent& event) {
    return IGUIElement::onEvent(event);
}

// 0x5469fc _ZN6glitch3gui14CGUIMeshViewer4drawEv
// Stub: the mesh and material submission through the video driver is not ported.
void CGUIMeshViewer::draw() {
    if (!visible) {
        return;
    }
    for (IGUIElement* child : children) {
        child->draw();
    }
}

// ---------------------------------------------------------------------------
// CGUIMessageBox
// ---------------------------------------------------------------------------

// 0x547014 _ZN6glitch3gui14CGUIMessageBox7onEventERKNS_6SEventE
// Partial: key handling for Enter, Escape, Y and N is approximated from the pseudocode.
// The button-slot roles are not confirmed, and the key-up and focus hand-off paths are
// not ported.
bool CGUIMessageBox::onEvent(const SEvent& event) {
    if (!enabled) {
        return IGUIElement::onEvent(event);
    }
    if (event.category == 2 && event.keyDown) {
        IGUIElement* target = nullptr;
        switch (event.keyOrRelated) {
            case kKeyEscape:
                if (modalActive) {
                    for (IGUIElement* button : buttons) {
                        if (button) {
                            button->setEnabled(false);
                        }
                    }
                    modalActive = false;
                    return IGUIElement::onEvent(event);
                }
                target = buttons[1];
                break;
            case kKeyReturn:
                target = buttons[0];
                break;
            case kKeyN:
                target = buttons[3];
                break;
            case kKeyY:
                target = buttons[2];
                break;
            default:
                return IGUIElement::onEvent(event);
        }
        if (!target) {
            return IGUIElement::onEvent(event);
        }
        target->setEnabled(true);
        modalActive = true;
        return IGUIElement::onEvent(event);
    }
    return IGUIElement::onEvent(event);
}

// 0x5472e0 _ZN6glitch3gui14CGUIMessageBox15refreshControlsEv
// Stub: the button and text placement is not ported in this pass.
void CGUIMessageBox::refreshControls() {}

// ---------------------------------------------------------------------------
// CGUIModalScreen
// ---------------------------------------------------------------------------

// 0x548260 _ZN6glitch3gui14CGUIModalScreen20updateAbsolutePositionEv
void CGUIModalScreen::updateAbsolutePosition() {
    if (parent) {
        relativeRect = SRect{0, 0, parent->absoluteRect.width(), parent->absoluteRect.height()};
    }
    IGUIElement::updateAbsolutePosition();
}

// 0x5482c8 _ZN6glitch3gui14CGUIModalScreen12removeChildEPNS0_11IGUIElementE
// Partial: the base removal is ported. The original then calls a virtual hook
// (vtable +28) when the list is empty; that hook is not mapped.
void CGUIModalScreen::removeChild(IGUIElement* child) {
    IGUIElement::removeChild(child);
}

// 0x5482fc _ZN6glitch3gui14CGUIModalScreen8addChildEPNS0_11IGUIElementE
// Partial: the base addition is ported. The environment hook (vtable +16) that follows
// is not mapped.
void CGUIModalScreen::addChild(IGUIElement* child) {
    IGUIElement::addChild(child);
}

// 0x5488bc _ZN6glitch3gui14CGUIModalScreen4drawEv
// Partial: the blinking background pass is not ported. The child loop is.
void CGUIModalScreen::draw() {
    for (IGUIElement* child : children) {
        child->draw();
    }
}

// 0x548a50 _ZN6glitch3gui14CGUIModalScreen7onEventERKNS_6SEventE
// Partial: the click timestamp and the parent forwarding are ported. The focus
// hand-off to the environment (vtable +16) is not.
bool CGUIModalScreen::onEvent(const SEvent& event) {
    if (event.category == 0 && event.eventCode == 0) {
        lastClickTime = getTimeMs();
        if (parent) {
            return parent->onEvent(event);
        }
        return false;
    }
    return IGUIElement::onEvent(event);
}

// ---------------------------------------------------------------------------
// CGUISkin
// ---------------------------------------------------------------------------

// 0x54aac4 _ZNK6glitch3gui8CGUISkin7getFontENS0_17EGUI_DEFAULT_FONTE
IGUIFont* CGUISkin::getFont(int which) const {
    if (which >= 0 && which <= 9 && fonts[which]) {
        return fonts[which];
    }
    return fonts[0];
}

// 0x54aaf0 _ZN6glitch3gui8CGUISkin7setFontEPNS0_10IGUIFontENS0_17EGUI_DEFAULT_FONTE
// Partial: the reference count of the font is not modelled; the slot is replaced.
void CGUISkin::setFont(IGUIFont* font, int which) {
    if (which >= 0 && which <= 9) {
        fonts[which] = font;
    }
}

// 0x54ab40 _ZN6glitch3gui8CGUISkin13setSpriteBankEPNS0_14IGUISpriteBankE
// Partial: the reference count of the bank is not modelled.
void CGUISkin::setSpriteBank(IGUISpriteBank* bank) {
    spriteBank = bank;
}

// 0x54abb0 _ZN6glitch3gui8CGUISkin8drawIconEPNS0_11IGUIElementENS0_17EGUI_DEFAULT_ICONEN5glitch4core8position2dIiEEjbPKNS_4core4rectIiEE
// Stub: the sprite draw call (bank vtable +36) is not in the stand-in sprite bank.
void CGUISkin::drawIcon(IGUIElement* element, int icon, int x, int y, std::uint32_t time, bool center,
                        const SRect* clip) {
    (void)element; (void)icon; (void)x; (void)y; (void)time; (void)center; (void)clip;
}

// 0x54b0ac _ZN6glitch3gui8CGUISkin16draw3DTabButtonEPNS0_11IGUIElementEbRKNS_4core4rectIiEEPKS7_NS0_12EGUI_ALIGNMENTE
// Stub: the tab button frame drawing is not ported in this pass.
void CGUISkin::draw3DTabButton(IGUIElement* element, bool pressed, const SRect& rect, const SRect* clip,
                               int alignment) {
    (void)element; (void)pressed; (void)rect; (void)clip; (void)alignment;
}

// 0x54b910 _ZN6glitch3gui8CGUISkin14draw3DTabBodyEPNS0_11IGUIElementEbbRKNS_4core4rectIiEEPKS7_iNS0_12EGUI_ALIGNMENTE
// Stub: the tab body frame drawing is not ported in this pass.
void CGUISkin::draw3DTabBody(IGUIElement* element, bool border, bool background, const SRect& rect,
                             const SRect* clip, int tabHeight, int alignment) {
    (void)element; (void)border; (void)background; (void)rect; (void)clip; (void)tabHeight;
    (void)alignment;
}

// 0x54c40c _ZN6glitch3gui8CGUISkin22draw3DWindowBackgroundEPNS0_11IGUIElementEbNS_5video6SColorERKNS_4core4rectIiEEPKS8_
// Stub: the window background frame drawing is not ported in this pass.
void CGUISkin::draw3DWindowBackground(IGUIElement* element, bool border, std::uint32_t color,
                                      const SRect& rect, const SRect* clip) {
    (void)element; (void)border; (void)color; (void)rect; (void)clip;
}

// 0x54d0bc _ZN6glitch3gui8CGUISkin14setDefaultTextENS0_17EGUI_DEFAULT_TEXTEPKw
void CGUISkin::setDefaultText(int which, const wchar_t* text) {
    if (which >= 0 && which <= 7) {
        defaultText[which] = text ? text : L"";
    }
}

// ---------------------------------------------------------------------------
// CGUISpinBox
// ---------------------------------------------------------------------------

// 0x54df40 _ZN6glitch3gui11CGUISpinBox8setRangeEff
void CGUISpinBox::setRange(float low, float high) {
    rangeLow = low;
    rangeHigh = high;
    refresh();
}

// 0x54df7c _ZN6glitch3gui11CGUISpinBox16verifyValueRangeEv
void CGUISpinBox::verifyValueRange() {
    const float value = getValue();
    if (value < rangeLow) {
        setValue(rangeLow);
    } else if (value > rangeHigh) {
        setValue(rangeHigh);
    }
}

// 0x54dfd8 _ZN6glitch3gui11CGUISpinBox7setTextEPKw
void CGUISpinBox::setText(const wchar_t* value) {
    if (editBox) {
        editBox->setText(value);
    }
    setValue(getValue());
    refresh();
}

// 0x54e028 _ZNK6glitch3gui11CGUISpinBox7getTextEv
const wchar_t* CGUISpinBox::getText() const {
    return editBox ? editBox->getText() : L"";
}

}  // namespace glitch::collada::recon_02
