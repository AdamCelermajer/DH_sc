//Included inside dh2::fx after the actual retained manager/Resource definition.
bool CharacterMeshFxOwnerV4::buff_anim_controller_v87(std::uintptr_t id,std::uintptr_t& out,std::string& e){
 auto found=impl_->records.find(id);
 if(found==impl_->records.end()||found->second->pooled||!found->second->state.visual||!found->second->resource){e="Required SAME active AnimatedFX.GetAnimController";return false;}
 out=reinterpret_cast<std::uintptr_t>(&found->second->timeline);e.clear();return true;
}
bool CharacterMeshFxOwnerV4::buff_play_clip_v87(std::uintptr_t controller,std::uint32_t clip,std::string& e){
 auto found=std::find_if(impl_->records.begin(),impl_->records.end(),[&](const auto& entry){return reinterpret_cast<std::uintptr_t>(&entry.second->timeline)==controller;});
 if(found==impl_->records.end()||found->second->pooled||!found->second->resource){e="Required SAME buff AnimController receiver";return false;}
 auto& r=*found->second;
 const auto count=dh2_bres_library_count(&r.resource->image,resources::Library::animation_clip);
 //474b94..474c2c: invalid clip returns0, and PROPS_AddBuff ignores that result.
 if(clip>=count){e.clear();return true;}
 std::int32_t start{},end{};if(!r.resource->clip(clip,start,end,e))return false;
 const auto previous=r.timeline.clip_index;
 //Source SelectClip writes range only on changed selection. A looping replay
 //of the same clip returns before loop/scale/NewAnim, preserving its phase.
 if(previous==static_cast<std::int32_t>(clip)&&r.timeline.loop){e.clear();return true;}
 if(previous!=static_cast<std::int32_t>(clip)&&dh2_timeline_clip(&r.timeline,static_cast<std::int32_t>(clip),start,end)){e="Buff authored clip selection failed";return false;}
 if(previous==static_cast<std::int32_t>(clip)&&dh2_timeline_jump(&r.timeline,start)){e="Buff nonlooping source clip restart failed";return false;}
 if(dh2_timeline_loop(&r.timeline,1)||dh2_timeline_scale(&r.timeline,1.f)){e="Buff source looping/scale failed";return false;}
 //The zero-displacement native FX root has no delta applicator. Next genuine
 //Scene phase samples this actual changed range; no emission/physics tick here.
 e.clear();return true;
}
