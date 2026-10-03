// Reuse the original DCT1 corpus reader and atomic/resource lease checks.
// Angle channels add original cosf calls (trace kind3) to the existing libm
// input/output fixtures. Other test paths retain the full-node corpus format.
#include "dynamic_compiled_transforms.cpp"
#ifndef DH2_TRANSFORM_ORACLE
extern "C" float __wrap_cosf(float value){return math(3,value);}
#endif
