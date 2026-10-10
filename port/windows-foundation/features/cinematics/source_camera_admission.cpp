#include "source_camera_admission.hpp"
#include <cstring>
namespace dh::foundation {namespace {
bool missing(const char* service,std::string& e){e=std::string("Required original cinematic camera ")+service;return false;}
bool same_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
}
bool SourceCameraResourceAdmission::borrow_camera(std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& out,std::string& e)const{
    e.clear();out.reset();const auto app=providers_.application.lock();const auto owner=providers_.cameraApplication.lock();const auto world=providers_.world.lock();
    if(!app||!owner||!world||!same_owner(app->native_camera_services_v20(),owner))return missing("published SAME Application V23/World authority",e);
    const auto session=owner->world();
    if(!owner->ready()||!session||!same_owner(session->bindings.world_lease,world)||!session->camera||!session->camera->loaded())return missing("current loaded SAME World V19",e);
    out=session->camera->level();
    if(!out||!out->loaded()||!out->scene()||!out->animator()||out->animator()->scene_borrow()!=out->scene())return missing("retained SAME CameraLevel/scene/animator",e);
    return true;
}
bool SourceCameraResourceAdmission::inspect(const OriginalCampaignCommand& command,bool skip,SourceCameraAdmission& out,std::string& e)const{
    out={};if(command.kind!=5)return missing("authored PlayCamera kind5",e);
    if(!borrow_camera(out.camera,e))return false;
    const auto owner=providers_.cameraApplication.lock();const auto app=providers_.application.lock();const auto session=owner->world();
    if(!providers_.dictionaryLease||!providers_.dictionary||!session->bindings.actual_tables_lease||!session->bindings.actual_tables)return missing("retained original dictionary/CamAnimSetTable",e);
    out.row=out.camera->source_animation_row80_v115();out.set=out.camera->set_id();
    const auto& rows=session->bindings.actual_tables->cameras;
    if(out.row<0||std::size_t(out.row)>=rows.size())return missing("loaded CameraLevel row80",e);
    if(skip)out.animation=rows[out.row].idle;
    else{const auto authored=command.scalars.find(8);if(authored==command.scalars.end())return missing("authored animation8",e);std::memcpy(&out.animation,&authored->second,4);}
    out.active=app->active_camera().source_active()==reinterpret_cast<std::uintptr_t>(out.camera.get());
    if(out.animation<0){e.clear();return true;} // Original -1 source no-play.
    if(std::size_t(out.animation)>=providers_.dictionary->values.size())return missing("actual AnimDict animation ID",e);
    out.path=providers_.dictionary->values[out.animation];
    const auto manager=owner->animation_manager();const auto set=manager?manager->find(out.set):nullptr;
    if(!set||!set->ready||set->failed)return missing("SAME complete registered AnimationSet",e);
    out.mapped=set->registration.lookup(out.animation);
    const auto loaded=set->by_id.find(out.animation);
    if(out.mapped<0){e.clear();return true;} // Source unmapped ID is no-play; not positive admission.
    if(loaded==set->by_id.end()||!loaded->second||loaded->second->bytes.empty()||loaded->second->ranges.empty())return missing("retained actual registered BRES/clip ranges",e);
    out.resource=loaded->second;out.registered=true;e.clear();return true;
}
bool SourceCameraResourceAdmission::scene_phase(std::uint32_t timestamp,std::string& e)const{
    std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;if(!borrow_camera(camera,e))return false;
    // Sole source scene timestamp, actual owner completion callback clears84.
    return providers_.cameraApplication.lock()->scene_phase(timestamp,e);
}
}
