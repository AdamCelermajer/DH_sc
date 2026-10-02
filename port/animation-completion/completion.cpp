#include "completion.hpp"
using namespace dh2::completion;
extern "C" Error dh2_completion_set(State *state, Callback callback, void *context) {
    if (!state) return Error::argument;
    state->context = context;
    state->callback = callback;
    return Error::ok;
}
extern "C" Error dh2_completion_check(State *state, std::uintptr_t timeline) {
    if (!state) return Error::argument;
    if (state->pending > 255) return Error::range;
    if (state->pending && state->callback) {
        state->callback(timeline, state->context);
        state->pending = 0;
    }
    return Error::ok;
}
