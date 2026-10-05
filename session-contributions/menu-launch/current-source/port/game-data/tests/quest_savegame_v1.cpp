#include "../quest_savegame_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::data;
using Buffer=std::vector<std::uint8_t>;
void check(bool v,const char* m){if(!v)throw std::runtime_error(m);}
std::uint32_t word(const std::uint8_t* p){return std::uint32_t(p[0])|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;}
void append(Buffer& b,std::uint32_t value){for(unsigned i=0;i<4;++i)b.push_back(std::uint8_t(value>>(8*i)));}
struct Reader{Buffer b;std::size_t at{};std::uint32_t next(){check(b.size()-at>=4,"fixture word");auto value=word(b.data()+at);at+=4;return value;}Buffer bytes(std::size_t n){check(n<=b.size()-at,"fixture bytes");Buffer out(b.begin()+at,b.begin()+at+n);at+=n;return out;}};
struct Deliveries{Buffer log;};
bool load(void* context,std::uintptr_t identity,Bytes bytes,bool flag,std::size_t& used,std::string& error){
 used=0;if(bytes.size<4){error="packet length missing";return false;}auto n=word(bytes.data);used=4;
 if(n>bytes.size-4){error="packet truncated";return false;}
 auto& log=static_cast<Deliveries*>(context)->log;auto value=identity-1;
 append(log,value/16);append(log,value%16);append(log,flag);append(log,n);log.insert(log.end(),bytes.data+4,bytes.data+4+n);used+=n;return true;
}
Buffer snapshot(const SavedQuestProgressV1& p){Buffer b;for(const auto* a:{&p.current_quest,&p.primary_quest,&p.current_act,&p.compatible_act})for(auto v:*a)append(b,std::uint32_t(v));return b;}
int main(int argc,char** argv){try{
 check(argc==2,"usage original-fixture");std::ifstream f(argv[1],std::ios::binary);check(bool(f),"fixture missing");Reader r{{std::istreambuf_iterator<char>(f),{}}};check(r.bytes(4)==Buffer({'Q','S','V','1'}),"fixture signature");auto count=r.next();unsigned deliveries=0;
 for(unsigned k=0;k<count;++k){std::array<std::uint32_t,3> sizes{},masks{};for(auto& n:sizes)n=r.next();for(auto& n:masks)n=r.next();
  auto mode=r.next(),difficulty=r.next(),flag=r.next();auto raw=r.bytes(r.next());auto expected_used=r.next();auto state=r.bytes(48);auto expected_calls=r.next();Buffer expected_log;
  for(unsigned i=0;i<expected_calls;++i){for(unsigned j=0;j<3;++j)append(expected_log,r.next());auto n=r.next();append(expected_log,n);auto bytes=r.bytes(n);expected_log.insert(expected_log.end(),bytes.begin(),bytes.end());}
  QuestSavegameV1 owner;std::array<std::vector<std::uintptr_t>,3> identities;
  for(unsigned d=0;d<3;++d)for(unsigned i=0;i<sizes[d];++i)identities[d].push_back(masks[d]&(1u<<i)?0:1+d*16+i);
  std::string error;check(owner.attach_initialized_quests(identities,error),"quest attach");Deliveries actual;QuestLoadServicesV1 services{&actual,load};std::size_t used;bool mismatch;std::array<bool,3> mismatches{};
  const Bytes bytes{raw.data(),raw.size()};bool ok=mode?owner.unpack(difficulty,bytes,flag,services,used,mismatch,error):owner.load(bytes,services,used,mismatches,error);
  check(ok,error.c_str());check(used==expected_used,"original quest cursor");check(snapshot(owner.progress())==state,"original quest progress fields");check(actual.log==expected_log,"original typed Quest deliveries");deliveries+=expected_calls;
 }
 check(r.at==r.b.size(),"fixture remainder");
 std::array<std::vector<std::uintptr_t>,3> empty{};std::string error;std::size_t used;std::array<bool,3> mismatch{};
 Buffer packed;for(unsigned d=0;d<3;++d){append(packed,0);append(packed,7+d);append(packed,11+d);append(packed,2+d);}
 for(std::size_t n=0;n<packed.size();++n){QuestSavegameV1 owner;check(owner.attach_initialized_quests(empty,error),"empty authored owner");
  check(!owner.load({packed.data(),n},{},used,mismatch,error)&&used==n/4*4,"bounded quest prefix cursor");
  for(unsigned d=0;d<3;++d){check(owner.progress().current_quest[d]==(n>=16*d+8?7+int(d):-1),"current quest prefix stores");check(owner.progress().primary_quest[d]==(n>=16*d+12?11+int(d):-1),"primary quest prefix stores");check(owner.progress().current_act[d]==(n>=16*d+16?2+int(d):1),"act prefix stores");check(owner.progress().compatible_act[d]==owner.progress().current_act[d],"paired act stores");}
 }
 QuestSavegameV1 regular,volatile_owner;check(regular.attach_initialized_quests(empty,error)&&volatile_owner.attach_initialized_quests(empty,error),"both quest owners");
 regular.set_current_acts({9,9,9});volatile_owner.set_current_acts({9,9,9});
 check(load_player_quests_v1({packed.data(),packed.size()},regular,volatile_owner,{},used,error)&&used==packed.size(),"both original QEST passes");
 check(regular.progress().current_act==std::array<std::int32_t,3>{2,3,4}&&volatile_owner.progress().current_act==regular.progress().current_act,"QEST overrides both LNAM act stores");
 QuestSavegameV1 missing;check(!missing.load({packed.data(),packed.size()},{},used,mismatch,error)&&used==0,"no synthesized initialized quest owner");
 std::cout<<"PASS "<<count<<" original quest dispatch cases, "<<deliveries<<" typed deliveries, 48 truncation prefixes and both QEST act overrides\n";
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
