#include "crypt_module_catalog_v1.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <map>
#include <set>
#include <string_view>
#include <tuple>
#include <utility>

namespace dh2::random_level {
namespace {

constexpr std::uintmax_t kMaxXmlBytes = 1024U * 1024U;
constexpr std::uintmax_t kMaxListBytes = 256U * 1024U;
constexpr std::uintmax_t kMaxModuleAssetBytes = 64U * 1024U * 1024U;
constexpr std::size_t kMaxMgxListEntries = 4096;
constexpr std::size_t kMaxXmlAttributes = 64;

struct ExactDirectory {
  std::map<std::string, std::filesystem::path> files;
  std::map<std::string, std::string> memory_files;
  std::string error;
};

bool is_space(char value) {
  return std::isspace(static_cast<unsigned char>(value)) != 0;
}

char ascii_lower(char value) {
  return value >= 'A' && value <= 'Z'
             ? static_cast<char>(value + ('a' - 'A'))
             : value;
}

bool equals_ascii_case_insensitive(std::string_view lhs,
                                   std::string_view rhs) {
  if (lhs.size() != rhs.size()) return false;
  for (std::size_t i = 0; i < lhs.size(); ++i) {
    if (ascii_lower(lhs[i]) != ascii_lower(rhs[i])) return false;
  }
  return true;
}

bool safe_ascii_basename(std::string_view value, std::string_view extension) {
  if (value.size() <= extension.size() || value.substr(value.size() - extension.size()) != extension) {
    return false;
  }
  if (value.find("..") != std::string_view::npos) return false;
  const auto stem = value.substr(0, value.size() - extension.size());
  if (stem.empty() || stem.find('.') != std::string_view::npos) return false;
  for (const char ch : value) {
    const bool allowed = (ch >= 'a' && ch <= 'z') ||
                         (ch >= 'A' && ch <= 'Z') ||
                         (ch >= '0' && ch <= '9') || ch == '_' || ch == '-' ||
                         ch == '.';
    if (!allowed) return false;
  }
  return true;
}

ExactDirectory index_exact_directory(const std::filesystem::path& directory) {
  ExactDirectory result;
  std::error_code error;
  if (!std::filesystem::is_directory(directory, error) || error) {
    result.error = "not a readable directory: " + directory.string();
    return result;
  }
  for (std::filesystem::directory_iterator it(directory, error), end;
       !error && it != end; it.increment(error)) {
    const auto type = it->symlink_status(error);
    if (error) break;
    if (!std::filesystem::is_regular_file(type)) continue;
    const auto filename = it->path().filename().string();
    if (!result.files.emplace(filename, it->path()).second) {
      result.error = "ambiguous duplicate exact filename in " + directory.string() +
                     ": " + filename;
      return result;
    }
  }
  if (error) result.error = "cannot enumerate directory: " + directory.string();
  return result;
}

ExactDirectory index_memory_directory(
    const std::map<std::string, std::string>& files) {
  ExactDirectory result;
  result.memory_files = files;
  return result;
}

bool has_asset_file(const ExactDirectory& directory, std::string_view filename) {
  const std::string key(filename);
  return directory.files.find(key) != directory.files.end() ||
         directory.memory_files.find(key) != directory.memory_files.end();
}

bool read_bounded_file(const std::filesystem::path& path,
                       std::uintmax_t max_bytes, std::string& output,
                       std::uintmax_t& file_size, std::string& error) {
  std::error_code fs_error;
  file_size = std::filesystem::file_size(path, fs_error);
  if (fs_error) {
    error = "cannot read file size: " + path.filename().string();
    return false;
  }
  if (file_size == 0) {
    error = "file is empty: " + path.filename().string();
    return false;
  }
  if (file_size > max_bytes) {
    error = "file exceeds parser bound: " + path.filename().string();
    return false;
  }
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    error = "cannot open file: " + path.filename().string();
    return false;
  }
  output.assign(static_cast<std::size_t>(file_size), '\0');
  input.read(output.data(), static_cast<std::streamsize>(output.size()));
  if (input.gcount() != static_cast<std::streamsize>(output.size())) {
    error = "short read: " + path.filename().string();
    output.clear();
    return false;
  }
  return true;
}

bool read_bounded_asset(const ExactDirectory& directory,
                        std::string_view filename,
                        std::uintmax_t max_bytes, std::string& output,
                        std::uintmax_t& file_size, std::string& error) {
  const std::string key(filename);
  if (const auto memory = directory.memory_files.find(key);
      memory != directory.memory_files.end()) {
    file_size = memory->second.size();
    if (file_size == 0) {
      error = "file is empty: " + key;
      return false;
    }
    if (file_size > max_bytes) {
      error = "file exceeds parser bound: " + key;
      return false;
    }
    output = memory->second;
    return true;
  }
  const auto file = directory.files.find(key);
  if (file == directory.files.end()) {
    error = "missing exact-case asset: " + key;
    return false;
  }
  return read_bounded_file(file->second, max_bytes, output, file_size, error);
}

bool validate_binary_asset(const ExactDirectory& directory,
                           const std::filesystem::path& configured_directory,
                           std::string_view filename, std::string_view extension,
                           std::uintmax_t& size, std::string& issue) {
  if (!safe_ascii_basename(filename, extension)) {
    issue = "unsafe or malformed asset filename: " + std::string(filename);
    return false;
  }
  const std::string key(filename);
  if (directory.files.find(key) == directory.files.end() &&
      directory.memory_files.find(key) == directory.memory_files.end()) {
    issue = "missing exact-case asset: " + configured_directory.string() + "/" +
            std::string(filename);
    return false;
  }
  std::string contents;
  std::string error;
  if (!read_bounded_asset(directory, filename, kMaxModuleAssetBytes,
                          contents, size, error)) {
    issue = "cannot use asset " + std::string(filename) + ": " + error;
    return false;
  }
  return true;
}

bool xml_name_start(char ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || ch == ':';
}

bool xml_name_char(char ch) {
  return xml_name_start(ch) || (ch >= '0' && ch <= '9') || ch == '-' || ch == '.';
}

struct XmlTag {
  std::string name;
  std::map<std::string, std::string> attributes;
  bool closing = false;
  bool self_closing = false;
};

class MvxTagScanner {
 public:
  explicit MvxTagScanner(std::string_view source) : source_(source) {}

  bool next(XmlTag& tag, bool& end_of_document) {
    tag = XmlTag{};
    end_of_document = false;
    if (!skip_misc()) return false;
    if (cursor_ == source_.size()) {
      end_of_document = true;
      return true;
    }
    if (source_[cursor_] != '<') return fail("expected MVX XML tag");
    ++cursor_;
    if (cursor_ < source_.size() && source_[cursor_] == '/') {
      tag.closing = true;
      ++cursor_;
    } else if (cursor_ < source_.size() && source_[cursor_] == '!') {
      return fail("unsupported MVX declaration");
    }
    if (!read_name(tag.name)) return false;
    if (tag.closing) {
      skip_space();
      if (cursor_ == source_.size() || source_[cursor_++] != '>') {
        return fail("malformed MVX closing tag");
      }
      return true;
    }
    while (true) {
      skip_space();
      if (source_.substr(cursor_, 2) == "/>" ) {
        cursor_ += 2;
        tag.self_closing = true;
        return true;
      }
      if (cursor_ < source_.size() && source_[cursor_] == '>') {
        ++cursor_;
        return true;
      }
      if (tag.attributes.size() >= kMaxXmlAttributes) {
        return fail("MVX attribute count exceeds parser bound");
      }
      std::string key;
      std::string value;
      if (!read_name(key)) return false;
      skip_space();
      if (cursor_ == source_.size() || source_[cursor_++] != '=') {
        return fail("MVX attribute is missing '='");
      }
      skip_space();
      if (!read_value(value)) return false;
      if (!tag.attributes.emplace(std::move(key), std::move(value)).second) {
        return fail("duplicate MVX attribute");
      }
    }
  }

  const std::string& error() const { return error_; }

 private:
  bool fail(std::string error) {
    if (error_.empty()) error_ = std::move(error) + " at byte " + std::to_string(cursor_);
    return false;
  }

  void skip_space() {
    while (cursor_ < source_.size() && is_space(source_[cursor_])) ++cursor_;
  }

  bool skip_misc() {
    while (true) {
      skip_space();
      if (source_.substr(cursor_, 4) == "<!--") {
        const auto end = source_.find("-->", cursor_ + 4);
        if (end == std::string_view::npos) return fail("unterminated MVX comment");
        cursor_ = end + 3;
      } else if (source_.substr(cursor_, 2) == "<?") {
        const auto end = source_.find("?>", cursor_ + 2);
        if (end == std::string_view::npos) return fail("unterminated MVX processing instruction");
        cursor_ = end + 2;
      } else {
        return true;
      }
    }
  }

  bool read_name(std::string& name) {
    if (cursor_ >= source_.size() || !xml_name_start(source_[cursor_])) {
      return fail("expected MVX XML name");
    }
    const auto begin = cursor_++;
    while (cursor_ < source_.size() && xml_name_char(source_[cursor_])) ++cursor_;
    name.assign(source_.substr(begin, cursor_ - begin));
    return true;
  }

  bool read_value(std::string& value) {
    if (cursor_ >= source_.size() ||
        (source_[cursor_] != '\'' && source_[cursor_] != '"')) {
      return fail("expected quoted MVX attribute");
    }
    const char quote = source_[cursor_++];
    const auto begin = cursor_;
    while (cursor_ < source_.size() && source_[cursor_] != quote) {
      if (source_[cursor_] == '&') return fail("MVX entities are outside parser scope");
      ++cursor_;
    }
    if (cursor_ == source_.size()) return fail("unterminated MVX attribute");
    value.assign(source_.substr(begin, cursor_ - begin));
    ++cursor_;
    return true;
  }

  std::string_view source_;
  std::size_t cursor_ = 0;
  std::string error_;
};

struct MvxSceneRootV1 {
  std::string object_name;
  std::string xrefmax;
  std::string fog_color;
  std::string is_solid;
  std::string scale;
  std::string xrefobject;
  std::string dae;
};

bool extract_mvx_scene_root(std::string_view xml, MvxSceneRootV1& scene,
                            std::string& error) {
  MvxTagScanner scanner(xml);
  XmlTag tag;
  bool end = false;
  if (!scanner.next(tag, end) || end || tag.name != "Module" || tag.closing || tag.self_closing) {
    error = scanner.error().empty() ? "MVX root must be a non-empty <Module>" : scanner.error();
    return false;
  }
  std::size_t decor_count = 0;
  bool saw_close = false;
  while (!saw_close) {
    if (!scanner.next(tag, end)) {
      error = scanner.error();
      return false;
    }
    if (end) {
      error = "unterminated MVX <Module>";
      return false;
    }
    if (tag.closing) {
      if (tag.name != "Module") {
        error = "unexpected MVX closing tag";
        return false;
      }
      saw_close = true;
      continue;
    }
    if (tag.name != "GameObject" || !tag.self_closing) {
      error = "MVX scene children must be self-closing <GameObject> records";
      return false;
    }
    const auto game_type = tag.attributes.find("gametype");
    if (game_type == tag.attributes.end() ||
        !equals_ascii_case_insensitive(game_type->second, "Decor")) {
      continue;
    }
    ++decor_count;
    const auto xref = tag.attributes.find("xrefobject");
    const auto dae_attribute = tag.attributes.find("dae");
    if (xref == tag.attributes.end() || xref->second.empty() ||
        dae_attribute == tag.attributes.end() || dae_attribute->second.empty()) {
      error = "MVX Decor root is missing xrefobject or dae";
      return false;
    }
    scene.xrefobject = xref->second;
    scene.dae = dae_attribute->second;
    const auto copy_optional = [&tag](std::string_view name,
                                      std::string& output) {
      const auto found = tag.attributes.find(std::string(name));
      if (found != tag.attributes.end()) output = found->second;
    };
    copy_optional("name", scene.object_name);
    copy_optional("xrefmax", scene.xrefmax);
    copy_optional("fog_color", scene.fog_color);
    copy_optional("is_solid", scene.is_solid);
    copy_optional("scale", scene.scale);
  }
  if (decor_count != 1) {
    error = "MVX must contain exactly one Decor scene root; found " +
            std::to_string(decor_count);
    return false;
  }
  if (!scanner.next(tag, end) || !end) {
    error = scanner.error().empty() ? "trailing MVX content" : scanner.error();
    return false;
  }
  return true;
}

std::set<std::string> parse_mgx_list(std::string_view contents,
                                    std::size_t& entry_count,
                                    std::vector<std::string>& issues) {
  std::set<std::string> entries;
  std::size_t cursor = 0;
  while (cursor <= contents.size()) {
    const auto end = contents.find('\n', cursor);
    auto line = contents.substr(
        cursor, end == std::string_view::npos ? contents.size() - cursor : end - cursor);
    while (!line.empty() && is_space(line.front())) line.remove_prefix(1);
    while (!line.empty() && is_space(line.back())) line.remove_suffix(1);
    if (!line.empty()) {
      if (entries.size() >= kMaxMgxListEntries) {
        issues.push_back("mgxlist: entry count exceeds parser bound");
        return entries;
      }
      if (!safe_ascii_basename(line, ".mgx")) {
        issues.push_back("mgxlist: unsafe or malformed filename: " + std::string(line));
      } else if (!entries.emplace(line).second) {
        issues.push_back("mgxlist: duplicate filename: " + std::string(line));
      }
    }
    if (end == std::string::npos) break;
    cursor = end + 1;
  }
  entry_count = entries.size();
  return entries;
}

std::set<std::string> read_mgx_list(const CryptModuleAssetPathsV1& paths,
                                   std::size_t& entry_count,
                                   std::vector<std::string>& issues) {
  std::string contents;
  std::uintmax_t size = 0;
  std::string error;
  if (!read_bounded_file(paths.mgx_list_file, kMaxListBytes, contents, size, error)) {
    issues.push_back("mgxlist: " + error);
    return {};
  }
  return parse_mgx_list(contents, entry_count, issues);
}

std::set<std::string> parse_mgx_list_bytes(
    std::string_view contents, std::size_t& entry_count,
    std::vector<std::string>& issues) {
  if (contents.empty()) {
    issues.push_back("mgxlist: file is empty");
    return {};
  }
  if (contents.size() > kMaxListBytes) {
    issues.push_back("mgxlist: file exceeds parser bound");
    return {};
  }
  return parse_mgx_list(contents, entry_count, issues);
}

void add_issue(CryptModuleAssetV1& asset, std::string issue) {
  asset.issues.push_back(std::move(issue));
}

CryptModuleAssetV1 audit_asset(
    const CryptModuleAssetKeyV1& key, const CryptModuleAssetPathsV1& paths,
    const std::set<std::string>& mgx_list,
    const ExactDirectory& mgx_directory, const ExactDirectory& mgp_directory,
    const ExactDirectory& mvp_directory, const ExactDirectory& mvx_directory) {
  CryptModuleAssetV1 asset;
  asset.key = key;

  if (key.name.empty() || key.gameplay.empty() || key.visual.empty()) {
    add_issue(asset, "rule candidate has an empty name/gameplay/visual field");
    return asset;
  }

  const std::string mgx_filename = key.name + ".mgx";
  if (!safe_ascii_basename(mgx_filename, ".mgx")) {
    add_issue(asset, "unsafe MGX module name: " + key.name);
  } else if (mgx_list.find(mgx_filename) == mgx_list.end()) {
    add_issue(asset, "MGX definition is not listed by mgxlist.txt: " + mgx_filename);
  } else {
    asset.mgx_listed = true;
    if (!mgx_directory.error.empty()) {
      add_issue(asset, "MGX directory: " + mgx_directory.error);
    } else if (!has_asset_file(mgx_directory, mgx_filename)) {
      add_issue(asset, "mgxlist entry has no exact-case file: " + mgx_filename);
    } else {
      std::string contents;
      std::string file_error;
      if (!read_bounded_asset(mgx_directory, mgx_filename, kMaxXmlBytes,
                              contents, asset.mgx_size, file_error)) {
        add_issue(asset, "MGX " + file_error);
      } else {
        const auto parsed = parse_crypt_mgx_v1(key.name, contents);
        if (!parsed) {
          add_issue(asset, "MGX parse failed for " + mgx_filename + ": " + parsed.error);
        } else {
          asset.mgx = parsed.module;
          asset.mgx_file_valid = true;
        }
      }
    }
  }

  std::string file_issue;
  asset.gameplay_file_valid = validate_binary_asset(
      mgp_directory, paths.mgp_directory, key.gameplay, ".mgp",
      asset.gameplay_size, file_issue);
  if (!asset.gameplay_file_valid) add_issue(asset, "gameplay: " + file_issue);

  file_issue.clear();
  asset.visual_file_valid = validate_binary_asset(
      mvp_directory, paths.mvp_directory, key.visual, ".mvp",
      asset.visual_size, file_issue);
  if (!asset.visual_file_valid) add_issue(asset, "visual: " + file_issue);

  const std::string mvx_filename = key.name + ".mvx";
  if (!safe_ascii_basename(mvx_filename, ".mvx")) {
    add_issue(asset, "unsafe MVX module name: " + key.name);
  } else if (!mvx_directory.error.empty()) {
    add_issue(asset, "MVX directory: " + mvx_directory.error);
  } else if (!has_asset_file(mvx_directory, mvx_filename)) {
    add_issue(asset, "missing exact-case scene definition: " + mvx_filename);
  } else {
    std::string contents;
    std::string file_error;
    if (!read_bounded_asset(mvx_directory, mvx_filename, kMaxXmlBytes,
                            contents, asset.mvx_size, file_error)) {
      add_issue(asset, "MVX " + file_error);
    } else {
      const auto geometry = parse_crypt_mgx_v1(key.name, contents);
      if (!geometry) {
        add_issue(asset, "MVX parse failed for " + mvx_filename + ": " + geometry.error);
      } else {
        asset.mvx_file_valid = true;
        asset.mvx_module = geometry.module;
        std::string scene_error;
        MvxSceneRootV1 scene;
        if (!extract_mvx_scene_root(contents, scene, scene_error)) {
          add_issue(asset, "MVX scene root invalid for " + mvx_filename + ": " + scene_error);
        } else {
          asset.mvx_scene_root_valid = true;
          asset.scene_object_name = std::move(scene.object_name);
          asset.xrefmax = std::move(scene.xrefmax);
          asset.fog_color = std::move(scene.fog_color);
          asset.is_solid = std::move(scene.is_solid);
          asset.scale = std::move(scene.scale);
          asset.xrefobject = std::move(scene.xrefobject);
          asset.dae = std::move(scene.dae);
          if (!geometry.module.has_grid_geometry) {
            add_issue(asset, "MVX root has no complete module/grid geometry: " + mvx_filename);
          } else if (asset.mgx_file_valid) {
            // Keep source precision: these XML dimensions are loaded through
            // the same float conversion as the MGX Block and drive tile fit.
            if (geometry.module.block_width != asset.mgx.block_width ||
                geometry.module.block_height != asset.mgx.block_height ||
                geometry.module.source_unit_width != asset.mgx.source_unit_width ||
                geometry.module.source_unit_height != asset.mgx.source_unit_height) {
              add_issue(asset, "MVX/MGX geometry mismatch for " + key.name);
            }
          }
        }
      }
    }
  }

  asset.is_complete = asset.issues.empty() && asset.mgx_listed &&
                      asset.mgx_file_valid && asset.gameplay_file_valid &&
                      asset.visual_file_valid && asset.mvx_file_valid &&
                      asset.mvx_scene_root_valid;
  return asset;
}

}  // namespace

bool CryptModuleAssetKeyLessV1::operator()(
    const CryptModuleAssetKeyV1& lhs,
    const CryptModuleAssetKeyV1& rhs) const noexcept {
  return std::tie(lhs.name, lhs.gameplay, lhs.visual) <
         std::tie(rhs.name, rhs.gameplay, rhs.visual);
}

std::size_t CryptModuleCatalogueV1::unresolved_candidate_count() const noexcept {
  std::size_t count = 0;
  for (const auto& candidate : rule_candidates) {
    const auto found = assets_by_exact_key.find(candidate.key);
    if (found == assets_by_exact_key.end() || !found->second.is_complete) ++count;
  }
  return count;
}

CryptModuleCatalogueV1 build_crypt_module_catalogue_v1(
    const CryptRuleDocumentV1& rules, const CryptModuleAssetPathsV1& paths) {
  CryptModuleCatalogueV1 catalogue;
  std::set<std::string> mgx_list;
  mgx_list = read_mgx_list(paths, catalogue.mgx_list_entry_count,
                           catalogue.catalogue_issues);

  const auto mgx_directory = index_exact_directory(paths.mgx_directory);
  const auto mgp_directory = index_exact_directory(paths.mgp_directory);
  const auto mvp_directory = index_exact_directory(paths.mvp_directory);
  const auto mvx_directory = index_exact_directory(paths.mvx_directory);

  if (!mgx_directory.error.empty()) catalogue.catalogue_issues.push_back("MGX directory: " + mgx_directory.error);
  if (!mgp_directory.error.empty()) catalogue.catalogue_issues.push_back("MGP directory: " + mgp_directory.error);
  if (!mvp_directory.error.empty()) catalogue.catalogue_issues.push_back("MVP directory: " + mvp_directory.error);
  if (!mvx_directory.error.empty()) catalogue.catalogue_issues.push_back("MVX directory: " + mvx_directory.error);

  for (const auto& list : rules.lists) {
    for (std::size_t index = 0; index < list.entries.size(); ++index) {
      const auto& entry = list.entries[index];
      CryptRuleAssetCandidateV1 candidate;
      candidate.list_name = list.name;
      candidate.list_index = index;
      candidate.key = {entry.name, entry.gameplay, entry.visual};
      catalogue.rule_candidates.push_back(candidate);

      if (catalogue.assets_by_exact_key.find(candidate.key) == catalogue.assets_by_exact_key.end()) {
        auto asset = audit_asset(candidate.key, paths, mgx_list,
                                 mgx_directory, mgp_directory,
                                 mvp_directory, mvx_directory);
        catalogue.assets_by_exact_key.emplace(candidate.key, std::move(asset));
      }
    }
  }
  return catalogue;
}

CryptModuleCatalogueV1 build_crypt_module_catalogue_from_bytes_v1(
    const CryptRuleDocumentV1& rules,
    const CryptModuleAssetBytesV1& assets) {
  CryptModuleCatalogueV1 catalogue;
  const CryptModuleAssetPathsV1 asset_labels{
      "assets/original-cache/data/3d/modules/crypt/mgx",
      "assets/worlds",
      "assets/worlds",
      "assets/original-cache/data/3d/modules/crypt/mvx",
      "assets/original-cache/data/3d/modules/crypt/mgx/mgxlist.txt"};
  const auto mgx_list = parse_mgx_list_bytes(
      assets.mgx_list, catalogue.mgx_list_entry_count,
      catalogue.catalogue_issues);
  const auto mgx_directory = index_memory_directory(assets.mgx);
  const auto mgp_directory = index_memory_directory(assets.mgp);
  const auto mvp_directory = index_memory_directory(assets.mvp);
  const auto mvx_directory = index_memory_directory(assets.mvx);

  for (const auto& list : rules.lists) {
    for (std::size_t index = 0; index < list.entries.size(); ++index) {
      const auto& entry = list.entries[index];
      CryptRuleAssetCandidateV1 candidate;
      candidate.list_name = list.name;
      candidate.list_index = index;
      candidate.key = {entry.name, entry.gameplay, entry.visual};
      catalogue.rule_candidates.push_back(candidate);

      if (catalogue.assets_by_exact_key.find(candidate.key) ==
          catalogue.assets_by_exact_key.end()) {
        auto asset = audit_asset(candidate.key, asset_labels, mgx_list,
                                 mgx_directory, mgp_directory,
                                 mvp_directory, mvx_directory);
        catalogue.assets_by_exact_key.emplace(candidate.key, std::move(asset));
      }
    }
  }
  return catalogue;
}

}  // namespace dh2::random_level
