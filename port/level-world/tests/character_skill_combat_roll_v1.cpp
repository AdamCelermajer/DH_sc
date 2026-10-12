#include "../character_skill_combat_roll_v1.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace k = dh2::character_skill_combat_roll_v1;
namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
struct Fixture {
    std::vector<std::string> calls;
    std::uint32_t skill_type = 0;
    std::uint32_t flags = 0;
    std::int32_t skill_element = 4;
    std::uintptr_t target = 0x2000;
    std::uint32_t unresolved_tag = 0;
    bool has_main = true;
    bool has_off = true;
    int fail_at = 0;
    int attack_number = 0;

    static int metadata(void* raw, std::uintptr_t character, std::uint32_t index,
                        std::uint32_t* type, std::uint32_t* flags,
                        std::int32_t* element,
                        std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("skill");
        if (character != 0x1000 || index != 2) return error = "skill args", 1;
        if (f.fail_at == 1) return error = "skill", 1;
        *type = f.skill_type; *flags = f.flags; *element = f.skill_element; return 0;
    }
    static int resolve(void* raw, std::uintptr_t userdata, std::uintptr_t* target,
                       std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("resolve");
        if (userdata != 0x3000) return error = "target", 1;
        if (f.fail_at == 2) return error = "resolve", 1;
        *target = f.target; return 0;
    }
    static int main_hand(void* raw, std::uintptr_t character, bool* present,
                        std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("main");
        if (character != 0x1000) return error = "main args", 1;
        *present = f.has_main; return 0;
    }
    static int off_hand(void* raw, std::uintptr_t character, bool* present,
                        std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("off");
        if (character != 0x1000) return error = "off args", 1;
        *present = f.has_off; return 0;
    }
    static int read_tag(void* raw, std::uintptr_t userdata, std::uint32_t* tag,
                        std::string&) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("tag");
        if (userdata != 0x3000) return 1;
        *tag = f.unresolved_tag; return 0;
    }
    static int consume(void* raw, std::uintptr_t, std::string&) {
        static_cast<Fixture*>(raw)->calls.push_back("consume"); return 0;
    }
    static int attack(void* raw, std::uintptr_t attacker, std::uintptr_t target,
                      std::uint32_t flags, std::int32_t element,
                      dh2::data::CombatResult* result, std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("attack");
        if (attacker != 0x1000 || target != f.target ||
            element != f.skill_element) return error = "attack args", 1;
        const auto expected = f.flags | (f.attack_number ? k::offhand_attack_flag : 0u);
        if (flags != expected) return error = "attack flags", 1;
        if (f.fail_at == 3) return error = "attack", 1;
        result->amount = 51 + f.attack_number++;
        return 0;
    }
    static int apply(void* raw, std::uintptr_t attacker, std::uintptr_t target,
                     std::uint32_t flag, dh2::data::CombatResult* result,
                     std::string& error) {
        auto& f = *static_cast<Fixture*>(raw); f.calls.push_back("apply");
        if (attacker != 0x1000 || target != f.target || flag != 0 ||
            result->amount != 51 + f.attack_number - 1)
            return error = "apply args", 1;
        if (f.fail_at == 4) return error = "apply", 1;
        return 0;
    }
    k::Services services() {
        return {this, metadata, resolve, main_hand, off_hand, read_tag, consume,
                attack, apply};
    }
};
}

int main() {
    Fixture f;
    auto services = f.services();
    k::Request request{0x1000, 2, 2, 6, k::userdata_type, 0x3000};
    k::Result result{};
    std::string error;

    check(k::execute(&request, &services, &result, error) == k::Status::complete &&
          result.disposition == k::Disposition::attacks && result.attack_count == 1 &&
          result.attacks[0].flags == 0 && result.attacks[0].amount == 51 &&
          f.calls == std::vector<std::string>{"resolve", "skill", "attack", "apply"},
          "normal Character::_SkillCombatRoll order/result differs");

    // The source reads no skill type: any table type (BashDown is 0) rolls.
    f.calls.clear(); f.skill_type = 3; f.attack_number = 0;
    check(k::execute(&request, &services, &result, error) == k::Status::complete &&
          f.calls == std::vector<std::string>{"resolve", "skill", "attack", "apply"},
          "skill table type must not gate Character::_SkillCombatRoll");
    f.skill_type = 0;

    f.calls.clear(); f.flags = k::dual_wield_flag; f.attack_number = 0;
    check(k::execute(&request, &services, &result, error) == k::Status::complete &&
          result.attack_count == 2 && result.attacks[0].flags == k::dual_wield_flag &&
          result.attacks[1].flags == (k::dual_wield_flag | k::offhand_attack_flag) &&
          result.attacks[0].amount == 51 && result.attacks[1].amount == 52 &&
          f.calls == std::vector<std::string>{"resolve", "skill", "main", "attack",
              "apply", "off", "attack", "apply"},
          "dual-wield main/offhand order or post-application amounts differ");

    f.calls.clear(); f.has_main = false; f.has_off = false;
    check(k::execute(&request, &services, &result, error) == k::Status::complete &&
          result.disposition == k::Disposition::unchanged && !result.attack_count &&
          f.calls == std::vector<std::string>{"resolve", "skill", "main", "off"},
          "empty dual-wield skill should produce no result");

    f.calls.clear(); f.target = 0; f.unresolved_tag = k::false_value_tag;
    check(k::execute(&request, &services, &result, error) == k::Status::complete &&
          result.disposition == k::Disposition::boolean_false &&
          f.calls == std::vector<std::string>{"resolve", "tag", "consume"},
          "unresolved type-8 target path differs");

    f.calls.clear(); request.argument_count = 1; f.target = 0x2000;
    check(k::execute(&request, &services, &result, error) == k::Status::complete &&
          f.calls.empty(), "source argument-count gate invoked providers");

    f.calls.clear(); request.argument_count = 2; f.flags = 0;
    f.attack_number = 0; f.fail_at = 4;
    check(k::execute(&request, &services, &result, error) == k::Status::provider_failed &&
          error == "apply" && result.attack_count == 0 &&
          f.calls == std::vector<std::string>{"resolve", "skill", "attack", "apply"},
          "caller must stop at a reached F_ApplyResult failure");
    return 0;
}
