#pragma once
#include "../game-data/skill_tables.hpp"
#include "../game-data/faery_tables.hpp"
#include "../game-data/properties.hpp"
#include <memory>
namespace dh2::character::skills {
struct Instance32 {std::uintptr_t owner;const char* script;std::uint32_t index;std::int32_t word18;float argument_index;std::uint32_t reserved;};
struct Slots16 {const Instance32* const* items;std::uint32_t count,reserved;};
struct List16 {const std::int32_t* ids;std::uint32_t count,reserved;};
struct Row16 {const char* script;std::uint32_t bytes,reserved;};
struct State40 {std::uintptr_t owner;Slots16 skills,spells;};
enum Operation:std::uint32_t {debug_load=1,debug_switch=2,active_script=3,capture_path=4,assign_path=5,release_path=6,get_list=7,reserve_slots=8,arguments_begin=9,get_row=10,load_file=11,declare_skill=12,reset_declaration=13,construct_instance=14,append_slot=15,arguments_end=16,init_vcb=17,state_predicate=18,set_skill=19,erase_results=20,call_update=21,call_cleanup=22,release_results=23};
struct Request48 {std::uint32_t operation,kind,index,count;std::uintptr_t script;const char* name;const Instance32* instance;std::uintptr_t payload;};
struct Response32 {std::uintptr_t object;const List16* list;const Row16* row;std::uint32_t source_error,count;};
struct Services16 {void* context;int(*invoke)(void*,State40*,const Request48*,Response32*);};
static_assert(sizeof(Instance32)==32&&sizeof(Slots16)==16&&sizeof(State40)==40);
static_assert(sizeof(Request48)==48&&sizeof(Response32)==32&&sizeof(Services16)==16);

// Pins actual tables, owns nullable slot vectors and each constructed instance's
// stable script bytes/arguments. No source Script/VM/Character ownership is
// manufactured. Nonmoving facade and providers must survive synchronous calls.
class CharacterSkillOwner {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 CharacterSkillOwner(std::uintptr_t,data::PropertyView*,data::SkillTables::Borrow,
  data::FaeryTables::Borrow,const Services16& script_services);
 ~CharacterSkillOwner();
 CharacterSkillOwner(const CharacterSkillOwner&)=delete;
 CharacterSkillOwner& operator=(const CharacterSkillOwner&)=delete;
 CharacterSkillOwner(CharacterSkillOwner&&)=delete;
 CharacterSkillOwner& operator=(CharacterSkillOwner&&)=delete;
 int configure();int update();int cleanup_skills();int cleanup_spells();
 int source_destroy_instances_v105(std::uint32_t kind);
 const State40& state()const noexcept;const std::string& error()const noexcept;
};
// The owner implements list/row/reserve/Arguments/construct/append/path snapshot
// storage internally. Required external operations: debug_load/debug_switch,
// active_script, capture_path (Response.row borrows exact path bytes),
// assign_path (Request.name/count are exact bytes, may contain NUL), load_file,
// declare_skill/reset_declaration, init_vcb, state_predicate, set_skill,
// erase_results/release_results, call_update/call_cleanup.
// active_script kind0 queries AI+1c for setup; kind1 queries instance owner's
// Character+3e4 for update/cleanup. Request.payload carries genuine owner ID.
// All script receivers are freshly queried at original reload points. Lua
// source errors are Response.source_error, separate from nonzero delivery
// failure. SetSkill returns a result token and projected count; erase/release
// must perform actual returned Value cleanup. Calls use Instance32's retained
// string and float argument_index, reset/update/cleanup have no arguments.
// Providers may synchronously reenter; they must not destroy owner/table/view.
}
extern "C" int dh2_character_skills_configure(dh2::character::skills::State40*,const dh2::character::skills::Services16*);
extern "C" int dh2_character_skills_update(dh2::character::skills::State40*,const dh2::character::skills::Services16*);
extern "C" int dh2_character_skills_cleanup(dh2::character::skills::State40*,std::uint32_t kind,const dh2::character::skills::Services16*);
extern "C" int dh2_character_skill_mapped_update_v70(dh2::character::skills::State40*,std::uint32_t kind,std::uint32_t index,const dh2::character::skills::Services16*);
