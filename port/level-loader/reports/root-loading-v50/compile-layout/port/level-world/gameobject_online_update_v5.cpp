#include "gameobject_online_update_v5.hpp"
namespace dh2::world {
bool gameobject_require_online_update_v5(CanonicalGameObjectBaseOwnerV1& base,const GameObjectOnlineUpdateServicesV5& s,std::string& e){
 bool online{};if(!s.online_byte5||!s.online_byte5(s.context,online,e)){if(e.empty())e="Required actual GetOnline.byte5";return false;}if(!online)return true;
 auto* net=base.pointer(0x100);if(!net){e="Required SAME GameObject NetStruct100 field";return false;}if(!*net)return true;
 // Source performs GetOnline AGAIN before IsHosting; preserve this query.
 if(!s.online_byte5(s.context,online,e))return false;
 bool host{};if(!s.hosting||!s.hosting(s.context,host,e)){if(e.empty())e="Required actual online IsHosting";return false;}
 if(!host){bool not_owned{};if(!s.not_owned_virtual54||!s.not_owned_virtual54(s.context,base.identity(),not_owned,e)){if(e.empty())e="Required actual GameObject network ownership virtual54";return false;}if(not_owned)return true;}
 return base.store_byte(0x119,1,e);
}
}
