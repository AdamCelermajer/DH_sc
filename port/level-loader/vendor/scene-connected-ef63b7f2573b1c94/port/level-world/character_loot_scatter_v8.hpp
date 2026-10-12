#pragma once
#include "../game-data/loot_tables_v2.hpp"
namespace dh2::character {
struct LootScatterServicesV8 {
 void* context{};
 // Original Point3D<float>::Normalize34d0b0. Required before any RNG draw
 // only when killer is present; zero-length/NaN behavior is not guessed.
 bool(*normalize)(void*,float[3],std::string&){};
 const float* vec3f_k{}; // actual source Vec3f_K global, not a chosen axis
};
// Whole GetRandomDropPos3ec668 ordering with same global Random authority as
// item generation. Source3ec5d4 and401bxx both resolve99f89c/99f8a4.
bool character_loot_scatter_v8(data::LootRandom8V2&,const float source[3],
                              const float* killer,float out[3],
                              const LootScatterServicesV8&,std::string&);
}
