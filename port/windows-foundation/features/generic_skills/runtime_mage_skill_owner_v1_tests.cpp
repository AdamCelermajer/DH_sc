// v21.8 magewire: unit coverage for RuntimeMageSkillOwnerV1 (external Pre/Use owner of ChainLightning, Cyclone, StoneEscape).
//
// Original (prince_mage_stone_escape.luac, via coordination/v21.8/mage-report.md): OnPreSkill_ plays the teleport FX at the
// old position, Use WarpTo()s the caster MOVE_RANGE (650) along its facing, summons a Dummy statue at the old position whose
// idle FX plays, and after TNT_DELAY (2200 ms) DoExplosion rolls SkillCombatRoll on every enemy within RANGE (650) of the
// statue, then the death FX plays. Covered here on a real CombatSession (MagePlayerBase, one lizard):
//   - handles(): the three owned skills, and not ColdRay (its arm is in the shared coordinator);
//   - without the shared projectile executor, Cyclone/ChainLightning report an explicit error (no silent cast);
//   - StoneEscape: Pre FX at the old position, Use WarpTo 650 along the facing, statue idle FX, nothing exploded before
//     2200 ms, then the blast reaches the lizard 300 units in front and the death FX plays; the idle FX is stopped.
// Not covered here (verified only in real runs): the projectile flight/contact of ChainLightning and Cyclone.
#include "runtime_mage_skill_owner_v1.hpp"
#include "runtime_skill_use_profile_registry_v1.hpp"
#include "../../combat_session.hpp"
#include "../../content_paths.hpp"
#include "../combat/runtime_player_combo_chain_v1.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace dh::foundation;
using namespace dh::foundation::generic_skills;
namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

struct Recorder {
    std::vector<std::string> trace;
    std::vector<std::string> effects;     // play_effect names, in order
    std::vector<std::string> stopped;     // stop_effect names
    std::vector<std::array<float, 3>> effect_at;
    unsigned moves = 0;
    std::array<float, 3> moved_to{};
    bool moved_teleport = false;
};
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Original shared asset root required");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase db;
        OriginalMeleeBindings bindings;
        check(load_original_property_tables(assets, "original-cache/data/pydata", db, error) &&
              bindings.load(assets, "original-melee-bindings.xml", error), error);
        const auto read = [&](const char* name) { return read_content(assets, std::string("data/pydata/") + name); };
        const auto bytes = [](const auto& buffer) { return dh2::data::Bytes{buffer.data(), buffer.size()}; };
        const auto clipNames = read("animations_dictionary_pyarraynames.bin"), clipValues = read("animations_dictionary_pyarray.bin");
        dh2::data::Dictionary clips;
        check(dh2::data::load_dictionary(bytes(clipNames), bytes(clipValues), clips, error), error);
        const auto records = read("animations_pyarray.bin"), names = read("animations_pyarraynames.bin"), fields = read("animations_pystructnames.bin");
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(records), bytes(names), bytes(fields), clips, animations, error), error);
        const auto itemRecords = read("loot_table_pyarray.bin"), itemNames = read("loot_table_pyarraynames.bin"), itemFields = read("loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(itemRecords), bytes(itemNames), bytes(itemFields), items, error), error);
        const auto constantsBytes = read_content(assets, "data/animations_pycst.bin");
        auto* constants = dh2_script_constants_create();
        check(constants, "Source constants unavailable");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants, constantsBytes.data(), static_cast<std::uint32_t>(constantsBytes.size()), &reload) == 0,
              "Source constants failed");
        equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
        check(equipment_menu::load_runtime_player_locomotion_constants_v1([&](const char* group, const char* key, std::int32_t& value, std::string& e) {
            if (dh2_script_constants_get(constants, group, key, &value)) { e = "Missing source constant"; return false; }
            return true;
        }, stance, error), error);
        dh2_script_constants_destroy(constants);
        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base;
        check(build_original_combat_visual_plan(assets, bindings, "MagePlayerBase", customization, "ranged-base", base, error), error);
        equipment_menu::RuntimePlayerLocomotionV1 equipped;
        const auto staff = dh2::data::item_id(items, "Staff01");
        check(staff >= 0, "Original Staff01 missing");
        check(equipment_menu::resolve_runtime_player_locomotion_v1(49, items, staff, -1, 0, stance, animations, clips, equipped, error), error);
        combat::RuntimePlayerComboChainPlanV1 attack;
        check(combat::plan_runtime_player_combo_chain_v1(assets, equipped, stance.stanced_list_mask, 3, true, animations, clips,
            "MagePlayerBase", base.config, "source-staff-bank", attack, error), error);

        // ---- handles() and the explicit no-executor errors (no session state needed) ----------------------------------
        RuntimeMageSkillOwnerV1 bare(RuntimeMageSkillServicesV1{});
        const auto* chain = find_skill_use_profile_registered_v1("ChainLightning", "prince_mage_chain_lightning");
        const auto* cyclone = find_skill_use_profile_registered_v1("Cyclone", "prince_mage_cyclone");
        const auto* stone = find_skill_use_profile_registered_v1("StoneEscape", "prince_mage_stone_escape");
        const auto* coldray = find_skill_use_profile_registered_v1("ColdRay", "prince_mage_coldray");
        check(chain && cyclone && stone && coldray, "Mage skill profile rows are missing");
        check(bare.handles(*chain) && bare.handles(*cyclone) && bare.handles(*stone), "Owner does not handle its three skills");
        check(!bare.handles(*coldray), "ColdRay must stay on the shared coordinator arm");
        std::cout << "handles: ChainLightning, Cyclone, StoneEscape true; ColdRay false\n";
        {
            CombatSession empty;
            dh2::data::PropertySheet sheet{};
            RuntimeSkillExternalContextV1 context;
            context.caster = 1;
            context.generation = 1;
            context.profile = cyclone;
            context.skill_name = "Cyclone";
            context.formula_sheet = &sheet;
            std::string cycloneError;
            check(!bare.use(empty, context, cycloneError) && cycloneError.find("shared projectile executor") != std::string::npos,
                  "Cyclone without the executor did not report an explicit error: " + cycloneError);
            context.profile = chain;
            context.skill_name = "ChainLightning";
            std::string chainError;
            check(!bare.use(empty, context, chainError) && !chainError.empty(),
                  "ChainLightning without tables did not report an explicit error");
            std::cout << "no-executor errors: Cyclone \"" << cycloneError << "\"; ChainLightning \"" << chainError << "\"\n";
        }

        // ---- StoneEscape on a real session ----------------------------------------------------------------------------
        CombatSessionConfig config;
        config.diagnosticRngSeed = 1234;
        config.playerId = 1;
        config.playerProfileId = "MagePlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = base.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        config.mainItemId = "Staff01";
        config.equippedItemIds = {"StartingSuitMage", "StartingBootsMage", "StartingGlovesMage", "Staff01"};
        CombatSessionProfile player;
        player.initialIdle = {"Idle", 0, {0}};
        player.propertyOptions = {256, true};
        player.reaction = CombatSessionChoice{"Injured", 0, {0}};
        player.death = CombatSessionChoice{"Died", 0, {0}};
        player.sequenceAction = attack.selection;
        player.retainedPhaseClock = true;
        player.sourceCombo = true;
        player.sourceAttackBank = attack.visual;
        player.sourceAttackPolicies = attack.sequence_policies;
        player.rangedProjectileMarkers = true;  // the Staff01 owner (ranged source weapon), as in combat_session_ranged_attack
        config.profiles.emplace(config.playerProfileId, player);
        CombatSessionProfile enemy;
        enemy.action = {"Attack", 0, {0, 1}};
        enemy.initialIdle = {"Idle", 0, {0}};
        enemy.propertyOptions = {20 * 256, true};
        enemy.damageMarkerNames = {"attack_mainhand"};
        enemy.customization = customization;
        config.profiles.emplace("Swamp_LizadMan_Type1", enemy);

        CombatSession session;
        CharacterVisual visual;
        ActorPopulation population;
        // The lizard placeholder is moved below once the caster heading is known (the session is created first).
        PopulationActor placed;
        placed.profileId = "Swamp_LizadMan_Type1";
        placed.definition.stableId = 2;
        placed.definition.sourceId = "stone-escape-lizard";
        placed.definition.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        placed.transform = placed.definition.placement;
        population.actors().push_back(std::move(placed));
        check(session.initialize(assets, db, bindings, config, visual, population, {0, 0, 0}, customization, error), error);
        const float heading = session.actor(1)->transform.rotation[2];
        const std::array<float, 3> look{std::sin(heading), -std::cos(heading), 0.0f};
        const std::array<float, 3> origin = session.actor(1)->transform.position;
        // A lizard 300 units in front: inside the StoneEscape blast radius (650) around the old position, and not warped with the caster.
        const std::array<float, 3> lizardAt{origin[0] + 300.0f * look[0], origin[1] + 300.0f * look[1], origin[2]};
        {
            auto* lizard = session.actor(2);
            check(lizard != nullptr, "Lizard actor is absent from the session");
            if (lizard) {
                // Place the actor exactly where the population asked (the session keeps its own transform copy).
                lizard->transform.position = lizardAt;
            }
        }
        const float lizardHealthBefore = session.actor(2)->health;

        Recorder rec;
        RuntimeMageSkillServicesV1 services;
        services.play_effect = [&](const std::string& name, const std::array<float, 3>& at, std::string&) {
            rec.effects.push_back(name);
            rec.effect_at.push_back(at);
            return true;
        };
        services.stop_effect = [&](const std::string& name, std::string&) {
            rec.stopped.push_back(name);
            return true;
        };
        services.move_actor = [&](ActorId actor, const std::array<float, 3>& position, bool teleport, std::string&) {
            check(actor == 1, "StoneEscape moved an actor other than the caster");
            ++rec.moves;
            rec.moved_to = position;
            rec.moved_teleport = teleport;
            return true;
        };
        services.trace = [&](const std::string& line) { rec.trace.push_back(line); };
        RuntimeMageSkillOwnerV1 owner(std::move(services));

        std::vector<ActorId> order{1, 2};
        std::vector<SkillTargetSourceFactsV1> facts{{1, true, false, false, true, true}, {2, true, false, false, true, true}};
        // The caster's resolved sheet (the F_SkillAttack attacker formula sheet of the real Session).
        dh2::data::PropertySheet sheet = session.world()->combat_properties(1)->sheets.resolved;
        {
            // The authored Skill_Mage_StoneEscape projection supplies the damage range the real Lua reads (GetProp SnS_*); this
            // test fixes a 100-damage range so the blast roll has a result. Damage values are not claimed as original.
            const auto find = [&](const char* name) {
                const auto found = std::find(db.characters.fields.begin(), db.characters.fields.end(), name);
                check(found != db.characters.fields.end(), std::string("CharacterProperties field missing: ") + name);
                return static_cast<std::size_t>(found - db.characters.fields.begin());
            };
            sheet[find("SnS_MinDamage")] = 100 * 256;
            sheet[find("SnS_MaxDamage")] = 100 * 256;
        }
        RuntimeSkillExternalContextV1 context;
        context.caster = 1;
        context.generation = 1;
        context.profile = stone;
        context.skill_name = "StoneEscape";
        // Test inputs for the SkillAttack roll (the coordinator supplies the real category/element of the SkillTable row).
        context.row_mask = 0x20000u;  // damage-type bit of the SkillCombatRoll mask (no damage bit = no damage: owner reports it)
        context.category = 0;
        context.element = 0;
        context.formula_sheet = &sheet;
        context.population_order = &order;
        context.target_facts = &facts;
        context.non_character_attackable_objects_absent = true;

        std::string e;
        check(owner.pre(session, context, e), "StoneEscape Pre failed: " + e);
        check(rec.effects.size() == 1 && rec.effects[0] == "spell_dh2_prince_mage_stone_escape_magefx", "StoneEscape Pre did not play the teleport FX");
        check(rec.effect_at.back() == origin, "StoneEscape Pre FX is not at the old position");

        check(owner.use(session, context, e), "StoneEscape Use failed: " + e);
        check(rec.moves == 1 && rec.moved_teleport, "StoneEscape Use did not WarpTo the caster once");
        const std::array<float, 3> expectedDestination{origin[0] + 650.0f * look[0], origin[1] + 650.0f * look[1], origin[2]};
        check(std::fabs(rec.moved_to[0] - expectedDestination[0]) < 0.01f && std::fabs(rec.moved_to[1] - expectedDestination[1]) < 0.01f,
              "StoneEscape destination is not MOVE_RANGE 650 along the facing");
        check(rec.effects.size() == 3 && rec.effects[1] == "spell_dh2_prince_mage_stone_escape_magefx" &&
              rec.effects[2] == "skill_dh2_prince_mage_stone_escape_idle", "StoneEscape Use FX sequence is wrong");
        check(rec.effect_at[2] == origin, "Statue idle FX is not at the old position");
        check(owner.pending_statues() == 1, "StoneEscape did not create its statue timer");

        // Before TNT_DELAY (2200 ms) nothing explodes.
        check(owner.advance(session, 2000.0, e), "advance before the blast failed: " + e);
        check(owner.pending_statues() == 1 && rec.stopped.empty(), "Statue exploded before TNT_DELAY");
        check(session.actor(2)->health == lizardHealthBefore, "Blast applied before its timer");
        // The timer crosses 2200 ms: the blast rolls on the lizard, the idle FX stops and the death FX plays.
        check(owner.advance(session, 300.0, e), "advance at the blast failed: " + e);
        check(owner.pending_statues() == 0, "Statue timer did not finish");
        check(rec.stopped.size() == 1 && rec.stopped[0] == "skill_dh2_prince_mage_stone_escape_idle", "Statue idle FX was not stopped");
        check(rec.effects.back() == "skill_dh2_prince_mage_stone_escape_death", "Statue death FX did not play");
        bool blastHit = false;
        for (const auto& line : rec.trace) if (line.find("StoneEscape blast hit target=2") != std::string::npos) blastHit = true;
        check(blastHit, "Blast did not reach the lizard 300 units in front");
        std::cout << "StoneEscape: warp " << rec.moved_to[0] << "," << rec.moved_to[1] << " (650 along facing), statue idle/death FX, blast hit lizard\n";
        std::cout << "mage_skill_owner_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "mage_skill_owner_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
