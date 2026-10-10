#include "../hotty_cast_v1.hpp"
#include "../hotty_character_cast_v1.hpp"
#include "../hotty_effects_v1.hpp"
#include "../celest_cast_v1.hpp"
#include "../celest_source_use_v1.hpp"
#include "../session_faery_page_v1.hpp"
#include "../../../combat_session.hpp"
#include "../../../original_actor_properties.hpp"
#include "../../../../game-data/ai.hpp"
#include "../../../../game-data/data.hpp"
#include "../../effects/runtime_effects_factory_v1.hpp"
#include "../../effects/celest_target_fx_dispatch_v1.hpp"
#include "../../skills_animation/skill_animation_program.hpp"
#include "../../generic_skills/runtime_skill_cast_coordinator_v1.hpp"
#include "../../../../level-world/character_animation_ai.hpp"
#include "../../../retained_animation_owner.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace {
using namespace dh::foundation;
using namespace dh::foundation::faery_menu;
using namespace dh::foundation::effects;
using Bytes = std::vector<std::uint8_t>;
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}
Bytes read(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("missing actual source input: " + path.string());
    return {std::istreambuf_iterator<char>(in), {}};
}
dh2::data::Bytes view(const Bytes& data) { return {data.data(), data.size()}; }

dh2::data::FaeryTables::Borrow load_faeries(const std::filesystem::path& root,
                                             dh2::data::FaeryTables& owner) {
    const auto records = read(root / "faeries_pyarray.bin");
    const auto names = read(root / "faeries_pyarraynames.bin");
    const auto schema = read(root / "faeries_pystructnames.bin");
    std::string error;
    check(owner.load(view(records), view(names), view(schema), error), error);
    return owner.borrow();
}
struct HottyFxCpuServicesV1 {
    const dh2::scene::Scene* expected_scene{};
    std::uint32_t next_texture{1};
    unsigned texture_uploads{}, texture_releases{}, submissions{};
    std::shared_ptr<const EffectRenderFrame> last_frame;

    static bool camera(void* raw, const dh2::scene::Scene& scene,
                       float view[16], float position[3], std::string& error) {
        auto& self = *static_cast<HottyFxCpuServicesV1*>(raw);
        if (&scene != self.expected_scene) {
            error = "Hotty FX CPU camera did not receive the retained same-session Scene";
            return false;
        }
        const float identity[16]{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
        std::copy_n(identity, 16, view);
        position[0] = position[1] = position[2] = 0;
        error.clear();
        return true;
    }
    static bool driver(void*, std::uint32_t& type, std::string& error) {
        type = 0;
        error.clear();
        return true;
    }
    bool upload(const TextureImage& image, std::uint32_t& texture, std::string& error) {
        if (!image.width || !image.height ||
            image.rgba.size() != std::size_t(image.width) * image.height * 4) {
            error = "Hotty FX CPU sink received invalid decoded texture pixels";
            return false;
        }
        texture = ++next_texture;
        ++texture_uploads;
        error.clear();
        return true;
    }
    void release(std::uint32_t) { ++texture_releases; }
    bool submit(std::shared_ptr<const EffectRenderFrame> frame, std::string& error) {
        last_frame = std::move(frame);
        ++submissions;
        error.clear();
        return true;
    }
};
}

int main(int argc, char** argv) {
    using namespace dh::foundation;
    try {
        const bool celest_fx_only = argc == 6 && std::string(argv[5]) == "--celest-fx-only";
        check(argc == 5 || celest_fx_only,
              "usage: hotty-session-cast <shared-assets> <faery-table-dir> <effects-table-dir> <private-fx-asset-root> [--celest-fx-only]");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);

        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan visual_plan;
        check(build_original_combat_visual_plan(assets, melee, "MagePlayerBase",
              customization, "hotty-connected-player", visual_plan, error), error);
        visual_plan.config.motion_node_id = "auto";
        visual_plan.config.consume_root_motion = true;

        const std::string animation_root = "original-cache/data/pydata/";
        const auto animation_rows = assets.read(animation_root + "animations_pyarray.bin");
        const auto animation_names = assets.read(animation_root + "animations_pyarraynames.bin");
        const auto animation_schema = assets.read(animation_root + "animations_pystructnames.bin");
        const auto animation_dictionary_values = assets.read(animation_root + "animations_dictionary_pyarray.bin");
        const auto animation_dictionary_names = AssetCatalog(
            std::filesystem::path(argv[1]).parent_path().parent_path()).read(
                ".local-inputs/actors/animations_dictionary_pyarraynames.bin");
        dh2::data::Dictionary animation_dictionary;
        dh2::data::AnimationTables animation_tables;
        check(dh2::data::load_dictionary(view(animation_dictionary_names),
              view(animation_dictionary_values), animation_dictionary, error), error);
        check(dh2::data::load_animation_tables(view(animation_rows), view(animation_names),
              view(animation_schema), animation_dictionary, animation_tables, error), error);
        OriginalActorProperties source_mage_properties;
        check(resolve_original_actor_properties(database.characters, database.classes,
              "MagePlayerBase", {256, false}, source_mage_properties, error), error);
        const auto spell_animation_table = dh2_character_animation_table_id(
            source_mage_properties.sheets.resolved[2],
            static_cast<std::int32_t>(animation_tables.characters.size()));
        check(spell_animation_table >= 0 &&
              static_cast<std::size_t>(spell_animation_table) < animation_tables.characters.size() &&
              animation_tables.characters[static_cast<std::size_t>(spell_animation_table)].fields.size() > 31,
              "Same Mage CharAnimTable lacks source bankSpells metadata");
        const auto& bank_spells = animation_tables.characters[
            static_cast<std::size_t>(spell_animation_table)].fields[31];
        check(bank_spells.size() > 4 && bank_spells[4] >= 0,
              "Source active bankSpells slot4 has no loadable Cast sequence");
        check(!bank_spells.empty() && bank_spells[0] >= 0,
              "Fresh Mage source bankSpells slot0 has no loadable Celest Cast sequence");
        const auto spell_sequence_id = bank_spells[4];
        const auto celest_sequence_id = bank_spells[0];
        skills_animation::SkillAnimationPrograms spell_bank;
        check(skills_animation::build_skill_animation_programs(assets, animation_tables,
              animation_dictionary, visual_plan.config, {celest_sequence_id, spell_sequence_id},
              "hotty-cast-player", spell_bank, error), error);
        check(spell_bank.plan.sequences.size() == 2 &&
              std::any_of(spell_bank.plan.sequences.begin(), spell_bank.plan.sequences.end(),
                  [&](const auto& sequence) { return sequence.id == spell_sequence_id; }) &&
              std::any_of(spell_bank.plan.sequences.begin(), spell_bank.plan.sequences.end(),
                  [&](const auto& sequence) { return sequence.id == celest_sequence_id; }),
              "Source bankSpells[0] and [4] did not load their exact authored Cast sequences");
        visual_plan.config = spell_bank.plan.config;
        CombatSessionConfig config;
        config.diagnosticRngSeed = 17;
        config.playerId = 1;
        config.playerProfileId = "MagePlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = visual_plan.config;
        CombatSessionProfile player_profile;
        player_profile.action = {"AttackStatic", 0, {0,1}};
        player_profile.initialIdle = {"Idle", 0, {0}};
        player_profile.damageMarkerNames = {"attack_mainhand"};
        player_profile.sourceAnimationClips = visual_plan.config.clips;
        OriginalAttackSelection player_sequence;
        player_sequence.state = "AttackStatic";
        player_sequence.variant = 0;
        player_sequence.group_path = {0};
        player_sequence.actor_rate = 1.0f;
        player_profile.sequenceAction = player_sequence;
        player_profile.retainedPhaseClock = true;
        player_profile.customization = customization;
        player_profile.propertyOptions = {256, true};
        config.profiles.emplace(config.playerProfileId, player_profile);
        ActorCustomization enemy_customization;
        enemy_customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan enemy_visual_plan;
        check(build_original_combat_visual_plan(assets, melee, "Swamp_LizadMan_Type1",
              enemy_customization, "hotty-connected-target", enemy_visual_plan, error), error);
        enemy_visual_plan.config.motion_node_id = "auto";
        enemy_visual_plan.config.consume_root_motion = true;
        CombatSessionProfile enemy_profile;
        enemy_profile.action = {"Attack", 0, {0,1}};
        enemy_profile.initialIdle = {"Idle", 0, {0}};
        enemy_profile.death = CombatSessionChoice{"Died", 0, {0}};
        enemy_profile.damageMarkerNames = {"attack_mainhand"};
        enemy_profile.customization = enemy_customization;
        enemy_profile.propertyOptions = {std::nullopt, true};
        config.profiles.emplace("Swamp_LizadMan_Type1", enemy_profile);
        ActorPopulation population;
        PopulationActor placed;
        placed.profileId = "Swamp_LizadMan_Type1";
        placed.definition.stableId = 2;
        placed.definition.sourceId = "hotty-connected-target";
        placed.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        placed.transform = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        population.actors().push_back(std::move(placed));
        CharacterVisual player_visual;
        auto session_owner = std::make_unique<CombatSession>();
        CombatSession& session = *session_owner;
        check(session.initialize(assets, database, melee, config, player_visual,
              population, {0,0,0}, customization, error), error);
        check(session.player_id() == 1 && session.actor(2) && session.world(),
              "connected Hotty fixture did not create the same live player/target world");
        const auto* initial_cast_visual = session.retained_actor_visual_borrow(1);
        check(initial_cast_visual, "same-session Cast source visual unavailable");
        bool source_clip_has_do_spell = false;
        std::vector<std::size_t> source_do_spell_phase_path;
        bool celest_clip_has_do_spell = false;
        std::vector<std::size_t> celest_do_spell_phase_path;
        for (const auto& sequence : spell_bank.plan.sequences) {
            auto& found = sequence.id == celest_sequence_id
                ? celest_clip_has_do_spell : source_clip_has_do_spell;
            auto& event_path = sequence.id == celest_sequence_id
                ? celest_do_spell_phase_path : source_do_spell_phase_path;
            for (const auto& phase : sequence.phases) {
                const auto* markers = initial_cast_visual->markers(phase.clipName, error);
                check(markers, "active bankSpells[4] BDAE marker track missing: " + error);
                for (const auto& marker : markers->markers()) {
                    if (marker.name == "do_spell") {
                        found = true;
                        event_path = phase.sourcePath;
                    }
                }
            }
        }
        check(source_clip_has_do_spell && !source_do_spell_phase_path.empty(),
              "Actual Hotty bankSpells[4] clip set has no retained do_spell marker");
        check(celest_clip_has_do_spell && !celest_do_spell_phase_path.empty(),
              "Fresh Mage bankSpells[0] Celest clip set has no retained do_spell marker");

        CharacterState character;
        character.id = "hotty-connected-player";
        character.name = "Knight";
        character.class_id = "mage";
        character.source_faery_state_known = true;
        character.source_faery_list_id = 2;
        character.faery_by_difficulty[0].current_faery = 4;
        character.faery_by_difficulty[0].faeries[4].state = 1;
        character.faery_by_difficulty[0].faeries[4].level = 1;
        session.actor(1)->persistent_character_id = character.id;
        const auto* player_traits = session.world()->traits(1);
        auto* player_properties = session.world()->combat_properties(1);
        check(player_traits && player_properties, "same-session player properties/traits unavailable");
        const auto full_player_properties = *player_properties;
        character.stats.resource = player_properties->sheets.resolved[41] / 256.0f;
        character.stats.max_resource = session.actor(1)->max_resource;
        session.actor(1)->resource = character.stats.resource;

        dh2::data::FaeryTables faery_owner;
        const auto faery_tables = load_faeries(std::filesystem::path(argv[2]), faery_owner);
        check(faery_tables && faery_tables.lists().at(2) == std::vector<std::int32_t>({1,13,14,15,7}) &&
              faery_tables.faery_names().at(7) == "Hotty" &&
              faery_tables.faeries().at(7).script == "faerie_hotty",
              "connected Hotty cast did not borrow actual FaeryList/Hotty source rows");

        // The menu click uses the same profile/table/Session and a bank built
        // before Session initialization. It may not fall back to writing the
        // CharacterState slot when source continuations are missing.
        generic_skills::RuntimeSkillAnimationBankV1 menu_spell_bank;
        menu_spell_bank.character_state_id = character.id;
        menu_spell_bank.class_id = character.class_id;
        menu_spell_bank.source_animation_table_id = spell_animation_table;
        menu_spell_bank.faery_cast_slots[0] = {spell_animation_table, 0,
            celest_sequence_id, skills_animation::skill_sequence_state(celest_sequence_id), true, {}};
        menu_spell_bank.faery_cast_slots[4] = {spell_animation_table, 4,
            spell_sequence_id, skills_animation::skill_sequence_state(spell_sequence_id), true, {}};
        auto menu_owner = std::make_shared<int>(1);
        CharacterStateFaeryBindingsV1 page_bindings;
        page_bindings.owner = menu_owner;
        page_bindings.character = &character;
        page_bindings.tables = faery_tables;
        page_bindings.difficulty = 0;
        page_bindings.validate_same_character = [&character](const CharacterState* actual, std::string&) {
            return actual == &character;
        };
        page_bindings.localize = [](const std::string& symbol, std::string& value, std::string&) {
            value = symbol;
            return true;
        };
        std::vector<std::string> menu_order;
        SessionFaeryActivationV1 activation;
        activation.owner = menu_owner;
        activation.character = &character;
        activation.session = &session;
        activation.tables = faery_tables;
        activation.animation_bank = &menu_spell_bank;
        activation.difficulty = 0;
        activation.profile_faery_list_id = 2;
        activation.validate_same_session = [](CombatSession& current, const CharacterState&, std::string&) {
            return current.world() && current.player_id() == 1;
        };
        activation.change_current = [&menu_order](CombatSession&, CharacterState& same, std::uint32_t slot, std::string&) {
            menu_order.push_back("ChangeFaery");
            same.faery_by_difficulty[0].current_faery = static_cast<std::int32_t>(slot);
            return true;
        };
        activation.update_all_skills = [&menu_order](CombatSession&, CharacterState&, std::string&) {
            menu_order.push_back("UpdateAllSkills");
            return true;
        };
        activation.place_selected_visual = [&menu_order](CombatSession& current, CharacterState&, std::string&) {
            menu_order.push_back("VisualPlacement");
            return current.retained_actor_visual_borrow(1) != nullptr;
        };
        auto incomplete_activation = activation;
        incomplete_activation.place_selected_visual = {};
        character_menu::SourcePageProviderV1 rejected_provider;
        check(!bind_session_faery_page_provider_v1(
                  page_bindings, incomplete_activation, rejected_provider, error) &&
              character.faery_by_difficulty[0].current_faery == 4 &&
              error.find("visual placement") != std::string::npos,
              "missing real visual placement continuation must reject before changing the selected slot");
        incomplete_activation = activation;
        incomplete_activation.update_all_skills = {};
        check(!bind_session_faery_page_provider_v1(
                  page_bindings, incomplete_activation, rejected_provider, error) &&
              character.faery_by_difficulty[0].current_faery == 4 &&
              error.find("UpdateAllSkills") != std::string::npos,
              "missing UpdateAllSkills continuation must reject before changing the selected slot");
        auto incomplete_bank = menu_spell_bank;
        incomplete_bank.faery_cast_slots[0].loaded = false;
        auto bank_activation = activation;
        bank_activation.animation_bank = &incomplete_bank;
        check(bind_session_faery_page_provider_v1(
                  page_bindings, bank_activation, rejected_provider, error), error);
        check(!rejected_provider.release(80.f, 80.f, error) &&
              character.faery_by_difficulty[0].current_faery == 4 && menu_order.empty(),
              "missing target Cast bank root must reject the click before ChangeFaery or its continuations");
        character_menu::SourcePageProviderV1 session_provider;
        check(bind_session_faery_page_provider_v1(
                  page_bindings, activation, session_provider, error), error);
        check(slot_at(80.f, 80.f) == 0 &&
              session_provider.release(80.f, 80.f, error) &&
              character.faery_by_difficulty[0].current_faery == 0 &&
              menu_order == std::vector<std::string>{"ChangeFaery", "UpdateAllSkills", "VisualPlacement"},
              "same-Session Faery release must use the preloaded Celest bank and source continuation order");
        check(session_provider.release(80.f, 80.f, error) &&
              menu_order.size() == 6 && menu_order[3] == "ChangeFaery" &&
              menu_order[4] == "UpdateAllSkills" && menu_order[5] == "VisualPlacement",
              "a second authored click must repeat the source action once in source continuation order");
        const auto changed_twice = menu_order.size();
        check(session_provider.release(0.f, 0.f, error) && menu_order.size() == changed_twice,
              "outside the authored Faery contour must be consumed without invoking source continuations");
        character.faery_by_difficulty[0].current_faery = 4;
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(database.characters, rules, error), error);

        InputActions input;
        check(session.update(.016, input, {0,0,0}, 0, error), error);
        HottyCooldownClockV1 clock;
        check(advance_hotty_cooldown_clock_v1(session, .016, clock, error), error);
        HottySourcePolicyV1 policy;
        policy.complete = true;
        const auto* enemy_ai = dh2::data::ai_props(
            session.world()->factions(), session.world()->combat_properties(2)->sheets.resolved[1]);
        check(enemy_ai, "same-session Hotty target lacks the original AI range row");
        const float source_range_offset = enemy_ai->interact_radius +
            session.world()->target_radius(*session.actor(1));
        session.actor(2)->transform.position = {602.0f + source_range_offset, 0.0f, 0.0f};
        HottyAuthoredActorOrderV1 actor_order{true, {1,2}};
        const std::vector<HottySourceActorFactsV1> source_facts{{2, true, false, {}, {}, true}};
        HottySourceTargetListV1 at_six_hundred, at_eight_hundred;
        check(query_hotty_character_targets_v1(*session.world(), 1, actor_order,
              source_facts, 600, at_six_hundred, error), error);
        session.actor(2)->transform.position = {798.0f + source_range_offset, 0.0f, 0.0f};
        check(query_hotty_character_targets_v1(*session.world(), 1, actor_order,
              source_facts, 800, at_eight_hundred, error), error);
        check(at_six_hundred.character_targets_in_source_order.empty() &&
              at_eight_hundred.character_targets_in_source_order == std::vector<ActorId>{2},
              "same-session Hotty effective 600/800 query boundary/order differs");
        const auto mana_before_unknown_order = player_properties->sheets.resolved[41];
        HottyPreparedCastV1 rejected_unknown_order;
        check(!prepare_hotty_spell_v1(session, character, 0, faery_tables,
              database.classes, rules, policy, clock, rejected_unknown_order, error) &&
              error.find("complete source target list") != std::string::npos &&
              session.world()->combat_properties(1)->sheets.resolved[41] == mana_before_unknown_order &&
              clock.spell_ready_at_ms.empty(),
              "unknown production Hotty order mutated same-session MP/cooldown");
        auto low_mana = *player_properties;
        low_mana.sheets.resolved[41] = 20 * 256 - 1;
        check(session.world()->update_combat_properties(1, low_mana, *player_traits, error), error);
        session.actor(1)->resource = float(low_mana.sheets.resolved[41]) / 256.0f;
        character.stats.resource = session.actor(1)->resource;
        HottyPreparedCastV1 rejected_mana;
        check(!prepare_hotty_spell_v1(session, character, 0, faery_tables,
              database.classes, rules, policy, clock, rejected_mana, error,
              &at_eight_hundred) &&
              error.find("insufficient") != std::string::npos && clock.spell_ready_at_ms.empty(),
              "same-session Hotty HasMana failure was not rejected before UseMana/cooldown");
        check(session.world()->update_combat_properties(1, full_player_properties, *player_traits, error), error);
        session.actor(1)->resource = float(full_player_properties.sheets.resolved[41]) / 256.0f;
        character.stats.resource = session.actor(1)->resource;

        const auto effect_root = std::filesystem::path(argv[3]);
        const auto effect_records = read(effect_root / "effects_pyarray.bin");
        const auto effect_names = read(effect_root / "effects_pyarraynames.bin");
        const auto effect_schema = read(effect_root / "effects_pystructnames.bin");
        const auto dictionary_names = read(effect_root / "effects_dictionary_pyarraynames.bin");
        const auto dictionary_records = read(effect_root / "effects_dictionary_pyarray.bin");
        dh2::data::EffectsTables effect_owner;
        check(effect_owner.load(view(effect_records), view(effect_names), view(effect_schema),
              view(dictionary_names), view(dictionary_records), error), error);
        const auto effect_tables = effect_owner.borrow();
        HottyEffectIdsV1 effect_ids;
        check(resolve_hotty_effect_ids_v1(effect_tables, effect_ids, error), error);

        const auto* retained_player = session.actor(1);
        const auto* retained_visual = session.retained_actor_visual_borrow(1);
        check(retained_player && retained_player->persistent_character_id == character.id &&
              retained_visual && retained_visual->retained_scene_borrow(),
              "Hotty FX factory fixture lacks the actual persistent CharacterState/Scene owner");
        AssetCatalog fx_assets(argv[4]);
        const auto spell_resource = fx_assets.read("data/3D/interface/spell_dh2_faery_fire.bdae");
        const auto player_pre_resource = fx_assets.read("data/3D/interface/skill_dh2_faery_fire.bdae");
        check(spell_resource.size() == 14788 && player_pre_resource.size() == 33092,
              "Private overlay did not stage the exact original Hotty BDAE resources");
        dh2::navigation::CollisionWorld same_pf_world{};
        HottyFxCpuServicesV1 cpu_fx;
        cpu_fx.expected_scene = retained_visual->retained_scene_borrow();
        RuntimeEffectsFactoryBindingsV1 factory_bindings;
        factory_bindings.assets = &fx_assets;
        factory_bindings.tables = effect_tables;
        factory_bindings.same_pf_world = &same_pf_world;
        factory_bindings.scene_view = {&cpu_fx, HottyFxCpuServicesV1::camera,
                                       HottyFxCpuServicesV1::driver};
        factory_bindings.textures = {
            [&cpu_fx](const TextureImage& image, std::uint32_t& texture, std::string& why) {
                return cpu_fx.upload(image, texture, why);
            },
            [&cpu_fx](std::uint32_t texture) { cpu_fx.release(texture); }};
        factory_bindings.submit = [&cpu_fx](std::shared_ptr<const EffectRenderFrame> frame,
                                            std::string& why) {
            return cpu_fx.submit(std::move(frame), why);
        };
        auto fx_factory = RuntimeEffectsFactoryV1::create(
            session, session.player_id(), std::move(factory_bindings), error);
        check(fx_factory != nullptr, "same-session Hotty FX factory creation: " + error);
        check(fx_factory->is_bound_to(session),
              "Hotty FX factory rejected the exact CombatSession holding the persistent CharacterState");
        CombatSession foreign_session;
        check(!fx_factory->is_bound_to(foreign_session),
              "Hotty FX factory accepted a foreign session with a coincident player ActorId");
        HottyRuntimeEffectsDispatchV1 hotty_fx_dispatch(*fx_factory, effect_tables, effect_ids);
        CelestTargetFxIdsV1 celest_fx_ids;
        check(resolve_celest_target_fx_ids_v1(effect_tables, celest_fx_ids, error), error);
        check(effect_tables.set_names().at(static_cast<std::size_t>(celest_fx_ids.target_main)) ==
                  "Celest_Level_1_Main" &&
              effect_tables.set_names().at(static_cast<std::size_t>(celest_fx_ids.player_pre)) ==
                  "Celest_Level_1_Player",
              "Celest dispatch did not resolve exact original AnimatedEffectTable names");
        dh::foundation::effects::CelestTargetFxDispatchV1 celest_fx_dispatch(
            *fx_factory, effect_tables, celest_fx_ids);

        if (celest_fx_only) {
            // Exercise the source Celest prepare/Use sequence on this live
            // fixture before unrelated Hotty coordinator checks. The helper
            // applies both source results before invoking the Main sink.
            character.faery_by_difficulty[0] = {};
            character.faery_by_difficulty[0].current_faery = 0;
            character.faery_by_difficulty[0].faeries[0].state = 1;
            character.faery_by_difficulty[0].faeries[0].level = 0;
            const auto* target_ai = dh2::data::ai_props(
                session.world()->factions(), session.world()->combat_properties(2)->sheets.resolved[1]);
            check(target_ai, "Celest-only target lacks source AI interaction radius");
            const float target_offset = target_ai->interact_radius +
                session.world()->target_radius(*session.actor(1));
            session.actor(2)->transform.position = {500.0f + target_offset, 0.0f, 0.0f};
            HottySourceTargetListV1 celest_targets;
            check(query_hotty_character_targets_v1(*session.world(), 1, actor_order,
                  source_facts, 600, celest_targets, error), error);
            check(celest_targets.character_targets_in_source_order == std::vector<ActorId>{2},
                  "Celest-only source target query did not retain the actual target");
            CelestPreparedCastV1 prepared;
            check(prepare_celest_spell_v1(session, character, 0, faery_tables,
                  database.classes, rules, policy, clock, prepared, error, &celest_targets), error);
            check(prepared.status == CelestPrepareStatusV1::prepared_pending_spell_combat &&
                  prepared.character_targets == std::vector<ActorId>{2},
                  "Celest-only source preparation did not retain the target");
            CelestEffectReceiptV1 pre_effect;
            check(dispatch_celest_player_pre_best_effort_v1(session, prepared,
                  character, clock, &celest_fx_dispatch, pre_effect, error), error);
            check(pre_effect.status == CelestEffectStatusV1::source_instance_created &&
                  fx_factory->is_bound_to(session),
                  "Celest Player source instance did not use the same live Session factory");
            CelestSourceUseV1 use;
            check(apply_celest_source_use_v1(session, prepared, celest_targets,
                  character, clock, faery_tables, database.classes, &celest_fx_dispatch,
                  use, error), error);
            check(use.source_use_complete && use.rolls.size() == 2 &&
                  use.rolls[0].applied && use.rolls[1].applied &&
                  use.target_effects.size() == 1 &&
                  use.target_effects.front().target == 2 &&
                  use.target_effects.front().status == CelestEffectStatusV1::source_instance_created,
                  "Celest Main dispatch did not follow both actual source result occurrences");
            std::cout << "{\"validation\":\"PASS\",\"source_set_ids\":["
                      << celest_fx_ids.target_main << ',' << celest_fx_ids.player_pre
                      << "],\"source_names\":[\"Celest_Level_1_Main\",\"Celest_Level_1_Player\"]"
                      << ",\"same_session_factory\":true,\"player_pre_instance_created\":true"
                      << ",\"target_results_applied_before_main\":2,\"target_main_instance_created\":true"
                      << ",\"live_packets_or_gpu_claimed\":false}\n";
            return 0;
        }

        dh::foundation::generic_skills::RuntimeSkillCastCoordinatorV1 coordinator;
        dh::foundation::generic_skills::RuntimeSkillFaerySpellArmV1 faery_arm;
        faery_arm.tables = &faery_tables;
        faery_arm.difficulty = 0;
        faery_arm.policy = &policy;
        faery_arm.actor_order = &actor_order;
        faery_arm.source_facts = &source_facts;
        faery_arm.targets = &at_eight_hundred;
        faery_arm.cooldown_clock = &clock;
        faery_arm.effect_dispatch = &hotty_fx_dispatch;
        dh::foundation::generic_skills::RuntimeSkillCastRequestV1 cast_request;
        cast_request.dispatch = dh::foundation::generic_skills::RuntimeSkillCastDispatchV1::native_hud_spell;
        cast_request.actor = session.player_id();
        cast_request.character = &character;
        cast_request.classes = &database.classes;
        cast_request.property_rules = &rules;
        cast_request.active_faery_spell = &faery_arm;
        // Keep the real cast-admission gate in this fixture: the source
        // bankSpells rows above already produced this exact runtime plan and
        // its per-sequence policies for the retained Mage visual.
        cast_request.visual_plan = &spell_bank.plan;
        cast_request.sequence_policies = &spell_bank.policies;
        cast_request.selection.state = skills_animation::skill_sequence_state(spell_sequence_id);
        cast_request.selection.variant = 0;
        // P15 HOTTY: whole state-7 root (pre clip, then do_spell clip), as main.cpp issues it. A do_spell-leaf scope hid the pre-to-Use chain.
        cast_request.selection.group_path = {};
        cast_request.selection.actor_rate = 1.0f;
        dh::foundation::generic_skills::RuntimeSkillCastReceiptV1 cast_receipt;
        const auto hp_before_cast = session.actor(2)->health;
        const auto rng_before_cast = session.world()->random_state();
        session.set_frame_begin_provider([&](CombatSession& frame_session, double dt,
                                              std::string& frame_error) {
            if (!advance_hotty_cooldown_clock_v1(frame_session, dt, clock, frame_error))
                return false;
            return coordinator.advance_after_session_update(frame_session, dt, frame_error);
        });

        std::uint32_t retained_spell_events = 0;
        std::uint32_t retained_completion_events = 0;
        std::vector<std::string> retained_event_names;
        RetainedAnimationEvent actual_do_spell;
        const double frame_dt = 0.016;
        const auto advance_hotty_clock = [&]() {
            if (clock.session_update_serial == session.update_serial()) return true;
            return advance_hotty_cooldown_clock_v1(session, frame_dt, clock, error);
        };
        CombatSessionStateAnimationServices animation_services;
        animation_services.event = [&](ActorId actor, const RetainedAnimationEvent& event,
                                       std::string& callback_error) {
            if (actor != session.player_id()) {
                callback_error = "Hotty Cast source event reached a foreign actor";
                return false;
            }
            retained_event_names.push_back(event.name);
            if (event.name != "do_spell") {
                callback_error.clear();
                return true;
            }
            ++retained_spell_events;
            actual_do_spell = event;
            callback_error.clear();
            return true;
        };
        animation_services.finished = [&](ActorId actor, std::string& callback_error) {
            ++retained_completion_events;
            if (actor != session.player_id()) {
                callback_error = "Hotty Cast completion reached a foreign actor";
                return false;
            }
            callback_error.clear();
            return true;
        };
        cast_request.downstream_animation_services = animation_services;
        check(coordinator.begin_skill_cast_v1(cast_request, session, cast_receipt, error), error);
        check(cast_receipt.phase == dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::prepared_pending_use &&
              cast_receipt.native_hud_spell && cast_receipt.faery_slot == 4 &&
              cast_receipt.faery_record_id == 7 &&
              cast_receipt.target_order == std::vector<ActorId>{2} &&
              cast_receipt.spell_type == static_cast<std::int32_t>(faery_tables.faeries()[7].scalar.words[7]) &&
              session.actor(1)->resource == character.stats.resource &&
              session.world()->combat_properties(1)->sheets.resolved[41] ==
                  full_player_properties.sheets.resolved[41] - 20 * 256 &&
              fx_factory->manager().cold_creations() >= 1,
              "NativeHUDSpell begin did not retain actual key4/Hotty row, debit 20 MP, dispatch Player_Pre and start bankSpells[4]");
        for (std::uint64_t frame = 1; frame <= 900 && !retained_completion_events; ++frame) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
        }
        if (retained_spell_events != 1 || retained_completion_events != 1) {
            std::cerr << "Cast bankSpells[4] sequence=" << spell_sequence_id << " retained events:";
            for (const auto& name : retained_event_names) std::cerr << ' ' << name;
            std::cerr << '\n';
        }
        check(retained_spell_events == 1 && retained_completion_events == 1 &&
              actual_do_spell.name == "do_spell" && actual_do_spell.generation != 0,
              "Active bankSpells[4] Cast clip did not deliver one actual retained do_spell event and finish");
        const auto* completed_cast = coordinator.receipt(session.player_id());
        check(completed_cast && completed_cast->phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::completed &&
              completed_cast->applied_results.size() == 1 &&
              completed_cast->applied_results[0].applied &&
              completed_cast->applied_results[0].source_mask == 0x1005554Au &&
              completed_cast->applied_results[0].source_outcomes.has_value() &&
              completed_cast->applied_results[0].health_removed > 0.0f &&
              session.actor(2)->health < hp_before_cast &&
              session.world()->random_state().calls > rng_before_cast.calls &&
              fx_factory->manager().cold_creations() >= 2,
              "Retained key4 do_spell did not deliver the same-world positive SpellCombatRoll/target FX prefix");
        const auto final_health = session.actor(2)->health;
        const auto final_rng = session.world()->random_state();
        dh::foundation::generic_skills::RuntimeSkillCastReceiptV1 duplicate_use;
        check(coordinator.apply_retained_use_event_v1(session, session.player_id(),
              cast_receipt.generation,
              dh::foundation::generic_skills::RuntimeSkillSourceAnimStateV1::cast,
              actual_do_spell, duplicate_use, error), error);
        check(session.actor(2)->health == final_health &&
              session.world()->random_state().seed == final_rng.seed &&
              session.world()->random_state().calls == final_rng.calls,
              "Duplicate retained Hotty do_spell callback mutated same-world HP/RNG");

        // A second cast with the same live Session/Character owner but no FX
        // sink proves presentation availability is not a combat admission gate.
        for (std::uint32_t i = 0; i < 320; ++i) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        dh::foundation::generic_skills::RuntimeSkillCastCoordinatorV1 no_fx_coordinator;
        dh::foundation::generic_skills::RuntimeSkillFaerySpellArmV1 no_fx_arm = faery_arm;
        no_fx_arm.effect_dispatch = nullptr;
        auto no_fx_request = cast_request;
        no_fx_request.active_faery_spell = &no_fx_arm;
        dh::foundation::generic_skills::RuntimeSkillCastReceiptV1 no_fx_begin;
        std::uint32_t no_fx_use_events = 0;
        std::uint32_t no_fx_finished_events = 0;
        CombatSessionStateAnimationServices no_fx_services;
        no_fx_services.event = [&](ActorId actor, const RetainedAnimationEvent& event,
                                   std::string& callback_error) {
            if (actor != session.player_id()) return false;
            if (event.name != "do_spell") { callback_error.clear(); return true; }
            ++no_fx_use_events;
            callback_error.clear();
            return true;
        };
        no_fx_services.finished = [&](ActorId actor, std::string& callback_error) {
            ++no_fx_finished_events;
            if (actor != session.player_id()) return false;
            callback_error.clear();
            return true;
        };
        no_fx_request.downstream_animation_services = no_fx_services;
        check(no_fx_coordinator.begin_skill_cast_v1(no_fx_request, session, no_fx_begin, error), error);
        check(no_fx_begin.phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::prepared_pending_use &&
              no_fx_begin.detail.find("Player_Pre FX diagnostic: dispatcher unavailable") != std::string::npos,
              "Missing Hotty FX sink vetoed OnPre or failed to retain its diagnostic");
        for (std::uint64_t frame = 1; frame <= 900 && !no_fx_finished_events; ++frame) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        const auto* no_fx_completed = no_fx_coordinator.receipt(session.player_id());
        if (!no_fx_completed || no_fx_completed->phase !=
                dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::completed ||
            no_fx_completed->detail.find("FX diagnostic: dispatcher unavailable") == std::string::npos) {
            std::cerr << "No-FX second cast events=" << no_fx_use_events << "/" << no_fx_finished_events
                      << " phase=" << (no_fx_completed ? int(no_fx_completed->phase) : -1)
                      << " results=" << (no_fx_completed ? no_fx_completed->applied_results.size() : 0)
                      << " detail=" << (no_fx_completed ? no_fx_completed->detail : "<none>") << '\n';
        }
        check(no_fx_use_events == 1 && no_fx_finished_events == 1 && no_fx_completed &&
              no_fx_completed->phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::completed &&
              !no_fx_completed->applied_results.empty() &&
              no_fx_completed->applied_results.front().applied &&
              no_fx_completed->detail.find("FX diagnostic: dispatcher unavailable") !=
                  std::string::npos &&
              session.world()->random_state().calls > final_rng.calls,
              "Missing target FX sink vetoed actual do_spell hit/completion or lost its diagnostic");
        check(effect_tables.set_names().at(static_cast<std::size_t>(effect_ids.character_target)) ==
                  "Hotty_Level_1_Target" &&
              !effect_tables.sets().at(static_cast<std::size_t>(effect_ids.character_target)).steps.empty() &&
              completed_cast->applied_results[0].health_removed > 0.0f,
              "Positive source SpellCombatRoll did not select the authored Hotty target-FX row");

        // Reuse this exact source-created Mage Character, live Session player,
        // property table, and ActorPopulation to exercise the ordinary fresh
        // slot-0 Celest row. The hotbar remains a different slot-4 Hotty cast.
        const auto mage_list = std::find(faery_tables.list_names().begin(),
                                         faery_tables.list_names().end(), "Mage");
        check(mage_list != faery_tables.list_names().end(),
              "Actual FaeryList table has no Mage source-created profile row");
        const auto mage_list_id = static_cast<std::int32_t>(
            mage_list - faery_tables.list_names().begin());
        check(faery_tables.lists().at(static_cast<std::size_t>(mage_list_id)) ==
                  std::vector<std::int32_t>({1,13,14,15,7}) &&
              faery_tables.faery_names().at(1) == "Celest" &&
              faery_tables.faeries().at(1).script == "faerie_celest",
              "Actual Mage list slot0 no longer resolves the source Celest row/script");
        character.source_faery_list_id = mage_list_id;
        character.faery_by_difficulty[0].current_faery = 0;
        character.faery_by_difficulty[0].faeries = {};
        check(session.world()->update_combat_properties(1, full_player_properties,
              *player_traits, error), error);
        session.actor(1)->resource = float(full_player_properties.sheets.resolved[41]) / 256.0f;
        character.stats.resource = session.actor(1)->resource;
        clock.spell_ready_at_ms.erase(1);
        auto* live_celest_target = session.actor(2);
        const auto* celest_target_ai = dh2::data::ai_props(
            session.world()->factions(),
            session.world()->combat_properties(2)->sheets.resolved[1]);
        check(celest_target_ai, "Celest target lacks the original AI interaction radius");
        const float celest_offset = celest_target_ai->interact_radius +
            session.world()->target_radius(*session.actor(1));
        live_celest_target->transform.position = {500.0f + celest_offset, 0.0f, 0.0f};
        HottySourceTargetListV1 celest_targets;
        check(query_hotty_character_targets_v1(*session.world(), 1, actor_order,
              source_facts, 600, celest_targets, error), error);
        check(celest_targets.character_targets_in_source_order == std::vector<ActorId>{2},
              "Fresh Celest current-Character NoSort query did not retain the actual target");

        auto celest_low_mana = *player_properties;
        celest_low_mana.sheets.resolved[41] = 10 * 256 - 1;
        check(session.world()->update_combat_properties(1, celest_low_mana,
              *player_traits, error), error);
        session.actor(1)->resource = float(celest_low_mana.sheets.resolved[41]) / 256.0f;
        character.stats.resource = session.actor(1)->resource;
        const auto celest_low_mana_rng = session.world()->random_state();
        CelestPreparedCastV1 rejected_celest_mana;
        check(!prepare_celest_spell_v1(session, character, 0, faery_tables,
              database.classes, rules, policy, clock, rejected_celest_mana,
              error, &celest_targets) && error.find("insufficient") != std::string::npos &&
              session.world()->combat_properties(1)->sheets.resolved[41] == 10 * 256 - 1 &&
              session.actor(1)->resource == character.stats.resource &&
              clock.spell_ready_at_ms.empty() &&
              session.world()->random_state().calls == celest_low_mana_rng.calls,
              "Celest insufficient-mana rejection mutated MP/resource/cooldown/RNG");
        check(session.world()->update_combat_properties(1, full_player_properties,
              *player_traits, error), error);
        session.actor(1)->resource = float(full_player_properties.sheets.resolved[41]) / 256.0f;
        character.stats.resource = session.actor(1)->resource;

        CelestPreparedCastV1 celest_prepared;
        check(prepare_celest_spell_v1(session, character, 0, faery_tables,
              database.classes, rules, policy, clock, celest_prepared, error,
              &celest_targets), error);
        check(celest_prepared.status == CelestPrepareStatusV1::prepared_pending_spell_combat &&
              celest_prepared.faery_slot == 0 && celest_prepared.faery_record_id == 1 &&
              celest_prepared.skill_level == 0 && celest_prepared.fixed_mana_cost == 10 * 256 &&
              celest_prepared.character_targets == std::vector<ActorId>{2} &&
              celest_prepared.mana_debited &&
              session.world()->combat_properties(1)->sheets.resolved[41] ==
                  full_player_properties.sheets.resolved[41] - 10 * 256 &&
              session.actor(1)->resource == character.stats.resource,
              "Fresh Celest OnPre did not use same CharacterState/list, debit source 10 MP or retain its target");
        CelestEffectReceiptV1 celest_player_effect;
        check(dispatch_celest_player_pre_best_effort_v1(session, celest_prepared,
              character, clock, &celest_fx_dispatch, celest_player_effect, error), error);
        check(celest_player_effect.status == CelestEffectStatusV1::source_instance_created &&
              fx_factory->is_bound_to(session),
              "Celest OnPre did not create the exact source Player effect through the same Session factory: " +
                  celest_player_effect.diagnostic);
        const auto celest_mp_after_pre = session.world()->combat_properties(1)->sheets.resolved[41];
        const auto celest_rng_before = session.world()->random_state();
        const auto celest_hp_before = live_celest_target->health;
        CelestPreparedCastV1 rejected_cooldown;
        check(!prepare_celest_spell_v1(session, character, 0, faery_tables,
              database.classes, rules, policy, clock, rejected_cooldown, error,
              &celest_targets) && error.find("HasSpellCooldown") != std::string::npos &&
              session.world()->combat_properties(1)->sheets.resolved[41] == celest_mp_after_pre &&
              session.world()->random_state().calls == celest_rng_before.calls,
              "Celest source cooldown rejection mutated MP or same-world RNG");
        CelestSourceUseV1 celest_use;
        check(apply_celest_source_use_v1(session, celest_prepared, celest_targets,
              character, clock, faery_tables, database.classes, &celest_fx_dispatch,
              celest_use, error), error);
        check(celest_use.source_use_complete && celest_use.source_hits_applied &&
              celest_use.rolls.size() == 2 && celest_use.rolls[0].applied &&
              celest_use.rolls[1].applied &&
              celest_use.rolls[0].source_mask == 0x1005554Au &&
              celest_use.rolls[1].source_mask == 0x1005554Au &&
              session.world()->random_state().calls > celest_rng_before.calls &&
              live_celest_target->health < celest_hp_before &&
              celest_use.target_effects.size() == 1 &&
              celest_use.target_effects.front().status == CelestEffectStatusV1::source_instance_created &&
              celest_use.target_effects.front().target == celest_targets.character_targets_in_source_order.front(),
              "Celest did not apply both actual same-Session rolls then create the exact target Main source effect");
        const auto celest_hp_after = live_celest_target->health;
        const auto celest_rng_after = session.world()->random_state();
        CelestSourceUseV1 duplicate_celest_use;
        check(!apply_celest_source_use_v1(session, celest_prepared, celest_targets,
              character, clock, faery_tables, database.classes, nullptr,
              duplicate_celest_use, error) &&
              live_celest_target->health == celest_hp_after &&
              session.world()->random_state().seed == celest_rng_after.seed &&
              session.world()->random_state().calls == celest_rng_after.calls,
              "Duplicate Celest Use occurrence mutated HP or same-world RNG");

        for (std::uint32_t i = 0; i < 320; ++i) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        dh::foundation::generic_skills::RuntimeSkillCastCoordinatorV1 celest_coordinator;
        dh::foundation::generic_skills::RuntimeSkillFaerySpellArmV1 celest_arm = faery_arm;
        celest_arm.targets = &celest_targets;
        celest_arm.effect_dispatch = nullptr;
        celest_arm.celest_effect_dispatch = nullptr;
        auto celest_cast_request = cast_request;
        celest_cast_request.active_faery_spell = &celest_arm;
        celest_cast_request.selection.state = skills_animation::skill_sequence_state(celest_sequence_id);
        celest_cast_request.selection.group_path = {};
        const auto coordinator_hp_before = live_celest_target->health;
        const auto coordinator_rng_before = session.world()->random_state();
        dh::foundation::generic_skills::RuntimeSkillCastReceiptV1 celest_begin;
        std::uint32_t coordinator_spell_events = 0;
        std::uint32_t coordinator_finished_events = 0;
        RetainedAnimationEvent celest_do_spell;
        CombatSessionStateAnimationServices celest_animation_services;
        celest_animation_services.event = [&](ActorId actor,
                const RetainedAnimationEvent& event, std::string& callback_error) {
            if (actor != session.player_id()) {
                callback_error = "Celest state7 event reached a foreign actor";
                return false;
            }
            if (event.name != "do_spell") { callback_error.clear(); return true; }
            ++coordinator_spell_events;
            celest_do_spell = event;
            callback_error.clear();
            return true;
        };
        celest_animation_services.finished = [&](ActorId actor, std::string& callback_error) {
            ++coordinator_finished_events;
            if (actor != session.player_id()) return false;
            callback_error.clear();
            return true;
        };
        celest_cast_request.downstream_animation_services = celest_animation_services;
        check(celest_coordinator.begin_skill_cast_v1(celest_cast_request, session,
              celest_begin, error), error);
        check(celest_begin.phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::prepared_pending_use &&
              celest_begin.native_hud_spell && celest_begin.faery_slot == 0 &&
              celest_begin.faery_record_id == 1 && celest_begin.source_script == "faerie_celest" &&
              celest_begin.target_order == std::vector<ActorId>{2} &&
              session.world()->combat_properties(1)->sheets.resolved[41] ==
                  celest_mp_after_pre - 10 * 256 &&
              celest_begin.detail.find("FX diagnostic: Celest_Level_1_Player") != std::string::npos,
              "Fresh Celest coordinator did not retain slot0, commit its source prefix or record optional FX absence");
        for (std::uint64_t frame = 1; frame <= 900 && !coordinator_finished_events; ++frame) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        const auto* celest_completed = celest_coordinator.receipt(session.player_id());
        if (!(coordinator_spell_events == 1 && coordinator_finished_events == 1 &&
              celest_do_spell.name == "do_spell" && celest_do_spell.generation != 0 &&
              celest_completed && celest_completed->phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::completed &&
              celest_completed->applied_results.size() == 2 &&
              celest_completed->celest_dead_target_calculations.empty() &&
              celest_completed->applied_results[0].source_mask == 0x1005554Au &&
              celest_completed->applied_results[1].source_mask == 0x1005554Au &&
              live_celest_target->health < coordinator_hp_before &&
              session.world()->random_state().calls > coordinator_rng_before.calls &&
              celest_completed->detail.find("Celest_Level_1_Player") != std::string::npos)) {
            std::cerr << "Celest coordinator events=" << coordinator_spell_events << "/"
                      << coordinator_finished_events << " marker=" << celest_do_spell.name
                      << " phase=" << (celest_completed ? static_cast<int>(celest_completed->phase) : -1)
                      << " applied=" << (celest_completed ? celest_completed->applied_results.size() : 0)
                      << " formula=" << (celest_completed ? celest_completed->celest_dead_target_calculations.size() : 0)
                      << " hp=" << live_celest_target->health << " before=" << coordinator_hp_before
                      << " rng=" << session.world()->random_state().calls << " before="
                      << coordinator_rng_before.calls << " detail="
                      << (celest_completed ? celest_completed->detail : "<none>") << '\n';
        }
        check(coordinator_spell_events == 1 && coordinator_finished_events == 1 &&
              celest_do_spell.name == "do_spell" && celest_do_spell.generation != 0 &&
              celest_completed && celest_completed->phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::completed &&
              celest_completed->applied_results.size() == 2 &&
              celest_completed->celest_dead_target_calculations.empty() &&
              celest_completed->applied_results[0].source_mask == 0x1005554Au &&
              celest_completed->applied_results[1].source_mask == 0x1005554Au &&
              live_celest_target->health < coordinator_hp_before &&
              session.world()->random_state().calls > coordinator_rng_before.calls &&
              celest_completed->detail.find("Celest_Level_1_Player") != std::string::npos,
              "Fresh Celest state7 do_spell coordinator path did not apply two same-session results and complete with non-veto FX diagnostic");
        const auto coordinator_hp_after = live_celest_target->health;
        const auto coordinator_rng_after = session.world()->random_state();
        dh::foundation::generic_skills::RuntimeSkillCastReceiptV1 duplicate_celest_event;
        check(celest_coordinator.apply_retained_use_event_v1(session,
              session.player_id(), celest_begin.generation,
              dh::foundation::generic_skills::RuntimeSkillSourceAnimStateV1::cast,
              celest_do_spell, duplicate_celest_event, error), error);
        check(live_celest_target->health == coordinator_hp_after &&
              session.world()->random_state().seed == coordinator_rng_after.seed &&
              session.world()->random_state().calls == coordinator_rng_after.calls,
              "Duplicate Celest retained state7 do_spell mutated same-session HP/RNG");

        for (std::uint32_t i = 0; i < 320; ++i) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        live_celest_target->transform.position = {1200.0f + celest_offset, 0.0f, 0.0f};
        HottySourceTargetListV1 empty_celest_targets;
        check(query_hotty_character_targets_v1(*session.world(), 1, actor_order,
              source_facts, 600, empty_celest_targets, error), error);
        check(empty_celest_targets.query_complete &&
              empty_celest_targets.character_targets_in_source_order.empty(),
              "Celest empty target branch did not retain a complete empty source query");
        dh::foundation::generic_skills::RuntimeSkillCastCoordinatorV1 empty_celest_coordinator;
        dh::foundation::generic_skills::RuntimeSkillFaerySpellArmV1 empty_celest_arm = faery_arm;
        empty_celest_arm.targets = &empty_celest_targets;
        empty_celest_arm.effect_dispatch = nullptr;
        empty_celest_arm.celest_effect_dispatch = nullptr;
        auto empty_celest_request = celest_cast_request;
        empty_celest_request.active_faery_spell = &empty_celest_arm;
        const auto empty_celest_hp_before = live_celest_target->health;
        const auto empty_celest_rng_before = session.world()->random_state();
        dh::foundation::generic_skills::RuntimeSkillCastReceiptV1 empty_celest_begin;
        std::uint32_t empty_celest_use_events = 0;
        std::uint32_t empty_celest_finished_events = 0;
        CombatSessionStateAnimationServices empty_celest_services;
        empty_celest_services.event = [&](ActorId actor,
                const RetainedAnimationEvent& event, std::string& callback_error) {
            if (actor != session.player_id()) return false;
            if (event.name != "do_spell") { callback_error.clear(); return true; }
            ++empty_celest_use_events;
            callback_error.clear();
            return true;
        };
        empty_celest_services.finished = [&](ActorId actor, std::string& callback_error) {
            ++empty_celest_finished_events;
            if (actor != session.player_id()) return false;
            callback_error.clear();
            return true;
        };
        empty_celest_request.downstream_animation_services = empty_celest_services;
        check(empty_celest_coordinator.begin_skill_cast_v1(empty_celest_request,
              session, empty_celest_begin, error), error);
        check(empty_celest_begin.phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::prepared_pending_use &&
              empty_celest_begin.faery_slot == 0 && empty_celest_begin.faery_record_id == 1 &&
              empty_celest_begin.target_order.empty() &&
              session.world()->combat_properties(1)->sheets.resolved[41] ==
                  full_player_properties.sheets.resolved[41] - 30 * 256,
              "Celest empty-list OnPre did not debit source mana and retain its no-target phase");
        for (std::uint64_t frame = 1; frame <= 900 && !empty_celest_finished_events; ++frame) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        const auto* empty_celest_completed = empty_celest_coordinator.receipt(session.player_id());
        check(empty_celest_use_events == 1 && empty_celest_finished_events == 1 &&
              empty_celest_completed && empty_celest_completed->phase ==
                  dh::foundation::generic_skills::RuntimeSkillCastPhaseV1::completed &&
              empty_celest_completed->applied_results.empty() &&
              empty_celest_completed->celest_dead_target_calculations.empty() &&
              live_celest_target->health == empty_celest_hp_before &&
              session.world()->random_state().seed == empty_celest_rng_before.seed &&
              session.world()->random_state().calls == empty_celest_rng_before.calls,
              "Celest empty-target do_spell did not complete without a source roll or HP/RNG mutation");

        for (std::uint32_t i = 0; i < 320; ++i) {
            check(session.update(frame_dt, {}, {0,0,0}, 0.0f, error), error);
            check(advance_hotty_clock(), error);
        }
        live_celest_target->transform.position = {500.0f + celest_offset, 0.0f, 0.0f};
        check(query_hotty_character_targets_v1(*session.world(), 1, actor_order,
              source_facts, 600, celest_targets, error), error);
        check(celest_targets.character_targets_in_source_order == std::vector<ActorId>{2},
              "Celest lethal fixture failed to restore its source target after empty-target case");
        live_celest_target->health = 0.001f;
        check(live_celest_target->alive(), "Lethal Celest source fixture target is not alive before Use");
        CelestPreparedCastV1 lethal_celest_prepared;
        check(prepare_celest_spell_v1(session, character, 0, faery_tables,
              database.classes, rules, policy, clock, lethal_celest_prepared,
              error, &celest_targets), error);
        CelestSourceUseV1 lethal_celest_use;
        const auto lethal_rng_before = session.world()->random_state();
        check(apply_celest_source_use_v1(session, lethal_celest_prepared,
              celest_targets, character, clock, faery_tables, database.classes,
              nullptr, lethal_celest_use, error), error);
        if (!(lethal_celest_use.source_use_complete &&
              lethal_celest_use.rolls.size() == 1 &&
              lethal_celest_use.rolls.front().target_died &&
              lethal_celest_use.dead_target_calculations.size() == 1 &&
              lethal_celest_use.dead_target_calculations.front().target == 2 &&
              lethal_celest_use.dead_target_calculations.front().event_index == 1 &&
              lethal_celest_use.dead_target_calculations.front().result.original.amount >= 0 &&
              !live_celest_target->alive() &&
              session.world()->random_state().calls > lethal_rng_before.calls)) {
            std::cerr << "Celest lethal prefix complete=" << lethal_celest_use.source_use_complete
                      << " applied=" << lethal_celest_use.rolls.size()
                      << " calc=" << lethal_celest_use.dead_target_calculations.size()
                      << " alive=" << live_celest_target->alive()
                      << " hp=" << live_celest_target->health
                      << " died=" << (!lethal_celest_use.rolls.empty() && lethal_celest_use.rolls.front().target_died)
                      << " rng=" << session.world()->random_state().calls
                      << " before=" << lethal_rng_before.calls
                      << " detail=" << lethal_celest_use.partial_prefix_diagnostic << '\n';
        }
        check(lethal_celest_use.source_use_complete &&
              lethal_celest_use.rolls.size() == 1 &&
              lethal_celest_use.rolls.front().target_died &&
              lethal_celest_use.dead_target_calculations.size() == 1 &&
              lethal_celest_use.dead_target_calculations.front().target == 2 &&
              lethal_celest_use.dead_target_calculations.front().event_index == 1 &&
              lethal_celest_use.dead_target_calculations.front().result.original.amount >= 0 &&
              !live_celest_target->alive() &&
              session.world()->random_state().calls > lethal_rng_before.calls,
              "Celest lethal first roll did not consume exactly one dead-target formula-only second occurrence");
        const auto lethal_hp_after = live_celest_target->health;
        const auto lethal_rng_after = session.world()->random_state();
        CombatSessionSourceHit duplicate_dead_calc;
        duplicate_dead_calc.attacker = session.player_id();
        duplicate_dead_calc.target = 2;
        duplicate_dead_calc.binding_lease = session.actor_binding_lease();
        duplicate_dead_calc.generation = lethal_celest_prepared.cooldown_ready_at_ms;
        duplicate_dead_calc.event_index = 1;
        duplicate_dead_calc.mask = 0x1005554Au;
        duplicate_dead_calc.source_id = "faerie_celest";
        duplicate_dead_calc.marker_name = "SpellCombatRoll";
        duplicate_dead_calc.category = -1;
        duplicate_dead_calc.element = lethal_celest_prepared.source_spell_type;
        duplicate_dead_calc.direct_amount = 0;
        duplicate_dead_calc.attacker_formula_sheet = &lethal_celest_prepared.spell_properties;
        CombatSessionSourceCalculation duplicate_dead_calculation;
        check(session.resolve_source_result_only(duplicate_dead_calc,
              duplicate_dead_calculation, error) &&
              !duplicate_dead_calculation.calculated &&
              live_celest_target->health == lethal_hp_after &&
              session.world()->random_state().seed == lethal_rng_after.seed &&
              session.world()->random_state().calls == lethal_rng_after.calls,
              "Duplicate Celest dead-target formula occurrence replayed RNG or health");
        const auto callbacks_before_session_teardown = menu_order.size();
        session_owner.reset();
        check(!session_provider.release(80.f, 80.f, error) &&
              error.find("Session lifetime") != std::string::npos &&
              menu_order.size() == callbacks_before_session_teardown,
              "Faery provider must reject after its bound Session is destroyed before invoking source callbacks");
        std::cout << "PASS connected Hotty/Celest behavior and Faery provider teardown guard; CPU-only source BDAE instance creation verified\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
