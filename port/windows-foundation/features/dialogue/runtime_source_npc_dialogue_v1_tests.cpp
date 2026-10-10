#include "runtime_source_npc_dialogue_v1.hpp"
#include "../../../game-data/data.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::dialogue;
namespace fs = std::filesystem;

namespace {
void check(bool value, const std::string& why) {
    if (!value) throw std::runtime_error(why);
}
std::vector<std::uint8_t> read(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    check(bool(file), "missing original source data: " + path.string());
    return {std::istreambuf_iterator<char>(file), {}};
}
struct TextFixture {
    fs::path data;
    dh2_script_constants* constants{dh2_script_constants_create()};
    std::map<std::uintptr_t, bool> leases;
    std::uintptr_t next_lease{1};
    const CharacterState* character{};
    ~TextFixture() { dh2_script_constants_destroy(constants); }
    static bool open(void* raw, const char* uri, bool& found,
                     std::vector<std::uint8_t>& bytes, std::uintptr_t& lease,
                     std::string& error) {
        auto& self = *static_cast<TextFixture*>(raw);
        std::ifstream file(self.data / uri, std::ios::binary);
        if (!file) { found = false; lease = 0; error.clear(); return true; }
        bytes.assign(std::istreambuf_iterator<char>(file), {});
        found = true; lease = self.next_lease++; self.leases.emplace(lease, true);
        error.clear(); return true;
    }
    static bool close(void* raw, std::uintptr_t lease, std::string& error) {
        auto& self = *static_cast<TextFixture*>(raw);
        if (self.leases.erase(lease) != 1) { error = "StringManager lease mismatch"; return false; }
        error.clear(); return true;
    }
    static bool debug(void*, const char*, std::string& error) {
        error.clear(); return true;
    }
    static bool constant(void* raw, const char* group, const char* key,
                         std::uint32_t& value, std::string& error) {
        auto& self = *static_cast<TextFixture*>(raw);
        std::int32_t signed_value{};
        if (dh2_script_constants_get(self.constants, group, key, &signed_value) != 0) {
            error = std::string("source constant lookup failed: ") + group + "." + key;
            return false;
        }
        std::memcpy(&value, &signed_value, sizeof value);
        error.clear(); return true;
    }
    static bool player_character(void* raw, std::uintptr_t& out, std::string& error) {
        auto& self = *static_cast<TextFixture*>(raw);
        out = reinterpret_cast<std::uintptr_t>(self.character);
        error.clear(); return true;
    }
    static bool player_name(void* raw, std::uintptr_t id, std::string& out,
                            std::string& error) {
        auto& self = *static_cast<TextFixture*>(raw);
        if (!self.character || id != reinterpret_cast<std::uintptr_t>(self.character)) {
            error = "localized NPC response escaped same CharacterState"; return false;
        }
        out = self.character->name;
        error.clear(); return true;
    }
};
}

int main(int argc, char** argv) {
    try {
        check(argc == 4, "usage: runtime_source_npc_dialogue_v1_tests quest-cache source-data assets-root");
        std::string error;
        const fs::path quest_cache(argv[1]);
        TextFixture fixture; fixture.data = fs::path(argv[2]);
        const fs::path asset_root(argv[3]);
        check(fixture.constants != nullptr, "actual StringManager constants owner");
        for (const auto& path : {fixture.data / "pydata/common_text_pycst.bin",
                                 fs::path(argv[3]) / "data/v2quests_pycst.bin"}) {
            const auto bytes = read(path);
            dh2_script_constants_reload reload{};
            check(dh2_script_constants_load(fixture.constants, bytes.data(),
                  static_cast<std::uint32_t>(bytes.size()), &reload) == 0 &&
                  reload.consumed == bytes.size(), "load actual source constants");
        }

        const auto quest_array = read(quest_cache / "v2quests_pyarray.bin");
        const auto quest_names = read(quest_cache / "v2quests_pyarraynames.bin");
        auto tables = std::make_shared<dh2::data::QuestTablesPersistenceV51>();
        check(tables->decode({quest_array.data(), quest_array.size()},
                             {quest_names.data(), quest_names.size()}, error), error);
        auto quest_it = std::find_if(tables->rows().begin(), tables->rows().end(),
            [](const auto& row) { return row.name == "Crypt02_explore"; });
        check(quest_it != tables->rows().end() && quest_it->act == 1,
              "actual Act 1 Crypt02_explore Quest row");
        const auto row_index = static_cast<std::int32_t>(quest_it - tables->rows().begin());
        auto objective_it = std::find_if(quest_it->objectives.begin(), quest_it->objectives.end(),
            [](const auto& row) { return row.type == 5 && row.oid1 == 348; });
        check(objective_it != quest_it->objectives.end(),
              "original Act 1 TalkToNPC row targets source NPC identity 348");

        // Resolve the authored TalkToNPC key against the actual source
        // CharacterTable. Its StringID is a separate localization namespace.
        const auto property_data = read(asset_root / "data/character_properties_pyarray.bin");
        const auto property_names = read(asset_root / "data/character_properties_pyarraynames.bin");
        const auto property_schema = read(asset_root / "data/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        check(dh2::data::load_characters(
                  {property_data.data(), property_data.size()},
                  {property_names.data(), property_names.size()},
                  {property_schema.data(), property_schema.size()}, characters, error), error);
        check(characters.names.size() == 448 && objective_it->oid1 >= 0 &&
                  objective_it->oid1 < static_cast<std::int32_t>(characters.names.size()),
              "source TalkToNPC target key lies in decoded CharacterTable");
        check(characters.names[static_cast<std::size_t>(objective_it->oid1)] == "SisterEllen",
              "Crypt02_explore target 348 resolves to source CharacterTable row SisterEllen");
        check(std::find(characters.names.begin(), characters.names.end(), "Celeste") ==
                  characters.names.end(),
              "the source CharacterTable does not contain a Celeste row to join to target 348");

        CharacterState character; character.id = "source-dialogue-character";
        character.name = "Ada";
        fixture.character = &character;
        CharacterQuestProgressV1 progress;
        check(progress.initialize_fresh(character, *tables, error), error);
        const CharacterQuestIdV1 quest_id{0, 0, row_index};
        check(progress.record_source_state(character, quest_id, 6, error), error);

        auto records = read(fixture.data / "pydata/common_text_pyarray.bin");
        auto names = read(fixture.data / "pydata/common_text_pyarraynames.bin");
        auto schema = read(fixture.data / "pydata/common_text_pystructnames.bin");
        dh2::ui::HudTextV1 text;
        check(text.load({records.data(), records.size()}, {names.data(), names.size()},
                        {schema.data(), schema.size()}, error), error);
        dh2::ui::LocalizationServices localization{
            &fixture, &TextFixture::open, &TextFixture::close, &TextFixture::debug,
            &TextFixture::constant, &TextFixture::player_character,
            &TextFixture::player_name};
        dh2::ui::HudTextEnvironmentV1 environment;
        environment.localization = localization;
        check(text.switch_pack(0, false, error), error);

        CharacterQuestTextV1 quest_text = [&text, &environment](
            const CharacterState&, std::int32_t id, std::string& value,
            std::string& callback_error) {
            bool is_null{};
            return text.integer_string(id, environment.localization, value,
                                       is_null, callback_error) && !is_null;
        };
        std::int32_t dialogue_id{};
        check(dh2_script_constants_get(fixture.constants, "StrID",
              "GRAVEYARD_CRYPT_INTRO_CELESTE_1", &dialogue_id) == 0 && dialogue_id == 393217,
              "actual source Crypt/Celeste localization symbol and StringID");

        // Keep this call as a projection/localization check only. The source
        // table check above has already rejected the proposed Celeste join;
        // this manually supplied route must not be treated as identity proof.
        SourceNpcDialogueRouteV1 route;
        route.collection = quest_id.collection; route.difficulty = quest_id.difficulty;
        route.quest_row = row_index; route.quest = &*quest_it;
        route.talk_objective = &*objective_it; route.npc_identity = objective_it->oid1;
        route.dialogue_string_id = dialogue_id;
        route.dialogue_symbol = "GRAVEYARD_CRYPT_INTRO_CELESTE_1";
        SourceNpcDialogueResponseV1 response;
        check(project_source_npc_dialogue_v1(character, progress, *tables, route,
              quest_text, text, environment, response, error), error);
        check(response.character_id == character.id && response.quest_name == "Crypt02_explore" &&
              response.quest_state == 6 && response.npc_identity == 348 &&
              response.dialogue_string_id == 393217 && response.quest_title.has_value() &&
              response.dialogue_text ==
                  "I feel a strange power here, Ada. Such... terrible sadness. Should we really go in?",
              "same Character/Act1 Quest/NPC and player-localized response");

        auto wrong_npc = route; ++wrong_npc.npc_identity;
        check(!project_source_npc_dialogue_v1(character, progress, *tables, wrong_npc,
              quest_text, text, environment, response, error) &&
              error.find("TalkToNPC target") != std::string::npos &&
              response.dialogue_text.empty(),
              "mismatched source NPC target is rejected");
        check(progress.record_source_state(character, quest_id, 0, error), error);
        check(!project_source_npc_dialogue_v1(character, progress, *tables, route,
              quest_text, text, environment, response, error) &&
              error.find("active source Quest") != std::string::npos,
              "inactive source quest does not produce an NPC response");
        CharacterState other = character; other.id = "other-character";
        check(!project_source_npc_dialogue_v1(other, progress, *tables, route,
              quest_text, text, environment, response, error) &&
              error.find("different CharacterState") != std::string::npos,
              "dialogue projection cannot cross CharacterState/quest owner");
        check(fixture.leases.empty(), "all source StringManager file leases closed");
        std::cout << "PASS source-table mapping gate: Crypt02_explore target 348 -> SisterEllen; Celeste StringID 393217 is not an actor mapping\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
