#include "gameplay_camera_damping_v1.hpp"
extern "C" void dh2_gameplay_camera_damping_v1(float v[3],float target[3],const float first[3],const float second[3],const float* ratio,std::uint32_t dt){
 // Separate operations preserve source softfloat rounding/no FMA.
 for(unsigned n=0;n<3;++n){const float desired=v[n]+target[n];const float delta=desired-first[n];v[n]=delta*(*ratio);}
 const float seconds=static_cast<float>(dt)*0.001f;
 for(unsigned n=0;n<3;++n){const float move=seconds*v[n];target[n]=move+second[n];}
}
namespace dh2::camera {
bool handle_damping_v1(DampingStateV1& s,bool root,float target[3],const DampingServicesV1& services,bool& applied,std::string& error){
 applied=false;if(!s.enabled||!root)return true;
 if(!target||!services.root_position){error="Required actual camera root position";return false;}
 float first[3],second[3];if(!services.root_position(first,error))return false;
 // Source velocity mutation precedes second root query and Application::GetDt.
 for(unsigned n=0;n<3;++n){const float desired=s.velocity[n]+target[n];const float delta=desired-first[n];s.velocity[n]=delta*s.ratio;}
 if(!services.root_position(second,error))return false;
 std::uint32_t dt;if(!services.application_dt){error="Required actual Application camera dt";return false;}if(!services.application_dt(dt,error))return false;
 const float seconds=static_cast<float>(dt)*0.001f;
 for(unsigned n=0;n<3;++n){const float move=seconds*s.velocity[n];target[n]=move+second[n];}
 applied=true;return true;
}
}
