#pragma once
#include "trophy_manager_owner_v1.hpp"
#include "application_services_owner_v5.hpp"
#include "application_save_files_owner_v61.hpp"
#include "original_cache_assets_v1.hpp"
#include "hud_text_v1.hpp"
namespace dh2::android_ui {
struct ProcessTrophyServicesV100 {
 std::shared_ptr<void> owner; //Independent UI/cache/platform lifetime; App weak.
 std::shared_ptr<character::DebugSwitches> debug;
 const character::DebugFileServices24* debug_files{};
 std::shared_ptr<void> debug_file_owner;
 std::function<bool(const char*,bool&,std::vector<std::uint8_t>&,std::string&)> cache;
 //Actual process FileManager getter, including genuine NULL only when its
 //source slot is observed NULL. Missing native enrollment is a failure.
 std::function<bool(std::shared_ptr<application::ApplicationSaveFilesOwnerV61>&,std::string&)> files;
 std::function<bool(std::int32_t,std::string&,std::string&)> text;
 std::function<bool(bool&,std::string&)> in_game;
 std::function<bool(const trophies::AchievementMessageV1&,std::string&)> achievement;
};
class SourceProcessTrophiesV100 {
 std::weak_ptr<application::ApplicationServicesOwnerV5> application_;
 ProcessTrophyServicesV100 services_;
 std::shared_ptr<const trophies::TrophyCatalogV1> catalog_;
 std::unique_ptr<trophies::TrophyManagerOwnerV1> manager_;
 std::unique_ptr<trophies::TrophyNativeBindingsV1> native_;
 bool construct_attempted_{},load_attempted_{},ready_{};
 std::string error_;
 explicit SourceProcessTrophiesV100(std::shared_ptr<application::ApplicationServicesOwnerV5>,ProcessTrophyServicesV100);
 static int service(void*,const trophies::TrophyRequestV1*,trophies::TrophyResponseV1*);
 bool files(std::shared_ptr<application::ApplicationSaveFilesOwnerV61>&,std::string&);
 bool save_bitmap(const std::array<std::uint32_t,4>&,std::string&);
 bool construct(std::string&);bool load(std::string&);
 friend class SourceGSInitProcessOwnersV100;
public:
 bool belongs_to(const std::shared_ptr<application::ApplicationServicesOwnerV5>&)const noexcept;
 bool ready()const noexcept{return ready_;}
 trophies::TrophyManagerOwnerV1* manager()const noexcept{return manager_.get();}
 trophies::TrophyNativeBindingsV1* native()const noexcept{return native_.get();}
 const std::string& error()const noexcept{return error_;}
};
struct ProcessTrophyBorrowV100 {
 std::shared_ptr<SourceProcessTrophiesV100> owner;
 trophies::TrophyManagerOwnerV1* manager{};
 trophies::TrophyNativeBindingsV1* native{};
};
//Read only. Does not allocate, initialize or load on gameplay callbacks.
bool borrow_source_process_trophies_v100(const std::shared_ptr<application::ApplicationServicesOwnerV5>&,
 ProcessTrophyBorrowV100&,std::string&);
//Actual GSInit scalar stage owner. Source C1 produces stage4=0. Root lends
//its genuine remaining startup bodies; each call executes one original stage.
//Stage11 is implemented here and publishes the sole process trophy receiver.
struct GSInitStageServicesV100 {
 std::shared_ptr<void> owner;
 // Borrow App+34's actual retained StringManager, including its lifetime.
 // This is not an inspection movie's independent localization cache.
 std::function<bool(ui::HudTextV1*&,std::shared_ptr<void>&,std::string&)> string_manager;
 std::function<bool(std::uint32_t&,std::string&)> device_time;
 std::function<bool(const std::function<bool(const char*,std::string&)>&,bool&,std::string&)> constants_load;
 std::function<bool(bool&,std::string&)> arrays_load;
 //Typed actual platform/process calls; GSInit owns all stage control.
 std::function<bool(std::string&)> audio_create,reset_startup_clocks,enter_main_state;
 std::function<bool(bool language_only,std::string&)> settings_load;
 std::function<bool(bool&,std::string&)> settings_loaded,movie_finished,menu_init;
 std::function<bool(std::int32_t language,std::string&)> start_movie,load_splash_texture;
};
class SourceGSInitProcessOwnersV100 {
 std::weak_ptr<application::ApplicationServicesOwnerV5> application_;
 GSInitStageServicesV100 stages_;
 ProcessTrophyServicesV100 trophy_services_;
 std::int32_t stage4_{};
 std::shared_ptr<SourceProcessTrophiesV100> trophies_;
 bool busy_{},failed_{};std::string error_;
public:
 SourceGSInitProcessOwnersV100(std::shared_ptr<application::ApplicationServicesOwnerV5>,
  GSInitStageServicesV100,ProcessTrophyServicesV100);
 bool update(std::string&);
 std::int32_t stage()const noexcept{return stage4_;}
 bool failed()const noexcept{return failed_;}
 bool complete()const noexcept{return stage4_==15&&!failed_;}
};
}
namespace model_renderer {
bool borrow_source_process_trophies_v100(dh2::android_ui::ProcessTrophyBorrowV100&,std::string&);
}
