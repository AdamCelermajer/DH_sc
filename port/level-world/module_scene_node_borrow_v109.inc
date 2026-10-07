namespace dh2::world {
bool ModuleStaticSceneV2::borrow_scene_node_v109(std::shared_ptr<void> root_lease,
 std::function<bool(std::string&)> current,std::uintptr_t id,RetainedSceneNodeBorrowV109& out,std::string& e){
 out={};
 if(root_lease.get()!=this||!current||!current(e)||!ready_||!id){if(e.empty())e="Required actual live Module scene loan";return false;}
 const bool root=id==reinterpret_cast<std::uintptr_t>(this);
 std::shared_ptr<RetainedVisualNodeV91> node;std::shared_ptr<RetainedMeshNodeV91> mesh;
 if(!root){
  for(const auto& n:native_nodes_v93_)if(reinterpret_cast<std::uintptr_t>(n.get())==id){node=n;break;}
  if(!node)for(const auto& m:native_meshes_v93_)if(reinterpret_cast<std::uintptr_t>(m.get())==id){mesh=m;break;}
  if(!node&&!mesh){e="Unknown actual Module child node";return false;}
 }
 auto members=[this,root_lease,current,root,node,mesh](std::size_t wanted,bool select,
   std::size_t& count,std::uintptr_t& selected,std::string& e){
  count=0;selected=0;
  if(!current(e)||!ready_||(node&&(node->native_destroyed_v106||!node->fields||node->fields->detached))||
     (mesh&&(mesh->native_destroyed_v106||!mesh->fields||mesh->fields->detached))){
   if(e.empty())e="Retired actual Module hierarchy receiver";return false;
  }
  const auto visit=[&](const auto& child){if(select&&count==wanted)selected=reinterpret_cast<std::uintptr_t>(child.get());++count;};
  if(root){
   for(const auto& child:native_nodes_v93_)if(child&&child->fields&&!child->native_destroyed_v106&&
      !child->fields->detached&&child->fields->parent<0&&child->native_parent_owned_v106)visit(child);
  }else if(node){
   for(const auto& child:node->children)if(child&&child->fields&&!child->native_destroyed_v106&&
      !child->fields->detached&&child->native_parent_owned_v106&&child->parent.lock()==node)visit(child);
   for(const auto& child:node->meshes)if(child&&child->fields&&!child->native_destroyed_v106&&
      !child->fields->detached&&child->parent.lock()==node)visit(child);
  }
  if(select&&!selected){e="Module child index outside actual membership";return false;}
  e.clear();return true;
 };
 std::size_t count{};std::uintptr_t ignored{};if(!members(0,false,count,ignored,e))return false;
 RetainedSceneNodeBorrowV109 b;b.identity=id;
 if(root){b.owner=root_lease;b.name24=&root_name24_v109_;b.flags11c=&root_flags_;}
 else if(node){b.owner=node;b.name24=&node->fields->name;b.flags11c=&node->fields->flags;}
 else{b.owner=mesh;b.name24=&mesh->fields->native_name24;b.flags11c=&mesh->fields->mesh_fields.flags;b.mesh_v111=mesh;b.dynamic_source_v111=mesh->fields->controller>=0;
  b.materials_v111=[this,root_lease,current,mesh](std::vector<scene::Material>& out,std::string& e){if(!current(e)||!ready_||!mesh->fields||mesh->native_destroyed_v106)return false;out.clear();for(auto index:mesh->fields->materials){if(index>=graph_.scene.materials.size()){e="Actual Module compile material index invalid";return false;}out.push_back(graph_.scene.materials[index]);}e.clear();return true;};
 }
 b.child_count=[members](std::size_t& count,std::string& e){std::uintptr_t ignored{};return members(0,false,count,ignored,e);};
 b.child=[this,root_lease,current,members](std::size_t index,RetainedSceneNodeBorrowV109& out,std::string& e){
  std::size_t count{};std::uintptr_t child{};if(!members(index,true,count,child,e))return false;
  return borrow_scene_node_v109(root_lease,current,child,out,e);
 };
 b.set_visible48=[this,root_lease,current,root,node,mesh,members](bool visible,std::string& e){
  std::size_t count{};std::uintptr_t ignored{};if(!members(0,false,count,ignored,e))return false;
  if(root)return set_root_local_visibility_v94(visible,e);
  if(node){auto& fields=*node->fields;const auto before=fields.flags&1u;fields.visibility.local120=visible?1:0;
   if(visible&&fields.visibility.parent121&&!fields.detached)fields.flags|=1u;else fields.flags&=~1u;
   if(before!=(fields.flags&1u)){propagate_node_visibility_v94(node->index);if(notify_hierarchy_)notify_hierarchy_();}
  }else{const auto before=mesh->fields->mesh_fields.flags&1u;mesh->source_set_visible(visible);
   if(before!=(mesh->fields->mesh_fields.flags&1u)&&notify_hierarchy_)notify_hierarchy_();}
  e.clear();return true;
 };
 out=std::move(b);e.clear();return true;
}
}
