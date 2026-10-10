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

// Original fallback name: first character becomes 'x', extension becomes
// `_backup.mlx` (IDA Level::Update stage 7; file `x02_swamp_witchcave_backup.mlx`).
std::string procedural_backup_uri(const std::string& ruleUri);

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

// Session seed (IDA Application::LoadLevel): a forced seed (test aid --procedural-seed) wins; otherwise the
// saved per-difficulty seed; otherwise `fresh` (a new non-zero seed, see fresh_seed()).
std::uint32_t session_seed(const CharacterState& state, std::int32_t difficulty,
                           std::optional<std::uint32_t> forced, std::uint32_t fresh);
// Records the session seed in the profile so the next save keeps it.
void record_session_seed(CharacterState& state, std::int32_t difficulty, std::uint32_t seed);
// New seed from the wall clock and process counter (non-zero). Production source of a first seed.
std::uint32_t fresh_seed();

} // namespace dh::foundation::procedural
