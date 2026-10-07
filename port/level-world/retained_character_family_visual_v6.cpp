#include "retained_character_family_visual_v6.hpp"
#include "character_live_body_projection_v62.hpp"
namespace dh2::character {
RetainedCharacterFamilyVisualV6::RetainedCharacterFamilyVisualV6(std::shared_ptr<RetainedCharacterActorV1> actor,CharacterFamilyVisualServicesV6 services):actor_(std::move(actor)),services_(std::move(services)){
 CharacterSameSceneAnimatorServicesV6 s;s.world=services_.world;s.same_set_manager=services_.animation_manager;
 s.animation_dictionary=services_.animation_dictionary;s.read=services_.animation_read;s.registration=services_.registration;
 s.register_set_v62=services_.register_set_v62;s.add_animation_trace_v116=services_.add_animation_trace_v116;
 s.unique_set_id_v116=[this](std::int32_t& id,std::string& e){
  CharacterAnimationSelectionV6 selected;if(!actor_||!services_.animation_selection||!services_.animation_selection(*actor_,selected,e))return false;
  id=static_cast<std::int32_t>((static_cast<std::uint32_t>(selected.table)<<8)|static_cast<std::uint32_t>(selected.skill_list));e.clear();return true;
 };
 animator_=std::make_unique<CharacterSameSceneAnimatorV6>(nullptr,std::move(s));
}
bool RetainedCharacterFamilyVisualV6::source_set_visual_null_batch_v112(std::string& e){
 auto same=visual();if(!assets_||!same||(!same->batch_animation_retained_v112()&&(same->root_animator_present()||!same->skinned_meshes().empty()))){e="Required SAME acquired compiled mesh/animator ownership before Character SetVisualObject(NULL)";return false;}
 return assets_->set_visual(std::uintptr_t{},e);
}
bool RetainedCharacterFamilyVisualV6::bind(std::string& error){
 if(assets_)return true;if(bind_attempted_){error="Failed Character visual field prefix cannot replay";return false;}bind_attempted_=true;
 if(!actor_||!services_.world||!services_.scene_manager||!services_.update_pf){error="Required SAME family Character/World/SceneManager/PF owners";return false;}
 world::GameObjectVisualFieldBorrowV5 fields;if(!actor_->visual_fields_v5(actor_,services_.update_pf,fields,error))return false;
 std::string* model=nullptr;std::string* xref=nullptr;if(!actor_->visual_strings_v6(model,xref,error))return false;
 connection_=std::make_shared<world::RetainedCharacterVisualConnectionV5>(fields,actor_->source_visual(),services_.visual,services_.scene_manager);
 assets_=std::make_unique<world::CharacterVisualAssetOwnerV6>(fields.identity,actor_->source_visual(),*model,*xref,connection_->services(connection_));return true;
}
bool RetainedCharacterFamilyVisualV6::load_visual(std::string& error){if(!bind(error))return false;return assets_->load_visual(error);}
bool RetainedCharacterFamilyVisualV6::source_set_visual_v109(const char* model,const char* xref,bool force,std::string& e){
 if(!bind(e))return false;return assets_->set_visual(model,xref,force,e);
}
bool RetainedCharacterFamilyVisualV6::source_add_animation_set_v109(std::string& e){
 auto same=visual();if(!same){e.clear();return true;}
 if(!animator_){e="Required existing SAME CharAnimator for ANIM_AddSetToRenderObject";return false;}
 return animator_->source_add_set_to_render_v109(std::move(same),e);
}
bool RetainedCharacterFamilyVisualV6::sync_visual(std::uintptr_t identity,std::string& error){if(!connection_){error="Required constructed same Character VisualObject";return false;}auto v=connection_->lookup(identity);if(!v){error="Required SAME retained visual Sync receiver";return false;}return v->sync(error);}
bool RetainedCharacterFamilyVisualV6::sync_visual_position_v7(std::uintptr_t identity,std::string& error){
 if(!connection_){error="Required constructed same Character visual position receiver";return false;}
 auto v=connection_->lookup(identity);if(!v||actor_->source_visual()!=identity){error="Required SAME attached visual.SyncPosition receiver";return false;}
 return v->sync_position_v7(error);
}
bool RetainedCharacterFamilyVisualV6::set_animation_set(const data::AnimationTables& tables,std::string& error){
 if(animation_attempted_){error="Failed/already-bound Character animation prefix cannot replay";return false;}animation_attempted_=true;
 auto v=visual();if(!v||!services_.animation_selection||!services_.animation_manager){error="Required actual Character animation selection/manager/visual";return false;}
 CharacterAnimationSelectionV6 selected;if(!services_.animation_selection(*actor_,selected,error))return false;
 if(!animator_||!animator_->bind_visual_v116(v,[v](auto remove,auto& e){return v->attach_character_animator_v6(std::move(remove),e);},
    [v](auto& e){return v->character_pose_changed_v6(e);},error)||!animator_->initialize(tables,selected.table,selected.skill_list,selected.skill_animations,error))return false;
 animator_->playback().observer=services_.events;animator_->playback().selection_fx_v2=services_.step_fx;return true;
}
bool RetainedCharacterFamilyVisualV6::body_projection(const physical::NpcBodyRequest& q,physical::NpcBodyProjection& out,std::string& error){auto v=visual();if(!v){error="Required same loaded Character visual for InitPhysicalObject";return false;}return character_live_body_projection_v6(*actor_,*v,q,out,error);}
bool RetainedCharacterFamilyVisualV6::body_projection_v62(const physical::NpcBodyRequest& q,physical::NpcBodyProjection& out,std::string& error){auto v=visual();if(!v){error="Required same loaded Character/Player visual for InitPhysicalObject";return false;}return character_live_body_projection_v62(*actor_,*v,q,out,error);}
bool RetainedCharacterFamilyVisualV6::close(std::string& error){
 if(!connection_){animator_.reset();error.clear();return true;}
 // Actual body/PF owner must be closed before this root/animator teardown.
 if(assets_&&!assets_->set_visual(std::uintptr_t{},error))return false;
 if(!connection_->discard_failed(error)||!connection_->discard_unattached(error))return false;
 animator_.reset();assets_.reset();connection_.reset();return true;
}
}
