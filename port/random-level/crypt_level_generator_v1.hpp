#pragma once

#include "crypt_generated_layout_v1.hpp"
#include "crypt_module_catalog_v1.hpp"
#include "crypt_rule_generation_runtime_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::random_level {

enum class CryptLevelGeneratorStatusV1 {
  success,
  invalid_arguments,
  unsupported_rules,
  incomplete_catalogue,
  no_solution,
  generation_error,
  serialization_error,
};

struct CryptGeneratedModuleSummaryV1 {
  std::string block_name;
  std::string xrefobject;
  std::string gameplay;
  std::string visual;
  std::int32_t grid_x = 0;
  std::int32_t grid_y = 0;
  float elevation = 0.0f;
};

struct CryptLevelGeneratorResultV1 {
  CryptLevelGeneratorStatusV1 status =
      CryptLevelGeneratorStatusV1::invalid_arguments;
  std::uint32_t seed = 0;
  std::uint32_t final_rng_state = 0;
  std::vector<CryptGeneratedModuleSummaryV1> modules;
  RootRuleRuntimeResultV1 root_result;
  CryptRuleGenerationResultV1 generation_result;
  CryptGeneratedLayoutV1 layout;
  CryptGeneratedLayoutDiagnosticV1 layout_diagnostic;
  std::string message;
};

// Executes the bounded Crypt Rule::Step/OneStep adapter entirely in memory.
// The document and catalogue must be the parsed/validated source data and stay
// alive for the duration of this call. One RandomGeneratorV1 is shared by root
// selection, Path construction, recursive distribution rows, and shuffles.
// Unsupported rule forms or absent binary-dependent data fail closed.
CryptLevelGeneratorResultV1 generate_crypt_level_v1(
    const CryptRuleDocumentV1& document,
    const CryptModuleCatalogueV1& catalogue, std::uint32_t seed,
    const CryptGeneratedSpawnV1* spawn = nullptr);

}  // namespace dh2::random_level
