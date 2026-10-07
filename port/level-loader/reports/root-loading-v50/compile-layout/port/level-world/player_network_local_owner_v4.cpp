#include "player_network_local_owner_v4.hpp"
namespace dh2::player {
bool MatchingLocalSelectionOwnerV4::get(std::shared_ptr<MatchingLocalIdentityOwnerV4>& out,std::string& e){
 if(local_){out=local_;return true;}
 // Get800fb0..800fc8 stores1 before invoking Local C1 when static mode0.
 if(!mode_)mode_=1;
 if(mode_!=1){e="Required original nonlocal Matching constructor for source mode "+std::to_string(mode_);return false;}
 auto candidate=std::make_shared<MatchingLocalIdentityOwnerV4>();
 candidate->source_reset_identity(); // Local C1 invokes Reset807e28.
 local_=std::move(candidate);out=local_;return true;
}
bool MatchingLocalSelectionOwnerV4::source_unallocated_mode_store(std::uint32_t mode,std::string& e){
 if(local_){e="Actual Matching replacement/destructor required before changing selected mode";return false;}
 mode_=mode;return true;
}
bool PlayerNetworkLocalOwnerV4::construct(PlayerInfoFieldsV1& record,std::shared_ptr<void> lease,std::string& e){
 if(!lease){e="Required same PlayerInfo parent constructor lease";return false;}
 auto actual=std::make_shared<PlayerInfoNetworkIdentityV4>();actual->receiver=&record;actual->parent_lease=std::move(lease);
 // CNet C1 initializes integer member+180/value1a0 then Reset80f27c
 // writes−1. Publish this narrow backing only after that source field store.
 actual->owner1a0=-1;records_[&record]=std::move(actual);return true;
}
std::shared_ptr<PlayerInfoNetworkIdentityV4> PlayerNetworkLocalOwnerV4::borrow(PlayerInfoFieldsV1& record)const{
 auto at=records_.find(&record);return at==records_.end()?nullptr:at->second;
}
bool PlayerNetworkLocalOwnerV4::is_local(PlayerInfoFieldsV1& record,bool& out,std::string& e){
 auto player=borrow(record);if(!player||player->receiver!=&record){e="Required same PlayerInfo CNet owner1a0 constructor successor";return false;}
 std::shared_ptr<MatchingLocalIdentityOwnerV4> matching;
 if(!matching_.get(matching,e))return false;
 // Source80f1ec loads owner AFTER actual matching.IsServer delivery.
 const bool server=matching->is_server();const auto owner=player->owner1a0;
 if(server&&owner<0){out=true;return true;}
 out=owner==matching->member();return true;
}
}
