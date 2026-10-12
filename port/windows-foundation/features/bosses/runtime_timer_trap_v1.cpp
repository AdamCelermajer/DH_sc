#include "runtime_timer_trap_v1.hpp"

#include "../../animation_markers.hpp"
#include "../../content_paths.hpp"
#include "../../embedded_scene_clips.hpp"

#include <algorithm>
#include <cstring>
#include <regex>

namespace dh::foundation::bosses {
namespace {
bool fail(std::string& error, const std::string& message) { error = message; return false; }

struct Cursor {
    dh2::data::Bytes bytes;
    std::size_t at = 0;
    bool word(std::uint32_t& v) {
        if (bytes.size - at < 4) return false;
        const auto* p = bytes.data + at;
        v = std::uint32_t(p[0]) | std::uint32_t(p[1]) << 8 | std::uint32_t(p[2]) << 16 | std::uint32_t(p[3]) << 24;
        at += 4;
        return true;
    }
    bool integer(std::int32_t& v) { std::uint32_t raw = 0; if (!word(raw)) return false; std::memcpy(&v, &raw, 4); return true; }
    bool text(std::string* out) {
        std::uint32_t n = 0;
        if (!word(n) || n > bytes.size - at) return false;
        if (out) out->assign(reinterpret_cast<const char*>(bytes.data + at), n);
        at += n;
        return true;
    }
};
// PyDataArrays.C1 addFuncsForFile order on game_objects and each Structs::*read shape (S len32 + bytes, b byte, i word),
// the same table as level-loader/game_object_arrays_owner_v81.cpp.
constexpr const char* kShapes[]{"iiiiiiibiiSiiii", "Siii", "iiSi", "iiiii", "i", "iibiSiii", "iiibiSii", "iiiSii", "iSii", "ibiiiii", "iiSii"};
constexpr std::size_t kDamagerGroup = 3, kTimerTrapGroup = 7;

std::string strip_comments(const std::string& text) {
    std::string out;
    for (std::size_t i = 0; i < text.size();) {
        if (text.compare(i, 2, "--") == 0) { const auto end = text.find('\n', i); i = end == std::string::npos ? text.size() : end; continue; }
        out.push_back(text[i++]);
    }
    return out;
}
} // namespace

bool load_timer_trap_tables_v1(dh2::data::Bytes records, dh2::data::Bytes names, std::vector<TimerTrapRowV1>& traps,
                               std::vector<GameObjectDamagerRowV1>& damagers, std::string& error) {
    if (!records.data || !names.data) return fail(error, "game_objects streams unavailable");
    Cursor r{records}, k{names};
    std::vector<TimerTrapRowV1> next_traps;
    std::vector<GameObjectDamagerRowV1> next_damagers;
    for (std::size_t group = 0; group < 11; ++group) {
        std::uint32_t count = 0, ncount = 0;
        if (!r.word(count) || !k.word(ncount) || count != ncount || count > 4096) return fail(error, "game_objects group count/name mismatch");
        std::vector<std::string> labels(ncount);
        for (auto& label : labels) if (!k.text(&label)) return fail(error, "short game_objects names");
        for (std::uint32_t i = 0; i < count; ++i) {
            if (group == kTimerTrapGroup) {
                TimerTrapRowV1 row;
                row.name = labels[i];
                if (!r.integer(row.damage) || !r.integer(row.delay_ms) || !r.integer(row.hurt_chance) || !r.text(&row.script) ||
                    !r.integer(row.sound) || !r.integer(row.visual)) return fail(error, "short TimerTraps row");
                next_traps.push_back(std::move(row));
            } else if (group == kDamagerGroup) {
                GameObjectDamagerRowV1 row;
                row.name = labels[i];
                if (!r.integer(row.damage) || !r.integer(row.immunity_delay) || !r.integer(row.max) || !r.integer(row.min) || !r.integer(row.type))
                    return fail(error, "short GameObjectDamager row");
                next_damagers.push_back(std::move(row));
            } else {
                for (const char* s = kShapes[group]; *s; ++s) {
                    std::uint32_t ignored = 0;
                    if (*s == 'S') { if (!r.text(nullptr)) return fail(error, "short game_objects CString"); }
                    else if (*s == 'b') { if (r.at >= records.size) return fail(error, "short game_objects bool"); ++r.at; }
                    else if (!r.word(ignored)) return fail(error, "short game_objects word");
                }
            }
        }
    }
    if (r.at != records.size || k.at != names.size) return fail(error, "unexpected game_objects stream tail");
    traps = std::move(next_traps);
    damagers = std::move(next_damagers);
    error.clear();
    return true;
}

bool parse_timer_trap_script_v1(const std::string& source, TimerTrapScriptV1& output, std::string& error) {
    const auto text = strip_comments(source);
    TimerTrapScriptV1 next;
    std::smatch m;
    if (!std::regex_search(text, m, std::regex("local\\s+COUNTER_DELAY\\s*=\\s*([0-9]+)"))) return fail(error, "trap script has no COUNTER_DELAY");
    next.counter_limit = std::stoi(m[1].str());
    if (!std::regex_search(text, m, std::regex("event\\s*==\\s*\"(\\w+)\"[\\s\\S]*?g_counter\\s*>=\\s*COUNTER_DELAY[\\s\\S]*?Remove\\(\\)")))
        return fail(error, "trap script has no counted Remove()");
    next.count_event = m[1].str();
    if (std::regex_search(text, m, std::regex("DealDamages\\(\\s*victim\\s*,\\s*GetPyOID\\(\\s*\"GameObjectDamager\"\\s*,\\s*\"(\\w+)\"\\s*\\)\\s*,\\s*GetPyCst\\(\\s*\"Elemental\"\\s*,\\s*\"(\\w+)\"")))
    { next.damage_damager = m[1].str(); next.damage_element = m[2].str(); }
    output = std::move(next);
    error.clear();
    return true;
}

bool load_timer_trap_model_v1(const AssetCatalog& assets, const std::string& uri, TimerTrapModelV1& output, std::string& error) {
    try {
        const auto bytes = read_content(assets, uri);
        std::vector<EmbeddedSceneClip> clips;
        if (!decode_embedded_scene_clips(bytes.data(), bytes.size(), clips, error)) return false;
        TimerTrapModelV1 next;
        next.uri = uri;
        bool idle = false, activate = false, remove = false;
        for (const auto& clip : clips) {
            if (clip.name == "idle") { next.idle_start = clip.start_ms; next.idle_end = clip.end_ms; idle = true; }
            else if (clip.name == "activate") { next.activate_start = clip.start_ms; next.activate_end = clip.end_ms; activate = true; }
            else if (clip.name == "remove") { next.remove_start = clip.start_ms; next.remove_end = clip.end_ms; remove = true; }
        }
        if (!idle || !activate || !remove) return fail(error, "TimerTrap model has no idle/activate/remove clips: " + uri);
        AnimationMarkers markers;
        std::int32_t last = std::max({next.idle_end, next.activate_end, next.remove_end});
        if (!markers.load(bytes.data(), bytes.size(), 0, last + 1, error)) return false;
        for (const auto& marker : markers.markers()) next.events.push_back({marker.name, marker.time_ms});
        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& problem) {
        return fail(error, problem.what());
    }
}

std::uint64_t RuntimeTimerTrapsV1::spawn(const TimerTrapDefinitionV1& def, const std::array<float, 3>& at) {
    Trap trap;
    trap.id = next_++;
    trap.def = def;
    trap.at = at;
    traps_.push_back(std::move(trap));
    log_.push_back("TimerTrap_" + std::to_string(traps_.back().id) + " " + def.row.name + " created at " + std::to_string(at[0]) + "," +
                   std::to_string(at[1]) + "," + std::to_string(at[2]) + " delay=" + std::to_string(def.row.delay_ms));
    return traps_.back().id;
}

void RuntimeTimerTrapsV1::update(std::int32_t dt_ms) {
    if (dt_ms <= 0) return;
    for (auto it = traps_.begin(); it != traps_.end();) {
        auto& t = *it;
        const auto& m = t.def.model;
        t.age_ms += dt_ms;
        const std::int32_t before = t.clip_ms;
        t.clip_ms += dt_ms;
        bool erase = false;
        if (t.phase == Phase::activate) {
            // Authored events of the activate span reach OnAnimEvent (count event -> counter / Remove).
            for (const auto& event : m.events) {
                const auto offset = event.second - m.activate_start;
                if (event.second < m.activate_start || event.second > m.activate_end || offset <= before || offset > t.clip_ms) continue;
                if (event.first == t.def.script.count_event) {
                    if (t.counter >= t.def.script.counter_limit) {
                        t.removing = true;
                        log_.push_back("TimerTrap_" + std::to_string(t.id) + " Remove() after " + std::to_string(t.activations) + " activations");
                    } else {
                        ++t.counter;
                    }
                }
            }
            if (t.removing) { t.phase = Phase::remove; t.clip_ms = 0; }
            else if (t.clip_ms >= m.activate_end - m.activate_start) { t.phase = Phase::idle; t.clip_ms = 0; }
        } else if (t.phase == Phase::remove) {
            if (t.clip_ms >= m.remove_end - m.remove_start) erase = true;
        }
        if (!t.removing && !erase) {
            // TimerTrap::SpecificUpdate: Activate every Delay ms.
            t.since_activate_ms += dt_ms;
            if (t.def.row.delay_ms > 0 && t.since_activate_ms >= t.def.row.delay_ms) {
                t.since_activate_ms -= t.def.row.delay_ms;
                t.phase = Phase::activate;
                t.clip_ms = 0;
                ++t.activations;
            }
        }
        if (erase) {
            log_.push_back("TimerTrap_" + std::to_string(t.id) + " deleted age_ms=" + std::to_string(t.age_ms));
            it = traps_.erase(it);
            continue;
        }
        ++it;
    }
}

std::vector<RuntimeTimerTrapsV1::View> RuntimeTimerTrapsV1::views() const {
    std::vector<View> out;
    for (const auto& t : traps_) {
        const auto& m = t.def.model;
        View v;
        v.id = t.id;
        v.model_uri = m.uri;
        v.position = t.at;
        v.age_ms = t.age_ms;
        if (t.phase == Phase::idle) {
            const auto span = std::max(1, m.idle_end - m.idle_start);
            v.source_ms = m.idle_start + t.clip_ms % span;
        } else if (t.phase == Phase::activate) {
            v.source_ms = m.activate_start + std::min(t.clip_ms, m.activate_end - m.activate_start);
        } else {
            v.source_ms = m.remove_start + std::min(t.clip_ms, m.remove_end - m.remove_start);
        }
        out.push_back(v);
    }
    return out;
}

std::vector<std::string> RuntimeTimerTrapsV1::take_log() {
    std::vector<std::string> out;
    out.swap(log_);
    return out;
}

} // namespace dh::foundation::bosses
