//Include in source_campaign_runtime_v61 model_renderer, before connect body.
static bool bind_source_campaign_stage12_v98(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 dh2::loader::SourceLoadingInputsV43& inputs,std::string& e){
 if(inputs.external.stage_body[12]){e.clear();return true;}
 if(!world||!world->application||!level||!inputs.preparation||inputs.preparation->level()!=level){e="Required SAME Stage12 actual preparation/menu campaign";return false;}
 const auto weak=std::weak_ptr<SourceWorldBorrowV61>(world);const auto expected=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(level);
 inputs.external.stage_body[12]=[weak,expected](std::string& e){auto world=weak.lock();auto level=expected.lock();dh2::loader::CanonicalCurrentLevelBorrowV1 current;
  if(!world||!level||!borrow_current_native_level_v27(current,e)||current.level()!=level||current.level().owner_before(level)||level.owner_before(current.level())||level->constructor_fields_v3().field130!=12){if(e.empty())e="Required SAME actual Stage12 Level/menu owner";return dh2::loader::LifecycleStepV36::failed;}
  return source_native_loadmenu1_v98(world->owner,e)?dh2::loader::LifecycleStepV36::complete:dh2::loader::LifecycleStepV36::failed;
 };e.clear();return true;
}
