#include "world_item_drop_sound_v2.hpp"
namespace dh2::character {
bool world_item_drop_sound_v2(sound::VoxPlay3DOwnerV2& vox,std::uintptr_t manager,std::uintptr_t item,std::int16_t id,const float* position,std::string& e){
 if(!position){e="Required same Item cached sound position1a8";return false;}
 CombatSoundPlayV1 p{};p.manager=manager;p.target=item;p.sound_id=id;
 for(unsigned i=0;i<3;++i)p.position[i]=position[i];
 p.source_bool=false;p.source_integer=1;p.source_float0=p.source_float1=-1.f;
 if(vox.play(p)){e=vox.error();return false;}return true;
}
}
