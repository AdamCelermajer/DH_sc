// Heap sort and binary search templates of glitch::core. The original instantiates
// them for several element types (scene entries, GUI font and sprite records); each
// instantiation is ledgered against this one template.
//
// heapsink works on 1-based heap positions: element e is array[e - 1]. `max` is one past
// the last element of the heap, so the original's heapsort passes size + 1 when it sinks
// into a heap of `size` elements. Children are compared with operator<, the larger one
// is swapped up while the sinking element is smaller than it.
#pragma once

#include <algorithm>
#include <utility>
#include <vector>

namespace glitch::core {

// 0x350f00 / 0x35104c / 0x6b41d8 (heapsink instantiations)
template <class T>
void heapsink(T* array, int element, int max) {
    int child = element * 2;
    while (child < max) {
        if (child + 1 < max && array[child - 1] < array[child]) ++child;
        if (!(array[element - 1] < array[child - 1])) return;
        std::swap(array[element - 1], array[child - 1]);
        element = child;
        child = element * 2;
    }
}

// 0x350fc0 / 0x3510f0 / 0x6b4338 (heapsort instantiations)
template <class T>
void heapsort(T* array, int size) {
    for (int i = (size - 1) / 2 + 1; i >= 1; --i) heapsink(array, i, size + 1);
    if (size < 1) return;
    int n = size;
    do {
        std::swap(array[0], array[n - 1]);
        heapsink(array, 1, n);
        --n;
    } while (n != 0);
}

// 0x538314 / 0x538470 / 0x5385cc / 0x539e3c (binary_search instantiations): the index of
// an element equal to key (neither less than the other), or -1.
template <class T, class Alloc>
int binary_search(const std::vector<T, Alloc>& values, const T& key) {
    if (values.empty()) return -1;
    const auto it = std::lower_bound(values.begin(), values.end(), key);
    if (it != values.end() && !(*it < key) && !(key < *it)) {
        return static_cast<int>(it - values.begin());
    }
    return -1;
}

}  // namespace glitch::core
