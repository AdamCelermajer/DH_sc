#include "source_owner_contract.hpp"

namespace dh::foundation::platform_input {
namespace {bool fail(std::string& e,const char* text){if(e.empty())e=text;return false;}}
bool source_level_input(const SourceOwnerBorrow& owner,bool& enabled,std::string& error) {
    error.clear();enabled=false;
    if(!owner.receiver_lease||!owner.current_level)return fail(error,"Required actual Application CurrentLevel provider");
    dh2::ui::MenuLevelBorrowV58 level;
    if(!owner.current_level(level,error))return false;
    if(!level.identity) {enabled=true;return true;}
    if(!level.actual_owner||!level.byte198)return fail(error,"Required same live Level byte198 and lease");
    enabled=*level.byte198!=0;return true;
}
bool InputOwnerEpoch::validate(const SourceOwnerBorrow& owner,std::uint64_t expected,std::string& error)const {
    error.clear();
    if(!owner.receiver_lease||owner.epoch!=epoch_||!expected||owner.actor_id!=expected||!owner.character||!owner.controller)
        return fail(error,"Input source borrow expired or changed actor/controller publication");
    return true;
}
bool InputOwnerEpoch::before_replace(SemanticInput& input,const std::function<bool(const Frame&,std::string&)>& release,std::string& error) {
    error.clear();
    if(!pending_release_) {input.lose_focus();pending_release_=input.take_frame();}
    if(!release||!release(*pending_release_,error))return fail(error,"Old input owner release failed before world replacement");
    pending_release_.reset();
    ++epoch_;if(!epoch_)++epoch_;return true;
}
}

