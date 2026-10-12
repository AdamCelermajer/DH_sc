// ObjToStr / StrToObj: text (de)serialisation helpers used by the original
// property and config code. Reconstructed from libDungeonHunter2.so pseudocode.
#pragma once

#include "core-platform/deps/glitch_core.hpp"

void ObjToStr(char* out, int& value);
void ObjToStr(char* out, glitch::core::vector3d<float>& value);
void StrToObj(const char* text, int& value);
