static bool target_marker_interaction_v28(void* raw,std::uintptr_t target,std::uintptr_t player,std::int32_t& out,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 if(!t.targets||!t.world||!t.world->player_object||player!=t.world->player_object->identity){error="Required same target marker player/World";return false;}
 dh2::character::skills::WorldTargetActorBorrowV1 actor{},self{};
 if(t.targets->actor(target,&actor)||t.targets->actor(player,&self)||!actor.character||!self.character){error="Required registered target's actual Character interaction receiver";return false;}
 auto& owner=t.targets->targets();auto services=owner.query_services();
 const auto& types=owner.source_ai_types_borrow_v4();
 if(dh2::character::skills::dh2_character_skill_target_query_live_flags_v6(&out,dh2::target_providers::interaction_type,actor.character,self.character,&types,&services)){
  error="Target marker GetInteractionType failed: "+owner.error();return false;
 }
 return true;
}
static void initialize_target_marker_v28(PlayerSkillsRuntime& t){
 if(!t.combat_fx||!t.world||!t.world->effects_owner||!t.world->player_object)throw std::runtime_error("Required same target marker Effects manager");
 t.target_marker_v28=std::make_unique<dh2::character::CharacterTargetMarkerV28>(t.combat_fx->manager(),t.world->effects_owner->borrow(),t.world->player_object->identity,
  dh2::character::TargetMarkerServicesV28{&t,target_marker_interaction_v28,nullptr});
 if(!t.target_marker_v28->initialize(t.error))throw std::runtime_error(t.error);
 unsigned declared=0;for(auto id:t.target_marker_v28->circles())declared+=id!=0;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original target indicator connected | source slots 9 | declared authored receivers %u | SAME CombatFX manager",declared);
}
static bool update_target_marker_v28(PlayerSkillsRuntime& t){
 if(!t.target_marker_v28||!t.world||!t.world->player_object||!t.world->player_canonical){t.error="Required SAME Character target-circle fields";return false;}
 const auto previous=t.target_marker_v28->selected();
 if(!t.target_marker_v28->update(t.world->player_object->target.last_target,t.world->player_canonical->ooi14a4,t.error))return false;
 const auto selected=t.target_marker_v28->selected();
 if(previous!=selected)__android_log_print(ANDROID_LOG_INFO,"DH2Native","Original target indicator selection | type %d | last_target %016llx | OOI %016llx",selected,
  static_cast<unsigned long long>(t.world->player_object->target.last_target),static_cast<unsigned long long>(t.world->player_canonical->ooi14a4));
 return true;
}
