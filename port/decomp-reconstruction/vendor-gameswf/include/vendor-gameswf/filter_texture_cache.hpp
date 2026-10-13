// gameswf::filter_texture_cache (Gameloft, not in gameswf r1714).
//
// A texture_cache (texture_cache.hpp) used for filter results. The binary calls the texture_cache
// members on this object (the used-region hash at +0x30, find_available_region), so the class derives
// from texture_cache and adds no members that are recovered yet.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack0-157e56-0.cpp. get_character_region (0x758a80) is
// not written: see the batch notes.

#pragma once

#include "vendor-gameswf/texture_cache.hpp"

namespace gameswf {

class filter_texture_cache : public texture_cache {
public:
	// 0x756cdc gameswf::filter_texture_cache::get_region_size_requirement_cellsize(int&, int&).
	// Rounds both sizes (pixels) up to whole 16-pixel cells, with a minimum of one cell. Returns the
	// remainder of the original height (height % 16). The object itself is not read.
	int get_region_size_requirement_cellsize(int& width, int& height);
};

}  // namespace gameswf
