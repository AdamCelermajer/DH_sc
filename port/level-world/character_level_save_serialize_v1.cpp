#include "character_level_save_serialize_v1.hpp"

#include <limits>

namespace dh2::character_level_save_serialize_v1 {
namespace {
// ARM32 writeAs<Structs::CharacterProperties> at 0x3a3ad0 writes 900
// bytes per image, including its first word and 224 property words.
constexpr std::size_t kPropertyBytes = 900;
constexpr std::size_t kVectorBytes = 12;

void* canonical_component(const character_runtime_factory_v1::Record& record,
                         character_constructor_owner_v1::Component component) {
    const auto index = static_cast<std::size_t>(component);
    if (index >= record.components.slots.size()) return nullptr;
    const auto& slot = record.components.slots[index];
    return slot.constructed ? slot.canonical_owner : nullptr;
}

bool valid_span(ByteSpan span, std::size_t expected) {
    return span.data && span.size == expected;
}
void append_u32(std::vector<std::uint8_t>& output, std::uint32_t value) {
    for (unsigned i = 0; i != 4; ++i)
        output.push_back(static_cast<std::uint8_t>(value >> (i * 8u)));
}
void append_span(std::vector<std::uint8_t>& output, ByteSpan span) {
    output.insert(output.end(), span.data, span.data + span.size);
}
}

Status serialize(std::uintptr_t character, const Services* services,
                 std::vector<std::uint8_t>& output, Result* result,
                 std::string& error) {
    if (!character || !services || !result) {
        error = "Character::Serialize requires canonical Character, services and result";
        return Status::invalid_argument;
    }
    if (!services->read_view || !services->read_game_object_fields) {
        error = "Character::Serialize source field or GameObject virtual provider unavailable";
        return Status::provider_unavailable;
    }
    try {
        View view{};
        if (services->read_view(services->context, character, &view, error)) {
            if (error.empty()) error = "canonical Character field provider failed";
            return Status::provider_failed;
        }
        if (view.is_player > 1 || view.dead > 1 || view.has_ai > 1 ||
            view.ai_flag_40 > 1 || view.ai_flag_41 > 1 ||
            !valid_span(view.position, kVectorBytes) ||
            !valid_span(view.rotation, kVectorBytes) ||
            !valid_span(view.vector_1306, kVectorBytes) ||
            !valid_span(view.vector_1309, kVectorBytes) ||
            (!view.is_player &&
             (!valid_span(view.properties_primary, kPropertyBytes) ||
              !valid_span(view.properties_secondary, kPropertyBytes))) ||
            (view.is_player &&
             ((view.properties_primary.size && !view.properties_primary.data) ||
              (view.properties_secondary.size && !view.properties_secondary.data)))) {
            error = "Character::Serialize fields do not match pinned source spans";
            return Status::invalid_view;
        }

        level_savegame_owner_v1::GameObjectFields game_object_fields{};
        if (services->read_game_object_fields(services->context, character,
                                               &game_object_fields, error)) {
            if (error.empty()) error = "canonical GameObject field provider failed";
            return Status::provider_failed;
        }
        std::vector<std::uint8_t> game_object;
        if (!level_savegame_owner_v1::serialize_game_object(
                &game_object_fields, game_object, error)) {
            return Status::invalid_view;
        }
        constexpr std::size_t fixed_tail = 4 + 4 * kVectorBytes + 1;
        const std::size_t properties = view.is_player ? 0 : 2 * kPropertyBytes;
        const std::size_t ai_tail = view.has_ai ? 6 : 0;
        if (properties > UINT32_MAX - game_object.size() - fixed_tail - ai_tail) {
            error = "Character::Serialize output exceeds source stream size";
            return Status::invalid_view;
        }

        std::vector<std::uint8_t> bytes;
        bytes.reserve(game_object.size() + fixed_tail + properties + ai_tail);
        bytes.insert(bytes.end(), game_object.begin(), game_object.end());
        append_u32(bytes, view.state_machine_state);
        if (!view.is_player) {
            append_span(bytes, view.properties_primary);
            append_span(bytes, view.properties_secondary);
        }
        append_span(bytes, view.position);
        append_span(bytes, view.rotation);
        bytes.push_back(view.dead);
        append_span(bytes, view.vector_1306);
        append_span(bytes, view.vector_1309);
        if (view.has_ai) {
            append_u32(bytes, static_cast<std::uint32_t>(view.ai_word_36));
            bytes.push_back(view.ai_flag_40);
            bytes.push_back(view.ai_flag_41);
        }
        Result next{};
        next.game_object_bytes = static_cast<std::uint32_t>(game_object.size());
        next.character_bytes = static_cast<std::uint32_t>(bytes.size());
        next.property_blocks = view.is_player ? 0 : 2;
        next.ai_tail_bytes = static_cast<std::uint32_t>(ai_tail);
        output.swap(bytes);
        *result = next;
        error.clear();
        return Status::complete;
    } catch (...) {
        error = "Character::Serialize provider or output allocation failed";
        return Status::allocation_failed;
    }
}

Status serialize_record(const character_runtime_factory_v1::Record& record,
                        const CanonicalServices* canonical,
                        std::vector<std::uint8_t>& output, Result* result,
                        std::string& error) {
    if (!canonical || !canonical->read_view ||
        !canonical->read_game_object_fields || !record.character.identity ||
        record.character.identity != record.game_object.identity ||
        record.character.object != &record.aggro_object ||
        record.aggro_object.identity != record.game_object.identity ||
        !canonical_component(record, character_constructor_owner_v1::Component::game_object) ||
        !canonical_component(record, character_constructor_owner_v1::Component::properties) ||
        !canonical_component(record, character_constructor_owner_v1::Component::state_machine)) {
        error = "Character save requires one complete canonical factory record";
        return Status::provider_unavailable;
    }

    struct Bridge {
        const character_runtime_factory_v1::Record* record{};
        const CanonicalServices* services{};
    } bridge{&record, canonical};
    const Services services{
        &bridge,
        [](void* raw, std::uintptr_t character, View* view,
           std::string& callback_error) {
            auto& b = *static_cast<Bridge*>(raw);
            if (character != b.record->character.identity) {
                callback_error = "Character save identity differs from factory record";
                return true;
            }
            const bool failed = b.services->read_view(
                b.services->context, *b.record, view, callback_error);
            if (failed) return true;
            if (view->has_ai &&
                !canonical_component(*b.record,
                    character_constructor_owner_v1::Component::ai)) {
                callback_error = "Character save AI bytes lack the canonical AI component";
                return true;
            }
            return false;
        },
        [](void* raw, std::uintptr_t character,
           level_savegame_owner_v1::GameObjectFields* fields,
           std::string& callback_error) {
            auto& b = *static_cast<Bridge*>(raw);
            if (character != b.record->character.identity) {
                callback_error = "GameObject save identity differs from factory record";
                return true;
            }
            return b.services->read_game_object_fields(
                b.services->context, *b.record, fields, callback_error);
        }};
    return serialize(record.character.identity, &services, output, result, error);
}

} // namespace dh2::character_level_save_serialize_v1
