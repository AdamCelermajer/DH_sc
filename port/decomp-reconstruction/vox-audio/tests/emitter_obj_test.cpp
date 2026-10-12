// EmitterObj accessors: gain modifier, ready flag, group mask, state and user data.
#include <cstdint>
#include <cstdio>

#include "vox-audio/vox/emitter_obj.hpp"

namespace {
int g_failures = 0;
#define CHECK(cond)                                                       \
    do {                                                                  \
        if (!(cond)) {                                                    \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__,   \
                         __LINE__, #cond);                                \
            ++g_failures;                                                 \
        }                                                                 \
    } while (0)
}  // namespace

int main() {
    vox::EmitterObj e;

    e.SetGainModifier(0.5f);
    CHECK(e.gain_modifier_ == 0.5f);

    // IsReady is the inverted busy byte at +0x1c.
    e.ready_flag_ = 0;
    CHECK(e.IsReady());
    e.ready_flag_ = 1;
    CHECK(!e.IsReady());

    e.SetGroup(3);
    CHECK(e.GetGroup() == 3);
    CHECK(e.IsGroup(1u << 3));
    CHECK(!e.IsGroup(1u << 2));
    CHECK(e.IsGroup((1u << 3) | 1u));

    e.SetState(7);
    CHECK(e.state_set_ == 7);   // SetState writes +0x94
    CHECK(e.GetState() == 0);   // GetState reads +0x90, as the original does
    e.state_ = 5;
    CHECK(e.GetState() == 5);

    e.SetLoop(true);
    CHECK(e.loop_ == 1);        // SetLoop writes +0x8d
    CHECK(!e.GetLoop());        // GetLoop reads +0x8c, as the original does
    e.loop_read_ = 1;
    CHECK(e.GetLoop());

    e.Reset();
    CHECK(e.reset_flag_ == 1);

    std::uintptr_t word = 0xabcd;
    e.SetUserData(word);
    CHECK(e.user_data_ == 0xabcd);

    std::printf("emitter_obj_test: %s\n", g_failures == 0 ? "ok" : "FAILED");
    return g_failures == 0 ? 0 : 1;
}
