#include "glitch-core-io/core/process_buffer.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "glitch-core-io/os/logging.hpp"

namespace glitch::core {

namespace {

// The heap object (20 bytes in the original). A block is a run of 32-bit words: its first
// word is the header and its last word the footer. A positive value is an allocated block
// of that many words (header included); a negative value is a free block. The raw
// allocation starts with a zero word, so start is one word past it.
struct CProcessBufferHeap {
    int* start = nullptr;        // +0x00: first block word
    int* end = nullptr;          // +0x04: bound (start plus the data words)
    int* top = nullptr;          // +0x08: first unused word
    int defaultSize = 0;         // +0x0c: size the heap is created with by allocProcessBuffer
    bool excessEnabled = false;  // +0x10: allocations may fall back to malloc when full

    // 0x53427c: 0 when the heap now has the requested size, 1 when blocks are in use and
    // force is clear (nothing changes), 2 when the new storage could not be allocated.
    int setSize(int bytes, bool force);
    // 0x53434c: writes each allocated block to the logger.
    void dump(glitch::os::ILogger* logger, glitch::os::ELOG_LEVEL level) const;
    // 0x53443c (D1): releases the storage; reports blocks that are still in use.
    ~CProcessBufferHeap();
};

CProcessBufferHeap heap;

}  // namespace

// 0x53427c _ZN6glitch4core12_GLOBAL__N_118CProcessBufferHeap7setSizeEib
// The decompiled delete passes the start pointer; the ARM listing subtracts four bytes
// first, so the raw allocation (one word before start) is what gets freed.
int CProcessBufferHeap::setSize(int bytes, bool force) {
    const auto words = (static_cast<std::uint32_t>(bytes) + 3u) >> 2;
    if (start) {
        if (words == static_cast<std::uint32_t>(end - start)) return 0;
        if (start < top && !force) return 1;
        std::free(start - 1);
        start = nullptr;
        end = nullptr;
        top = nullptr;
    }
    if (words == 0) return 0;
    auto* raw = static_cast<int*>(std::malloc((static_cast<std::size_t>(words) + 1) * sizeof(int)));
    if (!raw) return 2;
    raw[0] = 0;
    start = raw + 1;
    end = start + words;
    top = start;
    return 0;
}

// 0x53434c _ZNK6glitch4core12_GLOBAL__N_118CProcessBufferHeap4dumpEPNS_7ILoggerENS_10ELOG_LEVELE
void CProcessBufferHeap::dump(glitch::os::ILogger* logger, glitch::os::ELOG_LEVEL level) const {
    int* block = start;
    while (block != top) {
        const int header = *block;
        int step = 0;
        if (header < 1) {
            step = -header;
        } else {
            char text[1024];
            std::snprintf(text, sizeof text, "0x%08lx: %8d",
                          static_cast<unsigned long>(reinterpret_cast<std::uintptr_t>(block + 1)),
                          (header - 2) * 4);
            if (logger) logger->log(text, level);
            step = header;
        }
        // A zero header cannot occur in a valid heap; stop rather than loop forever.
        if (step == 0) break;
        block += step;
    }
}

// 0x53443c _ZN6glitch4core12_GLOBAL__N_118CProcessBufferHeapD1Ev
CProcessBufferHeap::~CProcessBufferHeap() {
    if (setSize(0, false) == 1) {
        os::Printer::log("Destroying process buffer heap with allocated blocks:", os::ELL_ERROR);
        dump(os::Printer::Logger, os::ELL_ERROR);
        if (start) std::free(start - 1);
    }
}

// 0x534218 _ZN6glitch4core24getProcessBufferHeapSizeEv
int getProcessBufferHeapSize() {
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(heap.end) - reinterpret_cast<std::uintptr_t>(heap.start));
}

// 0x534234 _ZN6glitch4core35getProcessBufferHeapAvailableMemoryEv
int getProcessBufferHeapAvailableMemory() {
    const auto free = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(heap.end) -
                                                 reinterpret_cast<std::uintptr_t>(heap.top)) &
                      0xfffffffcu;
    return static_cast<int>(free) - 8;
}

// 0x534254 _ZN6glitch4core32isProcessBufferHeapExcessEnabledEv
bool isProcessBufferHeapExcessEnabled() { return heap.excessEnabled; }

// 0x534268 _ZN6glitch4core33setProcessBufferHeapExcessEnabledEb
void setProcessBufferHeapExcessEnabled(bool enabled) { heap.excessEnabled = enabled; }

// 0x534334 _ZN6glitch4core24setProcessBufferHeapSizeEi
void setProcessBufferHeapSize(int size) { heap.setSize(size, false); }

// 0x5344b4 _ZN6glitch4core28dumpProcessBufferAllocationsEPNS_7ILoggerENS_10ELOG_LEVELE
void dumpProcessBufferAllocations(glitch::os::ILogger* logger, glitch::os::ELOG_LEVEL level) {
    heap.dump(logger, level);
}

// 0x5344d0 _ZN6glitch4core12_GLOBAL__N_121processBufferHeapInitEb
// PARTIAL: the shutdown branch's setSize call loses its arguments in the decompile; the
// zero size and no-force form are inferred from the destructor, which makes the same call.
void processBufferHeapInit(bool atStartup) {
    if (atStartup) {
        setProcessBufferHeapSize(0x40000);
        return;
    }
    if (heap.setSize(0, false) != 1) return;
    os::Printer::log("Freeing process buffer heap would fail due to the following allocated blocks:",
                     os::ELL_ERROR);
    dumpProcessBufferAllocations(os::Printer::Logger, os::ELL_ERROR);
    os::Printer::log("Freeing anyways (at Glitch exit)", os::ELL_ERROR);
    heap.setSize(0, true);
}

// 0x5345f4 _ZN6glitch4core18allocProcessBufferEi
// PARTIAL: the size used when the heap is first created is the field at +0x0c, whose
// static value is not recovered here (zero by default), so the heap is only created by an
// explicit setProcessBufferHeapSize call unless that field is set.
int* allocProcessBuffer(int size) {
    if (!heap.start) heap.setSize(heap.defaultSize, false);
    const auto words = static_cast<std::ptrdiff_t>(((static_cast<std::uint32_t>(size) + 3u) >> 2) + 2u);
    if (words <= heap.end - heap.top) {
        int* block = heap.top;
        *block = static_cast<int>(words);
        heap.top += words;
        heap.top[-1] = static_cast<int>(words);
        return block + 1;
    }
    if (!heap.excessEnabled) return nullptr;
    return static_cast<int*>(std::malloc(static_cast<std::size_t>(size)));
}

// 0x534688 _ZN6glitch4core20releaseProcessBufferEPv
// The original takes the pointer as an unsigned 32-bit value; it is a void* here.
void releaseProcessBuffer(void* block) {
    const auto addr = reinterpret_cast<std::uintptr_t>(block);
    if (addr < reinterpret_cast<std::uintptr_t>(heap.start) || reinterpret_cast<std::uintptr_t>(heap.end) <= addr) {
        if (block) std::free(block);
        return;
    }
    int* header = static_cast<int*>(block) - 1;
    // Mark the block free: header and footer hold its negated size.
    int h = -*header;
    *header = h;
    header[~h] = h;

    if (heap.top[-1] < 0) {
        // The block (or the free block below it) ends at the top: give the space back.
        heap.top += heap.top[-1];
        if (heap.top[-1] < 0) heap.top += heap.top[-1];
        return;
    }
    if (header[-1] < 0) {
        // Merge with the free block before it.
        h += header[-1];
        header += header[-1];
        *header = h;
        header[~h] = h;
    }
    const int next = header[-h];
    if (next < 0) {
        // Merge with the free block after it.
        h += next;
        *header = h;
        header[~h] = h;
    }
}

}  // namespace glitch::core
