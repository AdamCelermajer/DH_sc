#include "player_add_character_owner_v5.hpp"
#include "character_script_objects.hpp"
#include "../reference/player-add-character-v5/original-gold.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>
using namespace dh2;using namespace dh2::player;
struct Fixture {
 const PlayerAddGoldV5& gold;
 PlayerManagerOwnerV1 manager{{this,manager_service}};
 std::shared_ptr<int> lease=std::make_shared<int>(1);
 std::shared_ptr<character::ScriptCharacterObject> object;
 actor::RuntimeState runtime{};target_providers::Handle16 handle{};
 std::uint32_t type{};std::int32_t room{-1};const char* class_name="Player";
 std::string name="PlayerCharacter_7",archetype="Player";
 std::unique_ptr<world::CanonicalPlayerFacetV3> facet;
 std::unique_ptr<PlayerAddCharacterOwnerV5> owner;
 PlayerInfoFieldsV1* record{};std::int32_t* count{};
 std::int32_t source_class=2,controller{},internal{};std::uint8_t visible4e5{},visible{},zoned{};
 std::uintptr_t zone{};std::array<float,3> position{},rotation{},initial{},initialrotation{};
 std::array<float,3> hostposition{100,101,102},hostrotation{200,201,202},hostinitial{300,301,302},hostinitialrotation{400,401,402};
 std::vector<std::array<std::uint32_t,3>> trace;unsigned onlinecalls{};int failentry{-1};bool development{};
 explicit Fixture(const PlayerAddGoldV5& g):gold(g){
  const auto id=std::uintptr_t(0x100000123ull);
  object=std::make_shared<character::ScriptCharacterObject>(id,name,std::make_shared<data::PropertyState>(),std::make_shared<data::CombatActorState>(),position);
  handle.cached=id;zone=g.room?0x100000abcull:0;
  world::CanonicalPlayerFieldsV3 fields;fields.object=object;fields.runtime=&runtime;fields.handle=&handle;fields.type_f4=&type;fields.room64=&room;fields.class_name20=&class_name;fields.name=&name;fields.archetype=&archetype;fields.world_lease=lease;
  facet=std::make_unique<world::CanonicalPlayerFacetV3>(fields);
  assert(manager.initialize(error));assert(manager.add_player(7,9,0,g.local,error));assert(manager.get_by_internal(7,false,record,error));
  record->save_slot664=5;
  if(g.guard==1)source_class=-1;if(g.guard==2)record->character660=id;
  owner=std::make_unique<PlayerAddCharacterOwnerV5>(manager,PlayerAddServicesV5{lease,
   [this](const auto& q,auto& r,auto& e){return service(q,r,e);},
   [this](PlayerInfoFieldsV1& p,PlayerAddNetworkBorrowV5& out,std::string&){assert(&p==record);out={&source_class,&visible4e5};return true;},
   [this](std::uintptr_t id,PlayerAddCharacterBorrowV5& out,std::string& e){return borrow(id,out,e);}});
 }
 std::string error;
 bool borrow(std::uintptr_t id,PlayerAddCharacterBorrowV5& out,std::string& e){
  if(id==object->identity){out={id,lease,facet.get(),&controller,&internal,position.data(),rotation.data(),initial.data(),initialrotation.data(),&zone,&zoned};return true;}
  if(id==0x100000789ull){out={id,lease,nullptr,nullptr,nullptr,hostposition.data(),hostrotation.data(),hostinitial.data(),hostinitialrotation.data(),&zone,nullptr};return true;}
  e="Unknown fixture actor";return false;
 }
 static bool manager_service(void* p,const PlayerManagerRequestV1& q,PlayerManagerResponseV1& r,std::string& e){
  auto& f=*static_cast<Fixture*>(p);
  if(q.operation==PlayerManagerOperationV1::construct_player_info){*q.player=PlayerInfoFieldsV1{};return true;}
  if(q.operation==PlayerManagerOperationV1::character_initialization){f.count=q.character_count6c4;*f.count=4;return f.development?f.owner->continue_development_published(*q.player,f.count,q.id,f.object->identity,e):f.owner->add(*q.player,f.count,q.id,e);}
  r.value=0;return true; // Explicit offline manager-query fixture.
 }
 bool service(const PlayerAddRequestV5& q,PlayerAddResponseV5& r,std::string& e){
  const auto op=std::uint32_t(q.operation);if(int(op)==failentry){e="Required injected fixture source service";return false;}
  if(q.operation!=PlayerAddOperationV5::skill_count){
   std::uint32_t a=std::uint32_t(q.argument),b=std::uint32_t(q.secondary);
   if(q.operation==PlayerAddOperationV5::spawn)a=b=1;
   trace.push_back({op,a,b});
  }
  using O=PlayerAddOperationV5;
  switch(q.operation){
  case O::spawn:r.identity=object->identity;break;
  case O::is_local:r.value=gold.local;break;
  case O::is_active:r.value=gold.active;break;
  case O::online:r.value=gold.online;if(++onlinecalls==2&&gold.online&&gold.conflict)record->character660=gold.host_present?0x100000789ull:0;break;
  case O::is_host:r.value=gold.host;break;
  case O::get_host:r.identity=gold.host_present?0x100000789ull:0;break;
  case O::get_slots:{const std::int8_t bytes[3]{-3,2,-1};assert(q.size==3);std::memcpy(q.buffer,bytes,3);break;}
  case O::get_levels:assert(q.size==30);for(unsigned i=0;i<30;++i)static_cast<std::int8_t*>(q.buffer)[i]=i%4==0?-1:std::int8_t(i%6);break;
  case O::skill_count:r.value=gold.skills;break;
  case O::current_level:r.identity=gold.level?0x100000dddull:0;break;
  case O::set_visible:visible=std::uint8_t(q.argument);break;
  case O::set_position:std::memcpy(position.data(),q.buffer,12);break;
  case O::set_rotation:std::memcpy(rotation.data(),q.buffer,12);break;
  case O::set_initial_position:std::memcpy(initial.data(),q.buffer,12);break;
  case O::room_add:r.value=gold.room_accept;break;
  case O::remove_character:record->character660=0;break;
  default:break;
  }return true;
 }
 std::string events(){std::ostringstream s;for(std::size_t i=0;i<trace.size();++i){if(i)s<<',';s<<std::hex<<trace[i][0]<<std::dec<<':'<<trace[i][1]<<':'<<trace[i][2];}return s.str();}
};
int main(){unsigned checks=0;for(const auto& g:player_add_gold_v5){Fixture f(g);if(!f.manager.add_character(7,f.error)){std::cerr<<g.index<<' '<<f.error<<'\n';return 1;}
 assert(f.count&&unsigned(*f.count)==g.count);++checks;
 assert((f.record->character660!=0)==g.published);++checks;
 assert(unsigned(f.controller)==g.controller&&unsigned(f.internal)==g.internal&&f.visible==g.visible&&f.zoned==g.zoned);++checks;
 std::string expected=g.events;std::ostringstream filtered;bool first=true;
 std::size_t start=0;while(start<expected.size()){auto end=expected.find(',',start);auto part=expected.substr(start,end==std::string::npos?std::string::npos:end-start);if(part.rfind("36dfb0:",0)!=0){if(!first)filtered<<',';filtered<<part;first=false;}if(end==std::string::npos)break;start=end+1;}
 if(f.events()!=filtered.str()){std::cerr<<g.index<<" trace\n"<<f.events()<<"\n"<<filtered.str()<<'\n';return 2;}++checks;
 const auto count=*f.count;assert(!f.owner->add(*f.record,f.count,7,f.error)&&*f.count==count);++checks;
 }
 {Fixture f(player_add_gold_v5[1]);f.failentry=int(PlayerAddOperationV5::init_all);assert(!f.manager.add_character(7,f.error));assert(f.record->character660==f.object->identity&&*f.count==4&&f.owner->failed());assert(!f.owner->add(*f.record,f.count,7,f.error)&&*f.count==4);checks+=3;}
 {Fixture f(player_add_gold_v5[1]);f.failentry=int(PlayerAddOperationV5::attach_controller);assert(!f.manager.add_character(7,f.error));assert(*f.count==5&&f.owner->result().count_written);assert(!f.owner->add(*f.record,f.count,7,f.error)&&*f.count==5);checks+=3;}
 {Fixture f(player_add_gold_v5[1]);f.development=true;f.record->character660=f.object->identity;assert(f.manager.add_character(7,f.error));assert(*f.count==5&&f.owner->result().development_continuation&&f.owner->result().completed);assert(f.record->character660==f.object->identity&&f.trace.front()[0]==std::uint32_t(PlayerAddOperationV5::initialize_save));assert(!f.owner->continue_development_published(*f.record,f.count,7,f.object->identity,f.error)&&*f.count==5);checks+=4;}
 std::cout<<"Whole PlayerManager AddCharacter original96 same-facet/native PASS "<<checks<<" checks\n";
}
