#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../player_level_states_save_writer_v1.hpp"
#include "../level_tables.hpp"
#include "../world_map_tables.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace writer = dh2::data::player_level_states_save_writer_v1;
using namespace dh2::data;
using Raw = std::vector<std::uint8_t>;

namespace {
unsigned checks = 0;
const char* stage = "startup";

void require(bool value) {
    ++checks;
    if (!value)
        throw std::runtime_error(std::string("LVLS writer check ") +
                                 std::to_string(checks) + " at " + stage);
}

void put_word(Raw& out, std::int32_t value) {
    const auto bits = static_cast<std::uint32_t>(value);
    for (unsigned shift = 0; shift < 32; shift += 8)
        out.push_back(static_cast<std::uint8_t>(bits >> shift));
}

void put_string(Raw& out, const std::string& value) {
    const auto size = std::string(value.c_str()).size();
    put_word(out, static_cast<std::int32_t>(size + 1));
    out.insert(out.end(), value.c_str(), value.c_str() + size + 1);
}

struct Fixture {
    LevelTables levels;
    WorldMapTables world_map;
    PlayerSavegameV1 save;

    Fixture() {
        levels.levels.resize(2);
        levels.levels[0].name = "Crypt_Entrance";
        levels.levels[1].name = "Thamos_Catacombs";
        world_map.locations.resize(2);
        world_map.locations[0].name = "Thamos_Catacombs";
        world_map.locations[1].name = "Royal_Castle";
        for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
            auto* lvls = save.source_level_states(difficulty);
            lvls->count = 2;
            lvls->words = static_cast<std::int32_t*>(std::malloc(2 * sizeof(std::int32_t)));
            lvls->memory.release = [](void*, void* p) { std::free(p); };
            lvls->words[0] = static_cast<std::int32_t>(10 + difficulty);
            lvls->words[1] = static_cast<std::int32_t>(20 + difficulty);

            auto* maps = save.source_world_map_states(difficulty);
            maps->count = 2;
            maps->words = static_cast<std::int32_t*>(std::malloc(2 * sizeof(std::int32_t)));
            maps->memory.release = [](void*, void* p) { std::free(p); };
            maps->words[0] = static_cast<std::int32_t>(30 + difficulty);
            maps->words[1] = static_cast<std::int32_t>(40 + difficulty);
        }
    }

    Raw expected() const {
        Raw out;
        for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
            put_word(out, 2);
            for (std::uint32_t row = 0; row < 2; ++row) {
                put_string(out, levels.levels[row].name);
                put_word(out, save.source_level_states(difficulty)->words[row]);
            }
        }
        for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
            put_word(out, 2);
            for (std::uint32_t row = 0; row < 2; ++row) {
                put_string(out, world_map.locations[row].name);
                put_word(out, save.source_world_map_states(difficulty)->words[row]);
            }
        }
        return out;
    }
};

struct Sink {
    Raw bytes;
    std::vector<std::size_t> accepted_after_call;
    std::uint32_t calls{};
    std::uint32_t fail_call{};
    bool partial_failure{};
};

bool write(void* context, Bytes bytes, std::string& error) {
    auto& sink = *static_cast<Sink*>(context);
    ++sink.calls;
    if (sink.fail_call && sink.calls == sink.fail_call) {
        if (sink.partial_failure && bytes.size) {
            const auto partial = std::min<std::size_t>(2, bytes.size);
            sink.bytes.insert(sink.bytes.end(), bytes.data, bytes.data + partial);
        }
        error = "fixture stream failure";
        sink.accepted_after_call.push_back(sink.bytes.size());
        return false;
    }
    if (!bytes.data && bytes.size) {
        error = "invalid fixture bytes";
        return false;
    }
    if (bytes.size) sink.bytes.insert(sink.bytes.end(), bytes.data,
                                      bytes.data + bytes.size);
    sink.accepted_after_call.push_back(sink.bytes.size());
    return true;
}

writer::Status run(const Fixture& f, Sink& sink, std::string& error) {
    const writer::WriteServicesV1 services{&sink, write};
    return writer::write_lvls_v1(f.levels, f.world_map, f.save, services, error);
}

void exact_bytes_and_order() {
    stage = "source bytes, counts, names, difficulty order";
    Fixture f;
    Sink sink;
    std::string error;
    require(run(f, sink, error) == writer::Status::complete);
    require(error.empty());
    require(sink.bytes == f.expected());

    stage = "exact callback granularity";
    // Per row, source calls writeAs<string>: a 4-byte length write followed
    // by one NUL-terminated payload write, then a 4-byte state write.
    require(sink.calls == 3 * (1 + 2 * 3) + 3 * (1 + 2 * 3));
}

void failure_prefixes() {
    stage = "every stream failure retains only accepted prefix";
    Fixture f;
    Sink baseline;
    std::string error;
    require(run(f, baseline, error) == writer::Status::complete);
    const auto expected = f.expected();
    require(baseline.bytes == expected);

    for (std::uint32_t call = 1; call <= baseline.calls; ++call) {
        Sink failed;
        failed.fail_call = call;
        error.clear();
        require(run(f, failed, error) == writer::Status::failed);
        const auto prefix = call == 1 ? 0 : baseline.accepted_after_call[call - 2];
        require(failed.bytes.size() == prefix);
        require(std::equal(failed.bytes.begin(), failed.bytes.end(), expected.begin()));
        require(error == "fixture stream failure");
    }

    stage = "partial stream failure does not roll back sink bytes";
    Sink partial;
    partial.fail_call = 3; // count, string length, then string payload
    partial.partial_failure = true;
    error.clear();
    require(run(f, partial, error) == writer::Status::failed);
    require(partial.bytes.size() == 10);
    require(std::equal(partial.bytes.begin(), partial.bytes.end(), expected.begin()));
}

void missing_source_state_stops_at_reached_prefix() {
    stage = "missing canonical array fails after its name prefix";
    Fixture f;
    auto* states = f.save.source_level_states(1);
    std::free(states->words);
    states->words = nullptr;
    states->count = 0;
    Sink sink;
    std::string error;
    require(run(f, sink, error) == writer::Status::source_assertion_boundary);
    Raw expected_prefix;
    put_word(expected_prefix, 2);
    for (std::uint32_t row = 0; row < 2; ++row) {
        put_string(expected_prefix, f.levels.levels[row].name);
        put_word(expected_prefix, f.save.source_level_states(0)->words[row]);
    }
    put_word(expected_prefix, 2);
    put_string(expected_prefix, f.levels.levels[0].name);
    require(sink.bytes == expected_prefix);
    require(!error.empty());
}

void empty_and_argument_boundaries() {
    stage = "empty source tables write six zero counts without arrays";
    Fixture f;
    f.levels.levels.clear();
    f.world_map.locations.clear();
    Sink empty;
    std::string error;
    require(run(f, empty, error) == writer::Status::complete);
    require(empty.bytes == Raw(24, 0));

    stage = "required output stream";
    require(writer::write_lvls_v1(f.levels, f.world_map, f.save, {}, error) ==
            writer::Status::invalid_argument);
    require(!error.empty());
}
}  // namespace

int main() {
    try {
        exact_bytes_and_order();
        failure_prefixes();
        missing_source_state_stops_at_reached_prefix();
        empty_and_argument_boundaries();
        std::cout << "{\"validation\":\"PASS\",\"host_checks\":"
                  << checks << ",\"section\":\"LVLS\"}\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: " << ex.what() << '\n';
        return 1;
    }
}
