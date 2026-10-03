#include "../visual_motion.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
std::uint32_t word(const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;}
bool equal(const void* av,const void* bv,std::size_t n){auto a=static_cast<const std::uint8_t*>(av),b=static_cast<const std::uint8_t*>(bv);for(std::size_t i=0;i<n;i+=4){if(word(a+i)==word(b+i))continue;float x,y;std::memcpy(&x,a+i,4);std::memcpy(&y,b+i,4);if(!std::isnan(x)||!std::isnan(y))return false;}return true;}
struct Context {dh2::physical::BodyState* body;std::vector<std::uint8_t> events;};
std::uint32_t service(void* p,std::uint32_t e,float* values){auto& c=*static_cast<Context*>(p);auto* raw=reinterpret_cast<const std::uint8_t*>(&e);c.events.insert(c.events.end(),raw,raw+4);raw=reinterpret_cast<const std::uint8_t*>(values);c.events.insert(c.events.end(),raw,raw+12);if(e==6){std::memcpy(c.body->position,values,8);c.body->angle=values[2];}return 0;}
int main(int argc,char** argv){
 if(argc!=2)return 2;std::ifstream f(argv[1],std::ios::binary);std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)),{});if(bytes.size()<8||std::memcmp(bytes.data(),"VMG1",4))return 3;auto count=word(bytes.data()+4);std::size_t at=8;unsigned callbacks=0;
 for(unsigned i=0;i<count;++i){if(bytes.size()-at<16)return 4;auto op=word(bytes.data()+at),in=word(bytes.data()+at+4),out=word(bytes.data()+at+8),events=word(bytes.data()+at+12);at+=16;if(std::uint64_t(in)+out+16ull*events>bytes.size()-at)return 5;const auto* input=bytes.data()+at;const auto* expected=input+in;std::vector<std::uint8_t> actual(out);Context context{};
  if(op<2){if(in!=44||out!=28)return 6;dh2::visual::Delta d;float point[3];std::memcpy(&d,input,28);std::memcpy(point,input+32,12);auto rc=op?dh2_visual_reset_delta(&d,word(input+28),point):dh2_visual_calculate_delta(&d,word(input+28),point);if(rc)return 7;std::memcpy(actual.data(),&d,28);}
  else if(op==4){if(in<100||out!=100||in!=100+word(input+96)*12)return 8;dh2::visual::Root root;std::memcpy(&root,input,96);std::vector<float> deltas(word(input+96)*3);if(!deltas.empty())std::memcpy(deltas.data(),input+100,deltas.size()*4);dh2::visual::Displacement r{&root,deltas.data(),word(input+96),0};std::uint32_t moved=dh2_visual_displace(&r);std::memcpy(actual.data(),&moved,4);std::memcpy(actual.data()+4,&root,96);}
  else if(op==5){if(in!=12||out!=16)return 9;float point[3],q[4];std::memcpy(point,input,12);if(dh2_visual_rotation(q,point))return 10;std::memcpy(actual.data(),q,16);}
  else {if((op!=2&&op!=3)||in!=288||out!=192)return 11;dh2::subobjects::State s;dh2::physical::BodyState body;dh2::visual::Root root;dh2::physical::TransformRequest transform{};std::memcpy(&s,input,128);std::memcpy(&body,input+128,48);std::memcpy(&root,input+176,96);context.body=&body;dh2::subobjects::Services services{&context,service};const bool owner=word(input+280),present=word(input+276),hasroot=word(input+284);dh2::visual::Request r{owner?&s:nullptr,hasroot?&root:nullptr,present?&body:nullptr,&transform,&services,word(input+272),0};auto rc=op==2?dh2_visual_apply_position(&r):dh2_visual_sync_position(&r);if(rc)return 12;std::memcpy(actual.data(),&s,128);std::memcpy(actual.data()+128,&body,48);if(hasroot){std::memcpy(actual.data()+176,root.position,12);std::memcpy(actual.data()+188,&root.flags,4);} }
  if(!equal(actual.data(),expected,out)||context.events.size()!=events*16||!equal(context.events.data(),expected+out,events*16)){std::cerr<<"Visual motion mismatch "<<i<<" operation "<<op<<'\n';return 13;}callbacks+=events;at+=in+out+events*16;
 }
 if(at!=bytes.size())return 14;
 // Malformed caller contracts reject atomically; meaningful unlike mirroring
 // arithmetic. IEEE sample values are permitted for original comparisons.
 dh2::visual::Root r{};r.presence=4;dh2::visual::Displacement bad{&r,nullptr,0,0};if(dh2_visual_displace(&bad)!=-1||r.flags)return 15;r.presence=0;bad.reserved=1;if(dh2_visual_displace(&bad)!=-1||r.flags)return 16;bad.reserved=0;bad.count=1;if(dh2_visual_displace(&bad)!=-1)return 17;bad.count=0;bad.root=nullptr;if(dh2_visual_displace(&bad)!=-1)return 18;if(dh2_visual_displace(nullptr)!=-1)return 19;
 dh2::visual::Delta d{};if(dh2_visual_calculate_delta(nullptr,1,d.previous)!=-1||dh2_visual_reset_delta(&d,1,d.previous)!=-1||d.timestamp)return 20;float q[4]{};if(dh2_visual_rotation(q,q)!=-1||dh2_visual_rotation(nullptr,q)!=-1)return 21;
 std::cout<<"{\"comparisons\":"<<count<<",\"ordered_callbacks\":"<<callbacks<<",\"atomic_rejection_checks\":9,\"mismatches\":0}\n";return 0;
}
