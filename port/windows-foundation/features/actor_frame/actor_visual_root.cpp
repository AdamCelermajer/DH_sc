#include "actor_visual_root.hpp"
#include "../../../engine-animation/animation.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace dh::foundation::features {
bool ActorVisualRoot::validate(std::string& error)const {
 if(!bound_||!binding_.actor_lease||!binding_.model_lease||!binding_.actor||!binding_.model||!binding_.model->loaded()||!binding_.visual2d8||*binding_.visual2d8!=binding_.identity||!binding_.identity){error="Canonical visual root requires SAME assigned actor/model/visual slot";return false;}return true;
}
bool ActorVisualRoot::bind(ActorVisualRootBindings binding,std::string& error){
 if(!binding.actor_lease||!binding.model_lease||!binding.actor||!binding.model||!binding.model->loaded()||!binding.visual2d8||!binding.identity||*binding.visual2d8!=binding.identity||(binding.initial_root.presence&~3u)){error="Canonical visual root binding incomplete";return false;}
 const auto* config=binding.model->configuration();if(!config){error="Actual model configuration unavailable";return false;}
 const bool animated=!config->clips.empty()||std::any_of(config->animation_paths.begin(),config->animation_paths.end(),[](const auto& s){return !s.empty();});
 if(animated&&!config->consume_root_motion){error="Canonical rendered root requires original model root compensation; raw model would double displacement";return false;}
 // This root matrix is owner placement. CharacterVisual already applies the
 // helper compensation to its local posed vertices, so no second helper is
 // multiplied into this renderer placement.
 Mat4 matrix;dh2_node_matrix(matrix.data(),binding.initial_root.position,binding.initial_root.quaternion,binding.initial_root.scale);
 for(float value:matrix)if(!std::isfinite(value)){error="Nonfinite initial source visual root matrix";return false;}
 binding_=std::move(binding);root_=binding_.initial_root;placement_=matrix;bound_=true;error.clear();return true;
}
bool ActorVisualRoot::update_absolute(std::string& error){
 if(!validate(error))return false;Mat4 matrix;dh2_node_matrix(matrix.data(),root_.position,root_.quaternion,root_.scale);for(float value:matrix)if(!std::isfinite(value)){error="Source visual root matrix overflow";return false;}placement_=matrix;error.clear();return true;
}
bool ActorVisualRoot::stage_displacement(const std::vector<Vec3>& deltas,Vec3 sampled,bool& moved,std::string& error){
 if(!validate(error))return false;if(deltas.size()>65536){error="Source displacement count rejected";return false;}
 std::vector<float> xyz;xyz.reserve(deltas.size()*3);for(const auto& v:deltas){if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.z)){error="Nonfinite authored source delta";return false;}xyz.insert(xyz.end(),{v.x,v.y,v.z});}
 if(!std::isfinite(sampled.x)||!std::isfinite(sampled.y)||!std::isfinite(sampled.z)){error="Nonfinite sampled original root coordinate";return false;}
 root_.animated[0]=sampled.x;root_.animated[1]=sampled.y;root_.animated[2]=sampled.z;
 const dh2::visual::Displacement request{&root_,xyz.data(),static_cast<std::uint32_t>(deltas.size()),0};const auto status=dh2_visual_displace(&request);if(status<0){error="Original visual displacement rejected root";return false;}moved=status!=0;return update_absolute(error);
}
bool ActorVisualRoot::sync_position(std::string& error){if(!validate(error))return false;std::memcpy(root_.position,binding_.actor->transform.position.data(),12);root_.flags|=8;return update_absolute(error);}
bool ActorVisualRoot::sync_rotation(std::string& error){if(!validate(error))return false;float quaternion[4];if(dh2_visual_rotation(quaternion,binding_.actor->transform.rotation.data())){error="Original visual quaternion conversion rejected";return false;}if(std::equal(quaternion,quaternion+4,root_.quaternion)){error.clear();return true;}std::memcpy(root_.quaternion,quaternion,16);root_.flags|=4;return update_absolute(error);}
bool ActorVisualRoot::sync_scaling(std::string& error){
 if(!validate(error))return false;if(!binding_.scale120){error="Reached SAME source scale120 getter unavailable";return false;}std::array<float,3> scale;if(!binding_.scale120(scale,error))return false;for(float v:scale)if(!std::isfinite(v)){error="Nonfinite actual visual scale";return false;}
 if(std::equal(scale.begin(),scale.end(),root_.scale)){error.clear();return true;}
 // Source SetScaling stores scale before CalcMeshBox/ApplyMeshBox. Missing
 // reached tails preserve that exact prefix; no successful bounds placeholder.
 std::memcpy(root_.scale,scale.data(),12);root_.flags|=2;
 if(!binding_.changed_mesh_bounds||!binding_.changed_mesh_bounds(error)){if(error.empty())error="Reached source changed-scale mesh bounds unavailable";return false;}return update_absolute(error);
}
bool ActorVisualRoot::subobjects_visual(OriginalActorSubobjectsVisual& output,std::string& error){
 if(!validate(error))return false;auto self=weak_from_this().lock();if(!self){error="Canonical visual root requires shared receiver lease";return false;}
 output={};output.receiver=self;output.identity=binding_.identity;output.position_ac=root_.position;
 output.update=[self](std::string& e){if(!self->validate(e))return false;if(self->binding_.update_endpoint==0x470cf0){e.clear();return true;}if(!self->binding_.update_override){e="Reached nonbase actual Visual.Update unbound";return false;}return self->binding_.update_override(e);};
 output.update_absolute=[self](auto& e){return self->update_absolute(e);};
 output.apply_rotation=[self](ActorState& actor,std::string& e){if(!self->validate(e)||&actor!=self->binding_.actor)return false;if(self->binding_.apply_rotation_endpoint==0x470ccc){e.clear();return true;}if(!self->binding_.apply_rotation_override){e="Reached nonbase actual Visual.ApplyRotation unbound";return false;}return self->binding_.apply_rotation_override(actor,e);};
 output.sync_rotation=[self](const ActorState& actor,std::string& e){if(&actor!=self->binding_.actor){e="Rotation synchronization borrowed another actor";return false;}return self->sync_rotation(e);};
 output.sync_scaling=[self](const ActorState& actor,std::string& e){if(&actor!=self->binding_.actor){e="Scale synchronization borrowed another actor";return false;}return self->sync_scaling(e);};error.clear();return true;
}
bool ActorVisualRoot::submit(const Mat4& conversion,const Submit& submitter,std::string& error)const {
 if(!validate(error))return false;if(!submitter){error="Actual render submission unbound";return false;}const auto matrix=dh2::scene::multiply(conversion,placement_);for(float v:matrix)if(!std::isfinite(v)){error="Rendered root placement overflow";return false;}for(const auto& mesh:binding_.model->meshes())if(!submitter(mesh,matrix,error))return false;error.clear();return true;
}
bool reconcile_actor_frame(const std::shared_ptr<ActorVisualRoot>& root,const OriginalActorMotionFrameBorrow& actor,OriginalActorMotionFrameServices services,OriginalActorSubobjectsInput input,ActorFrameResult& output,std::string& error){
 if(!root||root->actor()!=actor.actor||input.actor!=actor.actor||input.owner.identity!=actor.identity||!input.owner.visual2d8||*input.owner.visual2d8!=root->identity()){error="Actor frame requires SAME actual canonical root/actor/native borrows";return false;}
 input.visual=[root](std::uintptr_t identity,OriginalActorSubobjectsVisual& out,std::string& e){if(identity!=root->identity()){e="Source visual assignment changed during frame";return false;}return root->subobjects_visual(out,e);};
 services.update_subobjects=[&](const auto&,auto& e){return update_original_actor_subobjects(input,output.subobjects,e);};return update_original_actor_motion_frame(actor,services,output.motion,error);
}
bool actor_frame_can_move_towards(const std::shared_ptr<dh2::player::PlayerManagerOwnerV1>& players,dh2::world::LevelCanMoveServicesV108 services,const float* position,const float* heading,bool& accepted,std::string& error){
 if(!players||!players->source_initialized_v59()){error="Required actual initialized player registry";return false;}
 services.count6c4=[players](std::int32_t& out,std::string& e){out=*players->character_count_field();e.clear();return true;};return dh2::world::level_can_move_towards_v108(position,heading,services,accepted,error);
}
}
