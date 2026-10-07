#pragma once
#include "character_world_aggro_event_v1.hpp"
#include "character_player_aggro_owner_v1.hpp"
#include "vox_music_state_owner_v1.hpp"
#include "../game-data/aggro.hpp"
namespace dh2::character {
// All borrows are the published World/selected AIS owners. No alternate threat
// tables, Character/FSM, counters, Level or sound-manager fields are created.
struct WorldPlayerAggroBorrowV2 {
 std::uintptr_t player{};AIEventState64* events{};
 PlayerAggroFieldsV1* selected_fields{};
 sound::VoxMusicStateOwnerV1* music{};
};
struct WorldPlayerAggroServicesV2 {
 void* context{};
 int(*events)(void*,std::uintptr_t,AIEventState64**){};
 int(*online)(void*,bool*){};
 int(*local_player)(void*,std::uintptr_t,bool*){};
 int(*level)(void*,PlayerAggroLevelBorrowV1*){};
 int(*weight)(void*,std::uintptr_t,std::int32_t*){};
 int(*threshold)(void*,std::int32_t*){};
 int(*play_music)(void*,const PlayerAggroRequestV1*){};
 int(*trace)(void*,const PlayerAggroRequestV1*){};
 // Other original AIS overrides and ClearTarget/Stop remain required.
 int(*remaining)(void*,std::uint32_t,std::uintptr_t,std::uintptr_t){};
};
class CharacterWorldPlayerAggroV2 {
 WorldPlayerAggroBorrowV2 fields_;WorldPlayerAggroServicesV2 services_;
 DebugSwitches& debug_;const DebugFileServices24& files_;std::string error_;
 static int ais(void*,AIEventState64*,std::uintptr_t,std::uintptr_t);
 static int service(void*,const PlayerAggroRequestV1*,PlayerAggroResponseV1*);
public:
 CharacterWorldPlayerAggroV2(WorldPlayerAggroBorrowV2,WorldPlayerAggroServicesV2,
  DebugSwitches&,const DebugFileServices24&);
 // Original Set/Add owner→target OnAggro receiver is TARGET, argument OWNER.
 int notify(std::uint32_t source_request,std::uintptr_t owner,std::uintptr_t target);
 static int callback(void* p,std::uint32_t bit,std::uintptr_t owner,std::uintptr_t target){return static_cast<CharacterWorldPlayerAggroV2*>(p)->notify(bit,owner,target);}
 const std::string& error()const noexcept{return error_;}
};
}
