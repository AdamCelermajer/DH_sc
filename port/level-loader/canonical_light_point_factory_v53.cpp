#include "canonical_light_point_factory_v53.hpp"
#include <cstring>
#include <algorithm>
namespace dh2::loader {
bool CanonicalLightPointFactoryV53::construct(const world::CanonicalFactoryEntryV1& entry,const world::CanonicalSourceObjectRequestV1& source,world::CanonicalClassReceiverV1& out,std::string& e){
 if(!entry.name||std::strcmp(entry.name,"LightPoint")||entry.original_address!=0x34115c){e="Require actual LightPoint catalog factory34115c";return false;}
 if(!input_.world||!source.source_lease){e="Required SAME LightPoint World/source declaration owner";return false;}
 auto record=std::make_shared<CanonicalLightPointRecordV53>();record->declaration=source.source_lease;
 world::LightPointInitServicesV53 s;s.owner=input_.world;
 if(input_.services&&!input_.services(source,record,s,e))return false;
 if(!s.owner||s.owner.owner_before(input_.world)||input_.world.owner_before(s.owner)||record->owner){e="LightPoint provider replaced World lease/replayed source constructor";return false;}
 record->constructor_state=CanonicalConstructorStateV89::constructing;
 try{record->owner=std::make_unique<world::CanonicalLightPointV53>(input_.world,std::move(s));}catch(...){record->constructor_state=CanonicalConstructorStateV89::failed;throw;}
 record->constructor_state=CanonicalConstructorStateV89::completed;
 auto receiver=std::shared_ptr<world::CanonicalLightPointV53>(record,record->owner.get());
 auto candidate=world::canonical_class_receiver_v1(receiver);candidate.source_lease=source.source_lease;
 candidate.source_loading_fields_v95=[receiver](auto& out,auto& e){return receiver->source_loading_fields_v95(receiver,out,e);};
 candidate.source_is_updatable_v95=[receiver](bool& value,std::string& e){(void)receiver;value=true;e.clear();return true;};
 candidate.init_post=[receiver](std::string& error){return receiver->init_post(error);};
 candidate.is_game_object=[](bool& value,std::string& error){value=false;error.clear();return true;};
 records_.push_back(std::move(record));out=std::move(candidate);e.clear();return true;
}
void CanonicalLightPointFactoryV53::erased(std::uintptr_t identity){
 records_.erase(std::remove_if(records_.begin(),records_.end(),[identity](const auto& record){return record->owner&&record->owner->identity()==identity;}),records_.end());
}
}
