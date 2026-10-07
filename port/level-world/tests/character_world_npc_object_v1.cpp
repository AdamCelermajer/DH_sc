#include "../character_world_npc_object_v1.hpp"
#include "../character_world_npc_room_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <memory>
using namespace dh2::character;
unsigned checks=0;
void require(bool v){++checks;if(!v)throw std::runtime_error("NPC Object lifecycle assertion");}
struct Fixture {
 bool visual{},obstacle{},remove_body{};int fail_method{-1};
 dh2::physical::NativeBody body{nullptr,.36f,1};bool physical{};
 std::vector<int> calls;
 static bool has_visual(void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(10);return f.visual;}
 static dh2::physical::NativeBody* get_physical(void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(11);return f.physical?&f.body:nullptr;}
 static int method(void* p,WorldNpcObjectMethodV1 m,std::string& e){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(int(m));if(int(m)==f.fail_method){e="declared fixture unavailable";return 1;}return 0;}
 static bool is_obstacle(void* p,bool& v,std::string&){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(12);v=f.obstacle;return true;}
 static bool weight(void* p,float& v,std::string&){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(13);v=1;return true;}
 static bool extent(void* p,float& v,std::string&){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(14);v=0;if(f.remove_body)f.physical=false;return true;}
 static bool aabb(void* p,float* v,std::string&){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(15);const float a[]{-4,-6,0,8,10,0};for(int i=0;i<6;++i)v[i]=a[i];return true;}
 WorldNpcObjectServicesV1 services(){return {this,has_visual,get_physical,method,is_obstacle,weight,extent,aabb};}
};
struct Rooms {
 CharacterWorldNpcRoomV1* room[2]{};
 static CharacterWorldNpcRoomV1* get(void* p,std::uintptr_t id){auto& r=*static_cast<Rooms*>(p);return id==501?r.room[0]:id==502?r.room[1]:nullptr;}
 static int open(void*,const char* name,std::uintptr_t* out){require(!std::strcmp(name,"DebugSwitches.savegame"));*out=0;return 0;}
 static int close(void*,std::uintptr_t){throw std::runtime_error("unexpected source Debug file close");}
};
int main(){try{
 {
  Fixture f;auto services=character_pf_services_v1(f.services());
  bool obstacle=false;float radius=-1,strength=-1;std::string error;
  require(services.context==&f&&services.physical==Fixture::get_physical);
  require(services.is_pf_obstacle(services.context,obstacle,error)&&obstacle);
  require(services.obstacle_weight(services.context,radius,error)&&radius==50.f);
  require(services.obstacle_extent(services.context,strength,error)&&strength==20.f);
  require(f.calls.empty());
  WorldNpcObjectFieldsV1 fields;dh2::navigation::NavigationObject pf;dh2_nav_object_defaults(&pf);
  CharacterWorldNpcObjectV1 object(fields,pf,nullptr,nullptr,1,services);
  const auto before=pf;float position[]{1,2,3},aabb[]{-4,-6,0,8,10,0};
  require(!object.init_pf_object(nullptr,aabb)&&!std::memcmp(&pf,&before,sizeof pf));
  require(!object.init_pf_object(position,aabb)&&!std::memcmp(&pf,&before,sizeof pf));
 }
 for(unsigned mask=0;mask<32;++mask){
  WorldNpcObjectFieldsV1 fields;fields.non_zonable2ed=bool(mask&1);fields.zoning2ee=bool(mask&2);
  dh2::navigation::NavigationObject pf;require(dh2_nav_object_defaults(&pf)==0);
  Fixture f;f.visual=mask&4;f.physical=mask&8;fields.enabled8a=bool(mask&16);
  CharacterWorldNpcObjectV1 o(fields,pf,nullptr,nullptr,1,f.services());
  std::uint8_t v=77;require(!o.read_visible(v)&&v==77);
  require(o.update_pf()&&f.calls.empty()); // genuine ctor-null PF floor branch
  require(o.enabled());require(o.read_visible(v)&&v==fields.enabled8a&&fields.updating85==1&&(pf.motion.object_flags&8));
  require(o.zone_exited());require(fields.entered2f0==0&&fields.updating85==(fields.non_zonable2ed||!fields.zoning2ee));
  require(o.zone_entered());require(fields.entered2f0==1&&fields.updating85==1);
  require(o.disabled());require(fields.visible80==0&&fields.updating85==0&&!(pf.motion.object_flags&8)&&fields.disabled373==1);
  require(f.calls.back()==int(WorldNpcObjectMethodV1::CharacterStop));
 }
 {
  WorldNpcObjectFieldsV1 fields;dh2::navigation::NavigationObject pf;dh2_nav_object_defaults(&pf);Fixture f;f.visual=true;
  CharacterWorldNpcObjectV1 o(fields,pf,nullptr,nullptr,1,f.services());
  require(o.set_enable(true)&&f.calls.empty()&&!fields.visible_written); // ctor enabled8a already1
  require(!o.zone_entered()&&fields.entered2f0==1&&!fields.visible_written);
  f.fail_method=int(WorldNpcObjectMethodV1::SyncVisibility);
  require(!o.enabled()&&fields.visible_written&&fields.visible80==1&&fields.updating85==0);
  f.fail_method=-1;require(o.set_enable(false)&&fields.enabled8a==0&&fields.visible80==0);
  require(o.set_enable(true)&&fields.enabled8a==1&&fields.visible80==1);
 }
 for(unsigned mask=0;mask<8;++mask){
  WorldNpcObjectFieldsV1 fields;dh2::navigation::NavigationObject pf;dh2_nav_object_defaults(&pf);pf.motion.floor=0;
  Fixture f;f.physical=mask&1;f.obstacle=mask&2;f.remove_body=mask&4;
  CharacterWorldNpcObjectV1 o(fields,pf,nullptr,nullptr,1,f.services());
  // Non-obstacle branches need no geometry/obstacle registry. Obstacle branch
  // deliberately reaches the real kernel with unavailable inputs and fails.
  if(f.obstacle){require(!o.update_pf());require(f.calls==std::vector<int>({12,11,13,14}));require(pf.radius==1);}
  else{require(o.update_pf());require(pf.radius==(f.physical?36.f:8.f));require(f.calls==std::vector<int>(f.physical?std::vector<int>{12,11}:std::vector<int>{12,11,15}));}
 }
 {
  std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug_owner(dh2_character_debug_create(),dh2_character_debug_destroy);auto* debug=debug_owner.get();require(debug!=nullptr);DebugFileServices24 files{nullptr,Rooms::open,Rooms::close};
  Rooms rooms;float bounds[]{0,0,0,10,10,10};
  CharacterWorldNpcRoomV1 first(501,bounds,debug,&files,{&rooms,Rooms::get}),second(502,bounds,debug,&files,{&rooms,Rooms::get});rooms.room[0]=&first;rooms.room[1]=&second;
  for(float x:{-1.f,0.f,5.f,10.f,11.f})for(float y:{-1.f,0.f,5.f,10.f,11.f}){
   WorldNpcObjectFieldsV1 fields;dh2::navigation::NavigationObject pf;dh2_nav_object_defaults(&pf);Fixture f;
   CharacterWorldNpcObjectV1 o(fields,pf,nullptr,nullptr,1,f.services());float pos[]{x,y,9999};WorldNpcRoomObjectV1 object{1,&fields,&o,pos};
   bool inside=x>=0&&x<=10&&y>=0&&y<=10;
   require(first.add_initial(&object)==int(inside));require(first.objects().size()==unsigned(inside));
   if(inside){
    require(fields.room2f4==501&&fields.assigned2ef&&fields.entered2f0&&!fields.visible_written);
    require(second.add_initial(&object)==1&&first.objects().empty()&&second.objects().size()==1&&fields.room2f4==502);
    second.add(1);require(second.objects().size()==1);second.remove(1);require(second.objects().empty()&&fields.room2f4==502&&fields.assigned2ef);
   }
  }
  std::uint32_t loaded,count;require(dh2_character_debug_snapshot(debug,&loaded,&count)==1&&loaded&&count==7);
  bool found=false;for(unsigned i=0;i<count;++i){const char* key;std::uint32_t value;require(dh2_character_debug_entry(debug,i,&key,&value)==1);found|=!std::strcmp(key,"isTracingRoomZoneInit");}require(found);
 }
 std::cout<<"{\"checks\":"<<checks<<",\"complete_NPC_AI\":false,\"explicit_virtual_service_fixtures\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
