// Include after PlayerSkillsRuntime. New declarations listed in handoff.
dh2::physical::NativeBody* PlayerSkillsRuntime::collision_physical(void*){
 return prince_body.body?&prince_body:nullptr;
}
int PlayerSkillsRuntime::collision_method(void* raw,dh2::character::WorldNpcObjectMethodV1 method,std::string& error){
 auto& t=*static_cast<PlayerSkillsRuntime*>(raw);
 if(method!=dh2::character::WorldNpcObjectMethodV1::EnablePhysicalFilter&&method!=dh2::character::WorldNpcObjectMethodV1::DisablePhysicalFilter){error="Required player Object lifecycle method";return -1;}
 const dh2::physical::NativePhysicalFilterBorrowV1 borrow{&actor_world,&prince_body,&t.collision_primary,&t.collision_secondary,&prince_initial_filter,&t.collision_filter_disabled};
 return dh2::physical::native_physical_filter_v1(borrow,method==dh2::character::WorldNpcObjectMethodV1::EnablePhysicalFilter,error)?0:-1;
}
void PlayerSkillsRuntime::bind_collisions(PlayerSkillsRuntime& t){
 if(!level.native_floor||!t.world||!t.world->player_object)throw std::runtime_error("Required same player collisions World");
 dh2::character::WorldNpcObjectServicesV1 services{};
 services.context=&t;services.physical=collision_physical;services.method=collision_method;
 services=dh2::character::character_pf_services_v1(services); // actual Character virtual leaves1,50,20
 t.collisions=std::make_unique<dh2::character::CharacterCollisionLifecycleV1>(prince_runtime.object,&level.native_floor->collision_world,live_registry,t.world->player_object->identity,services);
}
// Exact event3f helper body; call from event_service's ai_event_helper case.
// Original CharAI kernel then delivers the SAME FSM event; do not do that here.
int PlayerSkillsRuntime::enable_collisions_source(std::uintptr_t owner){
 if(!collisions||!world||!world->player_object||owner!=world->player_object->identity||!level.native_floor){error="Required same player GameObject.EnableCollisions receiver";return -1;}
 collisions->rebind_geometry(&level.native_floor->collision_world);
 if(!collisions->enable()){error=collisions->error();return -1;}return 0;
}
