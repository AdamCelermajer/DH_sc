#include "world_click_target_v1.hpp"
#include <cstring>
namespace dh2::world {
namespace {
bool missing(const char* name,std::string& e){e="required world Ctrl_Click source provider: ";e+=name;return false;}
float distance(const std::array<float,3>& a,const std::array<float,3>& b){const float x=a[0]-b[0],y=a[1]-b[1],z=a[2]-b[2];return ((x*x)+(y*y))+(z*z);}
bool debug(const WorldClickServicesV1& s,std::uint32_t site,bool& out,std::string& e){if(!s.debug)return missing("Debug caller",e);return s.debug(site,out,e);}
bool set(WorldClickFieldsV1& f,std::uintptr_t id,std::string& e){if(!f.target||!f.target_services.invoke)return missing("same whole AI_SetTarget owner/services",e);if(dh2_character_ai_set_target(f.target,id,0,&f.target_services)){e="whole source AI_SetTarget failed after prefix";return false;}return true;}
}
bool world_click_nearby_v1(const float* box,const float* point,float radius)noexcept{
 //38ac2c/34 and38ac70/78 construct c2c80000/42c80000 (-100/+100).
 if(!box||!point)return false;const float low=radius*-100.f,high=radius*100.f;
 for(unsigned i=0;i<3;++i)if(!(box[i]+low<=point[i])||!(box[i+3]+high>=point[i]))return false;return true;
}
bool world_click_target_v1(WorldClickFieldsV1& f,const std::array<float,3>& point,bool released,const WorldClickServicesV1& s,std::string& e){
 bool allowed=false;if(!s.ctrl_allowed)return missing("CTRLIsAllowed",e);if(!s.ctrl_allowed(allowed,e))return false;if(!allowed)return true;
 bool mode=false,casting=false,using_skill=false;
 if(!s.application_mode320e74)return missing("Application320e74",e);if(!s.application_mode320e74(mode,e))return false;
 if(!s.is_casting)return missing("same SM_IsCasting",e);if(!s.is_casting(casting,e))return false;
 if(!casting){if(!s.is_using_skill)return missing("same SM_IsUsingSkill",e);if(!s.is_using_skill(using_skill,e))return false;}
 if(!f.pending14c8||!f.skill14ca)return missing("same pending14c8/skill14ca",e);
 if(casting||using_skill){
  *f.pending14c8=released?0:1;
  if(!released){if(!f.destination14b0||!f.origin14bc)return missing("same pending click points",e);std::memcpy(f.destination14b0,point.data(),12);std::memcpy(f.origin14bc,point.data(),12);}
  *f.skill14ca=-1;return true;
 }
 *f.pending14c8=0;*f.skill14ca=-1;
 if(!s.characters)return missing("same ObjectManager Character list",e);
 std::uintptr_t cursor=0,id=0,chosen=0;float closest;const std::uint32_t initial=0x497423f0;std::memcpy(&closest,&initial,4);bool first=true;
 while(true){if(!s.characters(first,cursor,id,e))return false;first=false;if(!id)break;
  std::array<float,3> target;if(!s.target_position)return missing("GetTargetPosition3935dc",e);if(!s.target_position(id,target,e))return false;
  if(id==f.identity)continue;bool eligible=false;if(!s.interactive)return missing("virtual IsInteractive88",e);if(!s.interactive(id,f.identity,eligible,e))return false;if(!eligible)continue;
  if(mode){if(!s.enemy)return missing("whole AI_IsEnemy",e);bool enemy=false;if(!s.enemy(f.identity,id,enemy,e))return false;if(!enemy)continue;}
  if(!s.neutral)return missing("whole AI_IsNeutral",e);bool neutral=false;if(!s.neutral(f.identity,id,neutral,e))return false;if(neutral)continue;
  const float d=distance(point,target);if(!(closest>d))continue;
  if(!s.enemy)return missing("whole AI_IsEnemy radius branch",e);bool enemy=false;if(!s.enemy(f.identity,id,enemy,e))return false;
  const auto* radius=enemy?s.enemy_radius2c:s.friend_radius50;if(!radius)return missing("actual DesignSettings click radius",e);
  if(!s.nearby)return missing("whole IsNearby38ac0c",e);bool near=false;if(!s.nearby(id,point,*radius,near,e))return false;if(near){closest=d;chosen=id;}
 }
 if(chosen){bool ignored=false;if(!debug(s,0x3ae074,ignored,e))return false;if(!f.click_target413)return missing("same click target413",e);*f.click_target413=1;return set(f,chosen,e);}
 if(released&&!mode){
  if(!s.objects)return missing("same ObjectManager signed handle tree",e);first=true;cursor=0;std::memcpy(&closest,&initial,4);
  while(true){if(!s.objects(first,cursor,id,e))return false;first=false;if(!id)break;if(id==f.identity)continue;
   bool eligible=false;if(!s.interactive)return missing("generic IsInteractive88",e);if(!s.interactive(id,f.identity,eligible,e))return false;if(!eligible)continue;
   if(!s.object_type_name5c||!s.excluded_type_name)return missing("actual object5c/type literal",e);const char* type=nullptr;if(!s.object_type_name5c(id,type,e))return false;if(!type)return missing("source type5c string",e);if(!std::strcmp(type,s.excluded_type_name))continue;
   std::array<float,3> target;if(!s.target_position)return missing("generic GetTargetPosition",e);if(!s.target_position(id,target,e))return false;const float d=distance(point,target);if(!(closest>d))continue;
   if(!s.object_radius54||!s.nearby)return missing("actual generic click radius/IsNearby",e);bool near=false;if(!s.nearby(id,point,*s.object_radius54,near,e))return false;if(near){chosen=id;closest=d;}
  }
  if(chosen){bool ignored=false;if(!debug(s,0x3ae318,ignored,e))return false;return set(f,chosen,e);}
 }
 bool option=false;if(!debug(s,0x3ae208,option,e))return false;if(!option&&mode)return true;
 if(!debug(s,0x3ae248,option,e))return false;if(!set(f,0,e))return false;if(dh2_character_ai_sync_last_target(f.target)){e="source AI_SyncLastTarget unavailable";return false;}
 if(!s.move)return missing("actual virtual movement e4/ec",e);return s.move(point,released,e);
}
}
