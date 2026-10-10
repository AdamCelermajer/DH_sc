#include "source_summon.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace dh2;
using namespace dh2::enemy_ai;

namespace {
void check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
std::vector<std::uint8_t> bytes(const std::string& path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("missing CharacterTable input");return {std::istreambuf_iterator<char>(f),{}};}
struct Trace {
 std::shared_ptr<int> lease=std::make_shared<int>(7);
 std::vector<std::string> calls;
 std::string source_name;
 std::uint32_t field{};
 std::uint32_t spawn_calls{};
 std::uintptr_t last_identity{};
 std::uintptr_t next_identity{100};
 bool room_accept{};
 bool online{};
 bool fail_position{};
};
bool write_string(void* raw,std::uint32_t field,const std::string& value,std::string&){auto& t=*static_cast<Trace*>(raw);t.field=field;t.source_name=value;t.calls.push_back("character-cstring");return true;}
bool spawn(Trace& t,const data::CharacterTable* table,const char* type,const char* name,bool deferred,bool network,
 world::CanonicalClassReceiverV1& receiver,bool& created,std::string& error){
 if(!table||std::strcmp(type,"Character")||!name||!deferred||network){error="wrong source manager Spawn arguments";return false;}
 t.calls.push_back("manager-spawn");++t.spawn_calls;
 receiver={};receiver.object.identity=t.next_identity++;t.last_identity=receiver.object.identity;receiver.object.lease=t.lease;receiver.source_lease=t.lease;
 receiver.object.context=&t;receiver.properties=[&t]{return world::CanonicalPropertyActorV1{nullptr,nullptr,{&t,nullptr,nullptr,nullptr,nullptr,&write_string,nullptr,nullptr}};};
 receiver.init_post=[&t](std::string&){t.calls.push_back("init-post");return true;};
 receiver.source_init_final_v95=[&t](std::string&){t.calls.push_back("init-final");return true;};
 created=true;return true;
}
SourceCreateNpcServicesV1 creator_services(Trace& trace,const data::CharacterTable& table){
 SourceCreateNpcServicesV1 s;s.characters=&table;
 s.source_manager_spawn=[&trace,&table](const char* type,const char* name,bool deferred,bool network,auto& r,bool& created,std::string& e){return spawn(trace,&table,type,name,deferred,network,r,created,e);};
 s.set_idle_state=[&trace](auto& receiver,bool idle,std::string&){check(receiver.object.identity>0&&!idle,"source SM_SetIdleState(false)");trace.calls.push_back("idle-state");return true;};
 return s;
}
}

int main(int argc,char** argv){try{
 check(argc==2,"expected Character cache data directory");
 auto records=bytes(std::string(argv[1])+"/character_properties_pyarray.bin");
 auto names=bytes(std::string(argv[1])+"/character_properties_pyarraynames.bin");
 auto fields=bytes(std::string(argv[1])+"/character_properties_pystructnames.bin");
 data::CharacterTable table;std::string error;
 check(data::load_characters({records.data(),records.size()},{names.data(),names.size()},
  {fields.data(),fields.size()},table,error),"load original CharacterTable");
 auto row=std::find(table.names.begin(),table.names.end(),"Swamp_Moth_Minions");
 check(row!=table.names.end(),"authored Swamp_Moth_Minions source CharacterTable row");
 const auto id=static_cast<std::int32_t>(row-table.names.begin());

 Trace trace;auto creator=creator_services(trace,table);SourceCreateNpcResultV1 created;
 check(source_create_npc_v1(id,nullptr,false,false,creator,created,error),"source CreateNPC semantic body");
 check(created.created&&created.character_id==id&&created.generated_name=="0001"&&
  trace.source_name=="Swamp_Moth_Minions"&&trace.field==0x1398,
  "source CreateNPC CharacterTable lookup/name/field");
 check(trace.calls==std::vector<std::string>{"manager-spawn","character-cstring","init-post","init-final","idle-state"},
  "source CreateNPC ObjectManager/factory/InitPost/InitFinal/idle order");

 trace.calls.clear();
 SourceSummonServicesV1 services;services.owner={trace.lease,999,77,
  (const float*)nullptr,(const float*)nullptr};
 const float owner_position[3]{10,20,30},owner_rotation[3]{0,0,1};
 services.owner.position160=owner_position;services.owner.rotation16c=owner_rotation;services.characters=&table;
 services.create_npc=[&](std::int32_t character,SourceCreateNpcResultV1& out,std::string& e){return source_create_npc_v1(character,nullptr,false,false,creator,out,e);};
 services.source_position_from_offsets=[&](std::uintptr_t owner,const std::array<float,3>& base,const std::array<float,3>& rotation,const std::array<float,3>& offset,bool orient,std::array<float,3>& out,std::string&){
  check(owner==services.owner.identity&&rotation==std::array<float,3>{0,0,1}&&orient,"same owner GetLookAtVec position request");
  trace.calls.push_back("look-at-position");out={base[0]+offset[0],base[1]+offset[1],base[2]+offset[2]};return true;};
 services.set_initial_position=[&](auto& r,const auto& p,std::string&){check(r.object.identity==trace.last_identity&&p==std::array<float,3>{10,20,30},"same created Character initial position");trace.calls.push_back("initial-position");return true;};
 services.set_position=[&](auto& r,const auto& p,bool destination,std::string&){check(r.object.identity==trace.last_identity&&p==std::array<float,3>{10,20,30}&&destination,"same Character SetPosition");trace.calls.push_back("position");return !trace.fail_position;};
 services.set_rotation=[&](auto& r,const auto& p,std::string&){check(r.object.identity==trace.last_identity&&p==std::array<float,3>{0,0,1},"same Character SetRotation");trace.calls.push_back("rotation");return true;};
 services.mark_summoned=[&](auto& r,std::string&){check(r.object.identity==trace.last_identity,"same Character summoned flag");trace.calls.push_back("summoned");return true;};
 services.room_add_initial_object=[&](std::uintptr_t room,std::uintptr_t character,bool& accepted,std::string&){check(room==77&&character==trace.last_identity,"same caller RoomZone and new Character");trace.calls.push_back("room-add-initial");accepted=trace.room_accept;return true;};
 services.manager_add_no_room=[&](auto& r,std::string&){check(r.object.identity==trace.last_identity,"same Character AddNoRoomObject");trace.calls.push_back("no-room-add");return true;};
 services.zone_entered=[&](auto& r,std::string&){check(r.object.identity==trace.last_identity,"same Character ZoneEntered");trace.calls.push_back("zone-entered");return true;};
 services.set_spawn_state=[&](auto& r,bool a,bool b,std::string&){check(r.object.identity==trace.last_identity&&!a&&!b,"source SM_SetSpawnState(false,false)");trace.calls.push_back("spawn-state");return true;};
 services.online_byte5=[&](bool& value,std::string&){trace.calls.push_back("online-query");value=trace.online;return true;};
 services.assign_network_id=[&](auto& r,std::string&){check(r.object.identity==trace.last_identity,"same Character network identity");trace.calls.push_back("network-id");return true;};
 services.online_post_assignment=[&](auto& r,std::uintptr_t owner,std::string&){check(r.object.identity==trace.last_identity&&owner==999,"same source _Summon online tail");trace.calls.push_back("network-tail");return true;};
 dh2_script_value arguments[6]{};arguments[0].type=DH2_SCRIPT_NUMBER;arguments[0].number=static_cast<float>(id);
 arguments[1].type=DH2_SCRIPT_BOOLEAN;arguments[1].boolean=1;
 for(unsigned i=2;i<5;++i){arguments[i].type=DH2_SCRIPT_NUMBER;arguments[i].number=0.f;}
 arguments[5].type=DH2_SCRIPT_BOOLEAN;arguments[5].boolean=1;
 dh2_script_value result{};std::uint32_t returned{};char diagnostic[256]{};
 check(source_summon_callback_v1(&services,arguments,6,&result,1,&returned,diagnostic,sizeof(diagnostic))==0,
  "authored six-argument source _Summon");
 check(returned==1&&result.type==DH2_SCRIPT_IDENTITY&&result.identity==created.receiver.object.identity+1,
  "source _Summon returns same created Character userdata");
 const std::vector<std::string> expected_calls{"look-at-position","manager-spawn","character-cstring","init-post","init-final","idle-state","initial-position","position","rotation","summoned","room-add-initial","no-room-add","zone-entered","spawn-state","online-query"};
 if(trace.calls!=expected_calls){for(const auto& call:trace.calls)std::cerr<<call<<" ";std::cerr<<"\n";}
 check(trace.calls==expected_calls,"source _Summon placement/room/idle/ReturnValues/online prefix order");

 trace.calls.clear();trace.room_accept=true;
 check(source_summon_callback_v1(&services,arguments,6,&result,1,&returned,diagnostic,sizeof(diagnostic))==0&&returned==1,
  "source RoomZone accepted Summon");
 check(std::find(trace.calls.begin(),trace.calls.end(),"no-room-add")==trace.calls.end()&&
  std::find(trace.calls.begin(),trace.calls.end(),"zone-entered")==trace.calls.end(),
  "RoomZone AddInitialObject accepted branch skips no-room insertion");

 trace.calls.clear();trace.room_accept=false;trace.fail_position=true;returned=99;
 const auto failed=source_summon_callback_v1(&services,arguments,6,&result,1,&returned,diagnostic,sizeof(diagnostic));
 check(failed==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&returned==0&&
  std::find(trace.calls.begin(),trace.calls.end(),"rotation")==trace.calls.end(),
  "SetPosition failure preserves prefix and blocks later source operations");

 // Missing online post-assignment must reject after the original userdata and
 // AssignObjectNetworkId prefix; it is never treated as a completed online spawn.
 trace.calls.clear();trace.fail_position=false;trace.online=true;services.online_post_assignment={};
 const auto network_failed=source_summon_callback_v1(&services,arguments,6,&result,1,&returned,diagnostic,sizeof(diagnostic));
 check(network_failed==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE&&returned==1&&
  std::find(trace.calls.begin(),trace.calls.end(),"network-id")!=trace.calls.end()&&
  std::find(trace.calls.begin(),trace.calls.end(),"network-tail")==trace.calls.end(),
  "online tail fails closed after preserved ReturnValues/network-ID prefix");
 std::cout<<"PASS source CreateNPC/Summon semantic kernels; actual CharacterTable row, exact authored six-arg gate path, one receiver across manager/position/RoomZone/network services, required-prefix failures; live native world spawn=false\n";
 return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}


