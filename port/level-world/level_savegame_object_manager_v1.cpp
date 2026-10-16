#include "level_savegame_object_manager_v1.hpp"

#include <limits>
#include <vector>

namespace dh2::level_savegame_object_manager_v1 {
namespace {
struct Row {
    manager::SourceHandle handle{};
    manager::Address identity{};
    level_savegame_owner_v1::Object save_facts{};
    const factory::Record* character{};
};
struct SerializeContext {
    manager::Owner* manager{};
    const factory::Owner* factory{};
    const FactsServices* facts{};
    const std::vector<Row>* rows{};
};
struct CharacterContext {
    const character_level_save_serialize_v1::CanonicalServices* services{};
    const factory::Record* record{};
    std::uint8_t expected_player{};
};

struct IdentityCharacterContext {
    const character_level_save_serialize_v1::Services* services{};
    std::uintptr_t identity{};
    std::uint8_t expected_player{};
};

bool read_identity_character_view(void* raw, std::uintptr_t identity,
                         character_level_save_serialize_v1::View* view,
                         std::string& error) {
    auto& context = *static_cast<IdentityCharacterContext*>(raw);
    if (identity != context.identity || !view ||
        context.services->read_view(context.services->context, identity, view, error)) {
        if (error.empty()) error = "identity Character save facts are unavailable";
        return true;
    }
    if (view->is_player != context.expected_player) {
        error = "Character::IsPlayer disagrees with source OBJS eligibility facts";
        return true;
    }
    return false;
}

bool read_identity_character_game_object(void* raw, std::uintptr_t identity,
                                level_savegame_owner_v1::GameObjectFields* fields,
                                std::string& error) {
    auto& context = *static_cast<IdentityCharacterContext*>(raw);
    if (identity != context.identity || !fields ||
        context.services->read_game_object_fields(context.services->context,
                                                   identity, fields, error)) {
        if (error.empty()) error = "identity GameObject save fields are unavailable";
        return true;
    }
    return false;
}

bool read_character_view(void* raw, const factory::Record& record,
                         character_level_save_serialize_v1::View* view,
                         std::string& error) {
    auto& context = *static_cast<CharacterContext*>(raw);
    if (&record != context.record || !view ||
        context.services->read_view(context.services->context, record, view, error)) {
        if (error.empty()) error = "canonical Character save facts are unavailable";
        return true;
    }
    if (view->is_player != context.expected_player) {
        error = "Character::IsPlayer disagrees with source OBJS eligibility facts";
        return true;
    }
    return false;
}

bool read_character_game_object(void* raw, const factory::Record& record,
                                level_savegame_owner_v1::GameObjectFields* fields,
                                std::string& error) {
    auto& context = *static_cast<CharacterContext*>(raw);
    if (&record != context.record || !fields ||
        context.services->read_game_object_fields(context.services->context,
                                                   record, fields, error)) {
        if (error.empty()) error = "canonical GameObject save fields are unavailable";
        return true;
    }
    return false;
}

bool serialize_row(void* raw, std::uintptr_t identity,
                    std::vector<std::uint8_t>& output, std::string& error) {
    auto& context = *static_cast<SerializeContext*>(raw);
    for (const auto& row : *context.rows) {
        if (row.identity != identity) continue;
        const auto* live = context.manager->find_by_source_handle(row.handle);
        if (!live || live->identity != identity) {
            error = "ObjectManager row changed during LevelSavegame serialization";
            return true;
        }
        if (row.save_facts.is_character) {
            const auto* record = context.factory->find(row.handle);
            character_level_save_serialize_v1::Result result{};
            character_level_save_serialize_v1::Status status{};
            if (record) {
                if (!row.character || record != row.character ||
                    record->character.identity != identity ||
                    !context.facts->character) {
                    error = "Character OBJS row lost its canonical factory/save providers";
                    return true;
                }
                CharacterContext character_context{context.facts->character, record,
                                                    row.save_facts.is_player};
                const character_level_save_serialize_v1::CanonicalServices character_services{
                    &character_context, &read_character_view, &read_character_game_object};
                status = character_level_save_serialize_v1::serialize_record(
                    *record, &character_services, output, &result, error);
            } else {
                const bool canonical_character =
                    context.facts->is_canonical_character_identity &&
                    context.facts->is_canonical_character_identity(
                        context.facts->context, identity);
                if (!row.save_facts.is_character || !canonical_character ||
                    (row.save_facts.is_player != row.save_facts.is_local_player) ||
                    !context.facts->character_identity ||
                    !context.facts->character_identity->read_view ||
                    !context.facts->character_identity->read_game_object_fields) {
                    error = "Record-less Character OBJS row has no canonical identity serializer";
                    return true;
                }
                IdentityCharacterContext character_context{
                    context.facts->character_identity, identity,
                    row.save_facts.is_player};
                const character_level_save_serialize_v1::Services character_services{
                    &character_context, &read_identity_character_view,
                    &read_identity_character_game_object};
                status = character_level_save_serialize_v1::serialize(
                    identity, &character_services, output, &result, error);
            }
            if (status != character_level_save_serialize_v1::Status::complete) {
                if (error.empty()) error = "source Character::Serialize provider failed";
                return true;
            }
            return false;
        }
        if (!context.facts->serialize_noncharacter) {
            error = "eligible non-Character OBJS row has no exact virtual Serialize provider";
            return true;
        }
        return context.facts->serialize_noncharacter(context.facts->context,
                                                       *live, output, error);
    }
    error = "LevelSavegame serializer received an identity outside its ObjectManager snapshot";
    return true;
}
}

Status save_all(level_savegame_owner_v1::Owner& save,
                manager::Owner& objects, const factory::Owner& characters,
                std::uint32_t online, const FactsServices* facts,
                const level_savegame_owner_v1::PublishServices* publisher,
                Result* output, std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output || online > 1 || !facts || !facts->read_object_facts ||
        !publisher || !publisher->publish) {
        error = "LevelSavegame ObjectManager bridge requires source facts, publisher, result, and online bool";
        return Status::invalid_state;
    }

    Result result{};
    std::vector<Row> rows;
    std::vector<level_savegame_owner_v1::Object> save_objects;
    manager::Owner::Cursor cursor{};
    objects.reset(&cursor);
    for (;;) {
        manager::GameObject* live = nullptr;
        if (objects.next(&cursor, &live) != manager::Status::ok) {
            error = "LevelSavegame ObjectManager snapshot encountered an invalid map row";
            return Status::manager_failed;
        }
        if (!live) break;
        if (!live->identity || rows.size() == UINT32_MAX) {
            error = "LevelSavegame ObjectManager row identity/count is invalid";
            return Status::manager_failed;
        }
        const auto* record = characters.find(live->source_handle);
        if (record && (record->source_handle != live->source_handle ||
                       record->game_object.identity != live->identity ||
                       record->character.identity != live->identity ||
                       !record->object_registered || !record->character_listed)) {
            error = "LevelSavegame ObjectManager Character row differs from canonical factory identity";
            return Status::identity_mismatch;
        }
        Row row{};
        row.handle = live->source_handle;
        row.identity = live->identity;
        row.character = record;
        row.save_facts.identity = live->identity;
        row.save_facts.manager_key.clear();
        // Returns true on failure, like serialize_noncharacter.
        if (facts->read_object_facts(facts->context, *live, record,
                                     &row.save_facts, error)) {
            if (error.empty()) error = "source ObjectManager save-facts provider failed | " + live->name;
            return Status::facts_failed;
        }
        const auto& object = row.save_facts;
        // GameObject::CheckSpawnProbability (0x38bd64) deletes a row whose spawn_prob roll
        // failed through ObjectBase::Delete + ObjectManager::MarkForDeletion (0x3432f8); the
        // manager destroys the marked object, so the Level save never sees it. Here the row
        // stays in the map with +0x81 raised; a deleted non-Character row has no OBJS entry.
        if (!record && !object.is_character) {
            const auto deleted = live->live_fields.deleted_81
                ? *live->live_fields.deleted_81 : live->deleted_81;
            if (deleted == 1) continue;
        }
        const bool canonical_character = !record &&
            facts->is_canonical_character_identity &&
            facts->is_canonical_character_identity(facts->context, live->identity);
        const bool canonical_player = !record && facts->is_canonical_player_identity &&
            facts->is_canonical_player_identity(facts->context, live->identity);
        if (object.identity != live->identity || object.is_character > 1 ||
            object.is_player > 1 || object.is_local_player > 1 || object.enabled > 1 ||
            object.source_flag_129 > 1 ||
            (record && !object.is_character)) {
            error = "ObjectManager source facts conflict with canonical factory/type identity";
            return Status::identity_mismatch;
        }
        if (!record && object.is_character &&
            (!canonical_character ||
             (object.is_player && (!canonical_player || !object.is_local_player)) ||
             (!object.is_player && object.is_local_player) ||
             !facts->character_identity ||
             !facts->character_identity->read_view ||
             !facts->character_identity->read_game_object_fields)) {
            error = "Record-less Character OBJS row is not a canonical Character owner";
            return Status::identity_mismatch;
        }
        if (!record && !object.is_character && canonical_player) {
            error = "canonical retained Player is misclassified as a non-Character ObjectManager row";
            return Status::identity_mismatch;
        }
        if (record && !facts->character) {
            error = "eligible Character row has no canonical source serializer services";
            return Status::serializer_unavailable;
        }
        try {
            save_objects.push_back(row.save_facts);
            rows.push_back(std::move(row));
        }
        catch (...) {
            error = "LevelSavegame ObjectManager snapshot allocation failed";
            return Status::manager_failed;
        }
    }
    std::uint32_t factory_character_rows = 0;
    for (const auto& row : rows) {
        if (row.save_facts.is_character) ++result.character_rows;
        if (row.character) ++factory_character_rows;
    }
    if (factory_character_rows != characters.size()) {
        error = "canonical Character factory contains rows absent from ObjectManager snapshot";
        return Status::identity_mismatch;
    }
    result.manager_rows = static_cast<std::uint32_t>(rows.size());

    SerializeContext context{&objects, &characters, facts, &rows};
    const level_savegame_owner_v1::SerializeServices serializers{
        &context, &serialize_row};
    const auto status = save.save_all(
        save_objects.empty() ? nullptr : save_objects.data(), save_objects.size(), online,
        &serializers, publisher, &result.save, error);
    if (status != level_savegame_owner_v1::SaveStatus::saved) {
        if (error.empty()) error = "canonical LevelSavegame owner did not publish";
        return status == level_savegame_owner_v1::SaveStatus::provider_unavailable
            ? Status::serializer_unavailable : Status::owner_failed;
    }
    *output = result;
    return Status::saved;
}
} // namespace dh2::level_savegame_object_manager_v1
