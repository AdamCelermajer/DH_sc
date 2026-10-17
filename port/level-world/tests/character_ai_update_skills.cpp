#include "../character_ai_update_skills.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace update = dh2::character_ai_update_skills;
namespace data = dh2::data;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    std::vector<std::uintptr_t> skills;
    std::vector<std::uintptr_t> faeries;
    std::vector<std::pair<update::List, std::pair<std::uint32_t,
                                                  std::uintptr_t>>> calls;
    std::uint32_t vector_calls{};
    bool empty_vectors{};

    static std::int32_t vectors(void* raw, update::State* state,
                                update::List list, update::ScriptVector* out) {
        auto& self = *static_cast<Fixture*>(raw);
        if (!state || !out) return 1;
        ++self.vector_calls;
        const auto& values = list == update::List::skill
            ? self.skills : self.faeries;
        if (self.empty_vectors || values.empty()) {
            *out = {};
            return 0;
        }
        *out = {values.data(), values.data() + values.size()};
        return 0;
    }

    static std::int32_t on_update(void* raw, update::State* state,
                                  update::List list, std::uint32_t index,
                                  std::uintptr_t script) {
        auto& self = *static_cast<Fixture*>(raw);
        if (!state || state->ai != 0xA11 || state->owner != 0xB22 || !script)
            return 1;
        self.calls.push_back({list, {index, script}});
        return 0;
    }
};

bool save_slot_update(void*, std::uintptr_t, std::string&) { return true; }

void prepare_save(data::PlayerSavegameV1& save, std::string& error) {
    save.set_character(0xB22);
    require(save.initialize_skills_from_character_list({10, 20, 30}, error),
            "source skill rows did not initialize");
    const data::SavedSkillUpdateServicesV1 slot_updates{nullptr, save_slot_update};
    require(save.set_skill_in_slot(2, 1, slot_updates, error),
            "source skill slot2 did not initialize");
    require(save.set_skill_in_slot(0, 0, slot_updates, error),
            "source skill slot0 did not initialize");
    save.initialize_faeries();
    const std::array<std::uint8_t, 12> current_faeries{
        0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0};
    std::size_t consumed = 0;
    require(save.load_current_faery({current_faeries.data(),
                                    current_faeries.size()},
                                   consumed, error) && consumed == 12,
            "source current-faery payload did not load");
}
} // namespace

int main() {
    try {
        data::PlayerSavegameV1 save;
        std::string error;
        prepare_save(save, error);
        std::int32_t current_state = 5;
        std::int32_t difficulty = 1;
        update::State state{0xA11, 0xB22, &current_state, &save, &difficulty};
        Fixture fixture;
        fixture.skills = {0x101, 0, 0x303};
        fixture.faeries = {0x200, 0x201, 0x202};
        const update::Services services{&fixture, Fixture::vectors,
                                        Fixture::on_update};
        update::Result result{};

        require(update::update(&state, &services, &result) ==
                    update::Status::complete,
                "source UpdateSkills did not complete");
        require(result.decision == update::Decision::updated &&
                    result.saved_slots == 2 && result.script_updates == 3 &&
                    result.using_state_word == 5 &&
                    result.casting_state_word == 5 &&
                    result.faery_index == 1 && result.faery_updated == 1,
                "source gate/count/faery result differs");
        require(fixture.calls.size() == 3 &&
                    fixture.calls[0].first == update::List::skill &&
                    fixture.calls[0].second ==
                        std::pair<std::uint32_t, std::uintptr_t>{0, 0x101} &&
                    fixture.calls[1].first == update::List::skill &&
                    fixture.calls[1].second ==
                        std::pair<std::uint32_t, std::uintptr_t>{2, 0x303} &&
                    fixture.calls[2].first == update::List::faery &&
                    fixture.calls[2].second ==
                        std::pair<std::uint32_t, std::uintptr_t>{1, 0x201},
                "saved map key order or selected faery callback differs");

        current_state = 6;
        fixture.calls.clear();
        fixture.vector_calls = 0;
        require(update::update(&state, &services, &result) ==
                    update::Status::complete &&
                    result.decision ==
                        update::Decision::skipped_while_using_skill &&
                    result.using_state_word == 6 &&
                    fixture.calls.empty() && fixture.vector_calls == 0,
                "UsingSkill gate did not stop before Save/vector callbacks");

        current_state = 7;
        fixture.calls.clear();
        fixture.vector_calls = 0;
        require(update::update(&state, &services, &result) ==
                    update::Status::complete &&
                    result.decision == update::Decision::skipped_while_casting &&
                    result.using_state_word == 7 &&
                    result.casting_state_word == 7 &&
                    fixture.calls.empty() && fixture.vector_calls == 0,
                "Casting gate did not stop before Save/vector callbacks");

        current_state = 5;
        fixture.empty_vectors = true;
        fixture.calls.clear();
        fixture.vector_calls = 0;
        require(update::update(&state, &services, &result) ==
                    update::Status::complete &&
                    result.decision == update::Decision::updated &&
                    result.saved_slots == 2 && result.script_updates == 0 &&
                    fixture.calls.empty() && fixture.vector_calls == 3,
                "genuine pre-SetSkillsAndSpells empty vectors were not preserved");

        fixture.empty_vectors = false;
        const update::Services missing_callback{&fixture, Fixture::vectors,
                                                nullptr};
        require(update::update(&state, &missing_callback, &result) ==
                    update::Status::service_unavailable &&
                    result.saved_slots == 1 && result.script_updates == 0,
                "non-null skill callback silently succeeded without a provider");

        std::cout << "{\"validation\":\"PASS\",\"source\":\"CharAI::UpdateSkills@0x3d8a04\",\"skill_slots\":2,\"minimal_cases\":5,"
                     "\"using_and_casting_gates\":true,"
                     "\"selected_faery_uses_live_difficulty\":true,"
                     "\"pre_init_empty_vectors_preserved\":true,"
                     "\"missing_script_provider_fails\":true}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "character_ai_update_skills: " << error.what() << '\n';
        return 1;
    }
}
