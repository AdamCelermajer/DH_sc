#include "player_manager_character_delivery_v70.hpp"
namespace dh2::player {
bool PlayerManagerCharacterDeliveryV70::deliver(const PlayerManagerRequestV1& q,PlayerManagerResponseV1&,std::string& error){
 if(q.operation!=PlayerManagerOperationV1::character_initialization){error="PlayerManager character delivery received another source operation";return false;}
 if(delivering_||!manager_.source_initialized_v59()||!q.player||!q.character_count6c4||q.character_count6c4!=manager_.character_count_field()){
  error="Required nonreentrant SAME PlayerManager AddCharacter receiver/count";return false;
 }
 PlayerInfoFieldsV1* actual{};
 if(!manager_.get_by_internal(q.id,false,actual,error))return false;
 if(actual!=q.player){error="AddCharacter request does not borrow SAME internal-map PlayerInfo";return false;}
 if(attempts_.find(actual)!=attempts_.end()){error="Source AddCharacter delivery cannot replay a retained attempt";return false;}
 if(!add_services_.provider_lease||!add_services_.invoke||!add_services_.network_fields||!add_services_.character){error="Required complete canonical AddCharacter native providers";return false;}
 auto owner=std::make_unique<PlayerAddCharacterOwnerV5>(manager_,add_services_);
 auto* attempt=owner.get();attempts_.emplace(actual,std::move(owner));
 delivering_=true;struct Exit{bool& v;~Exit(){v=false;}} exit{delivering_};
 return attempt->add(*actual,q.character_count6c4,q.id,error);
}
bool PlayerManagerCharacterDeliveryV70::forget_removed(PlayerInfoFieldsV1& record,std::string& error){
 if(delivering_||record.character660){error="Required original Character unpublication before AddCharacter attempt release";return false;}
 attempts_.erase(&record);return true;
}
}
