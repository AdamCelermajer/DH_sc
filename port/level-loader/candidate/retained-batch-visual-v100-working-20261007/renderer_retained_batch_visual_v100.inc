//Include at SourceRuntime TU end INSIDE model_renderer, after the private
//source_campaign_v55/candidate declarations and public current borrower bodies.
bool bind_source_retained_batch_visual_v100(const SourceCampaignCandidateBorrowV55& actual,
 dh2::loader::BatchNativeServicesV96& bag,dh2::loader::BatchRootNameBorrowV100 names,std::string& e){
 if(bag.visual_root){e.clear();return true;} //Preserve genuine incoming native node implementation.
 if(!actual.actual_world||!actual.level||!actual.objects||!actual.roots){e="Required SAME actual batch campaign/Level/manager/Scene";return false;}
 const auto world=std::weak_ptr<void>(actual.actual_world);
 const auto level=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(actual.level);
 const auto objects=std::weak_ptr<dh2::world::CanonicalObjectManagerV1>(actual.objects);
 const auto roots=std::weak_ptr<dh2::world::GameObjectSceneRootRegistryV1>(actual.roots);
 auto scope=[world,level,objects,roots,prior=bag.validate_current](std::string& e){
  if(prior&&!prior(e))return false;auto w=world.lock();auto l=level.lock();auto o=objects.lock();auto r=roots.lock();SourceCampaignCandidateBorrowV55 now;
  if(!w||!l||!o||!r||!borrow_source_campaign_candidate_v55(now,e))return false;
  const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
  const auto phase=l->constructor_fields_v3().field130;
  if(!same(now.actual_world,w)||!same(now.level,l)||!same(now.objects,o)||!same(now.roots,r)||phase<21||phase>24){e="Retired/replaced actual batch visual source scope";return false;}return true;
 };
 bag.visual_root=[scope,names=std::move(names)](const dh2::loader::BatchObjectBorrowV96& loan,
  dh2::loader::BatchNodeBorrowV96& out,std::string& e){
  if(!scope(e)||!loan.owner||!loan.identity||!loan.visual2d8){if(e.empty())e="Required actual selected GameObject visual2d8 loan";return false;}
  const auto visual_id=*loan.visual2d8;if(!visual_id){out={};return true;}
  //Pin private source facades only within this delivery, never in the callback.
  auto* state=source_campaign_v55.get();if(!state||!state->world||!state->candidate||!state->candidate->graph){e="Required SAME retained source visual directory";return false;}
  auto source=state->world;auto candidate=state->candidate;
  auto module=candidate->graph->visual(visual_id);
  std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> generic;
  if(!module){const dh2::world::CanonicalObjectBorrowV1* entry{};std::int32_t key{};
   bool found=source->canonical_world->manager.source_ordered_begin_v38(key,entry);
   while(found&&(!entry||entry->identity!=loan.identity))found=source->canonical_world->manager.source_ordered_next_v38(key,key,entry);
   if(!found||!entry){e="Batch actor no longer published in SAME manager";return false;}
   const auto receipt=*entry; //Never retain/read mutable manager entry through callback delivery.
   std::uintptr_t character{};
   if(receipt.as_character&&!receipt.as_character(receipt.context,character,e))return false;
   if(!scope(e))return false;
   if(character){SourceCampaignCharacterBorrowV62 held;
    if(character!=loan.identity||!borrow_source_campaign_character_v62(source->owner,character,held,e)||!scope(e)||!held.character||!held.character->actor||!held.character->visual)return false;
    if(held.character->actor->source_visual()!=visual_id){e="Changed SAME Character visual2d8";return false;}
    generic=held.character->visual->visual();
   }else if(!source->gameobject_graph_v68||!source->gameobject_graph_v68->borrow_visual(visual_id,generic,e)||!scope(e))return false;
  }
  if((generic&&reinterpret_cast<std::uintptr_t>(generic.get())!=visual_id)||(module&&module->identity()!=visual_id)||(!generic&&!module)||!scope(e)||*loan.visual2d8!=visual_id){
   if(e.empty())e="Changed SAME actual retained visual allocation";return false;
  }
  if((generic&&generic->root_identity()&&generic->root_game_object()!=loan.identity)||
   (module&&module->root_identity()&&module->scene().source_root_game_object()!=loan.identity)){e="Retained batch Root204 belongs to another actual GameObject";return false;}
  return dh2::loader::RetainedBatchVisualViewV100::borrow(generic,module,scope,names,out,e);
 };e.clear();return true;
}