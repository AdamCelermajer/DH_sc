#pragma once
#include <cstdint>
// Source matrix inverse/local point. 0 success, -1 malformed/overlap; output
// stays untouched on rejection. Singular source matrix becomes identity with
// finite-clamped negative translation, not a failed hit-test.
extern "C" {
int dh2_ui_swf_inverse(float out[6],const float matrix[6]);
int dh2_ui_swf_inverse_point(float out[2],const float matrix[6],const float point[2]);
}
