// Test for batch v1-vendor-gameswf-gameswf_sprite_instance-d5206d-1: the frame-walk arithmetic of
// gameswf::sprite_instance::goto_frame(int) (DH2). Returns non-zero on failure.
#include "vendor-gameswf/gameswf_sprite_goto.hpp"

#include <cstdio>

using gameswf::dh2::frame_run;
using gameswf::dh2::sprite_goto_forward_run;
using gameswf::dh2::sprite_goto_rejected;
using gameswf::dh2::sprite_goto_reverse_run;

static int failures = 0;

#define CHECK(cond)                                                    \
	do {                                                               \
		if (!(cond)) {                                                 \
			std::printf("FAIL line %d: %s\n", __LINE__, #cond);        \
			++failures;                                                \
		}                                                              \
	} while (0)

int main()
{
	// Rejection: same frame, past the last frame (frame_count <= target), negative target.
	CHECK(sprite_goto_rejected(5, 5, 10));
	CHECK(sprite_goto_rejected(10, 2, 10));
	CHECK(sprite_goto_rejected(-1, 2, 10));
	CHECK(sprite_goto_rejected(0, 0, 10));
	CHECK(!sprite_goto_rejected(9, 2, 10));  // last frame is valid
	CHECK(!sprite_goto_rejected(0, 2, 10));  // frame 0 is valid

	// Backward 7 -> 2: reverse tags on 7, 6, 5, 4, 3 (not 2, not 8).
	frame_run r = sprite_goto_reverse_run(7, 2);
	CHECK(r.first == 7);
	CHECK(r.count == 5);
	CHECK(r.first - (r.count - 1) == 3);  // last frame run is target + 1

	// Backward 1 -> 0: only frame 1.
	r = sprite_goto_reverse_run(1, 0);
	CHECK(r.first == 1);
	CHECK(r.count == 1);

	// Forward 3 -> 7: state-only tags on 4, 5, 6 (not 3, not 7).
	frame_run f = sprite_goto_forward_run(3, 7);
	CHECK(f.first == 4);
	CHECK(f.count == 3);
	CHECK(f.first + f.count - 1 == 6);

	// Forward 3 -> 4: nothing runs.
	f = sprite_goto_forward_run(3, 4);
	CHECK(f.count == 0);

	// Forward 3 -> 5: frame 4 only.
	f = sprite_goto_forward_run(3, 5);
	CHECK(f.first == 4);
	CHECK(f.count == 1);

	// Forward 0 -> 9: frames 1 .. 8.
	f = sprite_goto_forward_run(0, 9);
	CHECK(f.first == 1);
	CHECK(f.count == 8);
	CHECK(f.first + f.count - 1 == 8);

	if (failures != 0) {
		std::printf("%d failure(s)\n", failures);
		return 1;
	}
	std::printf("sprite goto frame-walk tests passed\n");
	return 0;
}
