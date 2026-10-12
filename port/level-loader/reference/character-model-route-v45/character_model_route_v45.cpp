#include "character_model_route_v45.hpp"
namespace dh2::loader {
bool CharacterModelRouteV45::respond(world::CanonicalCharacterRecordV4& record,
 const character::NpcInitPostRequestV1& request,character::NpcInitPostResponseV1& response,std::string& error)const{
 auto factory=actual_factory.lock();auto same=factory?factory->find(request.subject):nullptr;
 if(request.source_entry!=0x3a54d4||request.argument0||request.argument1||request.payload||request.text||!same||same.get()!=&record||!record.actor||record.actor->shared_handle().cached!=request.subject){error="Required SAME canonical Character factory/model request ownership and source argument shape";return false;}
 // Existing retained NPC owner's current request witnesses the completed
 // ordered SafeProps -> LoadBaseProperties -> Recalc(true) source prefix.
 // No new stage bit, cache shadow, constructor projection or replay policy.
 if(!record.init_attempted||!record.init||record.init->last_entry()!=0x3a54d4||!record.init->error().empty()||record.fields.identity!=request.subject||!record.fields.called1394||!*record.fields.called1394){error="Required reached ordered Character NPC InitPost LoadBase/Recalc/model request";return false;}
 const auto* dictionary=record.services.models;
 if(!dictionary||!actual_models.dictionary_lease||actual_models.dictionary_lease.get()!=static_cast<const void*>(dictionary)||actual_models.files!=&dictionary->values){error="Required SAME actual pinned Character model dictionary";return false;}
 auto alias=std::shared_ptr<const void>(same,record.actor.get());character::CharacterModelFieldsV38 fields;
 if(!record.actor->model_name_fields_v38(alias,fields,error))return false;
 if(!record.properties||!record.actor->object||record.actor->object->properties.get()!=record.properties.get()||fields.properties!=record.fields.properties||fields.properties!=&record.view||fields.properties->base!=record.properties->base.data()||fields.properties->saved!=record.properties->saved.data()||fields.properties->gear!=record.properties->gear.data()||fields.properties->resolved!=record.properties->resolved.data()||fields.properties13c8!=record.fields.properties_id13c8){error="Required SAME actual initialized Character property/cache backing";return false;}
 character::CharacterModelServicesV38 services;
 if(main_services&&!main_services(record,services,error))return false;
 character::CharacterModelResultV38 result;
 if(!character::character_model_name_v38(fields,actual_models,services,result,error))return false;
 response.text=result.file?result.file->c_str():nullptr;error.clear();return true;
}
}
