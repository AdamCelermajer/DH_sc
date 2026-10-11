#pragma once

// P17 PROCGEN: procedural `.rule.xml` levels for the Windows EXE.
//
// The generator is the canonical port in port/level-loader (procedural_*_v1):
// rule reader -> MGX blocks -> connections -> lists -> RootRule -> layout(seed)
// -> module projection. This file adapts it to the Windows AssetCatalog and
// emits one Level XML document per (rule, seed). The document uses the same
// Module/LevelConfig shape as a fixed `.mlx`, so the existing geometry,
// declaration, collision, camera and music loaders consume it unchanged.
//
// Original behaviour (IDA rnd::RandomGenerator / Level::GenerateRandomLevel):
// the generated level is streamed in memory and loaded as the level data; on
// failure the level falls back to `x<NN>_<name>_backup.mlx` (authored fixed map).
// The seed is the per-difficulty PlayerSavegame seed (Application::LoadLevel).

#include "asset_catalog.hpp"
#include "../../character_state.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::procedural {

struct GeneratedLevel {
    std::string rule_uri;          // data/scene/NNN_name.rule.xml
    std::string level_uri;         // same path (served as a virtual file by AssetCatalog)
    std::uint32_t seed = 0;
    std::size_t tiles = 0;         // generated tile count (RootRule placements)
    std::size_t modules = 0;       // emitted Module declarations
    std::size_t unplaced_modules = 0;
    std::vector<std::uint8_t> xml; // generated Level document
    std::vector<std::string> notices;
};

// True for a procedural level path (suffix `.rule.xml`, case-insensitive).
bool is_procedural_level_uri(const std::string& uri);

// Original fallback name (IDA Level::_LoadProcess stage 7): the rule's file name with its first character set to
// 'x', cut at the first '.', plus `_BACKUP.mlx`. The shipped files are lowercase `_backup.mlx` (Windows is case
// insensitive; this returns the shipped case), e.g. `x02_swamp_witchcave_backup.mlx`.
std::string procedural_backup_uri(const std::string& ruleUri);
// Candidates in order: the IDA name, then the same name without a trailing `_NN` rule number. The shipped
// 007 fallback is `x07_crypt_backup.mlx` (IDA name `x07_crypt_01_backup.mlx`): the only one of the 30 shipped
// backups that does not follow the IDA rule exactly.
std::vector<std::string> procedural_backup_candidates(const std::string& ruleUri);
// The first candidate present in the asset root; the IDA name when none is (so the error names it).
std::string resolve_procedural_backup(const AssetCatalog& assets, const std::string& ruleUri);

// One generated tile as the original layout reports it (plain values; the fidelity test compares these with the
// original ARM oracle, reference/original-layout-seeds-0-1.tsv).
struct LayoutTileRecord {
    std::uint32_t block_source = 0;       // original Connection-map source index (not a save id)
    std::string name;
    std::int32_t grid_x = 0, grid_y = 0;
    std::uint32_t height_bits = 0;        // IEEE-754 bits of the float height (exact comparison)
    std::string list_block, list_gameplay, list_visual;
    std::int32_t chances = 100;
    std::vector<std::uint32_t> children;  // tile indices, in original order
};
// The layout stage of one (rule, seed): the original Generate result, final RNG state and event counters, and tiles.
struct LayoutRecord {
    bool generated = false;               // original Generate result (false = completed no-layout run)
    std::uint32_t seed = 0, final_state = 0;
    std::uint32_t random_calls = 0, root_attempts = 0, place_calls = 0, unspawn_calls = 0, step_calls = 0,
                  path_calls = 0, rule_calls = 0;
    std::vector<LayoutTileRecord> tiles;
};
// Layout stage only (sources, blocks, connections, lists, rules, original Generate). False is a checked failure
// (missing input, unsupported domain); a completed no-layout run is success with generated=false.
bool generate_layout(const AssetCatalog& assets, const std::string& ruleUri, std::uint32_t seed,
                     LayoutRecord& out, std::string& error);

// Runs the full canonical chain for `ruleUri` with `seed`. On success `out`
// holds the Level document. On failure `error` names the failing stage and the
// caller falls back to procedural_backup_uri().
bool generate_level(const AssetCatalog& assets, const std::string& ruleUri,
                    std::uint32_t seed, GeneratedLevel& out, std::string& error);

// Installs the generated document as the virtual level file `out.level_uri`
// (replacing any previous generation for that uri).
void install_generated_level(AssetCatalog& assets, const GeneratedLevel& level);

// FNV-1a over the generated document: the layout signature printed by the EXE.
std::uint64_t layout_signature(const GeneratedLevel& level);

// Named character component that keeps the per-difficulty procedural seeds. It lives in the schema v4
// component roster (append-only tail, see CharacterComponent), so no schema bump: a save without it reads
// as zero seeds (generated on first use). Bytes: three little-endian u32, one per difficulty.
inline constexpr const char* procedural_seed_component_id = "dh2.procgen.seed.v1";
using ProceduralSeeds = std::array<std::uint32_t, character_difficulty_count>;
// Seeds stored in the profile (zero = not generated yet). A missing or malformed component reads as zeros.
ProceduralSeeds saved_seeds(const CharacterState& state);
void store_saved_seeds(CharacterState& state, const ProceduralSeeds& seeds);

// Session seed (IDA Application::LoadLevel): a forced seed (test aid --procedural-seed) wins; otherwise the
// saved per-difficulty seed; otherwise `fresh` (a new non-zero seed, see fresh_seed()).
std::uint32_t session_seed(const CharacterState& state, std::int32_t difficulty,
                           std::optional<std::uint32_t> forced, std::uint32_t fresh);
// Records the session seed in the profile so the next save keeps it.
void record_session_seed(CharacterState& state, std::int32_t difficulty, std::uint32_t seed);
// New seed from the wall clock and process counter (non-zero). Production source of a first seed.
std::uint32_t fresh_seed();

} // namespace dh::foundation::procedural
