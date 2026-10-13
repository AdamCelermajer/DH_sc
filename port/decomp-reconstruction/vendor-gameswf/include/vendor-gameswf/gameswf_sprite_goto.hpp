// DH2: frame-walk arithmetic of gameswf::sprite_instance::goto_frame(int) (binary 0x781a3c, Gameloft).
//
// Pure code with no gameswf types, so the batch test (tests/v1-vendor-gameswf-gameswf_sprite_instance-d5206d-1_test.cpp)
// compiles it on its own. The batch body (src/v1-vendor-gameswf-gameswf_sprite_instance-d5206d-1.cpp) uses these
// helpers for its two frame loops.
#ifndef VENDOR_GAMESWF_SPRITE_GOTO_HPP
#define VENDOR_GAMESWF_SPRITE_GOTO_HPP

namespace gameswf {
namespace dh2 {

// A run of consecutive frames. A backward run visits first, first - 1, ..., first - count + 1.
// A forward run visits first, first + 1, ..., first + count - 1. A count of zero means no frame runs.
struct frame_run
{
	int first;
	int count;
};

// goto_frame rejects the request (sets the update flag and returns) when the target is past the last
// frame, negative, or already the current frame. frame_count is m_def->get_frame_count().
inline bool sprite_goto_rejected(int target, int current, int frame_count)
{
	return frame_count <= target || target < 0 || current == target;
}

// Backward jump (target < current): execute_frame_tags_reverse on current, current - 1, ..., target + 1.
inline frame_run sprite_goto_reverse_run(int current, int target)
{
	return frame_run{current, current - target};
}

// Forward jump: execute_frame_tags(f, true) on current + 1, ..., target - 1. Empty when target <= current + 1.
inline frame_run sprite_goto_forward_run(int current, int target)
{
	return frame_run{current + 1, target > current + 1 ? target - current - 1 : 0};
}

}  // namespace dh2
}  // namespace gameswf

#endif  // VENDOR_GAMESWF_SPRITE_GOTO_HPP
