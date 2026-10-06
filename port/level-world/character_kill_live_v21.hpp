#pragma once
#include "character_kill_fields_v21.hpp"
#include "combat_ctrl_kill_owner_v1.hpp"
#include "character_world_runtime_v1.hpp"
#include "player_manager_owner_v1.hpp"
#include "../game-data/aggro.hpp"
#include <functional>
#include <map>
namespace dh2::character {
struct CharacterKillLiveBorrowV21 {
 std::uintptr_t identity{},controller{},controllable_character{};
 std::shared_ptr<void> receiver;
 data::PropertyView* properties{};data::CombatActorState* life{};
 CharacterKillFieldsV21* fields{};
 // Exact source ObjectBase+64, not sharedHandle.key/DACT ordinal.
 const std::int32_t* oid64{};
 const std::int16_t* property13c8{};
 std::uintptr_t* tracked14a4{};
 data::AggroTable* outgoing{};
 target_providers::Handle16* shared_handle{};
};
struct CharacterKillLiveBackendsV21 {
 std::shared_ptr<void> provider_lease;
 // Remaining reached Kill services are synchronous actual source delivery.
 // In particular RaiseAsync is the source immediate Raise tail, not a queue.
 std::function<bool(KillActor56&,const KillRequest56&,KillResponse16&,std::string&)> invoke;
};
// GetAggroEntry builds a fresh converse map sorted by less<float> each call.
// Duplicate threat keys keep the first source key-order entry. Count remains
// the outgoing source map count (and may exceed converse unique-key size).
bool source_kill_aggro_entry_v21(const data::AggroTable&,std::int32_t,
 std::uintptr_t&,std::uint32_t&,std::string&);
class CharacterKillLiveWorldV21 {
 struct Entry {
  CharacterKillLiveBorrowV21 borrow;
  KillActor56 projection{};
  std::uintptr_t published_killer{},published_tracked{};
  std::unique_ptr<CombatCtrlKillOwnerV1> owner;
  bool attempted{},complete{},failed{};std::uint32_t depth{};
  KillResult24 result{};
 };
 skills::CharacterWorldRuntimeV1& world_;player::PlayerManagerOwnerV1& players_;
 KillWorld16& globals_;CharacterKillLiveBackendsV21 backends_;
 std::map<std::uintptr_t,std::unique_ptr<Entry>> actors_;
 std::string error_;
 bool synchronize();
 int query(std::uint32_t,std::uintptr_t,std::int32_t&);
 static int service(void*,KillActor56*,const KillRequest56*,KillResponse16*);
 int invoke(KillActor56&,const KillRequest56&,KillResponse16&);
public:
 CharacterKillLiveWorldV21(skills::CharacterWorldRuntimeV1& w,player::PlayerManagerOwnerV1& p,
  KillWorld16& g,CharacterKillLiveBackendsV21 s):world_(w),players_(p),globals_(g),backends_(std::move(s)){}
 bool add(CharacterKillLiveBorrowV21,std::string&);
 // Whole Cmd_Kill40570c literal controllable dispatch -> Ctrl_Kill3ad528.
 // No command lock/force/global gate exists in this original wrapper.
 int command(std::uintptr_t actual_controller,std::uintptr_t actual_character,
  std::uintptr_t actual_attacker,std::uint32_t force);
 const std::string& error()const noexcept{return error_;}
 const KillResult24* result(std::uintptr_t)const noexcept;
 bool complete(std::uintptr_t)const noexcept;
};
}
