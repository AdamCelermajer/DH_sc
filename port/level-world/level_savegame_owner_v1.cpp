#include "level_savegame_owner_v1.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <utility>

namespace dh2::level_savegame_owner_v1 {
namespace {
void append_u32(std::vector<std::uint8_t>& out, std::uint32_t value) {
    for (unsigned i = 0; i != 4; ++i)
        out.push_back(static_cast<std::uint8_t>(value >> (8u * i)));
}
void append_u64(std::vector<std::uint8_t>& out, std::uint64_t value) {
    for (unsigned i = 0; i != 8; ++i)
        out.push_back(static_cast<std::uint8_t>(value >> (8u * i)));
}
bool append_string(std::vector<std::uint8_t>& out, const std::string& value) {
    if (value.find('\0') != std::string::npos || value.size() >= UINT32_MAX)
        return false;
    append_u32(out, static_cast<std::uint32_t>(value.size() + 1));
    out.insert(out.end(), value.begin(), value.end());
    out.push_back(0);
    return true;
}
bool valid_tag(const std::string& tag) {
    return tag.size() == 4 && std::all_of(tag.begin(), tag.end(), [](char c) {
        return c >= 'A' && c <= 'Z';
    });
}
std::uint32_t nonnegative(std::int32_t value) {
    return value < 0 ? 0u : static_cast<std::uint32_t>(value);
}
}

Status serialize_objects(const Object* objects, std::size_t count,
                         std::uint32_t online,
                         const SerializeServices* services,
                         std::vector<std::uint8_t>& output,
                         Result* result, std::string& error) {
    if ((!objects && count) || !result || count > UINT32_MAX) {
        error = "LevelSavegame OBJS requires a bounded ObjectManager walk";
        return Status::invalid_argument;
    }
    Result next{};
    next.source_objects = static_cast<std::uint32_t>(count);
    try {
        std::vector<std::uint8_t> bytes;
        append_u32(bytes, 0); // __SaveObjects patches the count after traversal.
        for (std::size_t i = 0; i < count; ++i) {
            const auto& object = objects[i];
            if (!object.identity || object.name.empty() || object.manager_key.empty() ||
                object.name.find('\0') != std::string::npos ||
                object.manager_key.find('\0') != std::string::npos) {
                error = "OBJS ObjectManager row lacks source identity or names";
                return Status::invalid_argument;
            }
            if (!object.enabled || (online && object.is_character && object.is_player &&
                (!object.is_local_player || object.source_flag_129))) continue;
            if (!services || !services->serialize) {
                error = "eligible OBJS object has no source virtual Serialize provider";
                return Status::provider_unavailable;
            }

            std::vector<std::uint8_t> payload;
            if (services->serialize(services->context, object.identity, payload, error)) {
                if (error.empty()) error = "source virtual Serialize provider failed";
                return Status::provider_failed;
            }
            if (bytes.size() > UINT32_MAX || payload.size() > UINT32_MAX - bytes.size()) {
                error = "OBJS stream exceeds source addressable size";
                return Status::size_overflow;
            }
            if (!append_string(bytes, object.name)) {
                error = "OBJS object name cannot be represented by source string writer";
                return Status::invalid_argument;
            }
            const bool alias_player = object.is_character && object.is_player &&
                                      object.manager_key != "PlayerCharacter_0";
            if (!append_string(bytes, alias_player ? std::string("PlayerCharacter_0")
                                                    : object.manager_key)) {
                error = "OBJS manager name cannot be represented by source string writer";
                return Status::invalid_argument;
            }
            append_u32(bytes, static_cast<std::uint32_t>(object.type_word));
            append_u64(bytes, static_cast<std::uint64_t>(payload.size()));
            bytes.insert(bytes.end(), payload.begin(), payload.end());
            ++next.saved_objects;
        }
        bytes[0] = static_cast<std::uint8_t>(next.saved_objects);
        bytes[1] = static_cast<std::uint8_t>(next.saved_objects >> 8);
        bytes[2] = static_cast<std::uint8_t>(next.saved_objects >> 16);
        bytes[3] = static_cast<std::uint8_t>(next.saved_objects >> 24);
        next.serialized_bytes = static_cast<std::uint32_t>(bytes.size());
        output.swap(bytes);
        *result = next;
        error.clear();
        return Status::complete;
    } catch (...) {
        error = "OBJS stream allocation or provider failed";
        return Status::allocation_failed;
    }
}

bool format_filename(std::uint32_t source_arg0, std::int32_t source_arg1,
                     std::int32_t source_arg2, std::int32_t source_arg3,
                     std::string& output, std::string& error) {
    char buffer[96]{};
    const int size = std::snprintf(buffer, sizeof(buffer),
        "dh2_%03u_%01u_%03u_%03u_level.savegame", source_arg0,
        nonnegative(source_arg3), nonnegative(source_arg1),
        nonnegative(source_arg2));
    if (size < 0 || static_cast<std::size_t>(size) >= sizeof(buffer)) {
        error = "LevelSavegame filename exceeds source local buffer";
        return false;
    }
    output.assign(buffer, static_cast<std::size_t>(size));
    error.clear();
    return true;
}

bool format_checkpoint_filename(std::int32_t source_save_slot,
                                std::int32_t source_difficulty,
                                std::string& output, std::string& error) {
    if (source_save_slot < 0 || source_difficulty < 0 || source_difficulty > 2) {
        error = "LevelSavegame checkpoint filename requires the active slot and difficulty";
        return false;
    }
    char buffer[64]{};
    const int size = std::snprintf(buffer, sizeof(buffer),
        "dh2_%03u_%01u_single_level.checkpoint",
        static_cast<unsigned>(source_save_slot),
        static_cast<unsigned>(source_difficulty));
    if (size < 0 || static_cast<std::size_t>(size) >= sizeof(buffer)) {
        error = "LevelSavegame checkpoint filename exceeds source buffer";
        return false;
    }
    output.assign(buffer, static_cast<std::size_t>(size));
    error.clear();
    return true;
}

bool serialize_game_object(const GameObjectFields* fields,
                           std::vector<std::uint8_t>& output,
                           std::string& error) {
    if (!fields || fields->object_base_flag_80 > 1 ||
        fields->object_base_flag_8a > 1) {
        error = "GameObject::Serialize requires the two source bool fields and +0x270 word";
        return false;
    }
    try {
        std::vector<std::uint8_t> bytes;
        bytes.reserve(6);
        bytes.push_back(fields->object_base_flag_80);
        bytes.push_back(fields->object_base_flag_8a);
        append_u32(bytes, static_cast<std::uint32_t>(fields->game_object_word_270));
        output.swap(bytes);
        error.clear();
        return true;
    } catch (...) {
        error = "GameObject::Serialize output allocation failed";
        return false;
    }
}

bool serialize_trigger(const TriggerFields* fields,
                       std::vector<std::uint8_t>& output,
                       std::string& error) {
    if (!fields) {
        error = "Trigger::Serialize requires its canonical source fields";
        return false;
    }
    try {
        std::vector<std::uint8_t> bytes;
        if (!serialize_game_object(&fields->game_object, bytes, error))
            return false;
        append_u32(bytes, static_cast<std::uint32_t>(fields->pending_3b8));
        append_u32(bytes, static_cast<std::uint32_t>(fields->max_count_3b4));
        output.swap(bytes);
        error.clear();
        return true;
    } catch (...) {
        error = "Trigger::Serialize output allocation failed";
        return false;
    }
}

bool serialize_container(const ContainerFields* fields,
                         std::vector<std::uint8_t>& output,
                         std::string& error) {
    if (!fields || fields->state_394 > 4) {
        error = "Container::Serialize requires a valid source +0x394 state";
        return false;
    }
    try {
        std::vector<std::uint8_t> bytes;
        if (!serialize_game_object(&fields->game_object, bytes, error))
            return false;
        bytes.push_back(fields->state_394);
        output.swap(bytes);
        error.clear();
        return true;
    } catch (...) {
        error = "Container::Serialize output allocation failed";
        return false;
    }
}

bool serialize_door(const DoorFields* fields,
                    std::vector<std::uint8_t>& output,
                    std::string& error) {
    if (!fields || fields->state_3a8 < 0 || fields->state_3a8 > 3) {
        error = "Door::Serialize requires a valid source +0x3a8 DoorStates value";
        return false;
    }
    try {
        std::vector<std::uint8_t> bytes;
        if (!serialize_game_object(&fields->game_object, bytes, error))
            return false;
        append_u32(bytes, static_cast<std::uint32_t>(fields->state_3a8));
        output.swap(bytes);
        error.clear();
        return true;
    } catch (...) {
        error = "Door::Serialize output allocation failed";
        return false;
    }
}

bool serialize_info_word(std::int32_t source_info_word,
                         std::vector<std::uint8_t>& output,
                         std::string& error) {
    try {
        std::vector<std::uint8_t> bytes;
        bytes.reserve(4);
        append_u32(bytes, static_cast<std::uint32_t>(source_info_word));
        output.swap(bytes);
        error.clear();
        return true;
    } catch (...) {
        error = "LevelSavegame INFO output allocation failed";
        return false;
    }
}

bool serialize_savegame(const Section* sections, std::size_t count,
                        std::vector<std::uint8_t>& output,
                        std::string& error) {
    if (!sections || count != 2) {
        error = "LevelSavegame Savegame requires its exact INFO and OBJS registrations";
        return false;
    }
    try {
        std::map<std::string, const Section*, std::less<>> ordered;
        for (std::size_t i = 0; i < count; ++i) {
            if (!valid_tag(sections[i].tag) ||
                sections[i].payload.size() > UINT32_MAX ||
                !ordered.emplace(sections[i].tag, &sections[i]).second) {
                error = "LevelSavegame sections require unique four-letter source tags";
                return false;
            }
        }
        const auto info = ordered.find("INFO");
        const auto objects = ordered.find("OBJS");
        if (ordered.size() != 2 || info == ordered.end() || objects == ordered.end() ||
            info->second->payload.size() != 4) {
            error = "LevelSavegame Savegame accepts only four-byte INFO and OBJS";
            return false;
        }
        std::vector<std::uint8_t> bytes;
        append_u32(bytes, static_cast<std::uint32_t>(ordered.size()));
        for (const auto& row : ordered) {
            append_u32(bytes, static_cast<std::uint32_t>(row.second->payload.size()));
            bytes.insert(bytes.end(), row.first.begin(), row.first.end());
            bytes.insert(bytes.end(), row.second->payload.begin(),
                         row.second->payload.end());
        }
        output.swap(bytes);
        error.clear();
        return true;
    } catch (...) {
        error = "LevelSavegame section assembly failed";
        return false;
    }
}

bool read_savegame_info_word(const std::uint8_t* image, std::size_t image_size,
                             std::int32_t& info_word, std::string& error) {
    const auto read_u32 = [image, image_size](std::size_t offset,
                                              std::uint32_t& value) {
        if (!image || offset > image_size || image_size - offset < 4) return false;
        value = static_cast<std::uint32_t>(image[offset]) |
            (static_cast<std::uint32_t>(image[offset + 1]) << 8) |
            (static_cast<std::uint32_t>(image[offset + 2]) << 16) |
            (static_cast<std::uint32_t>(image[offset + 3]) << 24);
        return true;
    };
    std::uint32_t section_count = 0;
    if (!image || image_size < 4 || !read_u32(0, section_count) ||
        section_count != 2) {
        error = "LevelSavegame Load requires its exact two-section source image";
        return false;
    }
    bool found_info = false, found_objects = false;
    std::int32_t next_info = 0;
    std::size_t cursor = 4;
    for (std::uint32_t i = 0; i < section_count; ++i) {
        std::uint32_t payload_size = 0;
        if (!read_u32(cursor, payload_size) || cursor > image_size - 4) {
            error = "LevelSavegame Load section header is truncated";
            return false;
        }
        cursor += 4;
        if (image_size - cursor < 4 || payload_size > image_size - cursor - 4) {
            error = "LevelSavegame Load section payload is truncated";
            return false;
        }
        const bool is_info = std::memcmp(image + cursor, "INFO", 4) == 0;
        const bool is_objects = std::memcmp(image + cursor, "OBJS", 4) == 0;
        cursor += 4;
        if (is_info) {
            if (found_info || payload_size != 4) {
                error = "LevelSavegame Load INFO registration is duplicate or malformed";
                return false;
            }
            std::uint32_t raw = 0;
            if (!read_u32(cursor, raw)) {
                error = "LevelSavegame Load INFO word is truncated";
                return false;
            }
            std::memcpy(&next_info, &raw, sizeof(next_info));
            found_info = true;
        } else if (is_objects) {
            if (found_objects) {
                error = "LevelSavegame Load OBJS registration is duplicated";
                return false;
            }
            found_objects = true;
        } else {
            error = "LevelSavegame Load image contains an unsupported section";
            return false;
        }
        cursor += payload_size;
    }
    if (cursor != image_size || !found_info || !found_objects) {
        error = "LevelSavegame Load image has trailing bytes or missing INFO/OBJS";
        return false;
    }
    info_word = next_info;
    error.clear();
    return true;
}

bool Owner::construct(std::string source_filename, std::int32_t info_word,
                      std::string& error) {
    if (source_filename.rfind("dh2_", 0) != 0 ||
        source_filename.size() < 19 ||
        source_filename.compare(source_filename.size() - 15, 15,
                                "_level.savegame") != 0 ||
        source_filename.find_first_of("/\\") != std::string::npos ||
        source_filename.find('\0') != std::string::npos) {
        error = "LevelSavegame owner requires a source GetFilename result";
        return false;
    }
    // A renderer/EGL projection may be rebound while this logical Level and
    // its Savegame remain alive. Keep that same source owner and image instead
    // of resetting it on an identity-preserving bind.
    if (constructed_ && filename_ == source_filename && info_word_ == info_word) {
        error.clear();
        return true;
    }
    filename_ = std::move(source_filename);
    info_word_ = info_word;
    image_.clear();
    gate_39_ = 0; // LevelSavegame C1 stores false at +0x39.
    constructed_ = true;
    error.clear();
    return true;
}

SaveStatus Owner::save_all(const Object* objects, std::size_t count,
                           std::uint32_t online,
                           const SerializeServices* serializers,
                           const PublishServices* publisher,
                           SaveResult* output, std::string& error) {
    if (!output) {
        error = "LevelSavegame Savegame::saveAll requires result";
        return SaveStatus::invalid_state;
    }
    SaveResult result{};
    *output = result;
    error.clear();
    if (!constructed_ || !publisher || !publisher->publish) {
        error = !constructed_ ? "LevelSavegame owner is not constructed"
                              : "LevelSavegame file publisher unavailable";
        return !constructed_ ? SaveStatus::invalid_state
                             : SaveStatus::provider_unavailable;
    }
    std::vector<std::uint8_t> object_payload, info_payload, candidate;
    Result object_result{};
    if (serialize_objects(objects, count, online, serializers,
                          object_payload, &object_result, error) != Status::complete)
        return SaveStatus::provider_failed;
    if (!serialize_info_word(info_word_, info_payload, error))
        return SaveStatus::provider_failed;
    const Section sections[] = {{"INFO", std::move(info_payload)},
                                {"OBJS", std::move(object_payload)}};
    if (!serialize_savegame(sections, 2, candidate, error))
        return SaveStatus::provider_failed;
    try {
        if (!publisher->publish(publisher->context, filename_, candidate, error)) {
            if (error.empty()) error = "LevelSavegame file publisher failed";
            return SaveStatus::provider_failed;
        }
    } catch (...) {
        error = "LevelSavegame file publisher threw";
        return SaveStatus::provider_failed;
    }
    image_.swap(candidate);
    result.object_rows = object_result.source_objects;
    result.saved_objects = object_result.saved_objects;
    result.bytes = static_cast<std::uint32_t>(image_.size());
    result.publishes = 1;
    *output = result;
    error.clear();
    return SaveStatus::saved;
}

bool Owner::load_image(const std::uint8_t* image, std::size_t image_size,
                       std::string& error) {
    if (!constructed_) {
        error = "LevelSavegame Load requires the retained constructed owner";
        return false;
    }
    std::int32_t next_info = 0;
    if (!read_savegame_info_word(image, image_size, next_info, error)) return false;
    try {
        std::vector<std::uint8_t> candidate(image, image + image_size);
        image_.swap(candidate);
        info_word_ = next_info;
        error.clear();
        return true;
    } catch (...) {
        error = "LevelSavegame Load could not retain the source image";
        return false;
    }
}

void Owner::destroy() noexcept {
    filename_.clear();
    image_.clear();
    info_word_ = 0;
    gate_39_ = 0;
    constructed_ = false;
}

bool Owner::set_gate_39(std::uint8_t value) noexcept {
    if (!constructed_ || value > 1) return false;
    gate_39_ = value;
    return true;
}

} // namespace dh2::level_savegame_owner_v1
