// Included in namespace model_renderer, before private renderer receivers.
bool bind_root_stage0_sound_v44(dh2::loader::Stage0ServicesV46&services,std::string&error){
 return dh2::android_audio::bind_root_stage0_sound_v44(services,error);
}
std::function<bool(std::string&)> bind_container_precache_v45(std::function<bool(int&,std::string&)>getter){
 return dh2::android_audio::bind_root_container_precache_v44(std::move(getter));
}
std::function<bool(std::string&)> bind_openable_precache_v45(const std::shared_ptr<dh2::world::CanonicalOpenableContainerV1>&actual,std::shared_ptr<const dh2::world::OpenableContainerTableV1>table){
 return dh2::android_audio::bind_openable_raw_precache_v45(actual,std::move(table));
}
std::function<bool(std::string&)> bind_destructible_precache_v45(const std::shared_ptr<dh2::world::CanonicalDestructibleContainerV16>&actual,std::shared_ptr<const dh2::world::DestructibleContainerTableV16>table){
 return dh2::android_audio::bind_destructible_raw_precache_v45(actual,std::move(table));
}
std::function<bool(std::string&)> bind_openable_precache_slot_v45(std::shared_ptr<std::weak_ptr<dh2::world::CanonicalOpenableContainerV1>>slot,std::shared_ptr<const dh2::world::OpenableContainerTableV1>table){
 return dh2::android_audio::bind_openable_raw_precache_slot_v45(std::move(slot),std::move(table));
}
std::function<bool(std::string&)> bind_destructible_precache_slot_v45(std::shared_ptr<std::weak_ptr<dh2::world::CanonicalDestructibleContainerV16>>slot,std::shared_ptr<const dh2::world::DestructibleContainerTableV16>table){
 return dh2::android_audio::bind_destructible_raw_precache_slot_v45(std::move(slot),std::move(table));
}
