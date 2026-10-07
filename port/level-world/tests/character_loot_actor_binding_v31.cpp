#define main prior_kill_fixture_main_v31
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_kill_live_v21_native.cpp"
#pragma GCC diagnostic pop
#undef main
#include "character_loot_actor_binding_v31.hpp"
struct LootActorsV31 {
 Fixture& f;std::uint32_t type{};std::uintptr_t ooi{};bool wrong_handle{},wrong_sheet{};
 explicit LootActorsV31(Fixture& fixture):f(fixture){}
 static bool cast(void* p,std::uintptr_t& out,std::string&){out=static_cast<Actor*>(p)->id;return true;}
 static bool borrow(void* p,std::uintptr_t id,CharacterLootActorFieldsV31& out,std::string& e){
  auto& s=*static_cast<LootActorsV31*>(p);Actor* a=id==s.f.victim->id?s.f.victim.get():id==s.f.player->id?s.f.player.get():nullptr;if(!a){e="Missing registered fixture";return false;}
  out={};out.canonical.identity=id;out.canonical.lease=a->lease;out.canonical.shared_handle=s.wrong_handle?&s.f.player->handle:&a->handle;out.canonical.type_f4=&s.type;out.canonical.context=a;out.canonical.as_character=cast;
  out.receiver_lease=a->lease;out.properties=s.wrong_sheet?&s.f.player->view:&a->view;out.object_of_interest14a4=&s.ooi;return true;
 }
};
int main(){Fixture f;LootActorsV31 fields(f);std::string e;unsigned checks{};
 CharacterLootActorBindingV31 owner(*f.world,f.ai,{f.lease,&fields,LootActorsV31::borrow});
 CharacterLootLiveBorrowV22 victim{};assert(owner.drop_actor(f.victim->id,victim,e));assert(victim.actor.ai==&f.ai.rows[1]&&victim.table101c==f.victim->view.resolved+9&&victim.position160==f.victim->search.position);checks+=2;
 // Fresh source reads, rather than a captured table ID or property copy.
 f.victim->view.resolved[9]=37;assert(*victim.table101c==37);f.victim->view.resolved[1]=0;assert(owner.drop_actor(f.victim->id,victim,e)&&victim.actor.ai==&f.ai.rows[0]);checks+=2;f.victim->view.resolved[1]=1;
 bool player{};assert(owner.is_player(f.player->id,player,e)&&player);assert(owner.is_player(f.victim->id,player,e)&&!player);checks+=2;
 std::uintptr_t cast{};assert(owner.character_cast(f.victim->id,cast,e)&&cast==f.victim->id);assert(owner.character_cast(0,cast,e)&&!cast);checks+=2;
 LootPickupActorV23 picked{};assert(owner.pickup_actor(f.victim->id,picked,e)&&!picked.source_is_player&&picked.object_of_interest14a4==&fields.ooi);checks++;
 // A Player has no accepted pickup without its actual ready Gear.
 assert(!owner.pickup_actor(f.player->id,picked,e)&&e=="Required SAME actual pickup Gear authority");checks++;
 fields.wrong_handle=true;assert(!owner.drop_actor(f.victim->id,victim,e));checks++;fields.wrong_handle=false;
 fields.wrong_sheet=true;assert(!owner.drop_actor(f.victim->id,victim,e));checks++;fields.wrong_sheet=false;
 f.victim->view.resolved[1]=99;assert(owner.drop_actor(f.victim->id,victim,e)&&victim.actor.ai==&f.ai.rows[8]);checks++; // Original cached-row fallback8.
 assert(!owner.drop_actor(0xdeadbeef,victim,e));checks++;
 std::cout<<"PASS loot actor V31 "<<checks<<" same registeredWorld/Handle/property/AI/OOI checks; actor rows fixtures, positive Gear/award not claimed\n";
}
