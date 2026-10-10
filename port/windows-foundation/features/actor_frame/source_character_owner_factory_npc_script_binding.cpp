#include "source_character_owner_factory_npc_script_binding.hpp"

#include "../../../level-world/character_properties_temp_global_v62.hpp"

namespace dh2::windows_foundation {

bool SourceCharacterOwnerFactoryNpcScriptBindingV1::load(
 const character::ScriptAssetServicesV1& services,
 data::SkillTables::Borrow skills,std::string& error){
 if(!assets_.load(services,std::move(skills),error))return false;
 loaded_=true;error.clear();return true;
}

bool SourceCharacterOwnerFactoryNpcScriptBindingV1::bind_resources(
 world::CanonicalCharacterCandidateRecordV60& record,
 character::CharacterScriptSessionInput& input,
 const dh2_script_object_services* objects,std::string& error)const{
 if(!loaded_){error="Required same-cache CharacterScriptAssetsV1 snapshot";return false;}
 auto& actor=record.actor;
 if(!actor||!actor->object||!actor->machine||!actor->controller||!record.properties||!record.life||
    actor->source_name().empty()||actor->object->name!=actor->source_name()){
  error="Required same-record Character object/FSM/controller/properties/life";return false;
 }
 if(actor->object->identity==0||actor->object->properties!=record.properties||actor->object->life!=record.life){
  error="NPC script candidate aliases differ from the canonical Character record";return false;
 }
 if(!objects){error="Required same-world CharacterScriptObjects services";return false;}
 const auto* ai_tables=record.design.ai();
 if(!ai_tables){error="Required same CharacterGameDesign AI table";return false;}
 const auto* ai=data::ai_props(*ai_tables,record.properties->resolved[1]);
 if(!ai){error="Required resolved Character AI source row";return false;}

 auto resources=assets_.borrow();
 if(!resources){error="Required retained actual script-resource borrow";return false;}
 const data::Bytes common=resources.common();
 if(!common.data||common.size==0){error="Required actual source AI _commons.luac bytes";return false;}
 std::string external;
 if(!ai->script.empty()&&ai->script.front()!='_'){
  external="data/scripts/ai/"+ai->script;
  const auto suffix=external.find(".lua",std::string("data/scripts/ai/").size());
  if(suffix==std::string::npos)external+=".luac";
  else if(external.compare(suffix,5,".luac")!=0)external+='c';
 }
 const std::vector<std::uint8_t>* external_bytes=nullptr;
 if(!external.empty()){
  external_bytes=resources.find(external);
  if(!external_bytes||external_bytes->empty()){
   error="Required actual resolved NPC script cache entry: "+external;return false;
  }
 }
 std::vector<character::ScriptSessionFile> include_files;
 if(!resources.session_files(external,include_files,error))return false;

 const auto identity=actor->object->identity;
 const auto& temporary=character::character_properties_temp_global_v62();
 if((input.identity&&input.identity!=identity)||
    (!input.name.empty()&&input.name!=actor->source_name())||
    (input.properties&&input.properties!=record.properties)||
    (input.combat&&input.combat!=record.life)||
    (input.temporary&&input.temporary!=temporary)||
    (input.target&&input.target!=&actor->object->binding)||
    (input.state_machine&&input.state_machine!=&actor->machine->native_fsm())||
    (input.objects&&input.objects!=objects)){
  error="NPC script provider supplied a foreign same-record alias";return false;
 }

 input.identity=identity;
 input.name=actor->source_name();
 input.properties=record.properties;
 input.combat=record.life;
 input.temporary=temporary;
 input.position=actor->object->position;
 input.source_is_character=1;
 input.common=common;
 input.external=external_bytes
  ?data::Bytes{external_bytes->data(),external_bytes->size()}:data::Bytes{};
 input.include_files=std::move(include_files);
 input.target=&actor->object->binding;
 input.state_machine=&actor->machine->native_fsm();
 input.objects=objects;
 error.clear();return true;
}

}
