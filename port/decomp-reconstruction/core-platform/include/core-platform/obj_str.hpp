// ObjToStr / StrToObj: text (de)serialisation helpers used by the original
// property and config code. Reconstructed from libDungeonHunter2.so pseudocode.
#pragma once

#include "core-platform/deps/game_types.hpp"
#include "core-platform/deps/glitch_core.hpp"

#include <vector>

void ObjToStr(char* out, int& value);
void ObjToStr(char* out, bool& value);
void ObjToStr(char* out, float& value);
void ObjToStr(char* out, Point2D<int>& value);
void ObjToStr(char* out, glitch::core::vector3d<float>& value);
void ObjToStr(char* out, std::vector<Point3D<float>>& value);
void StrToObj(const char* text, int& value);
void StrToObj(const char* text, bool& value);
void StrToObj(const char* text, float& value);
void StrToObj(const char* text, std::vector<Point3D<float>>& value);
