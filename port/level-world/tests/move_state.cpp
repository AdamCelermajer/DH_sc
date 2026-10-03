#include "move_state.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
namespace {struct Record{std::uint32_t flags;std::int32_t walk,rotation;float factor;dh2::move::Policy policy;dh2::move::Speed speed;float rotation_speed;std::uint32_t focus_flags,focus_type;};static_assert(sizeof(Record)==68);}
int main(int argc,char** argv){if(argc!=2)return 2;std::ifstream stream(argv[1],std::ios::binary);std::uint32_t header[2];if(!stream.read(reinterpret_cast<char*>(header),8)||header[0]!=0x31564d44||header[1]>100000)return 2;std::vector<Record> rows(header[1]);if(!stream.read(reinterpret_cast<char*>(rows.data()),rows.size()*sizeof(Record)))return 2;
 for(const auto& r:rows){std::array<std::int32_t,224> sheet{};sheet[46]=r.walk;sheet[47]=r.rotation;dh2::move::Policy p;dh2::move::Speed s;float rot;auto f=r.flags;std::uint32_t type=~0u;
  if(dh2_move_policy(&p,&f)||dh2_move_speed(&s,sheet.data(),&r.factor)||dh2_move_rotation_speed(&rot,&f,sheet.data())||std::memcmp(&p,&r.policy,sizeof p)||std::memcmp(&s,&r.speed,sizeof s)||std::memcmp(&rot,&r.rotation_speed,4)||dh2_move_focus_begin(&f,&type)||f!=r.focus_flags||type!=r.focus_type)return 3;
 }
 std::printf("{\"original_derived_replay\":%zu,\"mismatches\":0}\n",rows.size());return 0;}
