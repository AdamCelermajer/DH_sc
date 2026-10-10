#include "runtime_skill_cast_prepare_v1.hpp"

#include "../platform_input/semantic_input.hpp"
#include "../../character_state.hpp"

#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;
using namespace dh::foundation::platform_input;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

bool same_slots(const std::vector<SkillSlotBinding>& left,
                const std::vector<SkillSlotBinding>& right) {
    if (left.size() != right.size()) return false;
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (left[i].equipment_set != right[i].equipment_set ||
            left[i].slot != right[i].slot ||
            left[i].saved_skill_row != right[i].saved_skill_row) return false;
    }
    return true;
}
}

int main() {
    try {
        const std::array<std::uint32_t, 3> authored_source_order{{2, 0, 1}};
        Bindings bindings;
        SemanticInput input(bindings);
        CharacterState saved;
        saved.skill_slots = {{0, 0, 4}, {0, 1, 1}, {0, 2, 0}};
        const auto original_saved_rows = saved.skill_slots;
        std::string error;

        for (std::size_t physical_key_index = 0; physical_key_index < 3;
             ++physical_key_index) {
            const int key_code = bindings.skills[physical_key_index];
            input.key(key_code, true);
            const auto frame = input.take_frame();
            require(frame.skills[physical_key_index].pressed &&
                    frame.skills[physical_key_index].held,
                    "Physical PC skill key did not produce its matching semantic key edge");
            for (std::size_t other = 0; other < frame.skills.size(); ++other) {
                if (other != physical_key_index)
                    require(!frame.skills[other].pressed,
                            "One physical key fabricated a second semantic skill edge");
            }

            // Production performs this once, from the pressed Frame.skills[i]
            // edge. The conversion addresses source slots; it never edits Save rows.
            std::uint32_t source_slot = UINT32_MAX;
            require(pc_skill_number_to_source_slot_v1(
                        static_cast<unsigned>(physical_key_index + 1), source_slot, error),
                    error.c_str());
            require(source_slot == authored_source_order[physical_key_index],
                    "PC key number no longer follows authored mapping-circle order [2,0,1]");
            require(same_slots(saved.skill_slots, original_saved_rows),
                    "PC input translation renumbered persisted skill-slot bindings");

            input.key(key_code, false);
            const auto released = input.take_frame();
            require(released.skills[physical_key_index].released,
                    "Physical PC skill key release edge was not preserved");
        }

        // The PC presentation conversion must not leak into source-native
        // NativeHUDSkill slot arguments, which remain logical 0/1/2.
        for (std::uint32_t native_slot = 0; native_slot < 3; ++native_slot)
            require(native_slot == std::array<std::uint32_t, 3>{{0, 1, 2}}[native_slot],
                    "Native skill slot identity changed from logical 0/1/2");
        require(same_slots(saved.skill_slots, original_saved_rows),
                "Semantic input adaptation changed the saved CharacterState rows");
        std::cout << "pc_skill_input_binding_v1_tests PASS: physical keys1/2/3 -> "
                     "source slots2/0/1 once; saved rows and native slots unchanged\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
