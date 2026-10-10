#include "../../../engine-resources/resources.hpp"
#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/items.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../../animation_markers.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "runtime_source_projectile_v1.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh::foundation::effects;
namespace {
unsigned checks{};
void check(bool condition, const std::string &why) {
  ++checks;
  if (!condition)
    throw std::runtime_error(why);
}
std::vector<std::uint8_t> read(const std::filesystem::path &p) {
  std::ifstream f(p, std::ios::binary);
  if (!f)
    throw std::runtime_error("Missing actual asset " + p.string());
  return {std::istreambuf_iterator<char>(f), {}};
}
std::int32_t row(const dh2::android_ui::SourceProcessArraysV101 &t,
                 const char *name) {
  const auto &g = *t.group("ProjectileTable");
  for (std::size_t i = 0; i < g.names.size(); ++i)
    if (g.names[i] == name)
      return static_cast<std::int32_t>(i);
  throw std::runtime_error("Missing actual row");
}
} // namespace
int main(int argc, char **argv) {
  try {
    check(argc == 2, "asset root argument");
    const std::filesystem::path root = argv[1];
    std::string error;
    auto tables = std::make_shared<dh2::android_ui::SourceProcessArraysV101>();
    bool complete = false;
    auto reader = [&](const std::string &uri, std::vector<std::uint8_t> &bytes,
                      std::string &) {
      bytes = read(root / uri.substr(5));
      return true;
    };
    while (!complete)
      check(tables->load_stage(reader, complete, error), error);
    check(tables->ready(), "Actual70 source load stages");
    auto item_records = read(root / "pydata/loot_table_pyarray.bin"),
         item_names = read(root / "pydata/loot_table_pyarraynames.bin"),
         item_fields = read(root / "pydata/loot_table_pystructnames.bin");
    auto item_bytes = [](const auto &b) {
      return dh2::data::Bytes{b.data(), b.size()};
    };
    dh2::data::ItemTable item_table;
    check(dh2::data::load_items(item_bytes(item_records),
                                item_bytes(item_names), item_bytes(item_fields),
                                item_table, error),
          error);
    for (const char *name : {"Staff01", "Bow01", "Wand01", "Sword01"}) {
      auto id = dh2::data::item_id(item_table, name);
      if (id >= 0) {
        const auto &w =
            item_table.rows[static_cast<std::size_t>(id)].record.words;
        std::cout << "source_item=" << name << " id=" << id << " type=" << w[22]
                  << " category=" << w[37] << " Param4=" << w[38]
                  << " Param5=" << w[39] << " Param6=" << w[40] << '\n';
      }
    }
    dh::foundation::AssetCatalog assets(root);
    dh::foundation::OriginalPropertyDatabase database;
    check(dh::foundation::load_original_property_tables(assets, "pydata",
                                                        database, error),
          error);
    dh::foundation::OriginalActorProperties mage;
    check(dh::foundation::resolve_original_fresh_player(
              database, "MagePlayerBase", mage, error),
          error);
    std::cout << "source_MagePlayerBase_resolved_range="
              << mage.sheets.resolved[30] << ',' << mage.sheets.resolved[31]
              << ',' << mage.sheets.resolved[32] << '\n';
    check(mage.sheets.resolved[32] == -1,
          "Actual Mage cached projectile uses inventory branch");
    dh2::character::CombatProperties896 current_properties{};
    std::copy(mage.sheets.resolved.begin(), mage.sheets.resolved.end(),
              current_properties.words);
    std::vector<dh2::character::CombatItemRecord164> query_rows(
        item_table.rows.size());
    for (std::size_t i = 0; i < item_table.rows.size(); ++i)
      std::copy_n(item_table.rows[i].record.words, 41, query_rows[i].words);
    dh2::character::CombatItemInstance4 staff{
        dh2::data::item_id(item_table, "Staff01")},
        wand{dh2::data::item_id(item_table, "Wand01")},
        bow{dh2::data::item_id(item_table, "Bow01")};
    const dh2::character::CombatItemInstance4 *staff_ref = &staff;
    const dh2::character::CombatItemInstance4 *wand_ref = &wand;
    dh2::character::CombatEquipSet8 equipped[2]{{&staff_ref}, {&wand_ref}};
    dh2::character::CombatInventory16 current_equipment{equipped, 2, 0};
    bool capable = false;
    RuntimeSourceRangeSelectionV1 selected;
    check(runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              capable,
          error);
    check(selected.origin == RuntimeSourceRangeOriginV1::current_equipment &&
              selected.selected_item == 1025 && selected.equipment_type == 5 &&
              selected.minimum == 1 && selected.maximum == 1000,
          "Actual starterStaff current-set outputs");
    check(selected.projectile.row == 20 &&
              selected.projectile.name == "FireWandProjectile" &&
              selected.projectile.model == 1 &&
              selected.projectile.model_uri ==
                  "data/3D/projectiles/elemental_bolt_fire.bdae" &&
              selected.projectile.velocity == 20.f &&
              selected.projectile.timer_ms == 1000,
          "Staff01 source projectile20 differs from FireBall18");
    const auto starter_staff_selection = selected;
    current_equipment.current_set = 1;
    check(runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              capable && selected.selected_item == 1226 &&
              selected.projectile.row == 15 &&
              selected.projectile.name == "EarthWandProjectile" &&
              selected.projectile.model == 0,
          "Current-set swap selects actualWand01 earth row");
    wand_ref = &bow;
    check(runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              capable && selected.equipment_type == 4 &&
              selected.projectile.row == 10 && selected.projectile.model == 9,
          "ActualBow type4 source dictionary selection");
    const auto saved_selection = selected.projectile.row;
    equipped[1].main_hand = nullptr;
    check(runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              !capable && selected.projectile.row == saved_selection,
          "Unarmed source denial preserves plan");
    dh2::character::CombatItemInstance4 nonrange{-1};
    for (std::size_t i = 0; i < query_rows.size(); ++i)
      if (query_rows[i].words[22] != 4 && query_rows[i].words[22] != 5) {
        nonrange.item_id = static_cast<std::int32_t>(i);
        break;
      }
    check(nonrange.item_id >= 0, "Actual nonranged ItemTable row");
    wand_ref = &nonrange;
    equipped[1].main_hand = &wand_ref;
    check(runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              !capable && selected.projectile.row == saved_selection,
          "Actual nonranged current item denied");
    current_equipment.current_set = 2;
    check(!runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              !capable && selected.projectile.row == saved_selection,
          "Malformed current equipment atomic");
    // Diagnostic cached-cell branch inputs: known source row10 and signedASR8.
    current_properties.words[30] = -257;
    current_properties.words[31] = 256000;
    current_properties.words[32] = 10;
    check(runtime_source_range_selection_v1(tables, current_properties, nullptr,
                                            nullptr, 0, capable, selected,
                                            error) &&
              capable &&
              selected.origin ==
                  RuntimeSourceRangeOriginV1::cached_property32 &&
              selected.minimum == -2 && selected.maximum == 1000 &&
              selected.projectile.row == 10 && selected.selected_item == -1,
          "Cached property shortcut ignores unavailable equipment");
    current_properties.words[32] = 100000;
    check(!runtime_source_range_selection_v1(tables, current_properties,
                                             nullptr, nullptr, 0, capable,
                                             selected, error) &&
              !capable && selected.projectile.row == 10,
          "Invalid cached projectile row atomic");
    std::copy(mage.sheets.resolved.begin(), mage.sheets.resolved.end(),
              current_properties.words);
    current_equipment.current_set = 0;
    const auto preserved_row = selected.projectile.row;
    staff.item_id = -1;
    check(!runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              !capable && selected.projectile.row == preserved_row,
          "Invalid actual selected item loan rejected atomically");
    staff.item_id = 1025;
    check(!runtime_source_range_selection_v1(
              {}, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              !capable && selected.projectile.row == preserved_row,
          "Reached source projectile tables unavailable");
    check(runtime_source_range_selection_v1(
              tables, current_properties, &current_equipment, query_rows.data(),
              static_cast<std::uint32_t>(query_rows.size()), capable, selected,
              error) &&
              capable && selected.projectile.row == 20,
          "Repeat native read-only selection yields same source row");
    check(current_equipment.current_set == 0 && staff.item_id == 1025 &&
              current_properties.words[32] == -1,
          "Selection leaves borrowed equipment and properties unchanged");
    const auto fire = row(*tables, "FireBall");
    RuntimeSourceProjectilePlanV1 plan;
    check(runtime_source_projectile_plan_v1(tables, fire, plan, error), error);
    check(plan.tables.get() == tables.get() && plan.row == fire &&
              plan.name == "FireBall",
          "Same decoded rows retained");
    check(plan.model == 1 &&
              plan.model_uri ==
                  "data/3D/projectiles/elemental_bolt_fire.bdae" &&
              plan.class_name == "Projectile",
          "Actual FireBall dictionary/class");
    check(plan.velocity == 15.f && plan.timer_ms == 2000 &&
              !plan.requires_model_duration(),
          "Exact original speed/timer");
    check(plan.velocity_damping == 0.25f && plan.max_distance == -1.f &&
              !plan.target_lock && plan.disappear_on_hit &&
              plan.impact_fx == 241 && plan.impact_sound == 459,
          "Actual FireBall motion/filter/impact rules preserved");
    const auto model = read(root / plan.model_uri);
    dh2::resources::BresView view;
    check(dh2_bres_open(&view, model.data(), model.size()) ==
              dh2::resources::BresError::ok,
          "Actual projectile mesh BRES");
    auto records = read(root / "pydata/animations_pyarray.bin"),
         names = read(root / "pydata/animations_pyarraynames.bin"),
         fields = read(root / "pydata/animations_pystructnames.bin");
    auto dict_names =
             read(root / "pydata/animations_dictionary_pyarraynames.bin"),
         dict_paths = read(root / "pydata/animations_dictionary_pyarray.bin");
    auto bytes = [](const auto &b) {
      return dh2::data::Bytes{b.data(), b.size()};
    };
    dh2::data::Dictionary clips;
    dh2::data::AnimationTables animation;
    check(dh2::data::load_dictionary(bytes(dict_names), bytes(dict_paths),
                                     clips, error),
          error);
    check(dh2::data::load_animation_tables(bytes(records), bytes(names),
                                           bytes(fields), clips, animation,
                                           error),
          error);
    std::int32_t clip_id = -1;
    for (std::size_t i = 0; i < clips.names.size(); ++i)
      if (clips.names[i] == "prince_ranged_attack")
        clip_id = static_cast<std::int32_t>(i);
    check(clip_id >= 0, "Actual ranged clip dictionary");
    bool linked = false;
    for (std::size_t i = 0; i < animation.sequences.size(); ++i)
      for (const auto &step : animation.sequences[i].steps)
        if (!step.redir && step.anim == clip_id) {
          linked = true;
          std::cout << "authored_sequence=" << animation.sequence_names[i]
                    << " id=" << i << " clip=" << clip_id << '\n';
        }
    check(linked, "Actual AnimTable references ranged marker clip");
    auto skill_records = read(root / "pydata/skills_pyarray.bin"),
         skill_names = read(root / "pydata/skills_pyarraynames.bin"),
         skill_fields = read(root / "pydata/skills_pystructnames.bin");
    dh2::data::SkillTables skill_tables;
    check(skill_tables.load(bytes(skill_records), bytes(skill_names),
                            bytes(skill_fields), error),
          error);
    auto skills = skill_tables.borrow();
    bool source_fire_skill = false;
    for (std::size_t i = 0; i < skills.skills().size(); ++i) {
      const auto &skill = skills.skills()[i];
      if (skill.script.find("npc_fireball") != std::string::npos) {
        source_fire_skill = true;
        const auto animation_id = skill.scalar.words[1];
        check(animation_id < animation.sequence_names.size(),
              "Actual NPC fireball animation row");
        std::cout << "source_skill=" << skills.skill_names()[i] << " row=" << i
                  << " script=" << skill.script
                  << " animation_id=" << animation_id
                  << " sequence=" << animation.sequence_names[animation_id]
                  << '\n';
        bool matches_exact_prince_ranged_clip = false;
        std::function<void(std::uint32_t, unsigned)> trace =
            [&](std::uint32_t id, unsigned depth) {
              check(depth < 4, "Source animation redirect depth");
              for (const auto &step : animation.sequences[id].steps) {
                std::cout
                    << "source_fire_skill_step=" << step.anim
                    << " redir=" << step.redir << " name="
                    << (step.redir
                            ? animation.sequence_names[static_cast<std::size_t>(
                                  step.anim)]
                            : clips.names[static_cast<std::size_t>(step.anim)])
                    << '\n';
                if (!step.redir && step.anim == clip_id)
                  matches_exact_prince_ranged_clip = true;
                if (step.redir)
                  trace(static_cast<std::uint32_t>(step.anim), depth + 1);
              }
            };
        trace(animation_id, 0);
        check(
            !matches_exact_prince_ranged_clip,
            "Exact NPC fireball skill does not use prince_ranged_attack clip");
        for (std::size_t list = 0; list < skills.lists().size(); ++list)
          for (auto entry : skills.lists()[list])
            if (entry == static_cast<std::int32_t>(i))
              std::cout << "source_skill_list=" << skills.list_names()[list]
                        << '\n';
      }
    }
    check(source_fire_skill, "Actual NPC fireball SkillTable row present");
    const auto clip =
        read(root /
             "data/3D/characters/prince/animations/prince_ranged_attack.bdae");
    dh::foundation::AnimationMarkers markers;
    check(markers.load(clip.data(), clip.size(), 0, 10000, error), error);
    const dh::foundation::AnimationMarker *launch = nullptr;
    for (const auto &m : markers.markers())
      if (m.name == "do_skill")
        launch = &m;
    check(launch != nullptr, "Actual prince ranged clip do_skill marker");
    std::cout << "authored_marker=" << launch->name
              << " sampling_ms=" << launch->time_ms
              << " authored_ms=" << launch->authored_time_ms << '\n';
    bool starter_handled = false;
    RuntimeSourceProjectilePlanV1 starter_packet;
    dh2::data::CombatEventContext starter_marker{
        5, 0, 1, 1, starter_staff_selection.projectile.row};
    check(runtime_source_ranged_event_v1(tables, starter_marker,
                                         launch->name.c_str(), starter_handled,
                                         starter_packet, error) &&
              starter_handled && starter_packet.row == 20 &&
              starter_packet.timer_ms == 1000,
          "Exact currentStaff selected row feeds actual1077 do_skill decision");
    bool handled = false;
    dh2::data::CombatEventContext event{5, 0, 1, 1, fire};
    check(runtime_source_ranged_event_v1(tables, event, launch->name.c_str(),
                                         handled, plan, error) &&
              handled,
          "Actual marker ranged decision");
    for (const char *marker : {"attack_ranged", "attack_mainhand", "do_skill"})
      check(runtime_source_ranged_event_v1(tables, event, marker, handled, plan,
                                           error) &&
                handled,
            "Original ranged aliases");
    const auto original = plan.row;
    for (const char *marker : {"attack_offhand", "fx_attack", "interact"})
      check(runtime_source_ranged_event_v1(tables, event, marker, handled, plan,
                                           error) &&
                !handled && plan.row == original,
            "Unreached event preserves plan");
    event.can_range = 0;
    check(runtime_source_ranged_event_v1(tables, event, "do_skill", handled,
                                         plan, error) &&
              !handled,
          "False capability no projectile");
    event.can_range = 1;
    event.state = 6;
    check(runtime_source_ranged_event_v1(tables, event, "do_skill", handled,
                                         plan, error) &&
              !handled,
          "Skill state is different caller");
    event.state = 5;
    event.can_range = 2;
    check(!runtime_source_ranged_event_v1(tables, event, "do_skill", handled,
                                          plan, error) &&
              !handled && plan.row == original,
          "Invalid capability atomic");
    event.can_range = 1;
    event.projectile = -1;
    check(!runtime_source_ranged_event_v1(tables, event, "do_skill", handled,
                                          plan, error) &&
              !handled && plan.row == original,
          "Negative selected row atomic");
    check(!runtime_source_projectile_plan_v1(tables, 100000, plan, error) &&
              plan.row == original,
          "Invalid row atomic");
    check(!runtime_source_projectile_plan_v1({}, fire, plan, error) &&
              plan.row == original,
          "Missing source tables atomic");
    check(!runtime_source_projectile_plan_v1(
              std::make_shared<dh2::android_ui::SourceProcessArraysV101>(),
              fire, plan, error) &&
              plan.row == original,
          "Not loaded source tables rejected");
    check(runtime_source_projectile_plan_v1(tables, row(*tables, "BlueLaser"),
                                            plan, error) &&
              plan.laser && plan.class_name == "LaserTypeProjectile",
          "Source IsLaser chooses actual class");
    check(runtime_source_projectile_plan_v1(tables, 0, plan, error) &&
              plan.model == -1 && plan.model_uri.empty(),
          "Original nullable-model row preserved");
    unsigned projected = 0;
    for (std::size_t i = 0; i < tables->group("ProjectileTable")->rows.size();
         ++i) {
      check(runtime_source_projectile_plan_v1(
                tables, static_cast<std::int32_t>(i), plan, error),
            error);
      ++projected;
    }
    tables.reset();
    check(plan.tables && plan.tables->ready(),
          "Plan preserves actual table lifetime");
    std::cout << "PASS checks=" << checks
              << " actual_projectile_rows=" << projected
              << " no_simulated_motion_or_damage\n";
    return 0;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
