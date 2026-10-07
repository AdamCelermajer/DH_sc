#pragma once
#include "character_ai_events.hpp"
#include "character_script_owner_v2.hpp"
#include "character_world_runtime_v1.hpp"
#include "character_mesh_fx_owner_v1.hpp"
#include "floors.hpp"
namespace dh2::character {
struct AnimationEventBorrowV1 {
 std::uintptr_t character{};
 ScriptOwnerV2* script_owner{};
 const std::uintptr_t* script_identity{};
 const std::int32_t* animator_lag{}; // actual Character+4f4
 data::PropertyView* properties{}; // cached +1014 is SAME resolved[7]
 skills::CharacterWorldRuntimeV1* world{};
 const std::uintptr_t* visual{}; // actual Character+2d8, zero is legitimate
 const scene::Scene* scene{};
 const std::uint32_t* visual_root{}; // actual VisualObject+8 node; UINT_MAX
 // represents its source CRootSceneNode container over serialized root forest.
 // Exact Character+1d8 cached PFFloor and its string +3c. Null output is
 // a delivered source null; absent callback is unavailable, never null.
 void* floor_context{};
 int(*floor_type)(void*,const char**){};
};
struct AnimationFloorBorrowV1 {
 const floors::World* world{};
 const std::uint32_t* cached_floor{}; // UINT_MAX is source null PFFloor
};
// Accepted floors::append rejects floortypes overrides. Thus these actual
// retained PFFloors keep their constructor-owned empty type string.
int animation_floor_type_v1(void*,const char**);
// Source SceneManager root-first, child-order DFS name lookup. UINT_MAX is the
// native flattened representation of the actual collada CRootSceneNode; its
// ordered children are exactly graph entries with parent=-1.
int animation_specific_node_v1(const scene::Scene&,std::uint32_t root,const char*,std::int32_t&);
class CharacterAnimationEventOwnerV1 {
 AIEventState64& ai_;
 AnimationEventBorrowV1 actor_;
 data::EffectsTables::Borrow tables_;
 fx::CharacterMeshFxOwnerV1* mesh_;
 std::string error_;
 std::uint32_t lua_error_{};
 bool foot(bool left,float output[3]);
 bool play(std::int32_t,const float[3]);
 bool default_event(const char*);
public:
 CharacterAnimationEventOwnerV1(AIEventState64&,AnimationEventBorrowV1,
  data::EffectsTables::Borrow,fx::CharacterMeshFxOwnerV1*);
 // Input is source CharAI relay text AFTER the ev_ prefix has been removed.
 // Null active AIS completes the actual relay. Unknown selected virtual fails.
 bool relay(const char* stripped_event);
 const std::string& error()const noexcept{return error_;}
 std::uint32_t source_lua_error()const noexcept{return lua_error_;}
};
}
