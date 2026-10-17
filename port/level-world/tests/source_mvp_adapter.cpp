#include "../world.hpp"
#include "../objects.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Unable to open: " + path);
    return {std::istreambuf_iterator<char>(file), {}};
}

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

bool near(float actual, float expected) {
    return std::isfinite(actual) && std::abs(actual - expected) < 0.001f;
}

struct Expected {
    std::uint32_t module;
    std::uint32_t record;
    const char* name;
    const char* root;
    const char* source;
};

constexpr const char* dae = "data/3d/props/swamp/prop_swamp_corpses.bdae";
constexpr std::array<Expected, 10> expected{{
    {1, 5, "_prop_corpse_01_001", "_prop_corpse_01", "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {1, 6, "_prop_corpse_02_001", "_prop_corpse_02", "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {1, 7, "_prop_small_bloodstain_001", "_prop_small_bloodstain", "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {1, 8, "_prop_small_bloodstain_02", "_prop_small_bloodstain", "data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp"},
    {2, 3, "_prop_corpse_02_001", "_prop_corpse_02", "data/3d/modules/swamp/mvp/obj_1of4_brdwalk_nse_00.mvp"},
    {3, 1, "_prop_corpses_small_pile_001", "_prop_corpses_small_pile", "data/3d/modules/swamp/mvp/corner_ruin_ws_00.mvp"},
    {4, 6, "_prop_corpse_01_001", "_prop_corpse_01", "data/3d/modules/swamp/mvp/merchantcamp_ruins_swe_00.mvp"},
    {4, 7, "_prop_corpse_02_001", "_prop_corpse_02", "data/3d/modules/swamp/mvp/merchantcamp_ruins_swe_00.mvp"},
    {6, 6, "_prop_small_bloodstain_001", "_prop_small_bloodstain", "data/3d/modules/swamp/mvp/deadend_brdwalk_w_00.mvp"},
    {8, 3, "_prop_corpses_pile_001", "_prop_corpses_pile", "data/3d/modules/swamp/mvp/obj_2of4_brdwalk_sw_00.mvp"},
}};

std::pair<std::size_t, std::size_t> object_span(const std::string& xml,
                                                 const std::string& name) {
    const auto match = xml.find("name=\"" + name + "\"");
    require(match != std::string::npos && xml.find("name=\"" + name + "\"", match + 1) == std::string::npos,
            "source MVP test object name is not unique");
    const auto begin = xml.rfind("<GameObject", match);
    const auto close = xml.find("/>", match);
    require(begin != std::string::npos && close != std::string::npos,
            "source MVP test object element is malformed");
    return {begin, close + 2};
}

std::string insert_attribute(std::string xml, const std::string& name,
                             const char* attribute) {
    const auto span = object_span(xml, name);
    xml.insert(span.second - 2, std::string(" ") + attribute);
    return xml;
}

void reject_mutation(const std::vector<std::uint8_t>& mlx,
                     const std::array<std::vector<std::uint8_t>, 9>& original,
                     const std::array<dh2::world::SourceMvpView, 9>& views,
                     std::size_t module_index, const std::string& changed,
                     const char* description) {
    auto changed_bytes = original;
    changed_bytes[module_index].assign(changed.begin(), changed.end());
    auto changed_views = views;
    changed_views[module_index].data = changed_bytes[module_index].data();
    changed_views[module_index].size = changed_bytes[module_index].size();
    std::vector<dh2::world::SourceMvpDecor> output(1);
    std::string error;
    require(!dh2::world::compile_source_mvp(mlx.data(), mlx.size(),
                changed_views.data(), changed_views.size(), output, error), description);
    require(output.empty(), "rejected source MVP mutation exposed partial rows");
    require(!error.empty(), "rejected source MVP mutation omitted its reason");
}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: source_mvp_adapter_audit original-cache-root\n";
        return 2;
    }
    try {
        const std::string cache_root = argv[1];
        const auto mlx = read(cache_root + "/data/scene/001_swamp.mlx");
        constexpr std::array<const char*, 9> names = {
            "obj_4of4_brdwalk_sw_00.mvp",
            "obj_3of4_brdwalk_sw_00.mvp",
            "obj_1of4_brdwalk_nse_00.mvp",
            "corner_ruin_ws_00.mvp",
            "merchantcamp_ruins_swe_00.mvp",
            "corner_brdwalk_se_00.mvp",
            "deadend_brdwalk_w_00.mvp",
            "bossroom_ruins_ns_.mvp",
            "obj_2of4_brdwalk_sw_00.mvp"
        };
        std::array<std::vector<std::uint8_t>, names.size()> mvp_bytes;
        std::array<std::string, names.size()> paths;
        std::array<dh2::world::SourceMvpView, names.size()> mvps{};
        for (std::size_t i = 0; i < names.size(); ++i) {
            paths[i] = std::string("data/3d/modules/swamp/mvp/") + names[i];
            mvp_bytes[i] = read(cache_root + "/" + paths[i]);
            mvps[i] = {paths[i].c_str(), mvp_bytes[i].data(), mvp_bytes[i].size()};
        }

        std::vector<dh2::world::SourceMvpDecor> decors;
        std::string error;
        require(dh2::world::compile_source_mvp(mlx.data(), mlx.size(), mvps.data(),
                    mvps.size(), decors, error), error.c_str());
        require(decors.size() == expected.size(), "source MVP Decor count differs from reviewed allowlist");
        for (std::size_t i = 0; i < expected.size(); ++i) {
            const auto& row = decors[i];
            const auto& want = expected[i];
            require(row.module_index == want.module && row.source_record == want.record,
                    "source MVP provenance order differs");
            require(row.name == want.name && row.xrefobject == want.root,
                    "source MVP instance or xrefobject root differs");
            require(row.dae_path == dae && row.source_path == want.source,
                    "source MVP asset paths differ");
        }

        const auto& first = decors.front();
        require(near(first.local.position[0], 799.1f) &&
                    near(first.local.position[1], 1154.6f) &&
                    near(first.local.position[2], 260.2f),
                "source MVP local position differs");
        require(near(first.world.position[0], -5200.9f) &&
                    near(first.world.position[1], 1154.6f) &&
                    near(first.world.position[2], 260.2f),
                "source MVP module-translated position differs");
        require(near(first.local.rotation_degrees[2], 90.0f) &&
                    near(first.world.rotation_degrees[2], 90.0f) &&
                    near(first.local.scale[0], 1.0f) && near(first.world.scale[0], 1.0f),
                "source MVP local/world rotation or scale differs");

        const auto prop_model = read(cache_root + "/" + dae);
        dh2::objects::Resource prop_resource;
        if (!dh2::objects::load_resource(prop_model.data(), prop_model.size(),
                    nullptr, 0, prop_resource, error))
            throw std::runtime_error("prop BDAE resource load failed: " + error);
        std::set<std::string> roots;
        for (const auto& decor : decors) {
            const auto root_id = decor.xrefobject + "-node";
            const auto matches = std::count_if(prop_resource.scene.graph.begin(),
                prop_resource.scene.graph.end(), [&](const auto& node) {
                    return node.id == root_id;
                });
            require(matches == 1, "source Decor xrefobject root is absent or ambiguous in prop BDAE");
            const auto root = static_cast<std::size_t>(std::find_if(
                prop_resource.scene.graph.begin(), prop_resource.scene.graph.end(),
                [&](const auto& node) { return node.id == root_id; }) -
                prop_resource.scene.graph.begin());
            std::vector<bool> subtree(prop_resource.scene.graph.size(), false);
            for (std::size_t i = 0; i < prop_resource.scene.graph.size(); ++i) {
                const auto parent = prop_resource.scene.graph[i].parent;
                subtree[i] = i == root || (parent >= 0 &&
                    static_cast<std::size_t>(parent) < subtree.size() &&
                    subtree[static_cast<std::size_t>(parent)]);
            }
            const auto primitive_count = std::count_if(prop_resource.primitives.begin(),
                prop_resource.primitives.end(), [&](const auto& primitive) {
                    return primitive.node < subtree.size() && subtree[primitive.node];
                });
            require(primitive_count > 0, "source Decor root has no visible primitive descendant");
            roots.insert(root_id);
        }
        require(roots.size() == 5, "ten source rows do not resolve to the five reviewed BDAE roots");

        const std::size_t module = 1;
        const std::string source_xml(mvp_bytes[module].begin(), mvp_bytes[module].end());
        reject_mutation(mlx, mvp_bytes, mvps, module,
            insert_attribute(source_xml, expected[0].name,
                "activate_cond=\"IsAfter_SwampEscape\""),
            "conditional source Decor was accepted");
        reject_mutation(mlx, mvp_bytes, mvps, module,
            insert_attribute(source_xml, expected[0].name, "script=\"SpawnLoot()\""),
            "scripted source Decor was accepted");

        auto changed_root = source_xml;
        const auto span = object_span(changed_root, expected[0].name);
        const auto root_begin = changed_root.find("xrefobject=\"", span.first);
        const auto root_end = changed_root.find('"', root_begin + 11);
        require(root_begin != std::string::npos && root_end != std::string::npos &&
                    root_end < span.second,
                "source MVP test root field is missing");
        changed_root.replace(root_begin + 11, root_end - root_begin - 11, "_wrong_prop_root");
        reject_mutation(mlx, mvp_bytes, mvps, module, changed_root,
            "changed xrefobject root was accepted");

        auto extra_row = source_xml;
        const auto first_span = object_span(extra_row, expected[0].name);
        auto copied = extra_row.substr(first_span.first, first_span.second - first_span.first);
        const std::string old_name = std::string("name=\"") + expected[0].name + "\"";
        const auto name_at = copied.find(old_name);
        require(name_at != std::string::npos, "source MVP test row has no name field");
        copied.replace(name_at, old_name.size(), "name=\"_prop_unexpected_extra\"");
        const auto module_end = extra_row.rfind("</Module>");
        require(module_end != std::string::npos, "source MVP test module has no closing tag");
        extra_row.insert(module_end, copied);
        reject_mutation(mlx, mvp_bytes, mvps, module, extra_row,
            "unexpected extra source Decor was accepted");

        std::cout << "source MVP adapter: ten ordered Decor rows and transforms verified; condition/script/root/extra-row mutations rejected\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
