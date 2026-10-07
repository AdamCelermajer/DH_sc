#include "application_services_owner_v5.hpp"
#include "process_resource_prefix_source_v95.hpp"
#include "application_spawn_random_owner_v4.hpp"
#include "visual_fx_manager_libraries_v63.hpp"
#include "../engine-ui/owned_hud_settings_v1.hpp"
namespace dh2::application {
bool ApplicationServicesOwnerV5::publish_source_settings4c_v67(std::shared_ptr<ui::OwnedHudSettingsV1> owner,std::string& error){
 if(!owner||(source_settings4c_v67_&&(source_settings4c_v67_.get()!=owner.get()||source_settings4c_v67_.owner_before(owner)||owner.owner_before(source_settings4c_v67_)))){error="Application already owns a different source SavegameManager4c";return false;}
 source_settings4c_v67_=std::move(owner);error.clear();return true;
}
const std::shared_ptr<world::ApplicationSpawnRandomOwnerV4>& ApplicationServicesOwnerV5::source_random_v62(){
 if(!source_random_v62_)source_random_v62_=std::make_shared<world::ApplicationSpawnRandomOwnerV4>();
 return source_random_v62_;
}
bool ApplicationServicesOwnerV5::publish_source_fx_libraries_v63(std::shared_ptr<fx::VisualFxManagerLibrariesV63> owner,std::string& error){
 if(!owner||owner->application_identity()!=identity()){
  error="Required SAME actual Application source FX library owner";return false;
 }
 if(source_fx_libraries_v63_&&(source_fx_libraries_v63_.get()!=owner.get()||
    source_fx_libraries_v63_.owner_before(owner)||owner.owner_before(source_fx_libraries_v63_))){
  error="Application already owns a different source FX manager library";return false;
 }
 source_fx_libraries_v63_=std::move(owner);error.clear();return true;
}
namespace {
// One real native heap allocation. The identity names this actual receiver,
// unlike Level's separate EventManager base identity at Level+0.
struct HeapEventManagerV5 {
 events::EventManagerOwnerV12 receiver;
 HeapEventManagerV5():receiver(reinterpret_cast<std::uintptr_t>(&receiver)){}
};
}
bool ApplicationServicesOwnerV5::post_init_events_v5(std::string& error){
 if(event_publication_attempted_){error="Application EventManager PostInit publication already attempted";return false;}
 event_publication_attempted_=true;
 auto allocation=std::make_shared<HeapEventManagerV5>();
 // Source32f818 allocate ->32f820 actual EventManager C1 ->32f828 publish14.
 events14_=std::shared_ptr<events::EventManagerOwnerV12>(allocation,&allocation->receiver);
 error.clear();return true;
}
}

namespace dh2::application {
bool ApplicationServicesOwnerV5::initialize_source_resource_prefix_v95(std::string& e){
 if(!source_resource_prefix_v95_)source_resource_prefix_v95_=std::make_shared<ProcessResourcePrefixSourceV95>();
 return source_resource_prefix_v95_->initialize_gl_media_player_source(e);
}
bool ApplicationServicesOwnerV5::borrow_source_resource_prefix_v95(ProcessResourcePrefixBorrowV95& out,std::string& e)const{
 if(!source_resource_prefix_v95_){e="Required actual process appInit RES_PATH owner before routing";return false;}
 return source_resource_prefix_v95_->borrow(out,e);
}
}
