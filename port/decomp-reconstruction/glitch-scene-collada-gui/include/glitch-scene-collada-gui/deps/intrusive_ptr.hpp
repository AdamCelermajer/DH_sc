// Minimal boost::intrusive_ptr for this category. Keeps the boost spelling and
// null semantics the reconstructed bodies rely on. Reference counting is not
// modelled: the bodies pass these handles through, and the owning code is not
// reconstructed here.
#pragma once

#include <cstddef>

namespace boost {

template <typename T>
class intrusive_ptr {
public:
    intrusive_ptr() noexcept = default;
    intrusive_ptr(std::nullptr_t) noexcept {}
    explicit intrusive_ptr(T* p) noexcept : m_ptr(p) {}

    T* get() const noexcept { return m_ptr; }
    T* operator->() const noexcept { return m_ptr; }
    T& operator*() const noexcept { return *m_ptr; }
    explicit operator bool() const noexcept { return m_ptr != nullptr; }

private:
    T* m_ptr = nullptr;
};

}  // namespace boost
