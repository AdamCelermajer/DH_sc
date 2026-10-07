#include "hud_manager.hpp"
#include "hud_player_values.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
namespace dh2::ui { namespace {
using Op=HudManagerOperation;
template<class T> bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
std::int32_t wrap_sub(std::int32_t a,std::int32_t b){return static_cast<std::int32_t>(static_cast<std::uint32_t>(a)-static_cast<std::uint32_t>(b));}
std::int32_t trunc_arm(float x){if(std::isnan(x))return 0;if(x>=2147483648.f)return INT32_MAX;if(x<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(x);}
std::int32_t pct(float f){volatile float v=f*100.f;return trunc_arm(v);}
const char* paths[]={
 "HUDelements.HealthBars.player.bar_hp","HUDelements.HealthBars.btn_potion.DistressGlow","_root.HurtCorners",
 "HUDelements.HealthBars.player.bar_mp","HUDelements.HealthBars.player.bar_xp","HUDelements.HealthBars.btn_potion.cnt.value",
 "HUDelements.controls.controls.btn_spell.CoolDown","HUDelements.controls.controls.btn_spell.Grey",
 "HUDelements.controls.controls.list.btn_0","HUDelements.controls.controls.list.btn_pre0","HUDelements.controls.controls.list.btn_post0",
 "HUDelements.controls.controls.list.btn_0.CoolDown","HUDelements.controls.controls.list.btn_pre0.CoolDown","HUDelements.controls.controls.list.btn_post0.CoolDown",
 "HUDelements.controls.controls.list.btn_0.Grey","HUDelements.controls.controls.list.btn_pre0.Grey","HUDelements.controls.controls.list.btn_post0.Grey",
 "HUDelements.btn_charactermenu.anim_levelup","HUDelements.controls.controls.Joystick","HUDelements.HealthBars.enemy",
 "HUDelements.HealthBars.enemy.enemy_name.text","HUDelements.HealthBars.enemy.enemy_level.value","HUDelements.HealthBars.enemy.HpBar.bar_hp",
 "HUDelements.btn_charactermenu.btimg.HudChar","_root.DeathTimer","_root.DeathTimer.DeathNumberTimeMc.TimerNumber_txt",
 "HUDelements.HealthBars.Ally0","HUDelements.HealthBars.Ally1","HUDelements.HealthBars.Ally2"};
const char* cd[]={"HUDelements.controls.controls.btn_skill1.CoolDown","HUDelements.controls.controls.btn_skill2.CoolDown","HUDelements.controls.controls.btn_skill3.CoolDown"};
const char* grey[]={"HUDelements.controls.controls.btn_skill1.Grey","HUDelements.controls.controls.btn_skill2.Grey","HUDelements.controls.controls.btn_skill3.Grey"};
struct Run {
 HudManagerState& s;const HudManagerServices& services;int failure=0;
 HudManagerResponse call(Op op,std::uint32_t i=0,std::int32_t v=0,std::int32_t other=0,std::uintptr_t subject=0,std::uintptr_t fx=0,const char* text=nullptr,const void* payload=nullptr,const float* xyz=nullptr){
  HudManagerResponse out{};if(failure)return out;
  HudManagerRequest q{op,i,v,other,subject,fx,text,payload,{0,0,0},0};if(xyz)std::memcpy(q.xyz,xyz,sizeof q.xyz);
  if(services.invoke(services.context,&s,&q,&out)!=1)failure=-2;
  return out;
 }
 std::uintptr_t get(std::uint32_t i){return call(Op::cache_get,i).identity;}
 bool checked(std::uintptr_t p){if(!p&&!failure)failure=-1;return p!=0&&!failure;}
 HudManagerPlayer* player(){auto*p=reinterpret_cast<HudManagerPlayer*>(call(Op::local_player,0,0).identity);if(!aligned(p)&&!failure)failure=-1;return failure?nullptr:p;}
 HudManagerActor* actor(HudManagerPlayer*p){if(!p)return nullptr;auto*a=p->actor;if(a&&!aligned(a))failure=-1;return failure?nullptr:a;}
 std::int32_t option(const char* p){return call(Op::saved_option,0,0,0,0,0,p).value;}
 void visible(std::uintptr_t p,std::uint8_t v){if(checked(p))call(Op::visible,0,v,0,p);}
 void go_clip(std::uint32_t i,std::uintptr_t fx,std::uintptr_t p,std::int32_t frame){if(!p||failure)return;if(!call(Op::sprite_type,i,2,0,p,fx).value||failure)return;call(Op::goto_frame,i,frame,0,p,fx);if(!failure)call(Op::play_state,i,1,0,p,fx);}
 void go(std::uint32_t i,std::int32_t frame){auto fx=s.render_fx;auto p=get(i);go_clip(i,fx,p,frame);}
 void text(std::uint32_t i,const char*t){auto fx=s.render_fx;auto p=get(i);if(!failure)call(Op::text,i,0,0,p,fx,t);}
 void label(std::uint32_t i,const char*t){auto fx=s.render_fx;auto p=get(i);if(!failure)call(Op::goto_label,i,0,0,p,fx,t);}
 float cooldown(std::uintptr_t script){return call(Op::cooldown,0,0,0,script).fraction;}
 void initialize(){
  if(!s.render_fx)return;
  auto style=option("HUDStyle");char root[64];std::snprintf(root,sizeof root,"_root.menu_HUD_%d",style);
  auto base=call(Op::root_lookup,0,0,0,0,s.render_fx,root).identity;
  auto init=[&](std::uint32_t i){call(Op::cache_initialize,i,style,0,(i==2||i==24||i==25)?0:base,s.render_fx,hud_manager_cache_path(i,style));};
  for(std::uint32_t i=0;i<8&&!failure;++i)init(i);
  if(style>1){for(std::uint32_t i=0;i<3&&!failure;++i){init(8+i);init(11+i);init(14+i);}}
  else for(std::uint32_t i=0;i<3&&!failure;++i){init(11+i);init(14+i);}
  for(auto i:{17u,18u,19u,20u,21u,22u,26u,27u,28u,23u,24u,25u})if(!failure)init(i);
  if(!failure)s.initialized=1;
 }
 void one_time(){
  auto p=get(18);auto dpad=option("DPad");visible(p,dpad!=0);if(failure)return;
  auto a=actor(player());if(!a||failure)return;
  auto cls=call(Op::player_class,0,0,0,reinterpret_cast<std::uintptr_t>(a)).value;
  auto frame=(cls>=290&&cls<=292)?2:(cls>=325&&cls<=327)?1:0;go(23,frame);
 }
 void slow(){
  auto a=actor(player());if(!a||failure)return;const auto id=reinterpret_cast<std::uintptr_t>(a);
  std::uint8_t usable[3]{};
  for(std::uint32_t i=0;i<3&&!failure;++i){auto slot=call(Op::skill_slot,i,0,0,id).value;if(slot!=-1)usable[i]=static_cast<std::uint8_t>(call(Op::skill_usable,static_cast<std::uint32_t>(slot),0,0,id).value);}
  auto p=get(7);auto spell=call(Op::spell_usable,0,0,0,id).value;visible(p,static_cast<std::uint8_t>(static_cast<std::uint32_t>(spell)^1u));
  auto style=option("HUDStyle");
  for(std::uint32_t i=0;i<3&&!failure;++i){std::uint32_t slot=i;
   if(style>1){auto button=get(8+i);if(!checked(button))return;slot=static_cast<std::uint32_t>(call(Op::as_slot_id,i,0,0,button,0,"SlotId").value);if(slot>2)slot=0;}
   auto p1=get(14+i);if(p1&&!failure){auto p2=get(14+i);visible(p2,usable[slot]^1u);}
  }
 }
 static int values(void*ctx,HudValuesState24*v,const HudValueRequest32*q,HudValueResponse16*out){auto&r=*static_cast<Run*>(ctx);static const std::uint32_t mapping[]={0,3,4,1,2};auto i=mapping[static_cast<std::uint32_t>(q->index)];
  if(q->operation==HudValueOperation::resolve_clip){out->clip=r.get(i);v->render_fx=r.s.render_fx;}
  else if(q->operation==HudValueOperation::is_sprite){out->value=r.call(Op::sprite_type,i,2,0,q->clip,q->render_fx).value;}
  else if(q->operation==HudValueOperation::goto_frame){r.call(Op::goto_frame,i,q->value,0,q->clip,q->render_fx);}
  else if(q->operation==HudValueOperation::set_play_state){r.call(Op::play_state,i,q->value,0,q->clip,q->render_fx);v->render_fx=r.s.render_fx;}
  else if(q->operation==HudValueOperation::divide_zero){out->value=r.call(Op::divide_zero,i,q->value,q->other).value;}
  return r.failure?0:1;
 }
 void status(HudManagerActor*a){if(!aligned(a->resolved)||a->resolved_count<44){failure=-1;return;}HudValuesState24 v{a->resolved,a->resolved_count,0,s.render_fx};HudValueServices16 sv{this,values};auto result=dh2_ui_hud_player_values(&v,&sv);if(result&&!failure)failure=result;}
 void online(HudManagerActor*a){
  auto local=player();if(!local||failure)return;
  if(local->death_ms>=0){char str[32];std::snprintf(str,sizeof str,"%d",local->death_ms/1000);text(25,str);}
  const auto count=call(Op::player_count).value;std::int32_t index=0;
  for(std::int32_t n=0;n<3&&!failure;++n){auto ally=get(26+n);HudManagerAllies args{};args.index=n;args.death_seconds=-1;const char* name="";
   while(index<count&&!failure){if(index>=65536){failure=-1;return;}auto*p=reinterpret_cast<HudManagerPlayer*>(call(Op::player_at,static_cast<std::uint32_t>(index),0).identity);if(!aligned(p)){if(!failure)failure=-1;return;}
    if(call(Op::player_remote,0,0,0,reinterpret_cast<std::uintptr_t>(p)).value){++index;ally=get(26+n);continue;}
    auto other=actor(p);if(other&&p->ready&&!failure){args.level=p->cached_level;
     args.hp_frame=std::clamp(wrap_sub(pct(call(Op::hp_fraction,0,0,0,reinterpret_cast<std::uintptr_t>(other)).fraction),1),0,99);
     name=p->name;if(!name){failure=-1;return;}args.death_seconds=p->death_ms>=0?p->death_ms/1000:-1;
     ++index;auto live=actor(p);if(!live){if(!failure)failure=-1;return;}float xyz[3];std::memcpy(xyz,live->position,sizeof xyz);volatile float z=xyz[2]+350.f;xyz[2]=z;
     auto screen=call(Op::project_position,0,0,0,0,0,nullptr,nullptr,xyz);
     auto invx=call(Op::inverse_pixel_x,0,0,0,0,s.render_fx).fraction;
     auto invy=call(Op::inverse_pixel_y,0,0,0,0,s.render_fx).fraction;
     volatile float sx=invx*static_cast<float>(screen.xy[0]),sy=invy*static_cast<float>(screen.xy[1]);
     call(Op::position,26+n,trunc_arm(sx),trunc_arm(sy),ally,s.render_fx);args.present=true;
    }
    break;
   }
   args.name=call(Op::format_multiplayer,0,1,0,0,0,name).text;if(failure)return;if(!args.name){failure=-1;return;}
   auto fx=s.render_fx;auto root=call(Op::root_character,0,0,0,0,fx).identity;
   call(Op::allies_callback,26+n,6,0,root,fx,"AlliesBarDisplay",&args);
  }(void)a;
 }
 void enemy(HudManagerActor*a){
  const auto id=reinterpret_cast<std::uintptr_t>(a);std::int32_t eligible=0;
  if(!call(Op::is_dead,0,0,0,id).value){auto*raw=a->target;if(raw){if(!aligned(raw)){failure=-1;return;}
   if(call(Op::is_character,0,0,0,reinterpret_cast<std::uintptr_t>(raw)).value){auto*reload=a->target;if(!reload){failure=-1;return;}if(call(Op::is_monster,0,0,0,reinterpret_cast<std::uintptr_t>(reload)).value)eligible=1;}}}
  auto*ai=reinterpret_cast<HudManagerActor*>(call(Op::target_character,0,0,0,id).identity);
  HudManagerActor* target=nullptr;
  bool chose_ai=false;
  if(ai){ai=reinterpret_cast<HudManagerActor*>(call(Op::target_character,0,0,0,id).identity);if(!aligned(ai)){if(!failure)failure=-1;return;}
   if(call(Op::is_monster,0,0,0,reinterpret_cast<std::uintptr_t>(ai)).value){chose_ai=true;target=reinterpret_cast<HudManagerActor*>(call(Op::target_character,0,0,0,id).identity);}}
  if(!chose_ai&&eligible)target=a->target;
  if(failure)return;
  if(!target){if(!s.cached_target)return;s.cached_target=nullptr;call(Op::cached_target_store);visible(get(19),0);go(22,0);label(19,"Hide");return;}
  if(!aligned(target)){failure=-1;return;}
  if(target!=s.cached_target){s.cached_target=target;call(Op::cached_target_store,0,0,0,reinterpret_cast<std::uintptr_t>(target));
   auto name=call(Op::string_symbol,0,static_cast<std::int32_t>(target->name_symbol),0,reinterpret_cast<std::uintptr_t>(target)).text;if(!name&&!failure){failure=-1;return;}
   char leveltext[32]="??";if(!call(Op::is_boss,0,0,0,reinterpret_cast<std::uintptr_t>(target)).value){auto level=call(Op::level,0,0,0,reinterpret_cast<std::uintptr_t>(target)).value;if(level!=-1)std::snprintf(leveltext,sizeof leveltext,"%d",level);}
   visible(get(19),1);label(19,"Show");call(Op::debug_load);auto debug=call(Op::debug_switch,0,0,0,0,0,"ShowMonstersNamesAndNetID").value;
   if(debug){std::snprintf(leveltext,sizeof leveltext,"%d",target->network_id);text(20,target->debug_name);text(21,leveltext);}
   else {text(20,name);text(21,leveltext);}
  }
  auto fraction=call(Op::hp_fraction,0,0,0,reinterpret_cast<std::uintptr_t>(target)).fraction;auto fx=s.render_fx;auto p=get(22);
  go_clip(22,fx,p,wrap_sub(std::min(pct(fraction),100),1));
 }
 void fast(){
  auto a=actor(player());if(!a||failure)return;auto id=reinterpret_cast<std::uintptr_t>(a);
  auto number=call(Op::potions,0,0,0,id).value;char str[32];std::snprintf(str,sizeof str,"%d",number);text(5,str);status(a);if(failure)return;
  auto levelup=get(17);auto available=call(Op::property_int,148,0,0,id).value;visible(levelup,available!=0);
  float fractions[3]{};
  for(std::uint32_t i=0;i<3&&!failure;++i){auto slot=call(Op::skill_slot,i,0,0,id).value;if(slot!=-1){if(!aligned(a->skills)||static_cast<std::uint32_t>(slot)>=a->skill_count){failure=-1;return;}auto script=a->skills[static_cast<std::uint32_t>(slot)];if(script)fractions[i]=cooldown(script);}}
  auto style=option("HUDStyle");
  for(std::uint32_t i=0;i<3&&!failure;++i){std::uint32_t slot=i;if(style>1){auto button=get(8+i);if(!checked(button))return;slot=static_cast<std::uint32_t>(call(Op::as_slot_id,i,0,0,button,0,"SlotId").value);if(slot>2)slot=0;}
   auto fx=s.render_fx;auto p=get(11+i);go_clip(11+i,fx,p,std::max(wrap_sub(pct(fractions[slot]),1),0));}
  if(!aligned(a->skills)||!a->skill_count){failure=-1;return;}
  if(a->skills[0]){auto fx=s.render_fx;auto p=get(6);if(!aligned(a->spells)||!a->spell_count||!a->spells[0]){failure=-1;return;}auto f=cooldown(a->spells[0]);go_clip(6,fx,p,std::max(wrap_sub(pct(f),1),0));}
  if(call(Op::online).value)online(a);
  if(!failure)enemy(a);
 }
 void flush(){
  const auto level=call(Op::current_level_flush);if(failure||!level.identity||!level.value)return;
  for(std::int32_t index=0;index<3&&!failure;++index){HudManagerAllies args{};
   args.name="";args.hp_frame=100;args.index=index;args.death_seconds=-1;
   const auto fx=s.render_fx;const auto root=call(Op::root_character,0,0,0,0,fx).identity;
   call(Op::allies_callback,26+index,6,0,root,fx,"AlliesBarDisplay",&args);
  }
  if(failure)return;visible(get(19),0);if(failure||!s.cached_target)return;
  const auto alive=call(Op::cached_target_alive,0,0,0,reinterpret_cast<std::uintptr_t>(s.cached_target)).value;
  if(failure)return;if(!alive){s.cached_target=nullptr;call(Op::cached_target_store);return;}
  const auto fx=s.render_fx;const auto timer=get(24);if(checked(timer))call(Op::goto_label,24,1,0,timer,fx,"Hide");
  if(!failure)visible(get(24),0);
 }
 void update(){
  auto level=call(Op::current_level);if(failure||(level.identity&&!level.value))return;
  if(!s.render_fx)return;
  if(!s.initialized){auto old=s.initialized;initialize();if(!failure)one_time();if(failure)return;s.cached_target=reinterpret_cast<HudManagerActor*>(static_cast<std::uintptr_t>(old));}
  if(s.slow_ms<0){s.slow_ms=500;slow();}
  else {auto old=s.slow_ms;auto dt=call(Op::elapsed).value;if(failure)return;s.slow_ms=wrap_sub(old,dt);}
  if(!failure)fast();
 }
};
}
const char* hud_manager_cache_path(std::uint32_t i,std::int32_t style)noexcept {if(i>28)return nullptr;if(style<=1&&i>=11&&i<14)return cd[i-11];if(style<=1&&i>=14&&i<17)return grey[i-14];return paths[i];}
}
extern "C" int dh2_ui_hud_manager_v1(dh2::ui::HudManagerState*s,std::uint32_t entry,const dh2::ui::HudManagerServices*services)noexcept{
 using namespace dh2::ui;if(!aligned(s)||!aligned(services)||!services->invoke||entry>5)return -1;
 Run run{*s,*services};switch(static_cast<HudManagerEntry>(entry)){case HudManagerEntry::update:run.update();break;case HudManagerEntry::initialize:run.initialize();break;case HudManagerEntry::one_time:run.one_time();break;case HudManagerEntry::fast:run.fast();break;case HudManagerEntry::slow:run.slow();break;case HudManagerEntry::flush:run.flush();break;}return run.failure;
}
extern "C" int dh2_ui_hud_enemy_v1(dh2::ui::HudManagerState* s,dh2::ui::HudManagerActor* actor,const dh2::ui::HudManagerServices* services)noexcept{
 using namespace dh2::ui;
 if(!aligned(s)||!aligned(actor)||!aligned(services)||!services->invoke)return -1;
 Run run{*s,*services};run.enemy(actor);return run.failure;
}
