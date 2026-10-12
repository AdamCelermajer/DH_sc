// boost::intrusive_ptr for the glitch reference-counted types (IReferenceCounted:
// count at +0x04, released with drop()). The copy operator adds a use to the new
// object and drops the old one, as the original does.
#pragma once

#include "glitch-core-io/deps/glitch_core.hpp"

namespace boost {

// Found by argument-dependent lookup for glitch types.
inline void intrusive_ptr_add_ref(const glitch::IReferenceCounted* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const glitch::IReferenceCounted* p) { p->drop(); }

template <typename T>
class intrusive_ptr {
public:
    intrusive_ptr() = default;
    intrusive_ptr(T* p) : px(p) {}  // NOLINT: implicit like boost (no add-ref, takes the reference)
    intrusive_ptr(const intrusive_ptr& other) : px(other.px) {
        if (px) intrusive_ptr_add_ref(px);
    }
    ~intrusive_ptr() {  // 0x41742c for ITexture
        if (px) intrusive_ptr_release(px);
    }
    // 0x384df8 (ITexture): adds a use to the new object, then drops the old one.
    intrusive_ptr& operator=(const intrusive_ptr& rhs) {
        T* newPx = rhs.px;
        if (newPx) intrusive_ptr_add_ref(newPx);
        T* oldPx = px;
        px = newPx;
        if (oldPx) intrusive_ptr_release(oldPx);
        return *this;
    }
    T* get() const { return px; }
    T* operator->() const { return px; }
    T& operator*() const { return *px; }
    explicit operator bool() const { return px != nullptr; }

    T* px = nullptr;
};

}  // namespace boost
