#include "projectile_target_methods_v113.hpp"
#include "projectile_methods_v112.hpp"
#include <cstring>
namespace dh2::world {
namespace {
bool missing(std::string& e,const char* field){e=std::string("Required SAME produced Projectile field: ")+field;return false;}
bool pointer(CanonicalProjectileV99& r,std::uint32_t offset,std::uintptr_t& out,std::string& e){
 const auto* cell=r.source_pointer_v99(offset);if(!cell)return missing(e,"native-width pointer domain");out=*cell;e.clear();return true;
}
bool position(CanonicalProjectileV99& r,std::uint32_t offset,ProjectilePositionWordsV113& out,std::string& e){
 ProjectilePositionWordsV113 loan{r.source_word_v99(offset),r.source_word_v99(offset+4),r.source_word_v99(offset+8)};
 if(!loan.x||!loan.y||!loan.z)return missing(e,"actual XYZ scalar word domain");out=loan;e.clear();return true;
}
}
bool projectile_set_target_v113(CanonicalProjectileV99& r,std::uintptr_t target,std::string& e){
 auto* cell=r.source_pointer_v99(0x384);if(!cell)return missing(e,"target384");
 *cell=target;e.clear();return true; //Original Character::_SetProjectileTarget3ba378 inline SetTarget.
}
bool projectile_get_target_v113(CanonicalProjectileV99& r,std::uintptr_t& out,std::string& e){return pointer(r,0x384,out,e);}
bool projectile_get_source_v113(CanonicalProjectileV99& r,std::uintptr_t& out,std::string& e){return pointer(r,0x380,out,e);}
bool laser_get_source_v113(CanonicalProjectileV99& r,std::uintptr_t& out,std::string& e){
 if(!r.is_laser_type())return missing(e,"selected Laser source3dc");return pointer(r,0x3dc,out,e);
}
bool projectile_get_speed_v113(CanonicalProjectileV99& r,float& out,std::string& e){
 //Native GetSpeed3e4da8/Laser3e3e24 returns raw float32 in r0. Reuse SAME
 //V112 scalar access; no clamp/default. SetInfo produces this cell first.
 return projectile_read_float_v112(r,0x3ac,out,e);
}
bool projectile_get_heading_angle_v113(CanonicalProjectileV99& r,float& out,std::string& e){
 const auto* angle=r.base().scalar(0x178);if(!angle)return missing(e,"inherited heading178");out=*angle;e.clear();return true;
}
bool ProjectilePositionWordsV113::read(std::array<float,3>& out,std::string& e)const{
 if(!x||!y||!z)return missing(e,"live XYZ loan");
 std::array<float,3> value;std::memcpy(&value[0],x,4);std::memcpy(&value[1],y,4);std::memcpy(&value[2],z,4);
 out=value;e.clear();return true;
}
bool projectile_source_position_words_v113(CanonicalProjectileV99& r,ProjectilePositionWordsV113& out,std::string& e){return position(r,0x388,out,e);}
bool projectile_previous_position_words_v113(CanonicalProjectileV99& r,ProjectilePositionWordsV113& out,std::string& e){return position(r,0x394,out,e);}
}