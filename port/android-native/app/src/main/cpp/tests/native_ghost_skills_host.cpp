#include "../native_debug_files.hpp"
#include "../native_ghost_death_v1.hpp"
#include "../native_ghost_skills.hpp"

#include "port/game-data/data.hpp"
#include "port/level-world/native_monster_attack_request_v1.hpp"
#include "port/level-world/character_ai_set_target.hpp"
#include "port/level-world/debug_switches_runtime.hpp"
extern "C" {
#include "port/pydata-constants/constants.h"
}

#include <array>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace data = dh2::data;
namespace native = dh2::native::ghost_skills;
namespace death = dh2::native::ghost_death;

namespace {
std::vector<std::uint8_t> read_file(const char* path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error(std::string("cannot read ") + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
std::size_t row_id(const data::CharacterTable& table, const char* name) {
    for (std::size_t i = 0; i < table.names.size(); ++i)
        if (table.names[i] == name) return i;
    throw std::runtime_error(std::string("missing Character row ") + name);
}
std::size_t property_id(const data::CharacterTable& table, const char* name) {
    for (std::size_t i = 0; i < table.fields.size(); ++i)
        if (table.fields[i] == name) return i;
    throw std::runtime_error(std::string("missing property ") + name);
}

struct InitVcbFixture { std::uintptr_t expected_active = 0; unsigned calls = 0; };
std::int32_t init_vcb(void* raw, std::uintptr_t active) {
    if (!raw) return 1;
    auto& fixture = *static_cast<InitVcbFixture*>(raw);
    if (active != fixture.expected_active) return 1;
    ++fixture.calls;
    return 0;
}
struct ScriptFixture { unsigned loads=0,calls=0,declared_with_args=0,declared_without_args=0; };
std::int32_t load_skill_script(void* raw,const char* name,std::size_t bytes,bool* loaded) {
    if(!raw||(!name&&bytes)||!loaded)return 1;
    auto& f=*static_cast<ScriptFixture*>(raw);++f.loads;*loaded=true;return 0;
}
std::int32_t call_skill_script(void* raw,const char* function,
                               const dh2_script_value* args,std::uint32_t count) {
    if(!raw||!function||std::string(function)!="DeclareSkill"||(count&&!args))return 1;
    auto& f=*static_cast<ScriptFixture*>(raw);++f.calls;
    if(count==2&&args[0].type==DH2_SCRIPT_STRING&&args[1].type==DH2_SCRIPT_NUMBER)
        ++f.declared_with_args;
    else if(count==0)++f.declared_without_args;
    else return 1;
    return 0;
}

struct MonsterClassFixture {
    dh2::character_ai_classification::AiRow row{0, 4};
    dh2::character_ai_classification::AiTable table{&row, 1};
};
std::int32_t monster_class_invoke(
    void* raw, dh2::character_ai_classification::State*,
    const dh2::character_ai_classification::Request* request,
    dh2::character_ai_classification::Response* response) {
    if (!raw || !request || !response) return 1;
    const auto& fixture = *static_cast<MonsterClassFixture*>(raw);
    if (request->operation == dh2::character_ai_classification::Operation::ai_count) {
        response->count = 1;
        return 0;
    }
    if (request->operation == dh2::character_ai_classification::Operation::ai_table) {
        response->table = &fixture.table;
        return 0;
    }
    return 1;
}
struct TargetFixture { unsigned calls = 0; };
int source_target_service(void* raw,
    const dh2::character::set_target::Request* request,
    dh2::character::set_target::Response* response) {
    if (!raw || !request || !response) return 1;
    auto& fixture = *static_cast<TargetFixture*>(raw);
    ++fixture.calls;
    if (request->operation == dh2::character::set_target::debug_switches_load ||
        request->operation == dh2::character::set_target::debug_switch_lookup)
        return 0;
    return 1;
}

void run_actor(const char* name, std::size_t row_index,
               const data::CharacterTable& character_table,
               const data::PropertyRules& rules,
               const data::SkillTables& skills, const data::FaeryTables& faeries,
               const dh2_pycst_view& constants,
               const dh2_pycst_view& animation_constants,
               dh2::native::debug_files::Backend& debug) {
    data::PropertyState properties{};
    data::reset_properties(rules, properties, &character_table.rows[row_index]);
    std::string error;
    require(data::recalc_properties(rules, properties, error), error.c_str());
    require(properties.resolved[28] == -1 && properties.resolved[29] == -1,
            "Crypt Ghost authored SkillTree/FaeryList defaults changed");
    auto property_view = data::property_view(rules, properties);

    dh2::character_ai_initialization::State ai{};
    ai.identity = reinterpret_cast<std::uintptr_t>(&ai);
    ai.owner_04 = reinterpret_cast<std::uintptr_t>(&properties);
    ai.active_ais_1c = ai.identity + 0x100;
    InitVcbFixture vcb{ai.active_ais_1c, 0};
    ScriptFixture scripts{};
    std::string ais_path = "data/scripts/ai/";
    native::Runtime runtime;
    native::Bindings bindings{&ai, &property_view, &skills, &faeries, &ais_path,
                              0, &constants, &debug.globals(), &debug.services(),
                              &vcb, init_vcb,&scripts,load_skill_script,call_skill_script};
    native::DeathCleanupResult premature_cleanup{};
    require(runtime.cleanup_death_list(
                dh2::character_ai_set_skills_and_spells::List::skill,
                premature_cleanup) == native::Status::invalid_argument &&
            premature_cleanup.completed == 0 && premature_cleanup.slots_examined == 0,
            "death cleanup accepted vectors before source initialization");
    native::Result first{};
    require(runtime.prepare(bindings, first) == native::Status::complete,
            "source Ghost skill/five-null-faery preparation failed");
    require(runtime.skill_scripts().empty() && runtime.faery_scripts().size() == 5,
            "native CharAI vectors differ from the source empty/default records");
    for (const auto identity : runtime.faery_scripts())
        require(identity == 0, "a zero-length Fake_* spell acquired a script object");
    require(first.source.skill_slots == 0 && first.source.faery_slots == 5 &&
            first.source.null_appends == 5 && first.source.declarations == 0 &&
            first.source.script_allocations == 0,
            "SetSkills source result counters changed");
    require(first.skill_list_property == -1 && first.faery_list_property == -1,
            "live PropertyView getters did not preserve raw -1 list IDs");
    require(first.arguments_created == 2 && first.arguments_destroyed == 2,
            "source temporary Arguments lifetime/order differs");
    require(first.debug_loads == 2 && first.debug_queries == 2,
            "source debug load/query phases were skipped");
    require(first.init_vcb_calls == 1 && vcb.calls == 1,
            "fresh active AIS InitVCB callback was not invoked");
    require(ais_path == "data/scripts/ai/",
            "AIS script path was not restored after the source call");

    native::DeathCleanupResult skill_cleanup{}, spell_cleanup{};
    require(runtime.cleanup_death_list(
                dh2::character_ai_set_skills_and_spells::List::skill,
                skill_cleanup) == native::Status::complete &&
            skill_cleanup.completed == 1 && skill_cleanup.slots_examined == 0 &&
            skill_cleanup.null_slots == 0,
            "source _SkillCleanUp did not consume the real empty Ghost vector");
    require(runtime.cleanup_death_list(
                dh2::character_ai_set_skills_and_spells::List::faery,
                spell_cleanup) == native::Status::complete &&
            spell_cleanup.completed == 1 && spell_cleanup.slots_examined == 5 &&
            spell_cleanup.null_slots == 5,
            "source _SpellCleanUp did not skip the five real null Ghost entries");

    const auto target_identity = ai.owner_04 + 0x200;
    const dh2::native_monster_attack_request_v1::Input attack_request{
        ai.owner_04, ai.owner_04, ai.owner_04, target_identity, target_identity,
        target_identity, -2, 0};
    require(dh2::native_monster_attack_request_v1::accepts(attack_request),
            "Crypt attack request rejected the retained Character/CharAI owner pair");
    dh2::character::set_target::OwnerFacts target_owner{
        ai.owner_04, 4, 0, 0};
    dh2::character::set_target::State target_state{
        ai.identity, &target_owner, target_identity, target_identity,
        target_identity, 1, 1, 0, 0};
    TargetFixture target_fixture;
    const dh2::character::set_target::Services target_services{
        &target_fixture, 0, source_target_service};
    require(dh2::character::set_target::dh2_character_ai_set_target(
                &target_state, 0, 0, &target_services) ==
                dh2::character::set_target::complete && target_state.target == 0 &&
            target_state.requested_target == 0 && target_fixture.calls == 2,
            "source AI_SetTarget did not clear the live Crypt target on the same CharAI");

    MonsterClassFixture class_fixture;
    dh2::character_ai_classification::State classification{
        ai.owner_04, 0, -1, name, 0};
    const dh2::character_ai_classification::Services class_services{
        &class_fixture, monster_class_invoke};
    death::Bindings death_bindings{ai.identity, ai.owner_04, &animation_constants,
        &classification, &class_services, &runtime};
    dh2::player_ai_death_v1::Request death_request{};
    death_request.ai = ai.identity;
    death_request.character = ai.owner_04;
    dh2::player_ai_death_v1::Reply death_reply{};
    error.clear();
    death_request.operation = dh2::player_ai_death_v1::Operation::anim_stance;
    require(death::invoke(&death_bindings, &death_request, &death_reply, error) == 0 &&
            death_reply.word == 0,
            "player_ai_death backend did not use the verified Monster stance owner");
    death_request.operation = dh2::player_ai_death_v1::Operation::skill_cleanup;
    require(death::invoke(&death_bindings, &death_request, &death_reply, error) == 0 &&
            death_reply.count == 0,
            "player_ai_death backend did not clean the canonical Ghost skill vector");
    death_request.operation = dh2::player_ai_death_v1::Operation::spell_cleanup;
    require(death::invoke(&death_bindings, &death_request, &death_reply, error) == 0 &&
            death_reply.count == 5,
            "player_ai_death backend did not clean the canonical Ghost faery vector");
    death_request.ai++;
    require(death::invoke(&death_bindings, &death_request, &death_reply, error) != 0,
            "player_ai_death backend accepted a different retained CharAI identity");
    death_request.ai = ai.identity;
    death_request.operation = dh2::player_ai_death_v1::Operation::group_died;
    require(death::invoke(&death_bindings, &death_request, &death_reply, error) != 0,
            "Ghost provider reported success for a source service owned by another backend");

    const auto first_snapshot = runtime.faery_scripts();
    native::Result repeated{};
    require(runtime.prepare(bindings, repeated) == native::Status::complete,
            "repeated source preparation failed");
    require(runtime.faery_scripts() == first_snapshot && repeated.source.faery_slots == 0 &&
            repeated.source.null_appends == 0 && repeated.init_vcb_calls == 1 &&
            vcb.calls == 2,
            "repeated preparation rebuilt or changed the existing faery vector");
    require(repeated.arguments_created == 1 && repeated.arguments_destroyed == 1,
            "repeat call did not retain the source's empty skill-list Arguments phase");
    require(ais_path == "data/scripts/ai/", "repeat call did not restore the AIS path");
    std::cout << "ghost=" << name << " first_slots=0/5 repeated_slots="
              << repeated.source.faery_slots << " vcb=" << vcb.calls << '\n';
}

void run_nonempty_skill_case(const data::CharacterTable& character_table,
               const data::PropertyRules& rules,
               const data::SkillTables& skills, const data::FaeryTables& faeries,
               const dh2_pycst_view& constants,
               dh2::native::debug_files::Backend& debug, std::size_t row_index) {
    std::size_t list_index=skills.skill_lists.size();
    for(std::size_t i=0;i<skills.skill_lists.size();++i) {
        const auto& members=skills.skill_lists[i].members;
        if(!members.empty()&&std::any_of(members.begin(),members.end(),[&](std::int32_t row) {
            return row>=0&&static_cast<std::size_t>(row)<skills.skills.size()&&
                   skills.skills[static_cast<std::size_t>(row)].script_length!=0;
        })) {list_index=i;break;}
    }
    require(list_index<skills.skill_lists.size(),"real skill cache has no scripted list for focused Ghost provider case");
    data::PropertyState properties{};
    data::reset_properties(rules,properties,&character_table.rows[row_index]);
    properties.base[28]=static_cast<std::int32_t>(list_index);
    std::string error;
    require(data::recalc_properties(rules,properties,error),error.c_str());
    auto view=data::property_view(rules,properties);
    dh2::character_ai_initialization::State ai{};
    ai.identity=reinterpret_cast<std::uintptr_t>(&ai);ai.owner_04=reinterpret_cast<std::uintptr_t>(&properties);
    ai.active_ais_1c=ai.identity+0x100;
    InitVcbFixture vcb{ai.active_ais_1c,0};ScriptFixture scripts{};
    std::string ais_path="data/scripts/ai/";
    native::Runtime runtime;
    native::Bindings bindings{&ai,&view,&skills,&faeries,&ais_path,0,&constants,
        &debug.globals(),&debug.services(),&vcb,init_vcb,&scripts,load_skill_script,call_skill_script};
    native::Result prepared{};
    const auto prepare_status=runtime.prepare(bindings,prepared);
    require(prepare_status==native::Status::complete,
            (std::string("nonempty source Ghost skill preparation failed; status ")+
             std::to_string(static_cast<int>(prepare_status))+" source "+
             std::to_string(static_cast<int>(prepared.source_status))+" slots "+
             std::to_string(prepared.source.skill_slots)+" calls "+
             std::to_string(scripts.calls)+" loads "+std::to_string(scripts.loads)+" op "+
             std::to_string(prepared.source.last_operation)).c_str());
    const auto& members=skills.skill_lists[list_index].members;
    require(runtime.skill_scripts().size()==members.size()&&
            prepared.source.skill_slots==members.size()&&prepared.source.script_allocations>0,
            "source nonempty skill vector or CharAISkillScript ownership was not retained");
    require(scripts.loads>0&&scripts.calls==scripts.declared_with_args+scripts.declared_without_args&&
            scripts.declared_with_args>0&&scripts.declared_without_args>0,
            "real preparation did not load skills and execute both DeclareSkill overloads on the callback VM");
    bool found=false;
    for(std::size_t slot=0;slot<members.size();++slot) {
        const auto row_index=members[slot];
        if(row_index<0||static_cast<std::size_t>(row_index)>=skills.skills.size())continue;
        const auto& row=skills.skills[static_cast<std::size_t>(row_index)];
        if(row.script_length==0)continue;
        const auto identity=runtime.skill_scripts()[slot];
        const auto* args=runtime.arguments_for(identity);
        require(identity&&args&&args->script_name==row.script&&args->skill_index==static_cast<float>(slot),
                "retained CharAISkillScript Arguments differ from source name/slot inputs");
        found=true;break;
    }
    require(found,"source list did not retain a nonnull skill script instance");
}
} // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 14) throw std::runtime_error(
            "expected six Skill/Faery table files, three Character files, two constants files, debug seed, output directory");
        std::array<std::vector<std::uint8_t>, 12> input;
        for (std::size_t i = 0; i < input.size(); ++i) input[i] = read_file(argv[i + 1]);

        data::SkillTables skills;
        data::FaeryTables faeries;
        data::CharacterTable characters;
        data::PropertyRules rules;
        std::string error;
        require(data::load_skill_tables(bytes(input[0]), bytes(input[1]), bytes(input[2]), skills, error), error.c_str());
        require(data::load_faery_tables(bytes(input[3]), bytes(input[4]), bytes(input[5]), faeries, error), error.c_str());
        require(data::load_characters(bytes(input[6]), bytes(input[7]), bytes(input[8]), characters, error), error.c_str());
        require(data::load_property_rules(characters, rules, error), error.c_str());
        require(skills.skill_lists.size() == 36 && skills.skills.size() == 127 &&
                faeries.faery_lists.size() == 4 && faeries.faeries.size() == 16,
                "real Skill/Faery table dimensions changed");
        require(skills.skill_lists[3].name == "DEFAULT" && skills.skill_lists[3].members.empty(),
                "source default SkillList row changed");
        require(faeries.faery_lists[0].name == "DEFAULT" &&
                faeries.faery_lists[0].members == std::vector<std::int32_t>{2, 4, 5, 6, 3},
                "source default FaeryList row changed");
        const std::array<const char*, 5> fake_names{
            "Fake_Celest", "Fake_Rocky", "Fake_Wetty", "Fake_Windy", "Fake_Hotty"};
        for (std::size_t i = 0; i < fake_names.size(); ++i) {
            const auto index = static_cast<std::size_t>(faeries.faery_lists[0].members[i]);
            require(index < faeries.faeries.size() && faeries.faeries[index].table_name == fake_names[i] &&
                    faeries.faeries[index].spell_script_length == 0 &&
                    faeries.faeries[index].spell_script.empty(),
                    "authored fake-faery source null gate changed");
        }

        dh2_pycst_view constants{};
        require(dh2_pycst_open(&constants, input[9].data(),
                               static_cast<std::uint32_t>(input[9].size())) == 0,
                "Faery constants cache did not parse");
        dh2_pycst_result count{};
        require(dh2_pycst_get(&constants, "FaeryTypes", 10, "COUNT", 5, &count) == 0 &&
                count.found && count.value == 5,
                "source FaeryTypes/COUNT value changed");
        dh2_pycst_view animation_constants{};
        require(dh2_pycst_open(&animation_constants, input[10].data(),
                               static_cast<std::uint32_t>(input[10].size())) == 0,
                "animation constants cache did not parse");
        dh2_pycst_result stance_mask{}, stance_count{};
        require(dh2_pycst_get(&animation_constants, "AnimStancedAnim", 15,
                              "SL__LIST_IPHONE", 15, &stance_mask) == 0 &&
                stance_mask.found && stance_mask.value == 210 &&
                dh2_pycst_get(&animation_constants, "AnimStances", 11,
                              "COUNT_IPHONE", 12, &stance_count) == 0 &&
                stance_count.found && stance_count.value == 5,
                "authored animation stance constants changed");

        const auto debug_dir = std::filesystem::absolute(argv[13]);
        std::filesystem::create_directories(debug_dir);
        dh2::native::debug_files::Backend debug;
        require(debug.initialize(debug_dir, input[11].data(), input[11].size(), error), error.c_str());
        const auto ghost = row_id(characters, "Crypt_Ghost");
        const auto ghost_re = row_id(characters, "Crypt_Ghost_RE");
        const auto skill_tree = property_id(characters, "SkillTree");
        const auto faery_list = property_id(characters, "FaeryList");
        require(skill_tree == 28 && faery_list == 29,
                "CharacterProperties property identifiers changed");

        run_actor("Crypt_Ghost", ghost, characters, rules, skills, faeries, constants,
                  animation_constants, debug);
        run_actor("Crypt_Ghost_RE", ghost_re, characters, rules, skills, faeries, constants,
                  animation_constants, debug);
        run_nonempty_skill_case(characters,rules,skills,faeries,constants,debug,ghost);
        std::cout << "{\"validation\":\"PASS\",\"cache_tables\":\"real\","
                     "\"actors\":2,\"source_vectors\":\"0 skills / 5 null faeries\","
                     "\"repeat_preserves_faeries\":true,\"unsupported_nonnull_script\":\"fail-closed\"}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
