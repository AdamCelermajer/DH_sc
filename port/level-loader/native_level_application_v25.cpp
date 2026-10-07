#include "native_level_application_v25.hpp"
#include <stdexcept>
namespace dh2::loader {
namespace {
struct ScriptFilesLeaseV25 {
 NativeLevelFilesV25 files;
 static bool read(void* p,const std::string& uri,std::vector<std::uint8_t>& bytes,bool& found,std::string& e){
  auto& self=*static_cast<ScriptFilesLeaseV25*>(p);
  if(!self.files.owner||!self.files.script){e="Required actual Application Lua file/cache provider";return false;}
  return self.files.script(uri,bytes,found,e);
 }
};
}
NativeLevelApplicationV25::NativeLevelApplicationV25(NativeLevelFilesV25 files):files_(std::move(files)){
 if(!files_.owner||!files_.script||!files_.saved)throw std::invalid_argument("Required actual Application Level file providers");
 // Cache pins independent file storage, not this Application facet/Level.
 auto retained=std::make_shared<ScriptFilesLeaseV25>();retained->files=files_;
 scripts_=std::make_shared<scripts::LuaScriptCacheOwnerV13>(scripts::LuaScriptCacheServicesV13{retained,retained.get(),ScriptFilesLeaseV25::read});
}
bool NativeLevelApplicationV25::saved(void* p,const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
 return static_cast<NativeLevelApplicationV25*>(p)->files_.saved(name,found,bytes,e);
}
LevelConstructorApplicationV4 NativeLevelApplicationV25::bind(LevelConstructorApplicationV4 a){
 if(!a.owner)throw std::invalid_argument("Required actual Application Level constructor lease");
 struct Lease {std::shared_ptr<NativeLevelApplicationV25> facet;std::shared_ptr<void> actual;level::LevelSavegameCacheServicesV1 prior;};
 auto lease=std::make_shared<Lease>(Lease{shared_from_this(),a.owner,a.saves.files});a.owner=lease;
 a.debug_level_load_count=&debug_level_load_count;a.module_id_global=&module_globals.next_module_id;a.lua_cache=scripts_;
 // Pins facet and actual application, without borrowing a containing Level.
 //Read and OBJS callbacks have distinct real providers. Preserve both while
 //retaining one cache services context; do not replace OBJS's raw context
 //with this facet and later cast the unrelated receiver as a save transport.
 a.saves.files.context=lease.get();
 a.saves.files.read_file=[](void* p,const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){return NativeLevelApplicationV25::saved(static_cast<Lease*>(p)->facet.get(),name,found,bytes,e);};
 if(lease->prior.load_objects)a.saves.files.load_objects=[](void* p,data::Bytes bytes,level::LevelSavegameFieldsV1& fields,std::string& e){auto& t=*static_cast<Lease*>(p);return t.prior.load_objects(t.prior.context,bytes,fields,e);};
 if(lease->prior.load_objects_stream_v2)a.saves.files.load_objects_stream_v2=[](void* p,data::Bytes bytes,std::uint64_t offset,level::LevelSavegameFieldsV1& fields,std::string& e){auto& t=*static_cast<Lease*>(p);return t.prior.load_objects_stream_v2(t.prior.context,bytes,offset,fields,e);};
 a.saves.files.storage_lease=a.owner;
 return a;
}
bool NativeLevelConnectionV25::allocate(LevelSourceRequestV1 request,std::shared_ptr<void> actual_application,std::string& e){
 if(allocation_attempted_){e=error_.empty()?"Same native Level allocation already attempted":error_;return false;}
 allocation_attempted_=true;
 if(!CanonicalLevelContextV1::create(std::move(request),std::move(actual_application),candidate_,e)){error_=e;return false;}
 error_.clear();return true;
}
bool NativeLevelConnectionV25::abort_unpublished_constructor_v114(std::string& e){
 if(complete_){e="Completed Level C1 requires its qualified Level release, not constructor unwind";return false;}
 if(!candidate_){e.clear();return true;} // Allocation failed before a receiver existed.
 auto b=candidate_->constructor_borrow_v3();
 if(!b.fields){e="Required retained failed Level constructor fields";return false;}
 const auto& f=*b.fields;
 // This operation cannot erase resources created by Level.Init/loading.
 if(f.field128||f.field12c||f.field130||f.field140||f.field14c||
    f.field154||f.field158||f.field15c||f.field194||
    candidate_->assigned_source_owner_slot_v65()||candidate_->batch_compiler_owner_slot_v96()){
  e="Level Init resources require the existing full release owner";return false;
 }
 if(bindings_&&!bindings_->release_unpublished_prefix_v114(std::move(b),e))return false;
 if(!bindings_&&(f.script44||f.save_ec||f.events)){
  e="Unpublished constructor resources lost their actual bindings";return false;
 }
 e.clear();return true;
}
bool NativeLevelConnectionV25::construct_allocated(LevelConstructorArgumentsV3 arguments,LevelConstructorApplicationV4 app,std::string& e){
 // Modern source identity (SWAMP) is distinct from original C1 filename.
 // Seed equality remains an explicit source-transaction consistency policy.
 if(attempted_){e=error_.empty()?"Same native Level C1 already attempted":error_;return false;}
 attempted_=true;
 if(!candidate_||!app.owner||!arguments.name||candidate_->source_request().seed!=arguments.seed){
  e=error_="Native Level C1 allocation/name/seed consistency differs";return false;
 }
 bindings_=LevelConstructorBindingsV4::create(std::move(app),e);
 if(!bindings_){error_=e;return false;}
 if(!candidate_->construct_source_v3(arguments,bindings_->services(),e)){error_=e;return false;}
 const auto* ctor=candidate_->constructor_owner_v3();
 if(!ctor||ctor->phase()!=LevelConstructorPhaseV3::complete){e=error_="Actual source Level C1 completion unavailable";return false;}
 complete_=true;error_.clear();e.clear();return true;
}
bool NativeLevelConnectionV25::construct(LevelSourceRequestV1 request,LevelConstructorArgumentsV3 arguments,LevelConstructorApplicationV4 app,std::string& e){
 if(!allocate(std::move(request),app.owner,e))return false;
 return construct_allocated(arguments,std::move(app),e);
}
}
