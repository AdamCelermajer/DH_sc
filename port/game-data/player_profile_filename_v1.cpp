#include "player_profile_filename_v1.hpp"
#include <cstdio>
#include <stdexcept>

namespace dh2::data {
const char* player_profile_prefix_v1() noexcept {return "dh2_";}
const char* player_profile_extension_v1() noexcept {return ".savegame";}
const char* player_checkpoint_extension_v1() noexcept {return ".checkpoint";}
std::string player_profile_filename_v1(std::uint32_t slot,bool checkpoint,bool multiplayer){
 const char* extra=checkpoint?(multiplayer?"_multi":"_single"):"";
 const char* extension=checkpoint?player_checkpoint_extension_v1():player_profile_extension_v1();
 char buffer[64];
 const auto size=std::snprintf(buffer,sizeof(buffer),"%s%03u%s%s",player_profile_prefix_v1(),unsigned(slot),extra,extension);
 if(size<0||std::size_t(size)>=sizeof(buffer))throw std::runtime_error("profile filename formatting failed");
 return std::string(buffer,std::size_t(size));
}
}
