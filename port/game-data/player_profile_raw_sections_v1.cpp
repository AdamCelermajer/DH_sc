#include "player_profile_raw_sections_v1.hpp"
#include <cstring>
#include <map>
#include <set>

namespace {
void append_word(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned i = 0; i != 4; ++i)
        out.push_back(static_cast<std::uint8_t>(value >> (8 * i)));
}

bool valid_tag(const char* tag) {
    return tag && !std::memchr(tag, 0, 4);
}
}

namespace dh2::data {
bool serialize_player_profile_raw_sections_v1(
    const PlayerProfileIndexV1::Borrow& prior,
    const std::vector<PlayerProfileRawSectionV1>& replacements,
    std::vector<std::uint8_t>& output,
    std::string& error) {
    if (!prior) {
        error = "source profile required";
        return false;
    }

    std::map<std::string, std::vector<std::uint8_t>, std::less<>> sections;
    try {
        const auto& source = prior.bytes();
        for (const auto& row : prior.source_sections()) {
            if (!valid_tag(reinterpret_cast<const char*>(row.tag))) {
                error = "source saveAll embedded-NUL tag span unsupported";
                return false;
            }
            if (row.offset > source.size() || row.size > source.size() - row.offset) {
                error = "source section outside indexed profile";
                return false;
            }
            std::vector<std::uint8_t> payload;
            if (row.size)
                payload.assign(source.begin() + row.offset,
                               source.begin() + row.offset + row.size);
            sections[std::string(reinterpret_cast<const char*>(row.tag), 4)] =
                std::move(payload);
        }

        std::set<std::string, std::less<>> seen;
        for (const auto& replacement : replacements) {
            const auto* tag = replacement.tag.data();
            if (!valid_tag(tag)) {
                error = "raw profile tag must contain four non-NUL bytes";
                return false;
            }
            const std::string key(tag, replacement.tag.size());
            if (!seen.insert(key).second) {
                error = "duplicate raw profile replacement tag";
                return false;
            }
            if (replacement.payload.size && !replacement.payload.data) {
                error = "invalid raw profile payload span";
                return false;
            }
            if (replacement.payload.size > UINT32_MAX) {
                error = "raw profile section exceeds source size word";
                return false;
            }
            std::vector<std::uint8_t> payload;
            if (replacement.payload.size) {
                payload.assign(replacement.payload.data,
                               replacement.payload.data + replacement.payload.size);
            }
            sections[key] = std::move(payload);
        }

        if (sections.size() > UINT32_MAX) {
            error = "profile tag count exceeds source word";
            return false;
        }
        std::vector<std::uint8_t> candidate;
        append_word(candidate, static_cast<std::uint32_t>(sections.size()));
        for (const auto& section : sections) {
            if (section.second.size() > UINT32_MAX) {
                error = "profile section exceeds source size word";
                return false;
            }
            append_word(candidate, static_cast<std::uint32_t>(section.second.size()));
            candidate.insert(candidate.end(), section.first.begin(), section.first.end());
            candidate.insert(candidate.end(), section.second.begin(), section.second.end());
        }
        output.swap(candidate);
        error.clear();
        return true;
    } catch (...) {
        error = "raw profile assembly failed";
        return false;
    }
}
}
