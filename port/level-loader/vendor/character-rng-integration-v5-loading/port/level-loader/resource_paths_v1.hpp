#pragma once
#include <string>
#include <vector>
namespace dh2::loader {
// Compiled-data branch of original Level::LoadFile, 0x3f3d14..0x3f3e30.
// Removes only the first occurrence of the first matching authoring marker,
// in the original marker priority. Case/slash folding belongs to asset lookup.
// Uncompiled-data mode and its conditional first read are not implemented here.
std::vector<std::string> compiled_level_paths_v1(const std::string& authored);
}
