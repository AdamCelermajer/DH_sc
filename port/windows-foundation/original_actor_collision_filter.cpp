#include "original_actor_collision_filter.hpp"
#include <exception>
namespace dh::foundation {
bool OriginalActorCollisionFilter::bind(ActorId id,OriginalCollisionReceiverKind kind,
    OriginalCollisionOwnerServices services,std::string& error){
    if(!id||!services.enabled80||(kind==OriginalCollisionReceiverKind::po_character&&!services.character_handle)||
       (kind!=OriginalCollisionReceiverKind::physical_object&&kind!=OriginalCollisionReceiverKind::po_character)){
        error="Required original physical receiver/handle/enabled services";return false;}
    owners_.insert_or_assign(id,Owner{kind,std::move(services)});error.clear();return true;
}
void OriginalActorCollisionFilter::remove(ActorId id){owners_.erase(id);}
bool OriginalActorCollisionFilter::permits_category(ActorId id,std::uint16_t category,
    bool& allowed,std::string& error)const{try{
    const auto found=owners_.find(id);if(found==owners_.end()){error="Original collision receiver unbound";return false;}
    const auto& owner=found->second;bool result=true;
    if(owner.kind==OriginalCollisionReceiverKind::po_character){
        bool character=false;if(!owner.services.character_handle(id,character,error))return false;
        if(character){
            std::int32_t state=0;
            if(!owner.services.original_state){error="Required actual same-owner Character FSM getter";return false;}
            if(!owner.services.original_state(id,state,error))return false;
            if(state==0||(state==10&&(category&3u)==0))result=false;
        }
    }
    allowed=result;error.clear();return true;
}catch(const std::exception& ex){error=ex.what();return false;}}
bool OriginalActorCollisionFilter::test_one(ActorId id,ActorId peer_id,
    const dh2::navigation::ContactFilter& own,const dh2::navigation::ContactFilter& peer,
    bool& allowed,std::string& error)const{try{
    if(own.present!=1||peer.present!=1){error="Required actual native shape filter snapshots";return false;}
    bool prefix=false;if(!permits_category(id,peer.category,prefix,error))return false;
    if(!prefix){allowed=false;return true;}
    const auto& owner=owners_.at(id);std::uint8_t enabled=0;
    if(!owner.services.enabled80(id,enabled,error))return false;
    if(!enabled){allowed=false;error.clear();return true;}
    if(peer_id){
        const auto other=owners_.find(peer_id);if(other==owners_.end()){error="Original peer enabled owner unbound";return false;}
        if(!other->second.services.enabled80(peer_id,enabled,error))return false;
        if(!enabled){allowed=false;error.clear();return true;}
    }
    // Source signed group policy takes precedence over both masks.
    const bool result=own.group!=0&&own.group==peer.group?own.group>0:
        (peer.category&own.mask)!=0&&(peer.mask&own.category)!=0;
    allowed=result;error.clear();return true;
}catch(const std::exception& ex){error=ex.what();return false;}}
bool OriginalActorCollisionFilter::test_pair(ActorId first,ActorId second,
    const dh2::navigation::ContactFilter& a,const dh2::navigation::ContactFilter& b,
    bool& allowed,std::string& error)const{
    bool one=false,two=false;
    if(!test_one(first,second,a,b,one,error))return false;
    if(!test_one(second,first,b,a,two,error))return false;
    allowed=one&&two;error.clear();return true;
}
} // namespace dh::foundation
