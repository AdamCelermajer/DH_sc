#include "../character_ai_skill_commands_v1.hpp"
#include "../player_skill_use_session_v1.hpp"
#include "../character_stance.hpp"
#include "../character_skill_state_queries.hpp"
#define main retained_use_session_fixture_main
#include "player_skill_session_v1.cpp"
#undef main

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::character_ai_skill_commands_v1;
namespace u = dh2::player_skill_use_session_v1;
namespace p = dh2::character_player_skills_preparation_v3;

namespace {
struct TraceRow {
    std::uint32_t operation, index, value;
    std::int32_t signed_value;
    std::uintptr_t subject;
};

struct World {
    explicit World(k::State* value) : state(value) {}
    k::State* state = nullptr;
    u::Runtime* runtime = nullptr;
    Fixture* fixture = nullptr;
    const dh2::data::SkillRow* selected_row = nullptr;
    std::vector<TraceRow> trace;
    std::uint32_t usable = 0, active = 0, is_player = 0;
    std::int32_t property_value = 0;
    std::uint32_t constant_mask = 0;
    std::int32_t stance = 0;
    std::uintptr_t manager = 0x76540000;
    std::vector<const char*> trophy_names{"other", "epic_withskills"};
    std::uint32_t event_state = 6;
    int fail_operation = -1;
    unsigned row_calls = 0;
    k::SkillRow rows[3]{};
    std::int32_t animations[3]{};
    bool moving[3]{};
    std::int32_t types[3]{};
    std::int32_t* property_216 = nullptr;
    k::Character* trophy_replacement = nullptr;
    std::uintptr_t property_get_subject = 0;
};

void record(World& world, const k::Request& request) {
    world.trace.push_back({static_cast<std::uint32_t>(request.operation), request.index,
                           request.value, request.signed_value, request.subject});
}

std::int32_t invoke(void* raw, k::State* state, const k::Request* request,
                    k::Response* response) {
    auto& world = *static_cast<World*>(raw);
    if (state != world.state || !request || !response) return -1;
    record(world, *request);
    if (world.fail_operation == static_cast<int>(request->operation)) return -1;
    switch (request->operation) {
    case k::Operation::get_skill_row: {
        const unsigned slot = world.row_calls < 3 ? world.row_calls++ : 2;
        response->row = world.rows[slot];
        return 0;
    }
    case k::Operation::check_active:
        response->word = world.runtime ? 0 : world.active;
        if (world.runtime) {
            u::Result result{}; std::string error;
            const int status = world.runtime->check(p::source::List::skill, request->index,
                                                     u::Check::active, result, error);
            if (status) return -1;
            response->word = result.value;
        }
        return 0;
    case k::Operation::check_usable:
        response->word = world.runtime ? 0 : world.usable;
        if (world.runtime) {
            u::Result result{}; std::string error;
            const int status = world.runtime->check(p::source::List::skill, request->index,
                                                     u::Check::usable, result, error);
            if (status) return -1;
            response->word = result.value;
        }
        return 0;
    case k::Operation::pre:
        if (!world.runtime) { response->word = 0; return 0; }
        else {
            u::Result result{}; std::string error;
            if (world.runtime->invoke(p::source::List::skill, request->index,
                                      u::Callback::pre, result, error)) return -1;
            response->word = result.value;
            return 0;
        }
    case k::Operation::get_constant:
        response->word = world.constant_mask;
        return 0;
    case k::Operation::get_anim_stance:
        response->signed_word = world.stance;
        return 0;
    case k::Operation::raise_state_event:
    case k::Operation::set_state:
        if (!world.state || !world.state->owner_04 || !world.state->owner_04->current ||
            !world.state->owner_04->current->skill_machine_4fc ||
            !world.state->owner_04->current->skill_machine_4fc->state_query ||
            !world.state->owner_04->current->skill_machine_4fc->state_query->current_state_id)
            return -1;
        *const_cast<std::int32_t*>(world.state->owner_04->current->skill_machine_4fc->state_query->current_state_id) =
            static_cast<std::int32_t>(world.event_state);
        return 0;
    case k::Operation::is_player:
        response->word = world.is_player;
        return 0;
    case k::Operation::property_add_int:
        if (!world.property_216 || request->index != 216) return -1;
        *world.property_216 += static_cast<std::int32_t>(request->value);
        return 0;
    case k::Operation::trophy_manager:
        if (world.trophy_replacement) state->owner_04->current = world.trophy_replacement;
        response->identity = world.manager;
        return 0;
    case k::Operation::property_get_int:
        world.property_get_subject = request->subject;
        response->signed_word = static_cast<std::int32_t>(world.property_value);
        return 0;
    case k::Operation::is_local_player:
        response->word = 1;
        return 0;
    case k::Operation::trophy_names:
        response->trophy_names = {world.trophy_names.data(),
                                  static_cast<std::uint32_t>(world.trophy_names.size())};
        return 0;
    case k::Operation::unlock_trophy:
    case k::Operation::stop_skill_loop:
    case k::Operation::assertion_log:
        return 0;
    }
    return -1;
}

struct Synthetic {
    std::int32_t state_id = 0, animation = 0;
    std::uint32_t skill_index = 0;
    std::uint8_t moving = 0, continued = 1, last = 1;
    std::uint32_t current_slot = 99;
    dh2::character_skill_state_queries::Machine state_query{&state_id};
    k::Machine machine{0x400004fc, 0x40000000, &state_query,
                       &animation, &skill_index, &moving};
    k::Character character{0x40000000, &machine, 0x4000049c};
    k::OwnerSlot owner{&character};
    std::uintptr_t scripts[2]{0x50000000, 0x50000100};
    k::SkillVector vector{scripts, scripts + 2};
    k::Fields fields{&current_slot, &continued, &last};
    k::State state{0x60000000, &owner, &vector, &fields, 0, 0};
    World world{&state};
    k::Services services{&world, invoke};
    k::Result result{};

    Synthetic() {
        world.rows[0] = {0x70000000, &world.animations[0], &world.moving[0], &world.types[0]};
        world.rows[1] = {0x70000100, &world.animations[1], &world.moving[1], &world.types[1]};
        world.rows[2] = {0x70000200, &world.animations[2], &world.moving[2], &world.types[2]};
        world.animations[0] = 0x111; world.animations[1] = 0x222; world.animations[2] = 0x333;
        world.moving[0] = true; world.moving[1] = false; world.moving[2] = true;
        world.types[0] = 2; world.types[1] = 2; world.types[2] = 2;
        world.property_216 = &property_value;
        world.state = &state;
    }
    std::int32_t property_value = 0;
};

void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

void source_kernel_cases() {
    unsigned checks = 0, guards = 0, failures = 0;
    {
        Synthetic f; f.world.types[0] = 1; f.world.active = 0xdeadbeefu;
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 1 &&
                f.result.field_writes == 0 && f.world.trace.size() == 3 &&
                f.world.trace[0].operation == std::uint32_t(k::Operation::get_skill_row) &&
                f.world.trace[1].operation == std::uint32_t(k::Operation::check_active) &&
                f.world.trace[2].operation == std::uint32_t(k::Operation::pre),
                "active type-one branch must discard Pre return and return one"); ++checks;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 0;
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 0 &&
                f.result.field_writes == 0 && f.current_slot == 99 &&
                f.world.trace.size() == 2 &&
                f.world.trace[1].operation == std::uint32_t(k::Operation::check_usable) &&
                f.world.trace[1].index == 0 && f.world.trace[1].value == 0,
                "zero usable return must stop before source fields"); ++checks;
    }
    {
        Synthetic f; f.state_id = 6; f.current_slot = 0; f.world.active = 0xdeadbeefu;
        const auto status = k::is_skill_active(&f.state, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 1 &&
                f.result.service_calls == 0 && f.world.trace.empty(),
                "current FSM skill must short-circuit source Active check"); ++checks;
    }
    {
        Synthetic f; f.world.active = 0x80000002u;
        const auto status = k::is_skill_active(&f.state, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 0x80000002u &&
                f.result.service_calls == 1 && f.world.trace.size() == 1 &&
                f.world.trace[0].operation == std::uint32_t(k::Operation::check_active),
                "noncanonical script Active word was normalized"); ++checks;
    }
    {
        Synthetic f; f.state_id = 0; f.continued = 0; f.last = 0xa5;
        const auto status = k::execute(&f.state, k::Command::end, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 0 &&
                f.result.field_writes == 0 && f.last == 0xa5 && f.world.trace.empty(),
                "EndSkill without source UsingSkill must do nothing"); ++checks;
    }
    {
        Synthetic f; f.state_id = 6; f.continued = 0; f.last = 0xa5; f.world.types[0] = 2;
        const auto status = k::execute(&f.state, k::Command::end, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.last == 1 && f.result.field_writes == 1 &&
                f.world.trace.size() == 1 &&
                f.world.trace[0].operation == std::uint32_t(k::Operation::get_skill_row),
                "EndSkill continue=0 must set the source last flag"); ++checks;
    }
    {
        Synthetic f; f.state_id = 6; f.continued = 1; f.last = 0xa5; f.world.types[0] = 2;
        const auto status = k::execute(&f.state, k::Command::end, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.last == 0xa5 && f.result.field_writes == 0 &&
                f.world.trace.size() == 2 &&
                f.world.trace[1].operation == std::uint32_t(k::Operation::stop_skill_loop) &&
                f.world.trace[1].value == 1,
                "EndSkill continue=1 must stop the owner loop"); ++checks;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 0;
        const auto status = k::execute(&f.state, k::Command::use, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 0 &&
                f.world.trace.size() == 2 && f.world.trace[0].operation == 0 &&
                f.world.trace[1].operation == std::uint32_t(k::Operation::check_usable),
                "UseSkill must skip End when Begin returns false"); ++checks;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 0x80000001u;
        f.world.constant_mask = 0x200000u; f.world.stance = -7;
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.result.value == 1 &&
                f.result.field_writes == 6 && f.current_slot == 0 && !f.continued && !f.last &&
                f.animation == 0x222 - 7 && f.skill_index == 0 && f.moving &&
                f.result.begin_row == f.world.rows[0].identity &&
                f.result.setter_row == f.world.rows[1].identity,
                "source field writes, retained Begin row and fresh setter row differ"); ++checks;
        require(f.world.trace.size() == 7 &&
                f.world.trace[2].operation == std::uint32_t(k::Operation::get_skill_row) &&
                f.world.trace[3].operation == std::uint32_t(k::Operation::get_constant) &&
                f.world.trace[4].operation == std::uint32_t(k::Operation::get_anim_stance) &&
                f.world.trace[5].operation == std::uint32_t(k::Operation::raise_state_event) &&
                f.world.trace[6].operation == std::uint32_t(k::Operation::is_player),
                "setter/event/player/query source order differs"); ++checks;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 1;
        f.world.constant_mask = 0; f.world.is_player = 1;
        f.world.property_value = 198; f.world.property_216 = &f.world.property_value;
        f.world.property_value = 198;
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.world.property_value == 199 &&
                f.result.trophy_manager == f.world.manager && f.result.trophy_index == -1,
                "player trophy threshold must use fresh property value"); ++checks;
        require(f.world.trace.size() == 9 &&
                f.world.trace[5].operation == std::uint32_t(k::Operation::is_player) &&
                f.world.trace[6].operation == std::uint32_t(k::Operation::property_add_int) &&
                f.world.trace[7].operation == std::uint32_t(k::Operation::trophy_manager) &&
                f.world.trace[8].operation == std::uint32_t(k::Operation::property_get_int),
                "player property/trophy-manager/GetInt order differs"); ++checks;
    }
    {
        Synthetic f, replacement;
        replacement.character.identity = 0x41000000;
        replacement.machine.identity = 0x410004fc;
        replacement.machine.owner_04 = replacement.character.identity;
        replacement.character.stop_skill_loop_receiver_49c = 0x4100049c;
        f.world.usable = 1; f.world.is_player = 1;
        f.world.property_value = 198; f.world.property_216 = &f.world.property_value;
        f.world.trophy_replacement = &replacement.character;
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::complete && f.world.property_value == 199 &&
                f.world.property_get_subject == f.character.identity &&
                f.owner.current == &replacement.character && f.result.value == 0 &&
                f.result.trophy_manager == f.world.manager && f.world.trace.size() == 9,
                "GetInt must retain owner captured before trophy manager; final query reloads owner"); ++checks;
    }
    {
        Synthetic f; f.state.skill_vector_b4 = nullptr; f.world.types[0] = 2;
        f.state.assertion_level = 0;
        const auto status = k::is_skill_active(&f.state, 0, &f.services, &f.result);
        require(status == k::Status::invalid_argument, "active query without vector accepted"); ++guards;
    }
    {
        Synthetic f; k::Result* alias = reinterpret_cast<k::Result*>(&f.fields);
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, alias);
        require(status == k::Status::invalid_argument && f.current_slot == 99,
                "overlapping output changed source controls"); ++guards;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 7;
        f.world.fail_operation = static_cast<int>(k::Operation::get_constant);
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::service_failed && f.current_slot == 0 &&
                !f.continued && !f.last && f.result.field_writes == 3,
                "setter failure must retain earlier source field writes"); ++failures;
    }
    {
        Synthetic f; f.world.types[0] = 2;
        f.world.fail_operation = static_cast<int>(k::Operation::check_usable);
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::service_failed && f.current_slot == 99 &&
                f.continued && f.last && f.result.field_writes == 0,
                "failed usable provider changed source skill fields"); ++failures;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 1;
        f.world.fail_operation = static_cast<int>(k::Operation::raise_state_event);
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::service_failed && f.current_slot == 0 &&
                !f.continued && !f.last && f.result.field_writes == 6,
                "event failure rolled back source AI and machine writes"); ++failures;
    }
    {
        Synthetic f; f.world.types[0] = 2; f.world.usable = 1;
        f.world.fail_operation = static_cast<int>(k::Operation::is_player);
        const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
        require(status == k::Status::service_failed && f.current_slot == 0 &&
                f.result.field_writes == 6,
                "IsPlayer failure erased completed skill-state effects"); ++failures;
    }
    std::cout << "{\"validation\":\"PASS\",\"source_cases\":" << checks
              << ",\"guards\":" << guards << ",\"failure_prefixes\":" << failures << "}\n";
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc == 5 && std::string(argv[1]) == "--model-begin") {
            Synthetic f; f.world.types[0] = 2;
            f.world.usable = static_cast<std::uint32_t>(std::stoul(argv[2], nullptr, 0));
            f.world.constant_mask = static_cast<std::uint32_t>(std::stoul(argv[3], nullptr, 0));
            f.world.is_player = static_cast<std::uint32_t>(std::stoul(argv[4], nullptr, 0));
            const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
            std::cout << "{\"status\":" << int(status) << ",\"value\":" << f.result.value
                      << ",\"slot\":" << f.current_slot << ",\"continued\":" << unsigned(f.continued)
                      << ",\"last\":" << unsigned(f.last) << ",\"writes\":" << f.result.field_writes
                      << ",\"animation\":" << f.animation << ",\"skill\":" << f.skill_index
                      << ",\"moving\":" << unsigned(f.moving) << ",\"state\":" << f.state_id
                      << ",\"trace\":[";
            for (std::size_t i=0; i<f.world.trace.size(); ++i) {
                if (i) std::cout << ',';
                std::cout << f.world.trace[i].operation;
            }
            std::cout << "]}\n";
            return 0;
        }
        if (argc == 5 && std::string(argv[1]) == "--model-active") {
            Synthetic f;
            f.state_id = static_cast<std::int32_t>(std::stol(argv[2], nullptr, 0));
            f.current_slot = static_cast<std::uint32_t>(std::stoul(argv[3], nullptr, 0));
            f.world.active = static_cast<std::uint32_t>(std::stoul(argv[4], nullptr, 0));
            const auto status = k::is_skill_active(&f.state, 0, &f.services, &f.result);
            std::cout << "{\"status\":" << int(status) << ",\"value\":" << f.result.value
                      << ",\"service_calls\":" << f.result.service_calls << ",\"trace\":[";
            for (std::size_t i=0; i<f.world.trace.size(); ++i) {
                if (i) std::cout << ',';
                std::cout << f.world.trace[i].operation;
            }
            std::cout << "]}\n";
            return 0;
        }
        if (argc == 3 && std::string(argv[1]) == "--model-use") {
            Synthetic f; f.world.types[0] = 2;
            f.world.usable = static_cast<std::uint32_t>(std::stoul(argv[2], nullptr, 0));
            const auto status = k::execute(&f.state, k::Command::use, 0, &f.services, &f.result);
            std::cout << "{\"status\":" << int(status) << ",\"value\":" << f.result.value
                      << ",\"slot\":" << f.current_slot << ",\"continued\":" << unsigned(f.continued)
                      << ",\"last\":" << unsigned(f.last) << ",\"writes\":" << f.result.field_writes
                      << ",\"trace\":[";
            for (std::size_t i=0; i<f.world.trace.size(); ++i) {
                if (i) std::cout << ',';
                std::cout << f.world.trace[i].operation;
            }
            std::cout << "]}\n";
            return 0;
        }
        if (argc == 6 && std::string(argv[1]) == "--model-end") {
            Synthetic f; f.state_id = static_cast<std::int32_t>(std::stol(argv[2], nullptr, 0));
            f.continued = static_cast<std::uint8_t>(std::stoul(argv[3], nullptr, 0));
            f.world.types[0] = static_cast<std::int32_t>(std::stol(argv[4], nullptr, 0));
            f.last = static_cast<std::uint8_t>(std::stoul(argv[5], nullptr, 0));
            const auto status = k::execute(&f.state, k::Command::end, 0, &f.services, &f.result);
            std::cout << "{\"status\":" << int(status) << ",\"value\":" << f.result.value
                      << ",\"last\":" << unsigned(f.last) << ",\"writes\":" << f.result.field_writes
                      << ",\"service_calls\":" << f.result.service_calls << ",\"trace\":[";
            for (std::size_t i=0; i<f.world.trace.size(); ++i) {
                if (i) std::cout << ',';
                std::cout << f.world.trace[i].operation;
            }
            std::cout << "]}\n";
            return 0;
        }
        if (argc == 2 && std::string(argv[1]) == "--model-owner-capture") {
            Synthetic f, replacement;
            replacement.character.identity = 0x41000000;
            replacement.machine.identity = 0x410004fc;
            replacement.machine.owner_04 = replacement.character.identity;
            replacement.character.stop_skill_loop_receiver_49c = 0x4100049c;
            f.world.usable = 1; f.world.is_player = 1;
            f.world.property_value = 198; f.world.property_216 = &f.world.property_value;
            f.world.trophy_replacement = &replacement.character;
            const auto status = k::execute(&f.state, k::Command::begin, 0, &f.services, &f.result);
            std::cout << "{\"status\":" << int(status) << ",\"value\":" << f.result.value
                      << ",\"property_value\":" << f.world.property_value
                      << ",\"get_owner\":" << (f.world.property_get_subject == f.character.identity ? 0 : 1)
                      << ",\"owner_after\":" << (f.owner.current == &replacement.character ? 1 : 0)
                      << ",\"manager\":" << f.result.trophy_manager << ",\"trace\":[";
            for (std::size_t i=0; i<f.world.trace.size(); ++i) {
                if (i) std::cout << ',';
                std::cout << f.world.trace[i].operation;
            }
            std::cout << "]}\n";
            return 0;
        }
        if (argc == 6 && std::string(argv[1]) == "--model-setter") {
            Synthetic f;
            f.world.constant_mask = static_cast<std::uint32_t>(std::stoul(argv[2], nullptr, 0));
            f.world.stance = static_cast<std::int32_t>(std::stol(argv[3], nullptr, 0));
            const auto force = std::stoul(argv[4], nullptr, 0) != 0;
            const auto moving = static_cast<std::uint8_t>(std::stoul(argv[5], nullptr, 0));
            const auto status = k::set_skill_state(&f.state, &f.machine, 0, moving,
                                                   0x76543210, force, &f.services, &f.result);
            std::cout << "{\"status\":" << int(status) << ",\"animation\":" << f.animation
                      << ",\"skill\":" << f.skill_index << ",\"moving\":" << unsigned(f.moving)
                      << ",\"state\":" << f.state_id << ",\"writes\":" << f.result.field_writes
                      << ",\"trace\":[";
            for (std::size_t i=0; i<f.world.trace.size(); ++i) {
                if (i) std::cout << ',';
                std::cout << f.world.trace[i].operation;
            }
            std::cout << "]}\n";
            return 0;
        }
        if (argc == 2 && std::string(argv[1]) == "--source") {
            source_kernel_cases();
            return 0;
        }
        if (argc == 3 && std::string(argv[1]) == "--real-vm") {
            Catalogue catalogue(argv[2]);
            const auto temp = std::filesystem::temp_directory_path() / "dh2-skill-commands-real-vm";
            Fixture fixture(argv[2], temp, catalogue, "KnightPlayerBase");
            fixture.common();
            p::source::Result prepared{};
            require(fixture.owner->prepare(&prepared) == p::source::Status::complete,
                    "real selected Player skill preparation failed");
            u::Runtime runtime(*fixture.session, *fixture.owner, CHAR);
            const auto raw_tree = fixture.view.resolved[28];
            const auto& slots = fixture.owner->slots(p::source::List::skill);
            k::SkillVector vector{slots.data(), slots.data() + slots.size()};
            std::int32_t current_state = 0, animation = 0;
            std::uint32_t current_slot = 0xffffffffu, skill_index = 0;
            std::uint8_t moving = 0, continued = 1, last = 1;
            dh2::character_skill_state_queries::Machine query{&current_state};
            k::Machine machine{0x100004fc, CHAR, &query, &animation, &skill_index, &moving};
            k::Character character{CHAR, &machine, 0x1000049c};
            k::OwnerSlot owner{&character};
            k::Fields fields{&current_slot, &continued, &last};
            k::State state{AIS, &owner, &vector, &fields, 0, 0};
            World world{&state}; world.runtime = &runtime; world.fixture = &fixture;
            world.property_216 = &fixture.view.resolved[216];
            for (std::size_t i=0; i<slots.size(); ++i) {
                const auto* row = catalogue.tables->skill(raw_tree, static_cast<std::uint32_t>(i));
                if (!row) continue;
                world.animations[0] = row->anim;
                world.moving[0] = row->anim_is_moving;
                world.types[0] = row->type;
                world.rows[0] = {reinterpret_cast<std::uintptr_t>(row), &world.animations[0],
                                 &world.moving[0], &world.types[0]};
            }
            // The authored passive at slot 7 is a real selected VM callback;
            // its Usable result is false, so the source must stop pre-mutation.
            const auto* passive = catalogue.tables->skill(raw_tree, 7);
            require(passive && passive->type != 1, "expected Knight slot 7 passive changed");
            world.types[0] = passive->type;
            world.rows[0] = {reinterpret_cast<std::uintptr_t>(passive), &world.animations[0],
                             &world.moving[0], &world.types[0]};
            k::Services services{&world, invoke}; k::Result result{};
            auto status = k::execute(&state, k::Command::begin, 7, &services, &result);
            require(status == k::Status::complete && result.value == 0 &&
                    result.field_writes == 0 && result.service_calls == 2,
                    "real Lua/FSM BeginSkill passive gate differs");
            current_state = 0; current_slot = 0xffffffffu;
            status = k::is_skill_active(&state, 7, &services, &result);
            require(status == k::Status::complete && result.value == 1 && result.service_calls == 1,
                    "real OnSkillCheck_Active was not composed after FSM query");
            std::cout << "{\"validation\":\"PASS\",\"real_vm_begin_usable_false\":true,"
                         "\"real_vm_active_check\":true,\"same_vm\":true,"
                         "\"native_activation\":false,\"loaded_script_paths\":[";
            bool first = true;
            for (const auto& path : fixture.loaded_paths) {
                if (!first) std::cout << ',';
                first = false;
                std::cout << '\"' << path << '\"';
            }
            std::cout << "]}\n";
            fixture.session.reset();
            return 0;
        }
        source_kernel_cases();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
