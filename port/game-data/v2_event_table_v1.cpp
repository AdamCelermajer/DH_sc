#include "v2_event_table_v1.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <unordered_set>

namespace dh2::data::v2_event_table_v1 {
namespace {

struct Reader {
    Bytes bytes;
    std::uint32_t offset = 0;

    bool u32(std::uint32_t& value) {
        if (!bytes.data || offset > bytes.size || bytes.size - offset < 4) return false;
        const auto* p = bytes.data + offset;
        offset += 4;
        value = std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
                (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
        return true;
    }

    bool i32(std::int32_t& value) {
        std::uint32_t raw = 0;
        if (!u32(raw)) return false;
        std::memcpy(&value, &raw, sizeof(value));
        return true;
    }

    bool text(std::string& value, bool allow_empty, std::uint32_t limit = 4096) {
        std::uint32_t size = 0;
        if (!u32(size) || (!allow_empty && size == 0) || size > limit ||
            offset > bytes.size || size > bytes.size - offset) return false;
        const auto* begin = bytes.data + offset;
        if (std::find(begin, begin + size, std::uint8_t(0)) != begin + size) return false;
        value.assign(reinterpret_cast<const char*>(begin), size);
        offset += size;
        return true;
    }

    bool done() const noexcept { return offset == bytes.size; }
};

bool parse_names(Bytes input, std::vector<std::string>& out) {
    Reader r{input};
    std::uint32_t count = 0;
    if (!r.u32(count) || count == 0 || count > 4096) return false;
    std::vector<std::string> parsed;
    parsed.reserve(count);
    std::unordered_set<std::string> unique;
    for (std::uint32_t i = 0; i < count; ++i) {
        std::string name;
        if (!r.text(name, false, 255) || !unique.insert(name).second) return false;
        parsed.push_back(std::move(name));
    }
    if (!r.done()) return false;
    out = std::move(parsed);
    return true;
}

bool parse_states(Bytes input, EventStates& out) {
    Reader r{input};
    std::uint32_t groups = 0;
    if (!r.u32(groups) || groups != 1) return false;
    std::string group;
    std::uint32_t count = 0;
    if (!r.text(group, false, 128) || group != "v2EventState" ||
        !r.u32(count) || count != 4) return false;
    EventStates parsed{};
    std::uint32_t seen = 0;
    for (std::uint32_t i = 0; i < count; ++i) {
        std::string name;
        std::int32_t value = 0;
        if (!r.text(name, false, 128) || !r.i32(value)) return false;
        if (name == "Active" && !(seen & 1u)) { parsed.active = value; seen |= 1u; }
        else if (name == "Completed" && !(seen & 2u)) { parsed.completed = value; seen |= 2u; }
        else if (name == "Count" && !(seen & 4u)) { parsed.count = value; seen |= 4u; }
        else if (name == "Inactive" && !(seen & 8u)) { parsed.inactive = value; seen |= 8u; }
        else return false;
    }
    if (!r.done() || seen != 15u || parsed.active != 1 || parsed.completed != 2 ||
        parsed.count != 3 || parsed.inactive != 0) return false;
    out = parsed;
    return true;
}

bool parse_trigger(Reader& r, Trigger& out) {
    Trigger parsed;
    if (!r.i32(parsed.type) || !r.i32(parsed.op1) || !r.i32(parsed.op2) ||
        !r.text(parsed.text1, true) || !r.text(parsed.text2, true) ||
        !r.i32(parsed.value1) || !r.i32(parsed.value2) || !r.i32(parsed.value3)) return false;
    out = std::move(parsed);
    return true;
}

} // namespace

bool load(Bytes packed, Bytes names, Bytes constants, Table& out, std::string& error) {
    constexpr std::uint32_t max_blob = 1024u * 1024u;
    if (!packed.data || !names.data || !constants.data || packed.size < 4 ||
        packed.size > max_blob || names.size > max_blob || constants.size > max_blob) {
        error = "invalid v2Event table input";
        return false;
    }

    std::vector<std::string> parsed_names;
    EventStates states;
    if (!parse_names(names, parsed_names) || !parse_states(constants, states)) {
        error = "v2Event names or v2EventState constants are malformed";
        return false;
    }

    Reader r{packed};
    std::uint32_t count = 0;
    if (!r.u32(count) || count == 0 || count != parsed_names.size() || count > 4096) {
        error = "v2Event row count does not match names";
        return false;
    }

    Table parsed;
    parsed.rows.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        Event event;
        std::uint32_t trigger_count = 0;
        if (!r.i32(event.level) || !r.text(event.script, false, 4096) ||
            !r.u32(trigger_count) || trigger_count > 4096) {
            error = "v2Event row header is truncated or out of bounds";
            return false;
        }
        event.name = std::move(parsed_names[i]);
        event.triggers.reserve(trigger_count);
        for (std::uint32_t j = 0; j < trigger_count; ++j) {
            Trigger trigger;
            if (!parse_trigger(r, trigger)) {
                error = "v2Event trigger is truncated or malformed";
                return false;
            }
            if (trigger.type < static_cast<std::int32_t>(ObjectiveType::kill_x_enemies) ||
                trigger.type > static_cast<std::int32_t>(ObjectiveType::invalid)) {
                error = "v2Event trigger objective type is unsupported";
                return false;
            }
            event.triggers.push_back(std::move(trigger));
        }
        parsed.rows.push_back(std::move(event));
    }

    if (!r.done()) {
        error = "v2Event table has trailing bytes";
        return false;
    }
    parsed.states = states;
    parsed.packed_bytes_consumed = r.offset;
    out = std::move(parsed);
    error.clear();
    return true;
}

const Event* find(const Table& table, const char* name) noexcept {
    if (!name) return nullptr;
    const auto found = std::find_if(table.rows.begin(), table.rows.end(), [name](const Event& event) {
        return event.name == name;
    });
    return found == table.rows.end() ? nullptr : &*found;
}

} // namespace dh2::data::v2_event_table_v1
