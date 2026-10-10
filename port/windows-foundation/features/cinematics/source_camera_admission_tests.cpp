#include "source_camera_admission.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
}
int main(int argc,char** argv){try{
    AssetCatalog assets(argc>1?argv[1]:".local-inputs/windows-camera-assets");std::string e;
    auto dictionary=std::make_shared<dh2::data::Dictionary>();auto tables=std::make_shared<dh2::data::AnimationTables>();dh2::data::DesignSettingsOwner design;
    std::vector<std::vector<std::uint8_t>> storage;storage.reserve(8);
    auto bytes=[&](const char* name){storage.push_back(assets.read(std::string("data/")+name));const auto& b=storage.back();return dh2::data::Bytes{b.data(),b.size()};};
    check(dh2::data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),*dictionary,e),e);
    check(dh2::data::load_animation_tables(bytes("animations_pyarray.bin"),bytes("animations_pyarraynames.bin"),bytes("animations_pystructnames.bin"),*dictionary,*tables,e),e);
    check(design.load(bytes("design_pyarray.bin"),bytes("design_pyarraynames.bin"),bytes("design_pystructnames.bin"),e),e);
    auto app=std::make_shared<dh2::application::ApplicationServicesOwnerV5>();check(app->post_init_events_v5(e),e);
    auto context=std::make_shared<dh2::camera::GameplayCameraApplicationV23>();check(app->publish_native_camera_services_v20(context,e),e);
    auto world=std::make_shared<int>(1);auto roots=std::make_shared<dh2::world::GameObjectSceneRootRegistryV1>();
    // Explicit external driver/device/Debug/actor/PM leaves. Application,
    // event, active slot, Scene registry, factory, Zoom, membership, timeline,
    // registration and resource owners are actual recovered constructors.
    dh2::camera::CameraApplicationBindingsV23 application;application.application=app;application.actual_roots=roots;
    application.actual_dictionary_lease=dictionary;application.actual_dictionary=dictionary.get();application.actual_file_system=world;
    application.backend.configured_backend=world;application.backend.viewport=[](auto& w,auto& h,auto&){w=960;h=540;return true;};
    application.actual_lg_devices=[](auto& lg,auto&){lg=0;return true;};application.debug_after_add=[](auto&){return true;};
    application.read=[&](const std::string& path,auto& out,auto& error){try{out=assets.read(path);return true;}catch(const std::exception& ex){error=ex.what();return false;}};
    check(context->initialize(application,e),e);
    dh2::camera::CameraWorldBindingsV23 bindings;bindings.world_lease=world;bindings.actual_tables_lease=tables;bindings.actual_tables=tables.get();bindings.design=design.borrow();
    dh2::camera::PointV2 origin{},anchor{100,200,300};bindings.actors.target.source_origin=&origin;
    bindings.actors.target.application_dt=[](auto& dt,auto&){dt=17;return true;};bindings.actors.target.actor_anchor=[&](auto,auto& point,auto&){point=anchor;return true;};
    bindings.actors.actor_position=bindings.actors.target.actor_anchor;bindings.actors.handle_centering=[](auto&,auto&){return true;};
    bindings.actors.debug_infinite_zoom=[](auto& yes,auto&){yes=false;return true;};bindings.actors.actor_disabled81=[](auto,auto& yes,auto&){yes=false;return true;};
    bindings.local_character0=[](auto& id,auto&){id=42;return true;};bindings.layout.world=world;
    bindings.layout.source_party_count6c4=[](auto& count,auto&){count=1;return true;};bindings.layout.has_current_level=[](auto& yes,auto&){yes=true;return true;};bindings.layout.num_local_players=[](auto& count,auto&){count=1;return true;};
    // The swamp LevelConfig omits camera_animset; the canonical declaration at
    // offset 0x264 supplies "Default". Keep the actual loaded table as the
    // authority and verify the selected row below rather than inventing a row.
    dh2::camera::CameraLevelConfigV19 config;config.camera={"data/3d/camera/cameratests.bdae","Default","PlayerCamera_Default",600,10000};
    std::shared_ptr<dh2::camera::CameraWorldSessionV23> session;check(context->load(config,bindings,session,e),e);
    const auto row=session->camera->level()->source_animation_row80_v115();
    check(row>=0&&std::size_t(row)<tables->camera_names.size()&&tables->camera_names[row]=="Default",
          "swamp LevelConfig did not select the original Default CamAnimSet row");
    SourceCameraResourceAdmission admission({app,context,world,dictionary,dictionary.get()});
    OriginalCampaignCommand command;command.kind=5;command.scalars[8]=static_cast<std::uint32_t>(tables->cameras.at(session->camera->level()->source_animation_row80_v115()).idle);
    SourceCameraAdmission receipt;check(admission.inspect(command,false,receipt,e)&&receipt.registered&&receipt.active&&receipt.camera==session->camera->level(),e);
    check(receipt.path==dictionary->values.at(receipt.animation)&&!receipt.resource->ranges.empty(),"actual exact path/resource admission");
    SourceCinematicProviders providers;providers.camera=[&](auto& out,auto& error){return admission.borrow_camera(out,error);};providers.trace=[](auto&){return true;};SourceCinematicCommands commands(providers);bool handled=false,blocking=false;
    check(commands.command(CampaignCommandPhase::execute,command,7,false,handled,blocking,e)&&handled,e);command.scalars[12]=1;
    check(commands.command(CampaignCommandPhase::is_blocking,command,7,false,handled,blocking,e)&&blocking,e);
    check(admission.scene_phase(0,e)&&admission.scene_phase(static_cast<std::uint32_t>(receipt.resource->ranges[0][1])+1,e),e);
    check(commands.command(CampaignCommandPhase::is_blocking,command,7,false,handled,blocking,e)&&!blocking,"actual completed timeline source84");
    command.scalars[8]=334;check(admission.inspect(command,false,receipt,e),e);
    check(receipt.animation==334&&receipt.path=="data/3D/camera/animations/cs_moth_intro/cs_moth_intro.bdae"&&!receipt.registered,
          "MothIntro334 must remain the exact source dictionary lookup and Default-row no-play");
    check(std::find(tables->cameras[row].cam_anims.begin(),tables->cameras[row].cam_anims.end(),334)==tables->cameras[row].cam_anims.end(),
          "MothIntro334 unexpectedly became an authored member of the original Default row");
    std::cout<<"original-row Default authored 334 path="<<receipt.path<<" row_member=0 source_no_play=1\n";
    command.scalars[8]=44;check(admission.inspect(command,false,receipt,e),e);
    check(receipt.animation==44&&receipt.path=="data/3D/camera/animations/common/cam_shake_horiz.bdae"&&receipt.registered&&receipt.active,
          "TrollReturn44 must admit from the same active original Default row");
    std::cout<<"original-row Default authored 44 path="<<receipt.path<<" registered=1 active=1\n";
    const auto sameManager=context->animation_manager();check(context->begin_world_release(e),e);context->flush_animation_sets();check(context->release_world_owners(e),e);session.reset();
    check(app->active_camera().source_active()==0&&sameManager->current_source_key()==-1,"original active-slot release/set flush");
    check(!admission.borrow_camera(receipt.camera,e),"retired source World cannot be borrowed");check(context->close_application(e),e);
    std::cout<<"SAME actual Application/V23/V19 camera admission/timeline/ordered retirement PASS; external leaves explicit\n";
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}return 0;}
