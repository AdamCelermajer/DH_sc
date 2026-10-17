#include "../crypt_module_catalog_v1.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>

namespace {

int fail(const std::string& message) {
  std::cerr << "FAIL: " << message << '\n';
  return 1;
}

std::string read_file(const std::filesystem::path& path) {
  std::ifstream input(path, std::ios::binary);
  if (!input) return {};
  return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

std::map<std::string, std::string> read_directory(
    const std::filesystem::path& path) {
  std::map<std::string, std::string> files;
  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (!entry.is_regular_file()) continue;
    files.emplace(entry.path().filename().string(), read_file(entry.path()));
  }
  return files;
}

}  // namespace

int main(int argc, char** argv) {
  using namespace dh2::random_level;
  if (argc != 2) {
    return fail("usage: crypt_module_catalog_v1 <cache/files>");
  }

  const CryptModuleAssetKeyV1 key_a{"straight_c_ns", "crypt_straight_c_ns_01.mgp",
                                    "crypt_straight_c_ns_01.mvp"};
  const CryptModuleAssetKeyV1 key_b{"straight_c_ns", "crypt_straight_c_ns_02.mgp",
                                    "crypt_straight_c_ns_01.mvp"};
  const CryptModuleAssetKeyLessV1 less;
  if (!less(key_a, key_b) || less(key_b, key_a)) {
    return fail("catalogue identity must preserve the exact name/gameplay/visual triple");
  }

  const std::filesystem::path cache_root(argv[1]);
  const auto rule_path = cache_root / "data" / "scene" / "007_crypt_01.rule.xml";
  const auto rule_xml = read_file(rule_path);
  if (rule_xml.empty()) return fail("could not read 007_crypt_01.rule.xml");

  const auto parsed = parse_crypt_rule_v1(rule_xml);
  if (!parsed) return fail("Crypt rule parse failed: " + parsed.error);

  const auto modules_root = cache_root / "data" / "3d" / "modules" / "crypt";
  const CryptModuleAssetPathsV1 paths{
      modules_root / "mgx", modules_root / "mgp", modules_root / "mvp",
      modules_root / "mvx", modules_root / "mgx" / "mgxlist.txt"};
  const auto catalogue = build_crypt_module_catalogue_v1(*parsed.document, paths);

  CryptModuleAssetBytesV1 bytes;
  bytes.mgx_list = read_file(paths.mgx_list_file);
  bytes.mgx = read_directory(paths.mgx_directory);
  bytes.mgp = read_directory(paths.mgp_directory);
  bytes.mvp = read_directory(paths.mvp_directory);
  bytes.mvx = read_directory(paths.mvx_directory);
  const auto byte_catalogue =
      build_crypt_module_catalogue_from_bytes_v1(*parsed.document, bytes);

  if (!catalogue.catalogue_issues.empty()) {
    for (const auto& issue : catalogue.catalogue_issues) std::cerr << "catalogue: " << issue << '\n';
    return fail("catalogue-level issue(s) found");
  }
  if (catalogue.mgx_list_entry_count != 21) {
    return fail("expected the source mgxlist to contain 21 definitions; found " +
                std::to_string(catalogue.mgx_list_entry_count));
  }
  if (catalogue.rule_candidates.empty()) return fail("Crypt rules contain no active list candidates");
  if (catalogue.assets_by_exact_key.empty()) return fail("catalogue has no exact-triple keys");

  for (const auto& candidate : catalogue.rule_candidates) {
    const auto found = catalogue.assets_by_exact_key.find(candidate.key);
    if (found == catalogue.assets_by_exact_key.end()) {
      return fail("candidate omitted from exact-triple catalogue");
    }
    if (!found->second.is_complete) {
      std::cerr << candidate.list_name << '[' << candidate.list_index << "] ("
                << candidate.key.name << ", " << candidate.key.gameplay << ", "
                << candidate.key.visual << ")\n";
      for (const auto& issue : found->second.issues) std::cerr << "  " << issue << '\n';
      return fail("candidate is not completely bound to source assets");
    }
    if (!found->second.mgx.has_grid_geometry ||
        !found->second.mvx_module.has_grid_geometry) {
      return fail("resolved module lost its MGX/MVX grid geometry");
    }
    if (found->second.xrefobject.empty() || found->second.dae.empty()) {
      return fail("resolved MVX scene root is missing xrefobject/dae");
    }
    if (found->second.scene_object_name != "_module_" + candidate.key.name ||
        found->second.xrefobject != "_module_" + candidate.key.name ||
        found->second.xrefmax != "data/iphone/3D/Modules/crypt/crypt.max" ||
        found->second.dae != "data/3D/Modules/crypt/crypt.bdae" ||
        found->second.scale != "1.0,1.0,1.0") {
      return fail("catalogue did not preserve source MVX object/path/scale attributes for " +
                  candidate.key.name);
    }
    if (!found->second.fog_color.empty() || !found->second.is_solid.empty()) {
      return fail("catalogue invented optional MVX fields absent from source data");
    }
  }
  if (catalogue.unresolved_candidate_count() != 0) {
    return fail("unresolved candidates reported after complete binding");
  }
  if (!byte_catalogue.catalogue_issues.empty() ||
      byte_catalogue.unresolved_candidate_count() != 0 ||
      byte_catalogue.rule_candidates.size() != catalogue.rule_candidates.size() ||
      byte_catalogue.assets_by_exact_key.size() != catalogue.assets_by_exact_key.size()) {
    return fail("byte-backed catalogue did not resolve the same source rule closure");
  }
  for (const auto& [key, filesystem_asset] : catalogue.assets_by_exact_key) {
    const auto found = byte_catalogue.assets_by_exact_key.find(key);
    if (found == byte_catalogue.assets_by_exact_key.end() ||
        !found->second.is_complete ||
        found->second.mgx_size != filesystem_asset.mgx_size ||
        found->second.gameplay_size != filesystem_asset.gameplay_size ||
        found->second.visual_size != filesystem_asset.visual_size ||
        found->second.mvx_size != filesystem_asset.mvx_size ||
        found->second.scene_object_name != filesystem_asset.scene_object_name ||
        found->second.xrefmax != filesystem_asset.xrefmax ||
        found->second.fog_color != filesystem_asset.fog_color ||
        found->second.is_solid != filesystem_asset.is_solid ||
        found->second.scale != filesystem_asset.scale ||
        found->second.xrefobject != filesystem_asset.xrefobject ||
        found->second.dae != filesystem_asset.dae) {
      return fail("byte-backed catalogue differs from filesystem catalogue for " + key.name);
    }
  }

  const auto sample_candidate = std::find_if(
      catalogue.rule_candidates.begin(), catalogue.rule_candidates.end(),
      [](const CryptRuleAssetCandidateV1& candidate) {
        return candidate.key.name == "straight_c_ns";
      });
  if (sample_candidate == catalogue.rule_candidates.end())
    return fail("source rules do not contain the straight_c_ns fixture");

  auto metadata_bytes = bytes;
  auto& metadata_mvx = metadata_bytes.mvx.at("straight_c_ns.mvx");
  const std::string decor_marker = "gametype=\"Decor\"";
  const auto decor_at = metadata_mvx.find(decor_marker);
  if (decor_at == std::string::npos)
    return fail("straight_c_ns MVX fixture has no Decor record");
  metadata_mvx.insert(decor_at + decor_marker.size(),
                      " fog_color=\"-1,-1,-1\" is_solid=\"1\"");
  const auto metadata_catalogue =
      build_crypt_module_catalogue_from_bytes_v1(*parsed.document, metadata_bytes);
  const auto metadata_asset =
      metadata_catalogue.assets_by_exact_key.find(sample_candidate->key);
  if (metadata_asset == metadata_catalogue.assets_by_exact_key.end() ||
      !metadata_asset->second.is_complete ||
      metadata_asset->second.fog_color != "-1,-1,-1" ||
      metadata_asset->second.is_solid != "1") {
    return fail("catalogue did not retain optional MVX fog_color/is_solid attributes");
  }

  auto malformed_bytes = bytes;
  auto& malformed_mvx = malformed_bytes.mvx.at("straight_c_ns.mvx");
  const std::string xref_marker = "xrefobject=\"_module_straight_c_ns\"";
  const auto xref_at = malformed_mvx.find(xref_marker);
  if (xref_at == std::string::npos)
    return fail("straight_c_ns MVX fixture has no expected xrefobject");
  malformed_mvx.insert(xref_at + xref_marker.size(),
                       " xrefobject=\"duplicate\"");
  const auto malformed_catalogue =
      build_crypt_module_catalogue_from_bytes_v1(*parsed.document, malformed_bytes);
  const auto malformed_asset =
      malformed_catalogue.assets_by_exact_key.find(sample_candidate->key);
  if (malformed_asset == malformed_catalogue.assets_by_exact_key.end() ||
      malformed_asset->second.is_complete || malformed_asset->second.issues.empty()) {
    return fail("malformed duplicate MVX attribute was not rejected");
  }

  std::cout << "Crypt module catalogue: " << catalogue.rule_candidates.size()
            << " active rule entries, " << catalogue.assets_by_exact_key.size()
            << " exact triples, " << catalogue.mgx_list_entry_count
            << " source MGX definitions; filesystem and byte-backed catalogues agree on"
            << " exact MGX/MGP/MVP/MVX bindings, MVX metadata and geometry; malformed XML rejected.\n";
  return 0;
}
