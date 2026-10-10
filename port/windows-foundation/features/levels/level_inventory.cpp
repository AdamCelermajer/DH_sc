// P16 LEVELS: read-only per-level coverage inventory (diagnostic tool, not game code).
// Uses the same loaders as the game: level_manifest/assembled_level (fixed .mlx geometry),
// actor_definitions (every authored GameObject of the level and its module MGP/MVP files),
// and the game-data level table (levels_pyarray.bin). Procedural .rule.xml files are
// summarized from their tags only (no rule evaluation).
//
// Usage: level_inventory --root ASSETS_ROOT --package PACKAGE_ASSETS --android FILES_ROOT
//                        --table DIR_WITH_levels_pyarray --out OUT.json
#include "actor_definitions.hpp"
#include "asset_catalog.hpp"
#include "assembled_level.hpp"
#include "../../../game-data/level_tables.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace dh::foundation;

namespace {
// Act band inferred from the file-id ranges (NOT stored in the level data; unverified).
std::string act_band(const std::string& id) {
    if (id.empty() || !std::isdigit((unsigned char)id[0])) return "?";
    const int n = std::stoi(id.substr(0, 3));
    if (n >= 100) return "debug";
    if (n <= 9) return "A1?";
    if (n <= 24) return "A2?";
    if (n <= 39) return "A3?";
    return "?";
}
std::string json_escape(const std::string& s) {
    std::string out;
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') { out += '\\'; out += char(c); }
        else if (c < 0x20) { out += ' '; }
        else out += char(c);
    }
    return out;
}
std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return char(std::tolower(c)); });
    return s;
}
std::string slash(std::string s) { std::replace(s.begin(), s.end(), '\\', '/'); return s; }

std::vector<std::uint8_t> read_file(const fs::path& p) {
    std::ifstream in(p, std::ios::binary);
    if (!in) return {};
    return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

// Candidate locations used by the game's asset resolution (level_manifest/find_asset rules).
bool present_in(const fs::path& root, const std::string& uri) {
    if (uri.empty()) return true;
    const auto u = slash(uri);
    const auto l = lower(u);
    // Same alias as level_manifest.cpp find_asset: data/iphone/ -> data/.
    std::vector<std::string> candidates{u, "original-cache/" + u, l, "original-cache/" + l};
    const std::string iphone = "data/iphone/";
    if (l.compare(0, iphone.size(), iphone) == 0) {
        const std::string alias = "data/" + l.substr(iphone.size());
        candidates.push_back(alias);
        candidates.push_back("original-cache/" + alias);
    }
    for (const auto& candidate : candidates) {
        std::error_code ec;
        if (fs::is_regular_file(root / candidate, ec)) return true;
    }
    return false;
}

void tag_histogram_xml(const std::string& text, std::map<std::string, int>& counts) {
    // Counts element names of a rule XML (tags only; attributes not interpreted).
    for (std::size_t i = 0; (i = text.find('<', i)) != std::string::npos; ++i) {
        std::size_t j = i + 1;
        if (j < text.size() && (text[j] == '?' || text[j] == '!' || text[j] == '/')) continue;
        std::string name;
        while (j < text.size() && (std::isalnum((unsigned char)text[j]) || text[j] == '_')) name += text[j++];
        if (!name.empty()) counts[name]++;
    }
}

std::string attr(const std::string& text, const std::string& key) {
    const auto pos = text.find(key + "=\"");
    if (pos == std::string::npos) return {};
    const auto start = pos + key.size() + 2;
    const auto end = text.find('"', start);
    return end == std::string::npos ? std::string{} : text.substr(start, end - start);
}

} // namespace

int main(int argc, char** argv) {
    std::string root, package, android, tableDir, outPath, mdPath;
    for (int i = 1; i + 1 < argc; i += 2) {
        const std::string k = argv[i], v = argv[i + 1];
        if (k == "--root") root = v;
        else if (k == "--package") package = v;
        else if (k == "--android") android = v;
        else if (k == "--table") tableDir = v;
        else if (k == "--out") outPath = v;
        else if (k == "--md") mdPath = v;
        else { std::cerr << "unknown option " << k << '\n'; return 2; }
    }
    if (root.empty() || outPath.empty()) { std::cerr << "--root and --out are required\n"; return 2; }

    AssetCatalog assets(root);
    std::unique_ptr<AssetCatalog> pkg, andr;
    if (!package.empty()) pkg = std::make_unique<AssetCatalog>(package);
    if (!android.empty()) andr = std::make_unique<AssetCatalog>(android);

    // Level table (game-data decoder used by the game).
    dh2::data::LevelTables tables;
    bool tableOk = false;
    std::string tableError;
    std::vector<std::uint8_t> rec, nam, sch;
    if (!tableDir.empty()) {
        rec = read_file(fs::path(tableDir) / "levels_pyarray.bin");
        nam = read_file(fs::path(tableDir) / "levels_pyarraynames.bin");
        sch = read_file(fs::path(tableDir) / "levels_pystructnames.bin");
        tableOk = dh2::data::load_levels({rec.data(), rec.size()}, {nam.data(), nam.size()}, {sch.data(), sch.size()}, tables, tableError);
    }
    std::map<std::string, std::size_t> tableByFile;
    if (tableOk) for (std::size_t r = 0; r < tables.levels.size(); ++r) tableByFile[lower(tables.levels[r].file)] = r;

    // Enumerate scene directory of the root (fixed maps first, procedural rules second).
    std::vector<std::string> files;
    std::set<std::string> backups;
    for (const auto& entry : fs::directory_iterator(fs::path(root) / "data" / "scene")) {
        const auto name = entry.path().filename().string();
        if (!entry.is_regular_file()) continue;
        if (name.size() > 0 && name[0] == 'x') { backups.insert(name); continue; }
        if (name.size() > 4 && (name.substr(name.size() - 4) == ".mlx" || name.size() > 9 && name.substr(name.size() - 9) == ".rule.xml"))
            files.push_back(name);
    }
    std::sort(files.begin(), files.end());

    std::ostringstream json;
    json << "{\n\"table_ok\":" << (tableOk ? "true" : "false") << ",\n\"table_error\":\"" << json_escape(tableError)
         << "\",\n\"table_rows\":" << tables.levels.size() << ",\n\"backups_not_loaded\":[";
    {
        bool first = true;
        for (const auto& b : backups) { json << (first ? "" : ",") << "\"" << json_escape(b) << "\""; first = false; }
    }
    json << "],\n\"levels\":[\n";

    std::vector<std::string> md;
    bool firstLevel = true;
    for (const auto& name : files) {
        const std::string rel = "data/scene/" + name;
        const bool fixed = name.size() > 4 && name.substr(name.size() - 4) == ".mlx";
        const std::string id = name.substr(0, name.find('_'));
        std::string stem = fixed ? name.substr(0, name.size() - 4) : name.substr(0, name.size() - 9);
        std::string desc, tableName;
        int tableRow = -1;
        if (tableOk) {
            auto it = tableByFile.find(lower(name));
            if (it != tableByFile.end()) { desc = tables.levels[it->second].description; tableName = tables.level_names[it->second]; tableRow = 1; }
        }
        std::string error;
        std::ostringstream rec2;
        rec2 << (firstLevel ? "" : ",\n") << "{\"id\":\"" << json_escape(id) << "\",\"file\":\"" << rel << "\",\"stem\":\"" << json_escape(stem)
             << "\",\"kind\":\"" << (fixed ? "fixed_mlx" : "procedural_rule_xml") << "\",\"table_description\":\"" << json_escape(desc)
             << "\",\"table_name\":\"" << json_escape(tableName) << "\",\"in_table\":" << (tableRow > 0 ? "true" : "false");
        firstLevel = false;

        const auto bytes = read_file(fs::path(root) / rel);
        const std::string text(bytes.begin(), bytes.end());
        // Procedural levels: summarize tags only.
        if (!fixed) {
            std::map<std::string, int> counts;
            tag_histogram_xml(text, counts);
            rec2 << ",\"rule\":{\"folder\":\"" << json_escape(attr(text, "folder")) << "\",\"scriptFile\":\"" << json_escape(attr(text, "scriptFile"))
                 << "\",\"light_set\":\"" << json_escape(attr(text, "light_set")) << "\",\"music\":\"" << json_escape(attr(text, "music"))
                 << "\",\"tags\":{";
            bool f = true;
            for (const auto& c : counts) { rec2 << (f ? "" : ",") << "\"" << c.first << "\":" << c.second; f = false; }
            rec2 << "}}";
            rec2 << ",\"status\":\"procedural: not instantiated by any runtime path (rule evaluation missing)\"}";
            {
                std::ostringstream row;
                row << "| " << id << " | " << act_band(id) << " | `" << name << "` | " << (tableName.empty() ? "-" : tableName) << " | procedural .rule.xml | - | - | - | - | "
                    << "RootRule:" << counts["RootRule"] << " lists:" << counts["list"] << " module elems:" << counts["elem"] << " ForceBlock:" << counts["ForceBlock"] << " | - | - | - | - |\n";
                md.push_back(row.str());
            }
            json << rec2.str();
            continue;
        }

        // Fixed map: geometry via the game loader.
        OriginalScene scene;
        bool geomOk = load_level(assets, rel, scene, error);
        rec2 << ",\"geometry\":{\"ok\":" << (geomOk ? "true" : "false") << ",\"error\":\"" << json_escape(error) << "\",\"triangles\":" << scene.triangleCount
             << ",\"instances\":" << scene.instanceCount << ",\"notices\":" << scene.notices.size() << "}";

        // Authored declarations (every GameObject of the level + module MGP/MVP children).
        std::vector<ActorDefinition> defs;
        std::string defError;
        bool declOk = load_actor_definitions(assets, rel, defs, defError);
        std::map<std::string, int> byClass, byModuleClass;
        std::set<std::string> unresolved, modelsMissing, refsMissing;
        int modules = 0;
        std::map<std::string, int> modelRefs;
        std::string levelScript, levelLight, levelFixedLight, levelMusic, levelCamera, levelAmbient;
        for (const auto& d : defs) {
            const std::string cls = d.gametype.empty() ? std::string("(none)") : d.gametype;
            byClass[cls]++;
            if (cls == "Module") modules++;
            if (cls == "LevelConfig") {
                auto get = [&](const char* k) { auto it = d.properties.find(k); return it == d.properties.end() ? std::string{} : it->second; };
                levelScript = get("scriptFile"); levelLight = get("light_set"); levelFixedLight = get("fixed_light_set");
                levelMusic = get("music"); levelCamera = get("camera_file");
            }
            for (const auto& u : d.unresolvedReferences) unresolved.insert(u);
            if (!d.modelPath.empty()) modelRefs[slash(d.modelPath)]++;
        }
        // Asset presence for each model/reference used by the declarations.
        std::set<std::string> allRefs;
        for (const auto& d : defs) {
            // Symbolic template/class names (no path separator or extension) are not files.
            for (const auto& r : d.references) if (slash(r).find_first_of("/.") != std::string::npos) allRefs.insert(slash(r));
            if (!d.modelPath.empty()) allRefs.insert(slash(d.modelPath));
            if (!d.animationConfig.empty()) allRefs.insert(slash(d.animationConfig));
        }
        int refPresentPkg = 0, refPresentAndroid = 0, refMissingAll = 0;
        std::vector<std::string> missingSamples;
        for (const auto& r : allRefs) {
            const bool p = pkg ? present_in(pkg->root(), r) : false;
            const bool a = andr ? present_in(andr->root(), r) : false;
            if (p) refPresentPkg++;
            if (a) refPresentAndroid++;
            if (!p && !a) { refMissingAll++; if (missingSamples.size() < 12) missingSamples.push_back(r); }
        }
        // First authored SpawnPoint (document order): used only as a coverage-run start position.
        std::string spawnText = "none";
        for (const auto& d : defs) if (d.gametype == "SpawnPoint") {
            std::ostringstream s;
            s << d.name << " " << d.placement[12] << " " << d.placement[13] << " " << d.placement[14];
            spawnText = s.str();
            break;
        }
        // Module-level files (dae/mvp/mgp) of Module declarations, as used by the game loader.
        std::set<std::string> moduleFiles;
        for (const auto& d : defs) if (d.gametype == "Module") {
            for (const char* k : {"dae", "mvp", "mgp"}) { auto it = d.properties.find(k); if (it != d.properties.end() && !it->second.empty()) moduleFiles.insert(slash(it->second)); }
        }
        rec2 << ",\"declarations\":{\"ok\":" << (declOk ? "true" : "false") << ",\"error\":\"" << json_escape(defError) << "\",\"total\":" << defs.size()
             << ",\"module_placements\":" << modules << ",\"by_class\":{";
        bool f = true;
        for (const auto& c : byClass) { rec2 << (f ? "" : ",") << "\"" << json_escape(c.first) << "\":" << c.second; f = false; }
        rec2 << "}},\"assets\":{\"referenced\":" << allRefs.size() << ",\"present_package\":" << refPresentPkg << ",\"present_android\":" << refPresentAndroid
             << ",\"missing_both\":" << refMissingAll << ",\"missing_samples\":[";
        f = true;
        for (const auto& s : missingSamples) { rec2 << (f ? "" : ",") << "\"" << json_escape(s) << "\""; f = false; }
        rec2 << "],\"module_files\":" << moduleFiles.size() << ",\"unresolved_refs\":" << unresolved.size() << "}";
        rec2 << ",\"level_config\":{\"scriptFile\":\"" << json_escape(levelScript) << "\",\"light_set\":\"" << json_escape(levelLight)
             << "\",\"fixed_light_set\":\"" << json_escape(levelFixedLight) << "\",\"music\":\"" << json_escape(levelMusic)
             << "\",\"camera_file\":\"" << json_escape(levelCamera) << "\"}";
        {
            std::ostringstream row;
            std::string classes;
            for (const auto& c : byClass) classes += c.first + ":" + std::to_string(c.second) + " ";
            row << "| " << id << " | " << act_band(id) << " | `" << name << "` | " << (tableName.empty() ? "-" : tableName) << " | fixed .mlx | "
                << (geomOk ? "yes" : "NO") << " | " << scene.triangleCount << " | " << modules << " | " << defs.size() << " | " << classes << "| "
                << refPresentPkg << "/" << allRefs.size() << " | " << refPresentAndroid << "/" << allRefs.size() << " | " << refMissingAll << " | " << unresolved.size()
                << " | " << scene.minimum.x << " " << scene.minimum.y << " " << scene.minimum.z << " | " << scene.maximum.x << " " << scene.maximum.y << " " << scene.maximum.z
                << " | " << spawnText << " |\n";
            md.push_back(row.str());
        }
        json << rec2.str() << "}";
    }
    json << "\n]\n}\n";
    std::ofstream out(outPath, std::ios::binary);
    if (!out) { std::cerr << "cannot write " << outPath << '\n'; return 1; }
    out << json.str();
    if (!mdPath.empty()) {
        std::ofstream m(mdPath, std::ios::binary);
        m << "| id | act (inferred) | file | level table name | kind | geometry ok | triangles | module placements | declarations | authored classes | assets pkg | assets android | missing both | unresolved refs | bounds min | bounds max |\n|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|\n";
        for (const auto& r : md) m << r;
    }
    std::cout << "levels=" << files.size() << " table_ok=" << tableOk << " out=" << outPath << '\n';
    return 0;
}
