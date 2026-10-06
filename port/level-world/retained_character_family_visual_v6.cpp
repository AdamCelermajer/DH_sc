#include "retained_character_family_visual_v6.hpp"
namespace dh2::character {
bool RetainedCharacterFamilyVisualV6::bind(std::string& error){
 if(assets_)return true;if(bind_attempted_){error="Failed Character visual field prefix cannot replay";return false;}bind_attempted_=true;
 if(!actor_||!services_.world||!services_.scene_manager||!services_.update_pf){error="Required SAME family Character/World/SceneManager/PF owners";return false;}
 world::GameObjectVisualFieldBorrowV5 fields;if(!actor_->visual_fields_v5(actor_,services_.update_pf,fields,error))return false;
 std::string* model=nullptr;std::string* xref=nullptr;if(!actor_->visual_strings_v6(model,xref,error))return false;
 connection_=std::make_shared<world::RetainedCharacterVisualConnectionV5>(fields,actor_->source_visual(),services_.visual,services_.scene_manager);
 assets_=std::make_unique<world::CharacterVisualAssetOwnerV6>(fields.identity,actor_->source_visual(),*model,*xref,connection_->services(connection_));return true;
}
bool RetainedCharacterFamilyVisualV6::load_visual(std::string& error){if(!bind(error))return false;return assets_->load_visual(error);}
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
 CharacterSameSceneAnimatorServicesV6 s;s.world=services_.world;s.same_set_manager=services_.animation_manager;s.animation_dictionary=services_.animation_dictionary;s.read=services_.animation_read;s.registration=services_.registration;
 s.attach=[v](auto remove,auto& e){return v->attach_character_animator_v6(std::move(remove),e);};s.pose_changed=[v](auto& e){return v->character_pose_changed_v6(e);};
 animator_=std::make_unique<CharacterSameSceneAnimatorV6>(v,std::move(s));if(!animator_->initialize(tables,selected.table,selected.skill_list,selected.skill_animations,error))return false;
 animator_->playback().observer=services_.events;animator_->playback().selection_fx_v2=services_.step_fx;return true;
}
bool RetainedCharacterFamilyVisualV6::body_projection(const physical::NpcBodyRequest& q,physical::NpcBodyProjection& out,std::string& error){auto v=visual();if(!v){error="Required same loaded Character visual for InitPhysicalObject";return false;}return character_live_body_projection_v6(*actor_,*v,q,out,error);}
bool RetainedCharacterFamilyVisualV6::close(std::string& error){
 if(!connection_)return true;
 // Actual body/PF owner must be closed before this root/animator teardown.
 if(assets_&&!assets_->set_visual(std::uintptr_t{},error))return false;
 if(!connection_->discard_failed(error)||!connection_->discard_unattached(error))return false;
 animator_.reset();assets_.reset();connection_.reset();return true;
}
}
