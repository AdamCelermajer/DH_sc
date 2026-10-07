#include "laser_projectile_methods_v112.hpp"
#include <cmath>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <utility>
#include <type_traits>
namespace dh2::world {
namespace {
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float div(float a,float b){volatile float v=a/b;return v;}
ProjectilePointV112 normalize(ProjectilePointV112 p){const float n=std::sqrt(add(add(mul(p[0],p[0]),mul(p[1],p[1])),mul(p[2],p[2])));for(auto& v:p)v=div(v,n);return p;}
std::int32_t signed_word(std::uint32_t w){std::int32_t v;std::memcpy(&v,&w,4);return v;}
float float_word(std::uint32_t w){float v;std::memcpy(&v,&w,4);return v;}
struct Body {
 CanonicalProjectileV99& p;ProjectileMethodServicesV112& s;LaserProjectileServicesV112& x;std::string& e;std::string first;
 bool fail(const char* why){if(first.empty())first=e.empty()?why:e;e=first;return false;}
 bool live(){if(!first.empty()){e=first;return false;}if(!s.owner||!s.current)return fail("Required current actual Projectile authority");std::string local;
  if(!s.current(p,local)){e=local;return fail("Projectile native scope changed");}return true;}
 template<class F,class... A>bool call(const char* why,const F& f,A&&... a){if(!live())return false;if constexpr(std::is_constructible_v<bool,const F&>){if(!static_cast<bool>(f))return fail(why);}std::string local;bool done=false;
  try{done=f(std::forward<A>(a)...,local);}catch(const std::exception& ex){local=ex.what();}catch(...){local=why;}
  if(!done){e=local;return fail(why);}return live();}
 std::uint32_t& word(unsigned o){auto* c=p.source_word_v99(o);if(!c)throw std::runtime_error("Required SAME Projectile scalar cell");return *c;}
 std::uintptr_t& ptr(unsigned o){auto* c=p.source_pointer_v99(o);if(!c)throw std::runtime_error("Required SAME native-width Projectile pointer cell");return *c;}
 std::uint8_t& byte(unsigned o){auto* c=p.source_byte_v99(o);if(!c)throw std::runtime_error("Required SAME Projectile byte cell");return *c;}
 std::uintptr_t baseptr(unsigned o){auto* c=p.base().pointer(o);if(!c)throw std::runtime_error("Required SAME GameObject resource cell");return *c;}
 ProjectilePointV112 basepoint(unsigned o){auto* c=p.base().vector3(o);if(!c)throw std::runtime_error("Required SAME GameObject vector cells");ProjectilePointV112 v;std::memcpy(v.data(),c,12);return v;}
 bool scalar(unsigned o,float& v){return projectile_read_float_v112(p,o,v,e);}
 bool store(unsigned o,float v){return live()&&projectile_write_float_v112(p,o,v,e);}
 bool pointcell(unsigned o,ProjectilePointV112& v){return projectile_read_point_v112(p,o,v,e);}
 bool object(std::uintptr_t id,ProjectileObjectV112& v){if(!id)return fail("Required nonNULL actual GameObject");if(!call("Required actual GameObject resource loan",s.object,id,v))return false;
  return v.owner&&v.identity==id?true:fail("GameObject resource identity/pin mismatch");}
 bool point(const ProjectileObjectV112& v,ProjectilePointV112& out){ProjectilePointBorrowV112 loan;
  if(!call("Required actual GetTargetPosition3935dc",s.target_position,v,loan))return false;
  if(!loan.owner||!loan.xyz)return fail("Required SAME target position cells");std::memcpy(out.data(),loan.xyz,12);return true;}
 bool point(std::uintptr_t id,ProjectilePointV112& out){ProjectileObjectV112 v;return object(id,v)&&point(v,out);}
 bool node(std::uintptr_t id,ProjectilePointV112& out){ProjectilePointBorrowV112 loan;if(!call("Required actual node absolute position",s.node_absolute_position,id,loan))return false;
  if(!loan.owner||!loan.xyz)return fail("Required SAME node position loan");std::memcpy(out.data(),loan.xyz,12);return true;}
 bool row(ProjectileTableRowV112& out){if(!call("Required SAME ProjectileTable row374",s.table_row,signed_word(word(0x374)),out))return false;return out.owner?true:fail("Required retained ProjectileTable row");}
 bool tw(const ProjectileTableRowV112& row,unsigned off,std::uint32_t& out){return call("Required original ProjectileTable word",row.word,off,out);}
 bool tb(const ProjectileTableRowV112& row,unsigned off,std::uint8_t& out){return call("Required original ProjectileTable byte",row.byte,off,out);}
 bool character(const ProjectileObjectV112& peer,std::uintptr_t& id,std::shared_ptr<void>& pin){if(!call("Required actual GetHandle.Character conversion",x.character_from_handle,peer,id,pin))return false;
  return !id||pin?true:fail("Required actual Character resource pin");}
 //The original captures this table row before peer.GetHandle conversion.
 bool admit(std::uintptr_t id,ProjectileObjectV112& peer,std::uintptr_t& chr,std::shared_ptr<void>& pin,bool& accepted){
  accepted=false;ProjectileTableRowV112 table;if(!row(table)||!object(id,peer)||!character(peer,chr,pin))return false;
  if(chr){std::uint8_t b{};if(!tb(table,4,b))return false;if(b){accepted=ptr(0x384)==id;return true;}
   if(!tb(table,0x10,b))return false;if(b){accepted=true;return true;}
   ProjectileObjectV112 owner;std::uintptr_t ownerchr{};std::shared_ptr<void> ownerpin;
   if(!object(ptr(0x380),owner)||!character(owner,ownerchr,ownerpin))return false;
   if(!ownerchr){accepted=true;return true;}
   return call("Required actual Character.AI_IsEnemy",x.character_ai_is_enemy,ownerchr,peer,accepted);
  }accepted=ptr(0x384)==0;return true;
 }
 bool expire(int kind){if(!live())return false;if(!projectile_on_expire_v112(p,kind,s,e))return fail("Actual Projectile OnExpire failed");return live();}
 bool invalid(unsigned line){if(!call("Required actual source assertion policy",s.invalid_argument,line))return false;e="Original invalid Laser parameter continues outside portable safe domain";return fail("Invalid Laser parameter");}
 bool physical(){if(baseptr(0x2dc))return true;auto prefix=std::make_shared<ProjectilePhysicalPrefixV112>();
  if(!call("Required retained native PhysicalObject prefix",s.retain_physical_prefix,prefix))return false;
  if(!s.construct_physical)return fail("Required actual PhysicalObject C2");
  auto construct=[&](CanonicalProjectileV99& actual,ProjectilePhysicalPrefixV112& r,std::string& err){bool done=s.construct_physical(actual,r,err);if(done)r.constructed=true;return done;};
  if(!call("Required actual PhysicalObject C2",construct,p,*prefix))return false;
  if(!prefix->receiver||!prefix->identity)return fail("Required actual constructed PhysicalObject receiver");
  if(!s.set_physical)return fail("Required actual SetPhysical");
  auto assign=[&](CanonicalProjectileV99& actual,ProjectilePhysicalPrefixV112& r,bool immediate,std::string& err){bool done=s.set_physical(actual,r,immediate,err);if(done)r.assigned=true;return done;};
  if(!call("Required actual SetPhysical(false)",assign,p,*prefix,false))return false;
  return call("Required assigned physical prefix retirement",s.retire_assigned_physical_prefix,prefix);
 }
 bool laser(){return p.is_laser_type()?true:fail("Laser method requires SAME LaserTypeProjectile10 receiver");}
};
template<class F>bool run(Body& b,F f){try{if(!b.live()||!f())return false;b.e.clear();return true;}catch(const std::exception& ex){b.e=ex.what();return b.fail("Laser/collision native delivery threw");}catch(...){return b.fail("Laser/collision native delivery threw");}}
}
bool projectile_on_collision_v112(CanonicalProjectileV99& p,std::uintptr_t peer,const std::array<float,2>& collision,
 ProjectileMethodServicesV112& s,LaserProjectileServicesV112& x,std::int32_t& result,std::string& e){
 e.clear();result=0;Body b{p,s,x,e,{}};return run(b,[&]{
  if(b.byte(0x3d0)||b.byte(0x3d1)||!peer||peer==b.ptr(0x380))return true;
  ProjectileObjectV112 other;std::uintptr_t chr{};std::shared_ptr<void> pin;bool admitted{};
  if(!b.admit(peer,other,chr,pin,admitted))return false;if(!admitted)return true;
  b.ptr(0x3cc)=peer;std::memcpy(&b.word(0x3c4),collision.data(),4);std::memcpy(&b.word(0x3c8),collision.data()+1,4);
  std::int32_t check=0;if(b.ptr(0x3b8)&&!b.call("Required actual collision check callback",s.callback,b.ptr(0x3b8),p,b.ptr(0x3c0),check))return false;
  if(check==2){std::int32_t ignored{};if(b.ptr(0x3bc)&&!b.call("Required actual collision hit callback",s.callback,b.ptr(0x3bc),p,b.ptr(0x3c0),ignored))return false;
   std::array<float,2> fresh;std::memcpy(fresh.data(),&b.word(0x3c4),4);std::memcpy(fresh.data()+1,&b.word(0x3c8),4);
   return b.call("Required actual HandleImpactFX(object,Point2)",s.impact_object,p,b.ptr(0x3cc),fresh);}
  if(check==3){if(!b.expire(1))return false;b.byte(0x3d1)=1;result=1;return true;}
  if(check==1)return true;b.byte(0x3d1)=1;result=1;return true;
 });
}
bool laser_projectile_on_collision_v112(CanonicalProjectileV99& p,std::uintptr_t peer,const std::array<float,2>& collision,
 ProjectileMethodServicesV112& s,LaserProjectileServicesV112& x,std::int32_t& result,std::string& e){
 e.clear();result=0;Body b{p,s,x,e,{}};return run(b,[&]{if(!b.laser())return false;
  if(b.byte(0x3d0)||b.byte(0x3d1)||!peer||peer==b.ptr(0x380))return true;
  ProjectileObjectV112 other;std::uintptr_t chr{};std::shared_ptr<void> pin;bool admitted{};
  if(!b.admit(peer,other,chr,pin,admitted))return false;if(!admitted)return true;b.ptr(0x384)=peer;
  ProjectileTableRowV112 table;std::uint32_t uid{};if(!b.row(table)||!b.tw(table,0x14,uid))return false;ProjectilePointV112 point;
  if(chr){ProjectilePointBorrowV112 loan;if(!b.call("Required actual peer position160",x.object_position160,other,loan))return false;
   if(!loan.owner||!loan.xyz)return b.fail("Required SAME peer position160 resource loan");std::memcpy(point.data(),loan.xyz,12);}
  else {ProjectilePointV112 target;if(!b.point(other,target))return false;point={collision[0],collision[1],target[2]};}
  if(!b.call("Required actual VisualFXManager.PlayAnimFXSet",x.play_anim_fx_set,signed_word(uid),point,std::uintptr_t{0},std::uintptr_t{0}))return false;
  b.byte(0x3d1)=1;result=1;return true;
 });
}
bool laser_projectile_set_info_v112(CanonicalProjectileV99& p,std::int32_t index,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check,std::uintptr_t hit,std::uintptr_t userdata,bool from_node,ProjectileMethodServicesV112& s,LaserProjectileServicesV112& x,std::string& e){
 (void)check;e.clear();Body b{p,s,x,e,{}};return run(b,[&]{if(!b.laser())return false;std::uint32_t count{};
  if(index<0)return b.invalid(0x3f);if(!b.call("Required actual ProjectileTable count",s.table_count,count))return false;
  if(static_cast<std::uint32_t>(index)>=count)return b.invalid(0x3f);if(!source)return b.invalid(0x40);
  ProjectileTableRowV112 table;if(!b.call("Required actual selected ProjectileTable row",s.table_row,index,table)||!table.owner)return b.fail("Required retained selected ProjectileTable row");
  std::memcpy(&b.word(0x374),&index,4);b.ptr(0x384)=target;b.ptr(0x3bc)=hit;b.byte(0x3d1)=0;b.ptr(0x3dc)=source;
  b.ptr(0x3c0)=userdata;b.ptr(0x380)=0;b.byte(0x3d0)=0;
  ProjectileObjectV112 actual;ProjectilePointV112 look{0,0,0},origin;
  if(!b.object(source,actual)||!b.call("Required actual source GetLookAtVec",s.look_at,actual,look)||!b.point(b.ptr(0x3dc),origin))return false;
  if(!projectile_write_point_v112(p,0x388,origin,e)||!b.store(0x3a0,origin[2]))return false;
  std::uint32_t bits{};if(!b.tw(table,0x20,bits))return false;const float range=float_word(bits);if(!b.store(0x3a8,range>=0?mul(range,range):-1.f))return false;
  if(!b.object(b.ptr(0x3dc),actual))return false;if(!actual.visual2d8)return b.fail("Required SAME source visual2d8 cell");b.ptr(0x3a4)=0;
  if(*actual.visual2d8){std::uintptr_t node{};if(!b.call("Required actual projectile_node lookup",s.node_from_name,*actual.visual2d8,"projectile_node",node))return false;b.ptr(0x3a4)=node;
   if(node){if(!b.node(node,origin)||!projectile_write_point_v112(p,0x388,origin,e)||!b.store(0x3a0,origin[2]))return false;}}
  for(auto pair:{std::pair<unsigned,unsigned>{0x40,0x3ac},{0x44,0x3b0},{0x3c,0x3b4}}){if(!b.tw(table,pair.first,bits))return false;b.word(pair.second)=bits;}
  b.word(0x3e0)=0xffffffffu;if(!b.tw(table,0x28,bits))return false;
  if(signed_word(bits)>=0){if(!b.call("Required actual SetVisualObject from model row",s.set_visual_index,p,signed_word(bits)))return false;}
  else if(!b.call("Required actual SetVisualObject(NULL)",s.set_visual_null,p))return false;
  const auto visual=b.baseptr(0x2d8);if(visual){std::uintptr_t node{};
   if(!b.call("Required actual _bone_origin lookup",s.node_from_name,visual,"_bone_origin",node))return false;b.ptr(0x3d4)=node;
   if(!b.call("Required actual _bone_target lookup",s.node_from_name,visual,"_bone_target",node))return false;b.ptr(0x3d8)=node;}
  else {b.ptr(0x3d4)=0;b.ptr(0x3d8)=0;}
  if(!b.physical())return false;const ProjectilePointV112 zero{0,0,0};if(!b.call("Required actual Laser SetPosition zero",s.set_position,p,zero,true))return false;
  if(!b.pointcell(0x388,origin)||!b.scalar(0x3a0,origin[2]))return false;
  if(!b.ptr(0x3d4)||!b.call("Required actual _bone_origin SetPosition",x.node_set_position,b.ptr(0x3d4),origin))return b.fail("Required nonNULL _bone_origin node");
  if(!b.ptr(0x3d8)||!b.call("Required actual _bone_target SetPosition",x.node_set_position,b.ptr(0x3d8),origin))return b.fail("Required nonNULL _bone_target node");
  if(b.baseptr(0x2d8)){
   if(!b.call("Required actual Visual.SetVisible(true)",x.visual_set_visible,b.baseptr(0x2d8),true))return false;
   if(!b.call("Required actual Visual.SetParent(NULL)",x.visual_set_parent,b.baseptr(0x2d8),std::uintptr_t{0}))return false;
   if(!b.call("Required actual animator virtual1c",s.animator_virtual1c,b.baseptr(0x2d8),0u,1u,0u,0u,0u))return false;
   const auto ownvisual=b.baseptr(0x2d8);std::uintptr_t root{};std::shared_ptr<void> pin;
   if(!b.call("Required actual Visual root8 loan",x.visual_root8,ownvisual,root,pin))return false;
   if(root){if(!pin)return b.fail("Required SAME root8 resource pin");if(!b.call("Required actual Visual.SetPosition zero",x.visual_set_position,ownvisual,zero)||!b.call("Required actual root virtual14(0)",x.root_virtual14,root,0u))return false;}}
  if(!b.pointcell(0x388,origin)||!b.call("Required actual Laser SetPosition origin",s.set_position,p,origin,true))return false;
  ProjectilePointV112 destination;
  if(b.ptr(0x384)){if(!b.point(b.ptr(0x384),destination))return false;}
  else {if(b.ptr(0x3a4)&&from_node){ProjectilePointV112 node;if(!b.node(b.ptr(0x3a4),node)||!b.pointcell(0x388,origin))return false;for(unsigned i=0;i<3;++i)look[i]=sub(node[i],origin[i]);look=normalize(look);}
   if(!b.pointcell(0x388,origin))return false;for(unsigned i=0;i<3;++i)destination[i]=add(origin[i],mul(look[i],1000.f));}
  if(!b.call("Required actual Laser SetDestination",s.set_destination,p,destination))return false;
  const auto& flying=x.set_flying?x.set_flying:s.pf_flying;const auto& swimming=x.set_swimming?x.set_swimming:s.pf_swimming;
  return b.call("Required actual PFObject.SetFlying(true)",flying,p,true)&&b.call("Required actual PFObject.SetSwimming(true)",swimming,p,true);
 });
}
bool laser_projectile_set_info_angle_v112(CanonicalProjectileV99& p,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check,std::uintptr_t hit,std::uintptr_t userdata,float ignored,ProjectileMethodServicesV112& s,LaserProjectileServicesV112& x,std::string& e){
 (void)ignored;e.clear();Body b{p,s,x,e,{}};return run(b,[&]{return b.laser()&&b.call("Required original unsupported Laser float overload assertion",s.invalid_argument,0xb2u)&&
  b.call("Required actual virtualc8 SetInfo(false)",s.virtual_set_info_bool,p,row,source,target,check,hit,userdata,false);});
}
bool laser_projectile_update_v112(CanonicalProjectileV99& p,ProjectileMethodServicesV112& s,LaserProjectileServicesV112& x,std::string& e){
 e.clear();Body b{p,s,x,e,{}};return run(b,[&]{if(!b.laser())return false;
  if(signed_word(b.word(0x3e0))>0){ProjectileApplicationV112 app;std::uint32_t dt{};
   if(!b.call("Required actual App cooldown loan",s.application,app)||!app.owner)return b.fail("Required SAME App resource pin");
   const auto previous=b.word(0x3e0);if(!b.call("Required actual App.GetDt cooldown",app.get_dt,dt))return false;b.word(0x3e0)=previous-dt;}
  if(b.byte(0x3d1)){
   if(b.ptr(0x3bc)&&signed_word(b.word(0x3e0))<0){std::int32_t ignored{};
    if(!b.call("Required actual Laser hit callback",s.callback,b.ptr(0x3bc),p,b.ptr(0x3c0),ignored))return false;
    ProjectileTableRowV112 table;std::uint32_t cooldown{};if(!b.row(table)||!b.tw(table,0x38,cooldown))return false;b.word(0x3e0)=cooldown;}
   ProjectileTableRowV112 table;std::uint8_t repeat{};if(!b.row(table)||!b.tb(table,5,repeat))return false;
   if(!repeat&&signed_word(b.word(0x3b4))>0){b.byte(0x3d1)=0;return true;}
   b.byte(0x3d0)=1;b.byte(0x3d1)=0;
   if(!b.call("Required actual ProjectileManager.Despawn(laser=true)",s.despawn,p,p.source_manager378_v99(),true))return false;
   if(!b.baseptr(0x2d8))return b.fail("Original Laser despawn requires nonNULL visual");return b.call("Required actual Laser Visual.SetVisible(false)",x.visual_set_visible,b.baseptr(0x2d8),false);
  }
  if(b.ptr(0x384)){ProjectileObjectV112 target;std::int32_t dead{};if(!b.object(b.ptr(0x384),target)||!b.call("Required actual target virtual34",s.object_virtual34,target,dead))return false;if(dead)b.ptr(0x384)=0;}
  if(b.byte(0x3d0))return true;ProjectileTableRowV112 table;std::uint8_t follow{};if(!b.row(table)||!b.tb(table,0x35,follow))return false;
  ProjectilePointV112 destination;if(follow&&b.ptr(0x384)){if(!b.point(b.ptr(0x384),destination))return false;}
  else {const auto old=b.basepoint(0x1a8);ProjectilePointV112 current;if(!b.point(p.base().identity(),current))return false;ProjectilePointV112 direction;
   for(unsigned i=0;i<3;++i)direction[i]=sub(old[i],current[i]);direction=normalize(direction);
   if(!b.point(p.base().identity(),current))return false;for(unsigned i=0;i<3;++i)destination[i]=add(current[i],mul(direction[i],1000.f));}
  if(!b.call("Required actual Laser SetDestination",s.set_destination,p,destination))return false;
  ProjectilePointV112 point;if(!b.point(b.ptr(0x3dc),point)||!b.ptr(0x3d4)||!b.call("Required actual _bone_origin SetPosition",x.node_set_position,b.ptr(0x3d4),point))return b.fail("Required nonNULL Laser source node");
  if(!b.point(p.base().identity(),point)||!b.ptr(0x3d8)||!b.call("Required actual _bone_target SetPosition",x.node_set_position,b.ptr(0x3d8),point))return b.fail("Required nonNULL Laser target node");
  if(!b.call("Required qualified SAME GameObject.Update38cbe8",s.gameobject_update,p))return false;
  ProjectileApplicationV112 app;if(!b.call("Required cached actual App loan",s.application,app)||!app.owner)return b.fail("Required SAME App resource pin");
  float speed{},deceleration{};if(!b.scalar(0x3ac,speed)||!b.scalar(0x3b0,deceleration))return false;std::uint32_t dt{};
  if(!b.call("Required actual App.GetDt speed",app.get_dt,dt)||!b.store(0x3ac,add(speed,div(mul(deceleration,static_cast<float>(dt)),-1000.f))))return false;
  const auto lifetime=b.word(0x3b4);if(!b.call("Required actual App.GetDt lifetime",app.get_dt,dt))return false;b.word(0x3b4)=lifetime-dt;
  if(!b.scalar(0x3ac,speed))return false;if(signed_word(b.word(0x3b4))<=0||speed<=0){if(!b.expire(1))return false;b.word(0x3b4)=0xffffffffu;return true;}
  float range{};if(!b.scalar(0x3a8,range))return false;if(range>=0){ProjectilePointV112 position,origin;
   if(!b.point(p.base().identity(),position)||!b.pointcell(0x388,origin))return false;for(unsigned i=0;i<3;++i)position[i]=sub(position[i],origin[i]);
   const float distance=add(add(mul(position[0],position[0]),mul(position[1],position[1])),mul(position[2],position[2]));
   if(range<=distance){if(!b.expire(1))return false;b.word(0x3b4)=0xffffffffu;return true;}}
  const auto position=b.basepoint(0x160);bool found{};float height{};ProjectileFloorV112 floor;
  if(!b.call("Required actual PFWorld.GetFloorHeightAt(true)",s.world_floor_height,p,position,true,found,height,floor))return false;
  int kind=0;if(found&&floor.identity){if(!floor.owner||!floor.flags24)return b.fail("Required SAME positive floor fields/resource pin");bool allowed{};
   if(!b.call("Required actual PFObject.CanPathOn",s.can_path_on,p,floor,allowed))return false;
   if(allowed||(*floor.flags24&0x02000000u)){if(!b.ptr(0x3a4))return true;float initial{};if(!b.scalar(0x3a0,initial))return false;if(!(initial<=height))return true;}kind=2;
  }else {ProjectileRoomV112 room;if(!b.call("Required actual PFWorld.GetRoomAt",s.world_room_at,p,position,room))return false;
   if(room.identity){if(!room.owner||!room.flags24)return b.fail("Required SAME positive room fields/resource pin");if(!(*room.flags24&1u))return true;kind=2;}}
  if(!b.expire(kind))return false;b.word(0x3b4)=0xffffffffu;return true;
 });
}
bool laser_projectile_get_speed_v112(CanonicalProjectileV99& p,float& speed,std::string& e){e.clear();if(!p.is_laser_type()){e="Laser GetSpeed requires SAME LaserTypeProjectile10";return false;}return projectile_read_float_v112(p,0x3ac,speed,e);}
}