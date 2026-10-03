#include "../visual_timeline.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
std::int32_t number(const std::uint8_t* p){std::int32_t v;std::memcpy(&v,p,4);return v;}
float floating(std::uint32_t u){float f;std::memcpy(&f,&u,4);return f;}
bool equal(const void* av,const void* bv,std::size_t n){auto a=static_cast<const std::uint8_t*>(av),b=static_cast<const std::uint8_t*>(bv);for(std::size_t i=0;i<n;i+=4){if(word(a+i)==word(b+i))continue;if(!std::isnan(floating(word(a+i)))||!std::isnan(floating(word(b+i))))return false;}return true;}
struct Context {dh2::timeline::Completion* completion;std::uint32_t params[4];std::vector<std::uint8_t> events;};
void callback(void* p,dh2::timeline::State* s){auto& c=*static_cast<Context*>(p);auto* raw=reinterpret_cast<const std::uint8_t*>(s);c.events.insert(c.events.end(),raw,raw+56);raw=reinterpret_cast<const std::uint8_t*>(c.completion);c.events.insert(c.events.end(),raw,raw+8);
 switch(c.params[2]){case 0:break;case 1:dh2_timeline_notify(c.completion,s);break;case 2:dh2_timeline_jump(s,number(reinterpret_cast<const std::uint8_t*>(c.params+1)));break;case 3:dh2_timeline_range(s,number(reinterpret_cast<const std::uint8_t*>(c.params+1)),number(reinterpret_cast<const std::uint8_t*>(c.params+3)),1);break;case 4:dh2_timeline_scale(s,floating(c.params[1]));break;}}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||std::memcmp(bytes.data(),"VTG1",4))return 3;auto count=word(bytes.data()+4);std::size_t at=8;unsigned callbacks=0;
 for(unsigned i=0;i<count;++i){if(bytes.size()-at<16)return 4;auto op=word(bytes.data()+at),in=word(bytes.data()+at+4),out=word(bytes.data()+at+8),events=word(bytes.data()+at+12);at+=16;if(in!=84||out!=64||std::uint64_t(in)+out+64ull*events>bytes.size()-at)return 5;const auto* input=bytes.data()+at;const auto* expected=input+in;dh2::timeline::State s;dh2::timeline::Completion completion;std::memcpy(&s,input,56);std::memcpy(&completion,input+72,8);Context c{&completion,{}, {}};std::memcpy(c.params,input+56,16);dh2::timeline::Services services{&c,word(input+80)?callback:nullptr};int rc=-1;
  switch(op){case 0:rc=dh2_timeline_update(&s,number(input+56),&services);break;case 1:rc=dh2_timeline_jump(&s,number(input+56));break;case 2:rc=dh2_timeline_init(&s,number(input+56),number(input+60));break;case 3:rc=dh2_timeline_range(&s,number(input+56),number(input+60),c.params[2]);break;case 4:rc=dh2_timeline_clip(&s,number(input+56),number(input+60),number(input+64));break;case 5:rc=dh2_timeline_loop(&s,c.params[0]);break;case 6:rc=dh2_timeline_scale(&s,floating(c.params[0]));break;case 7:rc=dh2_timeline_notify(&completion,c.params[3]?&s:nullptr);break;case 8:rc=dh2_timeline_extra(&completion.extra_ms,c.params[3]?&s:nullptr);break;}
  if(rc||!equal(&s,expected,56)||!equal(&completion,expected+56,8)||c.events.size()!=events*64||!equal(c.events.data(),expected+64,events*64)){std::cerr<<"Timeline mismatch "<<i<<" operation "<<op<<'\n';return 6;}callbacks+=events;at+=in+out+events*64;
 }
 if(at!=bytes.size())return 7;dh2::timeline::State s{};s.scale=1;auto original=s;s.loop=256;if(dh2_timeline_update(&s,100,nullptr)!=-1||s.initialized)return 8;s=original;s.library_present=2;if(dh2_timeline_clip(&s,0,0,800)!=-1||s.end_ms)return 9;s=original;if(dh2_timeline_loop(&s,256)!=-1||s.loop)return 10;if(dh2_timeline_range(&s,0,800,256)!=-1||s.end_ms)return 11;if(dh2_timeline_update(nullptr,0,nullptr)!=-1)return 12;
 dh2::timeline::Completion c{13,256};if(dh2_timeline_notify(&c,&s)!=-1||c.extra_ms!=13)return 13;if(dh2_timeline_extra(&s.current_ms,&s)!=-1)return 14;if(dh2_timeline_notify(nullptr,&s)!=-1)return 15;
 std::cout<<"{\"comparisons\":"<<count<<",\"callback_snapshots\":"<<callbacks<<",\"atomic_rejection_checks\":8,\"mismatches\":0}\n";return 0;
}
