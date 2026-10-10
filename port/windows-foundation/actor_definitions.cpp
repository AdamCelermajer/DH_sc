#include "actor_definitions.hpp"
#include "level_manifest.hpp"
#include "content_paths.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"
#include "../engine-math/math.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <locale>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace dh::foundation {
namespace {
using Properties = std::map<std::string, std::string>;
constexpr std::size_t maximum_bytes = 8 * 1024 * 1024;
constexpr std::size_t maximum_actors = 100000;

std::string value(const Properties& properties, const char* key) {
    const auto found = properties.find(key);
    return found == properties.end() ? "" : found->second;
}
std::string first(const Properties& properties, std::initializer_list<const char*> keys) {
    for (const char* key : keys) { auto result = value(properties, key); if (!result.empty()) return result; }
    return {};
}
Properties attributes(const TiXmlElement& element) {
    Properties result;
    for (const TiXmlAttribute* attr = element.FirstAttribute(); attr; attr = attr->Next())
        result.emplace(attr->Name(), attr->Value());
    // Property-table children occur in native templates as name/value entries.
    for (const TiXmlElement* child = element.FirstChildElement(); child; child = child->NextSiblingElement()) {
        const std::string tag = child->Value();
        if (tag != "Property" && tag != "property") continue;
        const char* name = child->Attribute("name");
        const char* property = child->Attribute("value");
        if (name && property) result[name] = property;
    }
    return result;
}
bool resolve(const AssetCatalog& assets, const std::string& authored,
             const std::string& source, std::string& resolved) {
    try {
        resolved = resolve_content_path(assets, authored, source).lexically_relative(assets.root()).generic_string();
        return true;
    } catch (const std::invalid_argument&) { throw; }
    catch (const std::exception&) { return false; }
}

// Limit TinyXML recursion before Parse. Full XML validation remains TinyXML's.
void bounds(std::string_view text) {
    std::size_t depth = 0, nodes = 0;
    for (std::size_t at = 0; at < text.size();) {
        const auto start = text.find('<', at);
        if (start == std::string_view::npos) break;
        std::string_view ending;
        std::size_t skip = 0;
        if (text.substr(start, 4) == "<!--") { ending = "-->"; skip = 4; }
        else if (text.substr(start, 9) == "<![CDATA[") { ending = "]]>"; skip = 9; }
        else if (text.substr(start, 2) == "<?") { ending = "?>"; skip = 2; }
        else if (text.substr(start, 2) == "<!") throw std::runtime_error("Actor XML DTD is unsupported");
        if (skip) {
            const auto end = text.find(ending, start + skip);
            if (end == std::string_view::npos) throw std::runtime_error("Unterminated actor XML declaration/comment");
            at = end + ending.size(); continue;
        }
        std::size_t end = start + 1;
        char quote = 0;
        for (; end < text.size(); ++end) {
            char c = text[end];
            if (quote) { if (c == quote) quote = 0; }
            else if (c == '\'' || c == '"') quote = c;
            else if (c == '>') break;
        }
        if (end == text.size()) throw std::runtime_error("Unterminated actor XML tag");
        if (text[start + 1] == '/') {
            if (!depth) throw std::runtime_error("Unexpected actor XML closing tag");
            --depth;
        } else {
            if (++nodes > maximum_actors || ++depth > 128) throw std::runtime_error("Actor XML element/depth limit exceeded");
            std::size_t previous = end;
            while (previous > start && std::isspace(static_cast<unsigned char>(text[previous-1]))) --previous;
            if (text[previous-1] == '/') --depth;
        }
        at = end + 1;
    }
}
std::unique_ptr<TiXmlDocument> document(const AssetCatalog& assets, const std::string& path) {
    if (std::filesystem::file_size(assets.resolve(path)) > maximum_bytes) throw std::runtime_error("Actor XML exceeds byte limit: " + path);
    const auto bytes = assets.read(path);
    if (bytes.empty() || std::find(bytes.begin(), bytes.end(), 0) != bytes.end()) throw std::runtime_error("Empty or NUL-containing actor XML: " + path);
    const std::string text(bytes.begin(), bytes.end());
    bounds(text);
    auto result = std::make_unique<TiXmlDocument>();
    result->Parse(text.c_str());
    if (result->Error()) throw std::runtime_error(path + ": " + result->ErrorDesc());
    if (!result->RootElement() || result->RootElement()->NextSiblingElement()) throw std::runtime_error("Actor XML requires one root: " + path);
    return result;
}
std::array<float, 3> tuple(std::string text, std::array<float, 3> fallback) {
    if (text.empty()) return fallback;
    std::replace(text.begin(), text.end(), ',', ' ');
    std::istringstream stream(text); stream.imbue(std::locale::classic());
    std::array<float, 3> result{};
    for (float& component : result) if (!(stream >> component) || !std::isfinite(component)) throw std::runtime_error("Invalid actor transform tuple");
    stream >> std::ws;
    if (!stream.eof()) throw std::runtime_error("Trailing actor transform data");
    return result;
}
Mat4 transform(const Properties& properties, const std::array<float, 3>& offset) {
    auto position = tuple(value(properties, "position"), {});
    auto rotation = tuple(value(properties, "rotation"), {});
    auto scale = tuple(value(properties, "scale"), {1,1,1});
    for (unsigned i=0; i<3; ++i) { position[i] += offset[i]; if (std::abs(scale[i]) < 0.0001f) scale[i] = 1; }
    constexpr float degrees = 0.01745329251994329577f;
    dh2::math::Quaternion q{};
    dh2_quat_from_euler(&q, rotation[1]*degrees, -rotation[0]*degrees, -rotation[2]*degrees);
    float x=q.x,y=q.y,z=q.z,w=q.w;
    Mat4 result{
        (1-2*y*y-2*z*z)*scale[0], (2*x*y+2*z*w)*scale[0], (2*x*z-2*y*w)*scale[0], 0,
        (2*x*y-2*z*w)*scale[1], (1-2*x*x-2*z*z)*scale[1], (2*y*z+2*x*w)*scale[1], 0,
        (2*x*z+2*y*w)*scale[2], (2*y*z-2*x*w)*scale[2], (1-2*x*x-2*y*y)*scale[2], 0,
        position[0],position[1],position[2],1};
    for (float component : result) if (!std::isfinite(component)) throw std::runtime_error("Actor transform overflow");
    return result;
}
std::uint64_t identity_hash(const std::string& identity) {
    std::uint64_t hash = 14695981039346656037ull;
    for (unsigned char c : identity) { hash ^= c; hash *= 1099511628211ull; }
    return hash ? hash : 1;
}
}

bool load_actor_definitions(const AssetCatalog& assets, const std::filesystem::path& levelPath,
                            std::vector<ActorDefinition>& output, std::string& error,
                            const ActorLoadOptions& options) {
    try {
        std::string level;
        if (!resolve(assets, levelPath.generic_string(), "", level)) throw std::runtime_error("Actor level XML not found: " + levelPath.generic_string());
        auto levelDocument = document(assets, level);
        std::vector<ModulePlacement> placements;
        // Reuse the validated authored level placement path.
        if (!decode_level_manifest(assets.read(level), placements, error)) throw std::runtime_error(error);
        std::map<std::string, Properties> templates;
        for (const auto& reference : options.referenceDocuments) {
            std::string resolved;
            if (!resolve(assets, reference.generic_string(), level, resolved)) throw std::runtime_error("Reference XML not found: " + reference.generic_string());
            auto source = document(assets, resolved);
            std::function<void(const TiXmlElement&)> index = [&](const TiXmlElement& element) {
                auto properties = attributes(element);
                const auto key = first(properties, {"name", "id"});
                const std::string tag = element.Value();
                if (tag != "Property" && tag != "property" && !key.empty() && !templates.emplace(key, std::move(properties)).second) throw std::runtime_error("Ambiguous actor template: " + key);
                for (auto* child = element.FirstChildElement(); child; child = child->NextSiblingElement()) index(*child);
            };
            index(*source->RootElement());
        }
        std::vector<ActorDefinition> staged;
        std::set<std::uint64_t> identities;
        std::set<std::string> activeDocuments;
        std::size_t moduleOrdinal = 0;
        std::function<void(const TiXmlElement&, const std::string&, const std::string&, const std::array<float,3>&, std::size_t&)> visit;
        visit = [&](const TiXmlElement& element, const std::string& sourcePath,
                    const std::string& instance, const std::array<float,3>& offset, std::size_t& ordinal) {
            const std::size_t elementOrdinal = ordinal++;
            if (std::string(element.Value()) == "GameObject") {
                if (staged.size() == maximum_actors) throw std::runtime_error("Actor count exceeds limit");
                ActorDefinition actor;
                actor.properties = attributes(element);
                actor.templateReference = first(actor.properties, {"_templateName", "templateName", "template"});
                Properties authored = actor.properties;
                std::set<std::string> activeTemplates;
                std::function<Properties(const std::string&)> defaults = [&](const std::string& key) {
                    if (!activeTemplates.insert(key).second) throw std::runtime_error("Actor template inheritance cycle: " + key);
                    Properties properties;
                    const auto found = templates.find(key);
                    if (found != templates.end()) {
                        const auto parent = first(found->second, {"_templateName", "templateName", "template"});
                        if (!parent.empty() && parent != key) properties = defaults(parent);
                        for (const auto& property : found->second) properties[property.first] = property.second;
                    } else actor.unresolvedReferences.push_back(key);
                    activeTemplates.erase(key);
                    return properties;
                };
                if (!actor.templateReference.empty()) {
                    actor.properties = defaults(actor.templateReference);
                    for (const auto& property : authored) actor.properties[property.first] = property.second;
                }
                actor.name = value(actor.properties, "name");
                actor.gametype = value(actor.properties, "gametype");
                actor.role = first(actor.properties, {"role", "_templateName", "templateName", "gametype"});
                actor.modelPath = first(actor.properties, {"dae", "modelFile", "model"});
                actor.animationConfig = first(actor.properties, {"animation_file", "animation_config", "animationFile", "animfile"});
                actor.sourcePath = sourcePath;
                actor.moduleName = instance;
                actor.sourceId = level + "|" + instance + "|" + sourcePath + "|" + std::to_string(elementOrdinal) + "|" + actor.name;
                actor.stableId = identity_hash(actor.sourceId);
                if (!identities.insert(actor.stableId).second) throw std::runtime_error("Duplicate actor stable identity");
                actor.placement = transform(actor.properties, offset);
                for (const char* key : {"dae", "mgp", "mvp", "template", "templateName", "_templateName", "charpropsname", "char_template", "data", "data_desc", "animation_file", "animation_config"}) {
                    const auto ref = value(actor.properties, key);
                    if (!ref.empty()) actor.references.push_back(ref);
                }
                const bool isModule = actor.gametype == "Module";
                const auto modulePosition = tuple(value(actor.properties, "position"), {});
                std::array<float,3> childOffset = offset;
                for (unsigned i=0;i<3;++i) childOffset[i] += modulePosition[i];
                const std::string childInstance = instance + "/" + actor.name + "#" + std::to_string(isModule ? moduleOrdinal++ : 0);
                std::vector<std::pair<std::string,std::string>> sources;
                if (isModule) for (const char* key : {"mgp", "mvp"}) {
                    const auto ref = value(actor.properties, key);
                    if (ref.empty()) continue;
                    std::string resolved;
                    if (!resolve(assets, ref, sourcePath, resolved)) {
                        actor.unresolvedReferences.push_back(ref);
                        if (options.requireReferencedFiles) throw std::runtime_error("Missing module actor XML: " + ref);
                    } else sources.emplace_back(key, resolved);
                }
                staged.push_back(std::move(actor));
                for (const auto& source : sources) {
                    if (activeDocuments.size() >= 128) throw std::runtime_error("Module actor reference depth limit exceeded");
                    if (!activeDocuments.insert(source.second).second) throw std::runtime_error("Module actor XML reference cycle");
                    auto childDocument = document(assets, source.second);
                    std::size_t childOrdinal = 0;
                    visit(*childDocument->RootElement(), source.second, childInstance, childOffset, childOrdinal);
                    activeDocuments.erase(source.second);
                }
            }
            for (auto* child = element.FirstChildElement(); child; child = child->NextSiblingElement())
                visit(*child, sourcePath, instance, offset, ordinal);
        };
        activeDocuments.insert(level);
        std::size_t ordinal = 0;
        visit(*levelDocument->RootElement(), level, "", {}, ordinal);
        output.swap(staged);
        error.clear();
        return true;
    } catch (const std::exception& exception) { error = exception.what(); return false; }
}

} // namespace dh::foundation
