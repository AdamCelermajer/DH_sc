#include "player_info_skill_buffers_v26.hpp"
#include <cstring>
namespace dh2::player {
bool PlayerInfoSkillBuffersV26::construct(PlayerInfoFieldsV1& p,
 PlayerNetworkLocalOwnerV4& network,std::string& error){
 error.clear();if(constructed_){error="PlayerInfo skill constructor prefix already produced";return false;}
 auto same=network.borrow(p);
 if(!same||same->receiver!=&p||!same->parent_lease){error="Required same CNetPlayerInfo constructor prefix";return false;}
 // C1's final call374a98→Reset373bdc, loops373ca8..b4 and373cbc..e0.
 // These are signed -1 bytes, not learned levels or zero-filled fixtures.
 slots_.fill(-1);levels_.fill(-1);receiver_=&p;network_=std::move(same);
 constructed_=true;return true;
}
namespace {
template<std::size_t N> bool get(const PlayerInfoFieldsV1* receiver,
 const std::shared_ptr<PlayerInfoNetworkIdentityV4>& network,bool constructed,
 const std::array<std::int8_t,N>& bytes,PlayerInfoFieldsV1& requested,
 void* destination,std::size_t capacity,std::int32_t& copied,std::string& error){
 error.clear();copied=0;
 if(!constructed||receiver!=&requested||!network||network->receiver!=receiver||!network->parent_lease){
  error="Required same retained PlayerInfo skill-buffer owner";return false;}
 // Original GetBuffer ignores the caller's capacity and memcpy's source size.
 // Native boundary validates that capacity before the identical copy.
 if(!destination||capacity<N){error="PlayerInfo GetBuffer destination is smaller than actual source buffer";return false;}
 std::memcpy(destination,bytes.data(),N);copied=std::int32_t(N);return true;
}
}
bool PlayerInfoSkillBuffersV26::get_slots(PlayerInfoFieldsV1& p,void* out,
 std::size_t n,std::int32_t& copied,std::string& error)const{
 return get(receiver_,network_,constructed_,slots_,p,out,n,copied,error);
}
bool PlayerInfoSkillBuffersV26::get_levels(PlayerInfoFieldsV1& p,void* out,
 std::size_t n,std::int32_t& copied,std::string& error)const{
 return get(receiver_,network_,constructed_,levels_,p,out,n,copied,error);
}
}
