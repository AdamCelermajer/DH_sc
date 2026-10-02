#pragma once
#include <cstdint>
namespace dh2::completion {
using Callback = void (*)(std::uintptr_t timeline, void *context);
struct State { Callback callback; void *context; std::uint32_t pending; };
enum class Error : std::uint32_t { ok, argument, range };
}
extern "C" {
// Portable AnimApplicator callback fields; borrowed callback/context/state.
// A callback must return normally and keep State alive throughout the call.
dh2::completion::Error dh2_completion_set(dh2::completion::State *,
                                        dh2::completion::Callback, void *context);
// A nonzero pending byte with a callback dispatches timeline/context, then
// clears pending AFTER the callback. With no callback, pending is retained.
// Reentrant callback field/context/pending changes are allowed; final pending
// is still cleared. No thread synchronization or game action implementation.
dh2::completion::Error dh2_completion_check(dh2::completion::State *, std::uintptr_t timeline);
}
