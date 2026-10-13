#include "level_savegame_player_load_v1.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <vector>

namespace dh2::level_savegame_player_load_v1 {
namespace {
constexpr std::size_t kMaxImageBytes = 16u * 1024u * 1024u;
constexpr std::uint32_t kMaxObjectRows = 4096u;
constexpr std::size_t kMaxStringBytes = 4096u;
constexpr std::size_t kPlayerPayloadBytes = 59u;
constexpr std::size_t kPlayerGroupInfoPayloadBytes = 65u;

struct Reader {
    const std::uint8_t* data{};
    std::size_t size{};
    std::size_t cursor{};

    bool take(std::size_t count, const std::uint8_t** out) {
        if (!out || cursor > size || count > size - cursor) return false;
        *out = data + cursor;
        cursor += count;
        return true;
    }
    bool u32(std::uint32_t* out) {
        const std::uint8_t* p{};
        if (!out || !take(4, &p)) return false;
        *out = std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
               (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
        return true;
    }
    bool u64(std::uint64_t* out) {
        const std::uint8_t* p{};
        if (!out || !take(8, &p)) return false;
        std::uint64_t value{};
        for (unsigned i = 0; i != 8; ++i)
            value |= std::uint64_t(p[i]) << (8u * i);
        *out = value;
        return true;
    }
};

struct SavedObject {
    std::string name;
    std::string key;
    std::int32_t type{};
    const std::uint8_t* payload{};
    std::size_t payload_size{};
};

bool read_string(Reader& in, std::string* output) {
    std::uint32_t length{};
    const std::uint8_t* bytes{};
    if (!output || !in.u32(&length) || !length ||
        length > kMaxStringBytes || !in.take(length, &bytes) ||
        bytes[length - 1] != 0 ||
        std::memchr(bytes, 0, length - 1) != nullptr) return false;
    output->assign(reinterpret_cast<const char*>(bytes), length - 1);
    return !output->empty();
}

bool parse_player_payload(const SavedObject& row, PlayerState* output) {
    if (!output || (row.payload_size != kPlayerPayloadBytes &&
                    row.payload_size != kPlayerGroupInfoPayloadBytes)) return false;
    Reader in{row.payload, row.payload_size, 0};
    const std::uint8_t* bytes{};
    std::uint32_t word{};
    PlayerState state{};
    if (!in.take(2, &bytes) || bytes[0] > 1 || bytes[1] > 1) return false;
    state.object_base_flag_80 = bytes[0];
    state.object_base_flag_8a = bytes[1];
    if (!in.u32(&word)) return false;
    state.game_object_word_270 = static_cast<std::int32_t>(word);
    if (!in.u32(&word)) return false;
    state.state_machine_state = static_cast<std::int32_t>(word);
    if (!in.take(12, &bytes)) return false;
    std::copy(bytes, bytes + 12, state.position.begin());
    if (!in.take(12, &bytes)) return false;
    std::copy(bytes, bytes + 12, state.rotation.begin());
    if (!in.take(1, &bytes) || bytes[0] > 1) return false;
    state.dead = bytes[0];
    if (!in.take(12, &bytes)) return false;
    std::copy(bytes, bytes + 12, state.vector_1306.begin());
    if (!in.take(12, &bytes)) return false;
    std::copy(bytes, bytes + 12, state.vector_1309.begin());
    if (row.payload_size == kPlayerGroupInfoPayloadBytes) {
        if (!in.u32(&word)) return false;
        state.has_group_info_tail = 1;
        state.group_word_36 = static_cast<std::int32_t>(word);
        if (!in.take(2, &bytes) || bytes[0] > 1 || bytes[1] > 1) return false;
        state.group_flag_40 = bytes[0];
        state.group_flag_41 = bytes[1];
    }
    if (in.cursor != in.size) return false;
    *output = state;
    return true;
}

bool parse_image(const std::uint8_t* image, std::size_t image_size,
                 std::vector<SavedObject>* objects, std::uint32_t* section_count,
                 std::string& error) {
    if (!image || !objects || !section_count || image_size < 8 ||
        image_size > kMaxImageBytes) {
        error = "LevelSavegame image is empty or exceeds the bounded reader";
        return false;
    }
    Reader in{image, image_size, 0};
    std::uint32_t count{};
    if (!in.u32(&count) || count != 2) {
        error = "LevelSavegame requires exactly the source INFO and OBJS sections";
        return false;
    }
    bool saw_info = false, saw_objects = false;
    std::vector<SavedObject> parsed_objects;
    for (std::uint32_t i = 0; i < count; ++i) {
        std::uint32_t section_bytes{};
        const std::uint8_t* tag{};
        if (!in.u32(&section_bytes) || !in.take(4, &tag) ||
            section_bytes > in.size - in.cursor) {
            error = "LevelSavegame section header or payload length is truncated";
            return false;
        }
        const std::string section_tag(reinterpret_cast<const char*>(tag), 4);
        const std::uint8_t* section_data{};
        if (!in.take(section_bytes, &section_data)) {
            error = "LevelSavegame section payload is truncated";
            return false;
        }
        if (section_tag == "INFO") {
            if (saw_info || section_bytes != 4) {
                error = "LevelSavegame INFO section is duplicate or not four bytes";
                return false;
            }
            saw_info = true;
            continue;
        }
        if (section_tag != "OBJS" || saw_objects) {
            error = "LevelSavegame contains an unknown or duplicate section";
            return false;
        }
        saw_objects = true;
        Reader row_reader{section_data, section_bytes, 0};
        std::uint32_t row_count{};
        if (!row_reader.u32(&row_count) || !row_count || row_count > kMaxObjectRows) {
            error = "LevelSavegame OBJS count is empty, truncated or over its bound";
            return false;
        }
        parsed_objects.reserve(row_count);
        for (std::uint32_t j = 0; j < row_count; ++j) {
            SavedObject row{};
            std::uint32_t type{};
            std::uint64_t payload_size{};
            const std::uint8_t* payload{};
            if (!read_string(row_reader, &row.name) ||
                !read_string(row_reader, &row.key) || !row_reader.u32(&type) ||
                !row_reader.u64(&payload_size) || payload_size > kMaxImageBytes ||
                payload_size > std::numeric_limits<std::size_t>::max() ||
                !row_reader.take(static_cast<std::size_t>(payload_size), &payload)) {
                error = "LevelSavegame OBJS record is malformed or exceeds its bound";
                return false;
            }
            row.type = static_cast<std::int32_t>(type);
            row.payload = payload;
            row.payload_size = static_cast<std::size_t>(payload_size);
            parsed_objects.push_back(std::move(row));
        }
        if (row_reader.cursor != row_reader.size) {
            error = "LevelSavegame OBJS section has trailing bytes";
            return false;
        }
    }
    if (!saw_info || !saw_objects || in.cursor != in.size) {
        error = "LevelSavegame sections are missing or trailing bytes remain";
        return false;
    }
    *objects = std::move(parsed_objects);
    *section_count = count;
    return true;
}

} // namespace

Status load_player(const std::uint8_t* image, std::size_t image_size,
                   manager::Owner& objects, const factory::Owner& characters,
                   const level_savegame_object_manager_v1::FactsServices* facts,
                   const CommitServices* commit, Result* output,
                   std::string& error) {
    error.clear();
    if (output) *output = {};
    if (!output) {
        error = "Player level-load requires a result destination";
        return Status::invalid_argument;
    }
    if (!facts || !facts->read_object_facts || !commit ||
        ((!commit->prepare_player || !commit->commit_player_atomically) &&
         (!commit->prepare_player_identity ||
          !commit->commit_player_identity_atomically))) {
        error = "canonical player load facts or atomic mutation provider unavailable";
        return Status::provider_unavailable;
    }

    std::vector<SavedObject> records;
    std::uint32_t sections{};
    if (!parse_image(image, image_size, &records, &sections, error))
        return Status::malformed_save;

    // Parse and bound every OBJS row, then select the unique Player row. The
    // current native slice intentionally applies only this canonical identity;
    // other records remain untouched until their own Deserialize providers
    // are connected.
    const SavedObject* player_saved = nullptr;
    for (const auto& record : records) {
        if (record.key != "PlayerCharacter_0") continue;
        if (player_saved) {
            error = "LevelSavegame contains duplicate PlayerCharacter_0 manager keys";
            return Status::unsupported_record;
        }
        player_saved = &record;
    }
    if (!player_saved) {
        error = "LevelSavegame has no PlayerCharacter_0 OBJS row";
        return Status::unsupported_record;
    }
    PlayerState staged{};
    if (!parse_player_payload(*player_saved, &staged)) {
        error = "Player Character payload is not the pinned 59-byte or 65-byte source layout";
        return Status::unsupported_record;
    }

    const auto& saved = *player_saved;
    const factory::Record* resolved_record = nullptr;
    manager::SourceHandle resolved_handle{};
    std::uintptr_t resolved_identity{};
    manager::Owner::Cursor cursor{};
    objects.reset(&cursor);
    for (;;) {
        manager::GameObject* live = nullptr;
        if (objects.next(&cursor, &live) != manager::Status::ok) {
            error = "canonical ObjectManager key walk failed";
            return Status::manager_failed;
        }
        if (!live) break;
        const auto* record = characters.find(live->source_handle);
        save::Object object{};
        object.identity = live->identity;
        if (facts->read_object_facts(facts->context, *live, record, &object, error)) {
            if (error.empty()) error = "ObjectManager name/type facts are unavailable";
            return Status::provider_failed;
        }
        if (object.identity != live->identity || object.is_character > 1 ||
            object.is_player > 1 || object.enabled > 1 ||
            object.is_local_player > 1 || object.source_flag_129 > 1 ||
            (record && (record->source_handle != live->source_handle ||
                        record->character.identity != live->identity ||
                        record->game_object.identity != live->identity ||
                        !record->object_registered || !record->character_listed))) {
            error = "ObjectManager key facts conflict with the canonical factory identity";
            return Status::identity_mismatch;
        }
        const bool canonical_direct_character = !record && object.is_character &&
            facts->is_canonical_character_identity &&
            facts->is_canonical_character_identity(facts->context, live->identity);
        if (object.is_character !=
                ((record || canonical_direct_character) ? 1 : 0) ||
            (!record && object.is_player &&
             (!facts->is_canonical_player_identity ||
              !facts->is_canonical_player_identity(facts->context, live->identity)))) {
            error = "ObjectManager Character facts have no matching canonical source owner";
            return Status::identity_mismatch;
        }
        if (object.manager_key != saved.key) continue;
        if (resolved_identity || !object.is_character || !object.is_player ||
            object.type_word != saved.type) {
            error = "saved PlayerCharacter_0 key does not resolve uniquely to the same player Character";
            return Status::identity_mismatch;
        }
        resolved_record = record;
        resolved_handle = live->source_handle;
        resolved_identity = live->identity;
    }
    if (!resolved_identity ||
        (resolved_record && characters.find(resolved_handle) != resolved_record) ||
        (!resolved_record && (!facts->is_canonical_player_identity ||
          !facts->is_canonical_player_identity(facts->context, resolved_identity)))) {
        error = "saved player key has no existing canonical ObjectManager Character owner";
        return Status::identity_mismatch;
    }
    if (staged.has_group_info_tail &&
        (resolved_record || !commit->commit_player_identity_atomically)) {
        error = "saved Player GroupInfo tail has no canonical GroupInfo owner provider";
        return Status::provider_unavailable;
    }

    if (resolved_record) {
        if (!commit->prepare_player || !commit->commit_player_atomically) {
            error = "factory Character load commit providers are unavailable";
            return Status::provider_unavailable;
        }
        if (commit->prepare_player(commit->context, *resolved_record, staged, error)) {
            if (error.empty()) error = "canonical player load destination preflight failed";
            return Status::provider_failed;
        }
    } else {
        if (!commit->prepare_player_identity ||
            !commit->commit_player_identity_atomically) {
            error = "retained Player identity load commit providers are unavailable";
            return Status::provider_unavailable;
        }
        if (commit->prepare_player_identity(commit->context, resolved_identity,
                                            staged, error)) {
            if (error.empty()) error = "retained Player load destination preflight failed";
            return Status::provider_failed;
        }
    }
    // Recheck map/factory identity after the provider preflight. The runtime is
    // single-threaded at this owner seam; a changed identity fails before commit.
    auto* current = objects.find_by_source_handle(resolved_handle);
    if (!current || current->identity != resolved_identity ||
        (resolved_record && characters.find(resolved_handle) != resolved_record) ||
        (!resolved_record && (!facts->is_canonical_player_identity ||
          !facts->is_canonical_player_identity(facts->context, resolved_identity)))) {
        error = "canonical player identity changed during load preparation";
        return Status::identity_mismatch;
    }
    const bool commit_failed = resolved_record
        ? commit->commit_player_atomically(commit->context, *resolved_record,
                                           staged, error)
        : commit->commit_player_identity_atomically(commit->context,
                                                    resolved_identity, staged, error);
    if (commit_failed) {
        if (error.empty()) error = "canonical player atomic load commit failed";
        return Status::commit_failed;
    }
    output->sections = sections;
    output->object_rows = static_cast<std::uint32_t>(records.size());
    output->player_rows = 1;
    output->unapplied_object_rows = static_cast<std::uint32_t>(records.size() - 1);
    output->source_handle = resolved_handle;
    output->identity = resolved_identity;
    output->payload_bytes = static_cast<std::uint32_t>(saved.payload_size);
    error.clear();
    return Status::loaded_player_only;
}

Status restore_factory_character_rows(
    const std::uint8_t* image, std::size_t image_size, manager::Owner& objects,
    const factory::Owner& characters,
    const level_savegame_object_manager_v1::FactsServices* facts,
    const CommitServices* commit, FactoryLifecycleResult* output,
    std::string& error) {
    error.clear();
    if (!output) {
        error = "Factory Character lifecycle restore requires a result destination";
        return Status::invalid_argument;
    }
    *output = {};
    if (!facts || !facts->read_object_facts || !commit ||
        !commit->supports_factory_character_restore ||
        !commit->prepare_factory_character_restore ||
        !commit->commit_factory_character_restore) {
        error = "canonical factory lifecycle restore providers are unavailable";
        return Status::provider_unavailable;
    }

    std::vector<SavedObject> rows;
    std::uint32_t sections{};
    if (!parse_image(image, image_size, &rows, &sections, error))
        return Status::malformed_save;
    (void)sections;

    struct Pending {
        std::string key;
        std::int32_t type{};
        FactoryDeserializeState state{};
        const factory::Record* record{};
        manager::SourceHandle handle{};
        std::uintptr_t identity{};
    };
    std::vector<Pending> candidates;
    for (const auto& row : rows) {
        if (row.key == "PlayerCharacter_0") continue;
        // Character::Serialize writes GameObject(6), FSM(4), two 900-byte
        // CharacterProperties images, two vectors, dead, and two vectors;
        // GroupInfo contributes the optional six-byte suffix.
        constexpr std::size_t kNonplayerBase = 6 + 4 + 2 * 900 + 4 * 12 + 1;
        if (row.payload_size != kNonplayerBase &&
            row.payload_size != kNonplayerBase + 6) continue;
        ++output->rows_examined;
        const auto* p = row.payload;
        const auto read_word = [](const std::uint8_t* bytes) {
            return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
                   (std::uint32_t(bytes[2]) << 16) |
                   (std::uint32_t(bytes[3]) << 24);
        };
        if (p[0] > 1 || p[1] > 1 || p[1834] > 1 ||
            (row.payload_size == kNonplayerBase + 6 &&
             (p[1863] > 1 || p[1864] > 1))) continue;
        if (std::any_of(candidates.begin(), candidates.end(),
                [&](const Pending& prior) { return prior.key == row.key; })) {
            error = "LevelSavegame contains duplicate candidate Character manager keys";
            return Status::unsupported_record;
        }
        FactoryDeserializeState state{};
        state.object_base_flag_80=p[0];
        state.object_base_flag_8a=p[1];
        state.game_object_word_270=static_cast<std::int32_t>(read_word(p+2));
        state.state_machine_state=static_cast<std::int32_t>(read_word(p+6));
        std::copy_n(p+10,4,state.properties.saved_header.begin());
        std::copy_n(p+910,4,state.properties.resolved_header.begin());
        for (std::size_t i = 0; i < state.properties.saved.size(); ++i) {
            const auto saved_word = read_word(p + 14 + i * 4);
            const auto resolved_word = read_word(p + 914 + i * 4);
            std::memcpy(&state.properties.saved[i], &saved_word, sizeof(saved_word));
            std::memcpy(&state.properties.resolved[i], &resolved_word, sizeof(resolved_word));
        }
        std::copy_n(p+1810,12,state.position.begin());
        std::copy_n(p+1822,12,state.rotation.begin());
        state.dead=p[1834];
        std::copy_n(p+1835,12,state.vector_1468.begin());
        std::copy_n(p+1847,12,state.vector_1474.begin());
        if(row.payload_size==kNonplayerBase+6){
            state.has_group_info_tail=1;
            state.group_word_36=static_cast<std::int32_t>(read_word(p+1859));
            state.group_flag_40=p[1863];state.group_flag_41=p[1864];
        }
        candidates.push_back({row.key,row.type,state,nullptr,0,0});
    }

    std::vector<Pending*> resolved;
    manager::Owner::Cursor cursor{};
    objects.reset(&cursor);
    for (;;) {
        manager::GameObject* live = nullptr;
        if (objects.next(&cursor, &live) != manager::Status::ok) {
            error = "canonical ObjectManager key walk failed during lifecycle restore";
            return Status::manager_failed;
        }
        if (!live) break;
        const auto* record = characters.find(live->source_handle);
        save::Object object{};
        object.identity = live->identity;
        if (facts->read_object_facts(facts->context, *live, record,
                                      &object, error)) {
            if (error.empty()) error = "ObjectManager lifecycle facts are unavailable";
            return Status::provider_failed;
        }
        for (auto& pending : candidates) {
            if (object.manager_key != pending.key) continue;
            // LevelSavegame::__LoadObjects 0x461e90 resolves every row with
            // ObjectManager::GetObjectByName(key, type) 0x34aca0 and, when no
            // object answers, seeks past the row. A same-key object that is not a
            // factory-created non-Player Character (record-less NPC/Actor
            // Characters, a non-Character, the Player) or whose type word differs
            // is therefore an unresolved row, not a corrupt image: it stays
            // unapplied and the remaining rows still restore.
            if (!record || object.is_character != 1 || object.is_player != 0 ||
                object.type_word != pending.type) continue;
            if (object.identity != live->identity ||
                record->source_handle != live->source_handle ||
                record->character.identity != live->identity ||
                record->game_object.identity != live->identity ||
                !record->object_registered || !record->character_listed ||
                characters.find(live->source_handle) != record) {
                error = "saved non-player Character row does not resolve to one canonical factory identity | key " +
                        pending.key;
                return Status::identity_mismatch;
            }
            if (pending.identity) {
                error = "saved non-player Character key resolves more than once";
                return Status::identity_mismatch;
            }
            pending.record = record;
            pending.handle = live->source_handle;
            pending.identity = live->identity;
            resolved.push_back(&pending);
        }
    }

    std::vector<Pending*> supported;
    for (auto* pending : resolved) {
        bool applies = false;
        if (commit->supports_factory_character_restore(commit->context,
                *pending->record, pending->state, &applies, error)) {
            if (error.empty()) error = "factory Character restore admission failed";
            return Status::provider_failed;
        }
        if (!applies) continue;
        if (commit->prepare_factory_character_restore(commit->context,
                *pending->record, pending->state,
                error)) {
            if (error.empty()) error = "factory Character restore preflight failed";
            return Status::provider_failed;
        }
        supported.push_back(pending);
    }

    for (auto* pending : supported) {
        auto* live = objects.find_by_source_handle(pending->handle);
        if (!live || live->identity != pending->identity ||
            characters.find(pending->handle) != pending->record ||
            pending->record->character.identity != pending->identity) {
            error = "canonical factory Character changed during lifecycle restore";
            return Status::identity_mismatch;
        }
        if (commit->commit_factory_character_restore(commit->context,
                *pending->record, pending->state, error)) {
            if (error.empty()) error = "factory Character source restore failed";
            return Status::commit_failed;
        }
        ++output->rows_fully_applied;
    }
    output->rows_still_unapplied=static_cast<std::uint32_t>(std::count_if(
        rows.begin(),rows.end(),[&](const SavedObject& row){
          if(row.key=="PlayerCharacter_0")return false;
          return std::none_of(supported.begin(),supported.end(),
              [&](const Pending* p){return p->key==row.key;});
        }));
    error.clear();
    return Status::loaded_player_only;
}

Status restore_source_noncharacter_rows(
    const std::uint8_t* image, std::size_t image_size, manager::Owner& objects,
    const factory::Owner& characters,
    const level_savegame_object_manager_v1::FactsServices* facts,
    const CommitServices* commit, FactoryLifecycleResult* output,
    std::string& error) {
    error.clear();
    if (!output) {
        error = "Source non-Character restore requires a result destination";
        return Status::invalid_argument;
    }
    *output = {};
    if (!facts || !facts->read_object_facts || !commit ||
        !commit->classify_source_noncharacter_restore ||
        !commit->prepare_source_noncharacter_restore ||
        !commit->commit_source_noncharacter_restore) {
        error = "source non-Character Deserialize providers are unavailable";
        return Status::provider_unavailable;
    }
    std::vector<SavedObject> rows;
    std::uint32_t sections{};
    if (!parse_image(image, image_size, &rows, &sections, error))
        return Status::malformed_save;
    (void)sections;

    struct Pending {
        const SavedObject* saved{};
        manager::SourceHandle handle{};
        std::uintptr_t identity{};
        SourceNoncharacterDeserializeState state{};
    };
    std::vector<Pending> supported;
    auto read_word = [](const std::uint8_t* bytes) {
        return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
               (std::uint32_t(bytes[2]) << 16) |
               (std::uint32_t(bytes[3]) << 24);
    };
    manager::Owner::Cursor cursor{};
    objects.reset(&cursor);
    for (;;) {
        manager::GameObject* live = nullptr;
        if (objects.next(&cursor, &live) != manager::Status::ok) {
            error = "canonical ObjectManager key walk failed during source Deserialize";
            return Status::manager_failed;
        }
        if (!live) break;
        const auto* record = characters.find(live->source_handle);
        save::Object object{};
        object.identity = live->identity;
        if (facts->read_object_facts(facts->context, *live, record,
                                      &object, error)) {
            if (error.empty()) error = "ObjectManager source Deserialize facts are unavailable";
            return Status::provider_failed;
        }
        if (object.identity != live->identity || object.is_character > 1 ||
            (record && (record->source_handle != live->source_handle ||
                        record->game_object.identity != live->identity))) {
            error = "source Deserialize facts conflict with the canonical ObjectManager row";
            return Status::identity_mismatch;
        }
        if (object.is_character) continue;
        if (object.type_word != live->module_id_64) {
            error = "source non-Character type fact conflicts with its manager row";
            return Status::identity_mismatch;
        }
        const SavedObject* saved = nullptr;
        for (const auto& row : rows) {
            if (row.key != object.manager_key) continue;
            if (saved) {
                error = "LevelSavegame contains duplicate non-Character manager keys";
                return Status::unsupported_record;
            }
            saved = &row;
        }
        if (!saved) continue;
        ++output->rows_examined;
        if (saved->type != object.type_word) {
            error = "saved non-Character type word disagrees with the same manager row";
            return Status::identity_mismatch;
        }
        SourceNoncharacterKind kind{};
        bool applies = false;
        if (commit->classify_source_noncharacter_restore(commit->context, *live,
                object, &kind, &applies, error)) {
            if (error.empty()) error = "source non-Character Deserialize admission failed";
            return Status::provider_failed;
        }
        if (!applies) continue;
        if (kind != SourceNoncharacterKind::game_object &&
            kind != SourceNoncharacterKind::door &&
            kind != SourceNoncharacterKind::trigger &&
            kind != SourceNoncharacterKind::container) {
            error = "source non-Character provider returned an unknown Deserialize kind";
            return Status::identity_mismatch;
        }
        const std::size_t expected = kind == SourceNoncharacterKind::door ? 10 :
            kind == SourceNoncharacterKind::trigger ? 14 :
            kind == SourceNoncharacterKind::container ? 7 : 6;
        if (saved->payload_size != expected ||
            (kind != SourceNoncharacterKind::container &&
             (saved->payload[0] > 1 || saved->payload[1] > 1))) {
            error = "source non-Character payload does not match its exact Deserialize layout";
            return Status::unsupported_record;
        }
        Pending pending{};
        pending.saved = saved;
        pending.handle = live->source_handle;
        pending.identity = live->identity;
        pending.state.kind = kind;
        pending.state.game_object.object_base_flag_80 = saved->payload[0];
        pending.state.game_object.object_base_flag_8a = saved->payload[1];
        pending.state.game_object.game_object_word_270 =
            static_cast<std::int32_t>(read_word(saved->payload + 2));
        if (kind == SourceNoncharacterKind::door) {
            pending.state.door_state_3a8 =
                static_cast<std::int32_t>(read_word(saved->payload + 6));
            if (pending.state.door_state_3a8 < 0 ||
                pending.state.door_state_3a8 > 3) {
                error = "saved DoorStates value is outside the source enum range";
                return Status::unsupported_record;
            }
        } else if (kind == SourceNoncharacterKind::trigger) {
            pending.state.trigger.game_object = pending.state.game_object;
            pending.state.trigger.pending_3b8 =
                static_cast<std::int32_t>(read_word(saved->payload + 6));
            pending.state.trigger.max_count_3b4 =
                static_cast<std::int32_t>(read_word(saved->payload + 10));
        } else if (kind == SourceNoncharacterKind::container) {
            pending.state.container.game_object = pending.state.game_object;
            pending.state.container.state_394 = saved->payload[6];
        }
        if (commit->prepare_source_noncharacter_restore(commit->context, *live,
                pending.state, error)) {
            if (error.empty()) error = "source non-Character Deserialize preflight failed";
            return Status::provider_failed;
        }
        supported.push_back(pending);
    }

    for (const auto& pending : supported) {
        auto* live = objects.find_by_source_handle(pending.handle);
        if (!live || live->identity != pending.identity ||
            live->module_id_64 != pending.saved->type ||
            characters.find(pending.handle)) {
            error = "canonical non-Character row changed during Deserialize preparation";
            return Status::identity_mismatch;
        }
        if (commit->commit_source_noncharacter_restore(commit->context, *live,
                pending.state, error)) {
            if (error.empty()) error = "source non-Character Deserialize commit failed";
            return Status::commit_failed;
        }
        ++output->rows_fully_applied;
    }
    output->rows_still_unapplied = static_cast<std::uint32_t>(
        output->rows_examined - output->rows_fully_applied);
    error.clear();
    return Status::loaded_player_only;
}

} // namespace dh2::level_savegame_player_load_v1
