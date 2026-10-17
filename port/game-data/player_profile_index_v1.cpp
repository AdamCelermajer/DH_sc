#include "player_profile_index_v1.hpp"
#include <cstring>
#include <limits>
#include <map>
#include <stdexcept>

namespace {
std::uint32_t word(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | std::uint32_t(p[1]) << 8 |
           std::uint32_t(p[2]) << 16 | std::uint32_t(p[3]) << 24;
}
bool overlap(const void* a, std::size_t an, const void* b, std::size_t bn) {
    const auto x = reinterpret_cast<std::uintptr_t>(a);
    const auto y = reinterpret_cast<std::uintptr_t>(b);
    if (an > UINTPTR_MAX - x || bn > UINTPTR_MAX - y) return true;
    return an && bn && x < y + bn && y < x + an;
}
std::string key(const std::uint8_t* p) {
    std::size_t n = 0;
    while (n < 4 && p[n]) ++n;
    return {reinterpret_cast<const char*>(p), n};
}
}

extern "C" int dh2_player_profile_v1_index(
    dh2::data::ProfileIndexSpan24V1* p,
    const dh2::data::ProfileIndexServices16V1* services) noexcept {
    using namespace dh2::data;
    if (!p || !services || !services->section || p->reserved || p->cursor ||
        (!p->bytes && p->size) ||
        overlap(p, sizeof(*p), services, sizeof(*services)) ||
        overlap(p, sizeof(*p), p->bytes, p->size)) return -1;
    const auto s = *services;
    const auto bytes = p->bytes;
    const auto size = p->size;
    if (size < 4) return -2;
    const auto count = word(bytes);
    if (count == UINT32_MAX) return -4;
    p->source_count = count;
    p->cursor = 4;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (size - p->cursor < 4) return -2;
        ProfileSection12V1 section{};
        section.size = word(bytes + p->cursor);
        p->cursor += 4;
        if (size - p->cursor < 4) return -2;
        std::memcpy(section.tag, bytes + p->cursor, 4);
        p->cursor += 4;
        section.offset = p->cursor;
        if (section.size > size - p->cursor) return -2;
        try {
            if (!s.section(s.context, &section)) return -3;
        } catch (...) { return -3; }
        // Caller services may consume storage, but cannot replace the stream
        // controls that this invocation is already using.
        if (p->bytes != bytes || p->size != size || p->cursor != section.offset ||
            p->source_count != count || p->reserved) return -1;
        p->cursor += section.size;
    }
    return 0;
}

namespace dh2::data {
struct PlayerProfileIndexV1::Snapshot {
    std::vector<std::uint8_t> bytes;
    std::vector<ProfileSection12V1> sections;
    std::map<std::string, std::size_t, std::less<>> last;
};
bool PlayerProfileIndexV1::load(Bytes bytes, std::string& error) {
    if (snapshot_ && snapshot_.use_count() != 1) {
        error = "profile snapshot borrowed";
        return false;
    }
    if (bytes.size > UINT32_MAX || (!bytes.data && bytes.size)) {
        error = "invalid profile byte span";
        return false;
    }
    try {
        auto next = std::make_shared<Snapshot>();
        if (bytes.size) next->bytes.assign(bytes.data, bytes.data + bytes.size);
        ProfileIndexSpan24V1 span{next->bytes.data(),
            static_cast<std::uint32_t>(next->bytes.size()), 0, 0, 0};
        const ProfileIndexServices16V1 services{next.get(),
            [](void* opaque, const ProfileSection12V1* section) {
                auto& target = *static_cast<Snapshot*>(opaque);
                target.last[key(section->tag)] = target.sections.size();
                target.sections.push_back(*section);
                return true;
            }};
        const auto status = dh2_player_profile_v1_index(&span, &services);
        if (status) {
            error = "source campaign section index failed: " + std::to_string(status);
            return false;
        }
        snapshot_ = std::move(next);
        error.clear();
        return true;
    } catch (...) {
        error = "profile allocation failed";
        return false;
    }
}
const std::vector<std::uint8_t>& PlayerProfileIndexV1::Borrow::bytes() const {
    if (!snapshot_) throw std::logic_error("missing profile");
    return snapshot_->bytes;
}
const std::vector<ProfileSection12V1>& PlayerProfileIndexV1::Borrow::source_sections() const {
    if (!snapshot_) throw std::logic_error("missing profile");
    return snapshot_->sections;
}
const ProfileSection12V1* PlayerProfileIndexV1::Borrow::section(const char* tag) const noexcept {
    if (!snapshot_ || !tag) return nullptr;
    const auto found = snapshot_->last.find(tag);
    return found == snapshot_->last.end() ? nullptr : &snapshot_->sections[found->second];
}
Bytes PlayerProfileIndexV1::Borrow::payload(const char* tag) const noexcept {
    const auto found = section(tag);
    return found ? Bytes{snapshot_->bytes.data() + found->offset, found->size} : Bytes{nullptr, 0};
}
}
