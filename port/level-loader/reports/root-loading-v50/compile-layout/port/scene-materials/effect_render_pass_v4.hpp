#pragma once
#include "blood_render_pass_v3.hpp"
namespace dh2::scene {
// Same proven rich-state conversion; shader/defines belong to the actual
// material technique. This successor supports other authored single-pass FX.
using EffectRenderPassV4=BloodRenderPassV3;
bool effect_render_pass_v4(const resources::BresView&,const char* actual_material,
 const char* actual_technique,EffectRenderPassV4&,std::string&);
}
