#pragma once
#include "character_template_factory.hpp"
#include "character_template_random.hpp"
#include "../game-data/player_save_load_owner_v1.hpp"

namespace dh2::character_saved_class_v1 {
enum class Status {complete,invalid_argument,busy,failed};
enum class Stage {not_started,cached,is_player,template_lookup,random,
                  save_load,save_class,publication,class_writeback,complete};
struct Services {
 void* context=nullptr;
 int (*is_player)(void*,std::uintptr_t,std::uint32_t*,std::string&)=nullptr;
};
struct Bindings {
 std::uintptr_t character=0;
 std::int16_t* property_cache=nullptr; // Character +13c8
 std::int16_t* template_cache=nullptr; // Character +13ca
 const character::template_factory::Source* authored=nullptr;
 // Reuses the existing ordered template records. Character names are read
 // from the actual CharacterTable below; Catalog's class mapping is unused.
 const character::template_factory::Catalog* templates=nullptr;
 const data::CharacterTable* characters=nullptr;
 dh2_random_state* random=nullptr;
 data::PlayerSavegameV1* const* current_savegame=nullptr; // live +14e8
 data::PlayerSaveLoadOwnerV1* const* current_loader=nullptr;
 Services services{};
};
struct Result {
 Stage stage=Stage::not_started;
 std::int32_t value=-1,template_id=-1,selected_slot=-1;
 std::uint32_t player_queries=0,load_calls=0,name_comparisons=0,
               random_draws=0,property_publications=0,class_writebacks=0;
};
class Runtime {
 Bindings bindings_;bool busy_=false;
public:
 explicit Runtime(Bindings);
 Status resolve(Result*,std::string& error);
};
// Whole SafeGetCharPropsId728B@3b3d38 plus its template-name helper176B.
// Borrows every cache, Save/profile, RNG and table; creates none. Cache bypass
// precedes IsPlayer/load. Player SG_Load(1) must use the live gameplay Save's
// own LoadOwner; metadata preview is not copied. Subsequent wrappers reread
// +14e8, then publish signed16 class and write it back to the current Save.
// Nonplayer template strings take priority over explicit names; first strcmp
// match, duplicate slots and halfword IDs are retained. Existing random uses
// only ordinary stream0. No factory conflict/ambiguity/class validation is
// inserted into this caller. Malformed/unbounded table and assertion domains
// fail at their reached native boundary. No failed latch or automatic retry.
// One thread; all borrowed backing and old/new Save/loader lifetimes survive
// callbacks. Providers cannot rebind/destroy this Runtime or its caches/tables.
}
