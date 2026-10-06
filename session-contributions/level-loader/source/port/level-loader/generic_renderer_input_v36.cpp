#include "generic_renderer_input_v36.hpp"
#include <algorithm>
#include <limits>
namespace dh2::loader {
namespace {
bool registered_roots(const std::vector<ModuleDrawFrameV1>& frames,
 const world::GameObjectSceneRootRegistryV1& roots,std::string& e){
 const auto registered=roots.roots();
 for(const auto& frame:frames){
  if(!frame.root_identity||!frame.visual_identity||!frame.resource||
     std::find(registered.begin(),registered.end(),frame.root_identity)==registered.end()){
   e="Required SAME registered Module visual root/resource";return false;
  }
 }
 return true;
}
void append_sources(const CanonicalCachedFileV1& file,
 std::vector<GenericRendererSourceV36>& out){
 for(const auto& attempt:file.attempts()){
  GenericRendererSourceV36 source;source.authored=attempt->source().entry();
  const auto request=attempt->source().request();
  source.module_occurrence=request.module_occurrence;source.module_id=request.runtime_module_id;
  source.module_offset=request.module_offset;
  if(const auto* factory=attempt->factory_attempt()){
   source.registry_key=factory->handle().key;source.construction_stage=factory->stage();
  }
  out.push_back(std::move(source));
 }
}
}
bool capture_generic_renderer_input_v36(const std::shared_ptr<RetainedLevelModuleGraphV1>& graph,
 const std::shared_ptr<world::GameObjectSceneRootRegistryV1>& roots,
 GenericRendererInputV36& out,std::string& e){
 if(!graph||!roots||!graph->level()||!graph->floor_world()||
    !graph->status().geometry_prepared||!graph->status().floors_prepared){
  e="Required actual retained Module geometry/floor graph and scene-root registry";return false;
 }
 GenericRendererInputV36 candidate;
 if(!graph->capture_draw_frames(candidate.modules_,e)||!registered_roots(candidate.modules_,*roots,e))return false;
 candidate.graph_=graph;candidate.roots_=roots;candidate.level_identity_=graph->level()->identity();
 candidate.request_=graph->level()->source_request();candidate.status_=graph->status();candidate.floors_=graph->floor_world();
 auto& manager=graph->manager();const auto end=manager.source_next_key4c();
 candidate.registry_next_key_=end;candidate.registry_count_=manager.source_count50();
 if(end>std::uint32_t(std::numeric_limits<std::int32_t>::max())){e="Registry key range exceeds canonical signed handle domain";return false;}
 for(std::uint32_t key=1;key<end;++key){
  const auto* receiver=manager.object(std::int32_t(key));if(!receiver)continue;
  if(!receiver->lease||!receiver->identity||!receiver->shared_handle||receiver->shared_handle->key!=std::int32_t(key)||!receiver->type_f4){
   e="Required SAME canonical object fields/lease/handle";return false;
  }
  candidate.objects_.push_back({std::int32_t(key),*receiver});
 }
 append_sources(graph->root_file(),candidate.authored_);
 const auto files=graph->module_files();
 if(!files||files->discarded()){e="Required retained original Module source journals";return false;}
 for(std::size_t i=0;i<files->file_count();++i)append_sources(files->file(i),candidate.authored_);
 out=std::move(candidate);e.clear();return true;
}
bool GenericRendererInputV36::validate_live(const std::shared_ptr<RetainedLevelModuleGraphV1>& graph,std::string& e)const{
 const auto captured=graph_.lock();const auto roots=roots_.lock();
 if(!graph||!captured||captured!=graph||!roots||!graph->level()||graph->level()->identity()!=level_identity_||graph->floor_world()!=floors_){
  e="Renderer input belongs to another/released retained Level graph";return false;
 }
 std::vector<ModuleDrawFrameV1> live;
 if(!graph->capture_draw_frames(live,e)||!registered_roots(live,*roots,e))return false;
 if(live.size()!=modules_.size()){e="Renderer Module membership changed; recapture input";return false;}
 for(std::size_t i=0;i<live.size();++i)if(live[i].root_identity!=modules_[i].root_identity||live[i].visual_identity!=modules_[i].visual_identity||live[i].module_id!=modules_[i].module_id){
  e="Renderer visual/root identity changed; recapture input";return false;
 }
 if(graph->manager().source_next_key4c()!=registry_next_key_||graph->manager().source_count50()!=registry_count_){
  e="Renderer canonical registry membership changed; recapture input";return false;
 }
 for(const auto& object:objects_){
  const auto* current=graph->manager().object(object.registry_key);
  if(!current||current->identity!=object.receiver.identity||current->lease!=object.receiver.lease||current->shared_handle!=object.receiver.shared_handle||!current->shared_handle||current->shared_handle->key!=object.registry_key){
   e="Renderer canonical registry handle became stale; recapture input";return false;
  }
 }
 e.clear();return true;
}
bool GenericRendererInputV36::borrow_navigation(const std::shared_ptr<RetainedLevelModuleGraphV1>& graph,
 std::shared_ptr<floors::World>& out,std::string& e)const{
 if(!validate_live(graph,e))return false;out=floors_;return true;
}
bool GenericRendererInputV36::borrow_object(const std::shared_ptr<RetainedLevelModuleGraphV1>& graph,
 std::int32_t key,world::CanonicalObjectBorrowV1& out,std::string& e)const{
 if(!validate_live(graph,e))return false;
 const auto found=std::find_if(objects_.begin(),objects_.end(),[key](const auto& v){return v.registry_key==key;});
 if(found==objects_.end()){e="Requested key is absent from this captured canonical registry";return false;}
 out=found->receiver;return true;
}
}
