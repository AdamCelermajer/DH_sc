// Slice 24 reconstruction (glitch init steps and CGUIComboBox selection/alignment).
#include "glitch-scene-collada-gui/recon/slice_24.hpp"

namespace glitch {

int InitializationCount = 0;

// 0x6a0f08 _ZN6glitch22getInitializationStepsEv
std::vector<void (*)(bool)>* getInitializationSteps() {
    static std::vector<void (*)(bool)> steps;
    return &steps;
}

// 0x6a0f90 _ZN6glitch4exitEv
bool exit() {
    if (--InitializationCount != 0) {
        return false;
    }
    std::vector<void (*)(bool)>& steps = *getInitializationSteps();
    for (std::size_t i = steps.size(); i-- > 0;) {
        steps[i](false);
    }
    return InitializationCount == 0;
}

// 0x6a1024 _ZN6glitch4initEv
char* init() {
    int count = InitializationCount;
    if (count == 0) {
        std::vector<void (*)(bool)>& steps = *getInitializationSteps();
        const std::size_t size = steps.size();
        for (std::size_t i = 0; i < size; ++i) {
            steps[i](true);
        }
        count = InitializationCount;
    }
    const int next = count + 1;
    InitializationCount = next;
    return next == 1 ? reinterpret_cast<char*>(1) : nullptr;
}

// 0x6a10b0 _ZN6glitch16registerInitStepEPFvbE
int registerInitStep(void (*step)(bool)) {
    std::vector<void (*)(bool)>& steps = *getInitializationSteps();
    steps.push_back(step);
    return static_cast<int>(steps.size());
}

}  // namespace glitch

namespace glitch::gui::recon_24 {

// 0x6aaa64 _ZN6glitch3gui12CGUIComboBox16setTextAlignmentENS0_14EGUI_ALIGNMENTES2_
void setTextAlignment(CGUIComboBox* combo, std::int32_t horizontal, std::int32_t vertical) {
    combo->horizontalAlignment = horizontal;  // +0x17c
    combo->verticalAlignment = vertical;      // +0x180
    // partial: the original then calls vtable slot +160 on textElement to refresh the
    // text element; that slot is not declared in layouts/gui.hpp, so the call is omitted.
}

// 0x6aab1c _ZN6glitch3gui12CGUIComboBox11setSelectedEi
void setSelected(CGUIComboBox* combo, std::int32_t index) {
    if (index < -1) {
        return;
    }
    // Items are 72-byte entries (+0x164 begin, +0x168 end), so the count is the byte span / 72.
    const std::ptrdiff_t span = static_cast<char*>(combo->itemsEnd) - static_cast<char*>(combo->itemsBegin);
    const auto count = static_cast<std::int32_t>(span / 72);
    if (index >= count) {
        return;
    }
    combo->selected = index;  // +0x170
    // partial: the original then calls vtable slot +68 on the text element with the item text
    // (or the empty string for -1); that slot is not declared in layouts/gui.hpp.
}

}  // namespace glitch::gui::recon_24
