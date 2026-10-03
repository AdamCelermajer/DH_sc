#include "../visual_timeline.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
bool equal(const void* av,const void* bv,std::size_t n){auto a=static_cast<const std::uint8_t*>(av),b=static_cast<const std::uint8_t*>(bv);for(std::size_t i=0;i<n;i+=4){if(word(a+i)==word(b+i))continue;float x,y;std::memcpy(&x,a+i,4);std::memcpy(&y,b+i,4);if(!std::isnan(x)||!std::isnan(y))return false;}return true;}
struct Context {bool update;std::vector<std::uint8_t> events;};
void callback(void* p,std::uint32_t event,dh2::timeline::State* s,const dh2::timeline::ReplayResult* plan){auto& c=*static_cast<Context*>(p);auto* raw=reinterpret_cast<const std::uint8_t*>(&event);c.events.insert(c.events.end(),raw,raw+4);raw=reinterpret_cast<const std::uint8_t*>(s);c.events.insert(c.events.end(),raw,raw+56);raw=reinterpret_cast<const std::uint8_t*>(plan);c.events.insert(c.events.end(),raw,raw+16);if(event==1&&plan->restart&&c.update)dh2_timeline_update(s,static_cast<std::int32_t>(plan->timestamp),nullptr);}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||std::memcmp(bytes.data(),"VTR1",4))return 3;auto count=word(bytes.data()+4);std::size_t at=8;unsigned callbacks=0,restarts=0;
 for(unsigned i=0;i<count;++i){if(bytes.size()-at<12)return 4;auto in=word(bytes.data()+at),out=word(bytes.data()+at+4),events=word(bytes.data()+at+8);at+=12;if(in!=92||out!=76||std::uint64_t(in)+out+76ull*events>bytes.size()-at)return 5;auto input=bytes.data()+at,expected=input+in;dh2::timeline::State s;dh2::timeline::ReplayFacts facts;dh2::timeline::ReplayResult result{};std::memcpy(&s,input,56);std::memcpy(&facts,input+56,32);Context c{bool(word(input+88)),{}};dh2::timeline::ReplayServices services{&c,callback};std::uint32_t accepted=dh2_timeline_replay(&result,&s,&facts,&services);
  if(accepted!=word(expected)||!equal(&s,expected+4,56)||!equal(&result,expected+60,16)||c.events.size()!=events*76||!equal(c.events.data(),expected+76,events*76)){std::cerr<<"Timeline replay mismatch "<<i<<'\n';return 6;}callbacks+=events;restarts+=result.restart;at+=in+out+76ull*events;
 }
 if(at!=bytes.size())return 7;dh2::timeline::State s{};dh2::timeline::ReplayFacts facts{};dh2::timeline::ReplayResult out{};facts.reserved=1;if(dh2_timeline_replay(&out,&s,&facts,nullptr)!=-1||s.scale)return 8;facts.reserved=0;facts.root_present=2;if(dh2_timeline_replay(&out,&s,&facts,nullptr)!=-1||s.scale)return 9;facts.root_present=0;facts.requested_loop=256;if(dh2_timeline_replay(&out,&s,&facts,nullptr)!=-1||s.scale)return 10;facts.requested_loop=0;if(dh2_timeline_replay(nullptr,&s,&facts,nullptr)!=-1)return 11;
 std::cout<<"{\"comparisons\":"<<count<<",\"ordered_callbacks\":"<<callbacks<<",\"root_restart_requests\":"<<restarts<<",\"atomic_rejection_checks\":4,\"mismatches\":0}\n";return 0;
}
