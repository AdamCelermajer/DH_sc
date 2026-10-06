#include "canonical_character_family_v4.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::world {
namespace {
class CharacterReceiver final:public character::RetainedCharacterActorV1 {
public:explicit CharacterReceiver(std::shared_ptr<void> world):RetainedCharacterActorV1(reinterpret_cast<std::uintptr_t>(this),std::move(world),"Character",character::RetainedCharacterConstructionV7::fresh_canonical){}
};
const data::AiProps* ai(CanonicalCharacterRecordV4& r){
 auto* table=r.design.ai();if(!table)return nullptr;auto index=r.properties->resolved[1];
 if(index<0||static_cast<std::size_t>(index)>=table->rows.size())index=8;
 return static_cast<std::size_t>(index)<table->rows.size()?&table->rows[index]:nullptr;
}
}
bool CanonicalCharacterFamilyFactoryV4::construct(const CanonicalFactoryEntryV1& entry,const CanonicalSourceObjectRequestV1& source,CanonicalClassReceiverV1& out,std::string& e){
 if(!entry.name||std::strcmp(entry.name,"Character")||entry.original_address!=0x340800){e="Required original Character factory340800";return false;}
 if(!services_.world||!services_.design||!services_.design->ready()||!services_.loot_tables||!services_.loot_tables->borrow()||!services_.loot_random||!source.attribute){e="Required SAME World/design/loot source Character constructor graph";return false;}
 const char* name=source.attribute(source.source_context,source.element,"name");if(!name){e="Required actual Character placement name";return false;}
 auto record=std::make_shared<CanonicalCharacterRecordV4>();record->services=services_;record->design=services_.design->borrow();
 record->properties=std::make_shared<data::PropertyState>();record->life=std::make_shared<data::CombatActorState>();
 // CharPropertiesC1 ResetAllProperties, source rule-default sheets; loading a
 // placement's Character row belongs to later InitPost LoadBaseProperties.
 data::reset_properties(*record->design.rules(),*record->properties);record->view=data::property_view(*record->design.rules(),*record->properties);
 record->actor=std::make_shared<CharacterReceiver>(services_.world);
 auto canonical=record->actor->canonical(record->actor);
 record->inventory=std::make_unique<character::NpcInventoryOwnerV1>(canonical.identity,services_.loot_tables->borrow(),*services_.loot_random,record->properties);
 auto object=std::make_shared<character::ScriptCharacterObject>(canonical.identity,name,record->properties,record->life,std::array<float,3>{});
 character::WorldNpcStateServicesV1 state;
 if(services_.state_services&&!services_.state_services(*record,state,e))return false;
 if(!record->actor->construct_fields(object,state)){e=record->actor->error();return false;}
 if(!record->actor->bind_initialization_properties(record->view,e))return false;
 CanonicalClassReceiverV1 result;result.object=record->actor->canonical(record);result.source_lease=source.source_lease;
 result.properties=[record]{return record->actor->properties();};result.init_post=[record](std::string& error){return record->initialize(error);};
 result.is_game_object=[](bool& v,std::string&){v=true;return true;};
 result.position=[record](std::array<float,3>& p,std::string& error){return record->actor->source_position(p,error);};
 result.set_position=[record](const std::array<float,3>& p,bool destination,std::string& error){if(!record->services.set_position){error="Required whole same Character SetPosition/PF/destination";return false;}return record->services.set_position(*record,p,destination,error);};
 records_.emplace(canonical.identity,record);out=std::move(result);return true;
}
std::shared_ptr<CanonicalCharacterRecordV4> CanonicalCharacterFamilyFactoryV4::find(std::uintptr_t identity)const{auto i=records_.find(identity);return i==records_.end()?nullptr:i->second;}
bool CanonicalCharacterRecordV4::initialize(std::string& e){
 if(init_attempted){if(!init){e="Failed Character family field binding cannot replay";return false;}if(!init->initialize()){e=init->error();return false;}return true;}init_attempted=true;
 if(!actor->init_post_fields(fields,e))return false;
 init=std::make_unique<character::CharacterNpcInitPostOwnerV1>(fields,character::NpcInitPostServicesV1{this,invoke});
 if(!init->initialize()){e=init->error();return false;}return true;
}
bool CanonicalCharacterRecordV4::invoke(void* context,const character::NpcInitPostRequestV1& q,character::NpcInitPostResponseV1& response,std::string& e){
 auto& r=*static_cast<CanonicalCharacterRecordV4*>(context);if(q.subject!=r.actor->shared_handle().cached){e="Required SAME retained Character InitPost subject";return false;}
 if(q.source_entry==0x3b3d38){
  if(*r.fields.properties_id13c8!=-1){response.value=*r.fields.properties_id13c8;return true;}
  const auto* templ=r.actor->source_string(0x1398);const auto* desc=r.actor->source_string(0x13b0);
  if(templ&&!templ->empty()){if(r.services.remaining_init)return r.services.remaining_init(r,q,response,e);e="Required actual CharacterPropertiesTemplate table/shared Random selector";return false;}
  const auto& names=r.design.characters()->names;auto at=desc?std::find(names.begin(),names.end(),*desc):names.end();
  const auto index=at==names.end()?-1:static_cast<std::int32_t>(at-names.begin());const auto word=static_cast<std::uint16_t>(index);std::memcpy(r.fields.properties_id13c8,&word,2);response.value=*r.fields.properties_id13c8;return true;
 }
 if(q.source_entry==0x3df2a4){const auto index=static_cast<std::int32_t>(q.argument0);const auto& rows=r.design.characters()->rows;if(index>=0&&static_cast<std::size_t>(index)<rows.size())r.properties->base=rows[index];return true;}
 if(q.source_entry==0x3e0810){return data::recalc_properties_with_class(*r.design.classes(),*r.design.rules(),*r.properties,e);}
 if(q.source_entry==0x3a2fec){response.ai=ai(r);if(!response.ai){e="Required original AI row/fallback8";return false;}return true;}
 if(q.source_entry==0x3a49f0){const auto* row=ai(r);if(!row){e="Required actual Character GetCharType";return false;}response.value=row->type==1;return true;}
 if(q.source_entry==0x3a54d4){const auto* row=ai(r);if(!row||row->type==1||row->type==3){if(r.services.remaining_init)return r.services.remaining_init(r,q,response,e);e="Required player/faery original model-name branch";return false;}
  const auto index=r.properties->resolved[3];if(index<0){response.text=nullptr;return true;}if(!r.services.models){e="Required same original character_models_dictionary";return false;}
  response.text=static_cast<std::size_t>(index)<r.services.models->values.size()?r.services.models->values[index].c_str():nullptr;return true;
 }
 if(q.source_entry==0x3bc4d0){return true;} // Source Character14e8 is NULL in fresh NPC C1; SG_Load returns at NULL.
 if(r.services.remaining_init)return r.services.remaining_init(r,q,response,e);
 e="Required actual Character family InitPost provider "+std::to_string(q.source_entry);return false;
}
}
