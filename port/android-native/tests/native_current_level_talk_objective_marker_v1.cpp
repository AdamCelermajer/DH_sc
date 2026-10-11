#include "../app/src/main/cpp/native_current_level_talk_objective_marker_v1.hpp"

#include <array>
#include <iostream>

namespace {
namespace fx = dh2::native_visual_fx_manager_v1;
namespace marker = dh2::native::current_level_talk_objective_marker_v1;
struct BackendFixture {
    std::array<int, 6> instances{};
    unsigned created{}, destroyed{}, hidden{};
    bool reject_create{};
};
bool create(void* raw, std::int32_t, std::string_view, void** instance,
            std::string& error) {
    auto& fixture = *static_cast<BackendFixture*>(raw);
    if (fixture.reject_create) { error = "resource load rejected"; return false; }
    if (fixture.created >= fixture.instances.size()) return false;
    *instance = &fixture.instances[fixture.created++];
    return true;
}
void destroy(void* raw, void*) { ++static_cast<BackendFixture*>(raw)->destroyed; }
bool start(void*, void*, const fx::EffectSequence&, void*, std::string&) { return true; }
bool reset(void*, void*, std::string&) { return true; }
bool bind(void*, void*, void*, std::string&) { return true; }
bool sync(void*, void*, bool, std::string&) { return true; }
bool visible(void* raw, void*, bool value, std::string&) {
    if (!value) ++static_cast<BackendFixture*>(raw)->hidden;
    return true;
}
fx::Backend backend(BackendFixture& fixture) {
    fx::Backend result{};
    result.context = &fixture;
    result.create = create; result.destroy = destroy;
    result.start_sequence = start; result.reset_for_pool = reset;
    result.bind_game_object = bind; result.bind_animator_character = bind;
    result.sync_irr_data = sync; result.set_visible = visible;
    result.resume_animator = reset;
    return result;
}
bool require(bool value, const char* message) {
    if (!value) std::cerr << message << '\n';
    return value;
}
}

int main() {
    BackendFixture fixture;
    fx::Catalog catalog;
    catalog.effect_paths.push_back("data/3D/interface/quest_marker.bdae");
    fx::EffectSequence sequence;
    sequence.row = 0;
    sequence.steps.emplace_back();
    sequence.steps.front().effect_dict_id = 0;
    catalog.sequences.push_back(sequence);
    marker::aggro::Character character{};
    std::string error;
    {
        fx::Owner owner(&catalog);
        owner.bind_backend(backend(fixture));
        std::array<std::uintptr_t, 6> slots{};
        for (auto& slot : slots)
            if (!require(marker::install_shared_fx_owner(&owner, 0, &character,
                    &slot, error) && slot && error.empty(), "initial marker allocation failed")) return 1;
        std::uintptr_t exhausted = 0;
        if (!require(marker::install_shared_fx_owner(&owner, 0, &character,
                &exhausted, error) && !exhausted && error.empty(),
                "source null Grab must not fail Quest compilation")) return 1;
        if (!require(marker::install_shared_fx_owner(&owner, 0, &character,
                &slots.front(), error) && !slots.front() && error.empty(),
                "reinstall must store null after hiding the previous marker")) return 1;
        if (!require(fixture.hidden == 1 && owner.pool(0).active == 6 &&
                owner.pool(0).allocated == 6, "reinstall eagerly dropped a source-owned FX")) return 1;
    }
    if (!require(fixture.destroyed == 6, "manager did not retire its six allocations")) return 1;
    fixture.reject_create = true;
    fx::Owner rejected(&catalog);
    rejected.bind_backend(backend(fixture));
    std::uintptr_t slot = 0;
    if (!require(!marker::install_shared_fx_owner(&rejected, 0, &character,
            &slot, error) && error == "resource load rejected" && !slot,
            "backend failure was incorrectly accepted as source pool exhaustion")) return 1;
    std::cout << "Talk marker source-null and backend-failure checks passed\n";
    return 0;
}
