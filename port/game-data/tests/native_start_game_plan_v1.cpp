#include "../native_start_game_plan_v1.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh2::data;
namespace {
unsigned checks = 0;
void require(bool value, const char* message) {
    ++checks;
    if (!value) throw std::runtime_error(message);
}
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream input(path, std::ios::binary);
    require(bool(input), "cannot read actual LevelList fixture");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
Bytes bytes(const std::vector<std::uint8_t>& value) { return {value.data(), value.size()}; }
}

int main(int argc, char** argv) {
    try {
        require(argc == 4, "pass actual LevelList records/names/schema");
        const auto records = read(argv[1]);
        const auto names = read(argv[2]);
        const auto schema = read(argv[3]);
        LevelTables levels;
        std::string error;
        require(load_levels(bytes(records), bytes(names), bytes(schema), levels, error), error.c_str());
        require(levels.levels.size() == 51, "actual LevelList row count changed");
        require(levels.levels[23].name == "GOTHICUS_CRYPT_01" &&
                levels.levels[23].level_file == "007_crypt_01.rule.xml",
                "Crypt LevelList row changed");
        require(levels.levels[41].name == "SWAMP" &&
                levels.levels[41].level_file == "001_swamp.mlx",
                "new-profile LevelList row changed");

        NativeStartGameSaveViewV1 save;
        save.slot = 2;
        save.unlocked_difficulty = 2;
        save.level_rows = {41, 23, 23};
        save.entry_points = {0, 3, 7};
        save.use_spawn_points = {1, 0, 1};
        NativeStartGameRequestV1 request;
        NativeStartGamePlanV1 plan;

        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(plan.should_launch && plan.slot == 2 && plan.level_row == 41 &&
                plan.level_name == "SWAMP" && plan.level_file == "001_swamp.mlx",
                "fresh profile did not resolve original SWAMP row");
        require(plan.entry_point == 0 && plan.load_spawn_flag == 1 && plan.resume &&
                plan.requested_difficulty_for_load == 0 && plan.save_after_numeric_request == false &&
                plan.clear_saved_spawn_flag_before_load && plan.saved_spawn_flag_row_to_clear == 0 &&
                !plan.remotely_triggered && plan.seed == 0 && plan.synchronized_seed == 0,
                "offline default LoadLevel arguments changed");

        request.current_difficulty = 1;
        request.has_numeric_difficulty = true;
        request.requested_difficulty = 2;
        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(plan.difficulty_before_request == 1 && plan.difficulty_for_level == 2 &&
                plan.requested_difficulty_for_load == 2 && plan.level_row == 23 &&
                plan.entry_point == 7 && plan.load_spawn_flag == 1 &&
                plan.save_after_numeric_request && plan.saved_spawn_flag_row_to_clear == 2 &&
                plan.seed == 0 && plan.synchronized_seed == 0,
                "unlocked numeric difficulty resolution changed");

        const auto saved_seed = resolve_application_load_seed_v1(303, false, 909);
        require(saved_seed.ordinary_seed == 303 && saved_seed.synchronized_seed == 303,
                "Application::LoadLevel did not select saved seed for both Random channels");
        const auto zero_seed = resolve_application_load_seed_v1(0, false, 909);
        require(zero_seed.ordinary_seed == 909 && zero_seed.synchronized_seed == 909,
                "zero saved seed did not select timer fallback for both Random channels");
        const auto debug_seed = resolve_application_load_seed_v1(303, true, 909);
        require(debug_seed.ordinary_seed == 909 && debug_seed.synchronized_seed == 909,
                "DontUsePlayerSeed did not force timer fallback for both Random channels");

        save.unlocked_difficulty = 0;
        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(plan.difficulty_for_level == 1 && plan.requested_difficulty_for_load == 2 &&
                plan.level_row == 23 && plan.entry_point == 3 && plan.save_after_numeric_request,
                "locked request did not preserve current level difficulty and raw LoadLevel request");

        save.level_rows[1] = -1;
        request.initial_level_row = 41;
        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(plan.level_row == 41 && plan.level_file == "001_swamp.mlx",
                "DesignSettings fallback row was not used for saved -1");

        save.level_rows[1] = 23;
        request.current_difficulty = 0;
        request.has_numeric_difficulty = false;
        request.online = true;
        request.local_player_hosting = false;
        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(plan.level_row == 41 && plan.entry_point == 1 && plan.load_spawn_flag == 0,
                "online client entrypoint/spawn selection changed");
        request.local_player_hosting = true;
        save.entry_points[0] = 5;
        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(plan.entry_point == 5 && plan.load_spawn_flag == 0,
                "online host entrypoint/spawn selection changed");

        request.current_level_present = true;
        request.current_level_state = 38;
        save.slot = -1;
        require(resolve_native_start_game_plan_v1(levels, save, request, plan, error), error.c_str());
        require(!plan.should_launch && plan.slot == -1 && plan.level_row == -1 &&
                plan.level_file.empty() && !plan.resume,
                "already-started source state guard did not return an empty no-launch plan");

        request.current_level_present = false;
        NativeStartGamePlanV1 preserved;
        preserved.slot = 77;
        save.slot = 2;
        request.current_difficulty = 3;
        require(!resolve_native_start_game_plan_v1(levels, save, request, preserved, error) &&
                preserved.slot == 77, "invalid difficulty changed output");
        request.current_difficulty = 0;
        request.has_numeric_difficulty = true;
        request.requested_difficulty = -1;
        require(!resolve_native_start_game_plan_v1(levels, save, request, preserved, error) &&
                preserved.slot == 77, "negative difficulty changed output");
        request.requested_difficulty = 3;
        preserved.level_row = 12;
        preserved.save_after_numeric_request = false;
        require(!resolve_native_start_game_plan_v1(levels, save, request, preserved, error) &&
                preserved.slot == 77 && preserved.level_row == 12 &&
                !preserved.save_after_numeric_request,
                "out-of-range request crossed the plan's side-effect boundary");
        request.requested_difficulty = 0;
        preserved.level_row = -1;
        save.level_rows[0] = 999;
        require(!resolve_native_start_game_plan_v1(levels, save, request, preserved, error) &&
                preserved.slot == 77, "invalid LevelList row changed output");

        std::cout << "{\"validation\":\"PASS\",\"checks\":" << checks
                  << ",\"source_rows\":51,\"mismatches\":0}\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "NativeStartGame plan: " << failure.what() << '\n';
        return 1;
    }
}
