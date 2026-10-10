#include "runtime_troll_return_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_character.hpp"
#include "../../content_paths.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
std::int32_t scalar(const OriginalCampaignCommand& command, unsigned offset) {
    const auto found = command.scalars.find(offset);
    if (found == command.scalars.end()) throw std::runtime_error("Missing authored scalar " + std::to_string(offset));
    std::int32_t value{};
    std::memcpy(&value, &found->second, sizeof(value));
    return value;
}
void run(const std::filesystem::path& repo) {
    const auto root = repo / ".local-inputs/windows-source-clock-checkpoint-v17/assets";
    AssetCatalog assets(root), cache(root / "original-cache");
    std::string error;
    OriginalCampaignRuntime campaign;
    OriginalMeleeBindings bindings;
    check(campaign.load(assets, "original-campaign.xml", error), error);
    check(bindings.load(assets, "original-melee-bindings.xml", error), error);
    ActorCustomization customization;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual;
    check(build_original_combat_visual_plan(cache, bindings, "Troll", customization,
                                             "troll-jump-source-audit", visual, error), error);
    TrollReturnSourceBankV1 bank;
    check(build_troll_return_source_bank_v1(cache, bindings, campaign, visual.config,
                                             "troll-jump-source-audit", bank, error), error);
    check(bank.script_id == campaign.script_id("TrollReturn", false) &&
          bank.trigger_key.find("obj_1of4_brdwalk_nse_00.mgp::_prim_TriggerZone_001") != std::string::npos,
          "Source TrollReturn trigger/script identity changed");
    const auto trigger = campaign.triggers().find(bank.trigger_key);
    check(trigger != campaign.triggers().end(), "Exact source trigger row absent from loaded campaign");
    const auto& attrs = trigger->second.attributes;
    check(attrs.at("gametype") == "TriggerZone" && attrs.at("script") == "TrollReturn" &&
          attrs.at("triggercount") == "1" && attrs.at("triggerdelay") == "0" &&
          attrs.at("script_move_out").empty() &&
          (attrs.find("activate_cond") == attrs.end() || attrs.at("activate_cond").empty()),
          "TrollReturn trigger declaration differs from the original one-shot row");

    const auto& script = campaign.scripts().at(static_cast<std::size_t>(bank.script_id));
    check(script.scope == "level" && script.commands.size() > 16,
          "Loaded TrollReturn script scope/rows changed");
    const auto& focus = script.commands.at(4);
    const auto& camera = script.commands.at(11);
    const auto& returnTarget = script.commands.at(16);
    check(focus.kind == 8 && scalar(focus, 8) == 1500 &&
          focus.strings.at(16) == "_prim_Monster_53_03_001",
          "Authored Troll camera-target transition changed");
    check(camera.kind == 5 && scalar(camera, 8) == 44 && scalar(camera, 12) == 1,
          "Authored blocking camera44 command changed");
    check(returnTarget.kind == 8 && scalar(returnTarget, 8) == 500 &&
          returnTarget.strings.at(16) == "LocalPlayer",
          "Authored camera return target/duration changed");
    check(bank.clip_needs.size() == 3, "TrollReturn pre/attack/post clip set changed");
    const std::int32_t ids[] = {1377, 1359, 1373};
    const std::string labels[] = {"pre", "attack", "post"};

    auto rootConfig = bank.plan.config;
    rootConfig.motion_node_id = "auto";
    rootConfig.consume_root_motion = true;
    CharacterVisual troll;
    check(troll.load(cache, rootConfig, error), error);
    for (std::size_t i = 0; i < bank.clip_needs.size(); ++i) {
        const auto& need = bank.clip_needs[i];
        check(need.animation_id == ids[i] && need.wait == (i != 1),
              "TrollReturn command order/wait semantics changed");
        const auto* phase = bank.plan.phase("Attack", 0, need.source_path);
        check(phase && phase->animationId == ids[i] && phase->moveGO == 1 &&
              std::abs(phase->speed - 1.3) < 0.0001,
              "TrollReturn leaf lost authored MoveGO/rate metadata");
        std::int32_t start{}, end{};
        check(troll.animation_range(need.alias, start, end, error), error);
        Vec3 origin{}, endpoint{};
        check(troll.sample_source_root_translation(need.alias, start, origin, error), error);
        check(troll.sample_source_root_translation(need.alias, end, endpoint, error), error);
        RootMotionHistory history;
        Vec3 sum{}, delta{};
        double travel = 0.0;
        const auto frames = std::max(1, end - start);
        for (std::uint32_t frame = 1; frame <= 60; ++frame) {
            const auto sourceMs = start + frames * static_cast<std::int32_t>(frame) / 60;
            check(troll.step_root_motion(need.alias, (sourceMs - start) / 1000.0,
                                         false, frame, history, delta, error), error);
            sum.x += delta.x; sum.y += delta.y; sum.z += delta.z;
            travel += std::hypot(delta.x, delta.y);
        }
        check(std::abs(sum.x - (endpoint.x - origin.x)) +
              std::abs(sum.y - (endpoint.y - origin.y)) +
              std::abs(sum.z - (endpoint.z - origin.z)) < 0.02,
              "Sampled Troll root deltas do not telescope to the authored root endpoints");
        std::cout << labels[i] << " id=" << need.animation_id << " range=" << start << ".." << end
                  << " root_delta=" << sum.x << ',' << sum.y << ',' << sum.z
                  << " xy_travel=" << travel << '\n';
    }
    check(cache.read("data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp").size() > 0,
          "Original Troll trigger MGP is unavailable");
    std::cout << "PASS exact TrollReturn MGP-derived trigger row, campaign command/camera rows, "
                 "melee binding leaves, and sampled BDAE root paths; source activation still requires real TriggerZone services\n";
}
}
int main(int argc, char** argv) {
    try {
        check(argc == 2, "Repository root required");
        run(argv[1]);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
