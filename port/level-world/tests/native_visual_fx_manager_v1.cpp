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
struct Fake {
    std::array<Fx, 32> instances{};
    std::size_t created{};
    std::vector<std::string> calls;
    void* expected_null_source{};
    bool finish_next{};
};
bool create(void* raw, std::int32_t id, std::string_view path,
            void** out, std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (f.created == f.instances.size() || path.empty()) return false;
    auto& instance = f.instances[f.created++]; instance = {id, true}; *out = &instance;
    f.calls.emplace_back("create:" + std::to_string(id)); return true;
}
void destroy(void*, void*) {}
bool start(void* raw, void*, const EffectSequence& sequence, void* source,
           std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (sequence.row != 137 || source != f.expected_null_source) return false;
    f.calls.emplace_back("start"); return true;
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
bool update(void* raw, void*, const EffectSequence& sequence, bool& drop,
            std::string&) {
    auto& f = *static_cast<Fake*>(raw);
    if (sequence.row != 137) return false;
    drop = f.finish_next; f.finish_next = false;
    f.calls.emplace_back("update"); return true;
}
bool render(void* raw, void*, std::string&) {
    static_cast<Fake*>(raw)->calls.emplace_back("render"); return true;
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
        catalog.effect_array_consumed == 12700 && catalog.effect_array_suffix == 253,
        "source effect tables decode with marker ID and bounded suffix");
    if (!ok) return 1;

    Fake fake; Backend backend{&fake, create, destroy, start, reset, bind_object,
        bind_animator, sync, visible, resume, update, render};
    Owner owner(&catalog); owner.bind_backend(backend);
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
    fake.finish_next = true;
    if (owner.Update(error) != Status::ok || objective_slot != current) return 1;
    if (owner.pool(69).allocated != 6 || owner.pool(69).active != 1 ||
        owner.pool(69).free != 5) return 1;
    std::cout << "native_visual_fx_manager_v1 source catalog/pool/marker/update checks passed\n";
    return 0;
}
