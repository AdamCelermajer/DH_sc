#include "vox_music_state_owner_v1.hpp"
namespace dh2::sound {
VoxMusicStateOwnerV1::VoxMusicStateOwnerV1(VoxMusicFieldsV1&fields,VoxMusicServicesV1 services):fields_(fields),services_(services){}
int VoxMusicStateOwnerV1::ask(const VoxMusicRequestV1&q,VoxMusicResponseV1&r){r={};if(!services_.invoke||services_.invoke(services_.context,&q,&r)){error_="Required original Vox music service "+std::to_string(q.service);return -2;}return 0;}
int VoxMusicStateOwnerV1::set_music_state(const char*state){
 error_.clear();if(!state)return -1;
 const auto music=fields_.current_music_24;
 if(music<0)return 0; // Complete first source branch before audio/table reads.
 VoxMusicResponseV1 r;VoxMusicRequestV1 q{vox_music_disabled_v1,identity(),0,0,music,0,state};
 if(ask(q,r))return -2;if(r.integer)return 0;
 q.service=vox_music_event_index_v1;if(ask(q,r))return -2;q.sound_index=r.integer;
 q.service=vox_music_channel_v1;if(ask(q,r))return -2;q.channel=r.identity;
 if(!q.channel)return 0;
 q.service=vox_music_channel_info_v1;if(ask(q,r))return -2;
 const auto count=r.integer;q.info=r.identity;
 if(!q.info){error_="Required actual Vox ChannelInfo owner";return -2;}
 int status=0;
 if(count>0){q.service=vox_music_channel_state_v1;if(ask(q,r))status=-2;}
 const auto prior=error_;q.service=vox_music_channel_info_destroy_v1;
 if(ask(q,r))return -2;if(status)error_=prior;
 return status;
}
}
