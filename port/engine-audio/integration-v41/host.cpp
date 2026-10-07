#include "audio_precache_v41.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace dh2::audio;
unsigned checks{};void check(bool v){++checks;if(!v)throw std::runtime_error("precache contract");}
struct F {std::uintptr_t global{1};int uid{33},bound{33};bool off{},cached{},shrink{},missing{};std::vector<int> trace;std::shared_ptr<int>lease=std::make_shared<int>(1);};
int main(){F f;AudioPrecacheServicesV41 s;s.context=&f;
s.capture_nullable_manager=[](void*p,AudioManagerLeaseV41&m,std::string&){auto&v=*static_cast<F*>(p);v.trace.push_back(1);m={v.global,v.lease};return true;};
s.actual_get_sound=[](void*p,int&uid,std::string&){auto&v=*static_cast<F*>(p);v.trace.push_back(2);uid=v.uid;v.global=2;return true;};
s.actual_disabled=[](void*p,bool&off,std::string&){auto&v=*static_cast<F*>(p);v.trace.push_back(3);off=v.off;return true;};
s.actual_pack_bound=[](void*p,std::uintptr_t m,int&b,std::string&){auto&v=*static_cast<F*>(p);check(m==1);v.trace.push_back(4);b=v.bound;return true;};
s.actual_bank_info=[](void*p,std::uintptr_t m,int uid,std::string&){auto&v=*static_cast<F*>(p);check(m==1&&uid==v.uid);v.trace.push_back(5);if(v.shrink)v.bound=0;return true;};
s.actual_cached_slot=[](void*p,std::uintptr_t m,int uid,bool&cached,std::string&){auto&v=*static_cast<F*>(p);check(m==1&&uid==v.uid);v.trace.push_back(6);cached=v.cached;return true;};
s.load_exact_pack_uid=[](void*p,std::uintptr_t m,int uid,std::string&e){auto&v=*static_cast<F*>(p);check(m==1&&uid==33);v.trace.push_back(7);if(v.missing){e="Required exact raw UID33 asset";return false;}return true;};
AudioPrecacheResultV41 result;std::string e;
check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::loaded);check(f.trace==std::vector<int>({1,2,3,4,5,4,6,7}));
auto reset=[&]{f=F{};};reset();f.global=0;check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::null_manager);check(f.trace==std::vector<int>({1}));
reset();f.off=true;check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::disabled);check(f.trace==std::vector<int>({1,2,3}));
reset();f.uid=-1;check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::out_of_range);check(f.trace==std::vector<int>({1,2,3}));
reset();f.uid=34;check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::out_of_range);check(f.trace==std::vector<int>({1,2,3,4}));
reset();f.shrink=true;check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::out_of_range);check(f.trace==std::vector<int>({1,2,3,4,5,4}));
reset();f.cached=true;check(audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::cached);check(f.trace==std::vector<int>({1,2,3,4,5,4,6}));
reset();f.missing=true;check(!audio_container_precache_v41(s,result,e)&&result==AudioPrecacheResultV41::required&&!e.empty());
reset();f.lease.reset();check(!audio_container_precache_v41(s,result,e)&&f.trace==std::vector<int>({1}));
auto absent=s;absent.actual_disabled=nullptr;reset();check(!audio_container_precache_v41(absent,result,e)&&f.trace==std::vector<int>({1,2}));
std::cout<<"PASS "<<checks<<" precache lease/order/raw-UID/gate checks; fixture providers only\n";
}
