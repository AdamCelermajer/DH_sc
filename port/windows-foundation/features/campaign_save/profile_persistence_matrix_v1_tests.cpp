#include "../../asset_catalog.hpp"
#include "../../game_save.hpp"
#include "../../original_actor_properties.hpp"
#include "../../playable_actor_world.hpp"
#include "../../save_store.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

CharacterState profile_state(const OriginalPropertyDatabase& source,
                             OriginalCombatProperties& combat,
                             std::string& error) {
    check(build_original_combat_properties(source, "RoguePlayerBase", {256, true}, {}, {},
                                            combat, error), error);
    CharacterState state;
    state.id = "persistence-matrix-rogue";
    state.name = "Persistence Matrix Rogue";
    state.class_id = "RoguePlayerBase";
    state.stats.level = static_cast<std::uint32_t>(combat.sheets.resolved[19] >> 8);
    state.stats.health = original_signed256(combat.sheets.resolved[36]);
    state.stats.max_health = original_signed256(combat.sheets.resolved[38]);
    state.stats.resource = original_signed256(combat.sheets.resolved[41]);
    state.stats.max_resource = original_signed256(combat.sheets.resolved[43]);
    state.stats.strength = std::max(0.0f, original_signed256(combat.sheets.resolved[1]));
    state.stats.dexterity = std::max(0.0f, original_signed256(combat.sheets.resolved[2]));
    state.stats.endurance = std::max(0.0f, original_signed256(combat.sheets.resolved[4]));
    state.stats.energy = std::max(0.0f, original_signed256(combat.sheets.resolved[5]));
    state.source_endurance_energy_known = true;
    state.experience = 123456;
    state.gold = 7890;
    state.source_stat_points = 7;
    state.source_skill_points = 3;
    state.source_points_known = true;
    state.inventory = {{"rogue-weapon-instance", "SourceShortSword", 1},
                       {"rogue-armor-instance", "SourceLeatherArmor", 1},
                       {"rogue-potion-instance", "Potion0", 3}};
    state.equipment = {{"main_hand", "rogue-weapon-instance", 0, 0},
                       {"body", "rogue-armor-instance", 0, 3}};
    // The audited Rogue source SkillList begins with JumpKick. These explicit
    // rank/slot values exercise the canonical saved-row representation; this
    // codec test does not claim the source native save currently has rank 2.
    state.skills = {{"JumpKick", 2}, {"ViciousStrike", 1}, {"Quickness", 0}};
    state.source_skill_slots_known = true;
    state.skill_slots = {{0, 0, 0}, {0, 1, 1}, {1, 0, 2}};
    const auto faery_column = std::find(source.characters.fields.begin(), source.characters.fields.end(),
                                        "FaeryList");
    check(faery_column != source.characters.fields.end(), "Rogue CharacterTable FaeryList column missing");
    const auto rogue_row = std::find(source.characters.names.begin(), source.characters.names.end(),
                                     "RoguePlayerBase");
    check(rogue_row != source.characters.names.end(), "Rogue CharacterTable row missing");
    state.source_faery_list_id = source.characters.rows[std::size_t(rogue_row - source.characters.names.begin())]
                                                            [std::size_t(faery_column - source.characters.fields.begin())];
    check(state.source_faery_list_id >= 0, "Rogue source FaeryList is unavailable");
    state.source_faery_state_known = true;
    state.faery_by_difficulty[0].current_faery = 2;
    state.faery_by_difficulty[0].faeries[2] = {1, 1};
    // Preserve explicit unknown legacy fields; the codec must not infer these.
    state.source_quest_progress_cqpg.clear();
    const auto valid = validate_character_state(state);
    check(valid.ok(), valid.errors.empty() ? "profile state invalid" : valid.errors.front());
    return state;
}

bool same_profile(const CharacterState& a, const CharacterState& b) {
    return a.schema_version == b.schema_version && a.id == b.id && a.name == b.name &&
        a.class_id == b.class_id && a.stats.level == b.stats.level &&
        a.stats.health == b.stats.health && a.stats.max_health == b.stats.max_health &&
        a.stats.resource == b.stats.resource && a.stats.max_resource == b.stats.max_resource &&
        a.stats.strength == b.stats.strength && a.stats.dexterity == b.stats.dexterity &&
        a.stats.endurance == b.stats.endurance && a.stats.energy == b.stats.energy &&
        a.experience == b.experience && a.gold == b.gold &&
        a.source_stat_points == b.source_stat_points && a.source_skill_points == b.source_skill_points &&
        a.source_endurance_energy_known == b.source_endurance_energy_known &&
        a.source_points_known == b.source_points_known &&
        a.source_skill_slots_known == b.source_skill_slots_known &&
        a.source_faery_state_known == b.source_faery_state_known &&
        a.inventory.size() == b.inventory.size() &&
        std::equal(a.inventory.begin(), a.inventory.end(), b.inventory.begin(), [](const auto& x, const auto& y) {
            return x.instance_id == y.instance_id && x.definition_id == y.definition_id && x.quantity == y.quantity;
        }) && a.equipment.size() == b.equipment.size() &&
        std::equal(a.equipment.begin(), a.equipment.end(), b.equipment.begin(), [](const auto& x, const auto& y) {
            return x.slot == y.slot && x.item_instance_id == y.item_instance_id &&
                   x.equipment_set == y.equipment_set && x.source_slot == y.source_slot;
        }) && a.skills.size() == b.skills.size() &&
        std::equal(a.skills.begin(), a.skills.end(), b.skills.begin(), [](const auto& x, const auto& y) {
            return x.id == y.id && x.rank == y.rank;
        }) && a.skill_slots.size() == b.skill_slots.size() &&
        std::equal(a.skill_slots.begin(), a.skill_slots.end(), b.skill_slots.begin(), [](const auto& x, const auto& y) {
            return x.equipment_set == y.equipment_set && x.slot == y.slot && x.saved_skill_row == y.saved_skill_row;
        }) && a.source_faery_list_id == b.source_faery_list_id &&
        std::equal(a.faery_by_difficulty.begin(), a.faery_by_difficulty.end(),
                   b.faery_by_difficulty.begin(), [](const auto& x, const auto& y) {
            if (x.current_faery != y.current_faery) return false;
            for (std::size_t i = 0; i < x.faeries.size(); ++i)
                if (x.faeries[i].state != y.faeries[i].state || x.faeries[i].level != y.faeries[i].level)
                    return false;
            return true;
        }) &&
        a.source_quest_progress_cqpg == b.source_quest_progress_cqpg;
}

std::vector<unsigned char> file_bytes(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

void write_legacy_schema1(const std::filesystem::path& path) {
    std::vector<unsigned char> bytes{'D','H','S','A','V','E',0,1};
    const auto word = [&](std::uint64_t value, unsigned width) {
        for (unsigned i = 0; i < width; ++i) bytes.push_back(static_cast<unsigned char>(value >> (8 * i)));
    };
    const auto string = [&](const std::string& value) { word(value.size(),4); bytes.insert(bytes.end(),value.begin(),value.end()); };
    const auto real = [&](float value) { std::uint32_t bits=0;std::memcpy(&bits,&value,4);word(bits,4); };
    word(1,4); word(1,4); string("legacy-rogue");string("Legacy Rogue");string("RoguePlayerBase");
    word(1,4);for(float value:{100.0f,100.0f,40.0f,40.0f,0.0f,0.0f,0.0f})real(value);
    word(100,8);word(50,8);word(0,4);word(0,4);word(0,4);word(0,4);
    std::ofstream stream(path,std::ios::binary|std::ios::trunc);
    check(bool(stream),"Could not create schema1 legacy fixture");
    stream.write(reinterpret_cast<const char*>(bytes.data()),std::streamsize(bytes.size()));
    stream.close();check(bool(stream),"Could not write schema1 legacy fixture");
}

void verify_legacy_unknowns(const std::filesystem::path& scratch) {
    const auto path=scratch/"legacy-schema1.dhsave";write_legacy_schema1(path);
    CharacterState loaded;std::string error;
    check(load_character(path,loaded,error),error);
    check(loaded.id=="legacy-rogue"&&loaded.schema_version==character_schema_version&&
          !loaded.source_endurance_energy_known&&!loaded.source_points_known&&
          !loaded.source_skill_slots_known&&!loaded.source_faery_state_known&&
          loaded.source_faery_list_id==-1&&loaded.skill_slots.empty()&&
          loaded.faery_by_difficulty[0].current_faery==0,
          "Schema1 decode inferred absent source metadata instead of retaining unknown defaults");
}

void verify_failure_atomicity(const std::filesystem::path& char_path,
                              const std::filesystem::path& game_path,
                              const std::filesystem::path& scratch,
                              const CharacterState& expected,
                              const GameSave& game) {
    std::string error;
    auto truncated_char=scratch/"truncated.dhsave";
    auto char_bytes=file_bytes(char_path);char_bytes.resize(std::min<std::size_t>(char_bytes.size(),17));
    {std::ofstream out(truncated_char,std::ios::binary|std::ios::trunc);out.write(reinterpret_cast<const char*>(char_bytes.data()),std::streamsize(char_bytes.size()));}
    CharacterState sentinel=expected;sentinel.name="sentinel-not-replaced";
    check(!load_character(truncated_char,sentinel,error)&&sentinel.name=="sentinel-not-replaced",
          "Failed SaveStore read partially replaced CharacterState");
    auto unchanged_char=file_bytes(char_path);auto invalid=expected;invalid.id.clear();
    check(!save_character(char_path,invalid,error)&&file_bytes(char_path)==unchanged_char,
          "Invalid SaveStore write replaced existing profile file");

    auto truncated_game=scratch/"truncated.game";auto game_bytes=file_bytes(game_path);
    game_bytes.resize(std::min<std::size_t>(game_bytes.size(),18));
    {std::ofstream out(truncated_game,std::ios::binary|std::ios::trunc);out.write(reinterpret_cast<const char*>(game_bytes.data()),std::streamsize(game_bytes.size()));}
    GameSave game_sentinel;game_sentinel.level_uri="sentinel-not-replaced";
    check(!load_game(truncated_game,game_sentinel,error)&&game_sentinel.level_uri=="sentinel-not-replaced",
          "Failed GameSave read partially replaced destination");
    const auto unchanged_game=file_bytes(game_path);auto invalid_game=game;invalid_game.character.id.clear();
    check(!save_game(game_path,invalid_game,error)&&file_bytes(game_path)==unchanged_game,
          "Invalid GameSave write replaced existing checkpoint file");
}
}

int main(int argc,char** argv) try {
    check(argc==5,"Use write|verify <assets-root> <character-save> <game-save>");
    const std::string mode=argv[1];
    const std::filesystem::path assets_path=argv[2],char_path=argv[3],game_path=argv[4];
    const auto scratch=char_path.parent_path();std::filesystem::create_directories(scratch);
    AssetCatalog assets(assets_path);OriginalPropertyDatabase source;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata/",source,error),error);
    OriginalCombatProperties combat;auto expected=profile_state(source,combat,error);
    if(mode=="write"){
        check(save_character(char_path,expected,error),error);
        dh2::data::CombatRandom random{0x5a17u,19};dh2::data::AiTables ai;
        check(load_original_ai_tables(assets,"original-cache/data/pydata/",ai,error),error);
        PlayableActorWorld world(std::move(ai),random);
        ActorState actor;actor.id=1;actor.definition_id="RoguePlayerBase";actor.class_id="RoguePlayerBase";
        actor.persistent_character_id=expected.id;actor.faction_id=combat.sheets.resolved[0];
        actor.health=expected.stats.health;actor.max_health=expected.stats.max_health;
        actor.resource=expected.stats.resource;actor.max_resource=expected.stats.max_resource;
        actor.equipment={{"main_hand","SourceShortSword","rogue-weapon-instance"},
                         {"body","SourceLeatherArmor","rogue-armor-instance"}};
        check(world.bind_actor(std::move(actor),combat,{true,true,std::nullopt},error),error);
        GameSave captured;check(capture_game_save("original-cache/data/scene/001_swamp.mlx",1,
                                                 expected,world,captured,error),error);
        check(save_game(game_path,captured,error),error);
        std::cout<<"{\"phase\":\"write\",\"profile\":\"RoguePlayerBase\",\"character_schema\":3,\"game_save_version\":"<<captured.version<<"}\n";
        return 0;
    }
    check(mode=="verify","Unknown persistence matrix phase");
    CharacterState decoded;check(load_character(char_path,decoded,error),error);
    check(same_profile(expected,decoded),"Fresh-process SaveStore decode changed profile persistence matrix");
    GameSave loaded;check(load_game(game_path,loaded,error),error);
    check(loaded.version==1&&same_profile(expected,loaded.character),
          "Fresh-process GameSave decode changed canonical profile fields");
    check(loaded.actors.size()==1&&loaded.actors[0].actor.id==1&&
          loaded.actors[0].actor.equipment.size()==2&&
          loaded.actors[0].combat.sheets.resolved[36]==combat.sheets.resolved[36]&&
          loaded.actors[0].combat.sheets.resolved[41]==combat.sheets.resolved[41],
          "GameSave did not retain same-profile actor vitals, source sheets, or gear bindings");
    dh2::data::CombatRandom random{0x5a17u,19};dh2::data::AiTables ai;
    check(load_original_ai_tables(assets,"original-cache/data/pydata/",ai,error),error);
    PlayableActorWorld world(std::move(ai),random);ActorState actor;
    actor.id=1;actor.definition_id="RoguePlayerBase";actor.class_id="RoguePlayerBase";
    actor.persistent_character_id=expected.id;actor.faction_id=combat.sheets.resolved[0];
    actor.health=expected.stats.health;actor.max_health=expected.stats.max_health;
    actor.resource=expected.stats.resource;actor.max_resource=expected.stats.max_resource;
    actor.equipment={{"main_hand","SourceShortSword","rogue-weapon-instance"},
                     {"body","SourceLeatherArmor","rogue-armor-instance"}};
    check(world.bind_actor(std::move(actor),combat,{true,true,std::nullopt},error),error);
    CharacterState restored;restored.id="restore-sentinel";
    check(restore_game_save(loaded,"original-cache/data/scene/001_swamp.mlx",world,restored,error),error);
    check(same_profile(expected,restored)&&world.find_actor(1)->equipment.size()==2,
          "GameSave restore did not restore same profile and actor gear state");
    verify_legacy_unknowns(scratch);verify_failure_atomicity(char_path,game_path,scratch,expected,loaded);
    auto mismatch_character=restored;mismatch_character.name="atomic-restore-sentinel";
    const auto before_health=world.find_actor(1)->health;const auto before_rng=world.random_state();
    check(!restore_game_save(loaded,"different-level.mlx",world,mismatch_character,error)&&
          mismatch_character.name=="atomic-restore-sentinel"&&world.find_actor(1)->health==before_health&&
          world.random_state().seed==before_rng.seed&&world.random_state().calls==before_rng.calls,
          "Rejected GameSave route changed profile/world/RNG");
    std::cout<<"{\"validation\":\"PASS\",\"profile\":\"RoguePlayerBase\",\"fresh_process_decode\":true,\"save_store\":true,\"game_save_restore\":true,\"hp_mp_source_derived\":true,\"xp_gold\":true,\"gear_instances\":true,\"skill_ranks_slots\":true,\"faery_selection\":true,\"legacy_unknowns\":true,\"failure_atomicity\":true}\n";
    return 0;
} catch(const std::exception& ex) { std::cerr<<ex.what()<<'\n';return 1; }
