#include "../player_save_load_owner_v1.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
namespace {
void check(bool yes,const char* why){if(!yes)throw std::runtime_error(why);}
struct Input {std::ifstream file;explicit Input(const char* path):file(path,std::ios::binary){check(bool(file),"gold input");}
 template<class T>T read(){T v;file.read(reinterpret_cast<char*>(&v),sizeof(v));check(bool(file),"truncated gold");return v;}};
using Event=std::array<std::uint32_t,6>;
constexpr const char* names[]{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP","LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"};
std::uint32_t token(std::uintptr_t identity){if(!identity)return 0;check(identity>=0xaa0000000001ULL&&identity<=0xaa0000000003ULL,"genuine test profile identity");return std::uint32_t(identity-0xaa0000000000ULL);}
struct Fixture {
 std::shared_ptr<PlayerSavegameV1> save=std::make_shared<PlayerSavegameV1>();
 std::shared_ptr<int> lease=std::make_shared<int>(1);
 std::vector<Event> events;unsigned flags=0;std::uint64_t length=0;std::size_t failure=SIZE_MAX;
 PlayerSaveProfileV1 profile(unsigned index){return {0xaa0000000000ULL+index,lease};}
 bool invoke(const PlayerSaveLoadRequestV1& request,PlayerSaveLoadResponseV1& response,std::string& error){
  check(request.save==save.get(),"same retained Save");
  Event event{std::uint32_t(request.operation),request.argument,token(request.profile.identity),0,0,0};
  switch(request.operation){
   case PlayerSaveLoadOpV1::filename:check(request.argument==std::uint32_t(save->slot()),"fresh source slot");response.text="source_profile";break;
   case PlayerSaveLoadOpV1::create_profile:check(request.filename&&!std::strcmp(request.filename,"source_profile"),"exact delivered filename");response.profile=profile(2);break;
   case PlayerSaveLoadOpV1::load_section:{
    check(request.section,"source section name");unsigned index=0;while(index<15&&std::strcmp(request.section,names[index]))++index;check(index<15,"source named section");
    event[3]=index;event[4]=request.reader_enabled;break;
   }
   case PlayerSaveLoadOpV1::online:response.flag=flags&1;break;
   case PlayerSaveLoadOpV1::hosting_quest_flag:response.flag=flags&2;break;
   case PlayerSaveLoadOpV1::local_hosting:response.flag=flags&4;break;
   case PlayerSaveLoadOpV1::load_volatile_flag:response.flag=flags&8;break;
   case PlayerSaveLoadOpV1::volatile_stream:response.profile=profile(3);break;
   case PlayerSaveLoadOpV1::stream_size:response.amount=length;break;
   case PlayerSaveLoadOpV1::stream_seek:check(request.argument==0,"original 64-bit seek zero");break;
   case PlayerSaveLoadOpV1::quest_definition:response.value=37;break;
   case PlayerSaveLoadOpV1::unpack_quests:check(request.definition==37&&request.argument==0,"original quest definition/false bool");event[5]=37;break;
   default:break;
  }
  events.push_back(event);
  if(events.size()==failure){error="Required source delivery rejected";return false;}
  return true;
 }
 PlayerSaveLoadServicesV1 services(){return {lease,[this](const auto& request,auto& response,auto& error){return invoke(request,response,error);}};}
};
}
int main(int argc,char** argv){try{
 check(argc==2,"save load gold path");Input input(argv[1]);check(input.read<std::uint32_t>()==0x314c5350,"gold magic");auto count=input.read<std::uint32_t>();
 std::uint32_t boundaries=0,prefixes=0;
 for(unsigned j=0;j<count;++j){
  auto mask=input.read<std::int32_t>();auto present=input.read<std::uint32_t>();auto slot=input.read<std::int32_t>();auto flags=input.read<std::uint32_t>();auto length=input.read<std::uint64_t>();auto final=input.read<std::uint32_t>();auto n=input.read<std::uint32_t>();
  std::vector<Event> expected;for(unsigned k=0;k<n;++k){Event row;for(auto& word:row)word=input.read<std::uint32_t>();expected.push_back(row);}boundaries+=n;
  Fixture fixture;fixture.flags=flags;fixture.length=length;fixture.save->set_slot(slot);
  PlayerSaveLoadOwnerV1 owner(fixture.save,fixture.services());std::string error;
  if(present)check(owner.publish_profile(fixture.profile(1),error),"original preexisting profile");
  check(owner.load(mask,error),error.c_str());check(fixture.events==expected,"whole original Save.Load ordered boundaries");
  check(token(owner.profile().identity)==final&&&owner.save()==fixture.save.get(),"source profile publication/same Save");
  // Each original event is a genuine required failure boundary, including the
  // ctor's publication boundary and the volatile-quest tail after sections.
  for(unsigned failed=1;failed<=n;++failed){
   Fixture rejected;rejected.flags=flags;rejected.length=length;rejected.failure=failed;rejected.save->set_slot(slot);
   PlayerSaveLoadOwnerV1 partial(rejected.save,rejected.services());if(present)check(partial.publish_profile(rejected.profile(1),error),"partial profile");
   check(!partial.load(mask,error)&&rejected.events==std::vector<Event>(expected.begin(),expected.begin()+failed),"required ordered failure prefix");++prefixes;
  }
 }
 // Source genuinely null profile and slot -1: complete masks8/0x20 do not
 // reach any service. This is an actual owned-state branch, no callback stub.
 auto fresh=std::make_shared<PlayerSavegameV1>();PlayerSaveLoadOwnerV1 blank(fresh);std::string error;
 check(blank.load(8,error)&&blank.delivered_calls()==0&&blank.load(0x20,error)&&blank.delivered_calls()==0,"complete original blank-startup Load");
 check(!blank.load(2,error),"missing reached initializer rejected");
 check(!blank.publish_profile({0xaa0000000001ULL,{}},error)&&!blank.profile().identity,"invalid lease atomic");
 std::cout<<"{\"validation\":\"PASS\",\"original_complete_cases\":"<<count<<",\"ordered_boundaries\":"<<boundaries<<",\"required_failure_prefixes\":"<<prefixes<<",\"genuine_null_profile_startup_masks\":2,\"component_and_global_services_are_fixtures\":true,\"whole_Savegame_file_class_proven\":false}\n";return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
