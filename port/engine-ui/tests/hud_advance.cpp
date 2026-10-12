#include "hud_advance.hpp"
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
using namespace dh2::ui;
namespace {
unsigned word(const unsigned char* p){unsigned x;std::memcpy(&x,p,4);return x;}
struct Context {unsigned n=0,dead=0;bool mutate=false,fail=false;std::array<HudAdvanceNode32,10>* nodes;std::array<HudWeakProxy8,10>* proxies;std::vector<std::vector<unsigned char>> events;};
std::vector<unsigned char> snapshot(Context& c){std::vector<unsigned char> raw(c.n*20);for(unsigned i=0;i<c.n;++i){auto& n=(*c.nodes)[i];auto& p=(*c.proxies)[i];unsigned out[5]={n.needs_advance,n.parent?unsigned(n.parent-c.nodes->data())+1:0,n.proxy?unsigned(n.proxy-c.proxies->data())+1:0,p.references,p.alive};std::memcpy(raw.data()+i*20,out,20);}return raw;}
int destroy(void* p,HudWeakProxy8* proxy){auto& c=*static_cast<Context*>(p);auto raw=snapshot(c);unsigned id=unsigned(proxy-c.proxies->data());raw.insert(raw.begin(),4,0);std::memcpy(raw.data(),&id,4);c.events.push_back(raw);if(c.fail)return 0;if(c.mutate){(*c.nodes)[c.dead].parent=&(*c.nodes)[0];(*c.nodes)[c.dead].proxy=&(*c.proxies)[9];}return 1;}
}
int main(int argc,char**argv){if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||word(bytes.data())!=0x31414448)return 2;unsigned count=word(bytes.data()+4),deletes=0;std::size_t at=8;
 for(unsigned i=0;i<count;++i){if(at+12>bytes.size())return 2;auto input=word(bytes.data()+at),output=word(bytes.data()+at+4),events=word(bytes.data()+at+8);at+=12;if(at+input+output+events*(output+4)>bytes.size())return 2;auto raw=bytes.data()+at;at+=input;auto expected=bytes.data()+at;at+=output;auto event=bytes.data()+at;at+=events*(output+4);unsigned n=word(raw),dead=word(raw+4),ref=word(raw+8);if(n>9||input!=16+n*4||output!=n*20)return 2;std::array<HudAdvanceNode32,10> nodes{};std::array<HudWeakProxy8,10> proxies{};
  for(unsigned k=0;k<n;++k){nodes[k]={k+1,k+1<n?&nodes[k+1]:nullptr,k+1<n?&proxies[k]:nullptr,std::uint8_t(word(raw+16+k*4)),{}};proxies[k]={ref,std::uint8_t(k==dead?0:k%3==0?255:1),{}};}Context c{n,dead,word(raw+12)!=0,false,&nodes,&proxies,{}};HudAdvanceServices16 services{&c,destroy};if(dh2_ui_hud_notify_v1(nodes.data(),&services)!=0||snapshot(c)!=std::vector<unsigned char>(expected,expected+output)||c.events.size()!=events){std::cerr<<"Advance record "<<i<<" mismatch\n";return 1;}for(unsigned j=0;j<events;++j)if(std::memcmp(c.events[j].data(),event+j*(output+4),output+4))return 1;deletes+=events;
 }
 if(at!=bytes.size())return 2;unsigned guards=0;auto check=[&](bool ok){if(!ok)throw guards;++guards;};try{std::array<HudAdvanceNode32,10> nodes{};std::array<HudWeakProxy8,10> proxies{};nodes[0]={1,&nodes[1],&proxies[0],0,{}};proxies[0]={1,0,{}};Context c{2,0,false,true,&nodes,&proxies,{}};HudAdvanceServices16 svc{&c,destroy};check(dh2_ui_hud_notify_v1(nullptr,&svc)==-1);check(dh2_ui_hud_notify_v1(nodes.data(),&svc)==-2&&nodes[0].needs_advance==1&&nodes[0].parent==&nodes[1]&&proxies[0].references==0);proxies[0].references=1;c.fail=false;check(dh2_ui_hud_notify_v1(nodes.data(),&svc)==0&&!nodes[0].parent&&!nodes[0].proxy);nodes[0].reserved[0]=1;check(dh2_ui_hud_notify_v1(nodes.data(),&svc)==-1);}catch(unsigned n){std::cerr<<"Advance guard "<<n<<" mismatch\n";return 1;}
 std::cout<<"{\"comparisons\":"<<count<<",\"deletion_callbacks\":"<<deletes<<",\"failure_guards\":"<<guards<<",\"mismatches\":0}\n";
}
