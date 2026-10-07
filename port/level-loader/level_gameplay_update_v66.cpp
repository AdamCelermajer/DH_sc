#include "level_gameplay_update_v66.hpp"
#include <exception>
namespace dh2::loader {
bool LevelGameplayUpdateV66::reject(const char* name){
 failed_=true;required_=name;if(error_.empty())error_=std::string("Required actual Level.Update: ")+name;return false;
}
bool LevelGameplayUpdateV66::switch_value(const char* key,bool& out){
 return call("DebugSwitches.load",services_.debug_load)&&call(key,services_.debug_switch,key,out);
}
bool LevelGameplayUpdateV66::body(){
 if(!call("MenuDebugHUD.GetInstance/counter-map-clear",services_.prepare_debug_hud))return false;
 bool toggle{},paused{};
 if(!switch_value("IsUsingToggleDisplayMode",toggle))return false;
 if(toggle)return true;
 if(!switch_value("IsLevelUpdatePaused",paused))return false;
 if(paused)return true;
 // This SAME C1 fields reference remains authoritative across every callback.
 auto& fields=*level_->constructor_borrow_v3().fields;
 if(fields.field148!=-1){
  bool view{};if(!call("Application.IsCurrentlyInGameView",services_.in_game_view,view))return false;
  if(view&&!call("ScriptManager.StartScript",services_.start_script,fields.field148,-1,false))return false;
 }
 std::uintptr_t character{};
 if(!call("PM.GetLocalPlayer(0,true)/Character660",services_.local_character,character))return false;
 if(character&&!call("Character.SG_Update(false)",services_.save_update,character,false))return false;
 if(!fields.byte144&&!call("PlayerManager.Update",services_.player_manager_update))return false;
 std::uintptr_t blocked{};
 if(!call("PlayerManager.actual714",services_.player_manager714,blocked))return false;
 if(!blocked&&!call("ScriptManager.ExecuteAllScripts",services_.execute_all_scripts))return false;
 if(!fields.field194)return reject("actual Level.GameEventManager194");
 if(!call("GameEventManager.Update",services_.game_events_update)||
    !call("CSceneManager.setAmbientLight(actual LevelConfig38)",services_.set_ambient)||
    !call("PhysicalWorld.update(actual App.dt8c)",services_.physical_update)||
    !call("SpawnGroupManager.update(double0)",services_.update_spawn_groups,0.0)||
    !call("CharAI.HandleGroups",services_.handle_ai_groups)||
    !call("ObjectManager.Update(float1)",services_.update_objects,1.0f)||
    !call("CharAI.IncUpdateQueue",services_.inc_ai_queue)||
    !call("Level.UpdateCameraZoom",services_.update_zoom)||
    !call("VisualFXManager.Update",services_.update_fx))return false;
 std::uintptr_t override{};
 if(fields.field12c){
  if(!call("actual camera override global",services_.camera_override_global,override))return false;
  if(fields.field12c==override&&!call("Camera12c.virtual10",services_.camera_override_update))return false;
 }
 if(!fields.field128)return reject("actual Level.Camera128");
 if(!call("Camera128.virtual10",services_.camera_update)||
    !call("Camera128.Node8.virtualB8(false)",services_.camera_node_update_false))return false;
 if(!call("SceneManager.UpdateFogEff(actual camera/LevelConfig)",services_.update_camera_fog)||
    !call("source Debug/render flags430/431",services_.update_render_flags)||
    !call("source Level sound144 suffix",services_.start_level_sound)||
    !call("Level.UpdateListener",services_.update_listener)||
    !call("Level.UpdateDynamicFog",services_.update_dynamic_fog))return false;
 bool automatic{};if(!switch_value("IsAutoLoadingLevels",automatic))return false;
 return !automatic||call("source positive automatic level loading",services_.auto_level_loading);
}
bool LevelGameplayUpdateV66::update(const std::shared_ptr<CanonicalLevelContextV1>& receiver,bool force,std::string& e){
 if(failed_){e=error_;return false;}
 if(busy_){reject("nonreentrant actual owning frame");e=error_;return false;}
 if(!receiver||receiver!=level_||receiver.owner_before(level_)||level_.owner_before(receiver)||
    !services_.actual_application||!services_.actual_object_manager||!services_.provider||
    !level_->constructor_owner_v3()||level_->constructor_owner_v3()->phase()!=LevelConstructorPhaseV3::complete){
  reject("SAME completed C1 Level/App/ObjectManager providers");e=error_;return false;
 }
 if(level_->constructor_fields_v3().field130!=38&&!force){reject("original gameplay selector38/force");e=error_;return false;}
 struct Guard{bool& busy;explicit Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;}} guard(busy_);
 try{if(!body()||failed_){e=error_;return false;}e.clear();return true;}
 catch(const std::exception& ex){error_=ex.what();reject("reached provider exception");e=error_;return false;}
 catch(...){reject("unknown reached provider exception");e=error_;return false;}
}
}
