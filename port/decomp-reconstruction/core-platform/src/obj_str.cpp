#include "core-platform/obj_str.hpp"

#include <cstdio>
#include <cstdlib>

// 0x30eea0 _Z8ObjToStrPcRi
void ObjToStr(char* out, int& value) {
    std::sprintf(out, "%d", value);
}

// 0x30eeb4 _Z8ObjToStrPcRb
void ObjToStr(char* out, bool& value) {
    std::sprintf(out, "%d", static_cast<unsigned>(value));
}

// 0x30eec8 _Z8ObjToStrPcRf
void ObjToStr(char* out, float& value) {
    std::sprintf(out, "%f", static_cast<double>(value));
}

// 0x30ee84 _Z8ObjToStrPcR7Point2DIiE
void ObjToStr(char* out, Point2D<int>& value) {
    std::sprintf(out, "%d,%d", value.X, value.Y);
}

// 0x30ef50 _Z8ObjToStrPcRN6glitch4core8vector3dIfEE
void ObjToStr(char* out, glitch::core::vector3d<float>& value) {
    std::sprintf(out, "%f,%f,%f", value.X, value.Y, value.Z);
}

// 0x30ee60 _Z8ObjToStrPcRSt6vectorI7Point3DIfESaIS2_EE
// The original body is empty: a vector of points is not serialised.
void ObjToStr(char*, std::vector<Point3D<float>>&) {}

// 0x30f03c _Z8StrToObjPKcRi
void StrToObj(const char* text, int& value) {
    value = std::atoi(text);
}

// 0x30f020 _Z8StrToObjPKcRb
void StrToObj(const char* text, bool& value) {
    value = std::atoi(text) != 0;
}

// 0x30f288 _Z8StrToObjPKcRf
void StrToObj(const char* text, float& value) {
    value = static_cast<float>(std::strtod(text, nullptr));
}

// 0x30f010 _Z8StrToObjPKcRSt6vectorI7Point3DIfESaIS3_EE
// Partial: the original only emits a "NEED TO CHECK" debug line and leaves the
// vector untouched. The debug sink is an original debug-output function that is
// not recovered; plain stdout is used here.
void StrToObj(const char*, std::vector<Point3D<float>>&) {
    std::printf("%s\n", "NEED TO CHECK Venkat Vikram ********** ********** ************");
}
