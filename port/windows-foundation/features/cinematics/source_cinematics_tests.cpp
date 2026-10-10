#include "source_cinematics.hpp"
#include "../../../game-data/animation_tables.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool result,const std::string& message){if(!result)throw std::runtime_error(message);}
// Explicit source leaf fixture, not a production actor surrogate.
struct ActorFixture {
    dh2::target_providers::Handle16 handle{};std::uint32_t type=2;std::int32_t room=7;
    std::uint8_t across=0,forced=0,static84=1;bool character=true;float xyz[3]{10,20,30};
    dh2::world::CanonicalObjectBorrowV1 borrow(const std::shared_ptr<ActorFixture>& owner){
        dh2::world::CanonicalObjectBorrowV1 b;b.identity=reinterpret_cast<std::uintptr_t>(this);b.lease=owner;
        b.shared_handle=&handle;b.type_f4=&type;b.room64=&room;b.across_rooms87=&across;b.context=this;
        b.set_name=[](void*,const char*,std::string&){return true;};b.set_archetype=b.set_name;
        b.as_character=[](void* p,std::uintptr_t& out,std::string&){auto* a=static_cast<ActorFixture*>(p);out=a->character?reinterpret_cast<std::uintptr_t>(a):0;return true;};return b;
    }
};
struct CameraFixture {
    AssetCatalog assets;dh2::data::Dictionary dictionary;dh2::data::AnimationTables tables;
    dh2::data::DesignSettingsOwner design;std::shared_ptr<int> app=std::make_shared<int>(1);
    std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;std::shared_ptr<dh2::camera::GameplayCameraSceneV3> graph;
    std::shared_ptr<dh2::camera::GameplayCameraAnimatorV10> animator;std::uintptr_t active=0;
    explicit CameraFixture(const std::filesystem::path& path):assets(path){
        std::string e;std::vector<std::vector<std::uint8_t>> data;data.reserve(8);
        auto bytes=[&](const char* name){data.push_back(assets.read(std::string("data/")+name));auto& b=data.back();return dh2::data::Bytes{b.data(),b.size()};};
        check(dh2::data::load_dictionary(bytes("animations_dictionary_pyarraynames.bin"),bytes("animations_dictionary_pyarray.bin"),dictionary,e),e);
        check(dh2::data::load_animation_tables(bytes("animations_pyarray.bin"),bytes("animations_pyarraynames.bin"),bytes("animations_pystructnames.bin"),dictionary,tables,e),e);
        check(design.load(bytes("design_pyarray.bin"),bytes("design_pyarraynames.bin"),bytes("design_pystructnames.bin"),e),e);
        auto read=[this](const std::string& uri,auto& out,std::string& error){try{out=assets.read(uri);error.clear();return true;}catch(const std::exception& ex){error=ex.what();return false;}};
        dh2::camera::CameraAnimationManagerServicesV10 ms;ms.application_lease=app;ms.dictionary=&dictionary;ms.read=read;
        // Explicit Application/device/Debug fixture leaves; real resource,
        // registration, controller, timeline and camera scene kernels execute.
        ms.actual_lg_devices=[](auto& lg,auto&){lg=0;return true;};ms.debug_after_add=[](auto&){return true;};
        dh2::camera::CameraRuntimeServicesV11 s;s.world_lease=app;s.tables=&tables;s.design=design.borrow();s.read=read;
        s.manager=std::make_shared<dh2::camera::GameplayCameraAnimationManagerV10>(ms);
        s.attach_graph=[this](auto g,auto&){graph=g;return bool(graph);};
        s.attach_animator=[this](auto g,auto a,auto&){if(g!=graph)return false;animator=a;return bool(animator);};
        s.detach=[this](auto g,auto a,auto&){if(g!=graph||a!=animator)return false;graph.reset();animator.reset();return true;};
        s.activate=[this](auto id,auto,auto,auto&){active=id;return true;};
        s.is_active=[this](auto id,auto& value,auto&){value=id==active;return true;};
        s.release_active=[this](auto id,auto&){if(id==active)active=0;return true;};
        camera=std::make_shared<dh2::camera::GameplayCameraRuntimeV11>(s);
        check(camera->load({"data/3d/camera/cameratests.bdae","Default","PlayerCamera_Default",900,4200},e),e);
        check(camera->activate(e),e);
    }
};
std::shared_ptr<OriginalCampaignRuntime> bank(const std::filesystem::path& repo){
    auto runtime=std::make_shared<OriginalCampaignRuntime>();std::string e;
    check(runtime->load(AssetCatalog(repo/".local-inputs/windows-encounter-source"),"original-campaign.xml",e),e);
    check(runtime->scripts().size()==70,"actual exported original source bank70");return runtime;
}
void run(const std::filesystem::path& repo){
    std::string error;auto runtime=bank(repo);CameraFixture native(repo/".local-inputs/windows-camera-assets");
    const auto& intro=runtime->scripts().at(runtime->script_id("LizardMan_Intro",false));
    const auto& moth=runtime->scripts().at(runtime->script_id("MothIntro",false));
    unsigned traces=0;std::uint8_t globalBlocked=0;auto controls=std::make_shared<int>(1);
    dh2::world::CanonicalObjectManagerServicesV1 managerServices;
    managerServices.missing_name_debug=[](void*,std::string& e){e.clear();return true;}; // declared missing-object Debug fixture
    auto objects=std::make_shared<dh2::world::CanonicalObjectManagerV1>(managerServices);
    auto targets=std::make_shared<CampaignCameraAdapter>(CampaignCameraProviders{
        [](const std::string& name,std::uint64_t& id,bool& found,std::string& e){found=name=="_prim_Waypoint_NewCamSpot"||name=="LocalPlayer";id=name=="LocalPlayer"?1:2;e.clear();return true;},
        [](auto& id,auto&){id=1;return true;},
        [](auto id,CameraVec3& p,auto&){p=id==1?CameraVec3{0,0,0}:CameraVec3{1000,0,0};return true;}});
    check(targets->seed_target(1,error),error);
    SourceCinematicProviders providers;providers.objects=objects;providers.targetCamera=targets;
    providers.camera=[&](auto& out,std::string& e){out=native.camera;e.clear();return true;};
    providers.trace=[&](auto& e){++traces;e.clear();return true;};
    providers.globalControl=[&](auto& pin,auto& cell,auto&){pin=controls;cell=&globalBlocked;return true;};
    auto commands=std::make_shared<SourceCinematicCommands>(providers);bool handled=false,blocking=false;
    check(commands->command(CampaignCommandPhase::execute,intro.commands.at(1),7,false,handled,blocking,error)&&handled&&globalBlocked==1,error);
    check(commands->command(CampaignCommandPhase::execute,intro.commands.at(8),7,false,handled,blocking,error)&&globalBlocked==0,error);
    const auto traceBeforeSkip=traces;
    check(commands->command(CampaignCommandPhase::execute,intro.commands.at(1),7,true,handled,blocking,error)&&globalBlocked==0&&traces==traceBeforeSkip,"source lock skip returns before Debug/control write");
    check(commands->command(CampaignCommandPhase::execute,intro.commands.at(2),7,false,handled,blocking,error)&&targets->transition_remaining()==1000,error);
    CampaignCameraFrame frame;check(targets->tick(500,frame,error)&&frame.anchor.x==500&&!frame.applyDamping,"original transition kernel source1000ms midpoint");
    check(commands->command(CampaignCommandPhase::execute,intro.commands.at(9),7,true,handled,blocking,error)&&targets->target()==1&&targets->transition_remaining()==0,"source skip target return uses authored LocalPlayer with duration0");
    const auto authoredIdle=std::find_if(moth.commands.begin(),moth.commands.end(),[](const auto& c){return c.kind==5&&c.scalars.at(12)==0;});
    check(authoredIdle!=moth.commands.end(),"actual Moth camera idle command");
    check(commands->command(CampaignCommandPhase::execute,*authoredIdle,7,false,handled,blocking,error),error);
    check(native.camera->level()->fields().shake84&&native.camera->animator()->timeline.clip_index==0,"actual authored camera animation selects native clip0 and starts source flag84");
    OriginalCampaignCommand waiting=*authoredIdle;waiting.scalars[12]=1; // explicit wait-branch fixture only
    check(commands->command(CampaignCommandPhase::is_blocking,waiting,7,false,handled,blocking,error)&&blocking,"camera wait reads SAME source84");
    check(commands->scene_phase(0,error),error);
    const auto actualEnd=native.camera->animator()->timeline.end_ms;
    check(actualEnd>=0&&commands->scene_phase(static_cast<std::uint32_t>(actualEnd)+1,error),error);
    check(commands->command(CampaignCommandPhase::is_blocking,waiting,7,false,handled,blocking,error)&&!blocking,"actual source timeline completion clears84; no fake authored duration");
    SourceCinematicCommands unbound({});
    check(!unbound.command(CampaignCommandPhase::execute,intro.commands.at(1),7,false,handled,blocking,error)&&error.find("Debug tracing")!=std::string::npos,"missing reached cinematic service fails honestly");
    auto actor=std::make_shared<ActorFixture>(),waypoint=std::make_shared<ActorFixture>();waypoint->character=false;
    dh2::target_providers::Handle16 registered{};
    check(objects->add(actor->borrow(actor),"_prim_Monster_FakeMoth_01","Character",7,false,registered,error),error);
    check(objects->add(waypoint->borrow(waypoint),"_prim_Waypoint_MothSpot_XYZ","Waypoint",7,false,registered,error),error);
    check(objects->add(waypoint->borrow(waypoint),"_prim_Waypoint_MothSpotLookTo","Waypoint",7,false,registered,error),error);
    std::vector<std::string> effectOrder;unsigned looks=0;bool failLook=false;
    providers.isPlayer=[](auto,bool& out,auto&){out=false;return true;};
    providers.setIdle=[&](auto id,bool wait,auto&){check(id==reinterpret_cast<std::uintptr_t>(actor.get())&&!wait,"source teleport idlefalse");effectOrder.push_back("idle");return true;};
    providers.position=[&](auto id,auto& pin,const float*& out,auto&){check(id==reinterpret_cast<std::uintptr_t>(waypoint.get()),"actual waypoint Position160");pin=waypoint;out=waypoint->xyz;return true;};
    providers.setPosition=[&](auto id,const float* xyz,bool update,auto&){check(id==reinterpret_cast<std::uintptr_t>(actor.get())&&xyz==waypoint->xyz&&update,"direct borrowed source position");effectOrder.push_back("position");return true;};
    providers.forceUpdatePosition=[&](auto,auto&){effectOrder.push_back("force-update");return true;};
    providers.controllerForced=[&](auto,auto& pin,auto& cell,auto&){pin=actor;cell=&actor->forced;return true;};
    providers.controllerLook=[&](auto,auto target,auto&){check(actor->forced==1&&target==reinterpret_cast<std::uintptr_t>(waypoint.get()),"actual forced interval and target");++looks;return !failLook;};
    SourceCinematicCommands actorCommands(providers);
    check(actorCommands.command(CampaignCommandPhase::execute,moth.commands.at(4),7,true,handled,blocking,error)&&handled&&effectOrder==std::vector<std::string>{"idle","position","force-update"},error);
    check(actorCommands.command(CampaignCommandPhase::execute,moth.commands.at(6),7,true,handled,blocking,error)&&actor->forced==0&&looks==1,"LookActor ignores skip and clears actual forced9");
    failLook=true;check(!actorCommands.command(CampaignCommandPhase::execute,moth.commands.at(6),7,false,handled,blocking,error)&&actor->forced==1,"failed source LookAt retains reached forced9 write");
    OriginalCampaignCommand move;move.kind=40;move.strings={{24,"_prim_Monster_FakeMoth_01"},{12,"_prim_Waypoint_MothSpot_XYZ"}};move.scalars={{16,1},{28,1}};
    providers.actorStatic=[&](auto,auto& pin,const std::uint8_t*& cell,auto&){pin=actor;cell=&actor->static84;return true;};
    providers.moveActor=[&](auto,auto,bool skip,bool collision,bool wait,std::uint8_t& captured,bool& retained,auto&){check(skip&&collision&&wait&&captured==1,"exact MoveActor source inputs");retained=true;return true;};
    providers.moveBlocking=[](auto,std::uint8_t captured,bool& out,auto&){check(captured==1,"captured84 retained across actual changes");out=true;return true;};
    SourceCinematicCommands moveCommands(providers);
    check(moveCommands.command(CampaignCommandPhase::execute,move,7,true,handled,blocking,error),error);actor->static84=0;
    check(moveCommands.command(CampaignCommandPhase::is_blocking,move,7,false,handled,blocking,error)&&blocking,error);
    auto finishedBank=bank(repo);std::vector<CinematicEvent> events;
    SourceCinematicSessionProviders sessionProviders;sessionProviders.commands=commands;
    sessionProviders.campaign.admit_start=[](int,int,bool,bool& admitted,auto&){admitted=true;return true;}; // explicit PM/network admission fixture
    sessionProviders.event=[&](const auto& event){events.push_back(event);};
    SourceCinematicSession finished(finishedBank,sessionProviders);
    check(finished.start(finishedBank->script_id("FaeryIdle"),7,true,error)&&finished.tick_scripts(0,error),error);
    check(finished.state()==CinematicState::finished&&events.size()==2&&events.front().state==CinematicState::running&&events.back().state==CinematicState::finished,"actual authored actor command NULL object leaf finishes session; events observe source termination");
    auto failedBank=bank(repo);SourceCinematicSession failed(failedBank,sessionProviders);
    check(failed.start(failedBank->script_id("LizardMan_Intro",false),7,true,error)&&failed.tick_scripts(0,error),error);
    check(!failed.tick_scripts(0,error)&&failed.state()==CinematicState::failed&&error.find("remaining reached authored effect")!=std::string::npos,"actual original full intro stops at first missing common-script provider, no fabricated cutscene success");
    auto cancelledBank=bank(repo);bool sourceSkip=false;unsigned aborted=0;
    sessionProviders.input=[&](auto,bool& out,auto&){sourceSkip=true;out=sourceSkip;return true;}; // actual borrowed input admission fixture
    SourceCinematicSession cancelled(cancelledBank,sessionProviders);
    check(cancelled.start(cancelledBank->script_id("LizardMan_Intro",false),7,true,error)&&cancelled.input(CinematicInput::skip,error)&&cancelled.skip_active(),error);
    check(!cancelled.input(CinematicInput::cancel,error)&&cancelled.state()==CinematicState::running,"missing abort provider cannot unlock/restore or claim cancellation");
    auto abortBank=bank(repo);sessionProviders.abortWorld=[&](int,int,std::string& e){++aborted;e.clear();return true;}; // explicit host world-restoration fixture
    SourceCinematicSession abort(abortBank,sessionProviders);
    check(abort.start(abortBank->script_id("LizardMan_Intro",false),7,true,error)&&abort.input(CinematicInput::cancel,error)&&abort.state()==CinematicState::cancelled&&aborted==1,error);
    check(!abort.tick_scripts(0,error),"cancelled exclusive schedule quarantined without invented source context reset");
    check(native.camera->close(error),error);
    std::cout<<"Cinematics original bank70/camera authoredClipEnd="<<actualEnd<<" source effects, completion, gap, input/cancel lifecycle PASS; host boundaries declared\n";
}
}
int main(int argc,char** argv){try{run(argc>1?argv[1]:".");return 0;}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
