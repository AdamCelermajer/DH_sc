#pragma once
#include "character_world_target_owner_v1.hpp"
#include "character_world_handle_v1.hpp"
#include "character_state.hpp"
#include "character_ai_update.hpp"
#include <list>
#include <memory>
#include <functional>
#include <utility>
#include "../game-data/combat_result.hpp"
namespace dh2::character::skills {
// Registration borrows the actual renderer/script actor. It owns no properties,
// life, FSM, controller, scene, skill, inventory or save storage. Refresh must
// return source fields; unavailable fields stay unavailable.
struct WorldActorRegistrationV1 {
 std::uintptr_t identity{};std::int32_t handle_key{};
 void* context{};
 int(*refresh)(void*,WorldTargetActorBorrowV1*){};
 State* machine{}; // Character+520 is this SAME State.flags.
 target_providers::Handle16* shared_handle{};
 std::uintptr_t* current_target{};
 int(*controller)(void*,ControllerCommandState32*){};
 AIUpdateState80* ai_update{};
 const AIUpdateServices24* ai_services{};
 // Source script/FSM Died delivery; it decides whether/how to clear a target.
 int(*target_died)(void*,std::uintptr_t){};
 int(*machine_state_v108)(void*,std::uint32_t*){};
};
class CharacterWorldRuntimeV1 {
 struct Registration {WorldActorRegistrationV1 actor;target_search::Entry16 entry;};
 const data::AiTables& ai_;
 std::list<Registration> actors_;
 target_search::Room16 room_sentinel_{},room_{};
 target_search::Entry16 entry_sentinel_{};
 target_search::Registry8 registry_{};
 std::vector<target_providers::Record16> records_;
 target_providers::Registry24 handles_{};
 std::vector<WorldAiFactionRowV1> factions_;
 const std::int32_t* assert_mode_{};
 WorldAiServicesV1 assertion_{};
 std::unique_ptr<CharacterWorldTargetOwnerV1> targets_;
 std::string error_;
 std::function<bool(std::uintptr_t,std::uintptr_t,const data::CombatResult&,std::string&)> combat_sound_v112_;
 Registration* find(std::uintptr_t);
 void relink();
 static int borrow(void*,std::uintptr_t,WorldTargetActorBorrowV1*);
 static int control(void*,std::uintptr_t,ControllerCommandState32*);
 static int flags(void*,std::uintptr_t,std::uint32_t*);
 static int machine_state_v108(void*,std::uintptr_t,std::uint32_t*);
 static int neutral_query_v108(void*,std::uintptr_t,std::uintptr_t,std::uintptr_t*);
 static int ai_service(void*,const WorldAiRequestV1*,WorldAiResponseV1*);
 static int enemy(void*,std::uintptr_t,std::uintptr_t,std::uintptr_t*);
 static int friendly(void*,std::uintptr_t,std::uintptr_t,std::uintptr_t*);
public:
 // Audio tail over this SAME registered world. Installed once by the
 // canonical owner; an absent tail preserves the existing service backend.
 using CombatSoundV112=std::function<bool(std::uintptr_t,std::uintptr_t,const data::CombatResult&,std::string&)>;
 void bind_combat_sound_v112(CombatSoundV112 sound){combat_sound_v112_=std::move(sound);}
 int deliver_combat_sound_v112(std::uintptr_t attacker,std::uintptr_t target,const data::CombatResult& result,std::string& error){
  if(!combat_sound_v112_)return 0;return combat_sound_v112_(attacker,target,result,error)?1:-1;
 }
 explicit CharacterWorldRuntimeV1(const data::AiTables&,std::uint32_t capacity=4096,
  const std::int32_t* assert_mode=nullptr,WorldAiServicesV1 assertion={});
 CharacterWorldRuntimeV1(const CharacterWorldRuntimeV1&)=delete;
 int add(const WorldActorRegistrationV1&);
 int remove(std::uintptr_t);void clear();
 void begin_frame(std::uint32_t frame)noexcept{handles_.frame=frame;}
 int refresh();
 int actor(std::uintptr_t id,WorldTargetActorBorrowV1* out){return borrow(this,id,out);}
 State* state_borrow(std::uintptr_t id){auto* r=find(id);return r?r->actor.machine:nullptr;}
 int relationship(std::uintptr_t owner,std::uintptr_t target,bool enemy,std::uintptr_t*);
 int neutral(std::uintptr_t owner,std::uintptr_t target,std::uintptr_t*);
 int get_handle(std::uintptr_t,target_providers::Handle16*);
 int handle_borrow(std::uintptr_t,target_providers::Handle16**,target_providers::Registry24**);
 int resolve(target_providers::Handle16*,std::uintptr_t*,bool asserted=false);
 int notify_death(std::uintptr_t);
 int update_ai(std::uintptr_t,AIUpdateResult16*);
 SkillNativeWorldV5 native_world(std::uintptr_t id,const dh2_script_object_services* objects){return targets_->native_world(id,objects);}
 CharacterWorldTargetOwnerV1& targets()noexcept{return *targets_;}
 const target_search::Registry8& registry()const noexcept{return registry_;}
 target_providers::Registry24& handles()noexcept{return handles_;}
 const std::string& error()const noexcept{return error_;}
};
}
