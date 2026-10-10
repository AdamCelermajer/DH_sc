// P17 TRANSITIONS: see level_transitions_v1.hpp for the original rules this implements.
#include "level_transitions_v1.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace dh::foundation::transitions {
namespace {

std::string property(const ActorDefinition& declaration, const char* key) {
    const auto found = declaration.properties.find(key);
    return found == declaration.properties.end() ? std::string() : found->second;
}

bool parse_int(const std::string& text, int& out) {
    if (text.empty()) return false;
    char* end = nullptr;
    const long value = std::strtol(text.c_str(), &end, 10);
    if (end == text.c_str() || *end != '\0') return false;
    out = static_cast<int>(value);
    return true;
}

std::string declaration_label(const ActorDefinition& declaration) {
    return (declaration.name.empty() ? std::string("<unnamed>") : declaration.name) + " (" + declaration.gametype + ")";
}

// Authored Block shape only: the same restriction the campaign and quest zones use.
bool require_block(const ActorDefinition& declaration, std::string& reason) {
    const std::string shape = property(declaration, "type");
    if (shape == "Block") return true;
    reason = "shape '" + shape + "' not built (only Block)";
    return false;
}

bool build_level_exit(const ActorDefinition& declaration, const std::vector<LevelRow>& levels,
                      ExitZone& zone, std::string& reason) {
    if (!require_block(declaration, reason)) return false;
    const std::string target = property(declaration, "levelName");
    const LevelRow* row = find_level(levels, target);
    zone.target_name = target;
    if (!row) {
        reason = "levelName '" + target + "' is not in the level table";
        return false;
    }
    zone.target_file = level_uri(*row);
    zone.target_procedural = row->file.size() > 9 && row->file.compare(row->file.size() - 9, 9, ".rule.xml") == 0;
    const std::string entry = property(declaration, "entrypointID");
    if (!entry.empty() && !parse_int(entry, zone.entry_id)) {
        reason = "entrypointID '" + entry + "' is not an integer";
        return false;
    }
    zone.fast_travel = property(declaration, "fasttravel");
    // Authored condition: activate_cond first, then condition_desc. "Invalid" and empty mean unconditional.
    for (const char* key : {"activate_cond", "condition_desc"}) {
        const std::string value = property(declaration, key);
        if (!value.empty() && value != "Invalid") { zone.condition = value; break; }
    }
    zone.box = campaign_host::oriented_zone_box(declaration.placement);
    zone.kind = ZoneKind::level_exit;
    zone.built = true;
    return true;
}

bool build_checkpoint(const ActorDefinition& declaration, const std::vector<LevelRow>&,
                      ExitZone& zone, std::string& reason) {
    if (!require_block(declaration, reason)) return false;
    zone.box = campaign_host::oriented_zone_box(declaration.placement);
    zone.kind = ZoneKind::checkpoint;
    zone.built = true;
    return true;
}

bool build_delegated_quest(const ActorDefinition&, const std::vector<LevelRow>&, ExitZone& zone, std::string& reason) {
    zone.kind = ZoneKind::delegated_quest;
    reason = "QuestMoveInZone is built by features/quest_runtime/quest_zones_v1 (MoveInZone objective)";
    return false;
}

} // namespace

const char* zone_kind_name(ZoneKind kind) noexcept {
    switch (kind) {
        case ZoneKind::level_exit: return "level_exit";
        case ZoneKind::checkpoint: return "checkpoint";
        case ZoneKind::delegated_quest: return "delegated_quest";
        case ZoneKind::unsupported: return "unsupported";
    }
    return "unsupported";
}

std::vector<LevelRow> level_rows(const dh2::data::LevelTables& tables) {
    std::vector<LevelRow> rows;
    rows.reserve(tables.levels.size());
    for (std::size_t i = 0; i < tables.levels.size(); ++i) {
        LevelRow row;
        row.name = i < tables.level_names.size() ? tables.level_names[i] : std::string();
        row.file = tables.levels[i].file;
        row.description = tables.levels[i].description; // LevelList display text (prompt line 2)
        rows.push_back(std::move(row));
    }
    return rows;
}

const LevelRow* find_level(const std::vector<LevelRow>& rows, const std::string& name) noexcept {
    for (const auto& row : rows) if (!name.empty() && row.name == name) return &row;
    return nullptr;
}

std::string level_uri(const LevelRow& row) {
    return "data/scene/" + row.file;
}

bool LoggedConditionStub::condition_active(const std::string& name) {
    if (logged_.insert(name).second)
        std::cout << "Transition condition stub: '" << name << "' treated as active (CONDITIONS evaluator not wired)" << std::endl;
    return true;
}

ZoneClassRegistry::ZoneClassRegistry() {
    add("TriggerZoneExitLevel", build_level_exit);
    add("CheckpointZone", build_checkpoint);
    add("QuestMoveInZone", build_delegated_quest);
}

void ZoneClassRegistry::add(const std::string& gametype, ZoneBuilder builder) {
    builders_[gametype] = std::move(builder);
}

const ZoneBuilder* ZoneClassRegistry::find(const std::string& gametype) const noexcept {
    const auto found = builders_.find(gametype);
    return found == builders_.end() ? nullptr : &found->second;
}

bool ExitZoneSet::build(const std::vector<ActorDefinition>& declarations,
                        const std::vector<LevelRow>& levels,
                        const ZoneClassRegistry& registry,
                        std::string& error) {
    error.clear();
    zones_.clear();
    notices_.clear();
    primed_ = false;
    for (const auto& declaration : declarations) {
        const ZoneBuilder* builder = registry.find(declaration.gametype);
        if (!builder) continue;
        ExitZone zone;
        zone.name = declaration.name;
        zone.gametype = declaration.gametype;
        zone.source = declaration.sourcePath + "::" + declaration.name;
        std::string reason;
        const bool ok = (*builder)(declaration, levels, zone, reason);
        if (!ok) {
            zone.built = false;
            zone.note = reason;
            notices_.push_back(declaration_label(declaration) + ": " + reason);
        }
        zones_.push_back(std::move(zone));
    }
    return true;
}

std::vector<ZoneEvent> ExitZoneSet::update(const std::array<float, 3>& player, bool alive) {
    std::vector<ZoneEvent> events;
    for (std::size_t i = 0; i < zones_.size(); ++i) {
        auto& zone = zones_[i];
        if (!zone.built) continue;
        const bool now = alive && campaign_host::point_inside(zone.box, player);
        if (primed_ && now && !zone.inside) events.push_back(ZoneEvent{i, zone.kind});
        zone.inside = now;
    }
    primed_ = true;
    return events;
}

std::size_t ExitZoneSet::find(const std::string& name) const noexcept {
    for (std::size_t i = 0; i < zones_.size(); ++i) if (zones_[i].name == name) return i;
    return npos;
}

std::size_t ExitZoneSet::built_count(ZoneKind kind) const noexcept {
    std::size_t count = 0;
    for (const auto& zone : zones_) if (zone.built && zone.kind == kind) ++count;
    return count;
}

SpawnResolution resolve_entry_spawn(const std::vector<ActorDefinition>& declarations, int entry_id) {
    SpawnResolution result;
    for (const auto& declaration : declarations) {
        if (declaration.gametype != "SpawnPoint") continue;
        int value = 0;
        if (!parse_int(property(declaration, "entrypointID"), value) || value != entry_id) continue;
        ++result.candidates;
        if (result.found) continue;
        result.found = true;
        result.name = declaration.name;
        result.position = {declaration.placement[12], declaration.placement[13], declaration.placement[14]};
        result.heading_radians = std::atan2(declaration.placement[1], declaration.placement[0]);
    }
    return result;
}

} // namespace dh::foundation::transitions
