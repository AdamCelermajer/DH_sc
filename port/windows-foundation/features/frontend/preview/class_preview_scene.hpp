#pragma once
#include "creation_preview.hpp"
#include "class_light_source.hpp"
#include "../../../../engine-animation/animation.hpp"
namespace dh::foundation::frontend {
// CLASS_SELECTION owns the three anchor worlds and animated Camera01.
// Load retains previous=-1. First sample runs the original destination-specific
// scene transition while CreationPreview runs that actor's MenuOnSelect.
// Calling sample follows original UpdateAnim: sample cursor, add dt, clamp.
class ClassPreviewScene {
public:
    bool load(const AssetCatalog&,std::string&);
    bool sample(int selected,int dtMilliseconds,std::string&);
    bool input_enabled() const noexcept;
    bool loaded() const noexcept {return loaded_;}
    const std::string& selected_clip()const noexcept{return clipName_;}
    int sampled_milliseconds()const noexcept{return sampled_;}
    const ClassSelectLightInputs& light_inputs()const noexcept{return lightInputs_;}
    const char* required_light_owner()const noexcept{return lightInputs_.authored_light_present?required_class_select_light_owner():"no authored light branch";}
    const std::array<Mat4,3>& anchors()const noexcept{return anchors_;}
    const Camera& camera()const noexcept{return camera_;}
    OriginalScene& backdrop()noexcept{return backdrop_;}
    const OriginalScene& backdrop()const noexcept{return backdrop_;}
private:
    struct Clip {std::string name;int start{},end{};};
    dh2::scene::Scene scene_;
    dh2::animation::Player animation_;
    OriginalScene backdrop_;
    std::vector<Clip> clips_;
    std::array<unsigned,3> anchorNodes_{};
    std::array<Mat4,3> anchors_{};
    Camera camera_;
    ClassSelectLightInputs lightInputs_;
    unsigned cameraNode_{};
    int selected_=-1,cursor_{},end_{};
    int sampled_{};
    std::string clipName_;
    bool loaded_=false,transition_=false;
};
}
