#include "../character_spell_combat_roll_v1.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::character_spell_combat_roll_v1;
namespace {
void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
struct Fixture {
    std::vector<std::string> calls;
    int fail_at = 0;
    std::uintptr_t target = 0x9000;
    std::uint32_t fallback_tag = 0;
    static int current(void* raw, std::uintptr_t character, std::int32_t* slot, std::string& e) {
        auto& f=*static_cast<Fixture*>(raw); f.calls.push_back("current");
        if (character != 0x1000) { e="character"; return 1; }
        if (f.fail_at == 1) { e="current"; return 1; }
        *slot = 3; return 0;
    }
    static int element(void* raw, std::uintptr_t character, std::int32_t slot, std::int32_t* value, std::string& e) {
        auto& f=*static_cast<Fixture*>(raw); f.calls.push_back("element");
        if (character != 0x1000 || slot != 3) { e="faery"; return 1; }
        if (f.fail_at == 2) { e="element"; return 1; }
        *value = 7; return 0;
    }
    static int resolve(void* raw, std::uintptr_t userdata, std::uintptr_t* value, std::string& e) {
        auto& f=*static_cast<Fixture*>(raw); f.calls.push_back("resolve");
        if (userdata != 0x2000) { e="userdata"; return 1; }
        if (f.fail_at == 3) { e="resolve"; return 1; }
        *value = f.target; return 0;
    }
    static int read_tag(void* raw, std::uintptr_t, std::uint32_t* value, std::string&) {
        auto& f=*static_cast<Fixture*>(raw); f.calls.push_back("read_tag"); *value = f.fallback_tag; return 0;
    }
    static int consume(void* raw, std::uintptr_t, std::string&) { static_cast<Fixture*>(raw)->calls.push_back("consume_false"); return 0; }
    static int attack(void* raw, std::uintptr_t attacker, std::uintptr_t defender, std::int32_t element_value,
               std::uint32_t mask, std::int32_t category, std::int32_t amount,
               dh2::data::CombatResult* result, std::string& e) {
        auto& f=*static_cast<Fixture*>(raw); f.calls.push_back("attack");
        if (attacker != 0x1000 || defender != f.target || element_value != 7 ||
            mask != k::spell_attack_mask || category != -1 || amount != 0)
            return e = "attack args", 1;
        if (f.fail_at == 4) { e="attack"; return 1; }
        result->amount = 51; result->outcomes = 8; result->element = element_value; return 0;
    }
    static int apply(void* raw, std::uintptr_t attacker, std::uintptr_t defender, std::uint32_t source_flag,
              dh2::data::CombatResult* result, std::string& e) {
        auto& f=*static_cast<Fixture*>(raw); f.calls.push_back("apply");
        if (attacker != 0x1000 || defender != f.target || source_flag != 0 || result->amount != 51) {
            e="apply args"; return 1;
        }
        if (f.fail_at == 5) { e="apply"; return 1; }
        return 0;
    }
    k::Services services() { return {this,current,element,resolve,read_tag,consume,attack,apply}; }
};
}
int main() {
    Fixture f; auto services=f.services(); k::Request request{0x1000,1,k::userdata_type,0x2000};
    k::Result result{}; std::string error;
    check(k::execute(&request,&services,&result,error)==k::Status::complete &&
          result.disposition==k::Disposition::integer && result.amount==51 && result.element==7 &&
          result.faery_slot==3 && f.calls==std::vector<std::string>{"current","resolve","element","attack","apply"},
          "Character::_SpellCombatRoll hit order/result differs");

    f.calls.clear(); f.target=0; f.fallback_tag=k::false_value_tag;
    check(k::execute(&request,&services,&result,error)==k::Status::complete &&
          result.disposition==k::Disposition::boolean_false &&
          f.calls==std::vector<std::string>{"current","resolve","read_tag","consume_false"},
          "Character::_SpellCombatRoll byte8 fallback differs");

    f.calls.clear(); f.fallback_tag=0;
    check(k::execute(&request,&services,&result,error)==k::Status::complete &&
          result.disposition==k::Disposition::unchanged &&
          f.calls==std::vector<std::string>{"current","resolve","read_tag"},
          "Character::_SpellCombatRoll unresolved non-byte8 result differs");

    f.calls.clear(); request.argument_type=3;
    check(k::execute(&request,&services,&result,error)==k::Status::complete &&
          result.disposition==k::Disposition::unchanged && f.calls==std::vector<std::string>{"current"},
          "Character::_SpellCombatRoll accepted an unsupported Lua argument");

    f.calls.clear(); request.argument_type=k::object_type; f.target=0x9000; f.fail_at=4;
    check(k::execute(&request,&services,&result,error)==k::Status::provider_failed &&
          error=="attack" && f.calls==std::vector<std::string>{"current","resolve","element","attack"},
          "Character::_SpellCombatRoll did not stop at failed F_SpellAttack");
    return 0;
}
