// GetLocalPosition(gameswf::character*, float, float) (<global>, batch g1-game-app-global-f216b6-21). Maps a point, scaled by
// 20, from the character's world space into its local space through the inverse of its world matrix. Defined in
// game-app/src/g1-game-app-global-f216b6-21_gameswf.cpp.
#pragma once

#include "gameswf/gameswf_character.h"  // gameswf::character, gameswf::point, gameswf::matrix (vendored gameswf r1714)

gameswf::point GetLocalPosition(gameswf::character* ch, float x, float y);  // 0x7aa2e0
