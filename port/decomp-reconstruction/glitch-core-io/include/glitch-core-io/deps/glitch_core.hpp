// Minimal stand-ins for glitch::core and glitch::video types used by the io
// attribute code. Layout matches the original: plain members, no vtable.
#pragma once

#include <cstdint>

namespace glitch {
// Reference-counted base; the counter is at +0x04 in the original.
class IReferenceCounted {
public:
    virtual ~IReferenceCounted() = default;
    // Called by drop() right before the object is deleted (vtable +0x08 in the original).
    virtual void onDelete() {}
    // Decrements the count; on zero calls onDelete() and deletes the object.
    // Returns true when the object was deleted. 0x31d584 _ZNK6glitch17IReferenceCounted4dropEv
    bool drop() const;
    mutable int referenceCounter = 1;  // +0x04
};

// Event receiver interface (only its destructor is used by the io code).
class IEventReceiver {
public:
    virtual ~IEventReceiver() = default;  // 0x31d8cc (D1)
};
}  // namespace glitch

namespace glitch::core {
template <typename T>
struct vector2d {
    T X, Y;
};
template <typename T>
struct vector3d {
    T X, Y, Z;
};
template <typename T>
struct vector4d {
    T X, Y, Z, W;
};
template <typename T>
struct position2d {
    T X, Y;
};
template <typename T>
struct rect {
    T X1, Y1, X2, Y2;  // upper-left and lower-right corners

    // 0x535c18 (rect<int>): if this rectangle is no larger than limit, moves it inside
    // limit (right edge, bottom, left, top, in that order) and returns true; otherwise
    // leaves it unchanged and returns false.
    bool constrainTo(const rect& limit) {
        if (limit.X2 - limit.X1 < X2 - X1 || limit.Y2 - limit.Y1 < Y2 - Y1) return false;
        T diff = limit.X2 - X2;
        if (diff < 0) {
            X1 += diff;
            X2 += diff;
        }
        diff = limit.Y2 - Y2;
        if (diff < 0) {
            Y1 += diff;
            Y2 += diff;
        }
        diff = X1 - limit.X1;
        if (diff < 0) {
            X1 -= diff;
            X2 -= diff;
        }
        diff = Y1 - limit.Y1;
        if (diff < 0) {
            Y1 -= diff;
            Y2 -= diff;
        }
        return true;
    }
};
// Modelled on upstream Irrlicht 1.8.5 (quaternion.h, triangle3d.h, line2d.h,
// line3d.h, dimension2d.h, aabbox3d.h, plane3d.h, matrix4.h); plain members only.
struct quaternion {
    float X, Y, Z, W;
};
template <typename T>
struct triangle3d {
    vector3d<T> A, B, C;
};
template <typename T>
struct line2d {
    vector2d<T> start, end;
};
template <typename T>
struct line3d {
    vector3d<T> start, end;
};
template <typename T>
struct dimension2d {
    T Width, Height;
};
template <typename T>
struct aabbox3d {
    vector3d<T> MinEdge, MaxEdge;

    // 0x35c150 addInternalBox: grows the box to contain b. Partial: the MaxEdge updates
    // are checked against the ARM (strict compares); the MinEdge updates are the upstream
    // Irrlicht ones and were not compared.
    void addInternalBox(const aabbox3d& b) {
        grow(MaxEdge.X, b.MaxEdge.X);
        grow(MaxEdge.Y, b.MaxEdge.Y);
        grow(MaxEdge.Z, b.MaxEdge.Z);
        shrink(MinEdge.X, b.MinEdge.X);
        shrink(MinEdge.Y, b.MinEdge.Y);
        shrink(MinEdge.Z, b.MinEdge.Z);
    }

private:
    static void grow(T& edge, T value) {
        if (value > edge) edge = value;
    }
    static void shrink(T& edge, T value) {
        if (value < edge) edge = value;
    }
};
template <typename T>
struct plane3d {
    vector3d<T> Normal;
    T D;
};
template <typename T>
struct CMatrix4 {
    T M[16];  // row-major, as in the original
    bool flag = false;  // +0x40: byte after the 16 values; its meaning is not recovered

    // 0x31f790 getTransposed: out[i] = this[(i % 4) * 4 + i / 4]; the flag is copied.
    void getTransposed(CMatrix4& out) const {
        out.flag = false;
        for (int i = 0; i < 16; ++i) out.M[i] = M[(i % 4) * 4 + i / 4];
        out.flag = flag;
    }
};
}  // namespace glitch::core

namespace glitch::video {
// Stand-in for the driver interface; only its reference counting is used here.
class IVideoDriver : public glitch::IReferenceCounted {};
// Reference-counted video objects held through boost::intrusive_ptr by the io code.
class ITexture : public glitch::IReferenceCounted {};
class CLight : public glitch::IReferenceCounted {};
struct SColor {
    std::uint32_t color;
};
struct SColorf {
    float r, g, b, a;
};

// CMaterial and CMaterialRenderer are not reference-counted through IReferenceCounted:
// the count is their first member (+0x00), and the release calls the destructor
// directly (no vtable is assumed). The intrusive_ptr release below deletes the object
// when the last use goes; the original also frees it with CustomFree (not modelled).
class CMaterial {
public:
    mutable int referenceCounter = 1;  // +0x00
};
class CMaterialRenderer {
public:
    mutable int referenceCounter = 1;  // +0x00
};

// Found by argument-dependent lookup from boost::intrusive_ptr (0x310be8, 0x351e3c,
// 0x3522b8 release through these).
inline void intrusive_ptr_add_ref(const CMaterial* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const CMaterial* p) {
    if (--p->referenceCounter == 0) delete p;
}
inline void intrusive_ptr_add_ref(const CMaterialRenderer* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const CMaterialRenderer* p) {
    if (--p->referenceCounter == 0) delete p;
}
}  // namespace glitch::video
