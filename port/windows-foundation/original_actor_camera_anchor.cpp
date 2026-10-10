#include "original_actor_camera_anchor.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../level-world/camera_anchor_owner_v75.hpp"
#include "../script-runtime/script_constants.hpp"
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace dh::foundation {
namespace {
bool finite(CameraVec3 p){return std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);}
bool valid(const OriginalActorCameraAnchorFrame& f){return finite(f.position)&&finite(f.heading)&&finite(f.lookAt);}
}
CameraVec3 original_actor_camera_look_at(float angle){
    const float x=::sinf(angle);float y=::cosf(angle);
    std::uint32_t bits;std::memcpy(&bits,&y,4);bits+=0x80000000u;std::memcpy(&y,&bits,4);
    return {x,y,0.0f};
}
bool load_original_actor_camera_anchor_config(const AssetCatalog& assets,bool useStatic,
                                              OriginalActorCameraAnchorConfig& out,std::string& error){
    try{
        OriginalActorCameraAnchorConfig config;config.useStaticCamera=useStatic;
        if(!useStatic){
            const auto data=read_content(assets,"data/design_pycst.bin");
            if(data.size()>std::numeric_limits<std::uint32_t>::max())throw std::runtime_error("Original design constants exceed source size");
            const auto destroy=[](dh2_script_constants* p){dh2_script_constants_destroy(p);};
            std::unique_ptr<dh2_script_constants,decltype(destroy)> table(dh2_script_constants_create(),destroy);
            dh2_script_constants_reload reload{};
            if(!table||dh2_script_constants_load(table.get(),data.data(),std::uint32_t(data.size()),&reload)!=0||reload.consumed!=data.size())
                throw std::runtime_error("Original design constants failed complete decoding");
            std::int32_t maximum=0,speed=0,threshold=0;
            if(dh2_script_constants_get(table.get(),"CharacterDesign","ForwardCamera_Max_Distance",&maximum)||
               dh2_script_constants_get(table.get(),"CharacterDesign","ForwardCamera_Distance_PerSec",&speed)||
               dh2_script_constants_get(table.get(),"CharacterDesign","ForwardCamera_Threshold",&threshold))
                throw std::runtime_error("Original forward camera constant lookup failed");
            config.maximumDistance=float(maximum);config.distancePerUpdate=float(speed);
            volatile float fraction=float(threshold)*0.01f;config.threshold=fraction;
            if(maximum<0||speed<0||threshold<0||threshold>100)
                throw std::runtime_error("Original forward camera constants violate constructor assertions");
        }
        out=config;error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
struct OriginalActorCameraAnchor::Impl {
    OriginalActorCameraAnchorFrame frame;
    std::array<float,3> worldPosition{},direction{};
    std::uint32_t headingActive=0,dt=0;
    std::shared_ptr<int> provider=std::make_shared<int>(1);
    std::unique_ptr<dh2::camera::CameraAnchorOwnerV75> anchor;
    void setFrame(const OriginalActorCameraAnchorFrame& value){
        frame=value;worldPosition={value.position.x,value.position.y,value.position.z};
        direction={value.heading.x,value.heading.y,value.heading.z};headingActive=value.headingActive?1:0;
    }
    dh2::camera::CameraAnchorServicesV75 services(){
        dh2::camera::CameraAnchorServicesV75 s;s.provider=provider;
        s.actor=[this](auto id,auto& out,auto& error){
            out={provider,id,worldPosition.data(),&headingActive,direction.data()};error.clear();return true;};
        s.handle_character=[](auto id,auto& character,auto& error){character=id;error.clear();return true;};
        s.moving_false=[this](auto,auto& out,auto& error){out=frame.moving;error.clear();return true;};
        s.attacking=[this](auto,auto& out,auto& error){out=frame.attacking;error.clear();return true;};
        s.look_at=[this](auto,auto& out,auto& error){out={frame.lookAt.x,frame.lookAt.y,frame.lookAt.z};error.clear();return true;};
        s.application_dt=[this](auto& out,auto& error){out=dt;error.clear();return true;};
        s.assertion=[](auto,const auto*,auto& error){error="Invalid original AnchorForward constructor arguments";return false;};
        return s;
    }
};
OriginalActorCameraAnchor::OriginalActorCameraAnchor():impl_(std::make_unique<Impl>()){}
OriginalActorCameraAnchor::~OriginalActorCameraAnchor()=default;
OriginalActorCameraAnchor::OriginalActorCameraAnchor(OriginalActorCameraAnchor&&) noexcept=default;
OriginalActorCameraAnchor& OriginalActorCameraAnchor::operator=(OriginalActorCameraAnchor&&) noexcept=default;
bool OriginalActorCameraAnchor::initialize(OriginalActorCameraAnchorConfig config,const OriginalActorCameraAnchorFrame& frame,std::string& error){
    if(!valid(frame)||!std::isfinite(config.maximumDistance)||!std::isfinite(config.distancePerUpdate)||!std::isfinite(config.threshold)||
       config.maximumDistance<0||config.distancePerUpdate<0||config.threshold<0||config.threshold>1){error="Invalid original actor camera anchor input";return false;}
    auto next=std::make_unique<Impl>();next->setFrame(frame);
    next->anchor=std::make_unique<dh2::camera::CameraAnchorOwnerV75>(next->services());
    if(!next->anchor->construct(1,!config.useStaticCamera,config.maximumDistance,config.distancePerUpdate,config.threshold,error))return false;
    impl_=std::move(next);error.clear();return true;
}
bool OriginalActorCameraAnchor::initialized() const noexcept{return impl_&&impl_->anchor&&impl_->anchor->constructed();}
bool OriginalActorCameraAnchor::update(const OriginalActorCameraAnchorFrame& frame,std::uint32_t dt,std::string& error){
    if(!initialized()||!valid(frame)){error="Actor camera anchor requires constructed receiver and finite source frame";return false;}
    // Source GameObject::SetPosition translates the attached Anchor+c by the
    // same position delta before its auxiliary Anchor.Update phase.
    auto* attached=impl_->anchor->source_position_c();
    const float next[3]{frame.position.x,frame.position.y,frame.position.z};
    for(unsigned i=0;i<3;++i)attached[i]+=next[i]-impl_->worldPosition[i];
    impl_->setFrame(frame);impl_->dt=dt;
    if(!impl_->anchor->update(error))return false;
    if(!finite(position())){error="Original camera anchor produced nonfinite position";return false;}
    error.clear();return true;
}
bool OriginalActorCameraAnchor::reset(const OriginalActorCameraAnchorFrame& frame,std::string& error){
    if(!initialized()||!valid(frame)){error="Actor camera anchor reset requires constructed receiver and finite source frame";return false;}
    impl_->setFrame(frame);return impl_->anchor->reset(error);
}
CameraVec3 OriginalActorCameraAnchor::position() const noexcept{
    if(!initialized())return {};
    const auto& p=impl_->anchor->fields().position_c;return {p[0],p[1],p[2]};
}
} // namespace dh::foundation
