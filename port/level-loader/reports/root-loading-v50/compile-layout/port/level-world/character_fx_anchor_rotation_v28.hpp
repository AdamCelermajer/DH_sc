#pragma once
#include "visual_motion.hpp"
#include <string>
namespace dh2::fx {
// Original AnimatedFX.SyncIrrData492b3c..bb8 positive VisualObject branch:
// actual CScene ROOT absolute transform -> getRotationDegrees432bbc ->
// float radians conversion. The root is the SAME owner root, not an authored
// animated bone/helper child and not a copied actor Euler substitute.
bool character_fx_anchor_rotation_v28(float out_radians[3],
 const visual::Root& same_visual_root,std::string& error);
}
