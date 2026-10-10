#include "level_batching_source_v96.hpp"
#include <cmath>
#include <cstring>
#include <limits>
#include <utility>
namespace dh2::loader {
namespace {bool required(std::string& e,const char* leaf){if(e.empty())e=std::string("Required original batching primitive: ")+leaf;return false;}}
bool original_batch_eligible_v96(const BatchObjectBorrowV96& b,const BatchNativeServicesV96& s,bool& out,std::string& e){
 out=false;if(!b.identity)return true; //Native ObjectHandle->GameObject NULL.
 if(s.validate_current&&!s.validate_current(e))return false;
 if(!b.owner||!b.deleted83)return required(e,"SAME GameObject83");if(*b.deleted83)return true;
 if(!b.archetype48)return required(e,"actual source GameObject CString48");
 const auto* type=b.archetype48->c_str();
 if(!std::strcmp(type,"Module")||!std::strcmp(type,"Decor")){if(!s.trace||!s.trace("isTracingBatchingCompiler",e))return required(e,"fresh included-object source trace");out=true;return true;}
 if(!std::strcmp(type,"Player"))return s.trace&&s.trace("isTracingBatchingCompiler",e);
 if(!b.name30)return required(e,"actual source GameObject CString30");
 if(std::strstr(b.name30->c_str(),"Player"))return s.trace&&s.trace("isTracingBatchingCompiler",e);
 bool character=!std::strcmp(type,"Character");
 bool accepted=character||!std::strcmp(type,"DestructibleContainer")||!std::strcmp(type,"OpenableContainer")||
  !std::strcmp(type,"Door")||!std::strcmp(type,"TimerTrap")||!std::strcmp(type,"TriggerTrap")||!std::strcmp(type,"AnimatedDecor");
 //Qualified GameObject.MeetCondition38ab60 is native literaltrue, not Eval.
 if(accepted&&character){bool faerie{};if(!s.is_faerie||!s.is_faerie(b.identity,faerie,e))return required(e,"actual Character.GetCharType/IsFaerie");if(s.validate_current&&!s.validate_current(e))return false;accepted=!faerie;}
 if(!s.trace||!s.trace("isTracingBatchingCompiler",e))return required(e,"fresh source batching trace");
 out=accepted;return true;
}
bool original_batch_list_v96(world::CanonicalObjectManagerV1& manager,const BatchListServicesV96& s,std::string& e){
 if(!s.validate_current||!s.validate_current(e))return required(e,"actual batch-list scope");
 std::int32_t key{};const world::CanonicalObjectBorrowV1* object{};
 for(bool more=manager.source_ordered_begin_v38(key,object);more;more=manager.source_ordered_next_v38(key,key,object)){
  if(!object||!object->identity)continue; //Native NULL-map node => NULL conversion.
  target_providers::Handle16 handle;const world::CanonicalObjectBorrowV1* live{};
  if(!manager.get_handle(key,handle,e)||!s.validate_current(e)||!manager.resolve_handle_v4(handle,false,live,{},e)||!s.validate_current(e))return required(e,"actual ObjectHandle C1/GetHandle/conversion");
  if(!live)continue;
  const BatchNativeServicesV96* native{};
  if(!s.native||!s.native(native,e)||!native||!s.validate_current(e))return required(e,"actual batch-list selection services");
  BatchObjectBorrowV96 actual;
  if(!native->as_gameobject||!native->as_gameobject(*live,actual,e)||!s.validate_current(e))return required(e,"actual selected ObjectHandle->GameObject");
  bool eligible{};if(!original_batch_eligible_v96(actual,*native,eligible,e)||!s.validate_current(e))return required(e,"original batch family/name/Faerie filters");
  if(eligible&&(!s.append||!s.append(actual,e)))return required(e,"SAME actual BatchNodeCompiler158 for eligible append");
  if(!s.validate_current(e))return required(e,"same ordered batch-list continuation");
 }
 e.clear();return true;
}
bool original_batch_limit_v96(std::int32_t input,const BatchNativeServicesV96& s,std::int32_t& out,std::string& e){
 //Original __aeabi_i2d -> multiply IEEE double1.02 -> __aeabi_d2iz.
 const double scaled=static_cast<double>(input)*1.02;
 const double truncated=std::trunc(scaled);
 if(std::isfinite(truncated)&&truncated>=static_cast<double>(std::numeric_limits<std::int32_t>::min())&&truncated<=static_cast<double>(std::numeric_limits<std::int32_t>::max())){out=static_cast<std::int32_t>(truncated);return true;}
 if(!s.exceptional_d2iz)return required(e,"actual exceptional imported __aeabi_d2iz");return s.exceptional_d2iz(scaled,out,e);
}
bool BatchNodeCompilerSourceV96::reject(std::string& e,const char* leaf){if(!failed_){failed_=true;failure_=e.empty()?std::string("Required original batch compiler: ")+leaf:e;}e=failure_;return false;}
bool BatchNodeCompilerSourceV96::current(std::string& e){if(failed_){e=failure_;return false;}if(destroyed_||destroying_)return reject(e,"live original compiler receiver");if(!services_.provider||!services_.validate_current||!services_.validate_current(e))return reject(e,"SAME native source owner");if(failed_){e=failure_;return false;}return true;}
bool BatchNodeCompilerSourceV96::append_object(const BatchObjectBorrowV96& object,std::string& e){if(!current(e)||busy_||!object.identity||!object.owner)return reject(e,"real selected non-reentrant compiler GameObject");objects10_.push_back(object.identity);return true;}
bool BatchNodeCompilerSourceV96::map_node(std::uintptr_t object,const BatchNodeBorrowV96& node,std::string& e){
 if(!node.identity)return true;if(!node.owner||!node.child_count||!node.child)return reject(e,"SAME ISceneNode ordered childf4 view");
 node_objects1c_[node.identity]=object; //Original operator[] overwrite FIRST.
 for(std::size_t cursor=0;;++cursor){std::size_t count{};if(!node.child_count(count,e)||!current(e))return reject(e,"actual child listf4");if(cursor>=count)break;
  BatchNodeBorrowV96 child;if(!node.child(cursor,child,e)||!current(e)||!map_node(object,child,e))return reject(e,"actual recursive _MapMeshNode");}
 return true;
}
bool BatchNodeCompilerSourceV96::build_map(std::string& e){
 if(!current(e)||busy_)return reject(e,"non-reentrant actual _LoadBatchMap");busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
 for(auto id:objects10_){BatchObjectBorrowV96 object;if(!services_.object||!services_.object(id,object,e)||object.identity!=id||!object.visual2d8||!current(e))return reject(e,"actual selected visual2d8 receiver");
  if(!*object.visual2d8)continue;BatchNodeBorrowV96 root;if(!services_.visual_root||!services_.visual_root(object,root,e)||!current(e)||!map_node(id,root,e))return reject(e,"actual VisualObject.root8 / node map");}
 return true;
}
bool BatchNodeCompilerSourceV96::visible_recursive(const BatchNodeBorrowV96& node,bool value,std::string& e){
 if(!node.identity)return true;if(!node.owner||!node.set_visible48||!node.set_visible48(value,e)||!current(e)||!node.child_count||!node.child)return reject(e,"actual selected ISceneNode.setVisible");
 for(std::size_t cursor=0;;++cursor){std::size_t count{};if(!node.child_count(count,e)||!current(e))return reject(e,"actual visibility childf4");if(cursor>=count)break;
  BatchNodeBorrowV96 child;if(!node.child(cursor,child,e)||!current(e)||!visible_recursive(child,value,e))return reject(e,"actual recursive node visibility");}
 return true;
}
bool BatchNodeCompilerSourceV96::no_batch_visible(const BatchNodeBorrowV96& node,bool value,bool& found,std::string& e){
 if(!node.identity||!node.owner||!node.name24||!node.child_count||!node.child)return reject(e,"actual node name24/childf4");
 found=std::strstr(node.name24->c_str(),"nobatch")!=nullptr;
 if(found&&!visible_recursive(node,value,e))return false;
 for(std::size_t cursor=0;;++cursor){std::size_t count{};if(!node.child_count(count,e)||!current(e))return reject(e,"actual nobatch childf4");if(cursor>=count)break;
  BatchNodeBorrowV96 child;if(!node.child(cursor,child,e)||!current(e))return reject(e,"actual nobatch child receiver");
  bool nested{};if(!no_batch_visible(child,value,nested,e))return false;found=found||nested;}
 return true;
}
bool BatchNodeCompilerSourceV96::linked(std::uintptr_t mesh,std::uintptr_t segment,std::string& e){
 std::uintptr_t node{};BatchSegmentBorrowV96 target;
 if(!services_.current_rendered_node9c||!services_.current_rendered_node9c(node,e)||!current(e)||
  !services_.segment||!services_.segment(mesh,segment,target,e)||!current(e)||!target.owner||!target.game_object2c)return reject(e,"actual current-rendered9c / mesh segment2c");
 const auto found=node_objects1c_.find(node);const auto object=found==node_objects1c_.end()?0:found->second;
 *target.game_object2c=object; //Native writesNULL BEFORE its unsafe assertion.
 if(!object)return reject(e,"original linked batch segment NULL GameObject assertion/dereference");
 BatchObjectBorrowV96 actual;if(!services_.object||!services_.object(object,actual,e)||actual.identity!=object||!actual.linked2fc||!current(e))return reject(e,"SAME linked GameObject2fc");
 *actual.linked2fc=1;return true;
}
bool BatchNodeCompilerSourceV96::trim(std::string& e){if(!current(e))return false;objects10_.clear();std::vector<std::uintptr_t>().swap(objects10_);node_objects1c_.clear();return true;}
bool BatchNodeCompilerSourceV96::compile(bool quantize,std::string& e){
 if(!current(e)||busy_)return reject(e,"non-reentrant actual compiler.Compile");busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
 if(!root34_.identity){
  if(!services_.construct_mesh||!services_.construct_mesh(0x30,temporary_mesh_,e)||!temporary_mesh_.owner||!temporary_mesh_.identity||!temporary_mesh_.grab||!temporary_mesh_.drop||!current(e))return reject(e,"actual CBatchMesh(attribute48) C1");
  if(!temporary_mesh_.grab(e)||!current(e))return reject(e,"actual temporary intrusive mesh grab");
  if(!services_.construct_root||!services_.construct_root(-1,temporary_mesh_,pending_root_,e)||!pending_root_.owner||!pending_root_.identity||pending_root_.node.identity!=pending_root_.identity||!pending_root_.mesh130||!current(e))return reject(e,"actual CBatchSceneNode(-1,SAME mesh) C1");
  root34_=std::move(pending_root_);pending_root_={};
  //Original publishes compiler34 AFTER completed C1, BEFORE these selected resource calls.
  if(!root34_.node.set_visible48||!root34_.node.set_visible48(true,e)||!current(e))return reject(e,"actual batch node.setVisible(true)");
  if(!root34_.set_automatic_culling||!root34_.set_automatic_culling(0,e)||!current(e)||!root34_.source_word138)return reject(e,"actual batch node culling/source-word138");
  *root34_.source_word138=2;
  if(!temporary_mesh_.drop(e))return reject(e,"actual temporary CBatchMesh drop");
  temp_drop_complete_=true;temporary_mesh_={}; //Record actual release BEFORE continuation.
  if(!current(e))return reject(e,"actual temporary drop continuation");
 }
 std::vector<BatchNodeBorrowV96> roots;
 for(auto id:objects10_){BatchObjectBorrowV96 object;if(!services_.object||!services_.object(id,object,e)||object.identity!=id||!object.visual2d8||!current(e))return reject(e,"actual compiler GameObject visual2d8");
  if(!*object.visual2d8)continue;BatchNodeBorrowV96 root;if(!services_.visual_root||!services_.visual_root(object,root,e)||!current(e))return reject(e,"actual compiler VisualObject.root8");
  if(!root.identity)continue;
  if(!services_.scene_add_child5c||!services_.scene_add_child5c(root,e)||!current(e))return reject(e,"actual Scene.root4.addChild original root");
  bool nobatch{};if(!no_batch_visible(root,false,nobatch,e))return false;
  if(nobatch&&(!services_.trace||!services_.trace("isTracingBatchingCompiler",e)||!current(e)))return reject(e,"fresh nobatch source trace");
  roots.push_back(std::move(root));
 }
 if(!roots.empty()){
  BatchLinkedCallbackV96 callback=[this](auto mesh,auto segment,std::string& e){return linked(mesh,segment,e);};
  if(!services_.scene_compile50||!services_.scene_compile50(roots,root34_,false,callback,nullptr,{0,0,0},e)||!current(e))return reject(e,"actual CSceneManager.compile vector/root/callback/NULL split/zeroPoint");
  if(quantize){std::uint32_t type{};if(!services_.driver_type5c||!services_.driver_type5c(type,e)||!current(e))return reject(e,"actual selected driver5c");
   if(type&7u){if(!services_.quantize_components||!services_.quantize_components(root34_.mesh130,false,true,e)||!current(e))return reject(e,"actual CBatchMesh.quantizeComponents(false,true)");}}
  if(!services_.flush_mesh_buffers||!services_.flush_mesh_buffers(root34_.mesh130,true,false,false,e)||!current(e))return reject(e,"actual FlushMeshBuffers(true,false,false)");
  compiled0_=true;
 }
 for(auto id:objects10_){BatchObjectBorrowV96 object;if(!services_.object||!services_.object(id,object,e)||object.identity!=id||!object.visual2d8||!current(e))return reject(e,"actual postcompile GameObject visual");
  if(!*object.visual2d8)continue;BatchNodeBorrowV96 root;if(!services_.visual_root||!services_.visual_root(object,root,e)||!current(e))return reject(e,"actual postcompile root8");
  bool nobatch{};if(!no_batch_visible(root,true,nobatch,e))return false;
  if(nobatch){if(!services_.trace||!services_.trace("isTracingBatchingCompiler",e)||!current(e))return reject(e,"fresh restored nobatch source trace");continue;}
  if(!object.archetype48)return reject(e,"actual postcompile source archetype48");
  const auto* type=object.archetype48->c_str();
  //50df94..50dfbc: only Decor/Module enter SetVisualObject(NULL).
  if(std::strcmp(type,"Decor")&&std::strcmp(type,"Module"))continue;
  if(!services_.set_visual_null||!services_.set_visual_null(object,e)||!current(e))return reject(e,"actual GameObject.SetVisualObject(NULL) ownership body");
 }
 return trim(e);
}
bool BatchNodeCompilerSourceV96::finish_scene_attachment(std::string& e){
 if(!current(e)||busy_||!root34_.identity)return reject(e,"actual non-reentrant compiler.root34 suffix");
 busy_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{busy_};
 if(compiled0_&&(!services_.scene_add_child5c||!services_.scene_add_child5c(root34_.node,e)||!current(e)))return reject(e,"actual Scene.root4.addChild compiled root");
 if(!root34_.node.set_visible48||!root34_.node.set_visible48(true,e)||!current(e))return reject(e,"actual compiler root.setVisible(true)");return true;
}
bool BatchNodeCompilerSourceV96::destroy_source(std::string& e){
 if(destroyed_){e.clear();return true;}if(busy_||destroying_)return required(e,"no native compiler D1 during/reentrant delivery");
 destroying_=true;struct Guard{bool& b;~Guard(){b=false;}} guard{destroying_};
 if(root34_.identity&&!root_drop_complete_){if(!root34_.drop||!root34_.drop(e))return required(e,"actual compiler34 intrusive drop with native quiescence");root_drop_complete_=true;}
 root34_={}; //Original Free nulls34 only AFTER genuine drop.
 if(pending_root_.identity){if(!pending_root_.drop||!pending_root_.drop(e))return required(e,"actual interrupted batch-node C1 prefix release");pending_root_={};}
 if(temporary_mesh_.identity&&!temp_drop_complete_){if(!temporary_mesh_.drop||!temporary_mesh_.drop(e))return required(e,"actual interrupted constructor temporary mesh drop");temp_drop_complete_=true;}
 temporary_mesh_={};objects10_.clear();node_objects1c_.clear();
 //D1 then releases actual vector allocation; source compiled0 is not reset by Free.
 std::vector<std::uintptr_t>().swap(objects10_);services_={};destroyed_=true;e.clear();return true;
}
bool release_level_batch_compiler_v96(const std::shared_ptr<CanonicalLevelContextV1>& level,std::string& e){
 if(!level){e="Required SAME original Level158 receiver";return false;}
 auto native=level->constructor_borrow_v3();auto& owner=level->batch_compiler_owner_slot_v96();
 if(!native.fields){e="Unproduced original Level158 field";return false;}
 if(!native.fields->field158&&!owner){e.clear();return true;}
 if(!owner||native.fields->field158!=owner->identity()){e="Required SAME typed original BatchNodeCompiler158 owner, no rawcast/adopted substitute";return false;}
 auto held=owner;const auto expected=held->identity(); //Pin independently of mutable Level slot.
 if(!held->destroy_source(e))return false;
 auto after=level->constructor_borrow_v3();
 if(!after.fields||after.fields->field158!=expected||!owner||owner.get()!=held.get()||
  owner.owner_before(held)||held.owner_before(owner)){e="Original compiler D1 completed, but Level158 owner changed; replacement retained";return false;}
 after.fields->field158=0;owner.reset();e.clear();return true;
}
}
