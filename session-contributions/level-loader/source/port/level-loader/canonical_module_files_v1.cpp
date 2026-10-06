#include "canonical_module_files_v1.hpp"
#include <cstring>
#include <exception>
namespace dh2::loader {
CanonicalModuleFilesV1::CanonicalModuleFilesV1(assets::ZipAssetPackV1 archive,
    world::CanonicalObjectManagerV1& manager,world::CanonicalClassServicesV1 classes,
    CanonicalFileSourceServicesV1 source,std::shared_ptr<CanonicalLevelContextV1> level,
    std::shared_ptr<void> candidate,ObjectEntryRouteV1 route,std::optional<std::string> filter):
    archive_(std::move(archive)),manager_(manager),classes_(classes),source_(std::move(source)),
    level_(std::move(level)),candidate_owner_(std::move(candidate)),route_(route),filter_(std::move(filter)){}
bool CanonicalModuleFilesV1::create(assets::ZipAssetPackV1 archive,
    world::CanonicalObjectManagerV1& manager,world::CanonicalClassServicesV1 classes,
    CanonicalFileSourceServicesV1 source,std::shared_ptr<CanonicalLevelContextV1> level,
    std::shared_ptr<void> candidate,std::shared_ptr<CanonicalModuleFilesV1>& out,std::string& error,
    ObjectEntryRouteV1 route,std::optional<std::string> filter){
    error.clear();if(!archive.mounted()||!level||!candidate){
        error="required mounted Module source archive, SAME Level and canonical candidate lease";return false;
    }
    out=std::shared_ptr<CanonicalModuleFilesV1>(new CanonicalModuleFilesV1(std::move(archive),manager,
        classes,std::move(source),std::move(level),std::move(candidate),route,std::move(filter)));return true;
}
bool CanonicalModuleFilesV1::fail(const std::string& reason,std::string& error){
    if(!failed_){failed_=true;error_=reason;}error=error_;return false;
}
bool CanonicalModuleFilesV1::begin_module(std::uint32_t occurrence,std::string& error){
    error.clear();if(discard_requested_||discarded_)return fail("canonical Module source release requested",error);
    if(failed_){error=error_;return false;}
    if(dispatching_||file_pending_)return fail("canonical Module occurrence changed during pending source delivery",error);
    if(occurrence==UINT32_MAX)return fail("required actual Module diagnostic occurrence",error);
    occurrence_=occurrence;module_started_=true;captured_context_=false;captured_={};return true;
}
world::ModuleLevelLoadBorrowV1 CanonicalModuleFilesV1::load_borrow(){
    world::ModuleLevelLoadBorrowV1 borrow;if(discard_requested_||discarded_)return borrow;
    auto fields=level_->module_load_fields();borrow.owner=shared_from_this();
    borrow.object_module_id18c=fields.object_module_id18c;borrow.module_offset160=fields.module_offset160;
    std::weak_ptr<CanonicalModuleFilesV1> weak=shared_from_this();
    borrow.load_file=[weak](const std::string& uri,const char* root,bool& loaded,std::string& error){
        auto relay=weak.lock();if(!relay){loaded=false;error="released canonical Module file relay";return false;}
        return relay->load_file(uri,root,loaded,error);
    };return borrow;
}
bool CanonicalModuleFilesV1::load_file(const std::string& uri,const char* root,bool& loaded,std::string& error){
    loaded=false;error.clear();if(failed_){error=error_;return false;}
    if(discard_requested_||discarded_)return fail("canonical Module source release requested",error);
    if(dispatching_)return fail("reentrant canonical Module source delivery",error);
    if(!module_started_)return fail("required begun actual Module source occurrence",error);
    if(!root||uri.empty()||uri.find('\0')!=std::string::npos)return fail("required nonempty Module source URI and root",error);
    auto fields=level_->module_load_fields();
    if(!captured_context_){
        captured_.candidate_owner=candidate_owner_;captured_.occurrence=occurrence_;
        captured_.runtime_module_id=*fields.object_module_id18c;
        std::memcpy(captured_.class_position.data(),fields.module_offset160,12);captured_context_=true;
    }else if(captured_.runtime_module_id!=*fields.object_module_id18c||
        std::memcmp(captured_.class_position.data(),fields.module_offset160,12)!=0){
        return fail("SAME Level Module load fields changed during source occurrence",error);
    }
    if(file_pending_&&(uri!=active_uri_||root!=active_root_))return fail("pending canonical Module file URI/root changed",error);
    if(!file_pending_){
        files_.push_back(std::make_unique<CanonicalCachedFileV1>(archive_,manager_,classes_,source_,captured_,route_,filter_));
        active_uri_=uri;active_root_=root;file_pending_=true;
    }
    dispatching_=true;LevelFileWalkStepV1 step;
    try{step=files_.back()->step(uri,root);}catch(const std::exception& e){dispatching_=false;return fail(e.what(),error);}
    dispatching_=false;if(failed_){error=error_;return false;}
    if(step==LevelFileWalkStepV1::failed)return fail(files_.back()->error(),error);
    if(step==LevelFileWalkStepV1::complete){file_pending_=false;loaded=true;}
    return true;
}
bool CanonicalModuleFilesV1::discard_after_owner_release(
    const std::function<bool(std::string&)>& release,std::string& error){
    error.clear();if(discarded_)return true;
    if(dispatching_||discarding_){error="cannot reenter canonical Module source release or release during delivery";return false;}
    discarding_=true;
    struct ResetDiscardGuard {bool& flag;~ResetDiscardGuard(){flag=false;}} guard{discarding_};
    discard_requested_=true;
    if(!owner_released_){
        if(!release){error="required canonical candidate release before Module source discard";return false;}
        if(!release(error))return false;owner_released_=true;
    }
    while(discard_cursor_<files_.size()){
        // The real candidate release completed above. This acknowledges that
        // prefix to each retained file without executing it a second time.
        if(!files_[discard_cursor_]->discard_after_owner_release([](std::string&){return true;},error))return false;
        ++discard_cursor_;
    }
    files_.clear();captured_={};candidate_owner_.reset();classes_={};source_={};archive_={};
    file_pending_=false;discarded_=true;return true;
}
}
