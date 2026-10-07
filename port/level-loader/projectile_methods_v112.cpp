#include "projectile_methods_v112.hpp"
#include <cmath>
#include <cstring>
#include <exception>
#include <utility>
#include <type_traits>
namespace dh2::world {
bool projectile_read_float_v112(CanonicalProjectileV99& p,unsigned offset,float& value,std::string& e){auto* cell=p.source_word_v99(offset);if(!cell){e="Required SAME Projectile float-bit cell";return false;}std::memcpy(&value,cell,4);return true;}
bool projectile_write_float_v112(CanonicalProjectileV99& p,unsigned offset,float value,std::string& e){auto* cell=p.source_word_v99(offset);if(!cell){e="Required SAME Projectile float-bit cell";return false;}std::memcpy(cell,&value,4);return true;}
bool projectile_read_point_v112(CanonicalProjectileV99& p,unsigned offset,ProjectilePointV112& value,std::string& e){for(unsigned i=0;i<3;++i)if(!projectile_read_float_v112(p,offset+4*i,value[i],e))return false;return true;}
bool projectile_write_point_v112(CanonicalProjectileV99& p,unsigned offset,const ProjectilePointV112& value,std::string& e){for(unsigned i=0;i<3;++i)if(!projectile_write_float_v112(p,offset+4*i,value[i],e))return false;return true;}
namespace {
//Force each source soft-float operation to round separately, without FMA.
//Native normalize divides by sqrtf even for zero: no invented zero-vector guard.
float add(float a,float b){volatile float x=a+b;return x;}
float sub(float a,float b){volatile float x=a-b;return x;}
float mul(float a,float b){volatile float x=a*b;return x;}
float div(float a,float b){volatile float x=a/b;return x;}
ProjectilePointV112 normalized(ProjectilePointV112 p){const float n=std::sqrt(add(add(mul(p[0],p[0]),mul(p[1],p[1])),mul(p[2],p[2])));for(auto& x:p)x=div(x,n);return p;}
struct Body {
 CanonicalProjectileV99& p;ProjectileMethodServicesV112& s;std::string& e;std::string first;
 bool fail(const char* missing){if(first.empty())first=e.empty()?missing:e;e=first;return false;}
 bool live(){if(!first.empty()){e=first;return false;}if(!s.owner||!s.current)return fail("Required current actual Projectile native authority");
  std::string local;const bool valid=s.current(p,local);if(!valid){e=local;return fail("Projectile native scope changed");}return true;}
 template<class F,class... A>bool call(const char* name,const F& f,A&&... args){
  if(!live())return false;if constexpr(std::is_constructible_v<bool,const F&>){if(!static_cast<bool>(f))return fail(name);}std::string local;bool delivered=false;
  try{delivered=f(std::forward<A>(args)...,local);}catch(const std::exception& ex){local=ex.what();}catch(...){local=name;}
  if(!delivered){e=local;return fail(name);}return live();
 }
 bool object(std::uintptr_t id,ProjectileObjectV112& out){if(!call("Required SAME source GameObject loan",s.object,id,out))return false;
  if(!out.owner||out.identity!=id)return fail("Projectile GameObject identity/pin mismatch");return true;}
 bool point(const ProjectileObjectV112& actual,ProjectilePointV112& out){ProjectilePointBorrowV112 loan;
  if(!call("Required actual GetTargetPosition3935dc",s.target_position,actual,loan))return false;
  if(!loan.owner||!loan.xyz)return fail("Required SAME live target-position cells");std::memcpy(out.data(),loan.xyz,12);return true;}
 bool point(std::uintptr_t id,ProjectilePointV112& out){ProjectileObjectV112 actual;return object(id,actual)&&point(actual,out);}
 bool row(ProjectileTableRowV112& out){std::int32_t index;auto* cell=p.source_word_v99(0x374);std::memcpy(&index,cell,4);
  if(!call("Required current SAME ProjectileTable row374",s.table_row,index,out))return false;
  return out.owner?true:fail("Required retained actual ProjectileTable row");}
 bool word(const ProjectileTableRowV112& row,unsigned off,std::uint32_t& v){return call("Required original ProjectileTable word",row.word,off,v);}
 bool byte(const ProjectileTableRowV112& row,unsigned off,std::uint8_t& v){return call("Required original ProjectileTable byte",row.byte,off,v);}
 bool scalar(unsigned off,float& v){return projectile_read_float_v112(p,off,v,e);}
 bool store(unsigned off,float v){return live()&&projectile_write_float_v112(p,off,v,e);}
 bool node(std::uintptr_t id,ProjectilePointV112& out){ProjectilePointBorrowV112 point;if(!call("Required SAME node.getAbsolutePosition",s.node_absolute_position,id,point))return false;
  if(!point.owner||!point.xyz)return fail("Required actual node position lease");std::memcpy(out.data(),point.xyz,12);return true;}
 bool expire(std::int32_t kind){
  ProjectileTableRowV112 table;if(!row(table))return false;std::uint8_t snap{};if(!byte(table,0x34,snap))return false;
  if(snap){ProjectileRoomV112 room;ProjectileFloorV112 floor;
   if(!call("Required SAME PFObject room/floor loan",s.pf_room_floor,p,room,floor))return false;
   ProjectilePointV112 previous;if(!projectile_read_point_v112(p,0x394,previous,e))return false;
   float height=0; //literal local zero before the original queries; query result bool is ignored.
   if(floor.identity){if(!floor.owner)return fail("Required current PFObject floor pin");if(!call("Required actual PFFloor.GetFloorHeightAt",s.floor_height,floor,previous,height))return false;}
   else if(room.identity){if(!room.owner)return fail("Required current PFObject room pin");if(!call("Required actual PFRoom.GetFloorHeightAt",s.room_floor_height,room,previous,height,floor))return false;}
   else {bool ignored{};if(!call("Required actual PFWorld.GetFloorHeightAt(false)",s.world_floor_height,p,previous,false,ignored,height,floor))return false;}
   //Native rereads394/398 after floor callbacks, preserving their live changes.
   if(!scalar(0x394,previous[0])||!scalar(0x398,previous[1]))return false;previous[2]=height;
   if(!call("Required actual projectile SetPosition",s.set_position,p,previous,true))return false;
  }
  if(!live())return false;*p.source_pointer_v99(0x3cc)=0;*p.source_byte_v99(0x3d1)=1;
  ProjectilePointV112 target;if(!point(p.base().identity(),target))return false;
  return call("Required actual HandleImpactFX(kind,Point3)",s.impact_kind,p,kind,target);
 }
};
bool invalid(Body& b,unsigned line){if(!b.call("Required actual Projectile assertion diagnostic",b.s.invalid_argument,line))return false;
 b.e="Original invalid Projectile parameter continues outside portable safe native domain";return b.fail("Invalid original Projectile parameter");}
}
bool projectile_on_expire_v112(CanonicalProjectileV99& p,std::int32_t kind,ProjectileMethodServicesV112& s,std::string& e){
 e.clear();Body b{p,s,e,{}};try{if(!b.live()||!b.expire(kind))return false;e.clear();return true;}catch(const std::exception& ex){e=ex.what();return b.fail("Projectile OnExpire threw");}catch(...){return b.fail("Projectile OnExpire threw");}
}
bool projectile_set_info_v112(CanonicalProjectileV99& p,std::int32_t index,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check,std::uintptr_t hit,std::uintptr_t userdata,bool from_node,ProjectileMethodServicesV112& s,std::string& e){
 e.clear();Body b{p,s,e,{}};try{
  if(!b.live())return false;std::uint32_t count{};
  if(index<0)return invalid(b,0x4e);
  if(!b.call("Required actual ProjectileTable count",s.table_count,count))return false;
  if(static_cast<std::uint32_t>(index)>=count)return invalid(b,0x4e);
  if(!source)return invalid(b,0x4f);
  ProjectileTableRowV112 row;if(!b.call("Required actual selected ProjectileTable row",s.table_row,index,row)||!row.owner)return b.fail("Required retained actual selected row");
  if(!b.live())return false;std::memcpy(p.source_word_v99(0x374),&index,4);
  std::uint8_t flag{};if(!b.byte(row,0x1d,flag))return false;
  *p.source_pointer_v99(0x384)=target;*p.source_byte_v99(0x37c)=flag;*p.source_pointer_v99(0x3b8)=check;
  *p.source_pointer_v99(0x3bc)=hit;*p.source_pointer_v99(0x380)=source;*p.source_byte_v99(0x3d0)=0;
  *p.source_pointer_v99(0x3c0)=userdata;*p.source_byte_v99(0x3d1)=0;
  ProjectilePointV112 look{0,0,0};ProjectileObjectV112 actual;
  if(!b.object(source,actual)||!b.call("Required actual owner GetLookAtVec",s.look_at,actual,look))return false;
  ProjectilePointV112 origin;if(!b.point(*p.source_pointer_v99(0x380),origin)||!projectile_write_point_v112(p,0x388,origin,e))return false;
  *p.source_pointer_v99(0x3a4)=0;if(!b.store(0x3a0,origin[2]))return false;
  std::uint32_t bits{};if(!b.word(row,0x20,bits))return false;float radius;std::memcpy(&radius,&bits,4);
  if(!b.store(0x3a8,radius>=0?mul(radius,radius):-1.f))return false; //native __aeabi_fcmpge, NaN false.
  if(!b.object(*p.source_pointer_v99(0x380),actual))return false;
  if(!actual.visual2d8)return b.fail("Required SAME source VisualObject2d8 cell");
  if(*actual.visual2d8){std::uintptr_t node{};
   if(!b.call("Required actual Visual.GetSpecificNode(projectile_node)",s.node_from_name,*actual.visual2d8,"projectile_node",node))return false;
   *p.source_pointer_v99(0x3a4)=node;
   if(node){ProjectilePointV112 absolute;if(!b.node(node,absolute))return false;
    if(!b.store(0x3a0,absolute[2])||!projectile_write_point_v112(p,0x388,absolute,e))return false;}
  }
  for(auto pair:{std::pair<unsigned,unsigned>{0x40,0x3ac},{0x44,0x3b0},{0x3c,0x3b4}}){if(!b.word(row,pair.first,bits))return false;*p.source_word_v99(pair.second)=bits;}
  if(!b.word(row,0x28,bits))return false;std::int32_t visual;std::memcpy(&visual,&bits,4);
  if(visual>=0){if(!b.call("Required SAME AnimatedFxLibrary row8/SetVisualObject(file,NULL,false)",s.set_visual_index,p,visual))return false;}
  else if(!b.call("Required actual SetVisualObject(NULL)",s.set_visual_null,p))return false;
  auto prefix=std::make_shared<ProjectilePhysicalPrefixV112>();
  if(!b.call("Required actual retained PhysicalObject constructor prefix",s.retain_physical_prefix,prefix))return false;
  if(!s.construct_physical)return b.fail("Required actual PhysicalObjectC2");
  auto construct=[&](CanonicalProjectileV99& actual,ProjectilePhysicalPrefixV112& receipt,std::string& error){const bool done=s.construct_physical(actual,receipt,error);if(done)receipt.constructed=true;return done;};
  if(!b.call("Required actual PhysicalObjectC2(false,true,true,true,0,20,51f,0)",construct,p,*prefix))return false;
  if(!prefix->receiver||!prefix->identity)return b.fail("Required SAME actual PhysicalObject constructor receiver");
  if(!s.set_physical)return b.fail("Required actual SetPhysicalObject");
  auto assign=[&](CanonicalProjectileV99& actual,ProjectilePhysicalPrefixV112& receipt,bool value,std::string& error){const bool done=s.set_physical(actual,receipt,value,error);if(done)receipt.assigned=true;return done;};
  if(!b.call("Required actual SetPhysicalObject(receiver,false)",assign,p,*prefix,false))return false;
  if(!b.call("Required actual assigned physical-prefix retirement",s.retire_assigned_physical_prefix,prefix))return false;
  if(!projectile_read_point_v112(p,0x388,origin,e)||!b.call("Required actual projectile SetPosition",s.set_position,p,origin,true))return false;
  if(!b.object(*p.source_pointer_v99(0x380),actual)||!actual.rotation16c)return b.fail("Required SAME source rotation16c cells");
  ProjectilePointV112 rotation;std::memcpy(rotation.data(),actual.rotation16c,12);
  if(!b.call("Required actual projectile SetRotation",s.set_rotation,p,rotation))return false;
  ProjectilePointV112 destination;
  if(*p.source_pointer_v99(0x384)){if(!b.point(*p.source_pointer_v99(0x384),destination))return false;}
  else{
   if(*p.source_pointer_v99(0x3a4)&&from_node){
    ProjectilePointV112 node;if(!b.node(*p.source_pointer_v99(0x3a4),node)||!b.point(*p.source_pointer_v99(0x380),origin))return false;
    look=normalized({sub(node[0],origin[0]),sub(node[1],origin[1]),0});
   }
   if(!b.point(*p.source_pointer_v99(0x380),origin))return false;
   for(unsigned i=0;i<3;++i)destination[i]=add(mul(look[i],1000.f),origin[i]);
  }
  if(!b.call("Required actual projectile SetDestination",s.set_destination,p,destination))return false;
  auto* visual_cell=p.base().pointer(0x2d8);
  if(*visual_cell){
   if(!b.call("Required actual Animator virtual1c(0,1,0,0,0)",s.animator_virtual1c,*visual_cell,0u,1u,0u,0u,0u))return false;
   if(!b.call("Required actual reloaded Visual.Sync",s.visual_sync,*visual_cell))return false;
   std::int32_t duration;std::memcpy(&duration,p.source_word_v99(0x3b4),4);
   if(duration==-666){if(!b.call("Required actual reloaded Animator virtual18(0)",s.animator_virtual18,*visual_cell,0u,duration))return false;std::memcpy(p.source_word_v99(0x3b4),&duration,4);}
  }
  if(!b.call("Required SAME PFObject.SetFlying(true)",s.pf_flying,p,true)||
     !b.call("Required SAME PFObject.SetSwimming(true)",s.pf_swimming,p,true))return false;
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return b.fail("Projectile SetInfo threw");}catch(...){return b.fail("Projectile SetInfo threw");}
}
bool projectile_set_info_angle_v112(CanonicalProjectileV99& p,std::int32_t row,std::uintptr_t source,std::uintptr_t target,
 std::uintptr_t check,std::uintptr_t hit,std::uintptr_t userdata,float angle,ProjectileMethodServicesV112& s,std::string& e){
 e.clear();Body b{p,s,e,{}};try{
  if(!b.call("Required actual virtualc8 SetInfo(bool=false) dispatch",s.virtual_set_info_bool,p,row,source,target,check,hit,userdata,false))return false;
  ProjectileObjectV112 actual;ProjectilePointV112 look{0,0,0};
  if(!b.object(*p.source_pointer_v99(0x380),actual)||!b.call("Required actual GetLookAtVec after virtual dispatch",s.look_at,actual,look))return false;
  const std::uint32_t radbits=0x3c8efa35;float radians;std::memcpy(&radians,&radbits,4);radians=mul(angle,radians);
  const float c=std::cos(radians),sn=std::sin(radians),x=look[0],y=look[1];
  look[0]=sub(mul(x,c),mul(y,sn));look[1]=add(mul(x,sn),mul(y,c));
  ProjectilePointV112 origin,destination;if(!b.point(*p.source_pointer_v99(0x380),origin))return false;
  for(unsigned i=0;i<3;++i)destination[i]=add(mul(look[i],1000.f),origin[i]);
  if(!b.call("Required actual angle SetDestination",s.set_destination,p,destination))return false;
  if(*p.base().pointer(0x2d8)&&!b.call("Required actual Visual.SyncRotation",s.visual_sync_rotation,*p.base().pointer(0x2d8)))return false;
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return b.fail("Projectile angle SetInfo threw");}catch(...){return b.fail("Projectile angle SetInfo threw");}
}
bool projectile_update_v112(CanonicalProjectileV99& p,ProjectileMethodServicesV112& s,std::string& e){
 e.clear();Body b{p,s,e,{}};try{
  if(!b.live())return false;
  if(*p.source_byte_v99(0x3d1)){
   std::int32_t result=0;const auto callback=*p.source_pointer_v99(0x3bc);
   if(callback&&!b.call("Required actual hit callback",s.callback,callback,p,*p.source_pointer_v99(0x3c0),result))return false;
   const auto peer=*p.source_pointer_v99(0x3cc);
   if(!peer||result==0){
    if(peer){std::array<float,2> point;if(!b.scalar(0x3c4,point[0])||!b.scalar(0x3c8,point[1])||
       !b.call("Required actual HandleImpactFX(peer,Point2)",s.impact_object,p,peer,point))return false;}
    *p.source_byte_v99(0x3d0)=1;*p.source_byte_v99(0x3d1)=0;
    if(!b.call("Required SAME manager.DeSpawn(false)",s.despawn,p,*p.source_pointer_v99(0x378),false))return false;
    e.clear();return true;
   }
   if(result==1){*p.source_byte_v99(0x3d1)=0;*p.source_byte_v99(0x3d0)=0;}
  }
  if(*p.source_pointer_v99(0x384)){ProjectileObjectV112 target;if(!b.object(*p.source_pointer_v99(0x384),target))return false;std::int32_t dead{};
   if(!b.call("Required actual target virtual34",s.object_virtual34,target,dead))return false;if(dead)*p.source_pointer_v99(0x384)=0;}
  if(*p.source_byte_v99(0x3d0)){e.clear();return true;}
  ProjectilePointV112 previous;if(!b.point(p.base().identity(),previous)||!projectile_write_point_v112(p,0x394,previous,e))return false;
  ProjectileTableRowV112 row;std::uint8_t follow{};if(!b.row(row)||!b.byte(row,0x35,follow))return false;
  ProjectilePointV112 destination;
  if(follow&&*p.source_pointer_v99(0x384)){if(!b.point(*p.source_pointer_v99(0x384),destination))return false;}
  else{
   if(!b.point(p.base().identity(),previous))return false;const float* dest=p.base().vector3(0x1a8);
   if(!dest)return b.fail("Required SAME actual destination1a8");
   auto direction=normalized({sub(dest[0],previous[0]),sub(dest[1],previous[1]),0});
   //Source calls GetTargetPosition again after normalization.
   if(!b.point(p.base().identity(),previous))return false;
   for(unsigned i=0;i<3;++i)destination[i]=add(mul(direction[i],1000.f),previous[i]);
  }
  if(!b.call("Required actual update SetDestination",s.set_destination,p,destination)||
     !b.call("Required qualified GameObject.Update",s.gameobject_update,p))return false;
  float speed,gravity;if(!b.scalar(0x3ac,speed)||!b.scalar(0x3b0,gravity))return false;
  ProjectileApplicationV112 app;if(!b.call("Required actual Application loan",s.application,app)||!app.owner)return b.fail("Required SAME cached Application GetDt");
  std::uint32_t dt{};if(!b.call("Required first actual Application.GetDt",app.get_dt,dt))return false;
  if(!b.store(0x3ac,add(speed,div(mul(gravity,static_cast<float>(dt)),-1000.f))))return false;
  const auto duration=*p.source_word_v99(0x3b4);if(!b.call("Required second SAME Application.GetDt",app.get_dt,dt))return false;
  const auto remaining=duration-dt;*p.source_word_v99(0x3b4)=remaining;std::int32_t signed_remaining;std::memcpy(&signed_remaining,&remaining,4);
  if(!b.scalar(0x3ac,speed))return false;
  if(signed_remaining<=0||speed<=0){if(!b.expire(1))return false;e.clear();return true;}
  float range;if(!b.scalar(0x3a8,range))return false;
  if(range>=0){ProjectilePointV112 now,origin;if(!b.point(p.base().identity(),now)||!projectile_read_point_v112(p,0x388,origin,e))return false;
   const float x=sub(now[0],origin[0]),y=sub(now[1],origin[1]),z=sub(now[2],origin[2]);
   const float distance=add(add(mul(x,x),mul(y,y)),mul(z,z));
   if(range<=distance){if(!b.expire(1))return false;e.clear();return true;}
  }
  const auto* actual_position=p.base().vector3(0x160);if(!actual_position)return b.fail("Required SAME position160");
  ProjectilePointV112 position;std::memcpy(position.data(),actual_position,12);bool found{};float height{};ProjectileFloorV112 floor;
  if(!b.call("Required actual PFWorld.GetFloorHeightAt(true)",s.world_floor_height,p,position,true,found,height,floor))return false;
  if(found&&floor.identity){
   if(!floor.owner||!floor.flags24)return b.fail("Required actual queried floor flags24");bool can{};
   if(!b.call("Required SAME PFObject.CanPathOn",s.can_path_on,p,floor,can))return false;
   if(can||(*floor.flags24&0x02000000u)){
    if(!*p.source_pointer_v99(0x3a4)){e.clear();return true;}
    float start;if(!b.scalar(0x3a0,start))return false;
    if(!(start<=height)){e.clear();return true;}
   }
   if(!b.expire(2))return false;e.clear();return true;
  }
  //Original uses SAME position160 address; query output/current reread after
  //callback is Main's actual loan, never a detached copied Scene/PF authority.
  std::memcpy(position.data(),actual_position,12);ProjectileRoomV112 room;
  if(!b.call("Required actual PFWorld.GetRoomAt",s.world_room_at,p,position,room))return false;
  if(!room.identity){if(!b.expire(0))return false;}
  else{if(!room.owner||!room.flags24)return b.fail("Required actual room flags24");if(*room.flags24&1u)if(!b.expire(2))return false;}
  e.clear();return true;
 }catch(const std::exception& ex){e=ex.what();return b.fail("Projectile Update threw");}catch(...){return b.fail("Projectile Update threw");}
}
}
