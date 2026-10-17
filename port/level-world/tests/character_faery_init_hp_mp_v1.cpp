#include "../character_faery_init_hp_mp_v1.hpp"
#include "../character_runtime_factory_v1.hpp"

#include <array>
#include <cassert>
#include <string>
#include <vector>

namespace faery_hp_mp = dh2::character_faery_init_hp_mp_v1;
namespace level = dh2::character_level_runtime;
namespace factory = dh2::character_runtime_factory_v1;
namespace ctor = dh2::character_constructor_owner_v1;
namespace manager = dh2::object_manager_runtime_owner_v1;
namespace aggro = dh2::character::aggro_search;
namespace data = dh2::data;

struct Fixture {
    std::array<int, factory::component_count> components{};
    std::vector<aggro::Character*> roster;
};

int component(void* raw, ctor::Component id, ctor::Identity,
              factory::ComponentStorage::Slot* slot, std::string&) {
    auto& f = *static_cast<Fixture*>(raw);
    slot->canonical_owner = &f.components[static_cast<std::size_t>(id)];
    return 0;
}
int associate(void*, ctor::Association, ctor::Identity,
              const factory::ComponentStorage&, std::string&) { return 0; }
int register_state(void*, ctor::Identity, std::uint32_t, std::string&) { return 0; }
void rollback(void*, ctor::Action, std::uint32_t, ctor::Identity,
              factory::ComponentStorage&) noexcept {}
int enroll(void* raw, aggro::Character* character, bool duplicate,
           bool* appended) {
    if (!character || duplicate || !appended) return 1;
    static_cast<Fixture*>(raw)->roster.push_back(character);
    *appended = true;
    return 0;
}
int remove_character(void* raw, aggro::Character* character,
                     std::size_t* removed) {
    if (!removed) return 1;
    auto& roster = static_cast<Fixture*>(raw)->roster;
    *removed = 0;
    for (auto it = roster.begin(); it != roster.end();) {
        if (*it == character) { it = roster.erase(it); ++*removed; }
        else ++it;
    }
    return 0;
}

int main() {
    manager::Owner object_manager;
    Fixture fixture;
    factory::Owner characters(object_manager,
        {&fixture, enroll, remove_character});
    const factory::Services constructor_services{&fixture, component,
        associate, register_state, rollback};
    factory::Record* record = nullptr;
    factory::Result factory_result{};
    std::string error;
    assert(characters.create(0x41, {}, {}, constructor_services, &record,
        &factory_result, error) == factory::Status::complete && record);
    assert(characters.mark_source_properties_ready(0x41, &factory_result,
        error) == factory::Status::complete);

    data::PropertyRules rules{};
    data::PropertyState properties{};
    rules.defaults.fill(0);
    rules.types.fill(8); // runtime-only values live in the resolved sheet
    auto view = data::property_view(rules, properties);
    properties.resolved[36] = properties.resolved[38] = 8192;
    properties.resolved[41] = properties.resolved[43] = 4096;
    const auto storage = level::Storage{
        record->constructor.identity,
        reinterpret_cast<std::uintptr_t>(&properties),
        properties.base.data(), &view, nullptr, 0, nullptr, 0, nullptr, nullptr};

    level::Result result{};
    assert(faery_hp_mp::initialize(*record, storage, &result, error) ==
        faery_hp_mp::Status::complete);
    assert(result.init.captured_character == record->constructor.identity &&
        result.init.calls == 2 && result.init.hp_completed &&
        result.init.mp_completed && result.hp.current == 8192 &&
        result.hp.maximum == 8192 && result.mp.current == 4096 &&
        result.mp.maximum == 4096 && record->faery_init_hp_mp &&
        record->faery_init_hp_mp->complete());

    // A later source Revive/CharAI initialization legitimately calls
    // _InitHpMp again. Change a live maximum so the returned result proves
    // the retained Owner reached the runtime instead of replaying a cache.
    properties.resolved[38] = 4096;
    level::Result repeated{};
    assert(faery_hp_mp::initialize(*record, storage, &repeated, error) ==
        faery_hp_mp::Status::complete);
    assert(repeated.init.calls == 2 && repeated.hp.maximum == 4096 &&
        repeated.mp.maximum == 4096 && properties.resolved[36] == 8192);

    properties.resolved[36] = 0;
    properties.resolved[38] = 8192;
    level::Result failed{};
    assert(faery_hp_mp::initialize(*record, storage, &failed, error) ==
        faery_hp_mp::Status::provider_failed);
    assert(failed.hp_attempted && !failed.mp_attempted &&
        properties.resolved[36] == 0);
    level::Result uncertain_retry{};
    assert(faery_hp_mp::initialize(*record, storage, &uncertain_retry, error) ==
        faery_hp_mp::Status::prior_failure);
    assert(uncertain_retry.hp_attempted && !uncertain_retry.mp_attempted &&
        properties.resolved[36] == 0);

    auto other_view = view;
    auto changed_storage = storage;
    changed_storage.view = &other_view;
    assert(faery_hp_mp::initialize(*record, changed_storage, nullptr, error) ==
        faery_hp_mp::Status::owner_mismatch);

    assert(characters.retire(0x41, &factory_result, error) ==
        factory::Status::complete);
}
