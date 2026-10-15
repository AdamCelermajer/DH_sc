// glitch::core::detail::SSharedStringHeapEntry::SData::get (0x6a5074) and SData::release (0x6a4d9c) over
// the table layout in shared_string_heap.hpp.
//
// The original's table is the anonymous-namespace global SharedStringHeap (ELF 0x9f765c), allocated by
// initSharedStringHeap (0x6a5538) and holding a boost::unordered_map<SSharedStringHeapEntry, SData*>. get
// (0x6a5074, 1104 bytes) is that container's find/insert/rehash inlined; this file reconstructs what it
// does, not its instruction sequence. Both functions take the table as a parameter (the port has no
// SharedStringHeap global), so their mangled names are not the original's - the same deviation the
// release row records.
#include "glitch-core-io/core/shared_string_heap_ops.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace glitch::core::detail {
namespace {

// The bucket chain slot for a text: the hash of the text modulo the bucket count, as get computes it.
// The chain ends in the sentinel slot, whose word is null, so a plain walk to null is the same walk.
SSharedStringNode** bucketSlot(SSharedStringHeapTable& table, const char* text) {
    const std::size_t index = table.bucketCount == 0 ? 0 : sharedStringHash(text) % table.bucketCount;
    return &table.buckets[index];
}

// The record holding `text`, or null. The container's SEqual compares the candidate's text with the key
// holder's text (find_iterator 0x6a4d04, whose found path is a strcmp re-entry), which is the comparison
// the port makes: the engine interns every name, so an equal text is the same record.
SSharedStringData* findRecord(SSharedStringHeapTable& table, const char* text) {
    if (table.bucketCount == 0) return nullptr;
    for (SSharedStringNode* node = *bucketSlot(table, text); node != nullptr; node = node->next) {
        const char* candidate = sharedStringText(node->value);
        if (candidate != nullptr && std::strcmp(candidate, text) == 0) return node->value;
    }
    return nullptr;
}

// commit (0x6a4cb8) on the uncommitted holder: the original allocates max(strlen + 5, 8) bytes, zeroes
// the first word and strcpys the text in at +0x04, so after commit the record OWNS its text and the
// caller's buffer is irrelevant. In this model the inline text is the std::string, so copying into it and
// dropping `external` is the same fact; it is not optional, because get (0x6a5074) commits the record it
// inserts and every later lookup reads that text after the caller's buffer may be gone.
void commitRecord(SSharedStringData* holder) {
    holder->inlineText = holder->external == nullptr ? "" : holder->external;
    holder->external = nullptr;
    holder->committed = true;
}

// The bucket count the rehash picks: the first prime in boost's prime list that is at least
// floor(maxSize / maxLoadFactor) + 1, or the last prime when even that is too small (the original's
// binary search over prime_list_template<unsigned int>::value, 0x8eafc0, with the 0.0x..42FFFFFC0
// double bound). The >= 4294967300.0 guard is the original's `__aeabi_dcmpge` against a double built from
// 0x41FFFFFF.. shifted, i.e. 2^32 + 4; the listing's result for it is 0xFFFFFFFF, so the minimum must be
// at least 2^32, which the size_t form spells out directly and which no prime in the list can satisfy.
std::size_t nextBucketCount(float maxLoadFactor, std::size_t currentBucketCount) {
    const std::size_t needed = currentBucketCount + currentBucketCount / 2;
    const std::size_t maxSize = std::max(needed, currentBucketCount + 1);
    const double wanted = std::floor(static_cast<double>(maxSize) / static_cast<double>(maxLoadFactor));
    const std::size_t minimum = wanted >= 4294967300.0
                                    ? static_cast<std::size_t>(0xFFFFFFFFu) + 1u
                                    : static_cast<std::size_t>(static_cast<std::uint32_t>(wanted)) + 1u;
    for (std::uint32_t prime : kSharedStringPrimes) {
        if (static_cast<std::size_t>(prime) >= minimum) return prime;
    }
    return kSharedStringPrimes[39];
}

// The rehash in get: a data block over the new bucket count, every record re-linked by the hash of its
// OWN text, the cursor recomputed as the lowest non-empty bucket (or the end sentinel when the table is
// empty), then the new blocks swapped in and the threshold set to ceil(maxLoadFactor * bucketCount).
// The original moves the records out of the old blocks while decrementing the SIZE word per record and
// writes the saved size back after the swap, so the count is the same before and after; this port keeps
// that bookkeeping but masks the drops, so `size` never passes through a bogus value and a caller cannot
// observe a zero. Only the bucketCount real buckets are walked - slots[bucketCount] is the end sentinel,
// not a chain head - and the cursor compare is the original's `newBlockAddress < cursorSlotAddress` over
// the new block's own slots.
void rehash(SSharedStringHeapTable& table, std::size_t newBucketCount) {
    std::vector<SSharedStringNode*> newBuckets(newBucketCount + 1, nullptr);
    std::size_t newCursor = newBucketCount;
    const std::size_t kept = table.size;
    for (std::size_t bucket = 0; bucket < table.bucketCount && bucket < table.buckets.size(); ++bucket) {
        for (SSharedStringNode* record = table.buckets[bucket]; record != nullptr;) {
            SSharedStringNode* next = record->next;
            const std::size_t index = sharedStringHash(record->value) % newBucketCount;
            SSharedStringNode** slot = &newBuckets[index];
            record->next = *slot;
            *slot = record;
            if (slot < &newBuckets[newCursor]) newCursor = index;
            --table.size;  // the original counts the record out of the old blocks here
            record = next;
        }
    }
    table.buckets.swap(newBuckets);
    table.size = kept;
    table.bucketCount = newBucketCount;
    table.cursor = newCursor;
    table.rehashThreshold = static_cast<std::size_t>(std::ceil(table.maxLoadFactor * static_cast<double>(newBucketCount)));
}

}  // namespace

// 0x6a5074 _ZN6glitch4core6detail22SSharedStringHeapEntry5SData3getEPKcb
//
// Verified instruction by instruction against the listing (1104 bytes):
//   * LDRSB R3,[R0]; CMP R3,#0 -> a null text and an empty text both return null (the two exits that
//     MOV R0,#0). Also proved by the two lookups that call it (getParameterID 0x5d308c and
//     getTechniqueID 0x5d4714 pass R1 = 0 and answer with their own sentinel when this returns null).
//   * The key holder is a two-word stack block: [SP+0x20] = the text (SData + 0x00), [SP+0x24] = 0,
//     i.e. an UNCOMMITTED record, and the container looks up by text.
//   * The find either returns a record or (create only) one is GlitchAlloc(8)'d: record +0x00 = value =
//     the holder, record +0x04 = next = the old bucket head (get does not clear them: only +0x00 and +0x04
//     are ever read).
//   * Before the insert the load-factor test is `size + 1 < threshold` (CMP R2,R1 / BCC past the rehash),
//     the wanted count is max(size + size/2, size + 1), and the new prime is only taken when it differs
//     from the current bucket count (CMP R2,R3 / BEQ past).
//   * After the insert: ++size; the cursor moves down when the new bucket is BELOW it (CMP/STRCC), which
//     because the table is always kept sorted-by-bucket only ever happens for a new lowest bucket.
//   * commit (0x6a4cb8) runs on the newly built holder only (the B to the store on the create path), and
//     the function returns the holder's +0x00 word, which is the holder's own address after commit. The
//     existing-record exits return the record's +0x00 word directly.
//   * The two type-shared helpers keep the paths a caller observes identical: null/empty -> null; the
//     create form always yields a committed record whose text is the interned text; the lookup form never
//     creates and yields null for a text that is not interned.
//
// The two deviations, both host-necessitated and both named:
//   (1) the table is a parameter because the port has no SharedStringHeap global (initSharedStringHeap
//       0x6a5538 and the .bss global are not reconstructed), so this function does not emit the original
//       mangled name. Same deviation as release (0x6a4d9c), and no port code needs the symbol.
//   (2) the original's use count. It is the +0x00 word of a record, incremented by the callers
//       (SSharedString::operator= 0x5daf94, getParameterID 0x5d308c: `LDR R6,[R0]; ADD R6,R6,#1`), and
//       release (0x6a4d9c) frees the record when it reaches zero. The port's SSharedStringData has no
//       use-count word (the missing item the SIDedCollection / SShaderParameterDef rows record), so the
//       table owns every record it creates and nothing is freed on this path. The interning plus the text
//       copy into the record mean a record does not alias the caller's buffer, so the difference is the
//       lifetime of the records, not the text or the identity a lookup returns.
SSharedStringData* getSharedString(SSharedStringHeapTable& table, const char* text, bool noSideEffect) {
    if (text == nullptr || *text == '\0') return nullptr;

    if (noSideEffect) {
        // The B to loc_6A5430: pure lookup, no record is created, null when the text is not interned.
        return findRecord(table, text);
    }

    if (SSharedStringData* found = findRecord(table, text)) return found;

    // The not-found path: the uncommitted key holder becomes the table's record (the original allocates
    // the 8-byte node and stores the holder in its value word, so the record and the holder are two
    // allocations; the port's node->value is the same pointer).
    auto* holder = new SSharedStringData;
    holder->external = text;
    holder->committed = false;

    const std::size_t nextSize = table.size + 1;
    if (nextSize >= table.rehashThreshold) {
        const std::size_t wanted = nextBucketCount(table.maxLoadFactor, table.bucketCount);
        if (wanted != table.bucketCount) rehash(table, wanted);
    }

    SSharedStringNode** slot = bucketSlot(table, text);
    auto* record = new SSharedStringNode;
    record->value = holder;
    record->next = *slot;
    *slot = record;
    ++table.size;
    if (slot < &table.buckets[table.cursor]) table.cursor = static_cast<std::size_t>(slot - table.buckets.data());

    commitRecord(holder);
    return holder;
}

// 0x6a4d9c _ZN6glitch4core6detail22SSharedStringHeapEntry5SData7releaseEPS3_
// The bucket index is the hash of the text selected by release (inline when committed). The chain is walked
// to the record that holds this data and that record is unlinked and freed. size (+0x20) drops by one. When
// the removed bucket was the cursor (+0x1c), the cursor moves to the next non-empty bucket, or to the end
// sentinel when the table is empty. The SData is deleted last (operator delete[] in the listing).
// PARTIAL. Deviation: the port's chain walk compares the stored `value` pointer with the argument, while
// the listing's committed case compares the two texts (it reads the record's own text at +0x00/+0x04 and
// strcmps it with the released record's text). get (0x6a5074) interns by text, so a table holding two
// records for one text cannot be produced through this port's get; the difference is still a real body
// difference. The table is passed in: the original reads the anonymous-namespace global, which is not in
// the port (see the header). Deleting the SData here is the original's last-use-free, expressed directly
// because the port's SSharedStringData has no use count.
void releaseSharedString(SSharedStringHeapTable& table, SSharedStringData* data) {
    if (table.bucketCount != 0) {
        const std::size_t index = sharedStringHash(data->text()) % table.bucketCount;
        SSharedStringNode** link = &table.buckets[index];
        while (*link != nullptr && (*link)->value != data) link = &(*link)->next;
        if (*link != nullptr) {
            SSharedStringNode* node = *link;
            *link = node->next;
            delete node;
            --table.size;
            if (index == table.cursor) {
                if (table.size == 0) {
                    table.cursor = table.bucketCount;
                } else {
                    // table.cursor <= table.bucketCount always holds (the sentinel is the start value), so
                    // this walk stays inside `buckets` and ends at the sentinel at worst.
                    while (table.cursor < table.bucketCount && table.buckets[table.cursor] == nullptr) ++table.cursor;
                }
            }
        }
    }
    delete data;
}

}  // namespace glitch::core::detail
