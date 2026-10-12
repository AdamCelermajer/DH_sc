// Minimal boost::intrusive_ptr-compatible smart pointer for the port. It keeps
// the boost spelling and null semantics the original code relies on. Reference
// counting is intentionally absent here: the reconstructed bodies only pass
// these around, and the owning code is not reconstructed yet.
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

    friend bool operator==(const intrusive_ptr& a, const intrusive_ptr& b) noexcept {
        return a.m_ptr == b.m_ptr;
    }
    friend bool operator!=(const intrusive_ptr& a, const intrusive_ptr& b) noexcept {
        return a.m_ptr != b.m_ptr;
    }

private:
    T* m_ptr = nullptr;
};

}  // namespace boost

namespace glitch::video {
template <typename T>
using intrusive_ptr = boost::intrusive_ptr<T>;
}  // namespace glitch::video
