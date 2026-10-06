#include "retained_module_visual_v3.hpp"
#include <algorithm>
namespace dh2::world {
RetainedModuleVisualV3::RetainedModuleVisualV3(CanonicalGameObjectBaseOwnerV1& base,RetainedModuleVisualServicesV3 services):base_(base),services_(std::move(services)){}
bool RetainedModuleVisualV3::missing(const char* p,std::string& e)const{e=std::string("Required actual Module VisualObject ")+p;return false;}
bool RetainedModuleVisualV3::source_asset_miss(std::string& e){e.clear();if(ready_||root_present_||bytes_)return missing("fresh source asset-miss receiver",e);ready_=true;return true;}
bool RetainedModuleVisualV3::initialize(std::shared_ptr<const std::vector<std::uint8_t>> bytes,const char* xref,bool& found,std::string& e){
 e.clear();found=false;if(root_present_||ready_||bytes_)return missing("fresh receiver",e);
 if(!bytes||!xref||!services_.owner||!services_.roots)return missing("resource and same SceneManager owners",e);
 bytes_=std::move(bytes);resource_lease_=std::make_shared<std::shared_ptr<const std::vector<std::uint8_t>>>(bytes_);if(dh2_bres_open(&bres_,bytes_->data(),bytes_->size())!=resources::BresError::ok)return missing("actual BRES",e);
 ModuleSelectedSceneV2 selected;if(!module_selected_scene_v2(bres_,xref,selected,found,e))return false;
 if(!found){ready_=true;return true;}
 if(selected.scene.ignored_instances)return missing("complete static geometry-only scene factory domain",e);
 if(dh2_bres_library_count(&bres_,resources::Library::animation)||dh2_bres_library_count(&bres_,resources::Library::animation_clip))return missing("static empty animator-library domain",e);
 for(const auto& mesh:selected.scene.instances)if(mesh.controller>=0)return missing("unskinned static domain",e);
 physical::DecorSceneMarker marker{};if(!physical::decor_scene_marker(bres_,selected.scene,marker,e))return false;
 if(marker.found)return missing("no-colbox traversal domain",e);
 auto* position=base_.vector3(0x160);auto* rotation=base_.vector3(0x16c);auto* scale=base_.vector3(0x120);
 if(!position||!rotation||!scale||base_.lifecycle().static84!=1)return missing("same static parent pose/scale",e);
 physical::ObjectVisualTransformV1 trs{};if(dh2_object_visual_transform_v1(&trs,position,rotation,scale))return missing("source Sync pose",e);
 root_=std::make_shared<ModuleStaticSceneV2>();
 root_scene().pin_resource(resource_lease_);
 std::weak_ptr<GameObjectSceneRootRegistryV1> manager=services_.roots;
 root_scene().bind_hierarchy_notify([manager]{if(auto p=manager.lock())p->notify_hierarchy_changed();});
 if(!root_scene().initialize(std::move(selected),trs,e))return false;root_present_=true;
 // LoadNode attaches the actual root before VisualObject::SetParent.
 GameObjectSceneRootBorrowV1 root;root.owner=root_;root.identity=root_identity();root.flags11c=&root_scene().source_root_flags();root.parentec=&root_scene().source_root_parent();
 std::weak_ptr<void> weak=root_;
 root.notify_visibility=[weak](bool parent,std::string& error){auto p=weak.lock();if(!p){error="Released Module root";return false;}return static_cast<ModuleStaticSceneV2*>(p.get())->notify_root_visibility(parent,error);};
 // Same ctor-empty root list, validated by the actual static resource domain.
 root.remove_animators=[weak](std::string& error){auto p=weak.lock();if(!p){error="Released Module root";return false;}return static_cast<ModuleStaticSceneV2*>(p.get())->remove_source_animators(error);};
 if(!services_.roots->add_child(std::move(root),e))return false;
 if(!services_.root_update_counter)return missing("same Root absolute-update counter producer",e);
 // Module's actual virtual80 is GameObject::IsAnimated3883a8, whose whole
 // body returns zero: SetParent takes Sync then OptimizeStatic. SyncPosition
 // invokes Root::updateAbsolutePosition(false) once; OptimizeStatic invokes
 // it once more on this root (its ordinary child nodes use the base method).
 ++*services_.root_update_counter;
 ++*services_.root_update_counter;
 if(!root_scene().optimize_static(e))return false;
 std::array<float,6> box{};if(!module_static_scene_bounds_v3(bres_,root_scene(),box,e))return false;
 if(!services_.modular_meshes)return missing("same SceneManager modular-mesh search",e);
 std::vector<std::uintptr_t> modular;if(!services_.modular_meshes(modular,e))return false;
 for(auto mesh:modular){if(!mesh)return missing("real modular-mesh receiver",e);modular_mesh_=mesh;modular_found_=1;}
 if(!services_.driver_type)return missing("same video-driver getDriverType",e);
 std::int32_t driver{};if(!services_.driver_type(driver,e))return false; // source queries and discards this result
 services_.roots->force_register();
 // SearchByName(colbox,true) found no marker above, so source skips hidden
 // marker receiver and traverses genuine static mesh local boxes in order.
 if(!calc_mesh_box(e)||!apply_mesh_box(e))return false;
 // Source allocates a real controller38 after ApplyMeshBox, even for an
 // empty animator library. Its lease is this same root (source grab/drop).
 controller38_=std::make_unique<VisualAnimControllerOwnerV4>();
 VisualAnimControllerRootServicesV4 controller_services;
 controller_services.context=root_.get();
 controller_services.get_animators=[](void* p,const std::vector<VisualAnimatorBorrowV4>*& list,std::string& error){return static_cast<ModuleStaticSceneV2*>(p)->get_source_animators(list,error);};
 controller_services.remove_animators=[](void* p,std::string& error){return static_cast<ModuleStaticSceneV2*>(p)->remove_source_animators(error);};
 if(!controller38_->construct(root_,root_identity(),controller_services,false,e))return false;
 ready_=true;return true;
}
bool RetainedModuleVisualV3::calc_mesh_box(std::string& e){
 std::vector<ModuleVisualMeshV2> meshes;
 for(const auto& instance:root_scene().selected().scene.instances){assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&bres_,instance.geometry)!=assets::Error::ok)return missing("static local mesh box",e);
  ModuleVisualMeshV2 input;std::copy_n(mesh.minimum,3,input.local_box.data());std::copy_n(mesh.maximum,3,input.local_box.data()+3);
  std::copy_n(root_scene().selected().scene.graph[instance.node_index].scale,3,input.parent_scale.data());meshes.push_back(input);
 }
 std::array<float,16> relative{};return root_scene().source_root_relative(relative,e)&&module_visual_mesh_box_v2(meshes,relative,mesh_box_,e);
}
bool RetainedModuleVisualV3::sync(std::string& e){
 e.clear();if(!root_present_)return true;
 if(!services_.root_update_counter)return missing("same Root absolute-update counter producer",e);
 auto* position=base_.vector3(0x160);auto* rotation=base_.vector3(0x16c);auto* scale=base_.vector3(0x120);
 if(!position||!rotation||!scale)return missing("same Sync parent fields",e);
 // SyncPosition always calls setPosition, then updateAbsolutePosition(false),
 // before SyncRotation and SyncScaling. Preserve that pre-rotation cache.
 ++*services_.root_update_counter;if(!root_scene().sync_root_position(position,e))return false;
 physical::ObjectVisualTransformV1 trs{};if(dh2_object_visual_transform_v1(&trs,position,rotation,scale))return missing("source rotation quaternion",e);
 std::array<float,4> q{};std::copy_n(trs.root_quaternion,4,q.data());
 bool equal=true;for(unsigned k=0;k<4;++k)equal=equal&&(q[k]==root_scene().root_quaternion()[k]);
 if(!equal){root_scene().store_root_rotation(q);if(!calc_mesh_box(e)||!apply_mesh_box(e))return false;}
 equal=true;for(unsigned k=0;k<3;++k)equal=equal&&(scale[k]==root_scene().root_scale()[k]);
 if(!equal){root_scene().store_root_scale(scale);if(!calc_mesh_box(e)||!apply_mesh_box(e))return false;}
 return true;
}
bool RetainedModuleVisualV3::apply_mesh_box(std::string& e){if(!root_present_)return true;return game_object_relative_box_v3(base_,mesh_box_.data(),services_.update_pf,e);}
bool RetainedModuleVisualV3::root_bounds(std::array<float,6>& box,std::string& e){if(!root_present_)return missing("retained root bbox",e);return module_static_scene_query_bounds_v3(root_scene(),box,e);}
bool RetainedModuleVisualV3::set_root_game_object(std::uintptr_t id,std::string& e){if(!root_present_||id!=base_.identity())return missing("same root+204 receiver identity",e);root_scene().source_root_game_object()=id;return true;}
bool RetainedModuleVisualV3::node_from_name(const char* name,std::uintptr_t& out,std::string& e)const{
 e.clear();out=0;if(!root_present_||!name)return missing("actual root node search",e);
 // This selected graph is immutable in size, so a retained source node's
 // native address remains stable until visual destruction. Detached nodes
 // cannot be returned by a new source search.
 const auto& graph=root_scene().selected().scene.graph;
 for(unsigned i=0;i<graph.size();++i)if(root_scene().node_attached(i)&&graph[i].name==name){out=reinterpret_cast<std::uintptr_t>(&graph[i]);break;}return true;
}
bool RetainedModuleVisualV3::release(std::string& e){
 if(!services_.roots)return missing("same SceneManager destructor owner",e);
 // VisualD1 first SetAnimController(NULL), then root removeAnimators/remove/
 // own drop. Controller destruction therefore precedes parent unpublication.
 controller38_.reset();
 if(root_present_){if(!services_.roots->release_visual_root(root_identity(),e))return false;root_present_=false;}
 root_.reset();
 services_.roots->force_register();ready_=false;bytes_.reset();resource_lease_.reset();bres_={};return true;
}
}
