#include "light_set_name_owner_v3.hpp"
#include <cstring>
namespace dh2::world {
LightSetNameOwnerV3::LightSetNameOwnerV3():names_{"PlayerLight","SceneLight","CameraLight","MonsterLight"}{}
std::int32_t LightSetNameOwnerV3::get_id(const std::string& name)const noexcept{
 // Whole GetLightSetIdFromName40c3cc: first strcmp match, else index zero.
 for(std::int32_t i=0;i<4;++i)if(!std::strcmp(name.c_str(),names_[i].c_str()))return i;
 return 0;
}
}
