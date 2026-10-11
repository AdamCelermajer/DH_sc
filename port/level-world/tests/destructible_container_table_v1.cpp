#include "../destructible_container_table_v1.hpp"
#include "../destructible_container_visual_provider_v1.hpp"

#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

using dh2::destructible_container_table_v1::Bytes;

bool read_file(const std::string& path, std::vector<std::uint8_t>& bytes) {
    std::ifstream input(path, std::ios::binary);
    if (!input) return false;
    bytes.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    return input.good() || input.eof();
}

Bytes view(const std::vector<std::uint8_t>& bytes) {
    return {bytes.data(), bytes.size()};
}

} // namespace

int main(int argc, char** argv) {
    if (argc != 3) return 2;
    const std::string root = argv[1];
    const std::string asset_root = argv[2];
    std::vector<std::uint8_t> data, names, fields, visuals, visual_names;
    if (!read_file(root + "/game_objects_pyarray.bin", data) ||
        !read_file(root + "/game_objects_pyarraynames.bin", names) ||
        !read_file(root + "/game_objects_pystructnames.bin", fields) ||
        !read_file(root + "/game_objects_dictionary_pyarray.bin", visuals) ||
        !read_file(root + "/game_objects_dictionary_pyarraynames.bin", visual_names))
        return 3;

    dh2::destructible_container_table_v1::Tables tables;
    std::string error;
    if (!tables.load(view(data), view(names), view(fields), view(visuals),
                     view(visual_names), error))
        return 4;
    if (tables.row_count() != 37) return 5;
    const auto* no_moth = tables.find("Swamp_NoMoth_DestructibleBarrel");
    const auto* no_moth_path = no_moth ? tables.visual_path(no_moth->visual) : nullptr;
    if (!no_moth || !no_moth_path || no_moth->visual != 70 ||
        *no_moth_path != "data/3D/GameObjects/go_swamp_urn_breakable.bdae")
        return 13;

    std::int32_t data_id = -1;
    const auto* swamp = tables.find("Swamp_Normal_DestructibleBarrel", &data_id);
    if (!swamp || data_id != 30 || swamp->hit_points != 1 ||
        swamp->interact_sound != 325 || swamp->loot != 9 ||
        swamp->script != "moth_spawn_container" || swamp->visual != 70)
        return 6;
    const auto* swamp_path = tables.visual_path(swamp->visual);
    if (!swamp_path || *swamp_path != "data/3D/GameObjects/go_swamp_urn_breakable.bdae")
        return 7;

    const auto* crypt = tables.find("Crypt02_Normal_Barrel", &data_id);
    if (!crypt || data_id != 1 || crypt->hit_points != 1 ||
        crypt->interact_sound != 325 || crypt->loot != 10 ||
        !crypt->script.empty() || crypt->visual != 74)
        return 8;
    const auto* crypt_path = tables.visual_path(crypt->visual);
    if (!crypt_path || *crypt_path != "data/3D/GameObjects/go_urn_crypt.bdae")
        return 9;

    std::vector<std::uint8_t> swamp_bdae, crypt_bdae;
    if (!read_file(asset_root + "/data/3d/gameobjects/go_swamp_urn_breakable.bdae",
                   swamp_bdae) ||
        !read_file(asset_root + "/data/3d/gameobjects/go_urn_crypt.bdae",
                   crypt_bdae))
        return 10;
    dh2::world::SourceObjectCensusV1 source{};
    source.gametype = "DestructibleContainer";
    source.template_name = "DestructibleContainer";
    source.data_desc_pydata = "DestructibleContainers";
    source.data_desc = "Swamp_Normal_DestructibleBarrel";
    source.name = "source_swamp_barrel";
    source.source_handle = 42;
    source.world.scale = {1.f, 1.f, 1.f};
    dh2::destructible_container_visual_provider_v1::Facts facts{};
    if (dh2::destructible_container_visual_provider_v1::build_init_post_facts(
            source, tables, *swamp_path, swamp_bdae.data(), swamp_bdae.size(),
            1, 0, facts, error) !=
        dh2::destructible_container_visual_provider_v1::Status::complete ||
        !facts.aabb.ready || facts.container_state_394 != 2 || facts.deleted_81 != 0 ||
        facts.condition_enabled != 1 || facts.aabb.max_x <= facts.aabb.min_x ||
        facts.aabb.max_y <= facts.aabb.min_y)
        return 11;

    source.data_desc = "Crypt02_Normal_Barrel";
    source.name = "source_crypt_barrel";
    if (dh2::destructible_container_visual_provider_v1::build_init_post_facts(
            source, tables, *crypt_path, crypt_bdae.data(), crypt_bdae.size(),
            1, 0, facts, error) !=
        dh2::destructible_container_visual_provider_v1::Status::complete ||
        !facts.aabb.ready || facts.container_state_394 != 2 || facts.deleted_81 != 0 ||
        facts.condition_enabled != 1 || facts.aabb.max_x <= facts.aabb.min_x ||
        facts.aabb.max_y <= facts.aabb.min_y)
        return 12;
    if (dh2::destructible_container_visual_provider_v1::build_init_post_facts(
            source, tables, *crypt_path, crypt_bdae.data(), crypt_bdae.size(),
            0, 0, facts, error) !=
        dh2::destructible_container_visual_provider_v1::Status::complete ||
        facts.aabb.ready || facts.container_state_394 != 4 ||
        facts.condition_enabled != 0)
        return 14;
    return 0;
}
