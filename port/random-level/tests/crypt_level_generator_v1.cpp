#include "../crypt_level_generator_v1.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace dh2::random_level;

namespace {

unsigned checks = 0;
unsigned failures = 0;

void check(bool condition, const char* label) {
  ++checks;
  if (!condition) {
    ++failures;
    std::cerr << "FAIL: " << label << '\n';
  }
}

bool read_file(const std::string& path, std::string& output) {
  std::ifstream stream(path, std::ios::binary);
  if (!stream) return false;
  output.assign(std::istreambuf_iterator<char>(stream),
                std::istreambuf_iterator<char>());
  return stream.good() || stream.eof();
}

std::uint64_t fnv1a64(const std::uint8_t* bytes, std::size_t size) {
  std::uint64_t value = UINT64_C(14695981039346656037);
  for (std::size_t index = 0; index < size; ++index) {
    value ^= bytes[index];
    value *= UINT64_C(1099511628211);
  }
  return value;
}

std::uint64_t fnv1a64(std::string_view text) {
  return fnv1a64(reinterpret_cast<const std::uint8_t*>(text.data()),
                 text.size());
}

bool contains_root(const std::string& bres, std::string_view root) {
  std::string marker(root);
  marker += "-node";
  return bres.find(marker) != std::string::npos;
}

int run(const std::string& rule_path, const std::string& cache_root,
        const std::string& bres_path) {
  std::string rule_xml;
  std::string bres;
  check(read_file(rule_path, rule_xml), "actual Crypt rule fixture is readable");
  check(read_file(bres_path, bres), "shared Crypt BRES fixture is readable");
  if (failures != 0) return 1;

  const auto parsed = parse_crypt_rule_v1(rule_xml);
  check(static_cast<bool>(parsed), "actual Crypt rule fixture parses");
  if (!parsed) {
    std::cerr << parsed.error << '\n';
    return 1;
  }
  const auto base = std::filesystem::path(cache_root) / "data" / "3d" /
                    "modules" / "crypt";
  const CryptModuleAssetPathsV1 paths{
      base / "mgx", base / "mgp", base / "mvp", base / "mvx",
      base / "mgx" / "mgxlist.txt"};
  const auto catalogue = build_crypt_module_catalogue_v1(*parsed.document,
                                                         paths);
  check(catalogue.catalogue_issues.empty(),
        "real Crypt module catalogue has no audit issues");
  check(catalogue.unresolved_candidate_count() == 0,
        "all rule list tuples bind to validated exact assets");
  if (!catalogue.catalogue_issues.empty()) {
    for (const auto& issue : catalogue.catalogue_issues) {
      std::cerr << "catalogue: " << issue << '\n';
    }
  }
  for (const auto& [key, asset] : catalogue.assets_by_exact_key) {
    if (!asset.is_complete) {
      std::cerr << "incomplete: " << key.name << ": ";
      for (const auto& issue : asset.issues) std::cerr << issue << "; ";
      std::cerr << '\n';
    }
  }

  constexpr std::uint32_t seed = 0x00C0FFEEU;
  const auto generated = generate_crypt_level_v1(*parsed.document, catalogue,
                                                  seed);
  check(generated.status == CryptLevelGeneratorStatusV1::success,
        "source-backed Crypt generation succeeds with explicit seed");
  if (generated.status != CryptLevelGeneratorStatusV1::success) {
    std::cerr << "generator status=" << static_cast<int>(generated.status)
              << " message=" << generated.message
              << " generation="
              << static_cast<int>(generated.generation_result.status)
              << " root=" << static_cast<int>(generated.root_result.status)
              << "\n";
    return 1;
  }
  check(generated.seed == seed && generated.final_rng_state != seed,
        "explicit seed is retained and shared source RNG advances");
  check(generated.generation_result.row_rollbacks > 0,
        "real Crypt fixture exercises failed-row cleanup and retry");
  check(!generated.modules.empty() &&
            generated.modules.front().block_name == "cemetery_entrance" &&
            generated.modules.front().xrefobject ==
                "_module_cemetery_entrance",
        "fixed RootRule candidate is first in generated preorder");
  check(generated.layout.dwld_v1.size() == 24 + generated.modules.size() * 128 &&
            generated.layout.dwld_v1.size() >= 24 &&
            std::string(reinterpret_cast<const char*>(
                            generated.layout.dwld_v1.data()), 4) == "DWLD",
        "DWLD v1 count and record shape match generated placements");
  check(!generated.layout.spawn_selected,
        "layout does not invent an unverified SpawnPoint selection");

  for (const auto& module : generated.modules) {
    check(contains_root(bres, module.xrefobject),
          "every emitted catalogue root exists in shared Crypt BRES");
  }

  const auto replay = generate_crypt_level_v1(*parsed.document, catalogue,
                                               seed);
  check(replay.status == CryptLevelGeneratorStatusV1::success &&
            replay.final_rng_state == generated.final_rng_state &&
            replay.layout.source_level_xml == generated.layout.source_level_xml &&
            replay.layout.dwld_v1 == generated.layout.dwld_v1,
        "same explicit seed reproduces XML, DWLD and final RNG state");

  std::cout << "Crypt generation seed=0x" << std::hex << seed << std::dec
            << " modules=" << generated.modules.size()
            << " rows=" << generated.generation_result.rows_tried
            << " rollbacks=" << generated.generation_result.row_rollbacks
            << " candidates="
            << generated.generation_result.one_step_candidates_tried
            << " final_rng=0x" << std::hex << generated.final_rng_state
            << std::dec << '\n';
  for (std::size_t index = 0; index < generated.modules.size(); ++index) {
    const auto& module = generated.modules[index];
    std::cout << index << ' ' << module.block_name << ' '
              << module.xrefobject << " grid=" << module.grid_x << ','
              << module.grid_y << " z=" << module.elevation << '\n';
  }
  std::cout << "XML FNV-1a64=0x" << std::hex
            << fnv1a64(generated.layout.source_level_xml)
            << " DWLD FNV-1a64=0x"
            << fnv1a64(generated.layout.dwld_v1.data(),
                       generated.layout.dwld_v1.size())
            << std::dec << '\n';
  std::cout << checks - failures << '/' << checks
            << " Crypt level generator checks passed\n";
  return failures == 0 ? 0 : 1;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cerr << "usage: crypt_level_generator_v1_audit <rule.xml> "
                 "<original-cache-root> <crypt.bdae>\n";
    return 2;
  }
  return run(argv[1], argv[2], argv[3]);
}
