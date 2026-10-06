#include "character_combat_sound_tables_v2.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::character {
bool CharacterCombatSoundTablesV2::load(const std::vector<std::uint8_t>& raw,std::string& error){
 try{std::size_t at=0;auto word=[&](){if(at>raw.size()||raw.size()-at<4)throw std::runtime_error("Original CharSounds word range");std::uint32_t v;std::memcpy(&v,raw.data()+at,4);at+=4;return v;};
  const auto count=word();if(count<3||count>65536)throw std::runtime_error("Original CharSounds count/fallback row");
  std::vector<Row> rows(count);for(auto& r:rows){for(auto& list:r.lists){const auto n=word();if(n>65536||n>(raw.size()-at)/4)throw std::runtime_error("Original CharSounds list range");list.reserve(n);for(unsigned i=0;i<n;++i){const auto bits=word();std::int32_t id;std::memcpy(&id,&bits,4);list.push_back(id);}}
   if(raw.size()-at<2)throw std::runtime_error("Original CharSounds flags range");r.flesh=raw[at++];r.metal=raw[at++];
  }rows_=std::move(rows);for(auto& r:rows_){r.view={{r.lists[0].data(),std::uint32_t(r.lists[0].size())},{r.lists[1].data(),std::uint32_t(r.lists[1].size())},{r.lists[2].data(),std::uint32_t(r.lists[2].size())},{r.lists[3].data(),std::uint32_t(r.lists[3].size())},r.flesh,r.metal};}
  error.clear();return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
const CombatSoundRowV1* CharacterCombatSoundTablesV2::get(std::int32_t id)const noexcept{
 if(id<0||std::size_t(id)>=rows_.size())id=2;return std::size_t(id)<rows_.size()?&rows_[id].view:nullptr;
}
}
