#pragma once
#include "../asset-payloads/payloads.hpp"
#include <array>
#include <string>
#include <vector>

namespace dh2::scene {
struct Material {
    std::string id, diffuse, alpha_map;
    float color[4]{1,1,1,1};
    float texture_matrix[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    float alpha_ref=0;
    bool additive=false, backface=false;
};
struct Instance {
    std::string node;
    std::uint32_t node_index;
    std::uint32_t geometry;
    std::array<float,16> world;
    std::vector<std::uint32_t> materials;
    std::int32_t controller=-1;
};
struct Node {
    std::string id,sid,name,user_properties;
    std::int32_t parent=-1;
    float translation[3]{},quaternion[4]{0,0,0,1},scale[3]{1,1,1};
    std::array<float,16> world{};
};
struct Scene {
    std::vector<Material> materials;
    std::vector<Instance> instances;
    std::vector<Node> graph;
    unsigned nodes=0, ignored_instances=0;
};
// Checked reconstruction of serialized node, image and material links.
// Owns strings and matrices, borrows no file bytes. Does not create the
// original engine's shaders, lights, animation or runtime object ABI.
bool load(const resources::BresView&, Scene&, std::string& error);
bool update_world(Scene&,std::string& error);
std::array<float,16> multiply(const std::array<float,16>&, const std::array<float,16>&);
}
// Pure matrix entry point for original-instruction differential validation.
extern "C" void dh2_node_matrix(float* out16,const float* translation3,const float* quaternion4,const float* scale3);
