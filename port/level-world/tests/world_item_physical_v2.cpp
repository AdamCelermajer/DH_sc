#define DH2_ITEM_FACTORY_FIXTURE_ONLY
#include "canonical_item_factory_v2.cpp"
#undef DH2_ITEM_FACTORY_FIXTURE_ONLY
#include "../world_item_physical_v2.hpp"
using namespace dh2::character;
namespace physical=dh2::physical;
struct PhysicsFixture {
 bool no_physics{},no_collision{};unsigned pf{},debugs{},peer_queries{};
 std::shared_ptr<int> pin=std::make_shared<int>(9);
 WorldItemPhysicalServicesV2 services(){WorldItemPhysicalServicesV2 s;s.owner=pin;
  s.debug=[this](const char* key,bool& value,std::string&){++debugs;if(!std::strcmp(key,"MP_NoCollisions"))value=no_collision;else if(!std::strcmp(key,"MP_NoPhysics"))value=no_physics;else return false;return true;};
  s.update_pf=[this](std::string&){++pf;return true;};
  s.peer_owner=[this](void*,std::uintptr_t& out,std::string&){++peer_queries;out=0;return true;};
  s.resolve=[](std::uintptr_t id,std::uintptr_t& out,std::uint32_t& type,std::string&){out=id;type=3;return true;};return s;
 }
};
int main(int argc,char** argv){try{
 check(argc==3);std::string error;auto read=[&](const std::string& dir,const char* prefix,const char* suffix){return file(dir+"/"+prefix+"_"+suffix+".bin");};
 auto b=read(argv[1],"loot_table","pyarray"),n=read(argv[1],"loot_table","pyarraynames"),s=read(argv[1],"loot_table","pystructnames");LootTablesV2 tables;check(tables.load(span(b),span(n),span(s),error),error);
 auto ab=read(argv[2],"loot_audiovisual","pyarray"),an=read(argv[2],"loot_audiovisual","pyarraynames"),as=read(argv[2],"loot_audiovisual","pystructnames");LootAudioVisualV8 av;check(av.load(span(ab),span(an),span(as),error),error);
 ItemFactoryFixture construction;CanonicalItemFactoryServicesV2 factories{&construction,ItemFactoryFixture::resolve,ItemFactoryFixture::condition,ItemFactoryFixture::debug,{&construction,ItemFactoryFixture::item,{},{},nullptr,nullptr}};
 CanonicalItemFactoryV2 factory(construction.manager,construction.map,std::make_shared<int>(1),tables.borrow(),av.borrow(),factories);construction.factory=&factory;
 std::shared_ptr<RetainedWorldItemObjectV1> item;check(factory.spawn("Item","PhysicalItem",false,true,item,error),error);
 auto* position=item->base().vector3(0x160);position[0]=200;position[1]=300;position[2]=50;
 // Explicit source bounds fixture; no visual mesh construction claimed here.
 auto* bounds=item->base().absolute_aabb12c();const float box[]{100,0,50,300,600,70};std::copy_n(box,6,bounds);
 physical::NativeWorld world;const float world_box[]{-100,-100,100,100};world.load(world_box);
 PhysicsFixture input;
 {WorldItemPhysicalV2 owner(*item,world,input.services());check(owner.construct(error),error);check(owner.native().body&&owner.secondary_shape());
  check(owner.config().shape.kind==0&&owner.config().shape.sensor==1&&owner.config().shape.radius==3.f);
  auto* shape=owner.secondary_shape();check(shape->IsSensor()&&shape->GetType()==e_circleShape);
  const auto filter=shape->GetFilterData();check(filter.groupIndex==-3&&filter.categoryBits==0x40&&filter.maskBits==4);
  check(owner.assign(error),error);check(input.pf==1&&*item->base().pointer(0x2dc)==reinterpret_cast<std::uintptr_t>(&owner));
  physical::NativeBodyObservation observed{};check(dh2_native_body_observe(&observed,&owner.native())==0);check(observed.position[0]==2&&observed.position[1]==3);
  check(owner.collision(physical::ContactEvent::add,nullptr,error));check(owner.collision(physical::ContactEvent::persist,nullptr,error));check(owner.collision(physical::ContactEvent::result,nullptr,error));
  item->fields().tooltip_character3c4=42;check(item->collision_end_v2(42,0,error));check(item->fields().tooltip_character3c4==0&&construction.item_callbacks==0);
  item->fields().tooltip_character3c4=42;check(item->collision_end_v2(42,item->base().identity(),error));check(item->fields().tooltip_character3c4==42);
  check(owner.detach(error),error);check(!owner.native().body&&!*item->base().pointer(0x2dc)&&input.pf==2);
 }
 input.no_collision=true;input.no_physics=true;
 {WorldItemPhysicalV2 owner(*item,world,input.services());check(owner.construct(error),error);check(owner.config().shape.group_index==-666);check(owner.assign(error),error);check(!owner.native().body&&input.pf==2);}
 std::cout<<"POItem native PASS actual-cache canonical Item+real Box2D circle/sensor/filter/pose/assignment/detach, source NULL-tooltip end and collision no-op methods; bounds/Debug/PF/Handle transports explicit fixtures; original differential pending; checks "<<checks<<'\n';return 0;
 }catch(const std::exception& e){std::cerr<<"check "<<checks<<" "<<e.what()<<'\n';return 1;}}
