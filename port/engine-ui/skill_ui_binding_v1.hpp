#pragma once
#include <cstdint>

namespace dh2::ui {
// SWF callbacks name a local Player index, while details, slots, training and
// assignment must resolve through that Player's one Save and skill runtime.
inline bool skill_ui_binding_v1(std::uintptr_t callback_character,
                                std::uintptr_t save_character,
                                std::uintptr_t skill_runtime_character) noexcept {
 return callback_character&&callback_character==save_character&&
        callback_character==skill_runtime_character;
}
}
