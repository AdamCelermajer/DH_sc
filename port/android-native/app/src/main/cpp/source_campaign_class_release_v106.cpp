#include "source_campaign_class_release_v106.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include <physical_world.hpp>
#include <gameobject_scene_root_registry_v1.hpp>
namespace model_renderer {
bool require_source_campaign_class_delivery_v106(const std::shared_ptr<void>& actual,std::string& e){
 SourceCampaignCandidateBorrowV55 c;std::shared_ptr<SourceWorldBorrowV61> w;
 if(!borrow_source_campaign_candidate_runtime_v61(c,e)||!actual||c.actual_world!=actual||c.actual_world.owner_before(actual)||actual.owner_before(c.actual_world)||
  !borrow_source_campaign_condition_world_v70(c,w,e)||!w->admission_v104||!c.physical_world||!c.roots){if(e.empty())e="Required actual class-release delivery owners";return false;}
 if(!w->admission_v104->require_class_cleanup_v106(e))return false;
 if(!c.physical_world->cleanup_delivery_idle_v106()){e="Class D0 overlaps actual PhysicalWorld Step/filter/contact/mutation delivery";return false;}
 if(c.roots->source_delivery_busy_v106()){e="Class D0 overlaps actual Scene animator/cache delivery";return false;}
 e.clear();return true;
}
}
