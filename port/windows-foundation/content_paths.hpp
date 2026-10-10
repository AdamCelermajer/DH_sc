#pragma once

#include "asset_catalog.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace dh::foundation {

// Original exporter paths are relative resource names, not network URLs.
// Converts backslashes, drops authoring-platform folders, and rejects unsafe
// absolute/traversing paths. Original artist-drive names such as q:/data/...
// are confined aliases of data/...; the drive is never accessed. Spelling is
// retained for case-aware diagnostics.
std::string normalize_content_uri(const std::string& uri);

// ownerRelative is the referencing asset (not its directory). Candidates are
// ordered from exact URI and owner-relative lookup to original compiled-data
// folders and the flattened development asset folders.
std::vector<std::filesystem::path> content_path_candidates(
    const std::string& uri, const std::filesystem::path& ownerRelative = {});

// Returns a canonical absolute file inside assets.root(). Case-insensitive
// component lookup also works on case-sensitive hosts; ambiguity rejects.
std::filesystem::path resolve_content_path(
    const AssetCatalog& assets, const std::string& uri,
    const std::filesystem::path& ownerRelative = {});

std::vector<std::uint8_t> read_content(
    const AssetCatalog& assets, const std::string& uri,
    const std::filesystem::path& ownerRelative = {});

} // namespace dh::foundation
