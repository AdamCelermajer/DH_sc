namespace dh2::world {
bool RetainedGameObjectVisualV1::borrow_scene_node_v109(
 const std::shared_ptr<RetainedGameObjectVisualV1>& lease,std::uintptr_t id,
 RetainedSceneNodeBorrowV109& out,std::string& e){
 out={};
 if(lease.get()!=this||!ready_||!root_present_||!id)return missing("live SAME visual hierarchy loan",e);
 const bool root=id==root_identity();
 std::shared_ptr<RetainedVisualNodeV91> node;
 std::shared_ptr<RetainedMeshNodeV91> mesh;
 if(!root){
  for(const auto& n:node_handles_)if(reinterpret_cast<std::uintptr_t>(n.get())==id){node=n;break;}
  if(!node)for(const auto& m:native_meshes_v93_)if(reinterpret_cast<std::uintptr_t>(m.get())==id){mesh=m;break;}
  if((!node&&!mesh)||(node&&(node->native_destroyed_v106||!node->fields||node->fields->detached))||
     (mesh&&(mesh->native_destroyed_v106||!mesh->fields||mesh->fields->detached)))
   return missing("attached SAME native scene node",e);
 }
 const std::weak_ptr<RetainedGameObjectVisualV1> weak=lease;
 // Resolve membership afresh on each delivery; no copied children/visibility.
 auto members=[weak,id,root,node,mesh](std::size_t wanted,bool select,std::size_t& count,
   std::uintptr_t& selected,std::string& e){
  count=0;selected=0;auto v=weak.lock();
  if(!v||!v->ready_||!v->root_present_||(root&&id!=v->root_identity())||
     (node&&(node->native_destroyed_v106||!node->fields||node->fields->detached))||
     (mesh&&(mesh->native_destroyed_v106||!mesh->fields||mesh->fields->detached))){
   e="Retired retained scene hierarchy receiver";return false;
  }
  const auto visit=[&](const auto& child){if(select&&count==wanted)selected=reinterpret_cast<std::uintptr_t>(child.get());++count;};
  if(root){
   for(const auto& child:v->node_handles_)if(child&&child->fields&&!child->native_destroyed_v106&&
      !child->fields->detached&&child->fields->parent<0&&child->native_parent_owned_v106)visit(child);
  }else if(node){
   // These are the existing native construction lists: graph children were
   // attached first, then mesh children. Never synthesize a pose hierarchy.
   for(const auto& child:node->children)if(child&&child->fields&&!child->native_destroyed_v106&&
      !child->fields->detached&&child->native_parent_owned_v106&&child->parent.lock()==node)visit(child);
   for(const auto& child:node->meshes)if(child&&child->fields&&!child->native_destroyed_v106&&
      !child->fields->detached&&child->parent.lock()==node)visit(child);
  }
  if(select&&!selected){e="Retained scene child index outside live membership";return false;}
  e.clear();return true;
 };
 RetainedSceneNodeBorrowV109 b;b.identity=id;
 if(root){b.owner=lease;b.name24=&root_name24_v109_;b.flags11c=&binding_.root.flags;}
 else if(node){b.owner=node;b.name24=&node->fields->name;b.flags11c=&node->fields->flags;}
 else{b.owner=mesh;b.name24=&mesh->fields->native_name24;b.flags11c=&mesh->fields->mesh_fields.flags;b.mesh_v111=mesh;b.dynamic_source_v111=animated_||mesh->fields->controller>=0||bool(character_animator_remove_v6_);
  if(b.dynamic_source_v111){b.animation_identity_v112=reinterpret_cast<std::uintptr_t>(this);b.animation_v112=[weak](BatchAnimationBorrowV112& out,std::string& e){auto v=weak.lock();if(!v){e="Retired SAME source animator transfer";return false;}return v->retain_batch_animation_v112(v,out,e);};}
  b.materials_v111=[weak,mesh](std::vector<scene::InstanceMaterialBindingV1>& out,std::string& e){auto v=weak.lock();if(!v||!v->ready_||!mesh->fields||mesh->native_destroyed_v106){e="Retired SAME compile material receiver";return false;}const auto& indices=mesh->fields->materials;const auto& symbols=mesh->fields->material_symbols_v1;if(symbols.size()!=indices.size()){e="Actual compile material-symbol binding domain differs";return false;}out.clear();out.reserve(indices.size());for(std::size_t i=0;i<indices.size();++i){const auto index=indices[i];if(index>=v->scene_.materials.size()){e="Actual compile material target index invalid";return false;}out.push_back({symbols[i].symbol,v->scene_.materials[index]});}e.clear();return true;};
 }
 b.child_count=[members](std::size_t& count,std::string& e){std::uintptr_t ignored{};return members(0,false,count,ignored,e);};
 b.child=[weak,members](std::size_t index,RetainedSceneNodeBorrowV109& out,std::string& e){
  std::size_t count{};std::uintptr_t child{};if(!members(index,true,count,child,e))return false;
  auto v=weak.lock();if(!v){e="Retired SAME child visual";return false;}
  return v->borrow_scene_node_v109(v,child,out,e);
 };
 b.set_visible48=[weak,id,root,node,mesh,members](bool visible,std::string& e){
  std::size_t count{};std::uintptr_t ignored{};if(!members(0,false,count,ignored,e))return false;
  auto v=weak.lock();if(!v){e="Retired SAME visibility receiver";return false;}
  if(root)return v->set_root_local_visibility_v3(visible,e);
  if(node)return v->set_node_local_visibility(id,visible,e);
  const auto before=mesh->fields->mesh_fields.flags&1u;mesh->source_set_visible(visible);
  if(before!=(mesh->fields->mesh_fields.flags&1u)){
   if(!v->services_.force_register)return v->missing("mesh visibility Scene notification",e);
   if(!v->services_.force_register(v->root_identity(),e))return false;
  }
  e.clear();return true;
 };
 out=std::move(b);e.clear();return true;
}
}
