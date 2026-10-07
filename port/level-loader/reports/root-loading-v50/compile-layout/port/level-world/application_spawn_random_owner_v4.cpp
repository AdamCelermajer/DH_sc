#include "application_spawn_random_owner_v4.hpp"
namespace dh2::world {
bool canonical_check_spawn_probability_v4(CanonicalGameObjectBaseOwnerV1& base,const CanonicalSpawnApplicationServicesV4& s,std::int32_t& roll,std::int32_t& probability,std::string& e){
 if(!s.application_lease||!s.random){e="Required SAME application Random globals owner";return false;}
 auto* cached=base.integer(0x270);auto* prob=base.integer(0x274);
 if(!cached||!prob||!s.handle_as_player){e="Required actual Handle-AsChar-IsPlayer";return false;}
 bool player=false;if(!s.handle_as_player(player,e))return false;
 if(player){*cached=-2;roll=-2;probability=*prob;return true;}
 if(*cached!=-1){roll=*cached;probability=*prob;return true;}
 if(!s.online_byte5){e="Required actual online byte5";return false;}
 bool online=false;if(!s.online_byte5(online,e))return false;
 bool alternate=false;if(online&&*base.integer(0x108)!=-1){
  std::int32_t owner;if(!base.source_online_owner_fc_v4(owner,e))return false;alternate=owner!=0;
 }
 if(dh2_loot_v2_random(&s.random->channel(alternate),99,&roll)){e="Required original Random call";return false;}
 *cached=roll;probability=*prob;
 if(roll<probability){*cached=-2;roll=-2;return true;}
 if(!s.set_visible_false){e="Required actual SetVisible(false)";return false;}
 if(!s.set_visible_false(e))return false;
 base.source_delete_v4(); // byte82=2 then disabled81=1.
 *base.byte(0x82)=0;     // Original failed-spawn continuation, SAME field.
 if(!s.mark_for_deletion){e="Required actual ObjectManager MarkForDeletion";return false;}
 if(!s.mark_for_deletion(e))return false;
 roll=*cached;return true;
}
}
