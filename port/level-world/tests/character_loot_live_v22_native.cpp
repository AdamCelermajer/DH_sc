#include "character_loot_live_v22.hpp"
#include "character_loot_creation_bindings_v22.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
using namespace dh2;using namespace dh2::character;
struct ActorV22 {
 std::uintptr_t id{},node{},target{};std::uint32_t type{};
 std::shared_ptr<int> lease=std::make_shared<int>(1);data::PropertyState sheets;
 data::PropertyView view;data::CombatActorState life{};data::AiProps ai{};State state{};
 target_search::Object48 search{};skills::SkillTargetCharacterV6 character{};scene::Scene scene;
 target_providers::Handle16 handle{};
 ActorV22(std::uintptr_t identity,const data::PropertyRules& rules):id(identity),view(data::property_view(rules,sheets)){
  sheets.resolved[9]=-1;search.identity=id;search.visible=1;character={id,view.resolved,"fixture",0,0,0,1,1};
 }
 static int refresh(void* p,skills::WorldTargetActorBorrowV1* out){auto& a=*static_cast<ActorV22*>(p);*out={a.id,&a.search,&a.character,&a.scene,&a.life,&a.node,nullptr,nullptr,a.search.position,nullptr,nullptr,nullptr,nullptr};return 0;}
 CharacterLootLiveBorrowV22 borrow(){return {{id,0,&ai,&view},view.resolved+9,search.position,&type,lease};}
};
struct FixtureV22 {
 data::PropertyRules rules;data::AiTables ai;skills::CharacterWorldRuntimeV1 world{ai};
 player::PlayerManagerOwnerV1 players{{this,manager}};data::LootRandom8V2 rng{0x13579bdf,0};
 ActorV22 victim{0x100000001ull,rules},player{0x100000002ull,rules};
 std::shared_ptr<int> lease=std::make_shared<int>(1);unsigned reads{};bool wrong_position{};
 static bool manager(void*,const player::PlayerManagerRequestV1& q,player::PlayerManagerResponseV1& out,std::string&){if(q.operation==player::PlayerManagerOperationV1::construct_player_info)*q.player=player::PlayerInfoFieldsV1{};out.value=0;return true;}
 static bool borrow(void* p,std::uintptr_t id,CharacterLootLiveBorrowV22& out,std::string& e){auto& f=*static_cast<FixtureV22*>(p);++f.reads;if(id==f.victim.id)out=f.victim.borrow();else if(id==f.player.id)out=f.player.borrow();else {e="Required registered fixture actor";return false;}if(f.wrong_position)out.position160=f.player.search.target_position;return true;}
 static bool is_player(void* p,std::uintptr_t id,bool& out,std::string&){out=id==static_cast<FixtureV22*>(p)->player.id;return true;}
 FixtureV22(){std::string e;assert(!world.add({victim.id,1,&victim,ActorV22::refresh,&victim.state,&victim.handle,&victim.target}));assert(!world.add({player.id,2,&player,ActorV22::refresh,&player.state,&player.handle,&player.target}));assert(players.initialize(e)&&players.add_player(0,0,0,true,e));player::PlayerInfoFieldsV1* record{};assert(players.get_by_internal(0,false,record,e));record->character660=player.id;}
 CharacterLootLiveServicesV22 services(){CharacterLootLiveServicesV22 s;s.provider_lease=lease;s.context=this;s.actor=borrow;s.is_player=is_player;return s;}
};
std::vector<std::uint8_t> read(const std::string& p){std::ifstream f(p,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),std::istreambuf_iterator<char>()};}
int absent(void* p,const char*,std::uintptr_t* out){++*static_cast<unsigned*>(p);*out=0;return 0;}
int closed(void*,std::uintptr_t){return -1;}
int main(int argc,char** argv){assert(argc==2);std::string e;const std::string dir=argv[1];auto b=read(dir+"/loot_table_pyarray.bin"),n=read(dir+"/loot_table_pyarraynames.bin"),s=read(dir+"/loot_table_pystructnames.bin");data::LootTablesV2 tables;assert(tables.load({b.data(),b.size()},{n.data(),n.size()},{s.data(),s.size()},e));unsigned checks=0;
 auto request=[](ActorV22& a,std::uintptr_t killer){KillRequest56 q{};q.service=kill_drop_loot;q.subject=a.id;q.target=killer;return q;};
 {FixtureV22 f;CharacterLootLiveV22 loot(f.world,f.players,tables.borrow(),{}, {},f.rng,f.services());WorldLootItemRuntimeV1 pool({},{});assert(loot.bind_pool(pool,e)&&!loot.bind_pool(pool,e));checks+=2;KillActor56 actor{};actor.identity=f.victim.id;KillResponse16 out{};bool handled{};auto q=request(f.victim,f.player.id);assert(loot.route(actor,q,out,handled,e)&&handled&&!loot.pending_inventory()&&!f.rng.calls);checks++;assert(loot.items()==&pool);checks++;auto services=loot.drop_services();std::uintptr_t local{};assert(services.local_player_character(services.context,0,true,local,e)&&local==f.player.id);assert(*f.players.character_count_field()==0);checks+=2;float xyz[3];assert(services.random_drop_position(services.context,f.victim.id,0,xyz,e)&&f.rng.calls==2);checks++;}
 {FixtureV22 f;CharacterLootLiveV22 loot(f.world,f.players,tables.borrow(),{}, {},f.rng,f.services());KillActor56 a{};a.identity=f.victim.id;KillResponse16 out{};bool handled{};auto q=request(f.victim,f.player.id);assert(!loot.route(a,q,out,handled,e)&&e=="Required SAME canonical Item145 pool binding"&&loot.pending_inventory());const auto reads=f.reads;assert(!loot.route(a,q,out,handled,e)&&f.reads==reads&&!f.rng.calls);checks+=2;}
 {FixtureV22 f;f.wrong_position=true;CharacterLootLiveV22 loot(f.world,f.players,tables.borrow(),{}, {},f.rng,f.services());KillActor56 a{};a.identity=f.victim.id;KillResponse16 out{};bool handled{};auto q=request(f.victim,f.player.id);assert(!loot.route(a,q,out,handled,e)&&e=="Loot position must borrow SAME source position160"&&!loot.pending_inventory());const auto reads=f.reads;assert(!loot.route(a,q,out,handled,e)&&f.reads==reads);checks+=2;}
 {FixtureV22 f;f.victim.sheets.resolved[9]=0;CharacterLootLiveV22 loot(f.world,f.players,tables.borrow(),{}, {},f.rng,f.services());WorldLootItemRuntimeV1 pool({},{});assert(loot.bind_pool(pool,e));KillActor56 a{};a.identity=f.victim.id;KillResponse16 out{};bool handled{};auto q=request(f.victim,f.player.id);assert(!loot.route(a,q,out,handled,e)&&e=="Required original AddLoot Debug provider"&&loot.pending_inventory()&&!f.rng.calls);const auto reads=f.reads;assert(!loot.route(a,q,out,handled,e)&&f.reads==reads);checks+=3;}
 {FixtureV22 f;CharacterLootLiveV22 loot(f.world,f.players,tables.borrow(),{}, {},f.rng,f.services());KillActor56 a{};a.identity=f.victim.id;KillResponse16 out{};bool handled{};auto q=request(f.victim,0);assert(loot.route(a,q,out,handled,e)&&handled&&!loot.pending_inventory());checks++;q.service=kill_is_dead;assert(loot.route(a,q,out,handled,e)&&!handled);checks++;}
 {FixtureV22 f;auto* debug=dh2_character_debug_create();assert(debug);unsigned opens{};data::ItemPresentationOwnerV5 presentation({});CharacterLootCreationBackendV22 backend;backend.provider_lease=f.lease;CharacterLootCreationBindingsV22 bindings(f.players,*debug,{&opens,absent,closed},presentation,{},backend);auto services=bindings.services();std::int32_t value=-1;
  assert(services.source.entry.invoke(services.source.entry.context,{data::LootEntryOperationV8::debug_load,0,nullptr},value,e));assert(services.source.entry.invoke(services.source.entry.context,{data::LootEntryOperationV8::debug_query,0,"MP_MinimalRandoms"},value,e)&&value==0&&opens==1);checks+=2;
  for(auto op:{data::LootEntryOperationV8::warrior_count,data::LootEntryOperationV8::mage_count,data::LootEntryOperationV8::rogue_count}){value=-1;assert(services.source.entry.invoke(services.source.entry.context,{op,0,nullptr},value,e)&&value==0);checks++;}
  data::LootCreationResponseV8 result{};assert(!services.source.query(services.source.context,{data::LootCreationOperationV8::current_level,0},result,e)&&e=="Required actual loot GetPlayersCount/current-Level producer");checks++;
  assert(!services.source.entry.invoke(services.source.entry.context,{data::LootEntryOperationV8::assertion,0,nullptr},value,e)&&e=="Required source loot assertion continuation");checks++;dh2_character_debug_destroy(debug);
 }
 std::cout<<"LootV22 native PASS "<<checks<<" checks; actual immutable loot cache "<<tables.borrow().loots().size()<<" tables; positive nonempty factory/audio remains required, actor/PM inputs are explicit fixtures\n";
}
