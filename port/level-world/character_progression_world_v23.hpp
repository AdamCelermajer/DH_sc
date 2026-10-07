#pragma once
#include "player_progression_v1.hpp"
#include "player_manager_owner_v1.hpp"
#include "character_kill.hpp"
namespace dh2::character {
struct CharacterProgressionServicesV23 {
 std::shared_ptr<void> owner;
 player::PlayerManagerOwnerV1* players{};
 const data::DesignSettingsProjection176* settings{};
 // Fresh same actor/property/Save/class/position/local/remote/currentLevel150
 // borrow. No copied progression counter or inferred all-player policy.
 std::function<bool(std::uintptr_t,ProgressionActorV1&,std::string&)> actor;
 ProgressionServicesV1 source;
};
struct CharacterProgressionReceiptV23 {
 std::uintptr_t killer{},victim{};std::int32_t source_count{};
 std::uint32_t qualifying{},given{},local_texts{};
 std::vector<ProgressionResultV1> recipients;
};
class CharacterProgressionWorldV23 {
 CharacterProgressionServicesV23 services_;bool running_{};
 CharacterProgressionReceiptV23 receipt_;
public:
 explicit CharacterProgressionWorldV23(CharacterProgressionServicesV23 s):services_(std::move(s)){}
 // Called only inside Kill at kill_distribute_xp. No duplicate-delivery ledger,
 // independent death hook, forced player count or direct fixture XP award.
 bool route(KillActor56&,const KillRequest56&,KillResponse16&,bool& handled,std::string&);
 bool distribute(std::uintptr_t nullable_killer,std::uintptr_t victim,std::string&);
 bool give_source_v108(std::uintptr_t,std::int32_t,bool,ProgressionResultV1&,std::string&);
 const CharacterProgressionReceiptV23& receipt()const noexcept{return receipt_;}
};
}
