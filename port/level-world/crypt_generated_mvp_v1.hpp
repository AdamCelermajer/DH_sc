#pragma once

#include "source_handle_ledger_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::world {

struct GeneratedCryptMvpFileV1 {
  const char* source_path = nullptr;
  const std::uint8_t* data = nullptr;
  std::size_t size = 0;
};

// Renderer-independent projection of unconditional source Crypt
// AnimatedDecor records. Provenance and source ObjectManager handles remain
// attached so runtime instancing can preserve first-name-wins ownership.
struct GeneratedCryptAnimatedDecorV1 {
  std::uint32_t module_index = 0;
  std::uint32_t source_record = 0;
  std::int32_t source_handle = -1;
  std::string name;
  std::string xrefobject;
  std::string dae_path;
  std::string source_path;
  std::string start_animation;
  float local_transform[9]{};
  float world_transform[9]{};
};

// IDA: Module::LoadModule (0x38a88c) calls Level::LoadFile (0x3f3b40)
// first for the gameplay XML selected by _ChooseXmls, then for its visual XML.
// This projection consumes the ordered visual files after the caller builds
// the same source handle ledger from interleaved MGP/MVP module file order.
// Conditioned/scripted or duplicate-name candidates are deferred/discarded;
// other MVP gametypes are outside this AnimatedDecor adapter.
bool compile_generated_crypt_animated_decor_v1(
    const std::uint8_t* level_xml, std::size_t level_size,
    const char* level_name, const char* level_source_path,
    const GeneratedCryptMvpFileV1* mvps, std::size_t mvp_count,
    const source_handle_ledger_v1::Ledger& handle_ledger,
    std::vector<GeneratedCryptAnimatedDecorV1>& output,
    std::size_t& deferred_count, std::string& error);

}  // namespace dh2::world
