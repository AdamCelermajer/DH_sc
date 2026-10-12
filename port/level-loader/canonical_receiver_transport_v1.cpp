#include "canonical_receiver_transport_v1.hpp"
#include <exception>

namespace dh2::loader {
namespace {
bool missing(const char* service,std::string& error){
    error="required actual catalog receiver ";error+=service;return false;
}
}
CanonicalReceiverTransportV1::CanonicalReceiverTransportV1(
    world::CanonicalPropertyMapV1& map,CanonicalReceiverTransportServicesV1 source):
    properties_(map),source_(std::move(source)){}
world::CanonicalClassReceiverV1* CanonicalReceiverTransportV1::find(
    const world::CanonicalObjectBorrowV1& object,std::string& error){
    auto at=receivers_.find(object.identity);
    if(!object.identity||!object.lease||at==receivers_.end()){
        missing("retained identity/lease",error);return nullptr;
    }
    return &at->second;
}
bool CanonicalReceiverTransportV1::receiver(const world::CanonicalObjectBorrowV1& object,
    const world::CanonicalClassReceiverV1*& out,std::string& error){
    auto* actual=find(object,error);if(!actual)return false;out=actual;return true;
}
bool CanonicalReceiverTransportV1::construct(void* context,const world::CanonicalFactoryEntryV1& entry,
    const world::CanonicalSourceObjectRequestV1& request,world::CanonicalObjectBorrowV1& out,std::string& error){
    auto& self=*static_cast<CanonicalReceiverTransportV1*>(context);
    if(!self.source_.owner||!self.source_.construct){
        error="required actual catalog class construction: ";error+=entry.name?entry.name:"<missing>";return false;
    }
    world::CanonicalClassReceiverV1 result;
    bool ok=false;
    try{ok=self.source_.construct(self.source_.context,entry,request,result,error);}
    catch(const std::exception& e){error=e.what();}
    result.source_lease=request.source_lease;
    if(result.object.identity&&result.object.lease){
        if(self.receivers_.count(result.object.identity))return missing("new constructor reused a live identity",error);
        auto stored=self.receivers_.emplace(result.object.identity,std::move(result));
        out=stored.first->second.object;
        // A genuine constructor's mutation prefix survives failed continuation.
        if(!ok)return false;
        if(!out.shared_handle||!out.class_name20||!stored.first->second.properties)
            return missing("Handle/class-name/property producer",error);
        return true;
    }
    if(ok)missing("constructed identity/lease",error);
    return false;
}
bool CanonicalReceiverTransportV1::init_properties(void* context,const world::CanonicalObjectBorrowV1& object,std::string& error){
    auto& self=*static_cast<CanonicalReceiverTransportV1*>(context);auto* actual=self.find(object,error);if(!actual)return false;
    if(!actual->properties)return missing("typed property producer",error);
    const auto lease=actual->object.lease;auto producer=actual->properties;
    auto actor=producer();return self.properties_.init_properties(actor,error);
}
bool CanonicalReceiverTransportV1::set_template(void* context,const world::CanonicalObjectBorrowV1& object,const char* name,std::string& error){
    auto& self=*static_cast<CanonicalReceiverTransportV1*>(context);auto* actual=self.find(object,error);if(!actual)return false;
    if(!actual->properties)return missing("typed property producer",error);
    const auto lease=actual->object.lease;auto producer=actual->properties;
    auto actor=producer();return self.properties_.set_template(actor,name,error);
}
bool CanonicalReceiverTransportV1::defaults(void* context,const world::CanonicalObjectBorrowV1& object,std::string& error){
    auto& self=*static_cast<CanonicalReceiverTransportV1*>(context);auto* actual=self.find(object,error);if(!actual)return false;
    if(!actual->properties)return missing("typed property producer",error);
    const auto lease=actual->object.lease;auto producer=actual->properties;
    auto actor=producer();return self.properties_.load_defaults(actor,error);
}
bool CanonicalReceiverTransportV1::overrides(void* context,const world::CanonicalObjectBorrowV1& object,
    const world::CanonicalSourceObjectRequestV1& request,std::string& error){
    auto& self=*static_cast<CanonicalReceiverTransportV1*>(context);auto* actual=self.find(object,error);if(!actual)return false;
    if(!actual->properties)return missing("typed property producer",error);
    const auto lease=actual->object.lease;auto producer=actual->properties;
    auto actor=producer();return self.properties_.load_overrides(actor,request,error);
}
bool CanonicalReceiverTransportV1::init_post(void* context,const world::CanonicalObjectBorrowV1& object,std::string& error){
    auto* actual=static_cast<CanonicalReceiverTransportV1*>(context)->find(object,error);if(!actual)return false;
    if(!actual->init_post)return missing("InitPost continuation",error);
    const auto lease=actual->object.lease;auto call=actual->init_post;return call(error);
}
bool CanonicalReceiverTransportV1::is_game_object(void* context,const world::CanonicalObjectBorrowV1& object,bool& result,std::string& error){
    auto* actual=static_cast<CanonicalReceiverTransportV1*>(context)->find(object,error);if(!actual)return false;
    if(!actual->is_game_object)return missing("IsGameObject continuation",error);
    const auto lease=actual->object.lease;auto call=actual->is_game_object;return call(result,error);
}
bool CanonicalReceiverTransportV1::position(void* context,const world::CanonicalObjectBorrowV1& object,std::array<float,3>& result,std::string& error){
    auto* actual=static_cast<CanonicalReceiverTransportV1*>(context)->find(object,error);if(!actual)return false;
    if(!actual->position)return missing("position producer",error);
    const auto lease=actual->object.lease;auto call=actual->position;return call(result,error);
}
bool CanonicalReceiverTransportV1::set_position(void* context,const world::CanonicalObjectBorrowV1& object,const std::array<float,3>& value,bool update,std::string& error){
    auto* actual=static_cast<CanonicalReceiverTransportV1*>(context)->find(object,error);if(!actual)return false;
    if(!actual->set_position)return missing("SetPosition continuation",error);
    const auto lease=actual->object.lease;auto call=actual->set_position;return call(value,update,error);
}
bool CanonicalReceiverTransportV1::unknown(void* context,const char* name,std::string& error){
    auto& self=*static_cast<CanonicalReceiverTransportV1*>(context);
    if(!self.source_.owner||!self.source_.unknown_type_debug)return missing("unknown-type Debug provider",error);
    return self.source_.unknown_type_debug(self.source_.context,name,error);
}
world::CanonicalClassServicesV1 CanonicalReceiverTransportV1::services()noexcept{
    return {this,construct,init_properties,set_template,defaults,overrides,init_post,is_game_object,position,set_position,unknown};
}
}
