#include "source_process_trophies_v100.hpp"
#include "model_renderer.hpp"
#include "owned_hud_settings_v1.hpp"
#include "audio_process_initialize_v100.hpp"
#include <array>
#include <exception>
#include <cstring>
namespace dh2::android_ui {
namespace {
//Original process BSS singleton, independent of current Level/Character and
//GL context. App shutdown must retire it after all gameplay borrowers release.
std::shared_ptr<SourceProcessTrophiesV100> process_trophies;
constexpr const char* filename="achievements.savegame";
bool required(const char* what,std::string& e){e=std::string("Required process trophy ")+what;return false;}
bool same_owner(const std::shared_ptr<void>& a,const std::shared_ptr<void>& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);}
}
SourceProcessTrophiesV100::SourceProcessTrophiesV100(
 std::shared_ptr<application::ApplicationServicesOwnerV5> app,ProcessTrophyServicesV100 services):
 application_(std::move(app)),services_(std::move(services)){}
bool SourceProcessTrophiesV100::belongs_to(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app)const noexcept{
 auto actual=application_.lock();return actual&&app&&actual.get()==app.get()&&same_owner(actual,app);
}
bool SourceProcessTrophiesV100::files(std::shared_ptr<application::ApplicationSaveFilesOwnerV61>& out,std::string& e){
 if(!services_.files||!services_.files(out,e))return required("actual FileManager getter",e);
 auto app=application_.lock();if(!app)return required("live Application",e);
 if(out&&!out->belongs_to_application(app))return required("SAME Application FileManager",e);
 return true;
}
bool SourceProcessTrophiesV100::construct(std::string& e){
 if(construct_attempted_)return required("once-only CreateInstance constructor",e);construct_attempted_=true;
 if(!services_.owner||!services_.cache||!services_.debug||!services_.debug_files||!services_.debug_file_owner)
  return required("original Arrays/Debug/file provider lifetimes",e);
 if(same_owner(services_.owner,application_.lock()))return required("independent process providers without Application cycle",e);
 std::array<std::vector<std::uint8_t>,3> bytes;
 const char* names[]{"data/pydata/trophies_pyarray.bin","data/pydata/trophies_pyarraynames.bin","data/pydata/trophies_pystructnames.bin"};
 for(unsigned i=0;i<3;++i){bool found{};if(!services_.cache(names[i],found,bytes[i],e))return false;
  if(!found)return required("authored TrophyTable bytes",e);}
 catalog_=trophies::TrophyCatalogV1::load({bytes[0].data(),bytes[0].size()},
  {bytes[1].data(),bytes[1].size()},{bytes[2].data(),bytes[2].size()},e);
 if(!catalog_)return false;
 manager_=trophies::TrophyManagerOwnerV1::create(catalog_,services_.debug.get(),services_.debug_files,{this,service},e);
 if(!manager_)return false;
 native_=std::make_unique<trophies::TrophyNativeBindingsV1>(*manager_);return true;
}
bool SourceProcessTrophiesV100::load(std::string& e){
 if(load_attempted_)return required("once-only startup LoadTrophies",e);load_attempted_=true;
 if(!manager_)return required("published constructor before Init/Load",e);
 //Init37f9d0 is the original literal return; C1 already called InitTrophies.
 std::shared_ptr<application::ApplicationSaveFilesOwnerV61> actual;
 if(!files(actual,e))return false;
 if(!actual){ready_=true;return true;} //Observed original FileManager34NULL.
 bool present{};std::vector<std::uint8_t> bytes;
 if(!actual->read_save(filename,present,bytes,e))return false;
 if(!present){ready_=true;return true;} //Actual openSavefile read returnsNULL.
 if(bytes.size()<16)return required("full source bitset128 stream read",e);
 std::array<std::uint32_t,4> bits{};
 for(unsigned word=0;word<4;++word)for(unsigned byte=0;byte<4;++byte)
  bits[word]|=std::uint32_t(bytes[word*4+byte])<<(byte*8);
 //Original Load calls UnlockTrophy for each stored bit, including actual
 //text/UI/save callbacks. Do not directly overwrite rows or suppress effects.
 if(manager_->load_bitmap(bits)!=0){e=manager_->error();return false;}
 ready_=true;return true;
}
bool SourceProcessTrophiesV100::save_bitmap(const std::array<std::uint32_t,4>& bits,std::string& e){
 std::shared_ptr<application::ApplicationSaveFilesOwnerV61> actual;if(!files(actual,e))return false;
 if(!actual)return true; //Genuine source FileManager absence branch.
 //FileManager.openSavefile's matching job flush precedes raw stream open.
 if(!actual->flush(filename,e))return false;
 auto services=actual->files()->services();void* stream{};
 if(!services.open_write||!services.write||!services.close)return required("same raw save stream operations",e);
 if(!services.open_write(services.context,filename,stream,e)||!stream)return required("actual trophy save stream",e);
 std::array<std::uint8_t,16> bytes{};
 for(unsigned word=0;word<4;++word)for(unsigned byte=0;byte<4;++byte)
  bytes[word*4+byte]=std::uint8_t(bits[word]>>(byte*8));
 std::uint64_t written{};const bool delivered=services.write(services.context,stream,{bytes.data(),bytes.size()},written,e);
 std::string close_error;const bool closed=services.close(services.context,stream,close_error);
 if(!delivered||written!=16){if(e.empty())e="Required original trophy bitset128 full write";return false;}
 if(!closed){e=close_error;return false;}return true;
}
int SourceProcessTrophiesV100::service(void* raw,const trophies::TrophyRequestV1* q,trophies::TrophyResponseV1* out){
 if(!raw||!q||!out)return -1;auto& self=*static_cast<SourceProcessTrophiesV100*>(raw);*out={};
 if(!self.application_.lock()){self.error_="Process trophy Application expired";return -1;}
 using namespace trophies;
 switch(q->service){
 case resolve_text_v1:return self.services_.text&&self.services_.text(q->text_id,out->text,self.error_)?0:-1;
 case application_in_game_v1:return self.services_.in_game&&self.services_.in_game(out->boolean,self.error_)?0:-1;
 case queue_message_v1:return q->message&&self.services_.achievement&&self.services_.achievement(*q->message,self.error_)?0:-1;
 case save_bitmap_v1:return q->bitmap&&self.save_bitmap(*q->bitmap,self.error_)?0:-1;
 default:self.error_="Unknown original process trophy service";return -1;
 }
}
bool borrow_source_process_trophies_v100(const std::shared_ptr<application::ApplicationServicesOwnerV5>& app,
 ProcessTrophyBorrowV100& out,std::string& e){
 if(!process_trophies||!process_trophies->belongs_to(app)||!process_trophies->ready())
  return required("completed GSInit11 process singleton",e);
 out={process_trophies,process_trophies->manager(),process_trophies->native()};e.clear();return true;
}
SourceGSInitProcessOwnersV100::SourceGSInitProcessOwnersV100(
 std::shared_ptr<application::ApplicationServicesOwnerV5> app,GSInitStageServicesV100 stages,
 ProcessTrophyServicesV100 trophies):application_(std::move(app)),stages_(std::move(stages)),trophy_services_(std::move(trophies)){}
bool SourceGSInitProcessOwnersV100::update(std::string& e){
 if(busy_||failed_){e=error_.empty()?"GSInit startup delivery reentered":error_;return false;}
 auto app=application_.lock();if(!app)return required("live GSInit Application",e);
 auto fail=[this,&e](const char* reason){failed_=true;if(e.empty())e=reason;error_=e;return false;};
 struct Busy{bool& v;Busy(bool& b):v(b){v=true;}~Busy(){v=false;}}scope(busy_);
 try{
  if(stage4_==0){if(!stages_.audio_create||!stages_.audio_create(e))return fail("Required actual GSInit0 SoundManager.CreateInstance");++stage4_;return true;}
  if(stage4_==1){if(!stages_.settings_load||!stages_.settings_load(true,e))return fail("Required actual GSInit1 loadSettings(true)");++stage4_;return true;}
  if(stage4_==4){
   bool loaded{};
   if(!stages_.settings_loaded||!stages_.settings_loaded(loaded,e))return fail("Required actual GSInit4 settings byte28");
   if(!loaded&&(!stages_.settings_load||!stages_.settings_load(false,e)))return fail("Required actual GSInit4 loadSettings(false)");
   ++stage4_;return true;
  }
  if(stage4_==5||stage4_==7){
   const auto settings=app->source_settings4c_v67();if(!settings)return fail("Required actual App4c startup language");
   const auto& call=stage4_==5?stages_.start_movie:stages_.load_splash_texture;
   if(!call||!call(settings->language(),e))return fail("Required actual startup movie/texture owner");
   ++stage4_;return true;
  }
  if(stage4_==6){
   bool finished{};if(!stages_.movie_finished||!stages_.movie_finished(finished,e))return fail("Required actual native movie completion flag");
   if(finished)stage4_=7;e.clear();return true;
  }
  if(stage4_==8){stage4_=9;e.clear();return true;}
  if(stage4_==12){
   if(!stages_.menu_init||!stages_.device_time)return fail("Required actual MenuManager.Init and Device.Timer");
   std::uint32_t start{};if(!stages_.device_time(start,e))return fail("Startup menu timer read failed");
   for(;;){
    bool complete{};if(!stages_.menu_init(complete,e))return fail("Original MenuManager.Init failed");
    if(complete){++stage4_;e.clear();return true;}
    std::uint32_t now{};if(!stages_.device_time(now,e))return fail("Startup menu timer read failed");
    const auto raw=now-start;std::int32_t elapsed{};std::memcpy(&elapsed,&raw,sizeof(elapsed));
    if(elapsed>49){e.clear();return true;}
   }
  }
  if(stage4_==13){if(!stages_.reset_startup_clocks||!stages_.reset_startup_clocks(e))return fail("Required actual GSInit13 timer fields/ComputeDt");++stage4_;return true;}
  if(stage4_==14){if(!stages_.enter_main_state||!stages_.enter_main_state(e))return fail("Required actual GSInit14 Reset/main state/platform calls");++stage4_;return true;}
  if(stage4_==2||stage4_==3){
   if(!stages_.owner||!stages_.device_time||
      (stage4_==2?!stages_.constants_load:!stages_.arrays_load))
    return fail("Required actual process PyData.Load and Device.Timer at GSInit2/3");
   std::uint32_t start{};
   if(!stages_.device_time(start,e))return fail("GSInit2 initial Device.Timer read failed");
   const auto debug=[this](const char* key,std::string& error){
    const auto& s=trophy_services_;if(!s.debug||!s.debug_files||!s.debug_file_owner){error="Required actual process PyDataConstants Debug owner";return false;}
    const int loaded=dh2_character_debug_load(s.debug.get(),s.debug_files);
    if(loaded<0){error="Process PyDataConstants Debug.load failed";return false;}
    std::uint32_t ignored{};if(dh2_character_debug_get(&ignored,s.debug.get(),key,s.debug_files)<0){error="Process PyDataConstants Debug.GetSwitch failed";return false;}
    return true;
   };
   for(;;){
    bool complete{};
    if(!(stage4_==2?stages_.constants_load(debug,complete,e):stages_.arrays_load(complete,e)))
     return fail("GSInit process PyData.Load failed");
    if(complete){++stage4_;e.clear();return true;}
    std::uint32_t now{};if(!stages_.device_time(now,e))return fail("GSInit2 Device.Timer read failed");
    const std::uint32_t raw=now-start;std::int32_t elapsed{};std::memcpy(&elapsed,&raw,sizeof(elapsed));
    if(elapsed>49){e.clear();return true;}
   }
  }
  if(stage4_==9||stage4_==10){
   if(stage4_==9){
    audio::AudioInitializeServicesV100 services;
    //Audio owns the complete Initialize body. Supply the SAME actual
    //process Debug receiver used by trophies; preserve load then GetSwitch.
    auto debug=trophy_services_.debug;auto files=trophy_services_.debug_files;
    auto file_owner=trophy_services_.debug_file_owner;
    services.debug=[debug,files,file_owner](const char* key,bool& value,std::string& error){
     if(!debug||!files||!file_owner||!key){error="Required actual process audio Debug owner/files";return false;}
     const int loaded=dh2_character_debug_load(debug.get(),files);
     if(loaded<0){error="Process audio Debug.load failed: "+std::to_string(loaded);return false;}
     std::uint32_t word{};const int status=dh2_character_debug_get(&word,debug.get(),key,files);
     if(status<0){error="Process audio Debug.GetSwitch failed: "+std::to_string(status);return false;}
     value=word!=0;error.clear();return true;
    };
    if(!model_renderer::initialize_process_audio_v100(app,std::move(services),e))return fail("GSInit9 SoundManager.Initialize failed");
   }
   //385134 falls through385138: phase9 also switches the language pack.
   //The next frame at phase10 repeats this SAME call, then advances to11.
   const auto settings=app->source_settings4c_v67();
   ui::HudTextV1* strings{};std::shared_ptr<void> string_owner;
   if(!settings||!stages_.string_manager||
      !stages_.string_manager(strings,string_owner,e)||!strings||!string_owner)
    return fail("Required actual App4c settings and App34 StringManager at GSInit9/10");
   if(!strings->switch_pack(settings->language(),false,e))return fail("GSInit StringManager.switchPack failed");
   ++stage4_;e.clear();return true;
  }
  if(stage4_==11){
   if(!trophies_){
    if(process_trophies){if(!process_trophies->belongs_to(app))return fail("Different Application already owns process trophies");trophies_=process_trophies;}
    else{
     trophies_=std::shared_ptr<SourceProcessTrophiesV100>(new SourceProcessTrophiesV100(app,trophy_services_));
     if(!trophies_->construct(e))return fail("GSInit trophy constructor failed");
     process_trophies=trophies_; //Original stores singleton AFTER C1, BEFORE Init/Load.
    }
   }
   if(!trophies_->load(e))return fail("GSInit trophy Init/Load prefix failed");
   ++stage4_;e.clear();return true;
  }
  return fail("GSInit original stage assertion domain (finished states must not Update again)");
 }catch(const std::exception& ex){e=ex.what();return fail("GSInit source owner threw");}
}
}
namespace model_renderer {
bool borrow_source_process_trophies_v100(dh2::android_ui::ProcessTrophyBorrowV100& out,std::string& e){
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;
 if(!borrow_actual_application_services_v5(app,e))return false;
 return dh2::android_ui::borrow_source_process_trophies_v100(app,out,e);
}
}
