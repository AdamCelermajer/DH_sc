// Stand-ins for engine services used by the session layer. Each one names the
// original symbol it replaces.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>

// _Znaj15MemoryHintState: operator new[] with an allocator hint.
inline void* NewArrayWithHint(std::size_t bytes, int /*hint*/) { return std::malloc(bytes); }

// _Z10CustomFreePv: releases memory obtained from the engine allocator.
inline void CustomFree(void* p) { std::free(p); }

// _Z9GetOnlinev()->+0x28: the online clock in milliseconds. The owning
// COnline object lives in another category; tests drive the clock directly.
inline std::int32_t& OnlineClockStorage() {
    static std::int32_t clockMs = 0;
    return clockMs;
}
inline std::int32_t OnlineNowMs() { return OnlineClockStorage(); }
