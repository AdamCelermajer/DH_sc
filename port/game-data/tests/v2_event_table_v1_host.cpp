#include "v2_event_table_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace event = dh2::data::v2_event_table_v1;
namespace {
unsigned checks = 0;
void check(bool value, const char* what) {
    ++checks;
    if (!value) throw std::runtime_error(what);
}
std::vector<std::uint8_t> file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot open source event cache file: " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
event::Bytes view(const std::vector<std::uint8_t>& value) {
    return {value.data(), static_cast<std::uint32_t>(value.size())};
}
void write_word(std::vector<std::uint8_t>& bytes, std::size_t at, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i) bytes.at(at + i) = static_cast<std::uint8_t>(value >> (8 * i));
}
}

int main(int argc, char** argv) {
    try {
        if (argc != 2) return 2;
        const std::string root = argv[1];
        const auto packed = file(root + "/v2eventmanager_pyarray.bin");
        const auto names = file(root + "/v2eventmanager_pyarraynames.bin");
        const auto constants = file(root + "/v2eventmanager_pycst.bin");
        event::Table table;
        std::string error;
        check(event::load(view(packed), view(names), view(constants), table, error), "real cache import failed");
        check(error.empty(), "success returned an error");
        check(table.rows.size() == 66, "expected all 66 event rows");
        check(table.packed_bytes_consumed == packed.size() && packed.size() == 5082,
              "packed stream consumption mismatch");
        check(table.states.active == 1 && table.states.completed == 2 &&
              table.states.count == 3 && table.states.inactive == 0, "event-state constants mismatch");

        const auto* abbey = event::find(table, "ABBEY_body_a");
        check(abbey != nullptr, "ABBEY_body_a missing");
        check(abbey->level == 0 && abbey->script == "AbbeyScripts.AbbeyB_inspect_dead_body",
              "ABBEY_body_a header mismatch");
        check(abbey->triggers.size() == 1, "ABBEY_body_a trigger count mismatch");
        const auto& trigger = abbey->triggers.front();
        check(trigger.type == static_cast<std::int32_t>(event::ObjectiveType::talk_to_npc),
              "ABBEY_body_a trigger type is not TalkToNPC");
        check(trigger.op1 == -1 && trigger.op2 == -1 && trigger.text1.empty() && trigger.text2.empty(),
              "ABBEY_body_a trigger common fields mismatch");
        check(trigger.value1 == 440 && trigger.value2 == 0 && trigger.value3 == 1,
              "ABBEY_body_a trigger values mismatch");
        check(event::find(table, "not-an-event") == nullptr && event::find(table, nullptr) == nullptr,
              "event lookup miss semantics mismatch");

        unsigned talk = 0, move = 0, kills = 0;
        for (const auto& row : table.rows) for (const auto& item : row.triggers) {
            talk += item.type == static_cast<std::int32_t>(event::ObjectiveType::talk_to_npc);
            move += item.type == static_cast<std::int32_t>(event::ObjectiveType::move_in_zone);
            kills += item.type == static_cast<std::int32_t>(event::ObjectiveType::kill_x_enemies);
        }
        check(talk == 58 && move == 2 && kills == 6, "bounded trigger type census mismatch");

        const auto stable_rows = table.rows.size();
        auto trailing = packed;
        trailing.push_back(0);
        check(!event::load(view(trailing), view(names), view(constants), table, error),
              "trailing table byte accepted");
        check(table.rows.size() == stable_rows, "failed load changed published table");
        auto bad_type = packed;
        write_word(bad_type, 53, 14);
        check(!event::load(view(bad_type), view(names), view(constants), table, error),
              "unknown objective type accepted");
        auto bad_names = names;
        write_word(bad_names, 0, 65);
        check(!event::load(view(packed), view(bad_names), view(constants), table, error),
              "row/name count mismatch accepted");
        auto truncated = packed;
        truncated.pop_back();
        check(!event::load(view(truncated), view(names), view(constants), table, error),
              "truncated event table accepted");
        auto bad_constants = constants;
        bad_constants.back() = 1;
        check(!event::load(view(packed), view(names), view(bad_constants), table, error),
              "malformed event-state constants accepted");

        std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"rows\":%zu,\"packed_bytes_consumed\":%u,\"trigger_counts\":{\"TalkToNPC\":%u,\"MoveInZone\":%u,\"KillXEnemies\":%u}}\n",
                    checks, table.rows.size(), static_cast<unsigned>(packed.size()), talk, move, kills);
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}
