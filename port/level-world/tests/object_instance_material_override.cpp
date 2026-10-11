#include "objects.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    try {
        const std::string root = argv[1];
        auto read = [&](const char* name) {
            std::ifstream file(root + "/actors/" + name, std::ios::binary);
            return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(file), {});
        };
        const auto model = read("swampking.bdae");
        const auto clip = read("swampking_idle.bdae");
        dh2::objects::Resource resource;
        std::string error;
        if (!dh2::objects::load_resource(model.data(), model.size(), clip.data(),
                                        clip.size(), resource, error))
            throw std::runtime_error(error);
        dh2::resources::BresView view{};
        if (dh2_bres_open(&view, model.data(), model.size()) != dh2::resources::BresError::ok)
            throw std::runtime_error("SwampKing BRES rejected");
        bool checked_override = false;
        for (const auto& instance : resource.scene.instances) {
            if (instance.node != "Box01-node") continue;
            dh2::assets::Mesh mesh{};
            dh2::assets::Primitive raw{};
            if (dh2_mesh_open(&mesh, &view, instance.geometry) != dh2::assets::Error::ok ||
                dh2_mesh_primitive(&mesh, 0, &raw) != dh2::assets::Error::ok ||
                std::string(raw.material) != "ColorMaterial" ||
                resource.scene.materials.at(instance.materials.at(0)).id != "ColorMaterial_00060687")
                throw std::runtime_error("Original instance override changed");
            for (const auto& primitive : resource.primitives)
                if (primitive.node == instance.node_index && primitive.material == instance.materials[0])
                    checked_override = true;
        }
        if (!checked_override) throw std::runtime_error("Instance override not applied to draw primitive");
        if (!dh2::objects::sample(resource, resource.animation.end, error))
            throw std::runtime_error(error);
        std::cout << "SwampKing instance override and idle sampling passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
