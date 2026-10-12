#pragma once
#include <level_batching_source_v96.hpp>
#include <retained_gameobject_visual_v1.hpp>
#include <retained_module_visual_v3.hpp>
namespace dh2::loader {
//Optional genuine native Root.getName24 receipt. Current retained Root owners
//have no native CString24 storage; absence remains a NULL name loan, never "".
using BatchRootNameBorrowV100=std::function<bool(std::uintptr_t,std::shared_ptr<void>&,const std::string*&,std::string&)>;
class RetainedBatchVisualViewV100 final:public std::enable_shared_from_this<RetainedBatchVisualViewV100> {
 std::weak_ptr<world::RetainedGameObjectVisualV1> generic_;
 std::weak_ptr<world::RetainedModuleVisualV3> module_;
 std::uintptr_t root_{};std::function<bool(std::string&)> current_;
 bool live(std::shared_ptr<world::RetainedGameObjectVisualV1>& generic,
  std::shared_ptr<world::RetainedModuleVisualV3>& module,std::string& e)const{
  if(!current_||!current_(e))return false;generic=generic_.lock();module=module_.lock();
  if(generic?(!generic->ready()||generic->root_identity()!=root_):(!module||!module->ready()||module->root_identity()!=root_)){
   e="Retired/replaced SAME retained batch VisualObject.root8";return false;}return true;
 }
 bool node_live(const std::shared_ptr<world::RetainedVisualNodeV91>& node,std::string& e)const{
  std::shared_ptr<world::RetainedGameObjectVisualV1> generic;std::shared_ptr<world::RetainedModuleVisualV3> module;
  if(!live(generic,module,e))return false;
  if(!node||!node->fields||node->native_destroyed_v106||node->fields->detached||!node->native_parent_owned_v106){e="Retired actual V91 batch child node";return false;}return true;
 }
 bool mesh_live(const std::shared_ptr<world::RetainedMeshNodeV91>& mesh,std::string& e)const{
  std::shared_ptr<world::RetainedGameObjectVisualV1> generic;std::shared_ptr<world::RetainedModuleVisualV3> module;
  if(!live(generic,module,e))return false;
  if(!mesh||!mesh->fields||mesh->native_destroyed_v106||mesh->fields->detached){e="Retired actual V91 batch mesh child";return false;}
  const auto parent=mesh->parent.lock();
  if(!parent||!parent->fields||parent->native_destroyed_v106||mesh->fields->parentec!=reinterpret_cast<std::uintptr_t>(parent.get())){e="Changed actual batch mesh parent/name domain";return false;}
  return true;
 }
 bool mesh(const std::shared_ptr<world::RetainedMeshNodeV91>& actual,BatchNodeBorrowV96& out,std::string& e){
  if(!mesh_live(actual,e))return false;auto parent=actual->parent.lock();
  struct NameLease {std::shared_ptr<world::RetainedMeshNodeV91> mesh;std::shared_ptr<world::RetainedVisualNodeV91> parent;};
  BatchNodeBorrowV96 b;b.owner=std::make_shared<NameLease>(NameLease{actual,parent});b.identity=reinterpret_cast<std::uintptr_t>(actual.get());
  //Actual IMeshSceneNode.getName: own nonempty name, otherwise SAME parent name.
  b.name24=actual->fields->native_name24.empty()?&parent->fields->name:&actual->fields->native_name24;
  auto self=shared_from_this();
  b.child_count=[self,actual](std::size_t& n,std::string& e){if(!self->mesh_live(actual,e))return false;n=0;return true;};
  b.child=[self,actual](std::size_t,BatchNodeBorrowV96&,std::string& e){if(!self->mesh_live(actual,e))return false;e="Native factory mesh is a ctor-empty leaf; child index out of range";return false;};
  b.set_visible48=[self,actual](bool visible,std::string& e){if(!self->mesh_live(actual,e))return false;actual->source_set_visible(visible);return self->mesh_live(actual,e);};
  out=std::move(b);return true;
 }
 bool node(const std::shared_ptr<world::RetainedVisualNodeV91>& actual,BatchNodeBorrowV96& out,std::string& e){
  if(!node_live(actual,e))return false;BatchNodeBorrowV96 b;b.owner=actual;b.identity=reinterpret_cast<std::uintptr_t>(actual.get());b.name24=&actual->fields->name;
  auto self=shared_from_this();
  b.child_count=[self,actual](std::size_t& count,std::string& e){return self->children(actual,0,nullptr,count,e);};
  b.child=[self,actual](std::size_t i,BatchNodeBorrowV96& out,std::string& e){std::size_t count{};return self->children(actual,i,&out,count,e);};
  b.set_visible48=[self,actual](bool visible,std::string& e){
   if(!self->node_live(actual,e))return false;auto generic=self->generic_.lock();auto module=self->module_.lock();
   const auto id=reinterpret_cast<std::uintptr_t>(actual.get());
   const bool ok=generic?generic->set_node_local_visibility(id,visible,e):module->scene().source_set_node_visible_v100(id,visible,e);
   if(!self->node_live(actual,e))return false;return ok;
  };out=std::move(b);return true;
 }
 bool children(const std::shared_ptr<world::RetainedVisualNodeV91>& parent,std::size_t wanted,BatchNodeBorrowV96* out,std::size_t& count,std::string& e){
  if(!node_live(parent,e))return false;count=0;
  //Original factory appends actual geometry children BEFORE actual SNode children.
  //Read existing membership vectors each call; no graph projection or copied list.
  for(const auto& entry:parent->meshes){auto actual=entry;if(!actual||!actual->fields){e="Malformed actual batch mesh membership";return false;}
   if(actual->fields->detached)continue;if(!mesh_live(actual,e))return false;
   if(actual->parent.lock()!=parent){e="Changed SAME batch mesh child membership";return false;}
   if(out&&count==wanted)return mesh(actual,*out,e);++count;
  }
  for(const auto& entry:parent->children){auto actual=entry;if(!actual||!actual->fields){e="Malformed actual batch SNode membership";return false;}
   if(actual->fields->detached)continue;if(!node_live(actual,e))return false;
   if(actual->parent.lock()!=parent){e="Changed SAME batch SNode child membership";return false;}
   if(out&&count==wanted)return node(actual,*out,e);++count;
  }
  if(out){e="Actual batch child index out of current membership range";return false;}return true;
 }
 bool root_children(std::size_t wanted,BatchNodeBorrowV96* out,std::size_t& count,std::string& e){
  std::shared_ptr<world::RetainedGameObjectVisualV1> generic;std::shared_ptr<world::RetainedModuleVisualV3> module;
  if(!live(generic,module,e))return false;count=0;
  const auto& nodes=generic?generic->source_child_owners_v100():module->scene().source_child_owners_v100();
  for(const auto& entry:nodes){auto actual=entry;if(!actual||!actual->fields){e="Malformed SAME batch root child owner";return false;}
   if(actual->fields->parent>=0||actual->fields->detached)continue;
   if(!node_live(actual,e)||!actual->parent.expired()){if(e.empty())e="Changed native root child membership";return false;}
   if(out&&count==wanted)return node(actual,*out,e);++count;
  }
  if(out){e="Actual batch root child index out of range";return false;}return true;
 }
public:
 static bool borrow(const std::shared_ptr<world::RetainedGameObjectVisualV1>& generic,
  const std::shared_ptr<world::RetainedModuleVisualV3>& module,std::function<bool(std::string&)> current,
  const BatchRootNameBorrowV100& names,BatchNodeBorrowV96& out,std::string& e){
  if(bool(generic)==bool(module)||!current){e="Required ONE actual retained generic/Module visual";return false;}
  auto view=std::make_shared<RetainedBatchVisualViewV100>();view->generic_=generic;view->module_=module;view->current_=std::move(current);
  view->root_=generic?generic->root_identity():module->root_identity();
  std::shared_ptr<world::RetainedGameObjectVisualV1> g;std::shared_ptr<world::RetainedModuleVisualV3> m;if(!view->live(g,m,e))return false;
  if(!view->root_){out={};return true;} //Genuine completed source asset/xref miss keeps NULL root8.
  BatchNodeBorrowV96 b;b.identity=view->root_;
  struct RootLease {std::shared_ptr<void> visual,name;};auto lease=std::make_shared<RootLease>();lease->visual=generic?std::shared_ptr<void>(generic):std::shared_ptr<void>(module);
  if(names){if(!names(b.identity,lease->name,b.name24,e)||!view->live(g,m,e))return false;if(!lease->name||!b.name24){e="Required genuine SAME Root CString24 lease/cell";return false;}}
  b.owner=lease;
  b.child_count=[view](std::size_t& n,std::string& e){return view->root_children(0,nullptr,n,e);};
  b.child=[view](std::size_t i,BatchNodeBorrowV96& out,std::string& e){std::size_t n{};return view->root_children(i,&out,n,e);};
  b.set_visible48=[view](bool visible,std::string& e){std::shared_ptr<world::RetainedGameObjectVisualV1> g;std::shared_ptr<world::RetainedModuleVisualV3> m;
   if(!view->live(g,m,e))return false;const bool ok=g?g->set_root_local_visibility_v3(visible,e):m->set_root_local_visibility_v94(visible,e);
   if(!view->live(g,m,e))return false;return ok;
  };out=std::move(b);return true;
 }
};
}