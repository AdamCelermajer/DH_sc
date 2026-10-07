// Call before the existing unanchored floor branch in combat_fx_remaining.
// Returns 1 handled,0 other,-1 required failure; no alternate actor authority.
static int source_player_fx_anchor_v4(PlayerSkillsRuntime& t,
 dh2::fx::MeshFxRequestV1& q,std::string& error){
 using O=dh2::fx::MeshFxOperationV1;
 if(q.operation!=O::anchor_dead&&q.operation!=O::anchor_disabled&&q.operation!=O::anchor_stationary&&
    q.operation!=O::anchor_position&&q.operation!=O::anchor_rotation&&q.operation!=O::anchor_scale)return 0;
 if(!t.world||!t.world->player_object||!t.targets||!q.identity){error="Required same authored FX anchor World";return -1;}
 dh2::character::skills::WorldTargetActorBorrowV1 actor{};
 if(t.targets->actor(q.identity,&actor)){error="Required registered live authored FX anchor";return -1;}
 if(q.operation==O::anchor_dead){if(!actor.life){error="Required same anchor life owner";return -1;}q.result=actor.life->dead!=0;return 1;}
 if(q.operation==O::anchor_position){const float* p=nullptr;if(!PlayerSkillsRuntime::heading_position(&t,q.identity,p,error))return -1;std::copy_n(p,3,q.point);return 1;}
 if(q.operation==O::anchor_rotation){
  // Current native character visual initialization publishes exactly this
  // retained root/Scene binding. Original Sync reads visual2d8 -> CScene root
  // absolute matrix when present; raw16c is only its absent-visual branch.
  if(q.identity==t.world->player_object->identity){
   if(actor.scene!=&current_scene||prince_visual.animated_node()<0){error="Required SAME produced player FX CScene root";return -1;}
   return dh2::fx::character_fx_anchor_rotation_v28(q.point,prince_visual.root,error)?1:-1;
  }
  const auto found=t.world_actors.find(q.identity);
  if(found!=t.world_actors.end()&&found->second.npc){auto& npc=*found->second.npc;
   if(npc.visual_initialized){if(!npc.animation||actor.scene!=&npc.animation->scene()){error="Required SAME produced NPC FX CScene root";return -1;}
    return dh2::fx::character_fx_anchor_rotation_v28(q.point,npc.animation->visual().root,error)?1:-1;}
   if(npc.source_visual()){error="Required live initialized NPC FX visual2d8 root";return -1;}
  }
  if(PlayerSkillsRuntime::combat_fx_rotation(&t,q.identity,q.point)){error="Required absent-visual anchor rotation16c";return -1;}return 1;
 }
 if(q.identity!=t.world->player_object->identity){
  const auto found=t.world_actors.find(q.identity);
  if(found==t.world_actors.end()||!found->second.npc||!found->second.npc->object||found->second.npc->object->identity!=q.identity){error="Required same retained NPC anchor receiver";return -1;}
  const auto& npc=*found->second.npc;
  if(q.operation==O::anchor_disabled||q.operation==O::anchor_stationary){const auto* field=npc.source_bool_field(q.operation==O::anchor_disabled?0x81:0x84);if(!field){error="Required actual NPC raw anchor byte producer";return -1;}q.result=*field;return 1;}
  if(!npc.visual_initialized){error="Required NPC Character.InitPost scale120 producer";return -1;}
  std::array<float,3> source_scale;if(!npc.source_scale(source_scale,error))return -1;
  std::copy(source_scale.begin(),source_scale.end(),q.point);return 1;
 }
 if(!t.world->player_canonical){error="Required same player canonical raw anchor fields";return -1;}
 // AnimatedFX::Update492f68 reads raw81 and raw84 directly. Enabled8a and
 // visual/static guesses are not interchangeable with these source bytes.
 if(q.operation==O::anchor_disabled){q.result=t.world->player_canonical->disabled81;return 1;}
 if(q.operation==O::anchor_stationary){q.result=t.world->player_canonical->source_static84;return 1;}
 if(!prince_source_scale120_written){error="Required actual Character.InitPost scale120 producer";return -1;}
 std::copy_n(prince_source_scale120.data(),3,q.point);return 1;
}
