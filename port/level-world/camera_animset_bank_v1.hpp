#pragma once

#include "camera_animset_v1.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh2::camera_animset_bank_v1 {

struct Clip {
    camera_animset_v1::Resource resource{};
    std::vector<std::uint8_t> bdae;
};

using ReadAsset = bool (*)(void*,const std::string&,std::vector<std::uint8_t>&,std::string&);

// Owns immutable selected CamAnimSet BDAE resources only. CameraLevel's one
// live pose, clock, and zoom remain with their existing runtime owners.
class Owner {
public:
    bool load(const camera_animset_v1::Selection&,ReadAsset,void*,std::string& error);
    void clear() noexcept;
    const Clip* resolve(const camera_animset_v1::PlayRequest&) const noexcept;
    // AnimationSet::GetAnimation (0x3660f4) loads an AnimDict missing from the
    // set on demand. Read and retain the BDAE of an on-demand request
    // (registration_order -1) and assign its ordinal; an already-retained clip
    // is reused. The set stays bounded (kMaxOnDemandClips).
    bool load_on_demand(camera_animset_v1::PlayRequest&,ReadAsset,void*,std::string& error);
    const std::string& set_name() const noexcept { return set_name_; }
    std::size_t size() const noexcept { return clips_.size(); }

private:
    std::string set_name_;
    std::vector<Clip> clips_;
};

} // namespace dh2::camera_animset_bank_v1
