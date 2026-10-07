#include "campaign_save_filename_v45.hpp"
#include <cstdio>
namespace dh2::level {
std::string campaign_save_filename_v45(std::uint32_t slot,bool checkpoint,bool multi){
 char buffer[64];std::snprintf(buffer,sizeof(buffer),"%s%03u%s%s","dh2_",unsigned(slot),
  checkpoint?(multi?"_multi":"_single"):"",checkpoint?".checkpoint":".savegame");return buffer;
}
}
