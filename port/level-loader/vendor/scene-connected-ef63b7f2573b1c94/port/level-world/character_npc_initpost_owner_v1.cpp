#include "character_npc_initpost_owner_v1.hpp"
#include "character_scene.hpp"
#include <cstring>
namespace dh2::character {
bool CharacterNpcInitPostOwnerV1::call(std::uint32_t entry,NpcInitPostResponseV1& out,std::uint32_t a,std::uint32_t b,std::uintptr_t payload,const char* text){
 last_entry_=entry;++calls_;out={};
 if(!services_.invoke||!services_.invoke(services_.context,{entry,a,b,fields_.identity,payload,text},out,error_)){
  if(error_.empty())error_="required source Character InitPost service "+std::to_string(entry);failed_=true;return false;
 }return true;
}
bool CharacterNpcInitPostOwnerV1::debug(){NpcInitPostResponseV1 r;return call(0x337888,r)&&call(0x337a88,r,0,0,0,"isTracingChar_Init");}
bool CharacterNpcInitPostOwnerV1::npc(){NpcInitPostResponseV1 r;if(!call(0x3a49f0,r))return false;if(r.value){error_="required whole player InitPost branch; NPC owner cannot accept IsPlayer=true";failed_=true;return false;}return true;}
bool CharacterNpcInitPostOwnerV1::initialize(){
 auto& f=fields_;if(failed_){error_="failed Character InitPost prefix cannot be retried";return false;}
 if(!f.identity||!f.called1394||!f.spawn_probability274||!f.starting_waypoint13e8||!f.room64||!f.properties_id13c8||!f.properties||!f.model290||!f.scale120||(!f.delayed3ec&&!f.live_delayed3ec)||!f.self_fx_offset1488||!f.self_fx1484||!f.visual2d8||!f.fade1440||!f.fade1444||!f.position160||!f.initial_position1450||!f.rotation16c||!f.initial_rotation145c){error_="required same Character InitPost field authority";return false;}
 if(*f.called1394)return true;
 *f.called1394=1;NpcInitPostResponseV1 r;
 if(!call(0x38bd64,r))return false;
 if(r.value>=*f.spawn_probability274)return true;
 if(!debug())return false;
 if(!f.starting_waypoint13e8->empty()){
  if(!call(0x34aca0,r,static_cast<std::uint32_t>(*f.room64),0,0,f.starting_waypoint13e8->c_str()))return false;
  const auto handle=r.identity;
  if(!call(0x33fdc0,r,0,0,handle))return false;
  if(r.identity){if(!call(0x33fee4,r,0,0,handle))return false;if(r.identity&&!call(0x405540,r,0,0,r.identity))return false;}
 }
 if(!call(0x3b3d38,r))return false;
 if(*f.properties_id13c8!=-1&&!debug())return false;
 const auto property_id=static_cast<std::uint32_t>(static_cast<std::int32_t>(*f.properties_id13c8));
 if(!call(0x3df2a4,r,property_id)||!call(0x3e0810,r,1)||!call(0x3a54d4,r))return false;
 if(r.text)*f.model290=r.text;
 if(!debug())return false;
 if(!f.properties->base){error_="required same live base Scale_X/Y/Z sheet";failed_=true;return false;}
 if(dh2_character_visual_scale(f.scale120,f.properties->base+12)!=0){error_="source Character visual scale failed";failed_=true;return false;}
 if(!call(0x38be5c,r)||!call(0x38ab60,r))return false;
 if(!r.value){if(!call(0xffffff40,r)||!call(0x33ddb4,r))return false;return true;}
 if(!call(0x3bc4d0,r,2)||!call(0x3a2fec,r))return false;
 const auto* ai=r.ai;if(!ai){error_="required captured actual AiProps row";failed_=true;return false;}
 if(!npc())return false;
 if(ai->delayed_load){auto* delayed=f.live_delayed3ec?f.live_delayed3ec(f.lifecycle_context):f.delayed3ec;if(!delayed){error_="required live same CharAI delayed field";failed_=true;return false;}*delayed=1;}
 else {if(!npc()||!call(0x3cf1f0,r))return false;}
 const auto fx_id=static_cast<std::uint32_t>(*f.self_fx_offset1488)+static_cast<std::uint32_t>(ai->self_fx);
 if(!call(0x495430,r,fx_id))return false;*f.self_fx1484=r.identity;
 if(!call(0x3b4738,r)||!debug()||!call(0x3c9f4c,r)||!debug()||!call(0x3b3b00,r)||!debug()||!npc())return false;
 if(!f.properties->resolved){error_="required same resolved FadeIn/FadeOut sheet";failed_=true;return false;}
 *f.fade1440=static_cast<float>(f.properties->resolved[210]);*f.fade1444=static_cast<float>(f.properties->resolved[211]);
 if(!call(0x3a58f4,r,0,0,reinterpret_cast<std::uintptr_t>(f.position160))||!call(0x393db4,r,1,0,reinterpret_cast<std::uintptr_t>(f.initial_position1450)))return false;
 std::memcpy(f.initial_rotation145c,f.rotation16c,3*sizeof(float));
 if(*f.visual2d8&&!call(0x470a54,r,0,0,*f.visual2d8))return false;
 if(!call(0x3a59ac,r,0,1)||!call(0x3b3a70,r))return false;
 auto* delayed=f.live_delayed3ec?f.live_delayed3ec(f.lifecycle_context):f.delayed3ec;
 if(!delayed){error_="required live same CharAI delayed field";failed_=true;return false;}
 if(!*delayed&&!call(0x3ce7c0,r,0))return false;
 return call(0x3d37d0,r)&&debug();
}
}
