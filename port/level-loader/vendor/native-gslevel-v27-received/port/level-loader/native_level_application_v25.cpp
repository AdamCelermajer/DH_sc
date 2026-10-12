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
 struct Lease {std::shared_ptr<NativeLevelApplicationV25> facet;std::shared_ptr<void> actual;};
 a.owner=std::make_shared<Lease>(Lease{shared_from_this(),a.owner});
 a.debug_level_load_count=&debug_level_load_count;a.module_id_global=&module_globals.next_module_id;a.lua_cache=scripts_;
 // Pins facet and actual application, without borrowing a containing Level.
 a.saves.files.context=this;a.saves.files.read_file=saved;a.saves.files.storage_lease=a.owner;
 return a;
}
bool NativeLevelConnectionV25::allocate(LevelSourceRequestV1 request,std::shared_ptr<void> actual_application,std::string& e){
 if(allocation_attempted_){e=error_.empty()?"Same native Level allocation already attempted":error_;return false;}
 allocation_attempted_=true;
 if(!CanonicalLevelContextV1::create(std::move(request),std::move(actual_application),candidate_,e)){error_=e;return false;}
 error_.clear();return true;
}
bool NativeLevelConnectionV25::construct_allocated(LevelConstructorArgumentsV3 arguments,LevelConstructorApplicationV4 app,std::string& e){
 if(attempted_){e=error_.empty()?"Same native Level C1 already attempted":error_;return false;}
 attempted_=true;
 if(!candidate_||!app.owner||!arguments.name||candidate_->source_request().identity!=arguments.name||candidate_->source_request().seed!=arguments.seed){
  e=error_="Native Level C1 selected request/arguments identity differs";return false;
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
