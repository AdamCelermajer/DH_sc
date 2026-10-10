#pragma once
#include <functional>
#include <string>

namespace model_renderer {
// SetupCharacter is a no-op until MainMenu::SetupScene publishes its scene
// node. If that same retained scene outlives a renderer teardown, restore the
// process PhysicalWorld only when no preview Character can still own bodies.
bool ensure_menu_preview_physics_v124(bool scene_present,bool character_present,
 const std::function<bool(bool&,std::string&)>& ready,
 const std::function<bool(float,float,float,float,std::string&)>& load,std::string& error){
 if(!scene_present){error.clear();return true;}
 if(!ready){error="Required same process menu PhysicalWorld readiness";return false;}
 bool loaded{};
 if(!ready(loaded,error))return false;
 if(loaded){error.clear();return true;}
 if(character_present){error="Cannot restore menu PhysicalWorld while its preview Character is retained";return false;}
 if(!load){error="Required native main-scene PhysicalWorld loader";return false;}
 return load(0.f,0.f,1.f,1.f,error);
}
}
