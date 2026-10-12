// Stand-in for CDataStats<T> (owned by another slice). 36 bytes in the original: a vtable
// pointer, a CReadWriteLock and the sample fields. The bodies are not reconstructed here,
// so the methods return zero.
#pragma once

template <typename T>
class CDataStats {
public:
    virtual ~CDataStats() = default;

    // CDataStats<int>::AddData. Stand-in: the sample is not stored.
    void AddData(T value) { (void)value; }
    // CDataStats<int>::GetSum. Stand-in: zero.
    T GetSum() const { return T{}; }
    // CDataStats<int>::GetAverage(float). Stand-in: zero.
    T GetAverage(float window) const {
        (void)window;
        return T{};
    }
};
