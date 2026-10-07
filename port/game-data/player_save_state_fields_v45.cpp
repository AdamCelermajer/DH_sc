#include "player_savegame_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::data {namespace {
bool word(Bytes b,std::size_t& at,std::uint32_t& out,std::string& e){if(at>b.size||b.size-at<4){e="Truncated original saved-state word";return false;}out=std::uint32_t(b.data[at])|std::uint32_t(b.data[at+1])<<8|std::uint32_t(b.data[at+2])<<16|std::uint32_t(b.data[at+3])<<24;at+=4;return true;}
bool text(Bytes b,std::size_t& at,std::string& out,std::string& e){std::uint32_t n;if(!word(b,at,n,e))return false;if(!n||at>b.size||n>b.size-at||b.data[at+n-1]!=0){e="Required bounded original saved CString";return false;}out.assign(reinterpret_cast<const char*>(b.data+at),n-1);at+=n;return true;}
std::int32_t signed_word(std::uint32_t u){std::int32_t v;std::memcpy(&v,&u,4);return v;}
}
bool PlayerSavegameV1::initialize_level_states_v45(const std::vector<std::int32_t>& levels,
 const std::vector<std::int32_t>& maps,std::string& e){
 if(levels.size()>INT32_MAX||maps.size()>INT32_MAX){e="Original Level/Map state allocation count exceeded";return false;}
 // Whole _InitLevelStates46954c allocates only absent tier pointers and copies
 // authored LevelTable+28/MapLocTable+8; it does not reset loaded arrays.
 for(unsigned tier=0;tier<3;++tier){if(!level_states_present_[tier]){level_states68_[tier]=levels;level_states_present_[tier]=true;}if(!map_states_present_[tier]){map_states74_[tier]=maps;map_states_present_[tier]=true;}}
 return true;
}
bool PlayerSavegameV1::load_level_states_v45(Bytes b,const std::vector<std::string>& levels,
 const std::vector<std::string>& maps,std::size_t& used,std::string& e){
 used=0;if((!b.data&&b.size)||b.size>UINT32_MAX){e="Invalid original LVLS span";return false;}
 auto load=[&](const auto& names,auto& states,const auto& present){for(unsigned tier=0;tier<3;++tier){
  if(!present[tier]||states[tier].size()!=names.size()){e="Required actual initialized source Level/Map state array";return false;}
  std::uint32_t count;if(!word(b,used,count,e))return false;
  for(std::int32_t i=0;i<signed_word(count);++i){std::string name;std::uint32_t value;if(!text(b,used,name,e)||!word(b,used,value,e))return false;
   const auto found=std::find(names.begin(),names.end(),name);if(found!=names.end())states[tier][std::size_t(found-names.begin())]=signed_word(value);
  }
 }return true;};
 return load(levels,level_states68_,level_states_present_)&&load(maps,map_states74_,map_states_present_);
}
bool PlayerSavegameV1::set_level_state_v117(std::int32_t level,std::int32_t state,std::int32_t difficulty,std::string& e){
 // Original466e48 stores into the initialized Save68[difficulty] array.
 // Reject unsafe native indices before access; never allocate a replacement.
 if(difficulty<0||difficulty>=3||!level_states_present_[difficulty]||level<0||
    std::size_t(level)>=level_states68_[difficulty].size()||state<0||state>1){
  e="Original SG_SetLevelState requires initialized tier and valid level/state";return false;
 }
 level_states68_[difficulty][level]=state;e.clear();return true;
}
bool PlayerSavegameV1::set_map_state_v117(std::int32_t location,std::int32_t state,std::int32_t difficulty,std::string& e){
 // Original466b18 targets the SAME Save74[difficulty] location array.
 if(difficulty<0||difficulty>=3||!map_states_present_[difficulty]||location<0||
    std::size_t(location)>=map_states74_[difficulty].size()||state<0||state>2){
  e="Original SG_SetMapLocState requires initialized tier and valid location/state";return false;
 }
 map_states74_[difficulty][location]=state;e.clear();return true;
}
bool PlayerSavegameV1::load_fast_travel_v45(Bytes b,std::size_t& used,std::string& e){
 used=0;if((!b.data&&b.size)||b.size>UINT32_MAX){e="Invalid original FTVL span";return false;}
 for(unsigned tier=0;tier<3;++tier){std::string value;if(!text(b,used,value,e))return false;
  if(value.size()>64)return true; // source469cbc exits whole reader early
  std::uint64_t bits=0;for(std::size_t i=0;i<value.size();++i){const auto c=value[value.size()-1-i];if(c=='1')bits|=std::uint64_t(1)<<i;else if(c!='0'){e="Required original bitset invalid-character exception";return false;}}
  fast_travel17c_[tier]=bits;
 }return true;
}
}
