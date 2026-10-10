#include "runtime_creation_source_loader_v1.hpp"

#include "../../../content_paths.hpp"
#include "../../../../script-runtime/script_constants.hpp"
#include "../../../../game-data/properties.hpp"

#include <array>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <memory>
#include <sstream>
#include <utility>

namespace dh::foundation::frontend::creation {
namespace {
using Bytes = dh2::data::Bytes;

constexpr std::array<std::uint32_t, 64> kSha256Round = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};

std::uint32_t rotr(std::uint32_t value, unsigned bits) noexcept {
    return (value >> bits) | (value << (32 - bits));
}

std::string sha256_hex(const std::vector<std::uint8_t>& bytes) {
    std::vector<std::uint8_t> message(bytes);
    const auto bit_length = static_cast<std::uint64_t>(message.size()) * 8u;
    message.push_back(0x80);
    while (message.size() % 64 != 56) message.push_back(0);
    for (int shift = 56; shift >= 0; shift -= 8)
        message.push_back(static_cast<std::uint8_t>(bit_length >> shift));

    std::array<std::uint32_t, 8> state = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    for (std::size_t offset = 0; offset < message.size(); offset += 64) {
        std::array<std::uint32_t, 64> words{};
        for (unsigned i = 0; i < 16; ++i) {
            const auto at = offset + i * 4;
            words[i] = (std::uint32_t(message[at]) << 24) |
                       (std::uint32_t(message[at + 1]) << 16) |
                       (std::uint32_t(message[at + 2]) << 8) | message[at + 3];
        }
        for (unsigned i = 16; i < words.size(); ++i) {
            const auto s0 = rotr(words[i - 15], 7) ^ rotr(words[i - 15], 18) ^ (words[i - 15] >> 3);
            const auto s1 = rotr(words[i - 2], 17) ^ rotr(words[i - 2], 19) ^ (words[i - 2] >> 10);
            words[i] = words[i - 16] + s0 + words[i - 7] + s1;
        }
        auto a = state[0], b = state[1], c = state[2], d = state[3];
        auto e = state[4], f = state[5], g = state[6], h = state[7];
        for (unsigned i = 0; i < words.size(); ++i) {
            const auto s1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
            const auto choice = (e & f) ^ (~e & g);
            const auto t1 = h + s1 + choice + kSha256Round[i] + words[i];
            const auto s0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
            const auto majority = (a & b) ^ (a & c) ^ (b & c);
            const auto t2 = s0 + majority;
            h = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }
        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        state[4] += e; state[5] += f; state[6] += g; state[7] += h;
    }
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (auto word : state) out << std::setw(8) << word;
    return out.str();
}

bool read_hashed(const AssetCatalog& assets, const std::string& authored_uri,
                 std::vector<std::uint8_t>& bytes,
                 std::vector<RuntimeCreationSourceHashV1>& hashes,
                 std::string& error,
                 std::filesystem::path* resolved_relative = nullptr) {
    try {
        const auto resolved = resolve_content_path(assets, authored_uri);
        bytes = read_content(assets, authored_uri);
        const auto relative = resolved.lexically_relative(assets.root());
        if (resolved_relative) *resolved_relative = relative;
        hashes.push_back({relative.generic_string(),
                          static_cast<std::uint64_t>(bytes.size()),
                          sha256_hex(bytes)});
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

bool load_caps(const std::vector<std::uint8_t>& bytes,
               std::array<std::uint32_t, 3>& caps, std::string& error) {
    std::unique_ptr<dh2_script_constants, decltype(&dh2_script_constants_destroy)> constants(
        dh2_script_constants_create(), &dh2_script_constants_destroy);
    if (!constants) { error = "Could not allocate CharacterDesign constants owner"; return false; }
    dh2_script_constants_reload receipt{};
    if (bytes.size() > UINT32_MAX ||
        dh2_script_constants_load(constants.get(), bytes.data(),
            static_cast<std::uint32_t>(bytes.size()), &receipt) != 0 ||
        receipt.consumed != bytes.size()) {
        error = "Original CharacterDesign constants did not load completely";
        return false;
    }
    constexpr const char* names[] = {
        "MaxSkillLevelBNormal", "MaxSkillLevelCHard", "MaxSkillLevelDVeryHard"};
    std::array<std::uint32_t, 3> loaded{};
    for (std::size_t i = 0; i < loaded.size(); ++i) {
        std::int32_t value{};
        if (dh2_script_constants_get(constants.get(), "CharacterDesign", names[i], &value) != 0 || value <= 0) {
            error = std::string("Missing positive original CharacterDesign cap ") + names[i];
            return false;
        }
        loaded[i] = static_cast<std::uint32_t>(value);
    }
    caps = loaded;
    error.clear();
    return true;
}
} // namespace

bool RuntimeCreationSourceOwnerV1::valid() const noexcept {
    return properties && loot_owner && skill_owner && caller_owned_random_adapter &&
           character_design_caps_known && source_hashes.size() == 13 &&
           static_cast<bool>(loot_owner->borrow()) && static_cast<bool>(skill_owner->borrow());
}

RuntimeCreationSourceV1 RuntimeCreationSourceOwnerV1::source() const {
    RuntimeCreationSourceV1 result;
    if (!valid()) return result;
    result.properties = properties;
    result.loot = loot_owner->borrow();
    result.skills = skill_owner->borrow();
    result.source_loot_random = caller_owned_random_adapter;
    result.source_skill_grant_context_known = true;
    result.source_skill_level_caps = character_design_skill_caps;
    return result;
}

bool load_runtime_creation_source_v1(
    const AssetCatalog& assets,
    dh2::data::LootRandom8V2& caller_owned_random_adapter,
    RuntimeCreationSourceOwnerV1& output,
    std::string& error) {
    try {
        RuntimeCreationSourceOwnerV1 next;
        next.caller_owned_random_adapter = &caller_owned_random_adapter;
        next.source_hashes.reserve(13);
        auto read = [&](const char* filename, std::vector<std::uint8_t>& bytes,
                        std::filesystem::path* resolved_relative = nullptr) {
            const auto uri = std::string("data/pydata/") + filename;
            return read_hashed(assets, uri,
                               bytes, next.source_hashes, error, resolved_relative);
        };

        std::vector<std::uint8_t> character_records, character_names, character_schema;
        std::vector<std::uint8_t> class_records, class_names, class_schema;
        std::array<std::filesystem::path, 6> property_paths;
        if (!read("character_properties_pyarray.bin", character_records, &property_paths[0]) ||
            !read("character_properties_pyarraynames.bin", character_names, &property_paths[1]) ||
            !read("character_properties_pystructnames.bin", character_schema, &property_paths[2]) ||
            !read("character_classes_pyarray.bin", class_records, &property_paths[3]) ||
            !read("character_classes_pyarraynames.bin", class_names, &property_paths[4]) ||
            !read("character_classes_pystructnames.bin", class_schema, &property_paths[5])) return false;
        for (const auto& path : property_paths) {
            if (path.parent_path() != property_paths.front().parent_path()) {
                error = "Original CharacterTable and CharacterClass files resolve to different roots";
                return false;
            }
        }

        auto properties = std::make_shared<OriginalPropertyDatabase>();
        if (!load_original_property_tables(assets,
                property_paths.front().parent_path().generic_string(), *properties, error)) return false;
        next.properties = std::move(properties);

        std::vector<std::uint8_t> loot_records, loot_names, loot_schema;
        if (!read("loot_table_pyarray.bin", loot_records, nullptr) ||
            !read("loot_table_pyarraynames.bin", loot_names, nullptr) ||
            !read("loot_table_pystructnames.bin", loot_schema, nullptr)) return false;
        next.loot_owner = std::make_shared<dh2::data::LootTablesV2>();
        const auto bytes = [](const auto& input) { return Bytes{input.data(), input.size()}; };
        if (!next.loot_owner->load(bytes(loot_records), bytes(loot_names), bytes(loot_schema), error))
            return false;

        std::vector<std::uint8_t> skill_records, skill_names, skill_schema;
        if (!read("skills_pyarray.bin", skill_records, nullptr) ||
            !read("skills_pyarraynames.bin", skill_names, nullptr) ||
            !read("skills_pystructnames.bin", skill_schema, nullptr)) return false;
        next.skill_owner = std::make_shared<dh2::data::SkillTables>();
        if (!next.skill_owner->load(bytes(skill_records), bytes(skill_names), bytes(skill_schema), error))
            return false;

        std::vector<std::uint8_t> design_constants;
        if (!read("design_pycst.bin", design_constants, nullptr) ||
            !load_caps(design_constants, next.character_design_skill_caps, error)) return false;
        next.character_design_caps_known = true;
        if (!next.valid()) {
            error = "Loaded source table owners or receipts failed completeness validation";
            return false;
        }
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

} // namespace dh::foundation::frontend::creation
