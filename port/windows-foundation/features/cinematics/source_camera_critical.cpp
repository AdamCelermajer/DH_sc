#include "source_camera_critical.hpp"
namespace dh::foundation {namespace {
bool missing(const char* service,std::string& e){if(e.empty())e=std::string("Required original critical camera ")+service;return false;}
}
bool SourceCameraCriticalEffect::apply(std::uintptr_t character,std::string& e)const{
    e.clear();std::uintptr_t camera128=0;
    if(!providers_.currentCamera128||!providers_.currentCamera128(camera128,e))return missing("actual CurrentLevel/Camera128",e);
    if(!camera128)return true; // F_ApplyResult source NULL guards.
    if(!character)return missing("nonnull CanPlayShakeAnim Character/assertion branch",e);
    SourceCameraResourceAdmission admission(providers_.authority);std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
    if(!admission.borrow_camera(camera,e))return false;
    if(reinterpret_cast<std::uintptr_t>(camera.get())!=camera128)return missing("SAME current CameraLevel128",e);
    bool player=false;if(!providers_.isPlayer||!providers_.isPlayer(character,player,e))return missing("actual Character.IsPlayer",e);
    if(player){
        const auto app=providers_.authority.application.lock();const auto pm=app->source_player_manager_v59();
        if(!pm||!pm->manager()||!pm->belongs_to_application(app))return missing("published SAME PlayerManager",e);
        std::int32_t count=0;if(!pm->manager()->num_local_players(false,count,e))return false;
        if(count!=1)return true;bool local=false;if(!pm->source_is_local_player_v61(character,local,e))return false;
        if(!local)return true;
    }
    dh2::camera::PointV2 look{};if(!providers_.lookAt||!providers_.lookAt(character,look,e))return missing("actual GameObject.GetLookAtVec",e);
    const auto owner=providers_.authority.cameraApplication.lock();const auto session=owner->world();
    const auto row=camera->source_animation_row80_v115();
    if(!session->bindings.actual_tables_lease||!session->bindings.actual_tables||row<0||std::size_t(row)>=session->bindings.actual_tables->cameras.size())return missing("retained CamAnimSetTable row80",e);
    // Source3b1750 passes table+0c, clip0 and hold1. Authored transform
    // channels in its exact BDAE define all movement, not guessed shake math.
    return camera->level()->play_animation(session->bindings.actual_tables->cameras[row].crit,0,true,e);
}
}
