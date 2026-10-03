#include "actor_rotation.hpp"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>
#include <vector>
namespace {struct Record {dh2::actor::RotationState before;dh2::actor::RotationPolicy policy;dh2::actor::RotationState expected;std::uint32_t sync;};static_assert(sizeof(Record)==68);}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream stream(argv[1],std::ios::binary);std::uint32_t h[2];if(!stream.read(reinterpret_cast<char*>(h),8)||h[0]!=0x31524f41||h[1]>100000)return 2;std::vector<Record> rows(h[1]);if(!stream.read(reinterpret_cast<char*>(rows.data()),rows.size()*sizeof(Record)))return 2;
 for(const auto& r:rows){auto s=r.before;std::uint32_t sync=~0u;if(dh2_actor_update_rotation(&s,&r.policy,&sync)||std::memcmp(&s,&r.expected,sizeof s)||sync!=r.sync)return 3;}
 // Caller failures must preserve both writable outputs and the policy input.
 const dh2::actor::RotationState good{{0,0,0},1,0,0};
 const dh2::actor::RotationPolicy good_policy{1,16,1,1};
 unsigned rejected=0;
 const auto reject=[&](dh2::actor::RotationState s,dh2::actor::RotationPolicy p,unsigned alias=0){
  const auto before=s;const auto before_policy=p;std::uint32_t sync=0xabcdef12;
  auto* sp=&s;const auto* pp=&p;auto* out=&sync;
  if(alias==1)sp=nullptr;if(alias==2)pp=nullptr;if(alias==3)out=nullptr;
  if(alias==4)out=&s.turn_positive;
  if(alias==5)out=&p.visual_present;
  if(alias==6)pp=reinterpret_cast<const dh2::actor::RotationPolicy*>(&s);
  if(dh2_actor_update_rotation(sp,pp,out)!=1||std::memcmp(&s,&before,sizeof s)||std::memcmp(&p,&before_policy,sizeof p)||sync!=0xabcdef12)return false;
  ++rejected;return true;
 };
 auto s=good;s.reserved=1;if(!reject(s,good_policy))return 4;
 s=good;s.turn_positive=2;if(!reject(s,good_policy))return 4;
 s=good;s.rotation[2]=std::numeric_limits<float>::infinity();if(!reject(s,good_policy))return 4;
 s=good;s.heading_angle=std::numeric_limits<float>::quiet_NaN();if(!reject(s,good_policy))return 4;
 auto p=good_policy;p.speed=std::numeric_limits<float>::infinity();if(!reject(good,p))return 4;
 p=good_policy;p.visual_present=2;if(!reject(good,p))return 4;
 p=good_policy;p.visual_with_rotation=2;if(!reject(good,p))return 4;
 p=good_policy;p.speed=3e38f;p.dt_ms=0xffffffff;if(!reject(good,p))return 4;
 s=good;s.rotation[2]=3e38f;s.heading_angle=-3e38f;if(!reject(s,good_policy))return 4;
 for(unsigned alias=1;alias<=6;++alias)if(!reject(good,good_policy,alias))return 4;
 std::printf("{\"original_derived_replay\":%zu,\"atomic_rejection_checks\":%u,\"mismatches\":0}\n",rows.size(),rejected);return 0;}
