#pragma once

#include "../../asset_catalog.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation::effects {

// The general content resolver has engine-compatible basename search folders.
// Source effect resources in data/3D/interface must retain their full URI so an
// animation clip with the same basename can never be mistaken for an FX scene.
bool is_runtime_source_fx_uri_v1(const std::string& uri) noexcept;

// Reads only the exact URI and original-cache/full-URI variants. This function
// never tries models/, animations/, or a basename-only search for interface FX.
// Non-interface URIs are reported as not handled and remain the caller's choice.
bool read_runtime_source_fx_asset_v1(
    const AssetCatalog& assets, const std::string& uri,
    std::vector<std::uint8_t>& bytes, std::string& resolved_path,
    std::string& error);

// Optional provenance check for callers/tests that have an expected source
// digest. Empty/malformed digests and mismatches fail closed.
bool verify_runtime_source_fx_sha256_v1(
    const std::vector<std::uint8_t>& bytes, const std::string& expected_sha256,
    std::string& error);

} // namespace dh::foundation::effects
