#include "quest_zones_v1.hpp"
#include "quest_runtime_v1.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <set>
#include <sstream>

namespace dh::foundation::quest_runtime {
namespace {

constexpr std::int32_t kMoveInZoneType = static_cast<std::int32_t>(QuestObjectiveTypeV1::move_in_zone);

bool parse_vec3_text(const std::string& text, std::array<float, 3>& out) {
    std::stringstream in(text);
    std::string part;
    std::array<float, 3> value{};
    for (std::size_t i = 0; i < 3; ++i) {
        if (!std::getline(in, part, ',')) return false;
        char* end = nullptr;
        value[i] = std::strtof(part.c_str(), &end);
        if (end == part.c_str() || !std::isfinite(value[i])) return false;
    }
    if (std::getline(in, part, ',')) return false;
    out = value;
    return true;
}

} // namespace

QuestZoneDeclarationV1 make_quest_zone_declaration_v1(const std::string& name,
    const std::map<std::string, std::string>& properties, const std::array<float, 3>& position) {
    QuestZoneDeclarationV1 out;
    out.name = name;
    out.position = position;
    const auto property = [&](const char* key) -> std::string {
        const auto found = properties.find(key);
        return found == properties.end() ? std::string() : found->second;
    };
    out.shape = property("type");
    if (!property("scale").empty()) parse_vec3_text(property("scale"), out.scale);
    for (auto& s : out.scale) if (std::fabs(s) < 0.0001f) s = 1.0f; // same rule as the actor transform
    if (!property("rotation").empty()) parse_vec3_text(property("rotation"), out.rotation);
    return out;
}

std::vector<std::string> quest_zone_names_v1(const QuestTableV1& table) {
    std::set<std::string> names;
    for (const auto& row : table.rows()) {
        const auto add = [&](const dh2::data::QuestObjectiveDefinitionV51& objective) {
            if (objective.type == kMoveInZoneType && !objective.str2.empty()) names.insert(objective.str2);
        };
        add(row.accept);
        for (const auto& objective : row.objectives) add(objective);
    }
    return std::vector<std::string>(names.begin(), names.end());
}

void QuestZoneSetV1::build(const QuestTableV1& table, const std::vector<QuestZoneDeclarationV1>& declarations) {
    zones_.clear();
    notes_.clear();
    primed_ = false;
    for (const auto& name : quest_zone_names_v1(table)) {
        // Level declarations are searched by authored name; the first Block wins.
        const QuestZoneDeclarationV1* match = nullptr;
        std::size_t count = 0;
        for (const auto& declaration : declarations) {
            if (declaration.name != name) continue;
            ++count;
            if (!match) match = &declaration;
        }
        if (!match) continue; // a zone of another level: nothing to report on every level
        if (match->shape != "Block") {
            notes_.push_back("quest zone " + name + " shape '" + match->shape + "' not supported");
            continue;
        }
        if (match->rotation != std::array<float, 3>{0, 0, 0}) {
            notes_.push_back("quest zone " + name + " is rotated (source rotation convention not verified); not built");
            continue;
        }
        if (count > 1) notes_.push_back("quest zone " + name + " has " + std::to_string(count) + " declarations; the first is used");
        QuestZoneV1 zone;
        zone.name = name;
        for (std::size_t i = 0; i < 3; ++i) {
            const float half = std::fabs(match->scale[i]) * 100.0f;
            zone.min[i] = match->position[i] - half;
            zone.max[i] = match->position[i] + half;
        }
        zones_.push_back(std::move(zone));
    }
}

std::vector<std::string> QuestZoneSetV1::update(const std::array<float, 3>& player) {
    std::vector<std::string> entered;
    for (auto& zone : zones_) {
        bool inside = true;
        for (std::size_t i = 0; i < 3; ++i) {
            if (!(zone.min[i] <= player[i] && player[i] <= zone.max[i])) { inside = false; break; }
        }
        if (inside && !zone.inside && primed_) entered.push_back(zone.name);
        zone.inside = inside;
    }
    primed_ = true;
    return entered;
}

} // namespace dh::foundation::quest_runtime
