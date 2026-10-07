#pragma once
#include "character_spawn_select.hpp"
#include <memory>
#include <string>
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::character {
//SM_SetSpawnState3c2734 over SAME Character1434/1438, App RNG channel0,
//TimerStore and existing FSM. Only projection storage; no constructor replay.
class CanonicalCharacterSpawnSelectV87 final {
 std::weak_ptr<world::CanonicalCharacterCandidateRecordV60> record_;
public:
 explicit CanonicalCharacterSpawnSelectV87(std::weak_ptr<world::CanonicalCharacterCandidateRecordV60> record):record_(std::move(record)){}
 bool select(std::uint32_t delay_enabled,std::uint32_t ignored_mode,std::string&);
};
bool source_character_select_spawn_v87(world::CanonicalCharacterCandidateRecordV60&,std::uint32_t,std::uint32_t,std::string&);
}
