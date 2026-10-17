#include "../quest_reward_give_xp_bridge_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace data = dh2::data;
namespace bridge = dh2::quest_reward_give_xp_bridge_v1;
namespace xp = dh2::character_give_xp_v1;
namespace dispatch = dh2::character_distribute_give_xp_dispatch_v1;
namespace {
constexpr std::uintptr_t character_id = 0x5150a1;

void check(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "FAIL quest_reward_give_xp_bridge_v1: %s\n", message);
        std::exit(1);
    }
}
void word(std::vector<unsigned char>& out, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i)
        out.push_back(static_cast<unsigned char>(value >> (i * 8)));
}
void string(std::vector<unsigned char>& out, const char* value) {
    const auto size = std::char_traits<char>::length(value);
    word(out, static_cast<std::uint32_t>(size));
    out.insert(out.end(), value, value + size);
}
struct Fixture {
    data::PropertyRules rules{};
    data::PropertyState state{};
    data::PropertyView properties{};
    data::PlayerSavegameV1 save{};
    std::vector<unsigned char> constant_bytes;
    dh2_pycst_view constants{};
    std::uint32_t invokes = 0;
    bool suppress = false;

    Fixture() {
        rules.defaults.fill(0);
        rules.types.fill(0);
        rules.types[19] = 32;
        rules.types[33] = 32;
        rules.types[34] = 16;
        rules.types[200] = 4;
        data::reset_properties(rules, state);
        state.saved[19] = 10 * 256;
        state.saved[33] = 100 * 256;
        state.base[34] = 1000 * 256;
        std::string error;
        check(data::recalc_properties(rules, state, error), "property view setup failed");
        properties = data::property_view(rules, state);
        save.set_character(character_id);
        save.set_unlocked_difficulty(0);
        word(constant_bytes, 1);
        string(constant_bytes, "CharacterDesign");
        word(constant_bytes, 1);
        string(constant_bytes, "MaxLevelBNormal");
        word(constant_bytes, 50);
        check(!dh2_pycst_open(&constants, constant_bytes.data(),
                              static_cast<std::uint32_t>(constant_bytes.size())),
              "PyData constant setup failed");
    }

    static std::int32_t invoke(void* raw, const xp::Request* request,
                               xp::Reply* reply, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        if (!request || !reply || request->character != character_id ||
            request->properties != &self.properties || request->save != &self.save) {
            error = "_GiveXP crossed the canonical test owners";
            return 1;
        }
        ++self.invokes;
        if (request->operation == xp::Operation::current_level_suppression)
            reply->word = self.suppress ? 1u : 0u;
        else if (request->operation == xp::Operation::character_virtual_40)
            reply->value = 1;
        else if (request->operation == xp::Operation::character_virtual_84 ||
                 request->operation == xp::Operation::one_kill_level_up ||
                 request->operation == xp::Operation::trace_character_stats)
            reply->value = 0;
        else if (request->operation == xp::Operation::current_level_difficulty)
            reply->value = 0;
        else if (request->operation == xp::Operation::player_by_character)
            reply->value = 9;
        else {
            error = "unexpected _GiveXP operation in threshold-free test";
            return 1;
        }
        error.clear();
        return 0;
    }
};
}

int main() {
    Fixture fixture;
    xp::Backend backend{};
    backend.context = &fixture;
    backend.invoke = Fixture::invoke;
    dispatch::Owner xp_owner{&fixture.constants, backend};
    dh2::character_distribute_xp_v1::CharacterView view{
        character_id, &fixture.properties, &fixture.save, 0.0f, 0.0f};
    std::int32_t receipt = 0;
    data::quest_reward_execution_v1::CharacterRef reward_character{
        character_id, nullptr, nullptr, &receipt};
    data::quest_reward_execution_v1::Record reward(0x100);
    bridge::Context context{&xp_owner, &view, character_id};
    bool granted = false;

    check(bridge::give_xp(&context, reward, reward_character, 3 * 256, 1,
                          &granted) == 0,
          "canonical Reward XP callback failed");
    check(granted && context.last_source_return == 1 && receipt == 0 &&
          fixture.state.saved[33] == 103 * 256 && context.calls == 1,
          "bridge did not reuse _GiveXP or wrote a duplicate Reward receipt");

    fixture.suppress = true;
    granted = true;
    const auto previous_receipt = receipt;
    const auto previous_xp = fixture.state.saved[33];
    check(bridge::give_xp(&context, reward, reward_character, 7 * 256, 1,
                          &granted) == 0,
          "source-suppressed _GiveXP should still complete Reward::Give");
    check(!granted && receipt == previous_receipt &&
          fixture.state.saved[33] == previous_xp && context.calls == 2,
          "_GiveXP false result did not suppress the Reward receipt only");

    check(bridge::give_xp(&context, reward, reward_character, 256, 0,
                          &granted) != 0 && context.calls == 3,
          "bridge accepted non-source a3 flag");
    std::puts("PASS quest_reward_give_xp_bridge_v1 3 checks");
    return 0;
}
