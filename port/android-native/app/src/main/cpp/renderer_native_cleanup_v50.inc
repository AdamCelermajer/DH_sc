// Functional Application.CleanGlitch adaptation over the EXISTING native
// renderer. UI/font/FX allocations remain in their directly retained RAII
// owners. No nonexistent original shader-baker/material cache is fabricated.
bool clean_native_graphics_v50(std::string& error){
 static bool busy=false,failed=false;
 if(busy||failed){error="Native graphics cleanup reentered or prior release failed";return false;}
 if(model_publication_depth_v50){error="Native graphics cleanup during unpublished GPU candidate construction";return false;}
 bool actual_high_performance=false;
 if(!borrow_actual_device_high_performance_v54(actual_high_performance,error))return false;
 (void)actual_high_performance; // Validate the actual device; do not override its source flag.
 const auto ledger=dh2::android_resources::budget_lease_v39();
 const auto context=ledger->context_state_v41();
 if(!context.ready){error="Native graphics cleanup requires the live actual context";return false;}
 busy=true;struct Guard{bool& busy;~Guard(){busy=false;}} guard{busy};
 try{
  std::set<std::uint32_t> texture_uses,buffer_uses;
  auto owning_images=[&](const auto& names){for(auto name:names)if(name)texture_uses.insert(name);};
  auto bindings=[&](const auto& batches){for(const auto& draw:batches){
   if(draw.diffuse)texture_uses.insert(draw.diffuse);
   if(draw.alpha)texture_uses.insert(draw.alpha);
   if(draw.vertices)buffer_uses.insert(draw.vertices);
   if(draw.indices)buffer_uses.insert(draw.indices);
  }};
  owning_images(images);bindings(draws);
  owning_images(equipment_images);bindings(equipment_draws);
  for(const auto& group:object_groups)bindings(group.draws);
  for(const auto& actor:class_preview_actors){
   owning_images(actor.images);bindings(actor.draws);
   for(const auto& weapon:actor.weapons){owning_images(weapon.images);bindings(weapon.draws);}
  }
  owning_images(loot_gpu_textures_v27);
  for(const auto& item:loot_gpu_records_v27)bindings(item.second.draws);
  auto validate=[&](std::uint32_t name,const auto& owner,std::string& e){
   if(!name||owner.budget!=ledger||owner.generation!=context.generation){
    e="Native cleanup allocation is outside the SAME live context";return false;
   }return true;
  };
  // Preflight both registries before either sweep can call a GL release.
  for(const auto& allocation:model_texture_charges_v40)if(!validate(allocation.first,allocation.second,error))return false;
  for(const auto& allocation:generic_buffer_charges_v41)if(!validate(allocation.first,allocation.second,error))return false;
  dh2::resources::NativeUnusedSweepV50 texture_counts,buffer_counts;
  auto release_texture=[&](std::uint32_t name,std::string&){
   for(auto i=loot_gpu_texture_cache_v27.begin();i!=loot_gpu_texture_cache_v27.end();){
    if(i->second==name)i=loot_gpu_texture_cache_v27.erase(i);else ++i;
   }
   release_model_texture_v40(name,false);return true;
  };
  auto release_buffer=[](std::uint32_t name,std::string&){release_buffer_v41(name,false);return true;};
  if(!dh2::resources::sweep_native_unused_v50(model_texture_charges_v40,texture_uses,validate,release_texture,texture_counts,error)||
     !dh2::resources::sweep_native_unused_v50(generic_buffer_charges_v41,buffer_uses,validate,release_buffer,buffer_counts,error)){
   failed=true;return false;
  }
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Native loading graphics cleanup | context %llu | textures %llu/%llu/%llu | buffers %llu/%llu/%llu | inspected/preserved/released | existing UI/font/FX owners retained",
   static_cast<unsigned long long>(context.generation),
   static_cast<unsigned long long>(texture_counts.inspected),static_cast<unsigned long long>(texture_counts.preserved),static_cast<unsigned long long>(texture_counts.released),
   static_cast<unsigned long long>(buffer_counts.inspected),static_cast<unsigned long long>(buffer_counts.preserved),static_cast<unsigned long long>(buffer_counts.released));
  error.clear();return true;
 }catch(const std::exception& failure){failed=true;error=failure.what();return false;}
}
