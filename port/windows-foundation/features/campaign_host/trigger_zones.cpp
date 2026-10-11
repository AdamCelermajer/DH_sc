#include "trigger_zones.hpp"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <set>
#include <sstream>

namespace dh::foundation::campaign_host {
namespace {

std::string lower_basename(const std::string& path) {
    const auto cut = path.find_last_of("/\\");
    std::string name = cut == std::string::npos ? path : path.substr(cut + 1);
    for (auto& c : name) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return name;
}

// Source fields that the runtime cannot honour (it fails the executor if they are present).
constexpr const char* kProviderFields[] = {"script_all_player", "script_all_player_move_out", "effect_one_player", "is_door_closed"};

} // namespace

TriggerZoneBox zone_box(const std::array<float,3>& position,const std::array<float,3>& scale) noexcept {
    TriggerZoneBox box;
    for (unsigned i = 0; i < 3; ++i) {
        const float half = std::fabs(scale[i]) * 100.0f;
        box.min[i] = position[i] - half;
        box.max[i] = position[i] + half;
        box.centre[i] = position[i];
        box.half[i] = half;
    }
    return box;
}

TriggerZoneBox oriented_zone_box(const std::array<float,16>& m) noexcept {
    TriggerZoneBox box;
    for (unsigned a = 0; a < 3; ++a) {
        const std::array<float,3> column{m[4 * a], m[4 * a + 1], m[4 * a + 2]};
        const float length = std::sqrt(column[0] * column[0] + column[1] * column[1] + column[2] * column[2]);
        box.half[a] = 100.0f * length;
        for (unsigned k = 0; k < 3; ++k) box.axes[a][k] = length > 0.0f ? column[k] / length : (a == k ? 1.0f : 0.0f);
    }
    for (unsigned i = 0; i < 3; ++i) box.centre[i] = m[12 + i];
    // Axis-aligned bounds of the oriented box: extent along world axis i is sum over box axes of |axis_i| * half.
    for (unsigned i = 0; i < 3; ++i) {
        float extent = 0.0f;
        for (unsigned a = 0; a < 3; ++a) extent += std::fabs(box.axes[a][i]) * box.half[a];
        box.min[i] = box.centre[i] - extent;
        box.max[i] = box.centre[i] + extent;
    }
    return box;
}

bool point_inside(const TriggerZoneBox& b,const std::array<float,3>& p) noexcept {
    const std::array<float,3> d{p[0] - b.centre[0], p[1] - b.centre[1], p[2] - b.centre[2]};
    for (unsigned i = 0; i < 3; ++i) {
        const auto& a = b.axes[i];
        const float along = d[0] * a[0] + d[1] * a[1] + d[2] * a[2];
        if (std::fabs(along) > b.half[i]) return false;
    }
    return true;
}

bool parse_vec3(const std::string& text,std::array<float,3>& out) noexcept {
    std::stringstream in(text);
    std::string part;
    std::array<float,3> value{};
    for (unsigned i = 0; i < 3; ++i) {
        if (!std::getline(in, part, ',')) return false;
        char* end = nullptr;
        value[i] = std::strtof(part.c_str(), &end);
        if (end == part.c_str() || !std::isfinite(value[i])) return false;
    }
    if (std::getline(in, part, ',')) return false;
    out = value;
    return true;
}

bool TriggerZoneSet::build(const std::vector<ActorDefinition>& declarations,OriginalCampaignRuntime& runtime,std::string& error) {
    zones_.clear();
    skipped_.clear();
    error.clear();
    std::set<std::string> used_keys;
    for (const auto& declaration : declarations) {
        if (declaration.gametype != "TriggerZone") continue;
        const auto property = [&](const char* key) -> std::string {
            const auto found = declaration.properties.find(key);
            return found == declaration.properties.end() ? std::string() : found->second;
        };
        const std::string script = property("script");
        if (script.empty()) continue; // a zone that raises nothing needs no host state
        const std::string label = (declaration.name.empty() ? std::string("<unnamed>") : declaration.name) +
                                  " (" + lower_basename(declaration.sourcePath) + ") script=" + script + ": ";
        const auto reject = [&](const std::string& why) { skipped_.push_back(label + why); };

        if (property("type") != "Block") { reject("shape '" + property("type") + "' not supported"); continue; }
        // P16 CINE2: the placed transform carries position (with the module offset), rotation and scale, so the
        // zone box is oriented by it. Rotation is validated here; the Euler convention is the one actor transform()
        // already uses for every placed object.
        std::array<float,3> rotation{}, scale{1,1,1};
        if (!property("rotation").empty() && !parse_vec3(property("rotation"), rotation)) { reject("invalid rotation"); continue; }
        if (!property("scale").empty() && !parse_vec3(property("scale"), scale)) { reject("invalid scale"); continue; }
        if (!property("activate_cond").empty()) { reject("activate_cond is not evaluated by the host"); continue; }
        bool provider_field = false;
        for (const char* field : kProviderFields) if (!property(field).empty()) { reject(std::string("requires source provider ") + field); provider_field = true; break; }
        if (provider_field) continue;
        if (runtime.script_id(script, false) < 0) { reject("script not in the loaded level bank"); continue; }
        if (!property("script_move_out").empty() && runtime.script_id(property("script_move_out"), false) < 0) {
            reject("move-out script not in the loaded level bank");
            continue;
        }

        // Trigger record: the extracted campaign XML when it carries this occurrence, otherwise the declaration itself.
        std::vector<std::string> matches;
        for (const auto& record : runtime.triggers()) {
            const auto split = record.first.rfind("::");
            if (split == std::string::npos) continue;
            if (record.first.substr(split + 2) != declaration.name) continue;
            if (lower_basename(record.first.substr(0, split)) != lower_basename(declaration.sourcePath)) continue;
            matches.push_back(record.first);
        }
        std::string key;
        if (matches.size() > 1) { reject("ambiguous campaign trigger records"); continue; }
        if (matches.size() == 1) key = matches.front();
        else {
            key = declaration.sourcePath + "::" + declaration.name;
            if (!runtime.register_trigger(key, declaration.properties, error)) { reject("trigger registration failed: " + error); error.clear(); continue; }
        }
        if (!used_keys.insert(key).second) { reject("duplicate occurrence of the same trigger key (instance keys not supported)"); continue; }

        TriggerZone zone;
        zone.key = key;
        zone.name = declaration.name;
        zone.script = script;
        zone.script_id = runtime.script_id(script, false);
        zone.box = oriented_zone_box(declaration.placement);
        zones_.push_back(std::move(zone));
    }
    return true;
}

void TriggerZoneSet::update(OriginalCampaignRuntime& runtime,const std::array<float,3>& player,bool qualified,int module) {
    for (auto& zone : zones_) {
        if (zone.disabled) continue;
        zone.inside = point_inside(zone.box, player);
        std::string error;
        if (!runtime.trigger_contact(zone.key, zone.inside, qualified, module, error)) {
            zone.disabled = true;
            std::cout << "[campaign] trigger " << zone.name << " disabled after contact error: " << error << '\n';
            continue;
        }
        const bool running = runtime.running(zone.script_id);
        if (running && !zone.was_running) std::cout << "[campaign] trigger " << zone.name << " -> script " << zone.script << " started\n";
        if (!running && zone.was_running) std::cout << "[campaign] script " << zone.script << " finished\n";
        zone.was_running = running;
    }
}

} // namespace dh::foundation::campaign_host
