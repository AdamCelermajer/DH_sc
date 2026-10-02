#include "../completion.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
using namespace dh2::completion;
static std::uint32_t calls;
static void callback(std::uintptr_t timeline, void *context) {
    assert(timeline == 0x123456789abcdef0ULL);
    auto *state = static_cast<State *>(context);
    assert(state->pending);
    state->pending = 255;
    state->callback = nullptr;
    ++calls;
}
int main() {
    for (std::uint32_t i = 0; i < 5000; ++i) {
        State state{}; state.pending = i % 512;
        assert(dh2_completion_set(&state, callback, &state) == Error::ok);
        const auto before = state;
        const auto count = calls;
        const auto error = dh2_completion_check(&state, 0x123456789abcdef0ULL);
        if (state.pending > 255) {
            assert(error == Error::range && std::memcmp(&state, &before, sizeof(state)) == 0);
        } else if (before.pending) {
            assert(error == Error::ok && state.pending == 0 && !state.callback && calls == count + 1);
        } else assert(error == Error::ok && calls == count);
        State delayed{}; delayed.pending = i % 256;
        assert(dh2_completion_check(&delayed, 0) == Error::ok && delayed.pending == i % 256);
    }
    assert(dh2_completion_set(nullptr, nullptr, nullptr) == Error::argument);
    assert(dh2_completion_check(nullptr, 0) == Error::argument);
    std::puts("completion safety: 5000 callback/reentry/range checks and 5000 no-callback checks");
}
