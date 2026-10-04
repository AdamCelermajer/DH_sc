#pragma once
#include <cstdint>
namespace dh2::ui {
struct SwfDragValues108 {std::int32_t mouse[2];float world[6],parent_world[6],local[6],bounds[4],offset[2];std::uint8_t initialized,lock_center,bounded,reserved;};
struct SwfDragResult44 {float matrix[6],offset[2],local_mouse[2];std::uint32_t initialized;};
static_assert(sizeof(SwfDragValues108)==108&&sizeof(SwfDragResult44)==44);
}
extern "C" {
// Original Character::do_mouse_drag 2D arithmetic, including source bounds
// comparison selection and finite stores. Distinct spans; -1 atomic malformed.
int dh2_ui_swf_drag_values(dh2::ui::SwfDragResult44*,const dh2::ui::SwfDragValues108*) noexcept;
}
