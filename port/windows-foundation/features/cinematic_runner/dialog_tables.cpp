#include "dialog_tables.hpp"

#include <cstring>

namespace dh::foundation::cinematic_runner {
namespace {
struct Reader {
    const std::vector<std::uint8_t>& bytes;
    std::size_t at = 0;
    bool word(std::uint32_t& v) {
        if (at > bytes.size() || bytes.size() - at < 4) return false;
        v = std::uint32_t(bytes[at]) | std::uint32_t(bytes[at + 1]) << 8 | std::uint32_t(bytes[at + 2]) << 16 | std::uint32_t(bytes[at + 3]) << 24;
        at += 4;
        return true;
    }
    bool integer(std::int32_t& v) {
        std::uint32_t w;
        if (!word(w)) return false;
        std::memcpy(&v, &w, 4);
        return true;
    }
    bool string(std::string& v) {
        std::uint32_t n;
        if (!word(n) || n > bytes.size() - at) return false;
        v.assign(reinterpret_cast<const char*>(bytes.data() + at), n);
        at += n;
        if (!v.empty() && v.back() == '\0') v.pop_back();
        return true;
    }
};
} // namespace

bool decode_dialog_actors(const std::vector<std::uint8_t>& bytes, std::vector<DialogActorRow>& rows, std::string& error) {
    rows.clear();
    Reader r{bytes};
    std::uint32_t count = 0;
    if (!r.word(count) || count > 65536) { error = "DialogActors: bad row count"; return false; }
    for (std::uint32_t i = 0; i < count; ++i) {
        DialogActorRow row;
        if (!r.integer(row.frame_id) || !r.string(row.frame_label) || !r.integer(row.name_string_id)) {
            error = "DialogActors: truncated row " + std::to_string(i);
            rows.clear();
            return false;
        }
        rows.push_back(std::move(row));
    }
    error.clear();
    return true;
}

bool decode_dialog_styles(const std::vector<std::uint8_t>& bytes, std::map<std::int32_t, std::string>& styles, std::string& error) {
    styles.clear();
    Reader r{bytes};
    std::uint32_t groups = 0;
    if (!r.word(groups) || groups > 64) { error = "DialogStyles: bad group count"; return false; }
    for (std::uint32_t g = 0; g < groups; ++g) {
        std::string group;
        std::uint32_t count = 0;
        if (!r.string(group) || !r.word(count) || count > 4096) { error = "DialogStyles: truncated group"; styles.clear(); return false; }
        for (std::uint32_t i = 0; i < count; ++i) {
            std::string name;
            std::int32_t value = 0;
            if (!r.string(name) || !r.integer(value)) { error = "DialogStyles: truncated constant"; styles.clear(); return false; }
            if (group == "DialogStyles") styles[value] = name;
        }
    }
    if (styles.empty()) { error = "DialogStyles group absent"; return false; }
    error.clear();
    return true;
}

} // namespace dh::foundation::cinematic_runner
