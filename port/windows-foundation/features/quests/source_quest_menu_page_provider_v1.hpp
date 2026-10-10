#pragma once

#include "runtime_quest_menu_v1.hpp"
#include "../character_menu/source_composition.hpp"

namespace dh::foundation {

struct SourceQuestMenuHitRouteV1 {
    bool handled{};
    RuntimeQuestMenuHitV1 hit{RuntimeQuestMenuHitV1::quest_row_release};
    CharacterQuestIdV1 quest{};
};

using SourceQuestMenuHitResolverV1 = std::function<bool(
    float authored_x, float authored_y, SourceQuestMenuHitRouteV1&, std::string&)>;

struct SourceQuestMenuPageProviderV1 {
    static constexpr const char* source_menu_symbol = "menu_QuestLogSheetNEW";
    static constexpr const char* source_movie_sha256 =
        "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0";
};

// SourcePageProviderV1 receives authored coordinates unchanged. For this
// pushed movie they are the 480x320 root SWF stage in pixels; the viewport
// adapter must first map touch coordinates to that stage. Sprite599/page and
// row transforms are already present in the generated source art matrices.
bool resolve_source_quest_menu_hit_v1(
    const RuntimeQuestMenuFrameV1&, float authored_stage_x, float authored_stage_y,
    SourceQuestMenuHitRouteV1&, std::string& error);

SourceQuestMenuHitResolverV1 source_quest_menu_hit_resolver_v1(
    const std::shared_ptr<RuntimeQuestCharacterMenuBindingV1>&);

// Preferred production factory: uses the source-backed row/Activate hit
// resolver below; root only supplies the exact existing character and lease.
bool bind_source_quest_menu_page_provider_v1(
    const std::shared_ptr<RuntimeQuestCharacterMenuBindingV1>&,
    const std::shared_ptr<CharacterState>& same_character,
    const std::shared_ptr<void>& canonical_source_owner,
    character_menu::SourcePageProviderV1&,
    std::string& error);

// Packages the existing CQPG/HudText-backed page binding as a provider for
// the exact authored NativePushMenu symbol. The explicit resolver overload is
// retained for controlled source fixtures or specialized source adapters.
bool bind_source_quest_menu_page_provider_v1(
    const std::shared_ptr<RuntimeQuestCharacterMenuBindingV1>&,
    const std::shared_ptr<CharacterState>& same_character,
    const std::shared_ptr<void>& canonical_source_owner,
    SourceQuestMenuHitResolverV1,
    character_menu::SourcePageProviderV1&,
    std::string& error);

} // namespace dh::foundation
