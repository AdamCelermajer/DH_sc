// Execute the extracted production refresh leaf on the packaged SWF tree.
// The owner below supplies only its two borrowed fields. AS execution and the
// lifecycle dispatcher are real; texture/startup game services are host fixtures.
#define main historical_movie_fixture_main
#include "../../engine-ui/tests/swf_movie.cpp"
#undef main
#include "../../level-loader/lifecycle_v36.hpp"
#include "../../engine-ui/swf_text_font_platform_v1.hpp"
#include "gameswf/gameswf_function.h"
#include <cstring>
#include <stdexcept>

namespace dh2::android_ui {
class OriginalUiSession {
 struct Impl {bool loaded{};std::shared_ptr<ui::SwfMovie> movie;};
 std::unique_ptr<Impl> impl_;
public:
 explicit OriginalUiSession(std::shared_ptr<ui::SwfMovie> movie):impl_(std::make_unique<Impl>()){
  impl_->loaded=bool(movie);impl_->movie=std::move(movie);
 }
 bool complete_refresh_stage26_v66(std::string&);
};
// The runner extracts this exact function from current original_ui_session.cpp.
// Keep the implementation out of this test so a strict callable regression fails.
#include "production_refresh.inc"
}

namespace {
void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
struct Authored:Test {
 unsigned settings{},multiplayer{};
 static bool font_read(void* raw,const char* name,bool,bool,std::vector<std::uint8_t>& bytes,std::string& e){
  auto& self=*static_cast<Authored*>(raw);
  // The authored HUD's two device fonts use the packaged font bytes. Locale,
  // bitmap-font resolution and GPU delivery remain declared host fixtures.
  const std::string file=std::string(name)=="Arial"?"wqy-zenhei":name;
  std::ifstream stream(self.base+"/../"+file+".ttf",std::ios::binary);
  if(!stream){bytes.clear();e.clear();return false;}
  bytes.assign(std::istreambuf_iterator<char>(stream),{});e.clear();return true;
 }
 static bool native_as(void* raw,const char* name,const gameswf::fn_call& call,std::string& e){
  auto& self=*static_cast<Authored*>(raw);
  if(!std::strcmp(name,"NativeLoadSettings")){++self.settings;e.clear();return true;}
  if(!std::strcmp(name,"NativeIsMultiplayerEnabled")){
   ++self.multiplayer;if(call.result)call.result->set_bool(false);e.clear();return true;
  }
  e=std::string("Unexpected startup host callback: ")+name;return false;
 }
};
struct Observed {
 std::uintptr_t receiver{};
 static bool absence(void* raw,SwfAsGraph& graph,std::string& e){
  auto& self=*static_cast<Observed*>(raw);SwfAsValue root,hud,method,result;
  bool found{},callable{};
  if(!graph.root_value(root,e)||!graph.find_target(root,"menu_HUD_0",hud,e))return false;
  check(hud.identity()!=0,"Actual authored menu_HUD_0 is absent");self.receiver=hud.identity();
  if(!graph.get_member(hud,"completeRefresh",method,found,e))return false;
  check(!found,"Test asset unexpectedly defines completeRefresh");
  if(!graph.invoke(hud,hud,"completeRefresh",{},result,callable,e))return false;
  check(!callable&&result.kind()==SwfAsValue::Kind::undefined,"Missing authored method changed AS result");
  return true;
 }
 static bool unchanged(void* raw,SwfAsGraph& graph,std::string& e){
  auto& self=*static_cast<Observed*>(raw);SwfAsValue root,hud,method;bool found{};
  if(!graph.root_value(root,e)||!graph.find_target(root,"menu_HUD_0",hud,e)||
     !graph.get_member(hud,"completeRefresh",method,found,e))return false;
  check(hud.identity()==self.receiver&&!found,"Optional invocation fabricated/replaced the authored method/tree");
  // RenderFX also ignores a missing target; preserve that independent branch.
  return graph.invoke_renderfx("menu_SourceAbsentTarget","completeRefresh",{},e);
 }
};
struct LevelFields {std::uint32_t progress{68},state{26},counter{20},current{9};};
}

int main(int argc,char** argv)try{
 check(argc==2,"Expected packaged menu directory");
 auto provider=std::make_shared<Authored>();provider->base=argv[1];
 auto movie=std::make_shared<SwfMovie>();provider->movie=movie.get();
 auto services=provider->services();services.native_owner=provider;services.native_action=Authored::native_as;
 services.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled"};std::string e;
 TextFontBackendsV2 fonts;fonts.bitmap_face=[](const auto&,TextBitmapFaceV2& out,std::string& e){out={};e.clear();return true;};
 SwfTextFontPlatformV1 platform({provider.get(),Authored::font_read,nullptr},services,provider,fonts,1);
 platform.policy().renderer_feature=[](const auto& command,std::string& e){
  if(command.kind==edit_text_display_v1::Command::grid_fit){e.clear();return true;}
  e="Host refresh regression has no render cache";return false;
 };
 check(movie->load({"dqshared_droid.swf"},"dqhud_droid.swf",platform.services(),e)&&movie->advance(0,e),e);
 check(provider->settings==1&&provider->multiplayer==1,"Authored startup callback counts changed");
 Observed observed;check(movie->menu_action_script(&observed,Observed::absence,e),e);
 dh2::android_ui::OriginalUiSession ui(movie);
 using namespace dh2::loader;
 auto fields=std::make_shared<LevelFields>();unsigned refresh_calls{},stage27_calls{};
 std::vector<std::pair<int,int>> progress;LifecycleServicesV36 callbacks;
 callbacks.stage_body[26]=[&](std::string& e){++refresh_calls;
  return ui.complete_refresh_stage26_v66(e)?LifecycleStepV36::complete:LifecycleStepV36::failed;
 };
 callbacks.stage_body[27]=[&](std::string& e){++stage27_calls;e="Stage27 must have no body";return LifecycleStepV36::failed;};
 callbacks.publish_progress=[&](int phase,int value,std::string& e){
  check(fields->state==std::uint32_t(phase)&&fields->progress==std::uint32_t(value),"Progress did not borrow actual scalar tail");
  progress.emplace_back(phase,value);e.clear();return true;
 };
 LifecycleV36 loading({&fields->progress,&fields->state,&fields->counter,&fields->current},fields,{},std::move(callbacks));
 check(loading.tick()==LifecycleStatusV36::loading,loading.diagnostics().error);
 check(fields->state==27&&fields->progress==71&&fields->counter==9,"Original stage26 scalar tail diverged");
 check(loading.tick()==LifecycleStatusV36::loading,loading.diagnostics().error);
 check(fields->state==28&&fields->progress==73&&refresh_calls==1&&stage27_calls==0,"26->27->28 replay/body/progress diverged");
 check(progress==std::vector<std::pair<int,int>>{{27,71},{28,73}},"Wrong published stage26/27 progress");
 check(movie->menu_action_script(&observed,Observed::unchanged,e),e);

 // Unavailable native movie is still a required delivery failure. It must not
 // inherit the authored missing-method no-op or replay after the failed tick.
 dh2::android_ui::OriginalUiSession unavailable({});auto failed_fields=std::make_shared<LevelFields>();
 unsigned attempts{},publications{},unloads{};LifecycleServicesV36 failing;
 failing.stage_body[26]=[&](std::string& e){++attempts;
  return unavailable.complete_refresh_stage26_v66(e)?LifecycleStepV36::complete:LifecycleStepV36::failed;
 };
 failing.publish_progress=[&](int,int,std::string&){++publications;return true;};
 failing.cancel_and_unload=[&](std::string& e){++unloads;e.clear();return LifecycleStepV36::complete;};
 LifecycleV36 rejected({&failed_fields->progress,&failed_fields->state,&failed_fields->counter,&failed_fields->current},failed_fields,{},std::move(failing));
 check(rejected.tick()==LifecycleStatusV36::failed&&failed_fields->state==26&&failed_fields->progress==68,
       "Unavailable native movie was accepted/incremented");
 check(rejected.tick()==LifecycleStatusV36::failed&&attempts==1&&publications==0,"Failed refresh prefix replayed");
 rejected.request_cancel();check(rejected.tick()==LifecycleStatusV36::cancelled&&unloads==1,"Failed refresh could not drain explicit cancellation");
 std::cout<<"{\"validation\":\"PASS\",\"real_authored_receiver\":true,\"completeRefresh_defined\":false,"
  "\"states\":[26,27,28],\"progress\":[68,71,73],\"refresh_calls\":"<<refresh_calls<<",\"stage27_body_calls\":"<<stage27_calls<<","
  "\"native_delivery_failure_latched\":true,\"cancel_after_failure\":true,\"asset_method_fabricated\":false,"
  "\"limits\":{\"stage26_prefix_services_are_fixtures\":true,\"progress_ui_is_fixture\":true,\"font_resolution_is_fixture\":true,\"texture_uploads_are_fixtures\":true,\"whole_campaign\":false}}\n";
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}
