#include "source_campaign_projectile_methods_v112.hpp"
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_retirement_v88.hpp"
#include "projectile_process_rows_v112.hpp"
#include "source_campaign_spawn_groups_v108.hpp"
#include "renderer_character_campaign_v62.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <canonical_gameobject_graph_v68.hpp>
#include <projectile_precache_source_v96.hpp>
#include <projectile_target_methods_v113.hpp>
#include <character_skill_projectile_callbacks_v112.hpp>
#include <source_assertion_process_v76.hpp>
#include <script_object_bridge.h>
#include <cmath>
#include <cstring>
#include <cstdio>
namespace model_renderer {namespace {
bool required(const char* leaf,std::string& e){if(e.empty())e=std::string("Required source projectile ")+leaf;return false;}
const dh2::character::skills::State40* skills(const dh2::world::CanonicalCharacterCandidateRecordV60& record){
 if(record.player_script_owner_v62)return &record.player_script_owner_v62->state();
 if(record.npc_skills_v84)return &record.npc_skills_v84->owner().state();return nullptr;
}
bool contains(const dh2::character::skills::State40& state,const dh2::character::skills::Instance32* instance){
 for(const auto& list:{state.skills,state.spells}){if(list.count&&!list.items)return false;
  for(std::uint32_t i=0;i<list.count;++i)if(list.items[i]==instance)return true;}
 return false;
}
}
class SourceCampaignProjectilesV112:public std::enable_shared_from_this<SourceCampaignProjectilesV112> {
 std::weak_ptr<SourceWorldBorrowV61> world_;
 SourceCampaignProjectileMethodsNativeV112 native_;
 std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays_;
public:
 SourceCampaignProjectilesV112(std::shared_ptr<SourceWorldBorrowV61> world,
  SourceCampaignProjectileMethodsNativeV112 native,std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays):
  world_(world),native_(std::move(native)),arrays_(std::move(arrays)){}
 bool scope(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world_.lock();if(!w||!borrow_source_campaign_candidate_runtime_v61(c,e)||
    c.actual_world!=w->owner||c.application!=w->application||!w->canonical_world||
    c.objects!=w->canonical_world->manager_lease||source_campaign_retirement_requested_v88())
   return required("current SAME campaign/manager scope",e);
  return true;
 }
 bool lend(dh2::world::CanonicalProjectileV99& projectile,dh2::world::ProjectileMethodServicesV112& s,
  dh2::world::LaserProjectileServicesV112& laser,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!scope(c,w,e)||!native_.lend(projectile,s,laser,e)||!bind_projectile_process_rows_v112(arrays_,s,e))return false;
  const auto weak=weak_from_this();s.callback=[weak](std::uintptr_t kind,auto& projectile,std::uintptr_t userdata,std::int32_t& value,std::string& e){
   auto self=weak.lock();return self&&self->callback(kind,projectile,userdata,value,e);};
  s.virtual_set_info_bool=[weak](auto& p,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
   std::uintptr_t check,std::uintptr_t hit,std::uintptr_t data,bool flag,std::string& e){auto self=weak.lock();
   dh2::world::ProjectileMethodServicesV112 common;dh2::world::LaserProjectileServicesV112 laser;
   if(!self||!self->lend(p,common,laser,e))return false;
   return p.is_laser_type()?dh2::world::laser_projectile_set_info_v112(p,row,source,target,check,hit,data,flag,common,laser,e):
    dh2::world::projectile_set_info_v112(p,row,source,target,check,hit,data,flag,common,e);};
  s.despawn=[weak](auto& projectile,std::uintptr_t id,bool type,std::string& e){auto self=weak.lock();
   SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;dh2::loader::ProjectileManagerPrecacheBorrowV96 manager;
   if(!self||!self->scope(c,w,e)||!w->projectile_precache_v96||!w->projectile_precache_v96->borrow_manager||
      !w->projectile_precache_v96->borrow_manager(manager,e)||manager.identity!=id||
      projectile.source_manager378_v99()!=id||!manager.despawn)return required("SAME assigned manager DeSpawn",e);
   return manager.despawn(projectile.base().identity(),type,e);};
  return true;
 }
 bool callback(std::uintptr_t kind,dh2::world::CanonicalProjectileV99& projectile,std::uintptr_t data,std::int32_t& value,std::string& e){
  if(kind!=0x3dae7cu&&kind!=0x3daff4u)return required("selected original projectile callback",e);
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e))return false;
  //Locate the actual userdata through owned stable skill vectors BEFORE
  //dereferencing the source raw pointer. No stale instance can be resurrected.
  std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
  std::int32_t key{};const dh2::world::CanonicalObjectBorrowV1* object{};
  bool next=c.objects->source_ordered_begin_v38(key,object);
  while(next){if(object&&object->as_character){std::uintptr_t character{};
    if(!object->as_character(object->context,character,e))return false;
    if(character){SourceCampaignCharacterBorrowV62 actual;
     if(!borrow_source_campaign_character_v62(c.actual_world,character,actual,e))return false;
     const auto* state=actual.character?skills(*actual.character):nullptr;
     if(state&&contains(*state,reinterpret_cast<const dh2::character::skills::Instance32*>(data))){record=actual.character;break;}}
   }
   next=c.objects->source_ordered_next_v38(key,key,object);
  }
  if(!record)return required("live owned original Skill userdata",e);
  const auto* instance=reinterpret_cast<const dh2::character::skills::Instance32*>(data);
  dh2::character::skills::ProjectileSkillCallbackServicesV112 services;services.owner=record;
  const auto weak=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(record);
  services.current_instance=[weak](const auto& instance,std::string& e){auto record=weak.lock();const auto* state=record?skills(*record):nullptr;
   if(!record||!state||!contains(*state,&instance)||state->owner!=instance.owner)return required("SAME current skill instance",e);e.clear();return true;};
  services.main_script=[weak](std::uintptr_t id,dh2::character::skills::ProjectileSkillScriptLoanV112& out,std::string& e){auto record=weak.lock();
   if(!record||!record->actor||!record->actor->object||record->actor->object->identity!=id)return required("SAME Character3e4 owner",e);
   dh2::character::skills::ProjectileSkillScriptLoanV112 loan;loan.owner=record;
   if(record->player_script_owner_v62){auto& session=record->player_script_owner_v62->session();session.owner().active(loan.script);loan.scope=session.current_skill_callback_scope();}
   else if(record->actor->session){auto& session=*record->actor->session;session.owner().active(loan.script);loan.scope=session.current_skill_callback_scope();}
   out=std::move(loan);e.clear();return true;};
  auto* collision=projectile.source_pointer_v99(0x3cc);if(!collision)return required("actual collision3cc pointer",e);
  return dh2::character::skills::skill_projectile_callback_v112(kind==0x3dae7cu?
   dh2::character::skills::ProjectileSkillCallbackV112::hit:dh2::character::skills::ProjectileSkillCallbackV112::check,
   *instance,projectile.base().identity(),*collision,services,value,e);
 }
 bool borrow(std::uintptr_t id,std::shared_ptr<dh2::world::CanonicalProjectileV99>& out,std::string& e){
  SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return scope(c,w,e)&&native_.borrow(id,out,e)&&out&&out->base().identity()==id;
 }
 bool update(std::uintptr_t id,std::string& e){std::shared_ptr<dh2::world::CanonicalProjectileV99> p;
  dh2::world::ProjectileMethodServicesV112 s;dh2::world::LaserProjectileServicesV112 laser;
  if(!borrow(id,p,e)||!lend(*p,s,laser,e))return false;
  //Native storage loan spans the complete synchronous callback/impact tail.
  //This pins the real canonical allocation, not its source alive/dead state.
  //Original deferred D0/unpublication remains authoritative and can still
  //invalidate a positive source method; no tombstone or fabricated actor is
  //substituted if the original ordering has already destroyed that receiver.
  dh2::world::ProjectileObjectV112 collision_pin;
  const auto pending=p->source_byte_v99(0x3d1);const auto collision=p->source_pointer_v99(0x3cc);
  if(pending&&*pending&&collision&&*collision&&(!s.object||!s.object(*collision,collision_pin,e)))return false;
  return p->is_laser_type()?dh2::world::laser_projectile_update_v112(*p,s,laser,e):dh2::world::projectile_update_v112(*p,s,e);
 }
 bool collision(std::uintptr_t id,std::uintptr_t peer,const float* xy,std::string& e){
  if(!xy)return required("real contact point",e);std::shared_ptr<dh2::world::CanonicalProjectileV99> p;
  dh2::world::ProjectileMethodServicesV112 s;dh2::world::LaserProjectileServicesV112 laser;
  if(!borrow(id,p,e)||!lend(*p,s,laser,e))return false;std::int32_t ignored{};const std::array<float,2> point{xy[0],xy[1]};
  dh2::world::ProjectileObjectV112 peer_pin;
  const auto dead=p->source_byte_v99(0x3d0),pending=p->source_byte_v99(0x3d1);const auto source=p->source_pointer_v99(0x380);
  if(peer&&dead&&!*dead&&pending&&!*pending&&source&&peer!=*source&&(!s.object||!s.object(peer,peer_pin,e)))return false;
  return p->is_laser_type()?dh2::world::laser_projectile_on_collision_v112(*p,peer,point,s,laser,ignored,e):
   dh2::world::projectile_on_collision_v112(*p,peer,point,s,laser,ignored,e);
 }
 bool fire(std::int32_t row,std::uintptr_t source,std::uintptr_t target,std::uintptr_t check,std::uintptr_t hit,
  std::uintptr_t data,bool angle_variant,float angle,bool node,std::uintptr_t& out,std::string& e){
  out=0;SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(c,w,e))return false;
  auto group=arrays_->group("ProjectileTable");if(!group||!group->records_loaded)return required("actual process ProjectileTable",e);
  if(row<0||static_cast<std::uint32_t>(row)>=group->declared_rows){e.clear();return true;}
  dh2::world::ProjectileMethodServicesV112 rows;if(!bind_projectile_process_rows_v112(arrays_,rows,e))return false;
  dh2::world::ProjectileTableRowV112 selected;std::uint8_t laser_type{};
  if(!rows.table_row(row,selected,e)||!selected.byte(0x1c,laser_type,e))return false;
  dh2::loader::ProjectileManagerPrecacheBorrowV96 manager;
  if(!w->projectile_precache_v96||!w->projectile_precache_v96->borrow_manager||
     !w->projectile_precache_v96->borrow_manager(manager,e)||!manager.create)return required("same source manager _Create",e);
  std::uintptr_t created{};if(!manager.create(true,laser_type!=0,created,e))return false;
  if(!created){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();
   if(!assertion||!assertion->source_level())return required("process assertion mode",e);
   const auto mode=*assertion->source_level();if(mode==2){e="Original ProjectileManager.Spawn NULL assertion fault";return false;}
   if(mode==1&&!assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Projectiles\\ProjectileManager.cpp",angle_variant?0x10a:0xf3,"p",e))return false;
   e.clear();return true;
  }
  std::shared_ptr<dh2::world::CanonicalProjectileV99> p;if(!borrow(created,p,e)||!native_.visible(*p,true,e))return false;
  dh2::world::ProjectileMethodServicesV112 s;dh2::world::LaserProjectileServicesV112 laser;if(!lend(*p,s,laser,e))return false;
  bool completed;
  if(p->is_laser_type())completed=angle_variant?dh2::world::laser_projectile_set_info_angle_v112(*p,row,source,target,check,hit,data,angle,s,laser,e):
   dh2::world::laser_projectile_set_info_v112(*p,row,source,target,check,hit,data,node,s,laser,e);
  else completed=angle_variant?dh2::world::projectile_set_info_angle_v112(*p,row,source,target,check,hit,data,angle,s,e):
   dh2::world::projectile_set_info_v112(*p,row,source,target,check,hit,data,node,s,e);
  if(!completed)return false;auto* updating=p->base().byte(0x85);if(!updating)return required("actual source updating85",e);
  *updating=1;out=created;e.clear();return true;
 }
};
namespace {
bool runtime(const SourceCampaignCandidateBorrowV55& candidate,std::shared_ptr<SourceCampaignProjectilesV112>& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world->projectile_runtime_v112)return required("installed active projectile runtime",e);
 out=world->projectile_runtime_v112;return true;
}
}
bool install_source_campaign_projectile_methods_v112(const SourceCampaignCandidateBorrowV55& c,SourceCampaignProjectileMethodsNativeV112 native,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> w;std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;
 if(!borrow_source_campaign_condition_world_v70(c,w,e)||!native.owner||!native.borrow||!native.lend||!native.visible||
    !w->projectile_precache_v96||w->projectile_runtime_v112||!borrow_process_spawn_arrays_v108(arrays,e))return required("once-bound native projectile methods",e);
 w->projectile_runtime_v112=std::make_shared<SourceCampaignProjectilesV112>(w,std::move(native),std::move(arrays));e.clear();return true;
}
bool source_campaign_projectile_update_v112(const SourceCampaignCandidateBorrowV55& c,std::uintptr_t id,std::string& e){std::shared_ptr<SourceCampaignProjectilesV112> p;return runtime(c,p,e)&&p->update(id,e);}
bool source_campaign_projectile_collision_v112(const SourceCampaignCandidateBorrowV55& c,std::uintptr_t id,std::uintptr_t peer,const float* xy,std::string& e){std::shared_ptr<SourceCampaignProjectilesV112> p;return runtime(c,p,e)&&p->collision(id,peer,xy,e);}
bool source_campaign_projectile_fire_v112(const SourceCampaignCandidateBorrowV55& c,std::int32_t row,std::uintptr_t source,std::uintptr_t target,std::uintptr_t check,
 std::uintptr_t hit,std::uintptr_t data,bool angle_variant,float angle,bool node,std::uintptr_t& out,std::string& e){std::shared_ptr<SourceCampaignProjectilesV112> p;
 return runtime(c,p,e)&&p->fire(row,source,target,check,hit,data,angle_variant,angle,node,out,e);}
namespace {
std::int32_t signed_integer(float value){if(std::isnan(value))return 0;
 if(value>=2147483648.f)return INT32_MAX;if(value< -2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(value);}
std::uint32_t unsigned_integer(float value){if(std::isnan(value)||value<=0)return 0;
 if(value>=4294967296.f)return UINT32_MAX;return static_cast<std::uint32_t>(value);}
bool game_position(const SourceCampaignCandidateBorrowV55& c,std::uintptr_t id,std::shared_ptr<void>& pin,const float*& xyz,std::string& e){
 const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
 bool next=c.objects->source_ordered_begin_v38(key,object);
 while(next&&(!object||object->identity!=id))next=c.objects->source_ordered_next_v38(key,key,object);
 if(!next||!object||!object->as_character)return required("same actual GameObject userdata",e);
 std::uintptr_t character{};if(!object->as_character(object->context,character,e))return false;
 if(character){SourceCampaignCharacterBorrowV62 actor;if(!borrow_source_campaign_character_v62(c.actual_world,character,actor,e)||!actor.character->actor)return false;
  pin=actor.character;xyz=actor.character->actor->source_position160_v7();
 }else{std::shared_ptr<SourceWorldBorrowV61> world;dh2::world::CanonicalGameObjectBaseOwnerV1* base{};
  if(!borrow_source_campaign_condition_world_v70(c,world,e)||!world->gameobject_graph_v68||!world->gameobject_graph_v68->borrow_base_v77(id,pin,base,e))return false;
  xyz=base->vector3(0x160);
 }
 if(!pin||!xyz)return required("live source position160 loan",e);return true;
}
struct CharacterProjectileBindingsV112 {
 std::weak_ptr<void> world;std::uintptr_t character{};
 void* previous_context{};
 int(*previous_binding)(void*,std::uint32_t,dh2_script_function*,void**){};
 static int failure(char* out,std::size_t size,const std::string& e){if(out&&size)std::snprintf(out,size,"%s",e.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;}
 bool scope(SourceCampaignCandidateBorrowV55& out,SourceCampaignCharacterBorrowV62& actor,std::string& e){
  const auto actual=world.lock();if(!actual||!borrow_source_campaign_candidate_runtime_v61(out,e)||out.actual_world!=actual||
    !borrow_source_campaign_character_v62(actual,character,actor,e))return required("live Character skill binding scope",e);
  return true;
 }
 static int select(void* opaque,std::uint32_t original,dh2_script_function* function,void** context){
  if(!opaque||!function||!context)return -1;auto& self=*static_cast<CharacterProjectileBindingsV112*>(opaque);
  if(original==0x3b99d0u){*function=&spawn;*context=&self;return 1;}
  if(original==0x3ba298u){*function=&set_target;*context=&self;return 1;}
  return self.previous_binding?self.previous_binding(self.previous_context,original,function,context):0;
 }
 static int spawn(void* opaque,const dh2_script_value* args,std::uint32_t n,dh2_script_value* out,
  std::uint32_t capacity,std::uint32_t* count,char* error,std::size_t size){
  if(!opaque||!count||(n&&!args))return 1;*count=0;
  if(n<=1||args[0].type!=DH2_SCRIPT_NUMBER||args[1].type!=DH2_SCRIPT_NUMBER)return 0;
  auto& self=*static_cast<CharacterProjectileBindingsV112*>(opaque);std::string e;
  try{
   SourceCampaignCandidateBorrowV55 c;SourceCampaignCharacterBorrowV62 actor;if(!self.scope(c,actor,e))return failure(error,size,e);
   std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> arrays;if(!borrow_process_spawn_arrays_v108(arrays,e))return failure(error,size,e);
   auto rows=arrays->group("ProjectileTable");if(!rows||!rows->records_loaded)return failure(error,size,"Required actual source ProjectileTable count");
   const auto row=unsigned_integer(args[1].number);if(row>=rows->declared_rows)return 0;
   const auto* state=skills(*actor.character);const auto index=signed_integer(args[0].number);
   if(!state||index<0||static_cast<std::uint32_t>(index)>=state->skills.count||!state->skills.items)return failure(error,size,"Original Skill vector index is outside live owned storage");
   const auto* instance=state->skills.items[index];
   if(!instance){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();if(!assertion||!assertion->source_level())return failure(error,size,"Required actual Skill instance assertion mode");
    if(*assertion->source_level()==1&&!assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Character.cpp",916,"skill",e))return failure(error,size,e);
    return failure(error,size,"Original NULL Skill instance would dereference in SpawnProjectile");}
   if(instance->owner!=self.character)return failure(error,size,"Foreign Skill owner in actual Character.skills47c");
   const bool angle=n>2&&args[2].type==DH2_SCRIPT_NUMBER;
   const bool from_node=n>2&&args[2].type==DH2_SCRIPT_BOOLEAN&&args[2].boolean;
   std::uintptr_t projectile{};
   if(!source_campaign_projectile_fire_v112(c,static_cast<std::int32_t>(row),self.character,0,
      0x3daff4u,0x3dae7cu,reinterpret_cast<std::uintptr_t>(instance),angle,angle?args[2].number:0,from_node,projectile,e))return failure(error,size,e);
   //Third UserData is the source override AFTER Spawn/SetInfo/85 publication.
   //Fourth UserData modifies only the destination; Z stays the projectile's
   //own current source168 on the third-argument SetPosition path.
   if(n>2&&args[2].type==DH2_SCRIPT_SOURCE_OBJECT&&projectile){
    std::shared_ptr<SourceCampaignProjectilesV112> active;std::shared_ptr<dh2::world::CanonicalProjectileV99> actual;
    if(!runtime(c,active,e)||!active->borrow(projectile,actual,e))return failure(error,size,e);
    std::shared_ptr<void> pin;const float* position{};
    if(!game_position(c,args[2].identity,pin,position,e))return failure(error,size,e);
    auto own=actual->base().vector3(0x160);if(!own)return failure(error,size,"Required actual projectile168 for override");
    const std::array<float,3> override_position{position[0],position[1],own[2]};
    if(!actual->set_position(override_position,true,e))return failure(error,size,e);
    if(n>3&&args[3].type==DH2_SCRIPT_SOURCE_OBJECT){
     if(!game_position(c,args[3].identity,pin,position,e))return failure(error,size,e);
     auto destination=actual->base().vector3(0x1a8);if(!destination)return failure(error,size,"Required same projectile destination1a8");
     std::memcpy(destination,position,12);
    }
   }
   if(!capacity||!out)return failure(error,size,"Projectile native pointer result capacity unavailable");
   out[0]={};out[0].type=DH2_SCRIPT_IDENTITY;out[0].identity=projectile;*count=1;return 0;
  }catch(const std::exception& failure){return CharacterProjectileBindingsV112::failure(error,size,failure.what());}
 }
 static int set_target(void* opaque,const dh2_script_value* args,std::uint32_t n,dh2_script_value*,
  std::uint32_t,std::uint32_t* count,char* error,std::size_t size){
  if(!opaque||!count||(n&&!args))return 1;*count=0;
  if(n<=1||args[0].type!=DH2_SCRIPT_IDENTITY||args[1].type!=DH2_SCRIPT_SOURCE_OBJECT||!args[0].identity)return 0;
  auto& self=*static_cast<CharacterProjectileBindingsV112*>(opaque);std::string e;
  try{SourceCampaignCandidateBorrowV55 c;SourceCampaignCharacterBorrowV62 actor;if(!self.scope(c,actor,e))return failure(error,size,e);
   std::shared_ptr<SourceCampaignProjectilesV112> active;std::shared_ptr<dh2::world::CanonicalProjectileV99> actual;
   if(!runtime(c,active,e)||!active->borrow(args[0].identity,actual,e))return failure(error,size,e);
   if(args[1].identity){std::shared_ptr<void> pin;const float* p{};if(!game_position(c,args[1].identity,pin,p,e))return failure(error,size,e);}
   return dh2::world::projectile_set_target_v113(*actual,args[1].identity,e)?0:failure(error,size,e);
  }catch(const std::exception& failure){return CharacterProjectileBindingsV112::failure(error,size,failure.what());}
 }
};
template<class Input>bool bind(const std::shared_ptr<void>& actual,std::uintptr_t id,Input& input,std::shared_ptr<void>& holder,std::string& e){
 if(!actual||!id||holder){e="Required once-bound same Character projectile script input";return false;}
 auto callback=std::make_shared<CharacterProjectileBindingsV112>();callback->world=actual;callback->character=id;
 callback->previous_context=input.gameplay_context;callback->previous_binding=input.gameplay_binding;
 input.gameplay_context=callback.get();input.gameplay_binding=&CharacterProjectileBindingsV112::select;
 holder=std::move(callback);e.clear();return true;
}
}
bool bind_source_character_projectile_natives_v112(const std::shared_ptr<void>& w,std::uintptr_t id,
 dh2::character::CharacterScriptSessionInput& in,std::shared_ptr<void>& holder,std::string& e){return bind(w,id,in,holder,e);}
bool bind_source_character_projectile_natives_v112(const std::shared_ptr<void>& w,std::uintptr_t id,
 dh2::character::CharacterScriptSessionInputV3& in,std::shared_ptr<void>& holder,std::string& e){return bind(w,id,in,holder,e);}
}
