#include "actor_profiles.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"
#include <cmath>
#include <set>
#include <stdexcept>

namespace dh::foundation {
namespace {
std::string field(const TiXmlElement& node, const char* name, bool required = false) {
    const char* value = node.Attribute(name);
    if (required && (!value || !*value))
        throw std::runtime_error(std::string("Actor profile missing ") + name);
    return value ? value : "";
}
std::string uri(const TiXmlElement& node, const char* name, bool required = false) {
    auto value = field(node, name, required);
    return value.empty() ? value : normalize_content_uri(value);
}
std::string resolved(const AssetCatalog& assets, const std::string& value,
                     const std::string& owner) {
    return resolve_content_path(assets, value, owner).lexically_relative(assets.root()).generic_string();
}
}
bool ActorProfileLibrary::load(const AssetCatalog& assets, const std::string& path, std::string& error) {
    try {
        auto bytes = read_content(assets, path);
        if (bytes.empty() || bytes.size() > 8 * 1024 * 1024)
            throw std::runtime_error("Actor profiles XML is empty or exceeds 8 MiB");
        std::string xml(bytes.begin(), bytes.end());
        if (xml.find('\0') != std::string::npos || xml.find("<!") != std::string::npos)
            throw std::runtime_error("Actor profiles XML contains unsupported declarations or NUL");
        TiXmlDocument document;
        document.Parse(xml.c_str());
        if (document.Error()) throw std::runtime_error(document.ErrorDesc());
        auto root = document.RootElement();
        if (!root || std::string(root->Value()) != "actorProfiles" || field(*root, "version") != "1")
            throw std::runtime_error("Expected actorProfiles version 1");
        std::map<std::string, ActorProfile> next;
        for (auto entry = root->FirstChildElement(); entry; entry = entry->NextSiblingElement()) {
            if (std::string(entry->Value()) != "actor") throw std::runtime_error("Unknown actorProfiles entry");
            ActorProfile profile;
            profile.id = field(*entry, "id", true);
            profile.character_uri = uri(*entry, "character");
            profile.model_uri = uri(*entry, "model", true);
            profile.template_clip_uri = uri(*entry, "template");
            profile.property_row = field(*entry, "propertyRow");
            profile.animation_table = field(*entry, "animationTable");
            profile.animation_table_name = field(*entry, "animationTableName");
            for (auto child = entry->FirstChildElement(); child; child = child->NextSiblingElement()) {
                const std::string tag = child->Value();
                if (tag == "state") {
                    auto name = field(*child, "name", true);
                    if (profile.states.count(name)) throw std::runtime_error("Duplicate actor state: " + name);
                    std::vector<ActorClip> clips;
                    for (auto clip = child->FirstChildElement(); clip; clip = clip->NextSiblingElement()) {
                        if (std::string(clip->Value()) != "clip") throw std::runtime_error("Unknown actor state child");
                        ActorClip value;
                        value.uri = uri(*clip, "uri", true);
                        value.weight_authored = field(*clip, "weightAuthored") == "true";
                        if (clip->QueryDoubleAttribute("weight", &value.weight) != TIXML_SUCCESS ||
                            !std::isfinite(value.weight) || value.weight < 0)
                            throw std::runtime_error("Invalid actor clip weight");
                        clips.push_back(std::move(value));
                    }
                    profile.states.emplace(name, std::move(clips));
                } else if (tag == "property") {
                    auto name = field(*child, "name", true);
                    auto raw = field(*child, "raw", true);
                    std::size_t consumed = 0;
                    ActorRawProperty value;
                    value.raw = std::stoll(raw, &consumed, 10);
                    if (consumed != raw.size()) throw std::runtime_error("Invalid raw actor property");
                    value.encoding = field(*child, "encoding", true);
                    if (!profile.raw_properties.emplace(name, value).second)
                        throw std::runtime_error("Duplicate raw actor property: " + name);
                } else throw std::runtime_error("Unknown actor profile child: " + tag);
            }
            if (!next.emplace(profile.id, std::move(profile)).second)
                throw std::runtime_error("Duplicate actor profile ID");
            if (next.size() > 4096) throw std::runtime_error("Actor profile count exceeds limit");
        }
        if (next.empty()) throw std::runtime_error("Actor profile library is empty");
        profiles_ = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& exception) { error = exception.what(); return false; }
}
// P16 PROFILES: derived profiles (see features/spawn/actor_profile_derivation_v1.hpp).
bool ActorProfileLibrary::add_derived(ActorProfile profile, std::string& error) {
    if (profile.id.empty()) { error = "Derived actor profile needs an ID"; return false; }
    if (profiles_.count(profile.id)) { error = "Actor profile already published: " + profile.id; return false; }
    const auto id = profile.id;
    profiles_.emplace(id, std::move(profile));
    error.clear();
    return true;
}
const ActorProfile* ActorProfileLibrary::find(const std::string& id) const noexcept {
    auto found = profiles_.find(id);
    return found == profiles_.end() ? nullptr : &found->second;
}
bool make_visual_config(const AssetCatalog& assets, const ActorProfile& profile,
                        const ActorCustomization& customization, CharacterVisualConfig& output,
                        std::string& error) {
    try {
        CharacterVisualConfig result;
        result.model_path = resolved(assets, profile.model_uri, profile.character_uri);
        if (!profile.template_clip_uri.empty())
            result.template_clip_path = resolved(assets, profile.template_clip_uri, profile.character_uri);
        std::set<std::string> names;
        for (const auto& state : profile.states) {
            for (std::size_t index = 0; index < state.second.size(); ++index) {
                auto name = state.first + (index ? "#" + std::to_string(index + 1) : "");
                if (!names.insert(name).second) throw std::runtime_error("Actor clip alias collision: " + name);
                result.clips.emplace_back(name, resolved(assets, state.second[index].uri, profile.character_uri));
            }
        }
        result.skin_id_contains = customization.skin_id_contains;
        result.controller_ids=customization.controller_ids;
        result.use_authored_modular_defaults=customization.use_authored_modular_defaults;
        result.expected_controller_count = customization.expected_controller_count;
        result.include_static_instances = customization.include_static_instances;
        result.allow_missing_animation_targets = customization.allow_missing_animation_targets;
        output = std::move(result);
        error.clear();
        return true;
    } catch (const std::exception& exception) { error = exception.what(); return false; }
}
} // namespace dh::foundation
