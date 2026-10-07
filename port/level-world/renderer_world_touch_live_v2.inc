// DACT development input adapter. Include at namespace scope after the headers
// listed in world-touch-target-v1/live-v2-integration.md. Every pointer is a
// synchronous borrow from the existing live world; this owns only ctor fields.
namespace dh2::world {
struct RendererWorldTouchLiveV2 {
 WorldClickFieldsOwnerV1 pending;
 struct Borrow {
  character::skills::CharacterWorldRuntimeV1* world{};
  floors::World* floors{};
  character::State* machine{};
  character::TargetState48* target{};
  character::TargetServices16 target_services{};
  const float* submitted_camera{};const int* width{};const int* height{};
  const std::uint32_t* blocked{};const std::uint32_t* forced{};
  const ui::OwnedHudSettingsV1* settings{};
  const data::DesignSettingsProjection176* design{};
  std::function<bool(std::uint32_t,bool&,std::string&)> debug;
  // When the live source camera is connected, use its unmodified engine
  // frustum ray. GPU-only depth/orientation conversion is not a picking input.
  std::function<bool(std::int32_t,std::int32_t,std::array<float,3>&,std::array<float,3>&,std::string&)> source_screen_ray_v20;
 } same;
 bool dispatch(float x,float y,bool released,bool& floor_hit,std::string& error){
  floor_hit=false;
  if(!same.world||!same.floors||!same.machine||!same.target||!same.design||!same.blocked||!same.forced){error="World touch requires same World/Floor/State/Target/Design/controller owners";return false;}
  auto& world=*same.world;auto& floor_world=*same.floors;
  WorldTouchProjectionServicesV1 projection;
  projection.screen_ray=[&](std::int32_t px,std::int32_t py,std::array<float,3>& start,std::array<float,3>& end,std::string& e){
   if(same.source_screen_ray_v20)return same.source_screen_ray_v20(px,py,start,end,e);
   if(!same.submitted_camera||!same.width||!same.height||*same.width<=0||*same.height<=0){e="World touch requires actual submitted camera/viewport";return false;}
   selector::Matrix inverse{};std::copy(same.submitted_camera,same.submitted_camera+16,inverse.values);
   if(dh2_selector_inverse(&inverse)!=1){e="World touch submitted camera is singular";return false;}
   // Explicit modern renderer inverse-view-projection ray producer. Source
   // selector/PF traversal below consumes this actual camera ray unchanged.
   const float nx=float(px)*2.f/float(*same.width)-1.f,ny=1.f-float(py)*2.f/float(*same.height);
   auto unproject=[&](float z,std::array<float,3>& out){float clip[4]{nx,ny,z,1},p[4]{};
    for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c)p[r]+=inverse.values[c*4+r]*clip[c];
    if(!std::isfinite(p[3])||p[3]==0)return false;
    for(unsigned i=0;i<3;++i){out[i]=p[i]/p[3];if(!std::isfinite(out[i]))return false;}return true;};
   if(!unproject(-1,start)||!unproject(1,end)){e="World touch camera ray has invalid homogeneous coordinates";return false;}return true;
  };
  projection.room_at=[&](std::uint32_t i,bool& present,std::uintptr_t& id,std::string&){present=i<floor_world.collision_room_floors.size();id=present?std::uintptr_t(i)+1:0;return true;};
  projection.floor_at=[&](std::uintptr_t room,std::uint32_t i,bool& present,WorldTouchFloorBorrowV1& out,std::string& e){
   if(!room||room>floor_world.collision_room_floors.size()){e="World touch floor room expired";return false;}
   const auto& ids=floor_world.collision_room_floors[room-1];present=i<ids.size();out={};if(!present)return true;
   const auto index=ids[i];if(index>=floor_world.records.size()){e="World touch floor index invalid";return false;}
   out={std::uintptr_t(index)+1,&floor_world.records[index]->flags.floor};return true;
  };
  projection.floor_collision=[&](std::uintptr_t id,const std::array<float,3>& start,const std::array<float,3>& end,bool& hit,std::array<float,3>& point,std::string& e){
   if(!id||id>floor_world.selectors.size()){e="World touch requires retained floor selector";return false;}
   auto& floor=floor_world.selectors[id-1];if(!floor.workspace){e="World touch floor selector workspace unavailable";return false;}
   collision::Ray ray{};std::copy(start.begin(),start.end(),ray.start);std::copy(end.begin(),end.end(),ray.end);collision::Result result{};
   const auto code=dh2_selector_raycast(&result,&floor.selector,floor.workspace,&ray);if(code<0){e="World touch actual floor selector raycast failed";return false;}hit=code==1;if(hit)std::copy(result.point,result.point+3,point.begin());return true;
  };
  std::array<float,3> point{};if(!world_touch_projection_v1(x,y,projection,floor_hit,point,error))return false;if(!floor_hit)return true;
  // Source caller projects first; Cmd_Click then permits forced input before
  // checking the SAME Application suspension and controller locked fields.
  if(!*same.forced&&(*same.blocked||same.machine->controller_locked))return true;
  WorldClickServicesV1 click;
  click.ctrl_allowed=[&](bool& v,std::string&){v=!(same.machine->flags&4)&&!(same.machine->flags&2);return true;};
  click.is_casting=[&](bool& v,std::string&){v=same.machine->current==7;return true;};
  click.is_using_skill=[&](bool& v,std::string&){v=same.machine->current==6;return true;};
  click.application_mode320e74=[&](bool& v,std::string& e){return world_click_using_dpad_v1(same.settings,v,e);};
  click.characters=[&](bool first,std::uintptr_t& cursor,std::uintptr_t& id,std::string& e){
   const auto* room=world.registry().rooms;if(!room||!room->next||!room->next->objects){e="World touch Character registration list unavailable";return false;}
   const auto* sentinel=room->next->objects;const auto* entry=first?sentinel->next:reinterpret_cast<const target_search::Entry16*>(cursor)->next;
   if(!entry){e="World touch Character registration list broken";return false;}cursor=reinterpret_cast<std::uintptr_t>(entry);id=entry==sentinel?0:entry->object?entry->object->identity:0;return true;
  };
  click.target_position=[&](std::uintptr_t id,std::array<float,3>& p,std::string& e){character::skills::WorldTargetActorBorrowV1 a{};
   if(world.actor(id,&a)||!a.position||!a.target_node||(*a.target_node&&!a.target_enabled)){e="World touch target position producer unavailable";return false;}
   const auto* src=character::skills::dh2_world_target_position_v1(a.position,a.cached_target_position,*a.target_node,*a.target_node?*a.target_enabled:0);
   if(!src){e="World touch source target cache unavailable";return false;}std::copy(src,src+3,p.begin());return true;};
  click.interactive=[&](std::uintptr_t id,std::uintptr_t owner,bool& v,std::string& e){auto service=world.targets().search_services();target_search::Request24 q{target_search::is_interactive,0,id,owner};target_search::Response16 r{};
   if(service.invoke(service.context,&q,&r)){e=world.targets().error();return false;}v=r.word!=0;return true;};
  click.neutral=[&](std::uintptr_t a,std::uintptr_t b,bool& v,std::string& e){std::uintptr_t result{};if(world.neutral(a,b,&result)){e=world.error();return false;}v=result!=0;return true;};
  click.enemy=[&](std::uintptr_t a,std::uintptr_t b,bool& v,std::string& e){std::uintptr_t result{};if(world.relationship(a,b,true,&result)){e=world.error();return false;}v=result!=0;return true;};
  click.nearby=[&](std::uintptr_t id,const std::array<float,3>& p,float radius,bool& v,std::string& e){character::skills::WorldTargetActorBorrowV1 a{};if(world.actor(id,&a)||!a.aabb6){e="World touch requires actual absolute actor AABB";return false;}v=world_click_nearby_v1(a.aabb6,p.data(),radius);return true;};
  // Preserve row bits; these floats are immutable per synchronous dispatch.
  float enemy{},friendly{},generic{};std::memcpy(&enemy,&same.design->words[0x2c/4],4);std::memcpy(&friendly,&same.design->words[0x50/4],4);std::memcpy(&generic,&same.design->words[0x54/4],4);
  click.enemy_radius2c=&enemy;click.friend_radius50=&friendly;click.object_radius54=&generic;click.debug=same.debug;
  // Current DACT adoption registers Characters only. Do not invent a generic
  // ObjectManager tree or successful click-to-move producer when reached.
  click.objects=[](bool,std::uintptr_t&,std::uintptr_t&,std::string& e){e="Required canonical generic ObjectManager click tree (DACT development adapter)";return false;};
  click.move=[](const std::array<float,3>&,bool,std::string& e){e="Required source Ctrl_Click movement continuation";return false;};
  auto fields=pending.borrow(same.target->owner?same.target->owner->identity:0,*same.target,same.target_services);
  return world_click_target_v1(fields,point,released,click,error);
 }
};
}
