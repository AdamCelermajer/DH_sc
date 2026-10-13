// glitch::core::detail::SSharedStringHeapEntry::SData::release (0x6a4d9c). Layout and hash: shared_string_heap.hpp.
#include "glitch-core-io/core/shared_string_heap_ops.hpp"

namespace glitch::core::detail {

// The bucket index is the hash of the text selected by release (inline when committed). The chain is walked
// to the record that holds this data and that record is unlinked and freed. size (+0x20) drops by one. When the
// removed bucket was the cursor (+0x1c), the cursor moves to the next non-empty bucket, or to the end sentinel
// when the table is empty. The SData is deleted last (operator delete[] in the listing).
// PARTIAL. Deviation: for a committed argument the listing walks the chain by identity (as this port
// does). For an uncommitted argument it compares each record's text with strcmp (the loop back-edge
// re-enters the strcmp initialisation); this port uses identity there too. The two differ only when a
// record has the same text as the released uncommitted holder. The table is passed in: the original
// reads the anonymous-namespace global, which is not in the port (see the header).
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
                    while (table.buckets[table.cursor] == nullptr) ++table.cursor;
                }
            }
        }
    }
    delete data;
}

}  // namespace glitch::core::detail
