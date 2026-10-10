#include "procedural_level_v1.hpp"

#include "../../../level-loader/procedural_asset_source_v1.hpp"
#include "../../../level-loader/procedural_sources_v1.hpp"
#include "../../../level-loader/procedural_blocks_v1.hpp"
#include "../../../level-loader/procedural_connections_v1.hpp"
#include "../../../level-loader/procedural_lists_v1.hpp"
#include "../../../level-loader/procedural_rules_v1.hpp"
#include "../../../level-loader/procedural_layout_v1.hpp"
#include "../../../level-loader/procedural_modules_v1.hpp"

#include <algorithm>
#include <cctype>
#include <exception>
#include <sstream>
#include <stdexcept>

namespace dh::foundation::procedural {
namespace {

using dh2::loader::ProceduralAssetSourceV1;

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

// Reads through the loose-file catalog. A missing resource is found=false, not an error.
class CatalogSource final : public ProceduralAssetSourceV1 {
public:
    explicit CatalogSource(const AssetCatalog& catalog) : catalog_(catalog) {}
    bool read(const std::string& uri, bool& found, std::vector<std::uint8_t>& bytes,
              std::string& error) const override {
        found = false;
        bytes.clear();
        error.clear();
        std::string key;
        if (!ProceduralAssetSourceV1::key(uri, key, error)) return false;
        std::filesystem::path path(uri);
        try {
            bytes = catalog_.read(path);
        } catch (const std::invalid_argument&) {
            error = "invalid procedural resource path: " + uri;
            return false;
        } catch (const std::exception&) {
            // Missing (or not a regular file): reported by the caller as a missing dependency.
            return true;
        }
        found = true;
        return true;
    }
private:
    const AssetCatalog& catalog_;
};

std::string escaped(const std::string& input) {
    std::string out;
    for (const unsigned char c : input) {
        switch (c) {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;"; break;
        case '>': out += "&gt;"; break;
        case '"': out += "&quot;"; break;
        case '\r': out += "&#13;"; break;
        case '\n': out += "&#10;"; break;
        case '\t': out += "&#9;"; break;
        default:
            if (c < 32) throw std::runtime_error("unsupported control byte in generated attribute");
            out += static_cast<char>(c);
        }
    }
    return out;
}

} // namespace

bool is_procedural_level_uri(const std::string& uri) {
    const auto name = lower(uri);
    return name.size() >= 9 && name.compare(name.size() - 9, 9, ".rule.xml") == 0;
}

std::string procedural_backup_uri(const std::string& ruleUri) {
    // IDA: first name byte becomes 'x'; the name is cut at its first '.' and
    // `_BACKUP.mlx` is appended. File names are case-insensitive on Windows.
    const auto slash = ruleUri.find_last_of('/');
    const auto folder = slash == std::string::npos ? std::string() : ruleUri.substr(0, slash + 1);
    std::string name = slash == std::string::npos ? ruleUri : ruleUri.substr(slash + 1);
    if (!name.empty()) name[0] = 'x';
    const auto dot = name.find('.');
    const auto base = dot == std::string::npos ? name : name.substr(0, dot);
    return folder + base + "_backup.mlx";
}

bool generate_level(const AssetCatalog& assets, const std::string& ruleUri,
                    std::uint32_t seed, GeneratedLevel& out, std::string& error) {
    error.clear();
    try {
        const CatalogSource source(assets);
        dh2::loader::ProceduralSourcesV1 sources;
        if (!sources.prepare(source, ruleUri, ruleUri, error)) throw std::runtime_error("sources: " + error);
        dh2::loader::ProceduralBlocksV1 blocks;
        if (!blocks.prepare(sources.borrow(), error)) throw std::runtime_error("blocks: " + error);
        dh2::loader::ProceduralConnectionsV1 connections;
        if (!connections.prepare(blocks.borrow(), error)) throw std::runtime_error("connections: " + error);
        dh2::loader::ProceduralListsV1 lists;
        if (!lists.prepare(connections.borrow(), error)) throw std::runtime_error("lists: " + error);
        dh2::loader::ProceduralRulesV1 rules;
        if (!rules.prepare(lists.borrow(), error)) throw std::runtime_error("rules: " + error);
        dh2::loader::ProceduralLayoutResultV1 layout;
        if (!dh2::loader::generate_procedural_layout_v1(rules.borrow(), seed, layout, error))
            throw std::runtime_error("layout: " + error);
        if (!layout.generated || layout.tiles.empty()) throw std::runtime_error("layout: generation produced no tiles");
        dh2::loader::ProceduralModulePlanV1 plan;
        if (!dh2::loader::prepare_procedural_modules_v1(source, layout, plan, error))
            throw std::runtime_error("modules: " + error);

        const auto root = sources.borrow().documents().at(0).elements().at(sources.borrow().rule_root());
        std::ostringstream xml;
        xml.imbue(std::locale::classic());
        xml << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<Level>\n<GameObject gametype=\"LevelConfig\"";
        // Rule-root attributes (music, fog, camera planes, light sets) are the LevelConfig source.
        for (const auto& attribute : root.attributes)
            if (attribute.first != "gametype") xml << ' ' << attribute.first << "=\"" << escaped(attribute.second) << '"';
        xml << "/>\n";
        out.modules = 0;
        out.unplaced_modules = 0;
        for (const auto& module : plan.modules) {
            // A tile without MVX geometry cannot be placed; the original tile would
            // draw nothing for it. Counted and reported, not silently dropped.
            if (!module.mvx_found || module.overrides.count("dae") == 0 || module.overrides.count("xrefobject") == 0) {
                ++out.unplaced_modules;
                out.notices.push_back("tile without module geometry: " + module.mvx_uri);
                continue;
            }
            xml << "<GameObject";
            for (const auto& attribute : module.overrides) xml << ' ' << attribute.first << "=\"" << escaped(attribute.second) << '"';
            xml << " visible=\"1\" static=\"1\"/>\n";
            ++out.modules;
        }
        xml << "</Level>\n";
        if (out.modules == 0) throw std::runtime_error("modules: no tile has module geometry");

        const auto text = xml.str();
        out.rule_uri = ruleUri;
        out.level_uri = ruleUri;
        out.seed = seed;
        out.tiles = layout.tiles.size();
        out.xml.assign(text.begin(), text.end());
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

void install_generated_level(AssetCatalog& assets, const GeneratedLevel& level) {
    assets.set_virtual_file(level.level_uri, level.xml);
}

} // namespace dh::foundation::procedural
