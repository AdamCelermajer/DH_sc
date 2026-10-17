#include "../scrolling_combat_text_bridge_v1.hpp"
#include "../scrolling_combat_text_position_v1.hpp"
#include "../../scene-materials/scene.hpp"
#include "../../level-world/character_distribute_xp_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace ui=dh2::ui;
namespace xp=dh2::character_distribute_xp_v1;
namespace data=dh2::data;

namespace {
constexpr std::uintptr_t KILLER=0x1001;
constexpr std::uintptr_t VICTIM=0x2002;
constexpr std::uintptr_t XP_RECIPIENT=0x3003;

void check(bool ok,const char* message){
 if(!ok){std::fprintf(stderr,"FAIL: %s\n",message);std::exit(1);}
}

struct CharacterOwner {
 data::PropertyRules rules{};
 data::PropertyState state{};
 data::PropertyView properties{};
 data::PlayerSavegameV1 save{};
 xp::CharacterView view{};
 CharacterOwner(std::uintptr_t id,int level,bool player,float x=0.f,float y=0.f){
  rules.defaults.fill(0);rules.types.fill(0);
  rules.types[19]=32;rules.types[35]=32;rules.types[200]=4;
  data::reset_properties(rules,state);properties=data::property_view(rules,state);
  state.saved[19]=level*256;state.saved[35]=100*256;
  std::string error;check(data::recalc_properties(rules,state,error),"resolve Character fixture properties");
  if(player){save.set_character(id);save.set_unlocked_difficulty(0);}
  view={id,&properties,player?&save:nullptr,x,y};
 }
};

struct Fixture {
 CharacterOwner killer{KILLER,1,false};
 CharacterOwner victim{VICTIM,10,false};
 CharacterOwner recipient{XP_RECIPIENT,8,true};
 xp::State state{&killer.view,&victim.view};
 xp::Services distributor{};
 ui::ScrollingCombatTextBridgeV1 bridge{};
 dh2::scene::Scene victim_scene;
 float victim_position[3]{10.f,20.f,30.f};
 float victim_bounds[6]{-1.f,-2.f,-3.f,1.f,2.f,7.f};
 std::uintptr_t position_query{};
 std::uintptr_t awarded_to{};
 std::uintptr_t locality_query{};
 std::uintptr_t text_receiver{};
 std::int32_t displayed_xp{};
 float screen_x{},screen_y{};
 std::string text;
 std::uint8_t rgba[4]{};
 std::string error;

 Fixture(){
  victim_scene.graph.resize(2);
  victim_scene.graph[0].name="actor_root";victim_scene.graph[0].parent=-1;
  victim_scene.graph[1].name="target_node";victim_scene.graph[1].parent=0;
  victim_scene.graph[1].world[12]=1.f;
  victim_scene.graph[1].world[13]=2.f;
  victim_scene.graph[1].world[14]=3.f;
  distributor={this,design,player_count,player_at,give_xp,is_local,difficulty,scroll_xp};
  ui::ScrollingCombatTextBridgeProvidersV1 p{};
  p.source_context=this;p.is_follower=follower;p.position=position;
  p.source_property=property;p.is_dual_wielding=dual;p.is_local_player=locality;
  p.ui_context=this;p.constant=constant;p.localized_string=localized;
  p.localized_formatted_string=formatted;p.style_id=style;
  p.decode_color_rgba=decode;p.play_authored_screen=play;
  const float matrix[16]={1,0,0,0, 0,1,0,0, 0,0,0,1, 0,0,0,0};
  check(bridge.bind(p,matrix,100,80,error),"bind selected engine-ui bridge");
 }
 static std::int32_t design(void*,std::uint32_t offset,float* out,std::string&){
  switch(offset){case 140:*out=5;break;case 144:*out=-2;break;case 148:*out=120;break;
   case 152:*out=50;break;case 156:*out=5;break;case 160:*out=10;break;case 164:*out=10;break;default:return 1;}
  return 0;
 }
 static std::int32_t player_count(void*,std::int32_t* out,std::string&){*out=1;return 0;}
 static std::int32_t player_at(void* raw,std::uint32_t ordinal,std::uint32_t require,
                               xp::CharacterView* out,std::string&){
  auto& f=*static_cast<Fixture*>(raw);if(ordinal||require!=1)return 1;*out=f.recipient.view;return 0;
 }
 static std::int32_t give_xp(void* raw,xp::CharacterView* target,std::int32_t,
                             std::uint32_t update,std::uint32_t* success,std::string&){
  auto& f=*static_cast<Fixture*>(raw);if(!target||update!=1)return 1;
  f.awarded_to=target->identity;*success=1;return 0;
 }
 static std::int32_t is_local(void* raw,std::uintptr_t id,std::uint32_t* local,std::string&){
  auto& f=*static_cast<Fixture*>(raw);f.locality_query=id;*local=id==XP_RECIPIENT;return 0;
 }
 static std::int32_t difficulty(void*,std::int32_t* value,std::string&){*value=0;return 0;}
 static bool lookup_position(void* raw,std::uintptr_t id,
     ui::ScrollingCombatTextPositionFactsV1& out,std::string& error){
  auto& f=*static_cast<Fixture*>(raw);
  if(id!=VICTIM){error="position lookup must receive killed ObjectActor identity";return false;}
  f.position_query=id;
  out={VICTIM,f.victim_position,&f.victim_scene,f.victim_bounds,1};
  return true;
 }
 static bool position(void* raw,std::uintptr_t id,float out[3],std::string& error){
  ui::ScrollingCombatTextPositionLookupV1 lookup{raw,lookup_position};
  return ui::scrolling_combat_text_position_v1(&lookup,id,out,error);
 }
 static std::int32_t scroll_xp(void* raw,std::uintptr_t receiver,std::int32_t amount,std::string& error){
  auto& f=*static_cast<Fixture*>(raw);f.text_receiver=receiver;f.displayed_xp=amount;
  ui::ScrollingCombatTextResultV1 result{};
  const auto status=ui::apply_scrolling_combat_xp_v1(receiver,amount,
      f.bridge.services(),result,error);
  return status==ui::ScrollingCombatTextStatusV1::complete?0:1;
 }
 static bool follower(void*,std::uintptr_t,bool& out,std::string&){out=false;return true;}
 static bool property(void*,std::uintptr_t,std::int32_t,std::int32_t& out,std::string&){out=0;return true;}
 static bool dual(void*,std::uintptr_t,bool& out,std::string&){out=false;return true;}
 static bool locality(void* raw,std::uintptr_t id,bool& out,std::string&){
  auto& f=*static_cast<Fixture*>(raw);f.locality_query=id;out=id==XP_RECIPIENT;return true;
 }
 static bool constant(void*,const char* group,const char* key,std::int32_t& out,std::string&){
  if(std::string(group)=="ScrollingCombatText"&&std::string(key)=="XPColor")out=static_cast<std::int32_t>(0xff0ed98au);
  else if(std::string(group)=="StrID"&&std::string(key)=="GAMEPLAYMENUS_REWARD_XP")out=7001;
  else return false;return true;
 }
 static bool localized(void*,std::int32_t,std::string& out,std::string&){out="unused";return true;}
 static bool formatted(void*,std::int32_t id,std::int32_t amount,std::string& out,std::string&){
  if(id!=7001)return false;out="XP "+std::to_string(amount);return true;
 }
 static bool style(void*,const char* name,std::int32_t& out,std::string&){
  if(std::string(name)!="anim_sct_xp")return false;out=91;return true;
 }
 static bool decode(void*,std::int32_t value,std::uint8_t out[4],std::string&){
  ui::decode_scrolling_combat_text_color_rgb_v1(value,out);return true;
 }
 static bool play(void* raw,const char* style_name,std::uint32_t slot,float x,float y,
                  const char* value,const std::uint8_t color[4],std::string&){
  auto& f=*static_cast<Fixture*>(raw);
  if(std::string(style_name)!="anim_sct_xp"||slot!=UINT32_MAX||color[0]!=0x0e||color[1]!=0xd9||color[2]!=0x8a||color[3]!=255)return false;
  f.screen_x=x;f.screen_y=y;f.text=value;for(int i=0;i<4;++i)f.rgba[i]=color[i];return true;
 }
};
}

int main(){
 Fixture f;xp::Result result{};
 check(xp::distribute(&f.state,&f.distributor,&result,f.error)==xp::Status::complete,
       "selected XP distributor to SCT path failed");
 check(f.awarded_to==XP_RECIPIENT&&f.locality_query==XP_RECIPIENT,
       "XP award/locality target did not remain the separately resolved recipient");
 check(f.text_receiver==VICTIM&&f.position_query==VICTIM,
       "scrolling text position resolver was not given killed Character/ObjectActor identity");
 check(f.displayed_xp==result.last_displayed_xp&&f.displayed_xp>0&&
       f.text=="XP "+std::to_string(f.displayed_xp),
       "victim SCT did not receive the distributor's already-adjusted displayed XP");
 check(std::fabs(f.screen_x-53.84615f)<0.02f&&std::fabs(f.screen_y-33.84615f)<0.02f,
       "victim target_node absolute position plus relative-box height did not reach projection");
 std::puts("PASS: selected DistributeXP award recipient stays distinct from killed-victim SCT identity, scene position and playback");
}
