#pragma once
#include "hud_initialization_v1.hpp"
#include "../game-data/skill_tables.hpp"
#include "../game-data/player_savegame_v1.hpp"
#include "../game-data/properties.hpp"
#include "owned_hud_settings_v1.hpp"
namespace dh2::ui {
// Immutable real SkillTable backing. The source Character+1068 list selector
// falls back to list3 when outside [0,list_count); list contents and row fields
// are retained in source order, without synthesized skill IDs/defaults.
class HudInitSkillCatalogV1 {
 data::SkillTables::Borrow tables_;std::vector<HudInitSkill96> records_;
public:
 explicit HudInitSkillCatalogV1(data::SkillTables::Borrow);
 HudInitSkillCatalogV1(const HudInitSkillCatalogV1&)=delete;
 HudInitSkillCatalogV1& operator=(const HudInitSkillCatalogV1&)=delete;
 const HudInitSkill96* character_skill(std::int32_t resolved_list,std::uint32_t row)const noexcept;
 const data::SkillTables::Borrow& tables()const noexcept{return tables_;}
};
struct HudInitPlayerProjectionV1 {
 std::uintptr_t identity{};data::PropertyView* properties{};
 const data::PlayerSavegameV1* saved{};const HudInitSkillCatalogV1* skills{};
 const std::int32_t* current_difficulty{};
};
// Handles only genuine decoded/saved/cached query requests for this retained
// actor. Returns1 delivered,0 reached required-failure,-1 outside owned domain.
// Missing saved owner is a genuine source null branch (including faery ID0).
// A nonnull owner with malformed row/list/difficulty is rejected, not a miss.
// Caller retains all property/save/catalog/difficulty backings across reentry.
int hud_initialization_owned_v1_query(const HudInitPlayerProjectionV1&,
 const HudInitRequest64&,HudInitResponse32&,std::string& error);
// Exact private Savegame option getters. Integer localization, language-build
// globals and Android JNI capability remain distinct required providers.
int hud_initialization_settings_v1_query(const OwnedHudSettingsV1&,
 const HudInitRequest64&,HudInitResponse32&);
}
