#pragma once

#include "crypt_mgx_connectivity_v1.hpp"
#include "native_rule_plan_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace dh2::random_level {

// Rule BlockSearch identity is the exact tuple, not the MGX block name alone.
// For example, several Crypt list elements reuse one MGX definition but pair
// it with a different gameplay file.
struct CryptModuleAssetKeyV1 {
  std::string name;
  std::string gameplay;
  std::string visual;
};

struct CryptModuleAssetKeyLessV1 {
  bool operator()(const CryptModuleAssetKeyV1& lhs,
                  const CryptModuleAssetKeyV1& rhs) const noexcept;
};

struct CryptModuleAssetPathsV1 {
  std::filesystem::path mgx_directory;
  std::filesystem::path mgp_directory;
  std::filesystem::path mvp_directory;
  std::filesystem::path mvx_directory;
  std::filesystem::path mgx_list_file;
};

// Byte-backed variant of the source cache inputs. Android assets are not
// regular filesystem paths, so the live app supplies the exact filenames and
// their bounded byte contents through this value instead of asking the
// catalogue to enumerate directories.
struct CryptModuleAssetBytesV1 {
  std::string mgx_list;
  std::map<std::string, std::string> mgx;
  std::map<std::string, std::string> mgp;
  std::map<std::string, std::string> mvp;
  std::map<std::string, std::string> mvx;
};

struct CryptModuleAssetV1 {
  CryptModuleAssetKeyV1 key;
  bool is_complete = false;

  bool mgx_listed = false;
  bool mgx_file_valid = false;
  bool gameplay_file_valid = false;
  bool visual_file_valid = false;
  bool mvx_file_valid = false;
  bool mvx_scene_root_valid = false;

  std::uintmax_t mgx_size = 0;
  std::uintmax_t gameplay_size = 0;
  std::uintmax_t visual_size = 0;
  std::uintmax_t mvx_size = 0;

  CryptMgxModuleV1 mgx;
  CryptMgxModuleV1 mvx_module;
  // Raw attributes from the sole MVX Decor GameObject. Keep source spellings
  // for the generated Level writer; several fields are optional in cache data.
  std::string scene_object_name;
  std::string xrefmax;
  std::string fog_color;
  std::string is_solid;
  std::string scale;
  std::string xrefobject;
  std::string dae;
  std::vector<std::string> issues;
};

struct CryptRuleAssetCandidateV1 {
  std::string list_name;
  std::size_t list_index = 0;
  CryptModuleAssetKeyV1 key;
};

struct CryptModuleCatalogueV1 {
  // Every active <elem> is retained here, including repeated triples and
  // repeated names with different gameplay/visual assets.
  std::vector<CryptRuleAssetCandidateV1> rule_candidates;
  // Exact triple keys match rnd::BlockSearch's lookup identity.
  std::map<CryptModuleAssetKeyV1, CryptModuleAssetV1,
           CryptModuleAssetKeyLessV1> assets_by_exact_key;
  std::vector<std::string> catalogue_issues;
  std::size_t mgx_list_entry_count = 0;

  std::size_t unresolved_candidate_count() const noexcept;
};

// Audits every active list element from the parsed Crypt rules against the
// original cache files. `mgx_list_file` is authoritative for MGX definitions;
// candidates never bind to a filesystem-only MGX omitted from that list.
// Referenced files are bounded, exact-case regular files. MVP/MGP are existence
// and bounded-size checked here; parsing their proprietary content is outside
// this catalogue slice.
CryptModuleCatalogueV1 build_crypt_module_catalogue_v1(
    const CryptRuleDocumentV1& rules,
    const CryptModuleAssetPathsV1& paths);

// Applies the same exact-triple, size, parser, scene-root and MGX/MVX geometry
// checks as the filesystem builder, using named bytes already read by the
// caller (for example through Android AAssetManager).
CryptModuleCatalogueV1 build_crypt_module_catalogue_from_bytes_v1(
    const CryptRuleDocumentV1& rules,
    const CryptModuleAssetBytesV1& assets);

}  // namespace dh2::random_level
