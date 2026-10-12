#include "net-session-online/game_center.hpp"

#include <cstdlib>
#include <sstream>

namespace {

// The original writes two longs into one std::stringstream, back to back, with no separator,
// and then parses the text with strtol(text, nullptr, 10). The result is therefore the decimal
// digits of the first value followed by those of the second, read as one number. A negative
// first value keeps its sign in front of the digits of the second.
[[maybe_unused]] std::int32_t ComposeGCPlayerGroup(long first, long second) {
    std::ostringstream text;
    text << first << second;
    const std::string digits = text.str();
    return static_cast<std::int32_t>(std::strtol(digits.c_str(), nullptr, 10));
}

}  // namespace

// 0x499694 _ZN10GameCenter16GetGCPlayerGroupEv
//
// Partial. The original body does this in order:
//   1. Reads the local player's user id. PlayerManager::GetLocalPlayer(0, false) gives the
//      player object; its word at +0x664 is the id, or -1 (then the id comes from the object at
//      dword_99F778, word +0x08).
//   2. Builds a PlayerSavegame(id, 1, false) on the stack.
//   3. Calls RenderFX::InvokeASCallback on the root movie with "GetCurrentDifficulty". This runs
//      the Flash side, which sets PlayerSavegame::m_difficultyLevel.
//   4. SG_Load(16), then SG_GetCurrentPrimaryQuest(m_difficultyLevel).
//   5. first = SG_GetQuestSG()[m_difficultyLevel] read as an int at +0x44 (the quest array).
//   6. second = the word at +0x0c of the object at dword_99F778.
//   7. Returns ComposeGCPlayerGroup(first, second), as above.
// Missing: the PlayerSavegame and PlayerManager layouts, the dword_99F778 object layout and the
// RenderFX/MenuManager calls are not declared for this category. Until they are, the body returns
// 0 instead of the composed value. Once the inputs are available, pass them through
// ComposeGCPlayerGroup.
std::int32_t GameCenter::GetGCPlayerGroup() {
    return 0;
}
