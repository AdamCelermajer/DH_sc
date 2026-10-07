#include "canonical_cached_file_v1.hpp"
#include <exception>
namespace dh2::loader {
CanonicalCachedFileV1::CanonicalCachedFileV1(assets::ZipAssetPackV1 archive,
    world::CanonicalObjectManagerV1& manager,world::CanonicalClassServicesV1 classes,
    CanonicalFileSourceServicesV1 source,CanonicalModuleContextV1 module,
    ObjectEntryRouteV1 route,std::optional<std::string> filter):
    file_(std::move(archive)),manager_(manager),classes_(classes),source_(std::move(source)),
    module_(std::move(module)),route_(route),filter_(std::move(filter)){}
bool CanonicalCachedFileV1::parse_result(bool ok,std::string& e){
    if(!source_.parse_result){e="required canonical XML parser-result provider";return false;}
    const bool result=source_.parse_result(ok,e);
    if(failed_){e=error_;return false;}
    return result;
}
bool CanonicalCachedFileV1::release_load_state(std::string& e){
    if(!source_.release_load_state){e="required canonical XML load-state release provider";return false;}
    return source_.release_load_state(e);
}
bool CanonicalCachedFileV1::load_element(const XmlDocumentV1::Borrow& document,
    std::uint32_t element,std::string& e){
    if(source_.before_element){
        const bool ready=source_.before_element(document,element,e);
        if(failed_){e=error_;return false;}
        if(!ready)return false;
    }
    CanonicalSourceBindingV1 binding;
    if(!prepare_canonical_source_binding_v1(document,element,route_,filter_,module_,binding,e))return false;
    attempts_.push_back(std::make_unique<CanonicalBoundSourceAttemptV1>(std::move(binding)));
    return attempts_.back()->execute(manager_,classes_,e);
}
LevelFileWalkStepV1 CanonicalCachedFileV1::step(const std::string& uri,const std::string& root){
    if(discarded_){error_="canonical file occurrence already discarded";return LevelFileWalkStepV1::failed;}
    if(failed_)return LevelFileWalkStepV1::failed;
    if(dispatching_){failed_=true;error_="reentrant canonical source-file delivery";return LevelFileWalkStepV1::failed;}
    if(discard_requested_){failed_=true;error_="canonical candidate release requested; source delivery cannot resume";return LevelFileWalkStepV1::failed;}
    if(started_&&(uri!=uri_||root!=root_)){
        failed_=true;error_="canonical source occurrence changed URI/root";
        return LevelFileWalkStepV1::failed;
    }
    if(completed_)return LevelFileWalkStepV1::complete;
    if(!module_.candidate_owner){failed_=true;error_="required retained canonical candidate context";return LevelFileWalkStepV1::failed;}
    started_=true;uri_=uri;root_=root;
    dispatching_=true;
    struct ResetDispatch {bool& flag;~ResetDispatch(){flag=false;}} guard{dispatching_};
    LevelFileWalkStepV1 result;
    try{result=file_.step(uri,root,*this);}
    catch(const std::exception& e){
        if(!failed_){failed_=true;error_=e.what();}
        return LevelFileWalkStepV1::failed;
    }
    // Preserve the first recursive failure rather than replacing its source
    // diagnostic or returning pending after the outer callback finishes.
    if(failed_)return LevelFileWalkStepV1::failed;
    if(result==LevelFileWalkStepV1::failed){failed_=true;error_=file_.error();}
    else if(result==LevelFileWalkStepV1::complete)completed_=true;
    return result;
}
LevelFileWalkStepV1 CanonicalCachedFileV1::step_document(XmlDocumentV1::Borrow document,std::shared_ptr<const void> owner,const std::string& root,CanonicalFileSourceServicesV1 source){
 if(discarded_){error_="canonical file occurrence already discarded";return LevelFileWalkStepV1::failed;}
 if(failed_)return LevelFileWalkStepV1::failed;
 if(dispatching_||discard_requested_){failed_=true;error_="assigned canonical delivery cannot reenter or resume release";return LevelFileWalkStepV1::failed;}
 if(!document||!owner||!module_.candidate_owner||!source.parse_result||!source.release_load_state||!source.before_element||!source.observe_walk){failed_=true;error_="Required actual assigned source/parser/cursor/release/candidate owners";return LevelFileWalkStepV1::failed;}
 if(started_&&(document.uri()!=uri_||root!=root_)){failed_=true;error_="canonical assigned source occurrence changed URI/root";return LevelFileWalkStepV1::failed;}
 if(completed_)return LevelFileWalkStepV1::complete;
 if(!started_){source_=std::move(source);started_=true;uri_=document.uri();root_=root;}
 dispatching_=true;struct Reset{bool& flag;~Reset(){flag=false;}} reset{dispatching_};
 LevelFileWalkStepV1 result;
 try{result=file_.step_document(std::move(document),std::move(owner),root,*this);}
 catch(const std::exception& ex){if(!failed_){failed_=true;error_=ex.what();}return LevelFileWalkStepV1::failed;}
 // First recursive failure stops further native cursor callbacks/result writes.
 if(failed_)return LevelFileWalkStepV1::failed;
 if(result==LevelFileWalkStepV1::pending){
  bool observed=false;
  try{observed=source_.observe_walk(file_.walk(),error_);}
  catch(const std::exception& ex){if(!failed_){failed_=true;error_=ex.what();}return LevelFileWalkStepV1::failed;}
  if(failed_)return LevelFileWalkStepV1::failed;
  if(!observed){failed_=true;return LevelFileWalkStepV1::failed;}
 }
 if(result==LevelFileWalkStepV1::failed){failed_=true;error_=file_.error();}
 else if(result==LevelFileWalkStepV1::complete)completed_=true;
 return result;
}
bool CanonicalCachedFileV1::discard_after_owner_release(
    const std::function<bool(std::string&)>& release,std::string& e){
    e.clear();if(discarded_)return true;
    if(dispatching_||discarding_){e="cannot reenter canonical file source release or release during delivery";return false;}
    discarding_=true;
    struct ResetDiscard {bool& flag;~ResetDiscard(){flag=false;}} guard{discarding_};
    discard_requested_=true;
    if(!owner_released_){
        if(!release){e="required canonical candidate release before source discard";return false;}
        if(!release(e))return false;
        owner_released_=true;
    }
    if(!file_.discard(*this,e))return false;
    attempts_.clear();module_.candidate_owner.reset();source_={};discarded_=true;
    return true;
}
}
