#include "camera_animset_bank_v1.hpp"

#include "../engine-resources/resources.hpp"

#include <algorithm>

namespace dh2::camera_animset_bank_v1 {
namespace {
constexpr std::size_t kMaxClipBytes=32u*1024u*1024u;
}

bool Owner::load(const camera_animset_v1::Selection& selection,ReadAsset reader,
                 void* context,std::string& error) {
    clear();error.clear();
    if(selection.name.empty()||selection.resources.empty()||!reader){
        error="Selected camera AnimSet or BDAE reader is missing";return false;
    }
    std::vector<Clip> candidate;candidate.reserve(selection.resources.size());
    for(std::size_t i=0;i<selection.resources.size();++i){
        const auto& resource=selection.resources[i];
        if(resource.registration_order!=static_cast<std::int32_t>(i)||resource.clip_id<0||
           resource.path.empty()||resource.path.size()>4096){
            error="Selected camera AnimSet resource identity is invalid";return false;
        }
        Clip clip;clip.resource=resource;
        if(!reader(context,resource.path,clip.bdae,error)){
            if(error.empty())error="Camera AnimSet BDAE resource read failed";
            return false;
        }
        if(clip.bdae.empty()||clip.bdae.size()>kMaxClipBytes){
            error="Camera AnimSet BDAE size is outside the accepted range";return false;
        }
        resources::BresView bres{};
        if(dh2_bres_open(&bres,clip.bdae.data(),clip.bdae.size())!=resources::BresError::ok||
           dh2_bres_library_count(&bres,resources::Library::animation)==0){
            error="Camera AnimSet resource is not a valid BRES animation";return false;
        }
        candidate.push_back(std::move(clip));
    }
    set_name_=selection.name;clips_=std::move(candidate);return true;
}

void Owner::clear() noexcept {set_name_.clear();clips_.clear();}

const Clip* Owner::resolve(const camera_animset_v1::PlayRequest& request) const noexcept {
    if(!request.present||request.resource.clip_id<0||request.resource.path.empty())return nullptr;
    const auto found=std::find_if(clips_.begin(),clips_.end(),[&](const Clip& clip){
        return clip.resource.clip_id==request.resource.clip_id&&
               clip.resource.registration_order==request.resource.registration_order&&
               clip.resource.slot==request.resource.slot&&clip.resource.path==request.resource.path;
    });
    return found==clips_.end()?nullptr:&*found;
}

} // namespace dh2::camera_animset_bank_v1
