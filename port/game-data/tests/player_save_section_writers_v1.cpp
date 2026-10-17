#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../player_save_section_writers_v1.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace writers = dh2::data::player_save_section_writers_v1;
using dh2::data::Bytes;
using dh2::data::PlayerSavegameV1;
using Raw = std::vector<std::uint8_t>;

namespace {
unsigned checks = 0;
const char* stage = "startup";

void require(bool value) {
    ++checks;
    if (!value)
        throw std::runtime_error(std::string("save-section writer check ") +
                                 std::to_string(checks) + " at " + stage);
}

void put_word(Raw& bytes, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i)
        bytes.push_back(static_cast<std::uint8_t>(value >> (i * 8)));
}

void put_halfword(Raw& bytes, std::uint16_t value) {
    bytes.push_back(static_cast<std::uint8_t>(value));
    bytes.push_back(static_cast<std::uint8_t>(value >> 8));
}

void put_string(Raw& bytes, const std::string& value) {
    put_word(bytes, static_cast<std::uint32_t>(value.size() + 1));
    bytes.insert(bytes.end(), value.begin(), value.end());
    bytes.push_back(0);
}

struct Sink {
    Raw bytes;
    std::uint32_t calls{};
    std::uint32_t fail_call{};
};

bool write(void* context, Bytes bytes, std::string& error) {
    auto& sink = *static_cast<Sink*>(context);
    ++sink.calls;
    if (sink.fail_call && sink.calls == sink.fail_call) {
        error = "fixture stream failure";
        return false;
    }
    if ((!bytes.data && bytes.size) || bytes.size > 1024) {
        error = "invalid fixture write";
        return false;
    }
    if (bytes.size) sink.bytes.insert(sink.bytes.end(), bytes.data,
                                      bytes.data + bytes.size);
    return true;
}

writers::Status run(const char* tag, const PlayerSavegameV1& save,
                    Sink& sink, std::string& error) {
    const writers::WriteServicesV1 stream{&sink, write};
    return writers::write_section_v1(tag, save, stream, error);
}

void initialize_save(PlayerSavegameV1& save) {
    save.initialize_faeries();
    const std::array<std::int32_t, 3> selected{
        0x01020304, -2, 0x11223344};
    Raw selected_bytes;
    for (const auto value : selected)
        put_word(selected_bytes, static_cast<std::uint32_t>(value));
    std::size_t consumed = 0;
    std::string error;
    require(save.load_current_faery({selected_bytes.data(), selected_bytes.size()},
                                    consumed, error));
    require(consumed == selected_bytes.size());
    for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
        for (std::uint32_t index = 0; index < 5; ++index) {
            const auto level = static_cast<std::int32_t>(
                0x1200 + difficulty * 0x100 + index * 3);
            const auto state = static_cast<std::int32_t>(
                0x80 + difficulty * 9 + index);
            require(save.set_faery_level(index, level, difficulty, error));
            require(save.set_faery_state(index, state, difficulty, error));
        }
    }
}

Raw expected_cfee(const PlayerSavegameV1& save) {
    Raw expected;
    for (const auto value : save.current_faeries())
        put_word(expected, static_cast<std::uint32_t>(value));
    return expected;
}

Raw expected_faes(const PlayerSavegameV1& save) {
    Raw expected;
    for (std::size_t difficulty = 0; difficulty < 3; ++difficulty) {
        put_word(expected, static_cast<std::uint32_t>(
                               save.current_faeries()[difficulty]));
        put_word(expected, 5);
        for (const auto& faery : save.faeries()[difficulty]) {
            put_halfword(expected, faery.level);
            expected.push_back(faery.state);
        }
    }
    return expected;
}

Raw expected_ftvl(const PlayerSavegameV1& save) {
    Raw expected;
    for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
        const auto* words = save.source_fast_travel_bits(difficulty);
        require(words != nullptr);
        std::string text;
        text.reserve(64);
        for (int bit = 63; bit >= 0; --bit)
            text.push_back(((*words)[std::uint32_t(bit) >> 5] &
                            (std::uint32_t{1} << (std::uint32_t(bit) & 31)))
                               ? '1'
                               : '0');
        put_string(expected, text);
    }
    return expected;
}

void check_exact_serialization() {
    stage = "CFEE bytes";
    PlayerSavegameV1 save;
    initialize_save(save);
    std::string error;
    Sink cfee;
    require(run("CFEE", save, cfee, error) == writers::Status::complete);
    require(error.empty() && cfee.bytes == expected_cfee(save));

    stage = "FAES bytes and source field order";
    Sink faes;
    require(run("FAES", save, faes, error) == writers::Status::complete);
    require(error.empty() && faes.bytes == expected_faes(save));

    stage = "FTVL three 64-bit strings";
    for (std::uint32_t difficulty = 0; difficulty < 3; ++difficulty) {
        auto* words = save.source_fast_travel_bits(difficulty);
        require(words != nullptr);
        if (difficulty == 0) *words = {0x80000001u, 0x01234567u};
        if (difficulty == 1) *words = {0u, 0x80000000u};
        if (difficulty == 2) *words = {0x89abcdefu, 0x76543210u};
    }
    Sink ftvl;
    require(run("FTVL", save, ftvl, error) == writers::Status::complete);
    const auto ftvl_expected = expected_ftvl(save);
    require(error.empty() && ftvl.bytes == ftvl_expected);
    require(ftvl.bytes.size() == 3 * 69);
}

void check_boundaries_and_prefixes() {
    std::string error;
    PlayerSavegameV1 blank;
    Sink sink;

    stage = "uninitialized CFEE source assertion";
    require(run("CFEE", blank, sink, error) ==
            writers::Status::source_assertion_boundary);
    require(sink.bytes.empty() && !error.empty());
    error.clear();

    stage = "uninitialized FAES source assertion";
    require(run("FAES", blank, sink, error) ==
            writers::Status::source_assertion_boundary);
    require(sink.bytes.empty() && !error.empty());
    error.clear();

    stage = "FTVL uses constructor-owned zero bitsets independently";
    Sink blank_ftvl;
    require(run("FTVL", blank, blank_ftvl, error) == writers::Status::complete);
    require(error.empty() && blank_ftvl.bytes == expected_ftvl(blank));
    error.clear();

    stage = "unknown tag";
    require(run("SKIL", blank, sink, error) == writers::Status::unsupported_tag);
    require(sink.bytes.empty() && !error.empty());
    error.clear();

    stage = "missing stream";
    require(writers::write_section_v1("CFEE", blank, {}, error) ==
            writers::Status::invalid_argument);
    require(sink.bytes.empty() && !error.empty());

    PlayerSavegameV1 save;
    initialize_save(save);
    stage = "CFEE stream failure preserves accepted prefix";
    const auto cfee_expected = expected_cfee(save);
    Sink cfee_failure;
    cfee_failure.fail_call = 3;
    require(run("CFEE", save, cfee_failure, error) == writers::Status::failed);
    require(cfee_failure.bytes.size() == 8 &&
            Raw(cfee_expected.begin(), cfee_expected.begin() + 8) ==
                cfee_failure.bytes);
    require(error == "fixture stream failure");
    error.clear();

    stage = "FAES stream failure preserves accepted prefix";
    const auto faes_expected = expected_faes(save);
    Sink faes_failure;
    faes_failure.fail_call = 5;
    require(run("FAES", save, faes_failure, error) == writers::Status::failed);
    require(faes_failure.bytes.size() == 11 &&
            Raw(faes_expected.begin(), faes_expected.begin() + 11) ==
                faes_failure.bytes);
    error.clear();

    stage = "FTVL string failure preserves length prefix";
    Sink ftvl_failure;
    ftvl_failure.fail_call = 2;
    require(run("FTVL", save, ftvl_failure, error) == writers::Status::failed);
    require(ftvl_failure.bytes == Raw({65, 0, 0, 0}));
    require(error == "fixture stream failure");
}
}  // namespace

int main() {
    try {
        check_exact_serialization();
        check_boundaries_and_prefixes();
        std::cout << "{\"validation\":\"PASS\",\"host_checks\":"
                  << checks << ",\"sections\":[\"CFEE\",\"FAES\",\"FTVL\"]}\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: " << ex.what() << '\n';
        return 1;
    }
}
