#include "npc_profile_binding.hpp"
namespace dh::foundation::encounters {
bool select_npc_profile(const dh2::data::CharacterTable& characters,
 const dh2::data::CharacterTemplateTableV78& templates,const std::string& array,
 const std::string& name,std::int16_t& cache,std::int16_t& template_cache,
 dh2::data::LootRandom8V2& random,NpcProfileSelection& out,std::string& error){
 struct Context {const dh2::data::CharacterTemplateTableV78* table;std::int16_t* cache;};
 Context context{&templates,&template_cache};
 dh2::character::CharacterPropsIdServicesV1 services;
 services.context=&context;
 services.is_player=[](void*,bool& player,std::string&){player=false;return true;};
 services.preset=[](void* p,const std::string& name,const std::int16_t*& rows,
                    std::uint32_t& count,std::string& e){
  auto& c=*static_cast<Context*>(p);std::int32_t id;
  if(!c.table->safe_template_id(name,*c.cache,id,e))return false;
  return c.table->preset(name,rows,count,e);
 };
 std::int32_t row=-1;
 if(!dh2::character::character_safe_props_id_v1(cache,array,name,characters,
       nullptr,random,services,row,error))return false;
 if(row<0||static_cast<std::size_t>(row)>=characters.names.size()||
    static_cast<std::size_t>(row)>=characters.rows.size()){
  error="Original NPC selected no valid CharacterTable row; source cache retained";return false;
 }
 out={row,characters.names[static_cast<std::size_t>(row)]};error.clear();return true;
}
}
