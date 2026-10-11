#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../player_savegame_v1.hpp"
#include "../properties.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
using namespace dh2::player_saved_skill_callbacks_v1;
namespace player_saved_skill_callbacks_v1 = dh2::player_saved_skill_callbacks_v1;
using Raw = std::vector<std::uint8_t>;
namespace {
unsigned checks = 0;
const char* stage = "startup";
void require(bool value) {
    ++checks;
    if (!value) throw std::runtime_error("PlayerSavegameV1 check " +
                                         std::to_string(checks) + " at " + stage);
}
Raw file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(bool(input));
    return Raw(std::istreambuf_iterator<char>(input), {});
}
void put(Raw& bytes, std::uint32_t value) {
    for (int i = 0; i < 4; ++i) bytes.push_back(std::uint8_t(value >> (8 * i)));
}
std::uint32_t read_word(const Raw& bytes, std::size_t& at) {
    require(at <= bytes.size() && bytes.size() - at >= 4);
    std::uint32_t value = 0;
    for (int i = 0; i < 4; ++i) value |= std::uint32_t(bytes[at++]) << (8 * i);
    return value;
}
Raw read_block(const Raw& bytes, std::size_t& at) {
    const auto count = read_word(bytes, at);
    require(count <= bytes.size() - at);
    Raw result(bytes.begin() + at, bytes.begin() + at + count);
    at += count;
    return result;
}
void append_snapshot(Raw& out, const PlayerSavegameV1& owner,
                     std::uint32_t update_calls) {
    put(out, static_cast<std::uint32_t>(owner.skills().size()));
    for (const auto& skill : owner.skills()) {
        put(out, static_cast<std::uint32_t>(skill.id));
        out.push_back(std::uint8_t(skill.level));
        out.push_back(std::uint8_t(skill.level >> 8));
        out.push_back(skill.flag);
        out.push_back(0);  // Source allocator padding is not source state.
    }
    for (const auto& slots : owner.skill_slots()) {
        put(out, static_cast<std::uint32_t>(slots.size()));
        for (const auto& entry : slots) {
            put(out, static_cast<std::uint32_t>(entry.first));
            put(out, entry.second);
        }
    }
    put(out, update_calls);
}
bool update_skills(void* context, std::uintptr_t character, std::string&) {
    require(character == UINT64_C(0x1234567800000099));
    ++*static_cast<std::uint32_t*>(context);
    return true;
}
struct GoldReader {
    const Raw& bytes;
    std::size_t at{};
};
}  // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 3);
        const auto gold = file(argv[1]);
        require(gold.size() >= 8 &&
                std::memcmp(gold.data(), "PGS1", 4) == 0);
        GoldReader reader{gold, 8};
        const auto cache = std::string(argv[2]);
        auto records = file(cache + "/skills_pyarray.bin");
        auto names = file(cache + "/skills_pyarraynames.bin");
        auto schema = file(cache + "/skills_pystructnames.bin");
        SkillTables tables;
        std::string error;
        require(load_skill_tables({records.data(), records.size()},
                                  {names.data(), names.size()},
                                  {schema.data(), schema.size()}, tables, error));
        {
            PlayerSavegameV1 sync;
            constexpr std::uintptr_t identity=UINT64_C(0x1234567800000099);
            require(sync.source_quest_sync_20()==0);
            sync.set_character(identity);
            require(!sync.source_quest_sync_update(identity,true) &&
                    sync.source_quest_sync_20()==0);
            require(!sync.source_quest_sync_update(identity+1,false) &&
                    sync.source_quest_sync_20()==0);
            require(sync.source_quest_sync_update(identity,false) &&
                    sync.source_quest_sync_20()==1);
        }
        std::size_t header_at = 4;
        const auto cases = read_word(gold, header_at);
        std::uint32_t comparisons = 0;
        std::uint32_t query_checks = 0;

        for (std::uint32_t c = 0; c < cases; ++c) {
            stage = "case input";
            const auto count = read_word(gold, reader.at);
            std::vector<std::int32_t> ids;
            ids.reserve(count);
            for (std::uint32_t i = 0; i < count; ++i)
                ids.push_back(static_cast<std::int32_t>(read_word(gold, reader.at)));

            PlayerSavegameV1 owner;
            require(owner.slot() == -1 && owner.level() == 0 &&
                    owner.class_id() == -1 && owner.name().empty() &&
                    !owner.has_skill_slots());
            owner.set_character(UINT64_C(0x1234567800000099));
            std::uint32_t update_calls = 0;
            const SavedSkillUpdateServicesV1 update{&update_calls, update_skills};

            const auto operation_count = read_word(gold, reader.at);
            for (std::uint32_t op_index = 0; op_index < operation_count; ++op_index) {
                stage = "operation parse";
                const auto operation = read_block(gold, reader.at);
                const auto expected = read_block(gold, reader.at);
                std::size_t op_at = 0;
                const auto kind = read_word(operation, op_at);
                switch (kind) {
                    case 0:
                        stage = "initialize source list";
                        if (!owner.skills_initialized()) ++update_calls;
                        require(owner.initialize_skills_from_character_list(ids, error));
                        break;
                    case 1: {
                        stage = "set level";
                        const auto index = read_word(operation, op_at);
                        const auto level = read_word(operation, op_at);
                        require(owner.set_skill_level(index,
                                                     static_cast<std::int32_t>(level),
                                                     error));
                        break;
                    }
                    case 2: {
                        stage = "set slot";
                        const auto slot = read_word(operation, op_at);
                        const auto index = read_word(operation, op_at);
                        require(owner.set_skill_in_slot(
                            static_cast<std::int32_t>(slot), index, update, error));
                        break;
                    }
                    case 3: {
                        stage = "load section";
                        const auto section = read_block(operation, op_at);
                        std::size_t consumed = 0;
                        require(owner.load_skills({section.data(), section.size()},
                                                  tables, consumed, error) == 0);
                        require(consumed == section.size());
                        break;
                    }
                    default:
                        require(false);
                }
                Raw actual;
                stage = "snapshot comparison";
                append_snapshot(actual, owner, update_calls);
                if (actual != expected) {
                    std::cerr << "mismatch case=" << c << " operation=" << op_index
                              << " actual=" << actual.size()
                              << " expected=" << expected.size() << '\n';
                    require(false);
                }
                ++comparisons;
                for (std::uint32_t i = 0; i < ids.size(); ++i) {
                    require(owner.skill_id(i) == ids[i]);
                    require(owner.skill_level(i) == owner.skills()[i].level);
                    const auto slot = owner.skill_slot(i);
                    if (slot != -1)
                        require(owner.skill_in_slot(slot) == static_cast<std::int32_t>(i));
                    query_checks += 3;
                }
            }
        }
        require(reader.at == gold.size());

        // A real-cache selection proves the row-3 fallback and zero-level
        // initialization without creating a PlayerSavegame for the fixture.
        std::uint32_t guards = 0;
        PlayerSavegameV1 selected;
        selected.set_character(UINT64_C(0x100000001));
        require(selected.initialize_skills(tables, -1, error));
        ++guards;
        require(selected.skills_initialized() &&
                selected.skills().size() == tables.skill_lists[3].members.size());
        ++guards;
        for (std::size_t i = 0; i < selected.skills().size(); ++i) {
            require(selected.skill_id(static_cast<std::uint32_t>(i)) ==
                    tables.skill_lists[3].members[i]);
            require(selected.skill_level(static_cast<std::uint32_t>(i)) == 0);
            guards += 2;
        }
        const auto before_repeat = selected.skills();
        require(selected.initialize_skills(tables, 0, error));
        ++guards;
        require(selected.skills().size() == before_repeat.size());
        ++guards;
        for (std::size_t i = 0; i < before_repeat.size(); ++i)
            require(selected.skills()[i].id == before_repeat[i].id &&
                    selected.skills()[i].level == 0);
        guards += static_cast<std::uint32_t>(before_repeat.size());

        stage = "saved callback projection";
        PropertyState property_state{};
        property_state.resolved[28] = -1;
        PropertyView live_properties{};
        live_properties.resolved = property_state.resolved.data();
        stage = "callback live property alias";
        require(live_properties.resolved == property_state.resolved.data());
        ++guards;
        const auto& fallback = tables.skill_lists[3].members;
        stage = "callback empty fallback";
        require(fallback.empty());
        ++guards;
        auto callback = player_saved_skill_callbacks_v1::get_skill_id_from_oid(
            &live_properties, &tables, INT32_MAX);
        stage = "GetSkillIDFromOID empty fallback";
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == -1);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, &selected,
            UINT64_C(0x100000001), 0);
        stage = "GetCurrentSkillInfo empty fallback";
        require(callback.disposition == player_saved_skill_callbacks_v1::Disposition::
                    no_return);
        ++guards;

        std::size_t valid_selector = tables.skill_lists.size();
        std::size_t alternate_selector = tables.skill_lists.size();
        for (std::size_t i = 0; i < tables.skill_lists.size(); ++i) {
            if (tables.skill_lists[i].members.empty()) continue;
            if (valid_selector == tables.skill_lists.size()) {
                valid_selector = i;
            } else if (tables.skill_lists[i].members !=
                           tables.skill_lists[valid_selector].members &&
                       std::find(tables.skill_lists[i].members.begin(),
                                 tables.skill_lists[i].members.end(),
                                 tables.skill_lists[valid_selector].members.front()) ==
                           tables.skill_lists[i].members.end()) {
                alternate_selector = i;
                break;
            }
        }
        stage = "callback real skill lists";
        require(valid_selector < tables.skill_lists.size() &&
                alternate_selector < tables.skill_lists.size());
        ++guards;
        property_state.resolved[28] = static_cast<std::int32_t>(valid_selector);
        PlayerSavegameV1 valid_saved;
        valid_saved.set_character(UINT64_C(0x100000002));
        require(valid_saved.initialize_skills(tables,
                                             static_cast<std::int32_t>(valid_selector),
                                             error));
        ++guards;
        const auto& valid_list = tables.skill_lists[valid_selector].members;
        callback = player_saved_skill_callbacks_v1::get_skill_id_from_oid(
            &live_properties, &tables, valid_list.front());
        stage = "GetSkillIDFromOID";
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == 0);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, &valid_saved,
            UINT64_C(0x100000002), 0);
        stage = "GetCurrentSkillInfo save owner";
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == 0);
        ++guards;
        require(valid_saved.set_skill_level(0, 7, error));
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, &valid_saved,
            UINT64_C(0x100000002), 0);
        stage = "GetCurrentSkillInfo live saved level";
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == 7);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, &valid_saved,
            UINT64_C(0x100000003), 0);
        stage = "GetCurrentSkillInfo owner identity";
        require(callback.disposition ==
                player_saved_skill_callbacks_v1::Disposition::owner_mismatch);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, nullptr,
            UINT64_C(0x100000002), 0);
        stage = "GetCurrentSkillInfo null save";
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == -1);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, &valid_saved,
            UINT64_C(0x100000002),
            static_cast<std::int32_t>(valid_list.size()));
        stage = "GetCurrentSkillInfo bounds";
        require(callback.disposition ==
                player_saved_skill_callbacks_v1::Disposition::no_return);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_skill_id_from_oid(
            &live_properties, &tables, INT32_MAX);
        stage = "GetSkillIDFromOID miss";
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == -1);
        ++guards;
        PropertyView missing_properties{};
        callback = player_saved_skill_callbacks_v1::get_skill_id_from_oid(
            &missing_properties, &tables, valid_list.front());
        stage = "callback missing property";
        require(callback.disposition ==
                player_saved_skill_callbacks_v1::Disposition::missing_projection);
        ++guards;
        PlayerSavegameV1 uninitialized;
        uninitialized.set_character(UINT64_C(0x100000099));
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &tables, &uninitialized,
            UINT64_C(0x100000099), 0);
        stage = "callback uninitialized save";
        require(callback.disposition == player_saved_skill_callbacks_v1::Disposition::
                    source_assertion_boundary);
        ++guards;
        SkillTables broken_skill_table = tables;
        broken_skill_table.skill_lists[valid_selector].members[0] = INT32_MAX;
        callback = player_saved_skill_callbacks_v1::get_current_skill_info(
            &live_properties, &broken_skill_table, &valid_saved,
            UINT64_C(0x100000002), 0);
        stage = "callback invalid table row";
        require(callback.disposition == player_saved_skill_callbacks_v1::Disposition::
                    source_assertion_boundary);
        ++guards;
        property_state.resolved[28] = static_cast<std::int32_t>(alternate_selector);
        stage = "callback live selector read";
        callback = player_saved_skill_callbacks_v1::get_skill_id_from_oid(
            &live_properties, &tables,
            tables.skill_lists[alternate_selector].members.front());
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == 0);
        ++guards;
        callback = player_saved_skill_callbacks_v1::get_skill_id_from_oid(
            &live_properties, &tables, valid_list.front());
        require(callback.disposition ==
                    player_saved_skill_callbacks_v1::Disposition::append_integer &&
                callback.integer == -1);
        ++guards;

        PlayerSavegameV1 no_character;
        require(!no_character.initialize_skills(tables, 0, error));
        ++guards;
        require(!no_character.skills_initialized() && no_character.skills().empty());
        ++guards;
        PlayerSavegameV1 bad_table;
        bad_table.set_character(1);
        SkillTables no_fallback;
        no_fallback.skill_lists.resize(2);
        require(!bad_table.initialize_skills(no_fallback, -1, error));
        ++guards;
        require(!bad_table.skills_initialized() && bad_table.skills().empty());
        ++guards;

        PlayerSavegameV1 mutation;
        mutation.set_character(UINT64_C(0x1234567800000099));
        require(mutation.initialize_skills_from_character_list({1, 2}, error));
        ++guards;
        const auto rows_before_failed_slot = mutation.skills();
        const SavedSkillUpdateServicesV1 unavailable{};
        require(!mutation.set_skill_in_slot(0, 0, unavailable, error));
        ++guards;
        require(mutation.skills().size() == rows_before_failed_slot.size() &&
                !mutation.has_skill_slots());
        ++guards;
        require(mutation.set_skill_level(0, -1, error));
        ++guards;
        require(mutation.skill_level(0) == 65535);
        ++guards;
        require(mutation.set_skill_level(0, 65536, error));
        ++guards;
        require(mutation.skill_level(0) == 0);
        ++guards;
        std::uint32_t reload_updates = 0;
        const SavedSkillUpdateServicesV1 reload_update{
            &reload_updates,
            [](void* context, std::uintptr_t character, std::string& message) {
                require(character == UINT64_C(0x1234567800000099));
                ++*static_cast<std::uint32_t*>(context);
                message.clear();
                return true;
            }};
        require(mutation.set_skill_in_slot(4, 1, reload_update, error));
        ++guards;
        require(mutation.set_skill_level(1, 9, error));
        ++guards;
        const auto same_character = mutation.character();
        require(mutation.reload_skills_from_character_list({7, 9, 11}, error));
        ++guards;
        require(mutation.character() == same_character && mutation.skills_initialized() &&
                mutation.skills().size() == 3 && mutation.skill_id(0) == 7 &&
                mutation.skill_id(1) == 9 && mutation.skill_id(2) == 11 &&
                mutation.skill_level(0) == 0 && mutation.skill_level(1) == 0 &&
                mutation.skill_level(2) == 0 && !mutation.has_skill_slots() &&
                mutation.skill_slots()[0].empty() && mutation.skill_slots()[1].empty());
        ++guards;
        require(reload_updates == 1);
        ++guards;
        std::uint32_t failing_updates = 0;
        const SavedSkillUpdateServicesV1 update_failure{
            &failing_updates,
            [](void* context, std::uintptr_t, std::string& message) {
                ++*static_cast<std::uint32_t*>(context);
                message = "source update callback failed";
                return false;
            }};
        require(!mutation.set_skill_in_slot(3, 1, update_failure, error));
        ++guards;
        require(mutation.skill_in_slot(3) == 1 && failing_updates == 1);
        ++guards;

        std::cout << "{\"validation\":\"PASS\",\"source_gold_cases\":"
                  << cases << ",\"source_state_comparisons\":" << comparisons
                  << ",\"query_checks\":" << query_checks
                  << ",\"guards\":" << guards << ",\"total_assertions\":" << checks
                  << ",\"mismatches\":0}\n";
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
