// glitch::core::detail::SSharedStringHeapEntry::SData::release (0x6a4d9c). Layout and hash: shared_string_heap.hpp.
#include "glitch-core-io/core/shared_string_heap_ops.hpp"

namespace glitch::core::detail {

// The bucket index is the hash of the text selected by release (inline when committed). The chain is walked
// to the record that holds this data and that record is unlinked and freed. size (+0x20) drops by one. When the
// removed bucket was the cursor (+0x1c), the cursor moves to the next non-empty bucket, or to the end sentinel
// when the table is empty. The SData is deleted last (operator delete[] in the listing).
// Deviation: the listing matches the first record by text (strcmp) and later ones by identity; this port
// matches by identity.
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
