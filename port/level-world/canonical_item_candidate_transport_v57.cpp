#include "canonical_item_candidate_transport_v57.hpp"
#include <canonical_loading_receiver_v95.hpp>
#include <cstring>
namespace dh2::world {
CanonicalItemCandidateTransportV57::CanonicalItemCandidateTransportV57(character::WorldItemLiveOwnerV5& items,
 CanonicalObjectManagerV1& manager,CanonicalPropertyMapV1& properties,std::shared_ptr<void> owner)
 :items_(&items),manager_(&manager),properties_(&properties),owner_(std::move(owner)){}
bool CanonicalItemCandidateTransportV57::coherent(std::shared_ptr<void>& pin,std::string& e)const{
 pin=owner_.lock();
 if(!pin||!items_||!manager_||!properties_){e="Required actual retained application Item owner/candidate lifetime";return false;}
 const auto& factory=items_->factory();
 if(&factory.source_manager_v57()!=manager_||&factory.source_property_map_v57()!=properties_){
  e="Item factory belongs to a different canonical manager/PropertyMap";return false;
 }
 return true;
}
bool CanonicalItemCandidateTransportV57::construct(const CanonicalFactoryEntryV1& entry,const CanonicalSourceObjectRequestV1& request,
 CanonicalClassReceiverV1& out,std::string& e){
 std::shared_ptr<void> pin;
 if(!coherent(pin,e))return false;
 if(!request.source_lease||!entry.name||std::strcmp(entry.name,"Item")||entry.original_address!=0x340d38){
  e="Required retained source declaration and exact Item catalog constructor";return false;
 }
 CanonicalClassReceiverV1 receiver;
 if(!items_->factory().construct_receiver(entry,receiver,e)){out=std::move(receiver);return false;}
 if(!receiver.object.identity){out=std::move(receiver);return true;} // Genuine source allocation failure, no fake receiver.
 auto item=items_->factory().find(receiver.object.identity);
 if(!item||receiver.object.lease.get()!=item.get()||!receiver.properties||!receiver.init_post||
    receiver.object.shared_handle!=&item->base().shared_handle()){
  e="Required SAME newly constructed actual Item/Handle/property/virtual receiver";out=std::move(receiver);return false;
 }
 bind_gameobject_loading_fields_v95(item,true,receiver);
 receiver.source_init_final_v95=[owner=owner_,items=items_,id=item->base().identity()](std::string& e){auto pin=owner.lock();if(!pin){e="Actual Item owner expired before source InitFinal";return false;}bool eligible{};return items->init_final_v23(id,eligible,e);};
 receiver.source_lease=request.source_lease;
 const auto owner=owner_;
 receiver.is_game_object=[owner,item](bool& result,std::string& error){
  auto pin=owner.lock();if(!pin){error="Actual Item owner expired before inherited IsGameObject";return false;}
  (void)item;result=true;return true; // Inherited GameObject340054: mov r0,#1; bx lr.
 };
 receiver.set_position=[owner,item](const std::array<float,3>& position,bool destination,std::string& error){
  auto pin=owner.lock();if(!pin){error="Actual Item owner expired before source SetPosition";return false;}
  return item->source_set_position_v57(position.data(),destination,error);
 };
 out=std::move(receiver);return true;
}
}
