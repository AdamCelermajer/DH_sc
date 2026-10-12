// Accessors of the process buffer heap (glitch::core). The heap itself (setSize,
// allocation, release and the dump) is not reconstructed yet; these read and write the
// state the original keeps in its heap object.
#pragma once

namespace glitch::core {

// 0x534218: heap size in bytes (end bound minus start).
int getProcessBufferHeapSize();
// 0x534234: free bytes between the top of the heap and its bound, less the header of 8.
int getProcessBufferHeapAvailableMemory();
// 0x534254
bool isProcessBufferHeapExcessEnabled();
// 0x534268
void setProcessBufferHeapExcessEnabled(bool enabled);

}  // namespace glitch::core
