#include "cached_level_file_v1.hpp"
#include "resource_paths_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
LevelFileWalkStepV1 CachedLevelFileV1::step(const std::string& uri,const std::string& root,LevelFileWalkServicesV1& services) {
    if(failed_)return LevelFileWalkStepV1::failed;
    try {
        if(active_&&(uri!=requested_||root!=root_))
            throw std::runtime_error("Pending XML request changed: "+requested_+" -> "+uri);
        if(!active_) {
            active_=true;requested_=uri;root_=root;resolved_.clear();error_.clear();walk_={};acquired_.clear();
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
bool CachedLevelFileV1::discard(LevelFileWalkServicesV1& services,std::string& error) {
    error.clear();
    if(walk_.document&&!discard_level_file_walk_v1(walk_,services,error))return false;
    walk_={};acquired_.clear();active_=false;failed_=false;requested_.clear();root_.clear();resolved_.clear();error_.clear();return true;
}
}
