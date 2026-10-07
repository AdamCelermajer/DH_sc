#include "vox_play3d_owner_v2.hpp"
namespace dh2::sound {
int VoxPlay3DOwnerV2::ask(VoxPlay3DOperationV2 op,const character::CombatSoundPlayV1& p,VoxPlay3DResponseV2& r,std::int32_t selected){
 ++phase_;if(!services_.invoke||services_.invoke(services_.context,{op,&p,selected},r)){error_="Required source Vox Play3D operation "+std::to_string(unsigned(op));return -2;}return 0;
}
int VoxPlay3DOwnerV2::play(const character::CombatSoundPlayV1& p){
 error_.clear();phase_=0;if(!p.manager){error_="Required same Vox manager";return -1;}
 VoxPlay3DResponseV2 r{};if(ask(VoxPlay3DOperationV2::disabled,p,r))return -2;if(r.value)return 0;
 r={};if(ask(VoxPlay3DOperationV2::current_level,p,r))return -2;if(!r.identity||r.value!=38)return 0;
 r={};if(ask(VoxPlay3DOperationV2::online,p,r))return -2;if(r.value){r={};if(ask(VoxPlay3DOperationV2::network_muted,p,r))return -2;if(r.value)return 0;}
 if(p.sound_id<0)return 0;r={};if(ask(VoxPlay3DOperationV2::platform_route,p,r))return -2;
 if(r.value){r={};return ask(VoxPlay3DOperationV2::platform_play,p,r);}
 r={};if(ask(VoxPlay3DOperationV2::sound_row,p,r))return -2;const auto selected=r.value;
 if(r.type==1){auto native=p;native.sound_id=selected;native.source_float0=native.source_float1=-1.f;r={};return ask(VoxPlay3DOperationV2::native_play,native,r,selected);}
 r={};if(ask(VoxPlay3DOperationV2::bank_info,p,r,selected))return -2;
 // The bank fields must survive the tracing query and reach Emit unchanged.
 const auto bank=r;VoxPlay3DResponseV2 trace{};if(ask(VoxPlay3DOperationV2::trace,p,trace,selected))return -2;
 r=bank;return ask(VoxPlay3DOperationV2::emit,p,r,selected);
}
}
