#include "../combat_ctrl_kill_owner_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::character;using namespace dh2::data;
unsigned checks{};void check(bool x){++checks;if(!x)throw std::runtime_error("Kill composition check "+std::to_string(checks));}
std::vector<std::uint8_t> file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f));return {std::istreambuf_iterator<char>(f),{}};}
struct Graph {
 PropertyRules rules;PropertyState player_state,target_state;PropertyView player_props,target_props;
 CombatActorState player_life,target_life;KillActor56 player{},target{};KillLevel16 level{};KillWorld16 world{};
 CombatCtrlKillOwnerV1* owner{};bool fail_loot{},reentered{};unsigned drops{},xp{},events{},quests{};
 std::vector<unsigned> trace;
 static int service(void* raw,KillActor56* receiver,const KillRequest56* q,KillResponse16* out){
  auto& g=*static_cast<Graph*>(raw);check(receiver==&g.target);g.trace.push_back(q->service);
  if(q->service>kill_is_dead){check(g.target_life.dead==1&&g.target.dead==1&&g.target_props.resolved[36]==0);}
  switch(q->service){
   case kill_is_dead:out->word=g.target_life.dead;return 0;
   case kill_is_player:out->word=q->subject==g.player.identity;return 0;
   case kill_is_local_player:out->word=0;return 0; // explicit host nonlocal branch
   case kill_current_level:out->pointer=reinterpret_cast<std::uintptr_t>(&g.level);return 0;
   case kill_drop_loot:{++g.drops;KillResult24 nested{};std::string error;check(g.owner->ctrl_kill(nested,g.player.identity,0,error)==1&&nested.status==1);g.reentered=true;return g.fail_loot?-1:0;}
   case kill_aggro_count:out->word=1;return 0;
   case kill_aggro_entry:out->pointer=reinterpret_cast<std::uintptr_t>(&g.player);return 0;
   case kill_raise_event:++g.events;check(q->argument==4||q->argument==2);return 0;
   case kill_is_character:out->word=q->subject==g.player.identity;return 0;
   case kill_handle_character:out->pointer=g.player.identity;return 0;
   case kill_distribute_xp:++g.xp;check(q->subject==g.player.identity&&q->target==g.target.identity);return 0; // observer, not production XP provider
   case kill_is_remotely_updated:out->word=0;return 0;
   case kill_constant:check(q->name&&q->key);out->word=0;return 0; // explicit host constant observer
   case kill_raise_async:++g.quests;check(q->event&&q->event->killer==g.player.identity&&q->event->level==g.level.identity);return 0;
   default:return -1;
  }
 }
};
int main(){try{
 const std::string assets="port/android-native/app/src/main/assets/data/";auto a=file(assets+"character_properties_pyarray.bin"),b=file(assets+"character_properties_pyarraynames.bin"),c=file(assets+"character_properties_pystructnames.bin");CharacterTable actors;std::string error;check(load_characters({a.data(),a.size()},{b.data(),b.size()},{c.data(),c.size()},actors,error));
 for(bool failure:{false,true}){
  Graph g;check(load_property_rules(actors,g.rules,error));reset_properties(g.rules,g.player_state,&actors.rows[0]);reset_properties(g.rules,g.target_state,&actors.rows[0]);
  // Explicit host health/identity/Level projections over actual cache rules.
  // Metadata/quest/drop/XP endpoints below are observers, never production.
  g.player_state.base[38]=51200;g.target_state.base[38]=2560;
  check(recalc_properties(g.rules,g.player_state,error)&&recalc_properties(g.rules,g.target_state,error));
  g.player_props=property_view(g.rules,g.player_state);g.target_props=property_view(g.rules,g.target_state);
  check(!dh2_property_set(&g.player_props,36,256)&&!dh2_property_set(&g.target_props,36,2560));
  g.player={0x100000123ULL,&g.player_props,0,nullptr,0,1,0,1,0,0,{}};
  g.target={0x100000456ULL,&g.target_props,0,nullptr,0,2,0,1,0,0,{}};
  g.level={0x100000777ULL,0,0};g.fail_loot=failure;
  CombatCtrlKillOwnerV1 owner(g.target,g.target_life,g.world,{&g,Graph::service});g.owner=&owner;
  CombatResult result{};result.amount=5120;result.hp_leech=256;result.outcomes=0x160;
  MonsterApplicationRequest request{&result,&g.player_props,&g.target_props,&g.player_life,&g.target_life};
  CombatCtrlKillResultV1 output;auto status=combat_apply_ctrl_kill_v1(output,request,g.player.identity,owner,false,false,error);
  check(g.target_life.dead==1&&g.target.dead==1&&g.target_props.resolved[36]==0&&g.drops==1&&g.reentered);
  if(failure){check(status==-2&&!output.complete&&output.kill_reached&&!output.leech_reached&&g.target_life.lifecycle==0&&g.player_props.resolved[36]==256&&g.xp==0&&g.quests==0);}
  else{check(status==1&&output.complete&&output.leech_reached&&g.target_life.lifecycle==3&&g.player_props.resolved[36]>256&&g.xp==1&&g.quests==4&&g.events==2);check(!(result.outcomes&0x160));}
  auto before=g.trace.size();KillResult24 retry;check(owner.ctrl_kill(retry,g.player.identity,0,error)==1&&g.trace.size()==before+1&&g.drops==1&&g.xp==(failure?0u:1u));
 }
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<"}\n";
 }catch(std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
