#include "destructible_container_data_v16.hpp"
#include <cstring>
namespace dh2::world {
bool DestructibleContainerTableV16::load(const std::uint8_t* p,std::size_t n,const std::uint8_t* names,std::size_t nn,std::string& e){
 if(!p||!names||n<4||nn<4){e="Required complete DestructibleContainers source groups";return false;}
 auto u=[](const std::uint8_t* b){return std::uint32_t(b[0])|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);};
 const auto count=u(p);if(count!=u(names)||count>(n-4)/57){e="DestructibleContainers source group count mismatch";return false;}
 std::size_t at=4,na=4;std::vector<DestructibleContainerRowV16> rows;std::vector<std::string> keys;
 auto word=[&](std::uint32_t& v){if(n-at<4)return false;v=u(p+at);at+=4;return true;};
 for(std::uint32_t i=0;i<count;++i){DestructibleContainerRowV16 r;
  for(auto& v:r.prefix4_1c)if(!word(v)){e="Short destructible prefix";return false;}
  if(at==n){e="Short destructible bool20";return false;}r.keep_physics20=p[at++];
  if(!word(r.raw24)||!word(r.loot28)||!word(r.script_length2c)||r.script_length2c>n-at){e="Invalid destructible script length";return false;}
  r.script30.assign(reinterpret_cast<const char*>(p+at),r.script_length2c);at+=r.script_length2c;
  for(auto& v:r.tail34_40)if(!word(v)){e="Short destructible tail";return false;}
  if(nn-na<4){e="Short destructible name length";return false;}auto z=u(names+na);na+=4;if(z>nn-na){e="Short destructible name";return false;}
  keys.emplace_back(reinterpret_cast<const char*>(names+na),z);na+=z;rows.push_back(std::move(r));
 }
 if(at!=n||na!=nn){e="Unexpected destructible source group tail";return false;}names_=std::move(keys);rows_=std::move(rows);return true;
}
std::int32_t DestructibleContainerTableV16::data_id(const std::string& name)const noexcept{for(std::size_t i=0;i<names_.size();++i)if(names_[i]==name)return static_cast<std::int32_t>(i);return -1;}
}
