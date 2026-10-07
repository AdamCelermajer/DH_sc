#pragma once
#include "scene.hpp"
namespace dh2::scene {
// Native source node/material graph projection for particle-only scenes.
// Recognizes animation/emitter/force records for later required construction;
// they are counted as ignored_instances and never fabricated as mesh draws.
bool load_particle_scene_v1(const resources::BresView&,Scene&,std::string&);
}
