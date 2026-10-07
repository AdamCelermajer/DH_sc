#include "cached_level_file_v1.hpp"
#include "resource_paths_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
LevelFileWalkStepV1 CachedLevelFileV1::step(const std::string& uri,const std::string& root,LevelFileWalkServicesV1& services) {
    if(failed_)return LevelFileWalkStepV1::failed;
    try {
        if(active_&&document_input_)throw std::runtime_error("Assigned stream occurrence cannot switch to ZIP acquisition");
        if(active_&&(uri!=requested_||root!=root_))
            throw std::runtime_error("Pending XML request changed: "+requested_+" -> "+uri);
        if(!active_) {
            active_=true;document_input_=false;assigned_source_owner_.reset();requested_=uri;root_=root;resolved_.clear();error_.clear();walk_={};acquired_.clear();
            if(!archive_.mounted())throw std::runtime_error("XML cache archive unavailable");
            std::vector<std::uint8_t> bytes;std::string error;
            for(const auto& candidate:compiled_level_paths_v1(uri)) {
                bool found=false;
                if(!archive_.read(candidate,found,bytes,error))
                    throw std::runtime_error("XML read failure in "+candidate+": "+error);
                if(found) {
                    if(!assets::ZipAssetPackV1::key(candidate,resolved_,error))throw std::runtime_error(error);
                    break;
                }
            }
            if(resolved_.empty())throw std::runtime_error("Missing authored XML: "+uri);
            acquired_=std::move(bytes);
            XmlDocumentV1 doc;
            if(!doc.capture_level_buffer(resolved_,acquired_,error)||
               !prepare_level_file_walk_v1(doc.borrow(),root_,walk_,error))throw std::runtime_error(error);
            acquired_.clear();
        }
        const auto result=step_level_file_walk_v1(walk_,services);
        if(result==LevelFileWalkStepV1::failed) {failed_=true;error_=walk_.error;}
        else if(result==LevelFileWalkStepV1::complete)active_=false;
        return result;
    }catch(const std::exception& error) {
        failed_=true;error_=error.what();return LevelFileWalkStepV1::failed;
    }
}
LevelFileWalkStepV1 CachedLevelFileV1::step_document(XmlDocumentV1::Borrow document,std::shared_ptr<const void> owner,const std::string& root,LevelFileWalkServicesV1& services){
 if(failed_)return LevelFileWalkStepV1::failed;
 try{
  if(!document||!document.used_level_buffer_route()||!owner)throw std::runtime_error("Required same owned assigned level-buffer document");
  if(active_&&(!document_input_||document.uri()!=requested_||root!=root_||assigned_source_owner_.get()!=owner.get()||assigned_source_owner_.owner_before(owner)||owner.owner_before(assigned_source_owner_)||!walk_.document||&walk_.document.source()!=&document.source()))throw std::runtime_error("Pending assigned XML document/owner occurrence changed");
  if(!active_){active_=true;document_input_=true;assigned_source_owner_=std::move(owner);requested_=document.uri();resolved_=requested_;root_=root;error_.clear();walk_={};acquired_.clear();
   if(!prepare_level_file_walk_v1(std::move(document),root,walk_,error_))throw std::runtime_error(error_);
  }
  const auto result=step_level_file_walk_v1(walk_,services);
  if(result==LevelFileWalkStepV1::failed){failed_=true;error_=walk_.error;}
  else if(result==LevelFileWalkStepV1::complete)active_=false;
  return result;
 }catch(const std::exception& error){failed_=true;error_=error.what();return LevelFileWalkStepV1::failed;}
}
bool CachedLevelFileV1::discard(LevelFileWalkServicesV1& services,std::string& error) {
    error.clear();
    if(walk_.document&&!discard_level_file_walk_v1(walk_,services,error))return false;
    walk_={};acquired_.clear();assigned_source_owner_.reset();document_input_=false;active_=false;failed_=false;requested_.clear();root_.clear();resolved_.clear();error_.clear();return true;
}
}
