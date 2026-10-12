// Slice 24 reconstruction declarations (glitch init steps and two CGUIComboBox setters).
// Member offsets come from glitch-scene-collada-gui/layouts/gui.hpp only.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/layouts/gui.hpp"

namespace glitch {

// 0x6a0f08 _ZN6glitch22getInitializationStepsEv
std::vector<void (*)(bool)>* getInitializationSteps();

// Global init nesting counter (glitch::InitializationCount); defined in src/recon/slice_24.cpp.
extern int InitializationCount;

// 0x6a0f90 _ZN6glitch4exitEv
bool exit();
// 0x6a1024 _ZN6glitch4initEv
char* init();
// 0x6a10b0 _ZN6glitch16registerInitStepEPFvbE
int registerInitStep(void (*step)(bool));

}  // namespace glitch

namespace glitch::gui::recon_24 {

// 0x6aaa64 _ZN6glitch3gui12CGUIComboBox16setTextAlignmentENS0_14EGUI_ALIGNMENTES2_
void setTextAlignment(CGUIComboBox* combo, std::int32_t horizontal, std::int32_t vertical);

// 0x6aab1c _ZN6glitch3gui12CGUIComboBox11setSelectedEi
void setSelected(CGUIComboBox* combo, std::int32_t index);

}  // namespace glitch::gui::recon_24
