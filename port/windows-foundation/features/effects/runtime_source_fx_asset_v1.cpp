#include "runtime_source_fx_asset_v1.hpp"

#include "../../../asset-payloads/sha256.hpp"
#include "../../content_paths.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace dh::foundation::effects {
namespace {
std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::string digest_hex(const std::vector<std::uint8_t>& bytes) {
    dh2::assets::Sha256Digest digest{};
    if (!dh2::assets::sha256(bytes.data(), bytes.size(), digest))
        throw std::runtime_error("Unable to hash source FX bytes");
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (const auto value : digest) out << std::setw(2) << unsigned(value);
    return out.str();
}

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}
} // namespace

bool is_runtime_source_fx_uri_v1(const std::string& uri) noexcept {
    try {
        const auto normalized = lower(normalize_content_uri(uri));
        return normalized.rfind("data/3d/interface/", 0) == 0 ||
               normalized.rfind("original-cache/data/3d/interface/", 0) == 0;
    } catch (...) {
        return false;
    }
}

bool read_runtime_source_fx_asset_v1(
    const AssetCatalog& assets, const std::string& uri,
    std::vector<std::uint8_t>& bytes, std::string& resolved_path,
    std::string& error) {
    bytes.clear();
    resolved_path.clear();
    error.clear();
    if (!is_runtime_source_fx_uri_v1(uri))
        return fail(error, "Not a canonical source interface FX URI: " + uri);

    std::string normalized;
    try {
        normalized = normalize_content_uri(uri);
    } catch (const std::exception& e) {
        return fail(error, e.what());
    }

    std::vector<std::filesystem::path> candidates;
    const std::filesystem::path requested(normalized);
    auto add = [&](std::filesystem::path candidate) {
        candidate = candidate.lexically_normal();
        if (candidate.empty() || candidate.is_absolute()) return;
        if (std::find(candidates.begin(), candidates.end(), candidate) == candidates.end())
            candidates.push_back(std::move(candidate));
    };
    // Preserve the full authored path in either source-cache prefix direction.
    // These are the only candidates: deliberately no general content search.
    add(requested);
    const auto folded = lower(requested.generic_string());
    constexpr const char* prefix = "original-cache/";
    if (folded.rfind(prefix, 0) == 0) {
        add(std::filesystem::path(requested.generic_string().substr(15)));
    } else {
        add(std::filesystem::path(prefix) / requested);
    }

    for (const auto& candidate : candidates) {
        try {
            const auto absolute = assets.resolve(candidate);
            const auto relative = absolute.lexically_relative(assets.root());
            bytes = assets.read(relative);
            resolved_path = relative.generic_string();
            error.clear();
            return true;
        } catch (const std::exception&) {
            // Try only the next exact full-URI cache variant.
        }
    }
    return fail(error, "Exact authored FX resource not found (basename fallback disabled): " + uri);
}

bool verify_runtime_source_fx_sha256_v1(
    const std::vector<std::uint8_t>& bytes, const std::string& expected_sha256,
    std::string& error) {
    error.clear();
    if (expected_sha256.size() != 64 ||
        !std::all_of(expected_sha256.begin(), expected_sha256.end(), [](unsigned char c) {
            return std::isxdigit(c) != 0;
        }))
        return fail(error, "Expected source FX SHA-256 must contain 64 hexadecimal digits");
    try {
        const auto actual = digest_hex(bytes);
        if (actual != lower(expected_sha256))
            return fail(error, "Source FX SHA-256 mismatch: expected " +
                               lower(expected_sha256) + ", received " + actual);
    } catch (const std::exception& e) {
        return fail(error, e.what());
    }
    return true;
}

} // namespace dh::foundation::effects
