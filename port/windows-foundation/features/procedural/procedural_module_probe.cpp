// P17 PROCGEN2 diagnostic (not a ctest): triangle and instance counts of named module nodes, decoded with the
// production module decoder (decode_original_scene_module, the path a generated Module GameObject takes).
// Usage: procedural_module_probe <module .bdae> <node name>...
#include "original_scene.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace dh::foundation;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: procedural_module_probe <module .bdae> <node name>...\n";
        return 2;
    }
    std::ifstream in(argv[1], std::ios::binary);
    if (!in) {
        std::cerr << "cannot open " << argv[1] << '\n';
        return 2;
    }
    const std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const std::array<float, 16> identity{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    for (int i = 2; i < argc; ++i) {
        OriginalScene scene;
        std::string error;
        if (!decode_original_scene_module(bytes, argv[i], identity, scene, error)) {
            std::cout << argv[i] << "\tERROR\t" << error << '\n';
            continue;
        }
        std::cout << argv[i] << "\ttriangles=" << scene.triangleCount << "\tinstances=" << scene.instanceCount
                  << "\tnodes=" << scene.nodeCount << '\n';
    }
    return 0;
}
