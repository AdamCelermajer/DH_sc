#pragma once
// Exact Point3D<float>::normalize34d0b0, distinct from glitch vector3d's
// zero-guarded reciprocal-multiply normalize. IEEE zero/NaN are preserved.
extern "C" float* dh2_point3d_normalize_v2(float* xyz);
