#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::world {

struct GeneratedMgpView {
    const char* source_path;
    const std::uint8_t* data;
    std::size_t size;
};

// Compile authored SpawnPoints from a serialized <Level> and its MGPs in
// source Module order into the Android runtime's SPWN v1 sidecar. IDA evidence:
// Level::_LoadPlayer (0x3f0018) matches active type-13 objects by entrypoint ID
// and calls SpawnPoint::PlaceObject (0x3ea22c), which applies position/rotation
// and runs the object's script. ObjectManager::LoadFromXML (0x34b868) adds the
// Module origin to MGP XYZ only; object rotation and scale remain authored.
//
// SPWN v1 cannot carry activation conditions or scripts, and its current
// reader rejects duplicate entrypoint IDs. This function therefore fails
// closed if any SpawnPoint has a nonempty activate_cond/deactivate_cond/script
// or if any entrypoint ID repeats. It does not decide which conditional point
// would be active at runtime.
bool compile_generated_spawnpoints_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedMgpView* mgps, std::size_t mgp_count,
    std::vector<std::uint8_t>& output, std::string& error);

// Compile only the selected entrypoint used by the current level load. The
// source Level::_LoadPlayer loop applies every active SpawnPoint with this ID
// in ObjectManager order, so the final transform comes from the last matching
// module/MGP record. Selected points with conditions or scripts still fail
// closed because this projection does not run those source callbacks.
bool compile_generated_spawnpoint_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedMgpView* mgps, std::size_t mgp_count,
    std::int32_t selected_entrypoint_id,
    std::vector<std::uint8_t>& output, std::string& error);

} // namespace dh2::world
