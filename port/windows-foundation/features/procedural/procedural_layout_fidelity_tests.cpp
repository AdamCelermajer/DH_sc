// P17 PROCGEN2 layout fidelity. Usage: procedural_layout_fidelity_tests <asset-root>
// Locks the procedural RNG and layout against the ORIGINAL ARM32 execution (reference/*.tsv, exported from the
// canonical oracle receipts procedural-random-differential.json and procedural-layout-original.json):
//  1. rnd::RandomGenerator sequences: NextInt, between, bounded (with the RNG state after each call) and Hash.
//  2. Layouts of all 35 authored procedural rules at seeds 0 and 1: generation success, final RNG state, call
//     counters, and every tile (source index, name, grid, exact height bits, list element, children).
// A seed the original fails on (017 seed 0, 027 seeds 0 and 1) must also fail here with generated=false.
#include "procedural_level_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../../level-loader/procedural_random_v1.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::procedural;
namespace fs = std::filesystem;

namespace {

int failures = 0;
int checks = 0;
void check(bool condition, const std::string& what) {
    ++checks;
    if (condition) return;
    ++failures;
    if (failures <= 40) std::cout << "FAIL " << what << '\n';
}

// Tab split that keeps empty fields, including a trailing one.
std::vector<std::string> split_tabs(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    for (const char c : line) {
        if (c == '\t') { fields.push_back(current); current.clear(); }
        else current.push_back(c);
    }
    fields.push_back(current);
    return fields;
}

std::vector<std::string> split_commas(const std::string& text) {
    std::vector<std::string> parts;
    if (text.empty()) return parts;
    std::string current;
    for (const char c : text) {
        if (c == ',') { parts.push_back(current); current.clear(); }
        else current.push_back(c);
    }
    parts.push_back(current);
    return parts;
}

std::vector<std::uint32_t> to_u32_list(const std::string& text) {
    std::vector<std::uint32_t> values;
    for (const auto& part : split_commas(text)) values.push_back(static_cast<std::uint32_t>(std::stoull(part)));
    return values;
}

std::vector<std::string> read_lines(const fs::path& path, bool& ok) {
    std::ifstream in(path);
    ok = bool(in);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty() && line[0] != '#') lines.push_back(line);
    }
    return lines;
}

std::string bytes_from_hex(const std::string& hex) {
    std::string bytes;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2)
        bytes.push_back(static_cast<char>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    return bytes;
}

void rng_checks(const fs::path& reference) {
    bool ok = false;
    const auto lines = read_lines(reference, ok);
    check(ok, "RNG reference is readable: " + reference.string());
    std::size_t nextRows = 0, betweenRows = 0, boundedRows = 0, hashRows = 0;
    for (const auto& line : lines) {
        const auto f = split_tabs(line);
        if (f[0] == "next") {
            ++nextRows;
            dh2::loader::ProceduralRandomV1 random(static_cast<std::uint32_t>(std::stoull(f[1])));
            bool match = f.size() == 2 + 16;
            for (std::size_t i = 2; match && i < f.size(); ++i)
                match = random.next() == static_cast<std::uint32_t>(std::stoull(f[i]));
            check(match, "NextInt sequence seed=" + f[1]);
        } else if (f[0] == "between") {
            ++betweenRows;
            dh2::loader::ProceduralRandomV1 random(static_cast<std::uint32_t>(std::stoull(f[1])));
            const auto expected = to_u32_list(f[4]);
            const auto value = random.between(std::stoi(f[2]), std::stoi(f[3]));
            check(expected.size() == 2 && static_cast<std::uint32_t>(value) == expected[0] && random.state() == expected[1],
                  "between seed=" + f[1] + " min=" + f[2] + " max=" + f[3]);
        } else if (f[0] == "bounded") {
            ++boundedRows;
            const auto bound = static_cast<std::uint32_t>(std::stoull(f[2]));
            dh2::loader::ProceduralRandomV1 random(static_cast<std::uint32_t>(std::stoull(f[1])));
            const auto expected = to_u32_list(f[3]);
            const auto value = bound ? random.bounded(bound) : 0u;
            check(bound != 0 && expected.size() == 2 && value == expected[0] && random.state() == expected[1],
                  "bounded seed=" + f[1] + " bound=" + f[2]);
        } else if (f[0] == "hash") {
            ++hashRows;
            const auto expected = to_u32_list(f[2]);
            const auto value = dh2::loader::ProceduralRandomV1::hash(bytes_from_hex(f[1]));
            check(expected.size() == 1 && value == expected[0], "Hash of input " + f[1]);
        }
    }
    std::cout << "info rng rows next=" << nextRows << " between=" << betweenRows << " bounded=" << boundedRows
              << " hash=" << hashRows << '\n';
    check(nextRows == 138 && betweenRows == 139 && boundedRows == 966 && hashRows == 135, "RNG reference row counts");
}

struct OracleRun {
    bool generated = false;
    LayoutRecord expected;
};

void layout_checks(const AssetCatalog& assets, const fs::path& reference) {
    bool ok = false;
    const auto lines = read_lines(reference, ok);
    check(ok, "layout reference is readable: " + reference.string());
    std::map<std::pair<std::string, std::uint32_t>, OracleRun> runs;
    std::vector<std::pair<std::string, std::uint32_t>> order;
    for (const auto& line : lines) {
        const auto f = split_tabs(line);
        if (f[0] == "R") {
            OracleRun run;
            const auto key = std::make_pair(f[1], static_cast<std::uint32_t>(std::stoull(f[2])));
            run.generated = f[3] == "1";
            run.expected.generated = run.generated;
            run.expected.seed = key.second;
            run.expected.final_state = static_cast<std::uint32_t>(std::stoull(f[4]));
            run.expected.random_calls = static_cast<std::uint32_t>(std::stoull(f[5]));
            run.expected.root_attempts = static_cast<std::uint32_t>(std::stoull(f[6]));
            run.expected.place_calls = static_cast<std::uint32_t>(std::stoull(f[7]));
            run.expected.unspawn_calls = static_cast<std::uint32_t>(std::stoull(f[8]));
            run.expected.step_calls = static_cast<std::uint32_t>(std::stoull(f[9]));
            run.expected.path_calls = static_cast<std::uint32_t>(std::stoull(f[10]));
            run.expected.rule_calls = static_cast<std::uint32_t>(std::stoull(f[11]));
            runs[key] = run;
            order.push_back(key);
        } else if (f[0] == "T") {
            const auto key = std::make_pair(f[1], static_cast<std::uint32_t>(std::stoull(f[2])));
            LayoutTileRecord tile;
            tile.block_source = static_cast<std::uint32_t>(std::stoull(f[4]));
            tile.name = f[5];
            tile.grid_x = std::stoi(f[6]);
            tile.grid_y = std::stoi(f[7]);
            tile.height_bits = static_cast<std::uint32_t>(std::stoull(f[8]));
            tile.list_block = f[9];
            tile.list_gameplay = f[10];
            tile.list_visual = f[11];
            tile.chances = std::stoi(f[12]);
            tile.children = to_u32_list(f.size() > 13 ? f[13] : std::string());
            runs[key].expected.tiles.push_back(std::move(tile));
        }
    }
    int matchedRuns = 0, generatedRuns = 0, noLayoutRuns = 0;
    for (const auto& key : order) {
        const auto& [rule, seed] = key;
        const auto& run = runs[key];
        const auto label = rule + " seed " + std::to_string(seed);
        LayoutRecord actual;
        std::string error;
        const bool called = generate_layout(assets, rule, seed, actual, error);
        check(called, label + " generate_layout: " + error);
        if (!called) continue;
        bool same = actual.generated == run.expected.generated && actual.final_state == run.expected.final_state &&
                    actual.random_calls == run.expected.random_calls && actual.root_attempts == run.expected.root_attempts &&
                    actual.place_calls == run.expected.place_calls && actual.unspawn_calls == run.expected.unspawn_calls &&
                    actual.step_calls == run.expected.step_calls && actual.path_calls == run.expected.path_calls &&
                    actual.rule_calls == run.expected.rule_calls && actual.tiles.size() == run.expected.tiles.size();
        check(same, label + " generated/final RNG state/counters/tile count match the original");
        if (run.generated) ++generatedRuns; else ++noLayoutRuns;
        if (same) {
            bool tilesMatch = true;
            for (std::size_t i = 0; i < actual.tiles.size(); ++i) {
                const auto& a = actual.tiles[i];
                const auto& e = run.expected.tiles[i];
                const bool tileSame = a.block_source == e.block_source && a.name == e.name && a.grid_x == e.grid_x &&
                    a.grid_y == e.grid_y && a.height_bits == e.height_bits && a.list_block == e.list_block &&
                    a.list_gameplay == e.list_gameplay && a.list_visual == e.list_visual && a.chances == e.chances &&
                    a.children == e.children;
                if (!tileSame) {
                    tilesMatch = false;
                    check(false, label + " tile " + std::to_string(i) + " (" + e.name + ") differs from the original");
                    break;
                }
            }
            check(tilesMatch, label + " every tile matches the original");
            if (tilesMatch) ++matchedRuns;
        }
    }
    std::cout << "info layouts generated=" << generatedRuns << " no-layout=" << noLayoutRuns
              << " matched=" << matchedRuns << " of " << order.size() << '\n';
    check(order.size() == 70, "layout reference covers 35 rules at seeds 0 and 1");
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: procedural_layout_fidelity_tests <asset-root>\n";
        return 2;
    }
    const fs::path reference = fs::path(__FILE__).parent_path() / "reference";
    AssetCatalog assets(argv[1]);
    rng_checks(reference / "original-rng-differential.tsv");
    layout_checks(assets, reference / "original-layout-seeds-0-1.tsv");
    std::cout << "checks " << checks << '\n';
    std::cout << (failures ? "procedural_layout_fidelity FAILED\n" : "procedural_layout_fidelity OK\n");
    return failures ? 1 : 0;
}
