#pragma once
#include "source_cinematics.hpp"
#include "../../../level-world/gameplay_camera_application_v23.hpp"
namespace dh::foundation {
struct SourceCameraAdmission {
    std::int32_t animation=-1,row=-1,set=-1,mapped=-1;
    std::string path;
    bool registered=false,active=false;
    // Retains the SAME decoded BRES/controller/scene backings for observation.
    std::shared_ptr<dh2::camera::CameraAnimationResourceV10> resource;
    std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
};
struct SourceCameraAdmissionProviders {
    std::weak_ptr<dh2::application::ApplicationServicesOwnerV5> application;
    std::weak_ptr<dh2::camera::GameplayCameraApplicationV23> cameraApplication;
    std::weak_ptr<void> world;
    std::shared_ptr<const void> dictionaryLease;
    const dh2::data::Dictionary* dictionary=nullptr;
};
// Borrows the existing Application/World V23 camera chain. Does not load a
// second graph or create an animation set for a command. Original registration
// already reads exact AnimDict paths during the Level camera load.
class SourceCameraResourceAdmission {
    SourceCameraAdmissionProviders providers_;
public:
    explicit SourceCameraResourceAdmission(SourceCameraAdmissionProviders p):providers_(std::move(p)){}
    bool borrow_camera(std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,std::string&)const;
    bool inspect(const OriginalCampaignCommand&,bool skip,SourceCameraAdmission&,std::string&)const;
    bool scene_phase(std::uint32_t actualSourceTimestamp,std::string&)const;
};
}
