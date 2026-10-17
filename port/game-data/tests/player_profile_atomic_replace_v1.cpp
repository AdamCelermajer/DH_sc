#include "../player_profile_atomic_replace_v1.hpp"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
void word(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned i = 0; i != 4; ++i)
        out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
}
void section(std::vector<std::uint8_t>& out, const char* tag,
             std::initializer_list<std::uint8_t> payload) {
    word(out, static_cast<std::uint32_t>(payload.size()));
    out.insert(out.end(), tag, tag + 4);
    out.insert(out.end(), payload.begin(), payload.end());
}
void write_file(const std::filesystem::path& path,
                const std::vector<std::uint8_t>& bytes) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    check(bool(file), "open test profile for write");
    if (!bytes.empty())
        file.write(reinterpret_cast<const char*>(bytes.data()),
                   static_cast<std::streamsize>(bytes.size()));
    check(bool(file), "write test profile");
}
std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    check(bool(file), "open test profile for read");
    const auto end = file.tellg();
    check(end >= 0, "tell test profile size");
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(end));
    file.seekg(0);
    if (!bytes.empty())
        file.read(reinterpret_cast<char*>(bytes.data()),
                  static_cast<std::streamsize>(bytes.size()));
    check(bool(file), "read test profile");
    return bytes;
}
std::filesystem::path scratch() {
    const auto tick = std::chrono::high_resolution_clock::now()
                          .time_since_epoch().count();
    return std::filesystem::temp_directory_path() /
        ("dh2-profile-atomic-" + std::to_string(tick));
}
}

int main() {
    namespace fs = std::filesystem;
    using namespace dh2::data::player_profile_atomic_replace_v1;
    const auto directory = scratch();
    try {
        fs::create_directories(directory);
        const auto primary = directory / "profile.bin";
        std::vector<std::uint8_t> source;
        word(source, 4);
        section(source, "ZZZZ", {0x11, 0x12}); // unknown/unselected
        section(source, "GEAR", {0xaa});
        section(source, "DUPL", {0x31});
        section(source, "DUPL", {0x32, 0x33}); // source lookup retains last
        write_file(primary, source);

        PlayerProfileIndexV1 index;
        std::string error;
        check(index.load({source.data(), source.size()}, error),
              "load canonical profile");
        auto canonical = index.borrow();
        auto retained_old_view = canonical;
        const std::uint8_t replacement_bytes[]{0x01, 0x02, 0x03};
        const std::vector<PlayerProfileRawSectionV1> replacements{
            {{{'G', 'E', 'A', 'R'}},
             {replacement_bytes, sizeof(replacement_bytes)}},
            {{{'N', 'E', 'W', '1'}}, {nullptr, 0}},
        };
        Result result{};
        check(replace_existing_profile_sections_v1(primary, index, canonical,
                  replacements, &result, error), "atomic replacement succeeds");
        check(error.empty(), "success clears error");
        check(result.backup_replaced && result.primary_replaced &&
                  result.index_published && result.replacement_count == 2,
              "replacement receipts reached");
        check(read_file(primary.string() + ".bak") == source,
              "backup contains prior primary bytes");
        const auto published = read_file(primary);
        check(canonical.bytes() == published && index.owns(canonical),
              "same canonical index and borrow publish committed bytes");
        check(retained_old_view.bytes() == source,
              "previous immutable borrow remains valid");
        check(canonical.payload("ZZZZ").size == 2 &&
                  canonical.payload("ZZZZ").data[0] == 0x11 &&
                  canonical.payload("ZZZZ").data[1] == 0x12,
              "unknown unselected section preserved");
        check(canonical.payload("DUPL").size == 2 &&
                  canonical.payload("DUPL").data[0] == 0x32 &&
                  canonical.payload("DUPL").data[1] == 0x33,
              "duplicate prior tag retains last payload");
        check(canonical.payload("GEAR").size == sizeof(replacement_bytes) &&
                  std::memcmp(canonical.payload("GEAR").data,
                              replacement_bytes, sizeof(replacement_bytes)) == 0,
              "requested section replaced");
        check(canonical.section("NEW1") &&
                  canonical.payload("NEW1").size == 0,
              "zero-length section inserted");

        const auto committed = read_file(primary);
        const auto committed_index = canonical.bytes();
        const auto backup_before_reject = read_file(primary.string() + ".bak");
        const std::uint8_t unused[]{7};
        const std::vector<PlayerProfileRawSectionV1> duplicate_replacements{
            {{{'G', 'E', 'A', 'R'}}, {unused, sizeof(unused)}},
            {{{'G', 'E', 'A', 'R'}}, {unused, sizeof(unused)}}};
        check(!replace_existing_profile_sections_v1(primary, index, canonical,
                  duplicate_replacements, &result, error),
              "invalid duplicate replacement rejected");
        check(read_file(primary) == committed &&
                  read_file(primary.string() + ".bak") == backup_before_reject &&
                  canonical.bytes() == committed_index && index.owns(canonical),
              "invalid replacement leaves files and canonical index unchanged");

        const auto stale_profile = canonical;
        std::vector<std::uint8_t> concurrent;
        word(concurrent, 1);
        section(concurrent, "RACE", {0x99});
        write_file(primary, concurrent);
        const auto backup_sentinel = std::vector<std::uint8_t>{0x55, 0x66};
        write_file(primary.string() + ".bak", backup_sentinel);
        check(!replace_existing_profile_sections_v1(primary, index, canonical,
                  {{{{'G', 'E', 'A', 'R'}}, {unused, sizeof(unused)}}},
                  &result, error), "stale canonical profile rejected");
        check(read_file(primary) == concurrent &&
                  read_file(primary.string() + ".bak") == backup_sentinel &&
                  canonical.bytes() == stale_profile.bytes() &&
                  index.owns(canonical),
              "stale primary leaves disk backup and index untouched");

        PlayerProfileIndexV1 other_index;
        check(other_index.load({canonical.bytes().data(),
                                canonical.bytes().size()}, error),
              "load byte-identical foreign index");
        auto foreign_view = other_index.borrow();
        check(!replace_existing_profile_sections_v1(primary, index, foreign_view,
                  replacements, &result, error),
              "foreign profile borrow rejected even when content matches");

        // A recovered Save may write only when the backup bytes are exactly
        // the canonical indexed profile. Missing, short, and marker primaries
        // all retain that old backup while atomically publishing the update.
        const std::vector<std::pair<std::string, std::vector<std::uint8_t>>> bad_bases{
            {"missing", {}}, {"short", {1, 2, 3}},
            {"marker", {0xff, 0xff, 0xff, 0xff}},
        };
        for (const auto& state : bad_bases) {
            const auto recovered_primary = directory / (state.first + ".savegame");
            const auto recovered_backup = std::filesystem::path(
                recovered_primary.string() + ".bak");
            write_file(recovered_backup, published);
            if (!state.second.empty()) write_file(recovered_primary, state.second);
            PlayerProfileIndexV1 recovered_index;
            check(recovered_index.load({published.data(), published.size()}, error),
                  "load recovered canonical backup");
            auto recovered_view = recovered_index.borrow();
            Result recovered_result{};
            check(replace_existing_profile_sections_v1(recovered_primary,
                      recovered_index, recovered_view, replacements,
                      &recovered_result, error),
                  "save through source-valid backup recovery");
            check(read_file(recovered_backup) == published &&
                      read_file(recovered_primary) == recovered_view.bytes() &&
                      recovered_index.owns(recovered_view) &&
                      recovered_result.index_published,
                  "recovered save keeps the old backup and publishes same index");
        }
        const auto malformed_primary = directory / "malformed.savegame";
        const auto malformed_backup = std::filesystem::path(
            malformed_primary.string() + ".bak");
        std::vector<std::uint8_t> malformed;
        word(malformed, 1); word(malformed, 100);
        malformed.insert(malformed.end(), {'P', 'N', 'A', 'M'});
        write_file(malformed_primary, malformed); write_file(malformed_backup, published);
        PlayerProfileIndexV1 malformed_index;
        check(malformed_index.load({published.data(), published.size()}, error),
              "load unrelated valid fallback snapshot");
        auto malformed_view = malformed_index.borrow();
        check(!replace_existing_profile_sections_v1(malformed_primary,
                  malformed_index, malformed_view, replacements, &result, error) &&
                  read_file(malformed_primary) == malformed &&
                  read_file(malformed_backup) == published &&
                  malformed_view.bytes() == published,
              "usable but malformed primary never falls back to backup");

        fs::remove_all(directory);
        std::cout << "player_profile_atomic_replace_v1: PASS\n";
        return 0;
    } catch (const std::exception& exception) {
        std::error_code ignored;
        fs::remove_all(directory, ignored);
        std::cerr << "player_profile_atomic_replace_v1: " << exception.what()
                  << '\n';
        return 1;
    }
}
