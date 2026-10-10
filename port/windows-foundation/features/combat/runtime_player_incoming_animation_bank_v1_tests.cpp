#include "runtime_player_incoming_animation_bank_v1.hpp"

#include "../../actor_profiles.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../original_character.hpp"
#include "../../actor_combat_runtime.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::combat;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read(const AssetCatalog& assets, const char* uri) {
    return read_content(assets, uri);
}

dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}

bool same_step(const dh2::data::AnimationStep& a, const dh2::data::AnimationStep& b) {
    return a.anchor_fx == b.anchor_fx && a.cam_dir == b.cam_dir &&
        a.move_go == b.move_go && a.swoosh == b.swoosh && a.anim == b.anim &&
        a.blend_out == b.blend_out && a.cam == b.cam && a.fx == b.fx &&
        a.redir == b.redir && a.sound == b.sound && a.speed == b.speed &&
        a.random_cam == b.random_cam;
}

std::int32_t root_id(const dh2::data::AnimationTables& animations, std::int32_t table,
                     const char* state) {
    const auto* sequence = dh2::data::animation_state(animations, table, state);
    check(sequence != nullptr, std::string("Missing source root ") + state);
    return static_cast<std::int32_t>(sequence - animations.sequences.data());
}

void check_markers(const OriginalCombatVisualPlan& plan,
                   const OriginalSequencePolicies& policies,
                   const OriginalAttackSelection& selection, CharacterVisual& visual,
                   const std::string& check_name) {
    auto visual_binding = combat_visual_binding(visual);
    OriginalAttackSequenceServices services;
    services.visual = visual_binding;
    services.restart_clip = [](const std::string&, std::string& error) {
        error.clear();
        return true;
    };
    OriginalAttackSequence sequence;
    std::string error;
    check(sequence.prepare(plan, policies, selection, services,
          "incoming-source-marker-check", error), error);
    check(sequence.phases().size() == 1,
          check_name + " source choice did not retain one authored leaf");
    const auto* authored = visual.markers(sequence.phases()[0].source.clipName, error);
    check(authored != nullptr, error);
    check(sequence.scheduled_markers().size() == authored->markers().size(),
          check_name + " source clip marker occurrences were lost");
    for (std::size_t i = 0; i < authored->markers().size(); ++i) {
        const auto found = std::find_if(sequence.scheduled_markers().begin(),
            sequence.scheduled_markers().end(), [&](const OriginalAttackScheduledMarker& marker) {
                return marker.source.name == authored->markers()[i].name &&
                       marker.source.time_ms == authored->markers()[i].time_ms &&
                       marker.source.authored_time_ms == authored->markers()[i].authored_time_ms &&
                       marker.source.index == authored->markers()[i].index;
            });
        check(found != sequence.scheduled_markers().end(),
              check_name + " source marker identity/time changed");
    }
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply the unified original asset root");
        AssetCatalog assets(argv[1]);
        std::string error;
        const auto clip_names = read(assets, "data/pydata/animations_dictionary_pyarraynames.bin");
        const auto clip_values = read(assets, "data/pydata/animations_dictionary_pyarray.bin");
        dh2::data::Dictionary clips;
        check(dh2::data::load_dictionary(bytes(clip_names), bytes(clip_values), clips, error), error);
        const auto records = read(assets, "data/pydata/animations_pyarray.bin");
        const auto names = read(assets, "data/pydata/animations_pyarraynames.bin");
        const auto fields = read(assets, "data/pydata/animations_pystructnames.bin");
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(records), bytes(names), bytes(fields),
              clips, animations, error), error);
        ActorProfileLibrary profiles;
        check(profiles.load(assets, "actor-profiles-v2.xml", error), error);

        struct ExpectedProfile { const char* id; std::int32_t table; };
        const ExpectedProfile expected[] = {
            {"KnightPlayerBase", 48}, {"RoguePlayerBase", 50}, {"MagePlayerBase", 49}
        };
        for (const auto& expected_profile : expected) {
            const auto* profile = profiles.find(expected_profile.id);
            check(profile != nullptr, std::string("Missing source profile ") + expected_profile.id);
            check(std::stoi(profile->animation_table) == expected_profile.table,
                  std::string("Wrong source animationTable for ") + profile->id);

            ActorCustomization customization;
            customization.allow_missing_animation_targets = true;
            CharacterVisualConfig actor_visual;
            check(make_visual_config(assets, *profile, customization, actor_visual, error), error);

            RuntimePlayerIncomingAnimationBankV1 bank;
            check(build_runtime_player_incoming_animation_bank_v1(assets, *profile,
                  actor_visual, animations, clips, "incoming-test-" + profile->id,
                  bank, error), error);
            const auto injury_root = root_id(animations, expected_profile.table, "Injured");
            const auto death_root = root_id(animations, expected_profile.table, "Died");
            check(injury_root == 267 && death_root == 259,
                  std::string("Actual player CharAnimTable incoming roots changed for ") + profile->id);
            check(bank.profile_id == profile->id && bank.animation_table == expected_profile.table &&
                  bank.injury_override_sequence_id == injury_root &&
                  bank.death_override_sequence_id == death_root,
                  std::string("Incoming roots escaped exact profile table for ") + profile->id);
            check(bank.visual.profileId == profile->id,
                  std::string("Incoming bank borrowed another profile's visuals for ") + profile->id);
            const auto* injury = bank.visual.sequence("Injured", 0);
            const auto* died = bank.visual.sequence("Died", 0);
            check(injury && died && injury->id == injury_root && died->id == death_root,
                  std::string("Incoming visual roots mismatch actual table for ") + profile->id);
            check(injury->type == animations.sequences[static_cast<std::size_t>(injury_root)].type &&
                  injury->loop == animations.sequences[static_cast<std::size_t>(injury_root)].loop &&
                  died->type == animations.sequences[static_cast<std::size_t>(death_root)].type &&
                  died->loop == animations.sequences[static_cast<std::size_t>(death_root)].loop,
                  std::string("Incoming root Type/Loop changed for ") + profile->id);
            check(bank.sequence_policies.at(injury_root).type == injury->type &&
                  bank.sequence_policies.at(injury_root).loop == injury->loop &&
                  bank.sequence_policies.at(death_root).type == died->type &&
                  bank.sequence_policies.at(death_root).loop == died->loop,
                  std::string("Incoming sequence policy lost source Type/Loop for ") + profile->id);
            check(injury->type == 2 && injury->loop == 0 && injury->phases.size() == 3 &&
                  died->type == 0 && died->loop == 0 && died->phases.size() == 1,
                  std::string("Unexpected authored incoming root shape for ") + profile->id);
            std::cout << profile->id << " table=" << expected_profile.table
                      << " Injured=" << injury_root << ':' << injury->name
                      << " Died=" << death_root << ':' << died->name << '\n';

            for (const auto* sequence : {injury, died}) {
                const auto& source = animations.sequences[static_cast<std::size_t>(sequence->id)];
                check(source.steps.size() == sequence->phases.size(),
                      std::string("Incoming source phase count differs for ") + profile->id);
                for (std::size_t i = 0; i < source.steps.size(); ++i) {
                    const auto& phase = sequence->phases[i];
                    const auto& step = source.steps[i];
                    const auto key = std::make_pair(sequence->state,
                                                    std::vector<std::size_t>{i});
                    const auto retained = bank.steps.find(key);
                    check(retained != bank.steps.end() && same_step(retained->second, step),
                          std::string("Full source AnimationStep changed for ") + profile->id);
                    const auto* uri = dh2::data::animation_clip(step, clips);
                    check(uri && phase.sourceUri == *uri && phase.animationId == step.anim &&
                          phase.speed == step.speed && phase.blendOut == step.blend_out &&
                          phase.moveGO == step.move_go && phase.redirect == step.redir &&
                          assets.resolve(phase.resolvedPath).has_filename(),
                          std::string("Incoming clip/rate/blend/MoveGO differs for ") + profile->id);
                }
            }

            CharacterVisual visual;
            check(visual.load(assets, bank.visual.config, error), error);
            for (std::size_t choice = 0; choice < injury->phases.size(); ++choice) {
                OriginalAttackSelection selection;
                check(make_runtime_player_injury_selection_v1(bank, choice, selection, error), error);
                check(selection.state == "Injured" && selection.variant == 0 &&
                      selection.choices.size() == 1 &&
                      selection.choices.at({}) == choice,
                      std::string("Caller-selected source injury choice changed for ") + profile->id);
                check_markers(bank.visual, bank.sequence_policies, selection, visual,
                              profile->id + " Injured choice " + std::to_string(choice));
            }
            OriginalAttackSelection death_selection;
            check(make_runtime_player_death_selection_v1(bank, death_selection, error), error);
            check(death_selection.state == "Died" && death_selection.variant == 0 &&
                  death_selection.choices.empty(),
                  std::string("Ordinary Died override applied stance/choice for ") + profile->id);
            check_markers(bank.visual, bank.sequence_policies, death_selection, visual,
                          profile->id + " Died");
            OriginalAttackSelection invalid;
            check(!make_runtime_player_injury_selection_v1(bank, injury->phases.size(),
                  invalid, error), "Out-of-range source injury choice was accepted");

            if (profile->id == "RoguePlayerBase") {
                check(profile->property_row == "325" && expected_profile.table == 50,
                      "Actual Rogue row325/table50 profile identity changed");
                const char* hurt[] = {"prince_hurt_01.bdae", "prince_hurt_02.bdae",
                                      "prince_hurt_03.bdae"};
                for (std::size_t i = 0; i < 3; ++i)
                    check(injury->phases[i].sourceUri.find(hurt[i]) != std::string::npos,
                          "Rogue injury choice used a non-Rogue source clip");
                check(died->phases[0].sourceUri.find("prince_dying_01.bdae") != std::string::npos,
                      "Rogue Died root used a non-Rogue source clip");
            }

            auto wrong_profile_visual = actor_visual;
            check(!profile->states.empty() && !profile->states.begin()->second.empty(),
                  "Source profile has no clip for the cross-bank rejection fixture");
            const auto& first_state = *profile->states.begin();
            const auto alias = first_state.first;
            auto configured = std::find_if(wrong_profile_visual.clips.begin(),
                wrong_profile_visual.clips.end(), [&](const auto& clip) {
                    return clip.first == alias;
                });
            check(configured != wrong_profile_visual.clips.end(),
                  "Source profile test visual omitted its authored state clip");
            configured->second = injury->phases.front().resolvedPath;
            RuntimePlayerIncomingAnimationBankV1 rejected;
            check(!build_runtime_player_incoming_animation_bank_v1(assets, *profile,
                  wrong_profile_visual, animations, clips, "wrong-profile", rejected, error),
                  "Incoming bank accepted an actor visual clip substituted from its incoming bank");
            if (profile->id == "RoguePlayerBase") {
                auto knight_table_fallback = *profile;
                knight_table_fallback.animation_table = "48";
                check(!build_runtime_player_incoming_animation_bank_v1(assets,
                      knight_table_fallback, actor_visual, animations, clips,
                      "forbidden-knight-fallback", rejected, error),
                      "Rogue profile accepted the Warrior/Knight CharAnimTable ID as fallback");
            }
        }

        std::cout << "PASS exact Warrior/Rogue/Mage incoming roots and clips; caller-selected Injured Type2 choices; ordinary no-stance Died Type0; full source step/marker data retained; no Knight fallback\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
