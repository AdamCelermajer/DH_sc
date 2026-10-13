// Test for batch v1-vendor-gameswf-gameswf-fddef6-4: edges_intersect_sub (float overload).
// The expected values are worked out by hand from the binary's control flow (Hex-Rays pseudocode):
// *e0_vs_e1 and *e1_vs_e0 take -1, 0 or +1, and the return value is det00 <= 0.
// Returns non-zero on failure.

#include "base/tu_types.h"
#include "base/utility.h"
#include "base/vert_types.h"

#include <cstdio>

namespace {

int g_failures = 0;

#define EXPECT(cond)                                                  \
	do {                                                              \
		if (!(cond)) {                                                \
			std::printf("FAIL line %d: %s\n", __LINE__, #cond);     \
			++g_failures;                                             \
		}                                                             \
	} while (0)

vec2<float> P(float x, float y)
{
	return vec2<float>(x, y);
}

}  // namespace

int main()
{
	int a = 99;
	int b = 99;
	bool r;

	// Both edges zero-length at the same point: coincident.
	r = edges_intersect_sub(&a, &b, P(0, 0), P(0, 0), P(0, 0), P(0, 0));
	EXPECT(r && a == 0 && b == 0);

	// Proper crossing: e0 (0,0)-(2,0), e1 (1,-1)-(1,1).
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(0, 0), P(2, 0), P(1, -1), P(1, 1));
	EXPECT(!r && a == -1 && b == -1);

	// Separate, not crossing: e0 (0,0)-(1,0), e1 (2,1)-(3,1).
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(0, 0), P(1, 0), P(2, 1), P(3, 1));
	EXPECT(r && a == 1 && b == 1);

	// Touching: the endpoint e1v0 (1,0) lies on e0.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(0, 0), P(2, 0), P(1, 0), P(1, 1));
	EXPECT(!r && a == -1 && b == 0);

	// e0 zero-length at (1,1), off the line of e1: det10 = 0 gives b = 0, det00 > 0 gives r false.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(1, 1), P(1, 1), P(0, 0), P(2, 0));
	EXPECT(!r && a == 1 && b == 0);

	// e0 zero-length at (1,0), on e1: det00 == 0 sets a = 0 and returns true.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(1, 0), P(1, 0), P(0, 0), P(2, 0));
	EXPECT(r && a == 0 && b == 0);

	// Vertical e0 crossing horizontal e1: both -1, and det00 < 0 so r is true.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(0, 0), P(0, 2), P(-1, 1), P(1, 1));
	EXPECT(r && a == -1 && b == -1);

	// Both zero-length at different points: det00 == 0 and det10 == 0.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(0, 0), P(0, 0), P(1, 1), P(1, 1));
	EXPECT(r && a == 0 && b == 0);

	// Collinear, overlapping: every determinant is 0.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(0, 0), P(2, 0), P(1, 0), P(3, 0));
	EXPECT(r && a == 0 && b == 0);

	// The x values are promoted to double before the difference (asm 0x7b01a0-0x7b01b8). A float
	// subtraction would give dx0 = 0.5 - 2^24 = -2^24 (rounded), and det10 would be 0 and b would be 0.
	// In double, dx0 = -16777215.5, det10 = 0.5 > 0, det11 = -16777217 < 0, so b = -1.
	// det00 = -16777218 < 0 and det01 = -0.5 < 0, so a = +1 and r is true.
	a = 99; b = 99;
	r = edges_intersect_sub(&a, &b, P(16777216.0f, 0), P(0.5f, 1), P(0, 1), P(2, 2));
	EXPECT(r && a == 1 && b == -1);

	if (g_failures != 0) {
		std::printf("%d failure(s)\n", g_failures);
		return 1;
	}
	std::printf("edges_intersect_sub: all cases passed\n");
	return 0;
}
