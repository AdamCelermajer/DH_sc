#include "character_props_id_owner_v1.hpp"
#include <cstring>
namespace dh2::character {
namespace {
std::int16_t narrow(std::int32_t value){std::uint16_t bits=std::uint16_t(value);std::int16_t out;std::memcpy(&out,&bits,2);return out;}
std::int32_t find(const data::CharacterTable& table,const std::string& name){for(std::size_t i=0;i<table.names.size();++i)if(!std::strcmp(name.c_str(),table.names[i].c_str()))return std::int32_t(i);return -1;}
}
bool character_safe_props_id_v1(std::int16_t& cache,const std::string& array,const std::string& name,const data::CharacterTable& table,data::PlayerSavegameV1* save,data::LootRandom8V2& random,const CharacterPropsIdServicesV1& s,std::int32_t& out,std::string& e){
 e.clear();if(cache!=-1){out=cache;return true;}
 if(!s.is_player){e="Required same Character virtual IsPlayer28";return false;}bool player;if(!s.is_player(s.context,player,e))return false;
 if(player){
  if(save){if(!s.load_save){e="Required original SAME Save SG_Load(1) before cached class read";return false;}if(!s.load_save(s.context,*save,1,e))return false;cache=narrow(save->class_id());}
  // Original NULL Save SG_Load is empty and getter returns -1.
  if(cache==-1)cache=narrow(find(table,"KnightPlayerBase"));
  if(save)save->set_player_class(cache);out=cache;return true;
 }
 if(!array.empty()){
  if(!s.preset){e="Required actual GetCharPropsArray receiver/table";return false;}
  const std::int16_t* rows{};std::uint32_t count{};if(!s.preset(s.context,array,rows,count,e))return false;
  if(!count){out=cache;return true;}if(!rows||count>std::uint32_t(INT32_MAX)){e="Required valid original CharPropsArray borrowed span";return false;}
  std::int32_t draw;if(dh2_loot_v2_random(&random,std::int32_t(count),&draw)){e="Required SAME source CharPropsArray Random";return false;}
  if(std::uint32_t(draw)>=count){e="Required original CharPropsArray index assertion";return false;}cache=rows[draw];out=cache;return true;
 }
 if(!name.empty())cache=narrow(find(table,name));out=cache;return true;
}
}
