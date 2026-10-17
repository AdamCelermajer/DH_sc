#include "../skill_equip_callback_v1.hpp"

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

    std::array<std::int32_t, 3> values{};
    unsigned calls = 0;
    const auto to_integer = [&](std::size_t index, std::int32_t& out,
                                std::string&) {
        ++calls;
        out = values[index];
        return true;
    };
    std::string error;
    std::vector<std::string> order;
    std::uintptr_t resolved_character = 0x1234;
    std::int32_t equipped_slot = -9, equipped_skill = -9;
    const auto resolve = [&](std::int32_t player, std::uintptr_t& character,
                             std::string&) {
        order.push_back("resolve:" + std::to_string(player));
        character = resolved_character;
        return true;
    };
    const auto equip = [&](std::uintptr_t character, std::int32_t slot,
                           std::int32_t skill, std::string&) {
        order.push_back("equip:" + std::to_string(character));
        equipped_slot = slot;
        equipped_skill = skill;
        return true;
    };

    values = {2, 7, 0};
    order.clear();calls = 0;
    check(dh2::ui::apply_skill_equip_callback_v1(3,to_integer,resolve,equip,
              error) && calls == 3 && equipped_slot == 2 && equipped_skill == 7 &&
              order == std::vector<std::string>{"resolve:0","equip:4660"},
          "authored [slot, skill, player] click resolves Player before the canonical equip provider");

    values = {-1, -1, 4};
    order.clear();calls = 0;
    check(dh2::ui::apply_skill_equip_callback_v1(3,to_integer,resolve,equip,
              error) && calls == 3 &&
              order == std::vector<std::string>{"resolve:4"},
          "negative skill follows source Player lookup and stops before Save mutation");

    values = {2, 7, 0};
    order.clear();calls = 0;resolved_character = 0;
    check(dh2::ui::apply_skill_equip_callback_v1(3,to_integer,resolve,equip,
              error) && calls == 3 && order == std::vector<std::string>{"resolve:0"},
          "missing Player Character exits before skill eligibility and Save mutation");

    calls = 0;
    order.clear();
    check(!dh2::ui::apply_skill_equip_callback_v1(2,to_integer,resolve,equip,
              error) && !error.empty() && calls == 0 && order.empty(),
          "wrong arity rejects before any callback argument conversion");

    values = {0, 3, 0};
    calls = 0;resolved_character = 0x1234;order.clear();
    const auto failing_integer = [&](std::size_t index, std::int32_t& out,
                                     std::string& failure) {
        ++calls;
        if (index == 1) { failure = "bad skill value"; return false; }
        out = values[index];
        return true;
    };
    check(!dh2::ui::apply_skill_equip_callback_v1(3,failing_integer,resolve,
              equip,error) && calls == 2 && error == "bad skill value" &&
              order.empty(),
          "conversion failure stops before player resolution and mutation");

    std::cout << "{\"validation\":\"" << (ok ? "PASS" : "FAIL")
              << "\",\"checks\":" << checks << "}\n";
    return ok ? 0 : 1;
}
