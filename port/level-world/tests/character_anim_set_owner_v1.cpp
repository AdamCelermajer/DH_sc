#include "character_anim_set_owner_v1.hpp"
#include "character_faery_visual_facade_v1.hpp"

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace {
using namespace dh2;
struct EffectLog {
    std::vector<std::string> events;
    std::int32_t fail_sound{-1};
};
bool load_sound(void* opaque, std::int32_t id, std::string& error) {
    auto& log = *static_cast<EffectLog*>(opaque);
    log.events.push_back("sound:" + std::to_string(id));
    if (id == log.fail_sound) {
        error = "intentional sound provider failure";
        return false;
    }
    return true;
}
bool register_fx(void* opaque, std::int32_t id, std::string&) {
    static_cast<EffectLog*>(opaque)->events.push_back("fx:" + std::to_string(id));
    return true;
}

data::AnimationSequence sequence(std::int32_t anim, std::int32_t sound = -1,
                                 std::int32_t fx = -1, bool redirect = false) {
    data::AnimationSequence result;
    data::AnimationStep step;
    step.anim = anim;
    step.sound = sound;
    step.fx = fx;
    step.redir = redirect ? 1 : 0;
    result.steps.push_back(step);
    return result;
}
}

int main() {
    using namespace dh2::character_anim_set_owner_v1;
    data::AnimationTables animations;
    animations.sequences = {
        sequence(29),                 // Faery Template
        sequence(30, 4, 5),           // ordinary CharAnim member
        sequence(31),
        sequence(33),                 // stance member
        sequence(34),                 // Interact member
        sequence(35),                 // Spells member
        sequence(36),                 // fallback Skill.Anim
        sequence(32, 6, 7),           // redirected AnimTable row
    };
    animations.characters.resize(1);
    auto& row = animations.characters.front();
    row.fields[33] = {0}; // raw member 36: Template
    row.fields[20] = {1}; // raw member 22
    row.fields[9] = {3};  // raw member 10
    row.fields[15] = {4}; // raw members 16/17: Interact vector
    row.fields[31] = {5}; // raw members 33/34: Spells vector
    // Make the ordinary sequence redirect to a second AnimTable row.
    animations.sequences[1].steps.push_back(data::AnimationStep{});
    animations.sequences[1].steps.back().anim = 7;
    animations.sequences[1].steps.back().redir = 1;

    data::SkillTables skills;
    skills.skill_lists.resize(4);
    skills.skill_lists[3].members = {0};
    skills.skills.resize(1);
    skills.skills[0].anim = 6;

    character_faery_visual_facade_v1::CharacterVisual visual;
    visual.character = 0x1234;
    EffectLog effects_log;
    Effects effects{&effects_log, load_sound, register_fx};
    std::string error;
    assert(register_faery_anim_set(visual, visual.character, animations, skills,
                                   0, -1, effects, error) == Result::registered);
    assert(error.empty());
    assert(visual.anim_set.phase == Phase::ready);
    assert(visual.anim_set.anim_set_id == 3); // (CharAnimTable 0 << 8) | fallback SkillList 3
    assert(visual.anim_set.skill_list_id == 3);

    const std::vector<std::int32_t> expected_dictionary_ids{29, 30, 32, 33, 34, 35, 36};
    assert(visual.anim_set.animation_dictionary_ids == expected_dictionary_ids);
    const auto mapped = visual.anim_set.animation_dictionary_ids_for_sequence(1);
    assert((mapped == std::vector<std::int32_t>{30, 32}));
    assert((effects_log.events == std::vector<std::string>{
        "sound:4", "fx:5", "sound:6", "fx:7"}));
    assert(register_faery_anim_set(visual, visual.character, animations, skills,
                                   0, -1, effects, error) == Result::already_registered);
    assert(effects_log.events.size() == 4);
    assert(retire(visual, visual.character));
    assert(visual.anim_set.phase == Phase::empty);

    // A callback failure retains source-order partial registration and cannot
    // silently retry/replay already-applied effects.
    visual.anim_set = {};
    effects_log = {};
    effects_log.fail_sound = 6;
    assert(register_faery_anim_set(visual, visual.character, animations, skills,
                                   0, -1, effects, error) == Result::effect_failed);
    assert(visual.anim_set.phase == Phase::failed);
    assert((effects_log.events == std::vector<std::string>{
        "sound:4", "fx:5", "sound:6"}));
    const auto prior_count = visual.anim_set.registrations.size();
    assert(register_faery_anim_set(visual, visual.character, animations, skills,
                                   0, -1, effects, error) == Result::identity_mismatch);
    assert(visual.anim_set.registrations.size() == prior_count);
}
