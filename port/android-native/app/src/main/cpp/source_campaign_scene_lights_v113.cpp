#include "source_campaign_scene_lights_v113.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "renderer_character_campaign_v62.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <gameobject_scene_root_registry_v1.hpp>
namespace model_renderer {
bool prepare_source_campaign_light_set_v113(const SourceCampaignCandidateBorrowV55& actual,std::shared_ptr<dh2::world::NativeLightSetV113>& out,std::string& e){
 SourceCampaignCandidateBorrowV55 current;if(!actual.actual_world||!actual.roots||!borrow_source_campaign_candidate_runtime_v61(current,e)||current.actual_world!=actual.actual_world||current.roots!=actual.roots){if(e.empty())e="Actual SAME Scene light publication scope required";return false;}
 auto names=actual.roots->source_light_names_v89();if(!names){e="Actual LightSet name C1 owner absent";return false;}
 out=actual.roots->source_light_runtime_v113();if(out){if(out->names()!=names){e="Replaced SAME LightSet source name owner";return false;}e.clear();return true;}
 //The existing Scene C1 executed the name subset; install only its missing
 //CLight slots/changed flags/static-active C1-empty domains, once on that owner.
 auto fresh=std::make_shared<dh2::world::NativeLightSetV113>(names);if(!actual.roots->publish_light_runtime_v113(fresh,e))return false;out=std::move(fresh);e.clear();return true;
}
bool source_campaign_character_init_light_material_v113(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCharacterBorrowV62 borrow;if(!borrow_source_campaign_character_v62(world,id,borrow,e)||!borrow.character||!borrow.character->actor)return false;
 auto& record=*borrow.character;const auto actual_visual=record.actor->source_visual();if(!actual_visual){e.clear();return true;} //original3b4860 NULL2d8 return.
 auto visual=record.visual?record.visual->visual():nullptr;
 if(!visual||reinterpret_cast<std::uintptr_t>(visual.get())!=actual_visual){e="SAME source Character2d8 visual required";return false;}
 SourceCampaignCandidateBorrowV55 actual;std::shared_ptr<dh2::world::NativeLightSetV113> lights;if(!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=world||!prepare_source_campaign_light_set_v113(actual,lights,e))return false;
 std::vector<bool> temporary;dh2::world::NativeLightSetV113::initialize_filter(temporary,true);
 const auto set=lights->names()->get_id("MonsterLight");visual->store_light_set(set);visual->source_light_filter44_v113()=temporary;
 //Exact original3b48f8→4900→4908→4910→4918 order. A reached positive
 //unbound auxiliary pass remains a failure with its prefix stores preserved.
 return visual->source_apply_light_set_v113(*lights,e)&&visual->source_apply_material_tail_v113(e);
}
}
