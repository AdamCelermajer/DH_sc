#include "../world.hpp"
#include "../objects.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::vector<std::uint8_t> read(const char* path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), {}};
}
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main(int argc, char** argv) {
    if (argc != 7) {
        std::cerr << "usage: swamp_static_world_audit swamp.bdae 001_swamp.dwld 001_swamp.spwn 001_swamp.dact 001_swamp.mlx asset-data-dir\n";
        return 2;
    }
    try {
        const auto bres_bytes = read(argv[1]);
        const auto descriptor = read(argv[2]);
        const auto spawn_bytes = read(argv[3]);
        const auto source_mlx = read(argv[5]);
        std::vector<std::uint8_t> source_descriptor;
        std::string error;
        const bool source_layout_ok = dh2::world::compile_source_layout(
                source_mlx.data(), source_mlx.size(), spawn_bytes.data(),
                spawn_bytes.size(), 0, source_descriptor, error);
        require(source_layout_ok, error.c_str());
        require(source_descriptor == descriptor,
                "runtime source MLX import differs from the checked-in DWLD checkpoint");
        dh2::resources::BresView bres{};
        require(dh2_bres_open(&bres, bres_bytes.data(), bres_bytes.size()) ==
                dh2::resources::BresError::ok, "SWAMP BRES rejected");

        dh2::world::Level level;
        require(dh2::world::load(bres, descriptor.data(), descriptor.size(), level, error),
                error.c_str());
        require(level.rooms == 9 && level.scene.instances.size() > 100 &&
                level.native_floor && level.native_floor->records.size() >= 9,
                "SWAMP module/world closure is incomplete");

        unsigned water_floors = 0, hole_floors = 0;
        for (const auto& floor : level.native_floor->records) {
            water_floors += (floor->flags.floor & 2u) != 0;
            hole_floors += (floor->flags.floor & 1u) != 0;
        }
        require(water_floors && hole_floors, "SWAMP water/hole floor flags were not decoded");
        for (const auto& instance : level.scene.instances) {
            dh2::assets::Mesh mesh{};
            require(dh2_mesh_open(&mesh, &bres, instance.geometry) == dh2::assets::Error::ok,
                    "SWAMP visual mesh rejected");
            require(mesh.primitives == instance.materials.size(),
                    "SWAMP material binding count differs");
            for (unsigned primitive_index = 0; primitive_index < mesh.primitives; ++primitive_index) {
                dh2::assets::Primitive primitive{};
                require(dh2_mesh_primitive(&mesh, primitive_index, &primitive) ==
                        dh2::assets::Error::ok, "SWAMP primitive rejected");
                const auto& material = level.scene.materials.at(instance.materials[primitive_index]);
                require(material.id == primitive.material,
                        "SWAMP source material binding differs from primitive symbol");
            }
        }

        std::vector<dh2::world::EntryPoint> entries;
        require(dh2::world::load_entrypoints(spawn_bytes.data(), spawn_bytes.size(),
                level.rooms, entries, error), error.c_str());
        require(entries.size() == 3 && entries[0].id == 0 && entries[1].id == 3 &&
                entries[2].id == 13, "SWAMP supported spawn IDs changed");
        dh2::world::SpawnSelection start;
        require(dh2::world::select_entrypoint(level, entries, 0, start, error), error.c_str());
        require(start.source.name == "_prim_EntryPoint" && start.floor_snapped &&
                std::all_of(start.position.begin(), start.position.end(),
                    [](float value) { return std::isfinite(value); }),
                "SWAMP entrypoint zero did not resolve to source floor");

        const auto dact_bytes = read(argv[4]);
        const std::string data_root = argv[6];
        const auto character_records = read((data_root + "/character_properties_pyarray.bin").c_str());
        const auto character_names = read((data_root + "/character_properties_pyarraynames.bin").c_str());
        const auto character_fields = read((data_root + "/character_properties_pystructnames.bin").c_str());
        const auto model_names = read((data_root + "/character_models_dictionary_pyarraynames.bin").c_str());
        const auto model_values = read((data_root + "/character_models_dictionary_pyarray.bin").c_str());
        dh2::data::CharacterTable characters;
        dh2::data::Dictionary models;
        require(dh2::data::load_characters({character_records.data(), character_records.size()},
                {character_names.data(), character_names.size()},
                {character_fields.data(), character_fields.size()}, characters, error),
                "SWAMP CharacterTable assets rejected");
        require(dh2::data::load_dictionary({model_names.data(), model_names.size()},
                {model_values.data(), model_values.size()}, models, error),
                "SWAMP model dictionary rejected");
        std::vector<dh2::objects::Record> actors;
        require(dh2::objects::load_records(dact_bytes.data(), dact_bytes.size(), level.rooms,
                characters, models, actors, error), error.c_str());
        require(actors.size() == 5 &&
                std::all_of(actors.begin(), actors.end(), [](const auto& actor) {
                    return actor.kind == 1 && actor.room < 9 &&
                           (actor.character == "Troll" ||
                            actor.character == "Swamp_LizadMan_Type1" ||
                            actor.character == "Swamp_LizadMan_Type2");
                }), "SWAMP direct Monster records differ from the supported source subset");

        std::cout << "{\"rooms\":" << level.rooms << ",\"floors\":"
                  << level.native_floor->records.size() << ",\"visual_instances\":"
                  << level.scene.instances.size() << ",\"water_floors\":" << water_floors
                  << ",\"hole_floors\":" << hole_floors
                  << ",\"material_mismatches\":0"
                  << ",\"entrypoints\":[0,3,13],\"entrypoint_zero_floor_snapped\":true"
                  << ",\"actors\":" << actors.size() << ",\"actor_models\":3}\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
