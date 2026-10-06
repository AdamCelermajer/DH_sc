#pragma once
#include "../scene-materials/scene.hpp"
#include <array>
#include <string>
#include <vector>
namespace dh2::skinning {
using Matrix=std::array<float,16>;
struct Influence {std::array<std::uint8_t,4> joints{};std::array<float,4> weights{};};
struct Skin {
    std::string id;
    unsigned geometry=0,influence_count=0;
    Matrix bind_shape{};
    std::vector<unsigned> nodes;
    std::vector<Matrix> inverse_bind;
    std::vector<Influence> influences;
};
// Complete-file deferred buffers are copied into owned, checked records.
bool load(const resources::BresView&,unsigned controller,const scene::Scene&,Skin&,std::string&);
bool palette(const Skin&,const scene::Scene&,std::vector<Matrix>&,std::string&);
bool positions(const Skin&,const std::vector<Matrix>&,const std::vector<std::array<float,3>>& input,
               std::vector<std::array<float,3>>& output,std::string&);
}
extern "C" void dh2_skin_matrix(float* out,const float* world,const float* inverse_bind);
extern "C" void dh2_skin_palette_matrix(float* out,const float* world,const float* inverse_bind,const float* bind_shape);
extern "C" void dh2_skin_point(float* out,const float* matrices,const std::uint8_t* indices,const float* weights,unsigned count,const float* point);
