#include "core-platform/obj_str.hpp"

#include <cstdio>
#include <cstdlib>

// 0x30eea0 _Z8ObjToStrPcRi
void ObjToStr(char* out, int& value) {
    std::sprintf(out, "%d", value);
}

// 0x30ef50 _Z8ObjToStrPcRN6glitch4core8vector3dIfEE
void ObjToStr(char* out, glitch::core::vector3d<float>& value) {
    std::sprintf(out, "%f,%f,%f", value.X, value.Y, value.Z);
}

// 0x30f03c _Z8StrToObjPKcRi
void StrToObj(const char* text, int& value) {
    value = std::atoi(text);
}
