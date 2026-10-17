#include "../scrolling_combat_text_bridge_v1.hpp"
#include "../../game-data/player_attack_result_edge_v1.hpp"
#include "../../game-data/combat_application.hpp"
#include "../../game-data/properties.hpp"
#include "../scrolling_combat_text_position_v1.hpp"
#include "../../scene-materials/scene.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace data = dh2::data;
namespace ui = dh2::ui;
namespace edge = dh2::data::player_attack_result_edge_v1;

namespace {
constexpr std::uintptr_t ATTACKER=0x1001;
constexpr std::uintptr_t DEFENDER=0x2002;

void check(bool ok,const char* message){
 if(!ok){std::fprintf(stderr,"FAIL: %s\n",message);std::exit(1);}
}

struct Fixture {
 data::PropertyRules rules{};
 data::PropertyState attacker_state{},defender_state{};
 data::PropertyView attacker{},defender{};
 data::CombatActorState attacker_life{},defender_life{};
 data::CombatResult result{};
 data::MonsterApplicationRequest application{};
 ui::ScrollingCombatTextBridgeV1 bridge{};
 dh2::scene::Scene scene;
 float fallback_position[3]={0,0,2};
 float bounds[6]={0,0,0,0,0,0};
 std::vector<edge::Step> steps;
 std::uintptr_t position_identity{},property_identity{},dual_identity{},locality_identity{};
 std::string played_style,played_text;
 float screen[2]{};
 std::uint8_t color[4]{};
 std::string error;

 Fixture(){
  rules.types.fill(8);attacker_state.resolved[204]=256;
  defender_state.resolved[36]=2000;
  attacker=data::property_view(rules,attacker_state);
  defender=data::property_view(rules,defender_state);
  result.amount=512;result.outcomes=0x20u;
  application={&result,&attacker,&defender,&attacker_life,&defender_life};
  scene.graph.resize(2);scene.graph[0].name="actor_root";scene.graph[0].parent=-1;
  scene.graph[1].name="target_node";scene.graph[1].parent=0;
  scene.graph[1].world[12]=0.f;scene.graph[1].world[13]=0.f;scene.graph[1].world[14]=2.f;

  ui::ScrollingCombatTextBridgeProvidersV1 p{};
  p.source_context=this;p.is_follower=follower;p.position=position;
  p.source_property=property;p.is_dual_wielding=dual;p.is_local_player=local_player;
  p.ui_context=this;p.constant=constant;p.localized_string=localized;
  p.localized_formatted_string=formatted;p.style_id=style;
  p.decode_color_rgba=decode;p.play_authored_screen=play;
  const float vp[16]={1,0,0,0, 0,1,0,0, 0,0,0,1, 0,0,0,0};
  check(bridge.bind(p,vp,100,80,error),"bind source SCT runtime bridge");
 }

 static bool follower(void* raw,std::uintptr_t id,bool& out,std::string&){
  auto& f=*static_cast<Fixture*>(raw);if(id!=DEFENDER)return false;out=false;return true;
 }
 static bool position_facts(void* raw,std::uintptr_t id,
      ui::ScrollingCombatTextPositionFactsV1& out,std::string& error){
  auto& f=*static_cast<Fixture*>(raw);if(id!=DEFENDER){error="position must target defender";return false;}
  f.position_identity=id;out={id,f.fallback_position,&f.scene,f.bounds,1};return true;
 }
 static bool position(void* raw,std::uintptr_t id,float xyz[3],std::string& error){
  ui::ScrollingCombatTextPositionLookupV1 lookup{raw,position_facts};
  return ui::scrolling_combat_text_position_v1(&lookup,id,xyz,error);
 }
 static bool property(void* raw,std::uintptr_t id,std::int32_t,
                      std::int32_t& out,std::string&){
  auto& f=*static_cast<Fixture*>(raw);if(id!=ATTACKER)return false;
  f.property_identity=id;out=0;return true;
 }
 static bool dual(void* raw,std::uintptr_t id,bool& out,std::string&){
  auto& f=*static_cast<Fixture*>(raw);if(id!=ATTACKER)return false;
  f.dual_identity=id;out=false;return true;
 }
 static bool local_player(void* raw,std::uintptr_t id,bool& out,std::string&){
  auto& f=*static_cast<Fixture*>(raw);if(id!=DEFENDER)return false;
  f.locality_identity=id;out=false;return true;
 }
 static bool constant(void*,const char* group,const char* key,
                      std::int32_t& out,std::string& error){
  if(std::string(group)=="StrID"&&std::string(key)=="INGAME_ATTACK_DAMAGE")out=44;
  else if(std::string(group)=="ScrollingCombatText"&&std::string(key)=="DamageColor")out=static_cast<std::int32_t>(0xff334455u);
  else{error="unexpected source constant";return false;}return true;
 }
 static bool localized(void*,std::int32_t id,std::string& out,std::string& error){
  if(id!=44){error="unexpected source string id";return false;}out="Damage";return true;
 }
 static bool formatted(void*,std::int32_t,std::int32_t,std::string& out,std::string&){
  out.clear();return true;
 }
 static bool style(void*,const char* name,std::int32_t& out,std::string& error){
  if(std::string(name)!="anim_sct_normaldamage"){error="unexpected animation style";return false;}
  out=7;return true;
 }
 static bool decode(void*,std::int32_t value,std::uint8_t rgba[4],std::string&){
  ui::decode_scrolling_combat_text_color_rgb_v1(value,rgba);return true;
 }
 static bool play(void* raw,const char* style_name,std::uint32_t slot,float x,float y,
                  const char* text,const std::uint8_t rgba[4],std::string& error){
  auto& f=*static_cast<Fixture*>(raw);
  if(std::string(style_name)!="anim_sct_normaldamage"||slot!=UINT32_MAX||
     !text||std::string(text)!="2"||rgba[0]!=0x33||rgba[1]!=0x44||
     rgba[2]!=0x55||rgba[3]!=255){error="authored playback payload mismatch";return false;}
  f.played_style=style_name;f.played_text=text;f.screen[0]=x;f.screen[1]=y;
  for(int i=0;i<4;++i)f.color[i]=rgba[i];return true;
 }
 static int invoke(void* raw,edge::Step step,std::uintptr_t attacker,
                   std::uintptr_t defender,data::CombatResult& result){
  auto& f=*static_cast<Fixture*>(raw);f.steps.push_back(step);
  if(attacker!=ATTACKER||defender!=DEFENDER)return -1;
  if(step==edge::Step::combat_text){
   ui::ScrollingCombatTextResultV1 output{};std::string error;
   const auto status=ui::apply_scrolling_combat_text_v1(result,defender,attacker,
       f.bridge.services(),output,error);
   if(status!=ui::ScrollingCombatTextStatusV1::complete){
    std::fprintf(stderr,"SCT status=%u checks=%u error=%s\n",unsigned(status),output.source_checks,error.c_str());return -2;
   }
   return 0;
  }
  return 0;
 }
};
}

int main(){
 Fixture f;
 edge::Request request{&f.application,ATTACKER,DEFENDER,&f,&Fixture::invoke};
 edge::Result output{};
 const auto status=edge::execute(&request,&output);
 if(status!=edge::Status::complete)std::fprintf(stderr,"edge status=%u last=%u provider=%d completed=%u/%u\n",unsigned(status),unsigned(output.last_step),output.provider_status,output.steps_completed,output.steps_attempted);
 check(status==edge::Status::complete,
       "selected player attack result edge through SCT runtime bridge failed");
 check(output.steps_completed==4&&f.steps.size()==4&&
       f.steps[0]==edge::Step::cancel_sneaking&&f.steps[1]==edge::Step::combat_text&&
       f.steps[2]==edge::Step::combat_sound&&f.steps[3]==edge::Step::ai_combat_result,
       "combat text did not remain in source attack-result tail order");
 check(f.position_identity==DEFENDER&&f.property_identity==ATTACKER&&
       f.dual_identity==ATTACKER&&f.locality_identity==DEFENDER,
       "SCT actor/object providers received the wrong source identities");
 if(!(f.played_style=="anim_sct_normaldamage"&&f.played_text=="2"&&
       std::fabs(f.screen[0]-50.f)<0.01f&&std::fabs(f.screen[1]-40.f)<0.01f))
  std::fprintf(stderr,"play style=%s text=%s screen=%f,%f color=%u,%u,%u,%u\n",f.played_style.c_str(),f.played_text.c_str(),f.screen[0],f.screen[1],f.color[0],f.color[1],f.color[2],f.color[3]);
 check(f.played_style=="anim_sct_normaldamage"&&f.played_text=="2"&&
       std::fabs(f.screen[0]-50.f)<0.01f&&std::fabs(f.screen[1]-40.f)<0.01f,
       "defender target-node world position did not reach authored screen playback");
 std::puts("PASS selected player attack edge -> source SCT bridge -> defender target position -> authored playback");
}
