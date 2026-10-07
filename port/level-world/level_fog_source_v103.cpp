#include "level_fog_source_v103.hpp"
namespace dh2::world {
bool source_level_update_fog_v103(const LevelFogServicesV103& s,std::string& e){
 if(!s.provider||!s.current_config){e="Required actual Level fog config provider";return false;}
 std::shared_ptr<CanonicalLevelConfigV1> config;
 if(!s.current_config(config,e))return false;
 if(!config){e.clear();return true;} // Original UpdateFog NULL38 return.
 const auto* end=config->integer(0x1dc);
 if(!end){e="Required source LevelConfig fog_end1dc";return false;}
 const bool enabled=*end!=0;
 //Both branches call Debug.load/GetSwitch(isTracingLevel); returned switch
 //does not select fog behavior. Preserve its callbacks before scene stores.
 if(!s.trace_level||!s.trace_level(e))return false;
 if(enabled&&!s.current_config(config,e))return false;
 std::shared_ptr<GameObjectSceneRootRegistryV1> scene;
 if(!s.scene||!s.scene(scene,e)||!scene){if(e.empty())e="Required SAME source SceneManager for fog";return false;}
 if(!enabled){scene->source_disable_fog_v67();e.clear();return true;}
 if(!config){e="Original EnableFog NULL LevelConfig assertion continuation";return false;}
 const auto* start=config->integer(0x1d8);end=config->integer(0x1dc);
 const auto* color=config->vector(0x1e0);
 if(!start||!end||!color){e="Required actual LevelConfig fog distance/color cells";return false;}
 scene->source_enable_fog_v68(static_cast<float>(*start),static_cast<float>(*end),*color);
 e.clear();return true;
}
}
