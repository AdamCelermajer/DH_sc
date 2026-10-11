#include "../native_visual_fx_manager_v1.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace dh2::native_visual_fx_manager_v1;
namespace {
struct Fx { int id{}; bool live{}; };
struct StartedStep { std::uint32_t row{}; std::size_t step{}; int dict{}; bool grab{}; void* instance{}; };
struct StartedSetStep { std::uint32_t row{}; std::size_t step{}; int dict{}; const void* set{}; const void* parent{}; AnimFXSetPlayback playback{}; void* instance{}; };
struct Fake {
    std::array<Fx, 32> instances{};
    std::size_t created{};
    std::vector<std::string> calls;
    std::vector<StartedStep> started_steps;
    std::vector<StartedSetStep> started_set_steps;
    void* expected_null_source{};
    void* finish_instance{};
    void* callback_instance{};
    std::uint32_t random_choice{};
    std::vector<std::uint32_t> random_bounds;
};
bool create(void* raw, std::int32_t id, std::string_view path,
            void** out, std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (f.created == f.instances.size() || path.empty()) return false;
    auto& instance = f.instances[f.created++]; instance = {id, true}; *out = &instance;
    f.calls.emplace_back("create:" + std::to_string(id)); return true;
}
void destroy(void*, void*) {}
bool start(void* raw, void* instance, const EffectSequence& sequence,
           std::size_t step_index, bool grab_entry, void* source,
           std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (step_index >= sequence.steps.size() || source != f.expected_null_source) return false;
    f.started_steps.push_back({sequence.row, step_index,
        static_cast<Fx*>(instance)->id, grab_entry, instance});
    f.calls.emplace_back("start"); return true;
}
bool start_set_step(void* raw, void* instance, const EffectSequence& sequence,
                    const AnimFXSetPlayback& playback, std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (playback.step_index >= sequence.steps.size() || !playback.set_data_token ||
        playback.effect_table_id != static_cast<std::int32_t>(sequence.row)) return false;
    f.started_set_steps.push_back({sequence.row, playback.step_index,
        static_cast<Fx*>(instance)->id, playback.set_data_token,
        playback.parent_set_data_token, playback, instance});
    f.calls.emplace_back("set-start");
    return true;
}
bool reset(void* raw, void*, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back("reset"); return true;
}
bool bind_object(void* raw, void*, void*, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back("object"); return true;
}
bool bind_animator(void* raw, void*, void*, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back("animator"); return true;
}
bool sync(void* raw, void*, bool force, std::string&) {
    if (!force) return false;
    static_cast<Fake*>(raw)->calls.emplace_back("sync"); return true;
}
bool visible(void* raw, void*, bool value, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back(value ? "visible" : "hidden"); return true;
}
bool resume(void* raw, void*, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back("resume"); return true;
}
bool update(void* raw, void* instance, const EffectSequence& sequence,
            std::size_t step_index, bool& end_of_loop_callback_due,
            bool& source_hidden, std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (step_index >= sequence.steps.size()) return false;
    end_of_loop_callback_due = instance == f.callback_instance;
    if (end_of_loop_callback_due) f.callback_instance = nullptr;
    source_hidden = instance == f.finish_instance;
    if (source_hidden) f.finish_instance = nullptr;
    f.calls.emplace_back("update"); return true;
}
bool render(void* raw, void*, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back("render"); return true;
}
bool random_below(void* raw, std::uint32_t bound, std::uint32_t& value,
                  std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    f.random_bounds.push_back(bound);
    f.calls.emplace_back("random:" + std::to_string(bound));
    value = bound ? f.random_choice % bound : 0;
    return true;
}
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream input(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
bool require(bool condition, const char* label) {
    if (!condition) std::cerr << "FAIL: " << label << '\n';
    return condition;
}
}

int main() {
    const auto root = "port/android-native/app/src/main/assets/original-cache/data/pydata/";
    const auto records = read((std::string(root) + "effects_pyarray.bin").c_str());
    const auto names = read((std::string(root) + "effects_pyarraynames.bin").c_str());
    const auto fields = read((std::string(root) + "effects_pystructnames.bin").c_str());
    const auto dictionary = read((std::string(root) + "effects_dictionary_pyarray.bin").c_str());
    const auto dictionary_names = read((std::string(root) + "effects_dictionary_pyarraynames.bin").c_str());
    Catalog catalog; std::string error;
    bool ok = load_catalog({records.data(), records.size()}, {names.data(), names.size()},
        {fields.data(), fields.size()}, {dictionary.data(), dictionary.size()},
        {dictionary_names.data(), dictionary_names.size()}, catalog, error);
    if (!require(ok, error.c_str())) return 1;
    ok = require(catalog.sequences.size() == 276 && catalog.effect_paths.size() == 284 &&
        catalog.sequences[137].name == "quest_marker" &&
        catalog.effect_paths[static_cast<std::size_t>(catalog.sequences[137].steps[0].effect_dict_id)] ==
            "data/3D/interface/quest_marker.bdae" &&
        catalog.effect_array_consumed == 12700 && catalog.effect_array_suffix == 253 &&
        catalog.character_effects.size() == 3 &&
        catalog.character_effects[0].name == "Default" &&
        catalog.character_effects[0].blood_effect == 80 &&
        catalog.character_effects[0].blood_death_effect == -1 &&
        catalog.character_effects[1].blood_effect == 79 &&
        catalog.character_effects[2].name == "NoBlood" &&
        catalog.character_effects[2].blood_effect == -1 &&
        catalog.character_effect_table_consumed == 12755 &&
        catalog.footstep_effects.size() == 7 &&
        catalog.footstep_effects[0].name == "Default" &&
        catalog.footstep_effects[0].floor_type.empty() &&
        catalog.footstep_effects[4].name == "Water" &&
        catalog.footstep_effects[4].floor_type == "water" &&
        catalog.footstep_effects[4].effect == 275 &&
        catalog.footstep_effects[4].run_sound == std::vector<std::int32_t>{259} &&
        catalog.footstep_effects[6].floor_type == "wood" &&
        catalog.remaining_effect_array_suffix == 0,
        "source effect, CharEffect and material-specific FootstepEffect tables decode");
    if (!ok) return 1;

    Fake fake; Backend backend{&fake, create, destroy, start, reset, bind_object,
        bind_animator, sync, visible, resume, update, render};
    backend.random_below = random_below;
    backend.start_anim_fx_set_step = start_set_step;
    Owner owner(&catalog); owner.bind_backend(backend);
    if (!require(owner.footstep_effect("water") &&
        owner.footstep_effect("water")->effect == 275 &&
        owner.footstep_effect("Water") == nullptr,
        "FootstepEffect lookup uses exact source material string matching")) return 1;
    if (!require(owner.RegisterFXSetToLoad(137, error) == Status::ok &&
        owner.registered_effect_sets() == std::vector<std::int32_t>{137} &&
        owner.registered_effect_dicts() == std::vector<std::int32_t>{69} &&
        owner.pool(69).allocated == 0,
        "RegisterFXSetToLoad records a direct source EffectDict without creating a second pool")) return 1;
    std::array<void*, 6> grabbed{};
    for (auto& handle : grabbed)
        if (owner.GrabAnimFX(137, nullptr, &handle, error) != Status::ok) return 1;
    void* extra = nullptr;
    if (!require(owner.GrabAnimFX(137, nullptr, &extra, error) == Status::pool_exhausted,
                 "source per-dictionary allocation cap is six")) return 1;
    std::uintptr_t full_objective_slot = reinterpret_cast<std::uintptr_t>(grabbed[0]);
    if (!require(owner.InstallObjectiveMarker(&full_objective_slot, 137,
            reinterpret_cast<void*>(std::uintptr_t{0x1111}), nullptr, error) ==
                Status::pool_exhausted && full_objective_slot == 0,
            "source exhausted Grab overwrites old hidden Objective handle with null")) return 1;
    const auto recycled_instance = grabbed[0];
    if (!require(owner.DropAnimatedFX(&grabbed[0], error) == Status::ok && !grabbed[0],
                 "DropAnimatedFX releases active ownership to pool")) return 1;
    void* reused = nullptr;
    if (!require(owner.GrabAnimFX(137, nullptr, &reused, error) == Status::ok && reused == recycled_instance,
                 "free instance is reused")) return 1;

    // Release all but one slot so repeated marker installs can demonstrate
    // that Remove hides the old FX without returning it to the pool.
    if (owner.DropAnimatedFX(&grabbed[1], error) != Status::ok) return 1;
    for (std::size_t i = 2; i < grabbed.size(); ++i)
        if (owner.DropAnimatedFX(&grabbed[i], error) != Status::ok) return 1;
    if (owner.DropAnimatedFX(&reused, error) != Status::ok) return 1;
    std::uintptr_t objective_slot = 0;
    auto* character = reinterpret_cast<void*>(std::uintptr_t{0x1234});
    if (owner.InstallObjectiveMarker(&objective_slot, 137, character, nullptr, error) != Status::ok)
        return 1;
    const auto installed = objective_slot;
    const auto repeat_begin = fake.calls.size();
    if (owner.InstallObjectiveMarker(&objective_slot, 137, character, nullptr, error) != Status::ok ||
        objective_slot == installed) return 1;
    const auto repeated_calls = std::vector<std::string>(fake.calls.begin() + repeat_begin,
                                                          fake.calls.end());
    if (!require(repeated_calls == std::vector<std::string>{"object", "sync", "animator", "hidden",
        "start", "object", "sync", "animator", "visible", "resume"},
        "reinstall hides retained old FX before allocating and binding replacement")) return 1;
    const auto current = objective_slot;
    if (owner.RemoveObjectiveMarker(&objective_slot, character, error) != Status::ok ||
        objective_slot != current) return 1;
    const std::vector<std::string> tail(fake.calls.end() - 4, fake.calls.end());
    if (!require(tail == std::vector<std::string>{"object", "sync", "animator", "hidden"},
        "marker install ordering and retained remove handle")) return 1;
    if (owner.Update(error) != Status::ok || owner.Render(error) != Status::ok) return 1;
    fake.finish_instance = reinterpret_cast<void*>(current);
    if (owner.Update(error) != Status::ok || objective_slot != current) return 1;
    if (!require(owner.pool(69).allocated == 6 && owner.pool(69).active == 1 &&
        owner.pool(69).free == 5,
        "hidden marker is auto-dropped to the free pool while Objective retains its pointer")) return 1;
    const auto after_drop_calls = fake.calls.size();
    if (!require(owner.RemoveObjectiveMarker(&objective_slot, character, error) == Status::ok &&
        objective_slot == current,
        "source Remove accepts a retained allocated handle after manager auto-drop")) return 1;
    const auto post_drop_remove = std::vector<std::string>(fake.calls.begin() + after_drop_calls,
                                                            fake.calls.end());
    if (!require(post_drop_remove == std::vector<std::string>{"object", "sync", "animator", "hidden"},
        "post-drop source Remove still runs detach, sync, animator clear, and hide")) return 1;
    if (!require(owner.InstallObjectiveMarker(&objective_slot, 137, character, nullptr, error) ==
        Status::ok && objective_slot == current,
        "reinstall after auto-drop removes retained handle then reuses the same free allocation")) return 1;
    if (!require(owner.pool(69).allocated == 6 && owner.pool(69).active == 2 &&
        owner.pool(69).free == 4,
        "post-drop reinstall reactivates pooled allocation without creating a second pool")) return 1;
    std::uintptr_t forged_slot = static_cast<std::uintptr_t>(0xdeadbeef);
    if (!require(owner.RemoveObjectiveMarker(&forged_slot, character, error) == Status::stale_instance,
        "retained-handle support still rejects pointers outside the manager allocation set")) return 1;

    EffectSequence chain{};
    chain.row = static_cast<std::uint32_t>(catalog.sequences.size());
    chain.name = "host_source_order_two_step";
    chain.loop = 1;
    chain.type = 1;
    EffectStep first{}; first.effect_dict_id = 69;
    EffectStep second{}; second.effect_dict_id = 70;
    chain.steps = {first, second};
    catalog.sequences.push_back(chain);
    void* chain_first = nullptr;
    const auto started_before_chain = fake.started_steps.size();
    if (!require(owner.GrabAnimFX(static_cast<std::int32_t>(chain.row), nullptr,
            &chain_first, error) == Status::ok && chain_first,
        "source Grab starts a multi-step sequence at its first EffectDict pool")) return 1;
    if (!require(fake.started_steps.size() == started_before_chain + 1 &&
        fake.started_steps.back().row == chain.row && fake.started_steps.back().step == 0 &&
        fake.started_steps.back().dict == 69 && fake.started_steps.back().grab,
        "initial sequence step uses GrabAnimFX start mode and source first-step dictionary")) return 1;

    if (!require(owner.Update(error) == Status::ok &&
        fake.started_steps.size() == started_before_chain + 1 && owner.pool(70).allocated == 0,
        "Grab entry does not invent a step callback before the source reports its boundary")) return 1;
    fake.callback_instance = chain_first;
    if (owner.Update(error) != Status::ok || fake.started_steps.size() != started_before_chain + 2)
        return 1;
    const auto chain_second = fake.started_steps.back().instance;
    if (!require(fake.started_steps.back().row == chain.row && fake.started_steps.back().step == 1 &&
        fake.started_steps.back().dict == 70 && !fake.started_steps.back().grab &&
        owner.pool(70).allocated == 1 && owner.pool(70).active == 1,
        "end-of-loop callback advances to step 1 through its own shared EffectDict pool")) return 1;

    fake.finish_instance = chain_first;
    fake.callback_instance = chain_second;
    if (owner.Update(error) != Status::ok || fake.started_steps.size() != started_before_chain + 3)
        return 1;
    if (!require(fake.started_steps.back().row == chain.row && fake.started_steps.back().step == 0 &&
        fake.started_steps.back().dict == 69 && !fake.started_steps.back().grab &&
        owner.pool(70).active == 1 && owner.pool(69).active == 3,
        "final-step callback applies source sequence loop count and reuses the first dictionary pool")) return 1;
    fake.finish_instance = chain_second;
    if (owner.Update(error) != Status::ok || owner.pool(70).active != 0 ||
        owner.pool(70).free != 1) return 1;

    EffectSequence random_set{};
    random_set.row = static_cast<std::uint32_t>(catalog.sequences.size());
    random_set.name = "host_source_type_two_random_set";
    random_set.type = 2;
    EffectStep random_first{}; random_first.effect_dict_id = 71;
    EffectStep random_second{}; random_second.effect_dict_id = 72;
    random_set.steps = {random_first, random_second};
    catalog.sequences.push_back(random_set);
    fake.random_choice = 1;
    const auto type2_start = fake.started_steps.size();
    const auto type2_call_start = fake.calls.size();
    void* type2_instance = nullptr;
    if (!require(owner.GrabAnimFX(static_cast<std::int32_t>(random_set.row), nullptr,
            &type2_instance, error) == Status::ok && type2_instance,
        "type-2 Grab obtains the source step-zero EffectDict before selection")) return 1;
    const std::vector<std::string> type2_calls(fake.calls.begin() + type2_call_start,
                                                fake.calls.end());
    if (!require(type2_calls == std::vector<std::string>{"create:71", "random:2", "start"} &&
        fake.random_bounds.size() == 1 && fake.random_bounds.back() == 2 &&
        fake.started_steps.size() == type2_start + 1 &&
        fake.started_steps.back().row == random_set.row &&
        fake.started_steps.back().step == 1 && fake.started_steps.back().dict == 71 &&
        fake.started_steps.back().grab && owner.pool(71).allocated == 1 &&
        owner.pool(72).allocated == 0,
        "type-2 choice uses one source-bounded draw after step-zero pool allocation")) return 1;
    fake.callback_instance = type2_instance;
    const auto type2_progress_count = fake.started_steps.size();
    if (!require(owner.Update(error) == Status::ok &&
        fake.started_steps.size() == type2_progress_count && owner.pool(71).allocated == 1,
        "top-level type-2 end callback does not invent a sibling allocation")) return 1;

    EffectSequence nested_set{};
    nested_set.row = static_cast<std::uint32_t>(catalog.sequences.size());
    nested_set.name = "host_source_registered_child_set";
    EffectStep nested_direct{}; nested_direct.effect_dict_id = 73;
    nested_set.steps = {nested_direct};
    catalog.sequences.push_back(nested_set);
    EffectSequence registration_root{};
    registration_root.row = static_cast<std::uint32_t>(catalog.sequences.size());
    registration_root.name = "host_source_registered_root_set";
    EffectStep root_direct{}; root_direct.effect_dict_id = 72;
    EffectStep root_nested{}; root_nested.effect_dict_id = static_cast<std::int32_t>(nested_set.row);
    root_nested.redirect = 1;
    registration_root.steps = {root_direct, root_nested};
    catalog.sequences.push_back(registration_root);
    if (!require(owner.RegisterFXSetToLoad(static_cast<std::int32_t>(registration_root.row),
            error) == Status::ok &&
        owner.registered_effect_sets() == std::vector<std::int32_t>{137,
            static_cast<std::int32_t>(registration_root.row),
            static_cast<std::int32_t>(nested_set.row)} &&
        owner.registered_effect_dicts() == std::vector<std::int32_t>{69,72,73} &&
        owner.pool(72).allocated == 0 && owner.pool(73).allocated == 0,
        "RegisterFXSetToLoad recurses through Redir and records direct dictionaries only")) return 1;

    bool matched = true, launched = true;
    const std::array<float, 3> point{12.5f, -3.0f, 7.25f};
    if (!require(owner.PlayAnimFXSetByName("missing_source_set", point, nullptr,
            matched, launched, error) == Status::ok && !matched && !launched,
        "unmatched source event name is a distinct source no-op")) return 1;
    EffectSequence chained_root{};
    chained_root.row = static_cast<std::uint32_t>(catalog.sequences.size());
    chained_root.name = "host_anim_fx_set_parent";
    chained_root.loop = 1;
    chained_root.type = 1;
    EffectStep parent_child{};
    parent_child.effect_dict_id = static_cast<std::int32_t>(chained_root.row + 1);
    parent_child.redirect = 1;
    chained_root.steps = {parent_child, parent_child, parent_child};
    catalog.sequences.push_back(chained_root);
    EffectSequence chained_child{};
    chained_child.row = static_cast<std::uint32_t>(catalog.sequences.size());
    chained_child.name = "host_anim_fx_set_child";
    chained_child.loop = 2;
    chained_child.type = 2;
    EffectStep child_first{}; child_first.effect_dict_id = 82;
    EffectStep child_second{}; child_second.effect_dict_id = 83; child_second.loop = 3;
    child_second.orient_once = true; child_second.orient_with_anchor = true;
    child_second.self_illum = false; child_second.scale_with_anchor = false;
    child_second.speed = 1.25f; child_second.play_time = 400;
    chained_child.steps = {child_first, child_second};
    catalog.sequences.push_back(chained_child);
    fake.random_choice = 1;
    const auto set_start_begin = fake.started_set_steps.size();
    if (!require(owner.PlayAnimFXSetByName(chained_root.name, point, nullptr,
            matched, launched, error) == Status::ok && matched && launched &&
        fake.started_set_steps.size() == set_start_begin + 1,
        "PlayAnimFXSet starts a matching source set through the existing pool")) return 1;
    const auto parent_first_start = fake.started_set_steps.back();
    if (!require(parent_first_start.row == chained_child.row &&
        parent_first_start.step == 1 && parent_first_start.dict == 83 &&
        parent_first_start.parent != nullptr &&
        parent_first_start.playback.position == point &&
        parent_first_start.playback.loop_count == 6 &&
        parent_first_start.playback.orient_once &&
        parent_first_start.playback.orient_with_anchor &&
        !parent_first_start.playback.self_illum &&
        !parent_first_start.playback.scale_with_anchor &&
        parent_first_start.playback.play_time == 400 &&
        parent_first_start.playback.speed == 1.25f,
        "GetAnimFXData child fields, parent token, and explicit position reach backend")) return 1;
    if (!require(owner.Update(error) == Status::ok &&
        fake.started_set_steps.size() == set_start_begin + 1,
        "set progression waits for the actual end-of-loop callback")) return 1;
    fake.callback_instance = parent_first_start.instance;
    if (!require(owner.Update(error) == Status::ok &&
        fake.started_set_steps.size() == set_start_begin + 2,
        "nested type-2 callback advances its waiting type-1 parent")) return 1;
    const auto child_start = fake.started_set_steps.back();
    if (!require(child_start.row == chained_child.row && child_start.step == 1 &&
        child_start.dict == 83 && child_start.parent != nullptr &&
        child_start.set != child_start.parent &&
        child_start.playback.position == point && child_start.playback.loop_count == 6 &&
        fake.random_bounds.back() == 2,
        "nested set retains stable parent token, inherited position, and source loop multiplication")) return 1;
    fake.callback_instance = child_start.instance;
    if (!require(owner.Update(error) == Status::ok &&
        fake.started_set_steps.size() == set_start_begin + 3,
        "child completion advances to the next redirected parent step")) return 1;
    const auto parent_last_start = fake.started_set_steps.back();
    if (!require(parent_last_start.row == chained_child.row && parent_last_start.step == 1 &&
        parent_last_start.dict == 83 && parent_last_start.parent == parent_first_start.parent &&
        parent_last_start.set != parent_last_start.parent &&
        parent_last_start.playback.position == point,
        "parent resumes at its next source child using a new child SetData identity")) return 1;
    fake.callback_instance = parent_last_start.instance;
    if (!require(owner.Update(error) == Status::ok &&
        fake.started_set_steps.size() == set_start_begin + 4 &&
        fake.started_set_steps.back().row == chained_child.row &&
        fake.started_set_steps.back().step == 1 &&
        fake.started_set_steps.back().parent == parent_first_start.parent,
        "source set loop counter restarts the same retained SetData chain")) return 1;
    std::cout << "native_visual_fx_manager_v1 source catalog/pool/marker/update checks passed\n";
    return 0;
}
