#include "canonical_navigation.hpp"
namespace dh::foundation::frontend::flow {
namespace {
CharacterState* borrow(const std::shared_ptr<CanonicalNavigationOwner>& owner,CanonicalOperation operation,const char* callback,std::string& error){
    if(!owner){error=std::string(callback)+": canonical root owner unavailable";return nullptr;}
    if(!owner->available(operation)){error=std::string(callback)+": actual source owner unavailable";return nullptr;}
    auto* character=owner->live_character();
    if(!character)error=std::string(callback)+": canonical live character loan unavailable";
    return character;
}
bool outcome(bool ok,const char* callback,std::string& error){
    if(ok){error.clear();return true;}
    error=std::string(callback)+": "+(error.empty()?"root owner rejected operation":error);
    return false;
}
}
Services bind_canonical_navigation(std::shared_ptr<CanonicalNavigationOwner> owner){
    Services out;
    out.create_save=[owner](const std::string& name,const std::string& cls,int& slot,std::string& error){
        auto* same=borrow(owner,CanonicalOperation::create_slot,"NativeCreateSaveSlot",error);
        if(!same)return false;
        return outcome(owner->create_slot(*same,name,cls,slot,error),"NativeCreateSaveSlot",error);
    };
    out.assign_save=[owner](int slot,int player,std::string& error){
        auto* same=borrow(owner,CanonicalOperation::assign_slot,"NativeAssignSaveSlotToPlayer",error);
        if(!same)return false;
        return outcome(owner->assign_slot(*same,slot,player,error),"NativeAssignSaveSlotToPlayer",error);
    };
    out.start_game=[owner](int difficulty,std::string& error){
        auto* same=borrow(owner,CanonicalOperation::start_game,"NativeStartGame",error);
        if(!same)return false;
        return outcome(owner->start_game(*same,difficulty,error),"NativeStartGame",error);
    };
    return out;
}
bool capture_canonical_checkpoint(CanonicalNavigationOwner& owner,GameSave& destination,std::string& error){
    if(!owner.available(CanonicalOperation::capture_checkpoint)){error="Canonical GameSave capture owner unavailable";return false;}
    const auto* same=owner.live_character();
    if(!same){error="Canonical GameSave live character loan unavailable";return false;}
    // Stage only the existing GameSave value; publication on success prevents
    // destination mutation by a failed capture. The root owns actual source cells.
    GameSave captured;
    if(!outcome(owner.capture_checkpoint(*same,captured,error),"Canonical GameSave capture",error))return false;
    destination=std::move(captured);return true;
}
}
