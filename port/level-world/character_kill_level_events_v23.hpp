#pragma once
#include "character_kill.hpp"
#include "event_manager_owner_v12.hpp"
#include "../level-loader/canonical_level_context_v1.hpp"
#include <functional>
namespace dh2::character {
// Pins real current-Level borrows only during a synchronous (possibly nested)
// Kill command. The callback reads the sole GSLevel::s_level, not another slot.
struct KillLevelProviderV23 {
 std::weak_ptr<void> lifetime;
 std::function<bool(loader::CanonicalCurrentLevelBorrowV1&,std::string&)> current;
};
class CharacterKillLevelEventsV23 {
 KillLevelProviderV23 provider_;std::vector<loader::CanonicalCurrentLevelBorrowV1> levels_;unsigned depth_{};
public:
 explicit CharacterKillLevelEventsV23(KillLevelProviderV23 s):provider_(std::move(s)){}
 void begin()noexcept{++depth_;}
 void end()noexcept{if(depth_&&!--depth_)levels_.clear();}
 bool route(const KillRequest56&,KillResponse16&,bool& handled,std::string&);
};
// Read-only source typed payload view. RaiseAsync is immediate; retaining this
// pointer/context beyond handler return is invalid. No payload copy/queue.
const KillQuest48* kill_quest_payload_v23(const events::EventBorrowV12&)noexcept;
}
