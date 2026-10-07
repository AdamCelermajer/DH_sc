// SourceRuntime native resource allocation, on the SAME campaign and Scene.
// This lookup directory owns no engine references; it indexes actual resource
// allocations weakly and never substitutes an object/Scene/World manager.
struct NativeBatchDirectoryV110 {
 std::weak_ptr<SourceWorldBorrowV61> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 std::weak_ptr<RendererSourceCandidateV55> candidate;
 std::map<std::uintptr_t,std::weak_ptr<dh2::world::NativeBatchMeshV110>> meshes;
 std::map<std::uintptr_t,std::weak_ptr<dh2::world::NativeBatchNodeV110>> nodes;
 bool current(std::shared_ptr<SourceWorldBorrowV61>& source,SourceCampaignCandidateBorrowV55& actual,std::string& e)const{
  source=world.lock();auto expected=level.lock();auto producer=candidate.lock();
  if(!source||!expected||!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=source->owner||
     actual.actual_world.owner_before(source->owner)||source->owner.owner_before(actual.actual_world)||actual.level!=expected||
     actual.level.owner_before(expected)||expected.owner_before(actual.level)||actual.application!=source->application||!actual.roots||
     !producer||!producer->factory||!producer->graph||producer->canonical!=source->canonical_world||
     actual.objects!=source->canonical_world->manager_lease){
   if(e.empty())e="Retired or replaced native batch resource campaign/Level/Scene";return false;
  }return true;
 }
};
static bool bind_source_batch_resources_v110(const std::shared_ptr<SourceWorldBorrowV61>& world,
 const std::shared_ptr<RendererSourceCandidateV55>& candidate,const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,std::string& e){
 if(!world||!level){e="Required actual batch resource World/Level";return false;}
 if(world->batch_resource_provider_v110){e.clear();return true;}
 auto p=std::make_shared<NativeBatchDirectoryV110>();p->world=world;p->level=level;p->candidate=candidate;
 const auto prior=world->batching_native_v96;
 world->batching_native_v96=[p,prior](const SourceCampaignCandidateBorrowV55& supplied,dh2::loader::BatchNativeServicesV96& s,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
  if(!p->current(world,actual,e)||actual.actual_world!=supplied.actual_world||actual.level!=supplied.level)return false;
  if(prior&&!prior(supplied,s,e))return false;
  if(!p->current(world,actual,e))return false;
  if(!s.provider)s.provider=p;
  const auto previous_current=s.validate_current;
  s.validate_current=[p,previous_current](std::string& e){std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e))return false;
   if(previous_current&&!previous_current(e))return false;return p->current(world,actual,e);
  };
  dh2::world::NativeBatchLifetimeV110 lifetime;lifetime.owner=p;
  lifetime.require_release=[weak=std::weak_ptr<SourceWorldBorrowV61>(world)](std::string& e){auto world=weak.lock();
   if(!world||!world->owner){e="Batch native D1 lost actual campaign authority";return false;}
   return require_source_campaign_class_delivery_v106(world->owner,e);
  };
  if(!s.construct_mesh)s.construct_mesh=[p,lifetime](std::uint32_t attributes,dh2::loader::BatchMeshBorrowV96& out,std::string& e){
   out={};std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e))return false;
   auto mesh=std::make_shared<dh2::world::NativeBatchMeshV110>(attributes,lifetime);
   p->meshes[mesh->identity()]=mesh;return mesh->borrow(out,e);
  };
  if(!s.construct_root)s.construct_root=[p,lifetime](std::int32_t id,const dh2::loader::BatchMeshBorrowV96& supplied,
    dh2::loader::BatchRootBorrowV96& out,std::string& e){
   out={};std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e))return false;
   auto at=p->meshes.find(supplied.identity);auto mesh=at==p->meshes.end()?nullptr:at->second.lock();
   if(!mesh||!supplied.owner||supplied.owner.get()!=mesh.get()||supplied.owner.owner_before(mesh)||mesh.owner_before(supplied.owner)){
    e="CBatchSceneNode constructor requires SAME actual CBatchMesh allocation";return false;
   }
   auto root=std::make_shared<dh2::world::NativeBatchNodeV110>(id,lifetime);p->nodes[root->identity()]=root;
   //Retain the real allocation before the constructor's mesh130/grab prefix.
   out.owner=root;out.identity=root->identity();out.node.owner=root;out.node.identity=root->identity();
   out.drop=[root](std::string& e){return root->drop(e);};
   if(!root->initialize_mesh(mesh,e))return false;
   return root->borrow(out,e);
  };
  if(!s.segment)s.segment=[p](std::uintptr_t id,std::uintptr_t index,dh2::loader::BatchSegmentBorrowV96& out,std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e))return false;
   auto at=p->meshes.find(id);auto mesh=at==p->meshes.end()?nullptr:at->second.lock();
   if(!mesh){e="Required actual compiled mesh segment owner";return false;}return mesh->segment(index,out,e);
  };
  if(!s.scene_add_child5c)s.scene_add_child5c=[p](const dh2::loader::BatchNodeBorrowV96& supplied,std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e)||!supplied.identity||!supplied.owner)return false;
   dh2::world::GameObjectSceneRootBorrowV1 loan;auto at=p->nodes.find(supplied.identity);
   if(at!=p->nodes.end()){
    auto root=at->second.lock();
    if(!root||supplied.owner.get()!=root.get()||supplied.owner.owner_before(root)||root.owner_before(supplied.owner)||
       !root->scene_loan(actual.roots,loan,e))return false;
   }else{
    if(!actual.roots->borrow_registered_root_v110(supplied.identity,loan,e))return false;
    if(!loan.owner||loan.identity!=supplied.identity){
     e="Batch attach differs from SAME registered original root";return false;
    }
   }
   return actual.roots->add_child(std::move(loan),e);
  };
  if(!s.set_visual_null)s.set_visual_null=[p](const dh2::loader::BatchObjectBorrowV96& object,std::string& e){
   std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   auto producer=p->candidate.lock();
   if(!p->current(world,actual,e)||!producer||!object.owner||!object.visual2d8)return false;
   const auto visual_id=*object.visual2d8;if(!visual_id){e.clear();return true;}
   if(producer->factory->modules->module(object.identity)){
    auto visual=producer->graph->visual(visual_id);
    if(!visual||visual->identity()!=visual_id){e="Batching lost actual Module visual assignment";return false;}
    const auto root=visual->root_identity();
    if(root&&!retire_source_campaign_root_geometry_v106(world->owner,root,e))return false;
    return producer->graph->source_set_visual_null_v110(object.identity,visual_id,e);
   }
   if(object.archetype48&&(*object.archetype48=="Character"||*object.archetype48=="Player")){
    e="Required compiled Character mesh/animator ownership transfer before SetVisualObject(NULL)";return false;
   }
   std::shared_ptr<dh2::world::RetainedGameObjectVisualV1> visual;
   if(!world->gameobject_graph_v68||!world->gameobject_graph_v68->borrow_visual(visual_id,visual,e)||!visual)return false;
   if(visual->root_animator_present()||!visual->skinned_meshes().empty()){
    e="Required compiled dynamic mesh/animator ownership transfer before visual retirement";return false;
   }
   if(!retire_source_campaign_object_geometry_v106(world->owner,object.identity,visual_id,e))return false;
   return world->gameobject_graph_v68->destroy_visual_source_v92(object.identity,visual_id,e);
  };
  NativeBatchResourceLoansV111 loans;loans.owner=p;loans.assets=world->apk_assets_v111;
  loans.initial_driver_limits=[](std::uint32_t& vertices,std::uint32_t& indices,std::string& e){
   return native_batch_backend_limits_v111(vertices,indices,e);
  };
  loans.borrow_mesh=[p](std::uintptr_t id,std::shared_ptr<dh2::world::NativeBatchMeshV110>& out,std::string& e){
   out.reset();std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e))return false;auto at=p->meshes.find(id);
   if(at==p->meshes.end()||!(out=at->second.lock())||!out->live()){e="Required SAME native batch mesh allocation";return false;}
   e.clear();return true;
  };
  loans.borrow_node=[p](std::uintptr_t id,std::shared_ptr<dh2::world::NativeBatchNodeV110>& out,std::string& e){
   out.reset();std::shared_ptr<SourceWorldBorrowV61> world;SourceCampaignCandidateBorrowV55 actual;
   if(!p->current(world,actual,e))return false;auto at=p->nodes.find(id);
   if(at==p->nodes.end()||!(out=at->second.lock())||!out->live()){e="Required SAME native batch node allocation";return false;}
   e.clear();return true;
  };
  return bind_native_batch_compilation_v110(actual,std::move(loans),s,e);
 };
 world->batch_resource_provider_v110=p;e.clear();return true;
}
