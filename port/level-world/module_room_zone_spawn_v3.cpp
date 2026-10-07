#include "module_room_zone_spawn_v3.hpp"
#include <algorithm>
namespace dh2::world {
void ModuleRoomZoneSpawnV3::erased_after_source_release_v91(std::uintptr_t id){
 receivers_.erase(id);
 attempts_.erase(std::remove_if(attempts_.begin(),attempts_.end(),[id](const auto& attempt){return attempt->constructed_receiver().object.identity==id||attempt->handle().cached==id;}),attempts_.end());
}

bool ModuleRoomZoneSpawnV3::construct(void* c,const CanonicalFactoryEntryV1& f,CanonicalClassReceiverV1& r,std::string& e){
 auto& s=*static_cast<ModuleRoomZoneSpawnV3*>(c);const bool ok=s.factory_.construct(f,r,e);
 if(r.object.identity&&r.object.lease)s.receivers_[r.object.identity]=r;return ok;
}
bool ModuleRoomZoneSpawnV3::resolve(void* c,target_providers::Handle16& h,bool refresh,const CanonicalObjectBorrowV1*& o,std::string& e){
 auto& s=*static_cast<ModuleRoomZoneSpawnV3*>(c);if(!s.source_.resolve){e="Required SAME RoomZone Handle source resolver";return false;}
 return s.source_.resolve(s.source_.context,h,refresh,o,e);
}
bool ModuleRoomZoneSpawnV3::condition(void* c,const CanonicalObjectBorrowV1& o,bool force,std::string& e){
 auto& s=*static_cast<ModuleRoomZoneSpawnV3*>(c);if(!s.source_.test_enable_condition){e="Required actual RoomZone outer TestEnableCondition";return false;}
 return s.source_.test_enable_condition(s.source_.context,o,force,e);
}
bool ModuleRoomZoneSpawnV3::updatable(void* c,const CanonicalObjectBorrowV1& o,bool& value,std::string& e){
 auto& s=*static_cast<ModuleRoomZoneSpawnV3*>(c);auto* zone=s.factory_.find(o.identity);
 if(!zone){e="Required SAME RoomZone virtual38 receiver";return false;}value=zone->is_updatable();return true;
}
bool ModuleRoomZoneSpawnV3::pending(void* c,const CanonicalObjectBorrowV1& o,std::string& e){return static_cast<ModuleRoomZoneSpawnV3*>(c)->manager_.append_pending(o,e);}
bool ModuleRoomZoneSpawnV3::receiver(void* c,const CanonicalObjectBorrowV1& o,const CanonicalClassReceiverV1*& out,std::string& e){
 auto& s=*static_cast<ModuleRoomZoneSpawnV3*>(c);auto i=s.receivers_.find(o.identity);if(i==s.receivers_.end()){e="Required retained duplicate RoomZone dispatch";return false;}out=&i->second;return true;
}
bool ModuleRoomZoneSpawnV3::spawn(const std::string& name,target_providers::Handle16& out,std::uint32_t& type,std::string& e){
 CanonicalSpawnServicesV1 services{};services.context=this;services.construct=construct;services.resolve=resolve;
 services.test_enable_condition=condition;services.virtual38=updatable;services.append_pending=pending;services.receiver=receiver;
 auto attempt=std::make_unique<CanonicalSpawnAttemptV1>(manager_,properties_,services);auto* actual=attempt.get();attempts_.push_back(std::move(attempt));
 if(!actual->spawn("RoomZone",name.c_str(),false,true,e))return false;
 out=actual->handle();const CanonicalObjectBorrowV1* object{};if(!resolve(this,out,true,object,e))return false;
 type=object&&object->type_f4?*object->type_f4:0;return true;
}
bool ModuleRoomZoneSpawnV3::init(target_providers::Handle16& handle,const std::array<float,6>& box,std::uintptr_t module,std::string& e){
 const CanonicalObjectBorrowV1* object{};if(!resolve(this,handle,true,object,e))return false;
 if(!object||!object->type_f4||*object->type_f4!=11){e="Required source generated RoomZone type11";return false;}
 auto* zone=factory_.find(object->identity);if(!zone){e="Required SAME retained generated RoomZone";return false;}return zone->init_bounds(box,module,e);
}
}
