#pragma once
#include "event_manager_owner_v12.hpp"
#include <array>
#include "gameplay_camera_active_v13.hpp"
#include "source_loading_application_fields_v55.hpp"
#include "source_online_loading_owner_v55.hpp"
namespace dh2::character {struct DebugSwitches;}
namespace dh2::loader {class ScriptManagerOwnerV52;}
namespace dh2::player {class ApplicationPlayerManagerBootstrapV59;}
namespace dh2::input {class SourceInputManagerV60;}
namespace dh2::camera {class GameplayCameraZoomV17;}
namespace dh2::world {class ApplicationSpawnRandomOwnerV4;}
namespace dh2::world {class SourceProcessObjectsV121;}
namespace dh2::fx {class VisualFxManagerLibrariesV63;}
namespace dh2::ui {class OwnedHudSettingsV1;}
namespace dh2::application {
class ApplicationSaveFilesOwnerV61;
class PlayerLightTweakerOwnerV90;
class ProcessResourcePrefixSourceV95;
struct ProcessResourcePrefixBorrowV95;
// Retained native Application identity and currently reconstructed service
// fields. This is not a claim that the whole Application ctor/PostInit/Shutdown
// is implemented. C1 zeros pointer14; the first PostInit service allocation
// constructs the distinct heap EventManager and publishes that SAME pointer.
class ApplicationServicesOwnerV5 {
 std::shared_ptr<events::EventManagerOwnerV12> events14_;
 camera::GameplayCameraActiveV13 active_camera_; // ONE process source global
 bool event_publication_attempted_{};
 // Native integration lifetime for the retained renderer's sole camera
 // service context. This is not a recovered Application byte-offset field.
 // SceneManager/factory/Zoom/animation services survive individual Worlds
 // and GL contexts; the adapter must not hold a strong Application cycle.
 std::shared_ptr<void> native_camera_services_v20_;
 //ONE source ZoomHandler process singleton, including its genuine pre-World
 //GetInstance from menu Hide. Later camera services borrow this SAME C1.
 std::shared_ptr<camera::GameplayCameraZoomV17> source_zoom_v119_;
 std::shared_ptr<world::SourceProcessObjectsV121> source_objects_v121_;
 std::shared_ptr<ProcessResourcePrefixSourceV95> source_resource_prefix_v95_;
 SourceLoadingApplicationFieldsV55 source_loading_v55_;
 std::shared_ptr<SourceOnlineLoadingOwnerV55> online_loading_v55_;
 // Native lifetime pins for genuine process singleton projections. Their
 // provider closures must not strongly capture this Application or a World.
 std::shared_ptr<character::DebugSwitches> source_debug_services_v55_;
 std::shared_ptr<loader::ScriptManagerOwnerV52> source_script_manager_v52_;
 std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> source_player_manager_v59_;
 // Native pin for the linked process InputManager singleton; no recovered
 // Application byte offset or hardware-detection result is asserted.
 std::shared_ptr<input::SourceInputManagerV60> source_input_manager_v60_;
 // Native retention for the actual private FileManager transport and sole
 // source Savegame jobs list. No new original Application byte offset.
 std::shared_ptr<ApplicationSaveFilesOwnerV61> source_save_files_v61_;
 // Pins the ONE process Random BSS projection used by source loading/spawn/
 // loot. Source Level stage7 writes these same channel seeds, never copies.
 std::shared_ptr<world::ApplicationSpawnRandomOwnerV4> source_random_v62_;
 std::shared_ptr<fx::VisualFxManagerLibrariesV63> source_fx_libraries_v63_;
 // SAME App+4c SavegameManager, already constructed/loaded by original UI.
 // It is also the sole mutable NativeSetCurrentDifficulty+0c authority.
 std::shared_ptr<ui::OwnedHudSettingsV1> source_settings4c_v67_;
 std::array<std::shared_ptr<PlayerLightTweakerOwnerV90>,4> source_tweakers58_v90_{};
public:
 ApplicationServicesOwnerV5()=default;
 bool initialize_source_resource_prefix_v95(std::string&);
 bool borrow_source_resource_prefix_v95(ProcessResourcePrefixBorrowV95&,std::string&)const;
 ApplicationServicesOwnerV5(const ApplicationServicesOwnerV5&)=delete;
 ApplicationServicesOwnerV5& operator=(const ApplicationServicesOwnerV5&)=delete;
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 SourceLoadingApplicationFieldsV55& source_loading_v55()noexcept{return source_loading_v55_;}
 const SourceLoadingApplicationFieldsV55& source_loading_v55()const noexcept{return source_loading_v55_;}
 const std::shared_ptr<world::ApplicationSpawnRandomOwnerV4>& source_random_v62();
 const std::shared_ptr<fx::VisualFxManagerLibrariesV63>& source_fx_libraries_v63()const noexcept{return source_fx_libraries_v63_;}
 bool publish_source_fx_libraries_v63(std::shared_ptr<fx::VisualFxManagerLibrariesV63>,std::string&);
 const std::shared_ptr<ui::OwnedHudSettingsV1>& source_settings4c_v67()const noexcept{return source_settings4c_v67_;}
 bool publish_source_settings4c_v67(std::shared_ptr<ui::OwnedHudSettingsV1>,std::string&);
 const std::shared_ptr<SourceOnlineLoadingOwnerV55>& get_online_loading_v55(){
  if(!online_loading_v55_)online_loading_v55_=std::make_shared<SourceOnlineLoadingOwnerV55>();
  return online_loading_v55_;
 }
 const std::shared_ptr<character::DebugSwitches>& source_debug_services_v55()const noexcept{return source_debug_services_v55_;}
 bool publish_source_debug_services_v55(std::shared_ptr<character::DebugSwitches> owner,std::string& error){
  if(!owner||(source_debug_services_v55_&&(source_debug_services_v55_.get()!=owner.get()||
      source_debug_services_v55_.owner_before(owner)||owner.owner_before(source_debug_services_v55_)))){
   error="Application already owns a different source Debug singleton";return false;
  }
  source_debug_services_v55_=std::move(owner);error.clear();return true;
 }
 const std::shared_ptr<loader::ScriptManagerOwnerV52>& source_script_manager_v52()const noexcept{return source_script_manager_v52_;}
 bool publish_source_script_manager_v52(std::shared_ptr<loader::ScriptManagerOwnerV52> owner,std::string& error){
  if(!owner||(source_script_manager_v52_&&(source_script_manager_v52_.get()!=owner.get()||
      source_script_manager_v52_.owner_before(owner)||owner.owner_before(source_script_manager_v52_)))){
   error="Application already owns a different source ScriptManager singleton";return false;
  }
  source_script_manager_v52_=std::move(owner);error.clear();return true;
 }
 const std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59>& source_player_manager_v59()const noexcept{return source_player_manager_v59_;}
 const std::shared_ptr<ApplicationSaveFilesOwnerV61>& source_save_files_v61()const noexcept{return source_save_files_v61_;}
 bool publish_source_save_files_v61(std::shared_ptr<ApplicationSaveFilesOwnerV61>,std::string&);
 const std::shared_ptr<input::SourceInputManagerV60>& source_input_manager_v60()const noexcept{return source_input_manager_v60_;}
 bool publish_source_input_manager_v60(std::shared_ptr<input::SourceInputManagerV60> owner,std::string& error){
  if(!owner||(source_input_manager_v60_&&(source_input_manager_v60_.get()!=owner.get()||
      source_input_manager_v60_.owner_before(owner)||owner.owner_before(source_input_manager_v60_)))){
   error="Application already retains a different source InputManager singleton";return false;
  }
  source_input_manager_v60_=std::move(owner);error.clear();return true;
 }
 bool publish_source_player_manager_v59(std::shared_ptr<player::ApplicationPlayerManagerBootstrapV59> owner,std::string& error){
  if(!owner||(source_player_manager_v59_&&(source_player_manager_v59_.get()!=owner.get()||
      source_player_manager_v59_.owner_before(owner)||owner.owner_before(source_player_manager_v59_)))){
   error="Application already owns a different source PlayerManager";return false;
  }
  source_player_manager_v59_=std::move(owner);error.clear();return true;
 }
 bool post_init_events_v5(std::string&);
 const std::shared_ptr<events::EventManagerOwnerV12>& events14()const noexcept{return events14_;}
 const std::shared_ptr<PlayerLightTweakerOwnerV90>& source_tweaker58_v90(std::size_t index=0)const{return source_tweakers58_v90_.at(index);}
 bool publish_source_tweaker58_v90(std::size_t index,std::shared_ptr<PlayerLightTweakerOwnerV90> value,std::string& e){
  if(index>=4||!value){e="Required actual App58..64 tweaker receiver";return false;}
  auto& slot=source_tweakers58_v90_[index];
  if(slot&&(slot.get()!=value.get()||slot.owner_before(value)||value.owner_before(slot))){e="Different source tweaker already published on actual App";return false;}
  slot=std::move(value);e.clear();return true;
 }
 camera::GameplayCameraActiveV13& active_camera()noexcept{return active_camera_;}
 const std::shared_ptr<void>& native_camera_services_v20()const noexcept{return native_camera_services_v20_;}
 const std::shared_ptr<camera::GameplayCameraZoomV17>& source_zoom_v119()const noexcept{return source_zoom_v119_;}
 const std::shared_ptr<world::SourceProcessObjectsV121>& source_objects_v121()const noexcept{return source_objects_v121_;}
 bool publish_source_objects_v121(std::shared_ptr<world::SourceProcessObjectsV121> owner,std::string& e){
  if(!owner||(source_objects_v121_&&(source_objects_v121_.get()!=owner.get()||
     source_objects_v121_.owner_before(owner)||owner.owner_before(source_objects_v121_)))){
   e="Application already owns a different process ObjectManager";return false;
  }
  source_objects_v121_=std::move(owner);e.clear();return true;
 }
 bool publish_source_zoom_v119(std::shared_ptr<camera::GameplayCameraZoomV17> owner,std::string& e){
  if(!owner||(source_zoom_v119_&&(source_zoom_v119_.get()!=owner.get()||
     source_zoom_v119_.owner_before(owner)||owner.owner_before(source_zoom_v119_)))){
   e="Application already owns a different source ZoomHandler";return false;
  }
  source_zoom_v119_=std::move(owner);e.clear();return true;
 }
 bool publish_native_camera_services_v20(std::shared_ptr<void> owner,std::string& error){
  if(!owner){error="Required retained native camera services owner";return false;}
  if(native_camera_services_v20_&&native_camera_services_v20_.get()!=owner.get()){
   error="Application camera services already published";return false;
  }
  native_camera_services_v20_=std::move(owner);return true;
 }
};
}
