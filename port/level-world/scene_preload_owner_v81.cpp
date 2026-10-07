#include "scene_preload_owner_v81.hpp"
#include "gameobject_scene_root_registry_v1.hpp"
#include <exception>
namespace dh2::world {
bool ScenePreloadCollectionV81::preload(const std::string& file,std::string& e){
 if(failed_){e=failure_;return false;}
 if(busy_){failed_=true;failure_=e="SceneManager.PreloadScene reentered native resource construction";return false;}
 busy_=true;
 const auto fail=[&](const std::string& error){failed_=true;failure_=error;busy_=false;e=failure_;return false;};
 try{
  if(!assets_.provider||!assets_.budget||!assets_.read||!assets_.maximum_roots||!assets_.maximum_encoded_bytes)
   return fail("Required SAME admitted SceneManager asset resource services");
  if(roots_.size()>=assets_.maximum_roots)return fail("Native SceneManager preloaded-root admission limit");
  auto root=std::make_shared<ScenePreloadedRootV81>();failed_prefix_=root;
  root->bytes_=std::make_shared<resources::AdmittedVectorV40>();bool found{};
  const auto before=[&](std::uint32_t bytes,std::string& error){
   if(bytes>assets_.maximum_encoded_bytes){error="Native SceneManager preload encoded resource limit";return false;}
   return root->bytes_->admission.reserve(assets_.budget,resources::ResourceScopeV37::actor,bytes,error);
  };
  if(!assets_.read(file,found,root->bytes_->bytes,before,e)){
   //The production ZIP reader publishes transactionally. Release its aborted
   //admission when it retained no payload; preserve any real partial producer
   //storage/accounting in the diagnostic failed prefix instead of fabricating
   //a fully unloaded resource after a nontransactional custom read.
   if(root->bytes_->bytes.empty())root->bytes_->admission.reset();
   else{std::string accounting;if(!root->bytes_->admission.commit(accounting))return fail(accounting);}
   return fail(e.empty()?"Actual Scene preload resource read failed":e);
  }
  if(!found){if(failed_)return fail(failure_);failed_prefix_.reset();busy_=false;e.clear();return true;} //Original LoadScene rootNULL branch.
  if(!root->bytes_->admission.commit(e))return fail(e);
  if(failed_)return fail(failure_);
  if(dh2_bres_open(&root->image_,root->bytes_->bytes.data(),root->bytes_->bytes.size())!=resources::BresError::ok)
   return fail("Invalid actual SceneManager preload BRES");
  if(!scene::load_authored_v76(root->image_,root->scene_,root->visibility_,e))return fail(e);
  //Construct the actual full static/skinned resource domain. Skin bindings use
  //the SAME decoded root graph and immutable BRES; no duplicate actor pose.
  for(const auto& instance:root->scene_.instances){
   assets::Mesh mesh{};
   if(dh2_mesh_open(&mesh,&root->image_,static_cast<std::int32_t>(instance.geometry))!=assets::Error::ok)
    return fail("Required valid authored Scene preload mesh");
   for(std::uint32_t p=0;p<mesh.primitives;++p){assets::Primitive primitive{};
    if(dh2_mesh_primitive(&mesh,static_cast<std::int32_t>(p),&primitive)!=assets::Error::ok)
     return fail("Required valid authored Scene preload primitive");
   }
   if(instance.controller>=0){skinning::Skin skin;
    if(!skinning::load(root->image_,static_cast<unsigned>(instance.controller),root->scene_,skin,e))return fail(e);
    if(skin.geometry!=instance.geometry)return fail("Scene preload controller/geometry identity mismatch");
    root->skins_.push_back(std::move(skin));
   }
  }
  root->fields_.file1bc=file;root->fields_.xref1d4.clear();root->complete_=true;
  roots_.push_back(root);failed_prefix_.reset();busy_=false;e.clear();return true;
 }catch(const std::exception& error){return fail(error.what());}
 catch(...){return fail("Scene preload native resource construction threw");}
}
void ScenePreloadCollectionV81::clear_at_scene_manager_clear()noexcept{
 roots_.clear();failed_prefix_.reset();failed_=false;failure_.clear();
}
bool GameObjectSceneRootRegistryV1::source_preload_collection_v81(std::shared_ptr<ScenePreloadCollectionV81> actual,std::string& e){
 if(!actual||(preloaded_scenes_v81_&&(preloaded_scenes_v81_.get()!=actual.get()||preloaded_scenes_v81_.owner_before(actual)||actual.owner_before(preloaded_scenes_v81_)))){e="SceneManager already retains a different actual preloaded-root collection";return false;}
 preloaded_scenes_v81_=std::move(actual);e.clear();return true;
}
void GameObjectSceneRootRegistryV1::source_clear_preloaded_scenes_v81()noexcept{
 if(preloaded_scenes_v81_)preloaded_scenes_v81_->clear_at_scene_manager_clear();
}
}
