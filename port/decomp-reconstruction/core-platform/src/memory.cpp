#include "core-platform/memory.hpp"

#include <cstdio>
#include <cstdlib>
#include <mutex>

int gAssertLevel = 0;

namespace {
// The original guards its unit heap with a CMutex (s_mem_mutex).
std::mutex s_mem_mutex;

// Shared malloc path with the original's out-of-memory handling.
void* MallocOrReport(std::size_t size) {
    void* p = std::malloc(size);
    if (p == nullptr) {
        ShowMemoryStats("Out of memory:");
        if (gAssertLevel == 2) {
            // The original writes to address 0 here to force a crash.
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT FAILED: out of memory\n");
        }
    }
    return p;
}
}  // namespace

// 0x310414 InitMemory
void InitMemory() {}

// 0x310418 UnInitMem
void UnInitMem() {}

// 0x31041c ShowMemoryStats(char const*)
void ShowMemoryStats(const char*) {}

// 0x317bb0 UnInitUnitMemory
void UnInitUnitMemory() {}

// 0x317bb4 ShowUnitHeapMemoryStats(char const*)
void ShowUnitHeapMemoryStats(const char*) {}

// 0x31044c _Z7SwfFreePv
void SwfFree(void* p) { std::free(p); }

// 0x310450 _Z10GlitchFreePv
void GlitchFree(void* p) { std::free(p); }

// 0x310564 _Z8SwfAllocjN3swf7MemHintE
void* SwfAlloc(unsigned size, swf::MemHint) { return MallocOrReport(size); }

// 0x310698 _Z8SwfAllocjN3swf7MemHintEPKci
void* SwfAlloc(unsigned size, swf::MemHint, const char* file, int line) {
    return CustomAlloc(size, file, static_cast<unsigned>(line), nullptr);
}

// 0x310568 _Z11GlitchAllocjN6glitch6memory13E_MEMORY_HINTE
void* GlitchAlloc(unsigned size, glitch::memory::E_MEMORY_HINT) { return MallocOrReport(size); }

// 0x3106a8 _Z11GlitchAllocjN6glitch6memory13E_MEMORY_HINTEiPKci
void* GlitchAlloc(unsigned size, glitch::memory::E_MEMORY_HINT, int, const char* file, int line) {
    return CustomAlloc(size, file, static_cast<unsigned>(line), nullptr);
}

// 0x3106b8 _Z11GlitchAllocjN6glitch6memory13E_MEMORY_HINTEiPKciS3_
void* GlitchAlloc(unsigned size, glitch::memory::E_MEMORY_HINT, int, const char* file, int line,
                  const char* func) {
    return CustomAlloc(size, file, static_cast<unsigned>(line), func);
}

// 0x310660 _Z11CustomAllocjPKcjS0_
void* CustomAlloc(unsigned size, const char*, unsigned, const char*) { return MallocOrReport(size); }

// 0x317c94 _Z15internal_mallocjPKcjS0_
void* internal_malloc(unsigned size, const char*, unsigned, const char*) { return std::malloc(size); }

// 0x317bd8 _Z13internal_freePv
void internal_free(void* p) { std::free(p); }

// 0x317bc8 _Z15LockMemoryMutexv
void LockMemoryMutex() { s_mem_mutex.lock(); }

// 0x317bb8 _Z17UnlockMemoryMutexv
void UnlockMemoryMutex() { s_mem_mutex.unlock(); }

// 0x317bdc _Z7FreeMemPv
void FreeMem(void* p) {
    if (p != nullptr) {
        LockMemoryMutex();
        internal_free(p);
        UnlockMemoryMutex();
    }
}

// Tagged operator new/delete overloads. Hint-tagged forms that the original
// forwards to CustomAlloc/malloc keep that behaviour; the extra parameters are
// accepted and ignored by the malloc path, as in the original.

// 0x5341ac _ZnwjN6glitch6memory13E_MEMORY_HINTE
void* operator new(std::size_t size, glitch::memory::E_MEMORY_HINT hint) { return GlitchAlloc(static_cast<unsigned>(size), hint); }

// 0x5341a8 _ZnajN6glitch6memory13E_MEMORY_HINTE
void* operator new[](std::size_t size, glitch::memory::E_MEMORY_HINT hint) { return GlitchAlloc(static_cast<unsigned>(size), hint); }

// 0x53419c _ZnwjN6glitch6memory13E_MEMORY_HINTEiPKci
void* operator new(std::size_t size, glitch::memory::E_MEMORY_HINT, int, const char* file, int line) {
    return CustomAlloc(static_cast<unsigned>(size), file, static_cast<unsigned>(line), nullptr);
}

// 0x534198 _ZnajN6glitch6memory13E_MEMORY_HINTEiPKci
void* operator new[](std::size_t size, glitch::memory::E_MEMORY_HINT, int, const char* file, int line) {
    return CustomAlloc(static_cast<unsigned>(size), file, static_cast<unsigned>(line), nullptr);
}

// 0x534194 _ZdlPvN6glitch6memory13E_MEMORY_HINTE
void operator delete(void* p, glitch::memory::E_MEMORY_HINT) { std::free(p); }

// 0x534190 _ZdaPvN6glitch6memory13E_MEMORY_HINTE
void operator delete[](void* p, glitch::memory::E_MEMORY_HINT) { std::free(p); }

// 0x53417c _ZdlPvN6glitch6memory13E_MEMORY_HINTEiPKci
void operator delete(void* p, glitch::memory::E_MEMORY_HINT, int, const char*, int) { std::free(p); }

// 0x534178 _ZdaPvN6glitch6memory13E_MEMORY_HINTEiPKci
void operator delete[](void* p, glitch::memory::E_MEMORY_HINT, int, const char*, int) { std::free(p); }

// 0x3106d4 _Znwj15MemoryHintStatePKcjS1_
void* operator new(std::size_t size, MemoryHintState, const char* file, unsigned line, const char* func) {
    return CustomAlloc(static_cast<unsigned>(size), file, line, func);
}

// 0x3106c4 _Znaj15MemoryHintStatePKcjS1_
void* operator new[](std::size_t size, MemoryHintState, const char* file, unsigned line, const char* func) {
    return CustomAlloc(static_cast<unsigned>(size), file, line, func);
}

// 0x752ba8 _ZnwjN3swf7MemHintE
void* operator new(std::size_t size, swf::MemHint hint) { return SwfAlloc(static_cast<unsigned>(size), hint); }

// 0x752ba4 _ZnajN3swf7MemHintE
void* operator new[](std::size_t size, swf::MemHint hint) { return SwfAlloc(static_cast<unsigned>(size), hint); }

// 0x752b48 _ZnwjN3swf7MemHintEPKci
// Partial: the decompiler dropped the forwarded arguments of this wrapper, so
// the forwarding is reconstructed from the 4-argument SwfAlloc signature.
void* operator new(std::size_t size, swf::MemHint hint, const char* file, int line) {
    return SwfAlloc(static_cast<unsigned>(size), hint, file, line);
}

// 0x752b44 _ZnajN3swf7MemHintEPKci (same caveat as 0x752b48)
void* operator new[](std::size_t size, swf::MemHint hint, const char* file, int line) {
    return SwfAlloc(static_cast<unsigned>(size), hint, file, line);
}
