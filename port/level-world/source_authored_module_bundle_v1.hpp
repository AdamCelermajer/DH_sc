#pragma once

#include "source_handle_ledger_v1.hpp"
#include "world.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::world::source_authored_module_bundle_v1 {

struct ModuleFiles {
    std::uint32_t module_index{};
    std::string name;
    std::string mgp_path;
    std::string mvp_path;
    std::vector<std::uint8_t> mgp_bytes;
    std::vector<std::uint8_t> mvp_bytes;
};

// Owns the ordered source bytes and the borrowed views used by census and
// ledger consumers. Keep this object alive while any view/ledger provenance
// is consumed; its vector storage is not copied into a second asset owner.
struct Bundle {
    std::vector<ModuleFiles> modules;
    std::vector<SourceMgpView> mgp_views;
    std::vector<SourceMvpView> mvp_views;
    std::vector<source_handle_ledger_v1::OrderedModuleFile> ordered_files;
    source_handle_ledger_v1::Ledger handle_ledger;
};

struct Services {
    void* context{};
    bool (*read_cache)(void* context, const char* normalized_path,
                       std::vector<std::uint8_t>& bytes,
                       std::string& error){};
};

enum class Status : std::uint8_t {
    complete,
    invalid_input,
    module_refs_failed,
    cache_read_failed,
    ledger_failed,
    allocation_failed,
};

// Imports an authored MLX using the source-world parser, reads each exact
// normalized module MGP then MVP in authored module order, and validates the
// resulting occurrence ledger through the existing ObjectManager model.
// `first_source_handle` and root offset are explicit lifecycle inputs; they
// are never inferred from module count or a generated-level convention.
Status load(const std::uint8_t* mlx, std::size_t mlx_size,
           const char* level_name, const char* source_path,
           const Services& services,
           std::uint32_t first_source_handle,
           std::uint32_t initial_top_level_handle_offset,
           Bundle& output, std::string& error);

}  // namespace dh2::world::source_authored_module_bundle_v1
