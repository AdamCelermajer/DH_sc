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
#include <cstring>
#include <exception>
#include <sstream>
#include <stdexcept>
#include <ctime>

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
            bytes = catalog_.read_authored(path); // the authored rule and MGX/MVX files, never a generated virtual document
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

namespace {

// The loader objects of one generation, held together because each stage borrows the previous one.
struct LoaderChain {
    dh2::loader::ProceduralSourcesV1 sources;
    dh2::loader::ProceduralBlocksV1 blocks;
    dh2::loader::ProceduralConnectionsV1 connections;
    dh2::loader::ProceduralListsV1 lists;
    dh2::loader::ProceduralRulesV1 rules;
    dh2::loader::ProceduralLayoutResultV1 layout;
};

// Canonical order: sources, blocks, connections, lists, rules, then the original Generate (layout). Throws a
// runtime_error that names the failing stage. A completed no-layout result is not an error here (layout.generated).
void run_layout_stage(const ProceduralAssetSourceV1& source, const std::string& ruleUri, std::uint32_t seed,
                      LoaderChain& chain) {
    std::string error;
    if (!chain.sources.prepare(source, ruleUri, ruleUri, error)) throw std::runtime_error("sources: " + error);
    if (!chain.blocks.prepare(chain.sources.borrow(), error)) throw std::runtime_error("blocks: " + error);
    if (!chain.connections.prepare(chain.blocks.borrow(), error)) throw std::runtime_error("connections: " + error);
    if (!chain.lists.prepare(chain.connections.borrow(), error)) throw std::runtime_error("lists: " + error);
    if (!chain.rules.prepare(chain.lists.borrow(), error)) throw std::runtime_error("rules: " + error);
    if (!dh2::loader::generate_procedural_layout_v1(chain.rules.borrow(), seed, chain.layout, error))
        throw std::runtime_error("layout: " + error);
}

} // namespace

bool generate_layout(const AssetCatalog& assets, const std::string& ruleUri, std::uint32_t seed,
                     LayoutRecord& out, std::string& error) {
    error.clear();
    out = LayoutRecord{};
    try {
        const CatalogSource source(assets);
        LoaderChain chain;
        run_layout_stage(source, ruleUri, seed, chain);
        const auto& layout = chain.layout;
        out.generated = layout.generated;
        out.seed = layout.seed;
        out.final_state = layout.final_state;
        out.random_calls = layout.events.random_calls;
        out.root_attempts = layout.events.root_attempts;
        out.place_calls = layout.events.place_calls;
        out.unspawn_calls = layout.events.unspawn_calls;
        out.step_calls = layout.events.step_calls;
        out.path_calls = layout.events.path_calls;
        out.rule_calls = layout.events.rule_calls;
        for (const auto& tile : layout.tiles) {
            LayoutTileRecord record;
            record.block_source = tile.block_source;
            record.name = tile.name;
            record.grid_x = tile.grid[0];
            record.grid_y = tile.grid[1];
            std::memcpy(&record.height_bits, &tile.height, sizeof record.height_bits);
            record.list_block = tile.list_element.block_name;
            record.list_gameplay = tile.list_element.gameplay;
            record.list_visual = tile.list_element.visual;
            record.chances = tile.list_element.chances;
            record.children = tile.children;
            out.tiles.push_back(std::move(record));
        }
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

std::vector<std::string> procedural_backup_candidates(const std::string& ruleUri) {
    const std::string suffix = "_backup.mlx";
    const auto first = procedural_backup_uri(ruleUri);
    std::vector<std::string> candidates{first};
    const auto stem = first.substr(0, first.size() - suffix.size());
    const auto underscore = stem.find_last_of('_');
    if (underscore != std::string::npos && underscore + 1 < stem.size() &&
        std::all_of(stem.begin() + std::ptrdiff_t(underscore + 1), stem.end(),
                    [](unsigned char c) { return std::isdigit(c) != 0; }))
        candidates.push_back(stem.substr(0, underscore) + suffix);
    return candidates;
}

std::string resolve_procedural_backup(const AssetCatalog& assets, const std::string& ruleUri) {
    const auto candidates = procedural_backup_candidates(ruleUri);
    for (const auto& candidate : candidates) {
        try {
            assets.resolve(candidate);
            return candidate;
        } catch (const std::exception&) {
            // absent: try the next candidate
        }
    }
    return candidates.front();
}

bool generate_level(const AssetCatalog& assets, const std::string& ruleUri,
                    std::uint32_t seed, GeneratedLevel& out, std::string& error) {
    error.clear();
    try {
        const CatalogSource source(assets);
        LoaderChain chain;
        run_layout_stage(source, ruleUri, seed, chain);
        auto& sources = chain.sources;
        const auto& layout = chain.layout;
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


std::uint64_t layout_signature(const GeneratedLevel& level) {
    std::uint64_t hash = 1469598103934665603ull;
    for (const auto byte : level.xml) {
        hash ^= byte;
        hash *= 1099511628211ull;
    }
    return hash;
}

ProceduralSeeds saved_seeds(const CharacterState& state) {
    ProceduralSeeds seeds{};
    for (const auto& component : state.components) {
        if (component.id != procedural_seed_component_id) continue;
        if (component.bytes.size() != seeds.size() * 4) return ProceduralSeeds{};
        for (std::size_t index = 0; index < seeds.size(); ++index) {
            std::uint32_t value = 0;
            for (unsigned byte = 0; byte < 4; ++byte)
                value |= std::uint32_t(component.bytes[index * 4 + byte]) << (8 * byte);
            seeds[index] = value;
        }
        return seeds;
    }
    return seeds;
}

void store_saved_seeds(CharacterState& state, const ProceduralSeeds& seeds) {
    CharacterComponent component;
    component.id = procedural_seed_component_id;
    for (const auto seed : seeds)
        for (unsigned byte = 0; byte < 4; ++byte)
            component.bytes.push_back(static_cast<std::uint8_t>(seed >> (8 * byte)));
    for (auto& existing : state.components) {
        if (existing.id == procedural_seed_component_id) {
            existing = std::move(component);
            return;
        }
    }
    state.components.push_back(std::move(component));
}

std::uint32_t session_seed(const CharacterState& state, std::int32_t difficulty,
                           std::optional<std::uint32_t> forced, std::uint32_t fresh) {
    if (forced && *forced != 0) return *forced;
    const auto index = static_cast<std::size_t>(difficulty < 0 || difficulty >= std::int32_t(character_difficulty_count) ? 0 : difficulty);
    const auto seeds = saved_seeds(state);
    if (seeds[index] != 0) return seeds[index];
    return fresh ? fresh : 1u;
}

void record_session_seed(CharacterState& state, std::int32_t difficulty, std::uint32_t seed) {
    if (difficulty < 0 || difficulty >= std::int32_t(character_difficulty_count)) return;
    auto seeds = saved_seeds(state);
    seeds[static_cast<std::size_t>(difficulty)] = seed;
    store_saved_seeds(state, seeds);
}

std::uint32_t fresh_seed() {
    static std::uint32_t counter = 0;
    const auto now = static_cast<std::uint64_t>(std::time(nullptr));
    const auto mixed = (now * 2654435761ull) ^ (static_cast<std::uint64_t>(++counter) * 40503ull) ^ static_cast<std::uint64_t>(std::clock());
    const auto seed = static_cast<std::uint32_t>(mixed ^ (mixed >> 32));
    return seed ? seed : 1u;
}

} // namespace dh::foundation::procedural
