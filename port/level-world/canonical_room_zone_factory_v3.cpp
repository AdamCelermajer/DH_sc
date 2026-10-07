#include "canonical_room_zone_factory_v3.hpp"
#include <canonical_loading_receiver_v95.hpp>
#include <algorithm>
#include <cstring>
namespace dh2::world {
bool CanonicalRoomZoneFactoryV3::construct(const CanonicalFactoryEntryV1& entry,CanonicalClassReceiverV1& out,std::string& error){
 error.clear();if(entry.original_address!=0x340f74u||!entry.name||std::strcmp(entry.name,"RoomZone")!=0){error="Required genuine RoomZone source factory entry";return false;}
 if(!construction_.world||!construction_.services){error="Required SAME World RoomZone construction services";return false;}
 auto record=std::make_shared<CanonicalRoomZoneRecordV3>();RoomZoneServicesV3 services;
 if(!construction_.services(record,services,error))return false;
 if(!construction_.admit_before_c1_v91||!construction_.admit_after_c1_v91){error="Required actual generated RoomZone release/transport admission";return false;}
 if(!construction_.admit_before_c1_v91(record,entry,error))return false;
 record->constructor_started_v91=true;
 try{record->receiver=std::make_unique<CanonicalRoomZoneV3>(construction_.world,record->runtime,services);}
 catch(...){record->constructor_failed_v91=true;error="Actual RoomZone C1 threw; qualified prefix retained";return false;}
 record->constructor_completed_v91=true;
 auto& receiver=*record->receiver;receiver.base().class_name20()=entry.name;
 CanonicalClassReceiverV1 result;result.object=receiver.canonical(record);
 bind_gameobject_loading_v95(std::shared_ptr<CanonicalRoomZoneV3>(record,record->receiver.get()),true,result);
 result.properties=[record]{return record->receiver->properties();};
 result.init_post=[record](std::string& e){return record->receiver->init_post(e);};
 result.is_game_object=[](bool& value,std::string& e){e.clear();value=true;return true;};
 result.position=[record](std::array<float,3>& value,std::string& e){e.clear();std::copy_n(record->runtime.subobjects.position,3,value.begin());return true;};
 result.set_position=[record,position=services.position](const std::array<float,3>& value,bool destination,std::string& e){return game_object_set_position_v2(record->receiver->base(),value.data(),destination,position,e);};
 records_.emplace(result.object.identity,record);out=std::move(result);return construction_.admit_after_c1_v91(record,out,error);
}
CanonicalRoomZoneV3* CanonicalRoomZoneFactoryV3::find(std::uintptr_t id)noexcept{auto i=records_.find(id);return i==records_.end()?nullptr:i->second->receiver.get();}
}
