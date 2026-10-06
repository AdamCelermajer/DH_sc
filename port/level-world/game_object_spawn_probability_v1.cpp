#include "game_object_spawn_probability_v1.hpp"
namespace dh2::world {
bool game_object_meet_condition_v1(bool& v,std::string&){v=true;return true;}
bool game_object_check_spawn_probability_v1(GameObjectSpawnProbabilityBorrowV1& b,std::int32_t& roll,std::int32_t& probability,std::string& e){
 if(!b.cached_roll270||!b.probability274||!b.handle_as_player_character){e="Required canonical GameObject spawn fields/Handle-AsChar-IsPlayer";return false;}
 bool player=false;if(!b.handle_as_player_character(player,e))return false;
 if(player){*b.cached_roll270=-2;roll=-2;probability=*b.probability274;return true;}
 if(*b.cached_roll270!=-1){roll=*b.cached_roll270;probability=*b.probability274;return true;}
 if(!b.online_byte5||!b.network_id108){e="Required original online/source network ID for spawn roll";return false;}
 bool online=false;if(!b.online_byte5(online,e))return false;
 bool alternate=false;
 if(online&&*b.network_id108!=-1){if(!b.online_owner_fc){e="Required canonical online owner fieldfc";return false;}alternate=*b.online_owner_fc!=0;}
 auto* random=alternate?b.random1:b.random0;
 if(!random||dh2_loot_v2_random(random,99,&roll)){e="Required shared original Random channel for source spawn roll";return false;}
 *b.cached_roll270=roll;probability=*b.probability274;
 if(roll<probability){*b.cached_roll270=-2;roll=-2;return true;}
 if(!b.set_visible_false||!b.object_base_delete||!b.byte82||!b.mark_for_deletion){e="Required actual failed-spawn visibility/delete/mark lifecycle";return false;}
 if(!b.set_visible_false(e)||!b.object_base_delete(e))return false;
 *b.byte82=0;
 if(!b.mark_for_deletion(e))return false;
 roll=*b.cached_roll270;return true;
}
}
