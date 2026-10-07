#include "native_batch_resources_v110.hpp"
#include "native_batch_compiler_v111.hpp"
#include <limits>
namespace dh2::world {
namespace {bool required(std::string& e,const char* message){if(e.empty())e=message;return false;}}
bool NativeBatchMeshV110::grab(std::string& e){
 if(!live()||references4_==UINT32_MAX)return required(e,"Invalid native batch-mesh grab");
 ++references4_;e.clear();return true;
}
bool NativeBatchMeshV110::drop(std::string& e){
 if(!live()||!references4_)return required(e,"Native batch-mesh drop without a real reference");
 if(references4_>1){--references4_;e.clear();return true;}
 if(!lifetime_.owner||!lifetime_.require_release||!lifetime_.require_release(e))return false;
 //Positive segment/data allocations require the completed compiler/resource
 //destructor. The C1 empty resource is a complete, genuine release domain.
 if(compiled_v111_&&!compiled_v111_->release(identity(),e))return false;
 if(!segments_.empty()&&!compiled_v111_)return required(e,"Compiled segments lost actual resource D1 owner");
 compiled_v111_.reset();
 destroying_=true;references4_=0;
 std::vector<NativeBatchSegmentV110>().swap(segments_);
 destroyed_=true;destroying_=false;e.clear();return true;
}
bool NativeBatchMeshV110::source_update_bounds_v111(const std::array<float,6>& box,std::string& e){
 if(!live()||!compiled_v111_)return required(e,"Actual compiled mesh bounding-box owner");bounds38_=box;bounds50_=box;e.clear();return true;
}
bool NativeBatchMeshV110::publish_compiled_v111(std::shared_ptr<NativeBatchCompiledV111> data,std::string& e){
 if(!live()||compiled_v111_||!data)return required(e,"Required fresh actual batch compiled resource");compiled_v111_=std::move(data);e.clear();return true;
}
bool NativeBatchMeshV110::borrow(loader::BatchMeshBorrowV96& out,std::string& e){
 out={};if(!live())return required(e,"Retired native CBatchMesh allocation");
 auto self=shared_from_this();out.owner=self;out.identity=identity();
 out.grab=[self](auto& e){return self->grab(e);};out.drop=[self](auto& e){return self->drop(e);};e.clear();return true;
}
bool NativeBatchMeshV110::segment(std::size_t index,loader::BatchSegmentBorrowV96& out,std::string& e){
 out={};if(!live()||index>=segments_.size())return required(e,"Batch segment outside real compiled resource");
 out.owner=shared_from_this();out.game_object2c=&segments_[index].game_object2c;e.clear();return true;
}
bool NativeBatchNodeV110::initialize_mesh(std::shared_ptr<NativeBatchMeshV110> mesh,std::string& e){
 if(!live()||mesh130_||mesh_grabbed_||!mesh||!mesh->live())return required(e,"Required fresh batch node and actual CBatchMesh");
 mesh130_=std::move(mesh); //57f1bc: actual130 store BEFORE native mesh grab.
 if(!mesh130_->grab(e))return false;mesh_grabbed_=true;e.clear();return true;
}
bool NativeBatchNodeV110::grab(std::string& e){
 if(!live()||references164_==UINT32_MAX)return required(e,"Invalid native batch-node grab");
 ++references164_;e.clear();return true;
}
bool NativeBatchNodeV110::drop(std::string& e){
 if(!live()||!references164_)return required(e,"Native batch-node drop without a real reference");
 if(references164_>1){--references164_;e.clear();return true;}
 if(parentec_)return required(e,"Batch node has live Scene parent without its native reference");
 if(!lifetime_.owner||!lifetime_.require_release||!lifetime_.require_release(e))return false;
 destroying_=true;
 if(mesh_grabbed_&&!mesh130_->drop(e)){destroying_=false;return false;}
 mesh_grabbed_=false;mesh130_.reset();references164_=0;manager110_=0;
 std::string{}.swap(name_);notify_visibility_={};destroyed_=true;destroying_=false;e.clear();return true;
}
bool NativeBatchNodeV110::set_visible(bool visible,std::string& e){
 if(!live())return required(e,"Retired native batch visibility receiver");
 const auto before=flags11c_&1u;local120_=visible?1:0;
 if(local120_&&parent121_)flags11c_|=1u;else flags11c_&=~1u;
 if(before!=(flags11c_&1u)&&notify_visibility_)notify_visibility_();e.clear();return true;
}
bool NativeBatchNodeV110::notify_visibility(bool visible,std::string& e){
 if(!live())return required(e,"Retired native batch parent visibility receiver");
 const auto before=flags11c_&1u;parent121_=visible?1:0;
 if(local120_&&parent121_)flags11c_|=1u;else flags11c_&=~1u;
 if(before!=(flags11c_&1u)&&notify_visibility_)notify_visibility_();e.clear();return true;
}
bool NativeBatchNodeV110::borrow(loader::BatchRootBorrowV96& out,std::string& e){
 out={};if(!live()||!mesh130_||!mesh_grabbed_)return required(e,"Incomplete native CBatchSceneNode C1 prefix");
 auto self=shared_from_this();out.owner=self;out.identity=identity();out.native_v111=self;out.mesh130=mesh130_->identity();out.source_word138=&word138_;
 out.node.owner=self;out.node.identity=identity();out.node.name24=&name_;
 out.node.child_count=[self](std::size_t& count,auto& e){if(!self->live())return required(e,"Retired batch child list");count=0;e.clear();return true;};
 out.node.child=[self](std::size_t,loader::BatchNodeBorrowV96&,auto& e){return required(e,self->live()?"Batch C1 child list is empty":"Retired batch child list");};
 out.node.set_visible48=[self](bool visible,auto& e){return self->set_visible(visible,e);};
 out.set_automatic_culling=[self](std::uint32_t value,auto& e){if(!self->live())return required(e,"Retired batch culling receiver");self->culling118_=value;e.clear();return true;};
 out.drop=[self](auto& e){return self->drop(e);};e.clear();return true;
}
bool NativeBatchNodeV110::scene_loan(const std::shared_ptr<GameObjectSceneRootRegistryV1>& scene,GameObjectSceneRootBorrowV1& out,std::string& e){
 out={};if(!live()||!scene||!mesh130_||!mesh_grabbed_)return required(e,"Required actual native batch root/Scene");
 auto self=shared_from_this();const auto weak=std::weak_ptr<GameObjectSceneRootRegistryV1>(scene);
 notify_visibility_=[weak]{if(auto scene=weak.lock())scene->notify_visibility_changed_v3();};
 out.owner=self;out.identity=identity();out.flags11c=&flags11c_;out.parentec=&parentec_;
 out.notify_visibility=[self](bool visible,auto& e){return self->notify_visibility(visible,e);};
 out.modular_receivers_v114=[self](auto&,auto& e){if(!self->live())return required(e,"Retired native batch type search");e.clear();return true;}; //actual compiled static/skinned mesh, not CModularSkinnedMeshSceneNode
 //Genuine ISceneNode C1 empty animator lists. Compilation must provide any
 //later animator/child domains rather than inheriting this empty constructor.
 out.remove_animators=[self](auto& e){if(!self->live())return required(e,"Retired batch animator owner");e.clear();return true;};
 out.scene_manager_changed=[self,weak](std::uintptr_t manager,auto& e){auto scene=weak.lock();
  if(!self->live()||!scene||manager!=scene->scene_manager_identity_v16())return required(e,"Replaced batch SceneManager");self->manager110_=manager;e.clear();return true;};
 out.acquire_parent_reference_v110=[self](auto& e){return self->grab(e);};
 out.release_parent_reference_v110=[self](auto& e){return self->drop(e);};
 out.scene_phase_v69=[self](auto phase,auto& e){return self->source_static_frame_v111(phase,e);};
 e.clear();return true;
}
bool NativeBatchNodeV110::source_static_frame_v111(std::uint32_t phase,std::string& e){
 if(!live()||!mesh130_||!mesh130_->live())return required(e,"Retired compiled scene frame receiver");
 (void)phase; //Actual onAnimate receives Timer milliseconds, not a phase enum.
 if(!((flags11c_&0x400u)&&!(flags11c_&1u))&&mesh130_->compiled_v111()&&!mesh130_->compiled_v111()->animate_v112(phase,e))return false;
 //The native compile owner now supplies retained dynamic animation delivery;
 //static C1 remains animator-empty. Both use the SAME source Scene timer and
 //original default root transform, never a second playback clock.
 if(!((flags11c_&0x400u)&&!(flags11c_&1u)))flags11c_=(flags11c_|0x120u)&~0x5eu;
 e.clear();return true;
}
bool NativeBatchNodeV110::source_compile_setup_v112(std::uint32_t solid,std::string& e){
 if(!live()||!mesh130_||!mesh130_->compiled_v111()||solid>INT32_MAX)return required(e,"Actual batch setup/sort result");
 word15c_=word144_=static_cast<std::int32_t>(solid);e.clear();return true;
}

}
