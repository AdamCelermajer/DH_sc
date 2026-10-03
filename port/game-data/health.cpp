#include "health.hpp"
#include <cstring>
namespace {
std::int32_t signed_bits(std::uint32_t v){std::int32_t r;std::memcpy(&r,&v,4);return r;}
}
extern "C" unsigned dh2_health_hit(dh2::data::HealthChange* out,const dh2::data::HealthRequest* request){
 using namespace dh2::data;
 if(!out||!request||request->facts&~4095u||request->low_health_armed>1||dh2_property_validate(request->properties))return 1;
 auto& view=*request->properties;const auto f=request->facts;
 HealthChange next{0,view.resolved[36],view.resolved[36],0,request->low_health_armed,0,-1,0};
 if(f&health_dead){next.skipped_dead=1;*out=next;return 0;}
 const bool permitted=(f&health_online)?(!(f&health_game_present)||request->session_state==0||request->session_state==5):((f&health_main_player_present)&&!(f&health_main_player_dead));
 if(permitted&&!((f&health_god_monster)&&(f&health_monster)))next.raw_add=signed_bits(0u-request->damage);
 dh2_property_add(&view,36,next.raw_add);
 if((f&health_one_shot)&&(!(f&health_player)||((f&health_child_present)&&!(f&health_child_player))))dh2_property_set(&view,36,0);
 if(view.resolved[36]<=0){dh2_property_set(&view,36,0);next.kill_requested=1;if(!(f&health_remote))next.lifecycle_write=3;}
 next.after=view.resolved[36];
 if(f&health_player){
  if(next.after<=view.resolved[38]/2){if(next.low_health_armed){next.low_health_armed=0;next.low_health_cue=1;}}
  else if(!next.low_health_armed){volatile float threshold=float(view.resolved[38])*.75f;if(float(next.after)>=threshold)next.low_health_armed=1;}
 }
 *out=next;return 0;
}
