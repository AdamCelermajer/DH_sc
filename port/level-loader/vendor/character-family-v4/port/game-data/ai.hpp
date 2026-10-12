#pragma once
#include "data.hpp"

namespace dh2::data {
struct AiProps {
 std::int32_t attack_delay,combat_beat,combat_music;
 std::uint32_t delayed_load,flags;
 float interact_radius,leash_distance,melee_radius;
 std::int32_t on_aggro_sfx;
 std::string script;
 std::int32_t self_fx,trophy,type;
 float view_radius,view_radius_no_aggro;
};
struct AiFactionEntry {std::int32_t id,value;};
struct AiTables {
 std::vector<std::string> names,faction_names;
 std::vector<AiProps> rows;
 std::vector<std::vector<AiFactionEntry>> factions;
};
// Packed cache formats, including the one-byte DelayedLoad and inline Script.
bool load_ai(Bytes data,Bytes names,Bytes schema,Bytes factions,Bytes faction_names,Bytes faction_schema,AiTables&,std::string&);
const AiProps* ai_props(const AiTables&,std::int32_t id);
bool ai_enemy(const AiTables&,std::int32_t owner,std::int32_t target,bool owner_player,bool target_player);

struct AiRangeRequest {float owner[3],target[3],owner_melee,target_melee,view;};
struct AiRangeResult {std::uint32_t distance_bits,melee,sight;};
enum AiTargetFacts : std::uint32_t {
 ai_target_present=1,ai_targetable=2,ai_target_alive=4,ai_target_sight=8,
 ai_owner_can_range=16,ai_target_close_range=32,ai_target_ranged_range=64,
 ai_target_melee_range=128,ai_callback_clears_dead=256,ai_callback_clears_sight=512
};
// Callback snapshots explicitly describe the supplied event backend. The
// monster script clears on Died/OutOfSight; other script backends can differ.
struct AiTargetRequest {std::int32_t owner_state;std::uint32_t facts,previous_alive,previous_sight;};
struct AiTargetResult {
 std::uint32_t target_present,last_target_present,alive,sight,event_count;
 std::uint32_t events[3],null_arguments;
};
}
extern "C" unsigned dh2_ai_range(dh2::data::AiRangeResult*,const dh2::data::AiRangeRequest*);
// Character branch only: source faction list, signed relationship, player pair.
extern "C" unsigned dh2_ai_enemy(const dh2::data::AiFactionEntry*,unsigned count,int target_id,unsigned owner_player,unsigned target_player);
extern "C" unsigned dh2_ai_target_update(dh2::data::AiTargetResult*,const dh2::data::AiTargetRequest*);
