// Inside model_renderer after V69/release, before producer. This independent
//authority borrows World weakly and uses actual delivery/GPU/registry owners.
struct CampaignReleasePrimitivesV107 {std::weak_ptr<SourceWorldBorrowV61> world;
 bool current(SourceCampaignCandidateBorrowV55& c,std::shared_ptr<SourceWorldBorrowV61>& w,std::string& e)const{
  w=world.lock();if(!w||!borrow_source_campaign_candidate_v55(c,e)||c.actual_world!=w->owner||c.actual_world.owner_before(w->owner)||w->owner.owner_before(c.actual_world)){if(e.empty())e="Retired native class primitive scope";return false;}return true;
 }
};
static bool build_campaign_release_primitives_v107(const SourceCampaignCandidateBorrowV55& actual,
 dh2::loader::NonCharacterNativeReleasePrimitivesV92& out,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;if(!borrow_source_campaign_condition_world_v70(actual,world,e))return false;
 auto p=std::make_shared<CampaignReleasePrimitivesV107>();p->world=world;out={};out.owner=p;out.gameobject.owner=p;out.object_base.owner=p;
 out.require_cleanup_delivery=[p](std::uintptr_t,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return p->current(c,w,e)&&require_source_campaign_class_delivery_v106(w->owner,e);};
 out.require_constructor_delivery=[p](const auto&,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  return p->current(c,w,e)&&require_source_campaign_class_delivery_v106(w->owner,e);};
 out.require_external_aliases_unpublished=[p](std::uintptr_t id,std::string& e){SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
  if(!p->current(c,w,e)||!require_source_campaign_class_delivery_v106(w->owner,e)||!c.navigation_registry||!c.objects)return false;
  //Actual PFObject D1 must have removed its sole source obstacle index.
  const auto& registry=c.navigation_registry->registry();
  for(std::uint32_t i=0;i<registry.count;++i)if(registry.entries[i].object==id){e="Actual PF obstacle still retains destroyed class";return false;}
  for(auto network:c.objects->source_network100_v102())if(network==id){e="Actual ObjectManager network100 still retains destroyed class";return false;}
  if(!retire_source_campaign_container_target_v104(w->owner,id,e))return false;
  //No new GPU allocation/Scene clock: retire this exact actor-only domain.
  //Positive ordinary object visual entries retired at their captured D0 site.
  return retire_source_campaign_object_geometry_v106(w->owner,id,0,e);
 };
 out.object_base.registered29_assertion=[](auto&,std::string& e){auto assertion=dh2::world::SourceAssertionProcessV76::borrow();
  if(!assertion||!assertion->source_level()){e="Required SAME process assertion field";return false;}
  const auto mode=*assertion->source_level();if(mode==2){e="Original ObjectBase29 assertion fatal NULL-write branch";return false;}
  if(mode==1)return assertion->report("..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\ObjectManager\\ObjectBase.cpp",113,"!IsLocked()",e);
  e.clear();return true;
 };
 //Unknown positive native allocations/attached/Lua/network domains retain
 //their original reached dependency. They are not freed through foreign casts.
 e.clear();return true;
}
