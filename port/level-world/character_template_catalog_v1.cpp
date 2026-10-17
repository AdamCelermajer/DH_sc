#include "character_template_catalog_v1.hpp"

#include "../game-data/data.hpp"

#include <algorithm>
#include <cstring>
#include <iterator>
#include <new>
#include <utility>
#include <vector>

namespace dh2::character::template_catalog_v1 {
namespace {

constexpr std::size_t kMaxInputBytes = 16U * 1024U * 1024U;
constexpr std::uint32_t kMaxRecords = 4096;
constexpr std::uint32_t kMaxNameBytes = 255;

bool read_u32(const std::uint8_t* data, std::size_t size,
              std::size_t& offset, std::uint32_t& value) {
  if (offset > size || size - offset < 4) return false;
  value = static_cast<std::uint32_t>(data[offset]) |
      (static_cast<std::uint32_t>(data[offset + 1]) << 8) |
      (static_cast<std::uint32_t>(data[offset + 2]) << 16) |
      (static_cast<std::uint32_t>(data[offset + 3]) << 24);
  offset += 4;
  return true;
}

bool parse_name_file(const std::uint8_t* bytes, std::size_t size,
                     std::vector<std::string>& output) {
  if (!bytes || !size || size > kMaxInputBytes) return false;
  std::size_t offset = 0;
  std::uint32_t count = 0;
  if (!read_u32(bytes, size, offset, count) || count > kMaxRecords ||
      count > (size - offset) / 4)
    return false;
  std::vector<std::string> parsed;
  parsed.reserve(count);
  for (std::uint32_t i = 0; i < count; ++i) {
    std::uint32_t length = 0;
    if (!read_u32(bytes, size, offset, length) || !length ||
        length > kMaxNameBytes || length > size - offset ||
        std::memchr(bytes + offset, 0, length))
      return false;
    parsed.emplace_back(reinterpret_cast<const char*>(bytes + offset), length);
    offset += length;
  }
  if (offset != size) return false;
  output = std::move(parsed);
  return true;
}

void fail(std::string& error, const char* message) { error = message; }

}  // namespace

bool load(const std::uint8_t* records, std::size_t records_size,
          const std::uint8_t* names, std::size_t names_size,
          const std::uint8_t* class_names, std::size_t class_names_size,
          const data::CharacterTable& characters,
          template_factory::Catalog& output, std::string& error) noexcept {
  output = {};
  error.clear();
  if (!records || !records_size || records_size > kMaxInputBytes ||
      !names || !names_size || names_size > kMaxInputBytes) {
    fail(error, "Character template array inputs are absent or oversized");
    return false;
  }
  if (characters.names.empty() || characters.rows.size() != characters.names.size() ||
      characters.fields.empty() || characters.fields.size() > 224 ||
      !class_names || !class_names_size ||
      class_names_size > kMaxInputBytes) {
    fail(error, "CharacterTable or ClassTable inputs are incomplete");
    return false;
  }

  try {
    std::vector<std::string> template_names;
    if (!parse_name_file(names, names_size, template_names)) {
      fail(error, "Character template names sidecar is malformed");
      return false;
    }
    std::vector<std::string> parsed_class_names;
    if (!parse_name_file(class_names, class_names_size, parsed_class_names) ||
        parsed_class_names.empty()) {
      fail(error, "ClassTable names sidecar is malformed");
      return false;
    }
    const auto class_field = std::find(characters.fields.begin(),
        characters.fields.end(), "ClassID");
    if (class_field == characters.fields.end() ||
        std::find(std::next(class_field), characters.fields.end(), "ClassID") !=
            characters.fields.end()) {
      fail(error, "CharacterTable ClassID field is absent or ambiguous");
      return false;
    }
    const auto class_index = static_cast<std::size_t>(
        class_field - characters.fields.begin());
    if (class_index >= 224) {
      fail(error, "CharacterTable ClassID field exceeds the source row width");
      return false;
    }
    template_factory::Catalog parsed;
    parsed.templates.reserve(template_names.size());
    parsed.characters.reserve(characters.names.size());
    parsed.class_names = std::move(parsed_class_names);
    for (std::size_t index = 0; index < characters.names.size(); ++index) {
      const auto class_id = characters.rows[index][class_index];
      if (class_id < -1 || (class_id >= 0 &&
          static_cast<std::size_t>(class_id) >= parsed.class_names.size())) {
        fail(error, "CharacterTable ClassID is outside the loaded ClassTable");
        return false;
      }
      parsed.characters.push_back({characters.names[index], class_id});
    }

    std::size_t offset = 0;
    std::uint32_t count = 0;
    if (!read_u32(records, records_size, offset, count) ||
        count != template_names.size() || count > kMaxRecords) {
      fail(error, "Character template array/name counts differ");
      return false;
    }
    for (const auto& name : template_names) {
      if (name.empty()) {
        fail(error, "Character template name is empty");
        return false;
      }
      std::uint32_t alternative_count = 0;
      if (!read_u32(records, records_size, offset, alternative_count) ||
          alternative_count > kMaxRecords ||
          alternative_count > (records_size - offset) / 4) {
        fail(error, "Character template alternative count is malformed");
        return false;
      }
      template_factory::TemplateRecord record;
      record.name = name;
      record.property_ids.reserve(alternative_count);
      for (std::uint32_t slot = 0; slot < alternative_count; ++slot) {
        std::uint32_t raw = 0;
        if (!read_u32(records, records_size, offset, raw)) {
          fail(error, "Character template alternative is truncated");
          return false;
        }
        std::int32_t property_id = 0;
        std::memcpy(&property_id, &raw, sizeof(property_id));
        if (property_id < 0 || property_id > 0x7fff ||
            static_cast<std::size_t>(property_id) >= parsed.characters.size()) {
          fail(error, "Character template property ID is outside CharacterTable");
          return false;
        }
        record.property_ids.push_back(property_id);
      }
      parsed.templates.push_back(std::move(record));
    }
    if (offset != records_size) {
      fail(error, "Character template array has trailing bytes");
      return false;
    }
    output = std::move(parsed);
    return true;
  } catch (const std::bad_alloc&) {
    output = {};
    fail(error, "Character template catalog allocation failed");
    return false;
  } catch (...) {
    output = {};
    fail(error, "Character template catalog loading failed unexpectedly");
    return false;
  }
}

}  // namespace dh2::character::template_catalog_v1
