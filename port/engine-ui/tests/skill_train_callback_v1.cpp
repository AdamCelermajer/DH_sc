#include "../skill_train_callback_v1.hpp"

#include <array>
#include <iostream>
#include <string>
#include <vector>

int main() {
    unsigned checks = 0;
    bool ok = true;
    const auto check = [&](bool value, const char* message) {
        if (!value) std::cerr << "FAIL: " << message << '\n';
        ok &= value;
        ++checks;
    };

    std::array<std::int32_t, 3> integers{};
    std::array<bool, 3> booleans{};
    unsigned integer_calls = 0, boolean_calls = 0;
    const auto to_integer = [&](std::size_t index, std::int32_t& out, std::string&) {
        ++integer_calls;
        out = integers[index];
        return true;
    };
    const auto to_boolean = [&](std::size_t index) {
        ++boolean_calls;
        return booleans[index];
    };
    dh2::ui::SkillTrainCallbackArgsV1 decoded;
    dh2::ui::SkillTrainCallbackResultV1 applied;
    dh2::ui::SkillSaveCallbackResultV1 saved;
    std::string error;

    integers = {7, 2, 0};
    check(dh2::ui::decode_skill_train_callback_v1(2, to_integer, to_boolean,
              decoded, error) && decoded.skill_index == 7 && decoded.player_index == 2 &&
              !decoded.test_only && integer_calls == 2 && boolean_calls == 0,
          "two-argument Talent click converts indices without reading a test flag");

    integers = {5, 0, 0};
    integer_calls = boolean_calls = 0;
    check(dh2::ui::decode_skill_train_callback_v1(2, to_integer, to_boolean,
              decoded, error) && decoded.skill_index == 5 && decoded.player_index == 0 &&
              !decoded.test_only && integer_calls == 2 && boolean_calls == 0,
          "SWF btn_add.onRelease arguments decode as [selected skill index, local PlayerInfo 0]");

    integers = {4, 1, 0};
    booleans = {false, false, true};
    integer_calls = boolean_calls = 0;
    check(dh2::ui::decode_skill_train_callback_v1(3, to_integer, to_boolean,
              decoded, error) && decoded.skill_index == 4 && decoded.player_index == 1 &&
              decoded.test_only && integer_calls == 2 && boolean_calls == 1,
          "three-argument source form coerces its optional test flag");

    integers = {-1, 0, 0};
    check(dh2::ui::decode_skill_train_callback_v1(2, to_integer, to_boolean,
              decoded, error) && decoded.skill_index == -1 && decoded.player_index == 0,
          "negative index reaches the callback's source no-op path");

    integer_calls = boolean_calls = 0;
    check(!dh2::ui::decode_skill_train_callback_v1(4, to_integer, to_boolean,
              decoded, error) && !error.empty() && integer_calls == 0 && boolean_calls == 0,
          "unsupported arity has no callback conversion side effects");

    // The original skill menu's NativeSaveGame route saves the one active
    // Player profile. Its native callback converts arg0 only for argc==1;
    // other arities select local PlayerInfo 0.
    std::vector<std::string> save_order;
    const auto resolve_save_player=[&](std::int32_t player,
            std::uintptr_t& character,std::string&){
        save_order.push_back("resolve:"+std::to_string(player));
        character=player==0?0x1234u:0u;
        return true;
    };
    const auto save_profile=[&](std::uintptr_t character,std::string&){
        save_order.push_back("save:"+std::to_string(character));
        return true;
    };
    const auto post_save=[&](std::uintptr_t character,std::string&){
        save_order.push_back("trophies:"+std::to_string(character));
        return true;
    };
    integers={0,2,0};integer_calls=0;save_order.clear();
    check(dh2::ui::apply_skill_save_callback_v1(1,to_integer,
              resolve_save_player,save_profile,post_save,saved,error)&&saved.player_found&&
              saved.save_called&&saved.post_save_called&&saved.player_index==0&&saved.character==0x1234u&&
              integer_calls==1&&save_order==std::vector<std::string>{"resolve:0","save:4660","trophies:4660"},
          "single-argument skill Save persists the canonical Player before source TrophyManager side effects");

    integers={7,2,0};integer_calls=0;save_order.clear();
    check(dh2::ui::apply_skill_save_callback_v1(3,to_integer,
              resolve_save_player,save_profile,post_save,saved,error)&&saved.player_found&&
              saved.save_called&&saved.post_save_called&&saved.player_index==0&&integer_calls==0&&
              save_order==std::vector<std::string>{"resolve:0","save:4660","trophies:4660"},
          "non-single-argument source Save callback defaults to PlayerInfo 0 without coercing extras");

    integers={9,0,0};integer_calls=0;save_order.clear();
    check(dh2::ui::apply_skill_save_callback_v1(1,to_integer,
              resolve_save_player,save_profile,post_save,saved,error)&&!saved.player_found&&
              !saved.save_called&&!saved.post_save_called&&integer_calls==1&&
              save_order==std::vector<std::string>{"resolve:9"},
          "missing/remote Player is a source no-op and never saves a different profile");

    const auto fail_save=[&](std::uintptr_t,std::string& failure){
        failure="profile write failed";return false;
    };
    integers={0,0,0};save_order.clear();
    check(!dh2::ui::apply_skill_save_callback_v1(0,to_integer,
              resolve_save_player,fail_save,post_save,saved,error)&&saved.player_found&&
              !saved.save_called&&!saved.post_save_called&&error=="profile write failed"&&
              save_order==std::vector<std::string>{"resolve:0"},
          "canonical Save transport failure is surfaced before menu Back proceeds");
    const auto fail_trophy_tail=[&](std::uintptr_t,std::string& failure){
        save_order.push_back("trophies-failed");
        failure="Trophy sidecar write failed";return false;
    };
    save_order.clear();
    check(!dh2::ui::apply_skill_save_callback_v1(0,to_integer,
              resolve_save_player,save_profile,fail_trophy_tail,saved,error)&&
              saved.save_called&&!saved.post_save_called&&error=="Trophy sidecar write failed"&&
              save_order==std::vector<std::string>{"resolve:0","save:4660","trophies-failed"},
          "NativeSaveGame retains the completed profile save and surfaces a later trophy-tail failure");

    std::vector<std::string> trophy_order;
    const auto unlock=[&](const char* name,std::string&){
        trophy_order.emplace_back(name);return true;
    };
    check(dh2::ui::apply_skill_save_trophy_effects_v1(
              {10,0,4},true,10,0,50,50,unlock,error)&&
              trophy_order==std::vector<std::string>{"epic_maxskill","epic_spent_all_skill_points"},
          "source save awards max-skill for every capped skill, but suppresses all-skills when one is zero");
    trophy_order.clear();
    check(dh2::ui::apply_skill_save_trophy_effects_v1(
              {10,4},true,10,0,50,50,unlock,error)&&
              trophy_order==std::vector<std::string>{"epic_maxskill","epic_all_skills_unlocked","epic_spent_all_skill_points"},
          "source save orders all-skills and spent-points unlocks after per-skill max unlocks");
    trophy_order.clear();
    check(dh2::ui::apply_skill_save_trophy_effects_v1(
              {},true,10,5,49,50,unlock,error)&&
              trophy_order==std::vector<std::string>{"epic_all_skills_unlocked"},
          "source empty-skill-list path retains its all-skills flag and applies the level/points gates");
    trophy_order.clear();
    check(dh2::ui::apply_skill_save_trophy_effects_v1(
              {10,10},false,10,0,50,50,unlock,error)&&
              trophy_order==std::vector<std::string>{"epic_maxskill","epic_maxskill"},
          "source max-skill trophies precede and remain independent of the Character IsPlayer gate");

    std::vector<std::string> order;
    std::uintptr_t resolved_character=0x1234;
    const auto resolve=[&](std::int32_t player,std::uintptr_t& character,
                           std::string&){
        order.push_back("resolve:"+std::to_string(player));
        character=resolved_character;return true;
    };
    const auto read_points=[&](std::uintptr_t character,std::int32_t& points,
                               std::string&){
        order.push_back("points:"+std::to_string(character));points=9;return true;
    };
    const auto train=[&](std::uintptr_t character,std::int32_t skill,
                         bool test,std::uint32_t& source_return,
                         std::int32_t& points,std::string&){
        order.push_back("train:"+std::to_string(character)+":"+
                        std::to_string(skill)+":"+(test?"1":"0"));
        source_return=test?1u:0u;points=8;return true;
    };
    integers={6,0,0};booleans={false,false,true};
    integer_calls=boolean_calls=0;order.clear();
    check(dh2::ui::apply_skill_train_callback_v1(3,to_integer,to_boolean,
              resolve,read_points,train,applied,error)&&applied.player_found&&
              applied.training_called&&applied.has_result&&applied.test_only&&
              applied.character==0x1234&&applied.skill_index==6&&
              applied.source_return==1&&applied.points_left==8&&
              order==std::vector<std::string>{"resolve:0","train:4660:6:1"},
          "authored Talent click resolves Player then calls the canonical train provider with testOnly");

    integers={-1,0,0};integer_calls=boolean_calls=0;order.clear();
    check(dh2::ui::apply_skill_train_callback_v1(2,to_integer,to_boolean,
              resolve,read_points,train,applied,error)&&applied.player_found&&
              applied.has_result&&!applied.training_called&&
              applied.skill_index==-1&&applied.points_left==9&&
              order==std::vector<std::string>{"resolve:0","points:4660"},
          "negative source index returns current points without mutating Save/V4");

    integers={4,2,0};resolved_character=0;integer_calls=boolean_calls=0;order.clear();
    check(dh2::ui::apply_skill_train_callback_v1(2,to_integer,to_boolean,
              resolve,read_points,train,applied,error)&&!applied.player_found&&
              !applied.has_result&&order==std::vector<std::string>{"resolve:2"},
          "missing Player preserves source no-result path and never reaches Save/V4 providers");

    std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
              << "\",\"checks\":" << checks << "}\n";
    return ok ? 0 : 1;
}
