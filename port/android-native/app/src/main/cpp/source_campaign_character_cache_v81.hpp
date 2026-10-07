#pragma once
#include <memory>
#include <string>
namespace dh2::character {struct CharacterScriptSessionInput;struct CharacterOidPreloadServicesV81;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
//Native SceneManager resource adaptation is supplied by its real allocator/
//preloaded-root owner. Binding certifies ownership only, not any preload work.
bool bind_source_campaign_character_preload_v81(const SourceCampaignCandidateBorrowV55&,
 dh2::character::CharacterOidPreloadServicesV81,std::string&);
bool prepare_source_campaign_character_preload_v81(const SourceCampaignCandidateBorrowV55&,std::string&);
bool bind_source_campaign_register_summon_v81(const std::shared_ptr<void>& actual_world,
 dh2::character::CharacterScriptSessionInput&,std::string&);
//Only original LevelD1 calls this; drain/loading/context loss are not D1.
void clear_source_character_oid_cache_at_level_d1_v81()noexcept;
}
