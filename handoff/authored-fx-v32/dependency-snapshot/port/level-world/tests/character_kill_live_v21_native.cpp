#include "character_kill_live_v21.hpp"
#include "../reference/character-kill-live-v21/aggro-original.hpp"
#include <cassert>
#include <iostream>
#include <cstring>
using namespace dh2;using namespace dh2::character;
struct Actor {
 std::uintptr_t id{},controller{},tracked{},node{};std::int32_t oid{7};std::int16_t prop{18};
 std::shared_ptr<int> lease=std::make_shared<int>(1);data::PropertyState sheets;data::PropertyView view;
 data::CombatActorState life{};CharacterKillFieldsV21 fields;State state{};
 target_search::Object48 search{};skills::SkillTargetCharacterV6 character{};scene::Scene scene;
 target_providers::Handle16 handle{};data::AggroEntry storage[8]{};data::AggroTable outgoing{storage,0,8};
 explicit Actor(std::uintptr_t identity,const data::PropertyRules& rules,std::int32_t ai):id(identity),controller(identity+0x100),view(data::property_view(rules,sheets)){
  std::string e;assert(fields.construct_fresh(e));sheets.resolved[1]=ai;sheets.resolved[36]=1000;state.flags=0x2380;
  search.identity=id;search.visible=1;character={id,view.resolved,"fixture",0,0,0,1,1};
 }
 static int refresh(void* p,skills::WorldTargetActorBorrowV1* out){auto& a=*static_cast<Actor*>(p);a.character.dead1449=std::uint8_t(a.life.dead);*out={a.id,&a.search,&a.character,&a.scene,&a.life,&a.node,nullptr,nullptr,a.search.position,nullptr,nullptr,nullptr,nullptr};return 0;}
 CharacterKillLiveBorrowV21 borrow(){return {id,controller,id,lease,&view,&life,&fields,&oid,&prop,&tracked,&outgoing,&handle};}
};
struct Fixture {
 data::PropertyRules rules;data::AiTables ai;std::unique_ptr<skills::CharacterWorldRuntimeV1> world;
 player::PlayerManagerOwnerV1 players{{this,manager}};
 std::shared_ptr<int> lease=std::make_shared<int>(1);
 std::unique_ptr<Actor> victim,player;KillWorld16 globals{0,0};KillLevel16 level{0x100000abcull,0,0};
 std::unique_ptr<CharacterKillLiveWorldV21> kills;
 std::vector<unsigned> operations;std::vector<KillQuest48> immediate;
 unsigned drops{},xp{},onkill{},ondied{};int reject{-1};std::string error;bool reenter{},reentered{};
 static bool manager(void*,const player::PlayerManagerRequestV1& q,player::PlayerManagerResponseV1& out,std::string&){if(q.operation==player::PlayerManagerOperationV1::construct_player_info)*q.player=player::PlayerInfoFieldsV1{};out.value=0;return true;}
 Fixture(){
  rules.types[36]=rules.types[23]=rules.types[24]=rules.types[25]=8;
  ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;
  victim=std::make_unique<Actor>(0x100000123ull,rules,1);player=std::make_unique<Actor>(0x100000456ull,rules,0);
  world=std::make_unique<skills::CharacterWorldRuntimeV1>(ai);
  assert(!world->add({victim->id,1,victim.get(),Actor::refresh,&victim->state,&victim->handle,&victim->tracked}));
  assert(!world->add({player->id,2,player.get(),Actor::refresh,&player->state,&player->handle,&player->tracked}));
  assert(players.initialize(error)&&players.add_player(0,0,0,true,error));player::PlayerInfoFieldsV1* info{};assert(players.get_by_internal(0,false,info,error));info->character660=player->id;
  player->tracked=victim->id;float threat=1;std::uint32_t bits;std::memcpy(&bits,&threat,4);victim->storage[0]={player->id,bits,0};victim->outgoing.count=1;
  kills=std::make_unique<CharacterKillLiveWorldV21>(*world,players,globals,CharacterKillLiveBackendsV21{lease,[this](auto& a,const auto& q,auto& r,auto& e){return backend(a,q,r,e);}});
  assert(kills->add(victim->borrow(),error));assert(kills->add(player->borrow(),error));
 }
 bool backend(KillActor56&,const KillRequest56& q,KillResponse16& out,std::string& e){
  operations.push_back(q.service);if(victim->life.dead!=1||victim->view.resolved[36]!=0){std::cerr<<"backend "<<q.service<<" dead "<<victim->life.dead<<" HP "<<victim->view.resolved[36]<<'\n';return false;}
  if(int(q.service)==reject){e="Required explicit test lifecycle backend";return false;}
  switch(q.service){
  case kill_current_level:out.pointer=reinterpret_cast<std::uintptr_t>(&level);break;
  case kill_drop_loot:assert(q.subject==victim->id&&q.target==player->id);++drops;if(reenter&&!reentered){reentered=true;assert(kill()==1&&!kills->complete(victim->id));}break;
  case kill_raise_event:if(q.argument==4){assert(q.subject==player->id&&q.target==victim->id);++onkill;}else{assert(q.argument==2&&q.subject==victim->id);++ondied;}break;
  case kill_is_local_player:out.word=q.subject==player->id;break;
  case kill_distribute_xp:assert(q.subject==player->id&&q.target==victim->id&&player->tracked==0&&victim->fields.killer144c==player->id);++xp;break;
  case kill_is_remotely_updated:out.word=0;break;
  case kill_constant:out.word=11+int(immediate.size());break;
  case kill_raise_async:assert(q.event&&q.event->oid==victim->oid);immediate.push_back(*q.event);break;
  default:e="Unsupported declared fixture Kill service";return false;
  }return true;
 }
 int kill(){return kills->command(victim->controller,victim->id,player->id,0);}
};
int main(){unsigned checks=0;
 for(const auto& gold:kill_aggro_gold_v21){data::AggroEntry entries[13]{};for(unsigned i=0;i<gold.count;++i)entries[i]={gold.entries[2*i],gold.entries[2*i+1],0};data::AggroTable table{entries,gold.count,13};std::uintptr_t id=0xa5a5a5a5;std::uint32_t threat=0xa5a5a5a5;std::string e;assert(source_kill_aggro_entry_v21(table,gold.index,id,threat,e));assert(id==gold.character&&threat==gold.threat);checks+=2;}
 {CharacterKillFieldsV21 f;std::string e;assert(f.construct_fresh(e)&&f.template13ca==-1&&!f.killer144c&&!f.master14d4&&!f.suppress_quest14e4);assert(!f.construct_fresh(e));CharacterKillFieldsV21 adopted;assert(adopted.adopt_observed(0x100000123ull,0x100000456ull,27,2,e));assert(adopted.template13ca==27&&adopted.suppress_quest14e4==2);checks+=4;}
 {Fixture f;const auto status=f.kill();if(status!=1){std::cerr<<"Kill status "<<status<<" "<<f.kills->error()<<" ops ";for(auto op:f.operations)std::cerr<<op<<',';std::cerr<<'\n';return 2;}assert(f.drops==1&&f.onkill==1&&f.xp==1&&f.ondied==1&&f.immediate.size()==2);assert(f.victim->life.dead==1&&f.player->tracked==0&&f.victim->fields.killer144c==f.player->id);assert(f.kills->complete(f.victim->id));const auto calls=f.operations.size();assert(f.kill()==1&&f.operations.size()==calls);checks+=5;}
 {Fixture f;assert(f.kills->command(f.victim->controller+1,f.victim->id,f.player->id,0)==-1);assert(!f.victim->life.dead&&f.victim->view.resolved[36]==1000&&f.operations.empty());checks+=2;}
 {Fixture f;f.reject=kill_current_level;assert(f.kill()==-2);assert(f.victim->life.dead==1&&f.victim->view.resolved[36]==0&&!f.drops&&!f.xp&&!f.ondied);const auto calls=f.operations.size();assert(f.kill()==-2&&f.operations.size()==calls);checks+=3;}
 {Fixture f;f.reject=kill_drop_loot;assert(f.kill()==-2);assert(f.kills->result(f.victim->id)->phase==kill_drop_loot+1&&!f.onkill&&!f.xp&&!f.ondied);checks+=2;}
 {Fixture f;f.reject=kill_raise_async;assert(f.kill()==-2);assert(f.drops==1&&f.onkill==1&&f.xp==1&&!f.ondied&&f.immediate.empty());checks+=2;}
 {Fixture f;auto foreign=f.victim->borrow();foreign.identity+=1;std::string e;assert(!f.kills->add(foreign,e));checks++;}
 {Fixture f;f.reenter=true;assert(f.kill()==1&&f.reentered&&f.drops==1&&f.xp==1&&f.ondied==1&&f.kills->complete(f.victim->id));checks++;}
 {Fixture f;f.reenter=true;f.reject=kill_raise_async;assert(f.kill()==-2&&f.reentered&&!f.kills->complete(f.victim->id));checks++;}
 std::cout<<"Kill live same-World/property/life/controller/C1 +originalAggro64 native PASS "<<checks<<" checks; loot/XP/quest/AI backends are explicit fixtures\n";
}
