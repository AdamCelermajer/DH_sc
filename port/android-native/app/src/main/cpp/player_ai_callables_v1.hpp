#pragma once
#include <cstdint>
namespace dh2::android_ui {
// Exact ARM source dispatch identities, not executable native vtables.
// Caller assigns only to its actual retained CharAI/AISPlayerIPhone selection.
const std::uintptr_t* player_char_ai_keys_v1()noexcept;
const std::uintptr_t* player_iphone_ai_keys_v1()noexcept;
}
