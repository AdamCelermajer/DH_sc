#pragma once
#include <settings_language_scene_v1.hpp>
#include <memory>
#include <string>
namespace dh2::world {class CanonicalObjectManagerV1;}
namespace model_renderer {
struct SourceCampaignLanguageSceneV109 {
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager;
 dh2::ui::SettingsLanguageScene24V1* scene{};
};
// Borrow for one settings delivery. Persistent callers capture World weakly
// and obtain the current manager again; never retain this in that World.
bool borrow_source_campaign_language_scene_v109(const std::shared_ptr<void>&,
 SourceCampaignLanguageSceneV109&,std::string&);
}
