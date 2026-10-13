// VectorSet<T>: a vector that holds each value at most once. push_back_unique searches the storage first and
// appends only when the value is absent. Used with int, AnimatedFX* and SWFAnim* (SWFAnimManager::FlushBank).
// The storage is the three-pointer std::vector layout of the original (STLport): begin, end, end of storage.
// Original 32-bit size 0x0c (12). Only the pointer names are reconstructed; the 64-bit build uses 8-byte pointers.
#pragma once

template <typename T>
class VectorSet {
public:
    // Instantiations: VectorSet<int>::push_back_unique 0x49437c, VectorSet<AnimatedFX*> 0x494550,
    // VectorSet<SWFAnim*> 0x49777c. Each is a 264-byte function.
    void push_back_unique(const T& value);

    T* m_start;                                                         // +0x00 first element
    T* m_finish;                                                        // +0x04 one past the last element
    T* m_endOfStorage;                                                  // +0x08 one past the allocated storage
};
