#include "../app/src/main/cpp/native_level_sg_update_v1.hpp"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace level = dh2::native::level_sg_update_v1;
namespace source = dh2::source_level_owner_v1;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

struct Fixture {
    bool in_game_view = false;
    std::vector<std::string> order;
    std::int32_t started_id = -9;
    std::int32_t started_argument = 0;
    bool started_flag = true;
};

bool game_view(void* raw, bool& value, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.emplace_back("view");
    value = fixture.in_game_view;
    error.clear();
    return true;
}

bool start_script(void* raw, std::uintptr_t, std::int32_t id,
                  std::int32_t argument, bool flag, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.emplace_back("start");
    fixture.started_id = id;
    fixture.started_argument = argument;
    fixture.started_flag = flag;
    error.clear();
    return true;
}

bool character_update(void* raw, std::uintptr_t, std::uintptr_t,
                      bool checkpoint, std::string& error) {
    auto& fixture = *static_cast<Fixture*>(raw);
    fixture.order.emplace_back(checkpoint ? "checkpoint" : "character");
    error.clear();
    return true;
}

bool execute_scripts(void* raw, std::uintptr_t, std::string& error) {
    static_cast<Fixture*>(raw)->order.emplace_back("scripts");
    error.clear();
    return true;
}
}

int main() {
    try {
        Fixture fixture;
        const level::ServicesV1 services{&fixture, game_view, start_script,
                                         character_update, execute_scripts};
        source::Snapshot source_level{};
        source_level.phase = source::Phase::active;
        source_level.source_level = UINT64_C(0x1001);
        source_level.player_character = UINT64_C(0x2002);
        source_level.pending_script_id = -1;
        source_level.pending_script_id_known = true;

        level::FrameV1 frame{};
        std::string error;
        require(level::frame_from_source_level_v1(source_level, true, frame, error),
                "known active SourceLevelOwner projection rejected");
        level::ResultV1 result{};
        require(level::dispatch_v1(frame, services, result, error),
                "sentinel Level frame dispatch failed");
        require(fixture.order == std::vector<std::string>{"character", "scripts"} &&
                result.character_update_called && result.script_manager_executed,
                "sentinel path must run Character then shared ScriptManager");

        fixture.order.clear();
        source_level.pending_script_id = 73;
        fixture.in_game_view = true;
        require(level::frame_from_source_level_v1(source_level, true, frame, error) &&
                level::dispatch_v1(frame, services, result, error),
                "non-sentinel Level frame dispatch failed");
        require(fixture.order == std::vector<std::string>{
                    "view", "start", "character", "scripts"} &&
                fixture.started_id == 73 && fixture.started_argument == -1 &&
                !fixture.started_flag,
                "pending script, Character SG_Update and ExecuteAllScripts order diverged");

        fixture.order.clear();
        source_level.pending_script_id_known = false;
        require(level::frame_from_source_level_v1(source_level, true, frame, error) &&
                !level::dispatch_v1(frame, services, result, error) &&
                error.find("source-owned Level+0x148") != std::string::npos &&
                fixture.order.empty(),
                "unknown borrowed Level field must fail closed without a fake sentinel");
        std::cout << "PASS native Level SG_Update frame order and source-field provenance\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
