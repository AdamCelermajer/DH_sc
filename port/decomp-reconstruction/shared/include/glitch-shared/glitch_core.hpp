// Minimal stand-ins for glitch::core and glitch::video types used by the io
// attribute code. Layout matches the original: plain members, no vtable.
#pragma once

#include <cmath>
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

    // 0x420060 vector2d<float>::getAngle: the angle in degrees of the vector, measured as the
    // original does (0 when Y or X is zero; quadrant offsets of 90 and 270 degrees). The
    // original returns the float bits through an int register; here it returns a float.
    float getAngle() const {
        const float x = static_cast<float>(X);
        const float y = static_cast<float>(Y);
        if (y == 0.f) return 0.f;
        if (x == 0.f) return 0.f;
        const float len = std::sqrt(x * x + y * y);
        const float ratio = y / len;
        float angle = std::atan(std::sqrt(1.f - ratio * ratio) / ratio) * 57.2957795f;
        if (x > 0.f) {
            if (y > 0.f) return angle + 270.f;
            if (y < 0.f) return angle + 90.f;
        }
        if (x < 0.f) {
            if (y < 0.f) return 90.f - angle;
            if (y > 0.f) return 270.f - angle;
        }
        return angle;
    }
};
template <typename T>
struct vector3d {
    T X, Y, Z;

    // 0x35e8e0 vector3d<float>::normalize(void): when the squared length v2 = X*X + Y*Y + Z*Z is
    // nonzero, every component is multiplied by 1 / sqrt(v2) (a reciprocal, not a division). A zero
    // vector is left unchanged. Returns this, as the original returns its argument.
    vector3d& normalize() {
        const T v2 = static_cast<T>((X * X + Y * Y) + Z * Z);
        if (v2 != T(0)) {
            const T inv = static_cast<T>(T(1) / std::sqrt(v2));
            X = X * inv;
            Y = Y * inv;
            Z = Z * inv;
        }
        return *this;
    }
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
struct line3d;
template <typename T>
struct aabbox3d;

template <typename T>
struct triangle3d {
    vector3d<T> A, B, C;

    // 0x6c3da0 triangle3d<float>::isPointInsideFast. Barycentric test with the sign-bit
    // combination of the original: both numerators (X for v, Y for u) are non-negative and
    // the sum minus the denominator is negative. Degenerate triangles give a false result.
    bool isPointInsideFast(const vector3d<T>& p) const {
        const T ux = B.X - A.X, uy = B.Y - A.Y, uz = B.Z - A.Z;
        const T vx = C.X - A.X, vy = C.Y - A.Y, vz = C.Z - A.Z;
        const T wx = p.X - A.X, wy = p.Y - A.Y, wz = p.Z - A.Z;
        const T d00 = ux * ux + uy * uy + uz * uz;
        const T d01 = ux * vx + uy * vy + uz * vz;
        const T d11 = vx * vx + vy * vy + vz * vz;
        const T d20 = wx * ux + wy * uy + wz * uz;
        const T d21 = wx * vx + wy * vy + wz * vz;
        const T x = d00 * d21 - d01 * d20;  // v numerator
        const T y = d11 * d20 - d01 * d21;  // u numerator
        const T z = (x + y) - (d00 * d11 - d01 * d01);
        return !std::signbit(x) && !std::signbit(y) && std::signbit(z);
    }

    // 0x585bb8 triangle3d<float>::isOnSameSide(p1, p2, a, b) const. The receiver is unused. With
    // e = b - a, w = p1 - a and z = p2 - a, the listing computes dot((e x w), (e x z)) >= 0, with the
    // cross-product terms written as in the listing (the sign flips are negations of e's components).
    bool isOnSameSide(const vector3d<T>& p1, const vector3d<T>& p2, const vector3d<T>& a,
                      const vector3d<T>& b) const {
        const T ex = b.X - a.X, ey = b.Y - a.Y, ez = b.Z - a.Z;
        const T wx = p1.X - a.X, wy = p1.Y - a.Y, wz = p1.Z - a.Z;
        const T zx = p2.X - a.X, zy = p2.Y - a.Y, zz = p2.Z - a.Z;
        const T t1 = (wz * -ey) + (ez * wy);
        const T b1 = (-ey * zz) + (ez * zy);
        const T t2 = (wx * -ez) + (ex * wz);
        const T b2 = (-ez * zx) + (ex * zz);
        const T t3 = (wy * -ex) + (ey * wx);
        const T b3 = (-ex * zy) + (ey * zx);
        return (((t1 * b1) + (t2 * b2)) + (t3 * b3)) >= T(0);
    }

    // 0x585de8 triangle3d<float>::isPointInside(p) const: p is on the inner side of all three edges.
    bool isPointInside(const vector3d<T>& p) const {
        return isOnSameSide(p, A, B, C) && isOnSameSide(p, B, A, C) && isOnSameSide(p, C, A, B);
    }

    // 0x585e80 triangle3d<float>::getIntersectionOfPlaneWithLine(start, dir, out) const. The plane
    // normal is cross(C - A, B - A) (listing: v29), normalized with vector3d::normalize. Returns false
    // when |N . dir| <= 1e-6 (double compare, as in the listing); otherwise out = start + t * dir with
    // t = (N . A - N . start) / (N . dir).
    bool getIntersectionOfPlaneWithLine(const vector3d<T>& start, const vector3d<T>& dir,
                                        vector3d<T>& out) const {
        const T ux = B.X - A.X, uy = B.Y - A.Y, uz = B.Z - A.Z;
        const T vx = C.X - A.X, vy = C.Y - A.Y, vz = C.Z - A.Z;
        vector3d<T> n{(vz * -uy) + (uz * vy), (vx * -uz) + (ux * vz), (vy * -ux) + (uy * vx)};
        n.normalize();
        const T nd = ((n.X * dir.X) + (n.Y * dir.Y)) + (n.Z * dir.Z);
        if (std::fabs(static_cast<double>(nd)) <= 0.000001) return false;
        const T ns = ((n.X * start.X) + (n.Y * start.Y)) + (n.Z * start.Z);
        const T na = ((n.X * A.X) + (n.Y * A.Y)) + (n.Z * A.Z);
        const T t = (na - ns) / nd;
        out.X = start.X + (t * dir.X);
        out.Y = start.Y + (t * dir.Y);
        out.Z = start.Z + (t * dir.Z);
        return true;
    }

    // 0x58615c triangle3d<float>::getIntersectionWithLine(start, dir, out) const: the plane hit, and
    // then the inside test on that point.
    bool getIntersectionWithLine(const vector3d<T>& start, const vector3d<T>& dir, vector3d<T>& out) const {
        if (!getIntersectionOfPlaneWithLine(start, dir, out)) return false;
        return isPointInside(out);
    }

    // 0x586188 triangle3d<float>::getIntersectionWithLineSegment(segment, lengthSq, dir, box, out) const.
    // lengthSq is compared with squared distances (out to segment.start and out to segment.end). The
    // box test rejects the triangle when its corners all lie past one face of the box on some axis.
    // dir is the segment direction passed on to getIntersectionWithLine.
    bool getIntersectionWithLineSegment(const line3d<T>& segment, T lengthSq, const vector3d<T>& dir,
                                        const aabbox3d<T>& box, vector3d<T>& out) const {
        const T minX = box.MinEdge.X, maxX = box.MaxEdge.X;
        const T minY = box.MinEdge.Y, maxY = box.MaxEdge.Y;
        const T minZ = box.MinEdge.Z, maxZ = box.MaxEdge.Z;
        if (minX > A.X && minX > B.X && minX > C.X) return false;
        if (A.X > maxX && maxX < B.X && maxX < C.X) return false;
        if (minY > A.Y && minY > B.Y && minY > C.Y) return false;
        if (A.Y > maxY && maxY < B.Y && maxY < C.Y) return false;
        if (minZ > A.Z && minZ > B.Z && minZ > C.Z) return false;
        if (!((A.Z <= maxZ) || (maxZ >= B.Z) || (maxZ >= C.Z))) return false;
        if (!getIntersectionWithLine(segment.start, dir, out)) return false;
        const T dx = out.X - segment.start.X, dy = out.Y - segment.start.Y, dz = out.Z - segment.start.Z;
        if (!(lengthSq > ((dx * dx + dy * dy) + dz * dz))) return false;
        const T ex = out.X - segment.end.X, ey = out.Y - segment.end.Y, ez = out.Z - segment.end.Z;
        return lengthSq > ((ex * ex + ey * ey) + ez * ez);
    }
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

    // 0x35c150 addInternalBox: grows the box to contain b. The original folds in b's MaxEdge
    // corner and then its MinEdge corner, and each corner updates both edges (strict compares,
    // per axis). For a valid box the MaxEdge corner's min update and the MinEdge corner's max
    // update change nothing, but they are kept so invalid boxes match too.
    void addInternalBox(const aabbox3d& b) {
        addInternalPoint(b.MaxEdge);
        addInternalPoint(b.MinEdge);
    }

    void addInternalPoint(const vector3d<T>& p) {
        grow(MaxEdge.X, p.X);
        shrink(MinEdge.X, p.X);
        grow(MaxEdge.Y, p.Y);
        shrink(MinEdge.Y, p.Y);
        grow(MaxEdge.Z, p.Z);
        shrink(MinEdge.Z, p.Z);
    }

    // 0x585970 intersectsWithLine_impl_1d (const): clips the parameter interval [tmin, tmax] of a
    // segment start..end (1-d) against the slab bmin..bmax. Returns false when the slab misses.
    bool intersectsWithLine_impl_1d(T bmin, T bmax, T start, T end, T& tmin, T& tmax) const {
        const T d = end - start;
        T enter = 0;
        T leave = 0;
        if (start >= end) {
            if (!(end <= bmax && start >= bmin)) return false;
            enter = start > bmax ? static_cast<T>((bmax - start) / d) : static_cast<T>(0);
            if (end < bmin) {
                leave = static_cast<T>((bmin - start) / (end - start));
            } else {
                leave = static_cast<T>(1);
            }
        } else {
            if (start > bmax || end < bmin) return false;
            enter = start < bmin ? static_cast<T>((bmin - start) / d) : static_cast<T>(0);
            if (end <= bmax) {
                leave = static_cast<T>(1);
            } else {
                leave = static_cast<T>((bmax - start) / (end - start));
            }
        }
        if (tmin < enter) tmin = enter;
        if (tmax > leave) {
            tmax = leave;
        } else {
            leave = tmax;
        }
        return tmin <= leave;
    }

    // 0x585b08 intersectsWithSegment (const): clips the segment against the three slabs.
    bool intersectsWithSegment(const line3d<T>& segment, T& tmin, T& tmax) const {
        tmin = 0;
        tmax = 1;
        return intersectsWithLine_impl_1d(MinEdge.X, MaxEdge.X, segment.start.X, segment.end.X, tmin, tmax) &&
               intersectsWithLine_impl_1d(MinEdge.Y, MaxEdge.Y, segment.start.Y, segment.end.Y, tmin, tmax) &&
               intersectsWithLine_impl_1d(MinEdge.Z, MaxEdge.Z, segment.start.Z, segment.end.Z, tmin, tmax);
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
    vector3d<T> Normal;  // +0x00
    T D;                 // +0x0c

    // 0x561bdc setPlane(a, b, c): Normal = (c - a) x (b - a), normalized with vector3d::normalize
    // (0x35e8e0, confirmed from the listing: a call, then D = -dot(a, Normal) with a sign-bit flip).
    void setPlane(const vector3d<T>& a, const vector3d<T>& b, const vector3d<T>& c) {
        const T ux = b.X - a.X, uy = b.Y - a.Y, uz = b.Z - a.Z;  // edge a->b
        const T wx = c.X - a.X, wy = c.Y - a.Y, wz = c.Z - a.Z;  // edge a->c
        Normal.X = wy * uz - wz * uy;
        Normal.Y = wz * ux - wx * uz;
        Normal.Z = wx * uy - wy * ux;
        Normal.normalize();
        D = -(a.X * Normal.X + a.Y * Normal.Y + a.Z * Normal.Z);
    }

    // 0x34033c getIntersectionWithLine(start, dir, out) const: returns false when N . dir == 0 (exact
    // compare). Otherwise t = -(N . start + D) / (N . dir) and out = start + t * dir.
    bool getIntersectionWithLine(const vector3d<T>& start, const vector3d<T>& dir, vector3d<T>& out) const {
        const T nd = ((Normal.X * dir.X) + (Normal.Y * dir.Y)) + (Normal.Z * dir.Z);
        if (nd == T(0)) return false;
        const T ns = ((Normal.X * start.X) + (Normal.Y * start.Y)) + (Normal.Z * start.Z);
        const T t = -(ns + D) / nd;
        out.X = start.X + (t * dir.X);
        out.Y = start.Y + (t * dir.Y);
        out.Z = start.Z + (t * dir.Z);
        return true;
    }

    // 0x40f0b4 getIntersectionWithLimitedLine(start, end, out) const: the hit must exist and must lie
    // on the segment, i.e. its squared distance to start and to end are both at most the squared
    // segment length. Writes out before the segment test, as the listing does.
    bool getIntersectionWithLimitedLine(const vector3d<T>& start, const vector3d<T>& end, vector3d<T>& out) const {
        const vector3d<T> d{end.X - start.X, end.Y - start.Y, end.Z - start.Z};
        if (!getIntersectionWithLine(start, d, out)) return false;
        const T lx = end.X - start.X, ly = end.Y - start.Y, lz = end.Z - start.Z;
        const T len2 = ((lx * lx) + (ly * ly)) + (lz * lz);
        const T sx = out.X - start.X, sy = out.Y - start.Y, sz = out.Z - start.Z;
        const T distStart = ((sx * sx) + (sy * sy)) + (sz * sz);
        const T ex = out.X - end.X, ey = out.Y - end.Y, ez = out.Z - end.Z;
        const T distEnd = ((ex * ex) + (ey * ey)) + (ez * ez);
        return len2 >= distStart && len2 >= distEnd;
    }
};
template <typename T>
struct CMatrix4 {
    enum eConstructor { EM4CONST_NOTHING, EM4CONST_COPY, EM4CONST_IDENTITY };

    CMatrix4() = default;
    // 0x631c14 (clone), 0x6c0ce4: the identity matrix, with the flag set. The decompile ignores
    // the argument and always builds the identity.
    explicit CMatrix4(eConstructor) : M{}, flag(false) {
        M[0] = M[5] = M[10] = M[15] = static_cast<T>(1);
        flag = true;
    }

    // 0x5ba19c: the flag at +0x40 caches the answer. When it is clear, each diagonal element must
    // be 1 within 1e-6 (float arithmetic, as the ARM code does: x + tol >= 1 and x - tol <= 1) and
    // every off-diagonal element must satisfy |x| <= 1e-6. A full match sets the flag. The flag is
    // written by this const method, so it is mutable.
    bool isIdentity() const {
        if (flag) return true;
        constexpr float kTolerance = 1e-6f;  // 0x358637BD
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                const float x = static_cast<float>(M[row * 4 + col]);
                if (row == col) {
                    if (!(static_cast<float>(x + kTolerance) >= 1.0f &&
                          static_cast<float>(x - kTolerance) <= 1.0f)) {
                        return false;
                    }
                } else if (!(std::fabs(x) <= kTolerance)) {
                    return false;
                }
            }
        }
        flag = true;
        return true;
    }

    T M[16];  // row-major, as in the original
    mutable bool flag = false;  // +0x40: byte after the 16 values; set by the identity constructor and by isIdentity

    // 0x35e998 CMatrix4<float>::operator*(const CMatrix4&) const: this * other, through
    // detail::CMatrix4Base<float>::mult (0x35e118). DECLARED ONLY: the definition is in
    // glitch-core-io/core/matrix4_operators.hpp, so include that header to use it.
    CMatrix4 operator*(const CMatrix4& other) const;

    // 0x31f790 getTransposed: out[i] = this[(i % 4) * 4 + i / 4]; the flag is copied.
    void getTransposed(CMatrix4& out) const {
        out.flag = false;
        for (int i = 0; i < 16; ++i) out.M[i] = M[(i % 4) * 4 + i / 4];
        out.flag = flag;
    }

    // 0x598ee4 getScale: the length of each of the three rows (elements 0..2, 4..6, 8..10).
    vector3d<T> getScale() const {
        return {static_cast<T>(std::sqrt(M[0] * M[0] + M[1] * M[1] + M[2] * M[2])),
                static_cast<T>(std::sqrt(M[4] * M[4] + M[5] * M[5] + M[6] * M[6])),
                static_cast<T>(std::sqrt(M[8] * M[8] + M[9] * M[9] + M[10] * M[10]))};
    }

    // 0x35dd08 transformPlane (const): plane.Normal becomes the rotation part of the matrix applied
    // to the normal; plane.D becomes -(P' . N') where P' is the matrix applied to the point -D*N.
    // The listing is row-major with the translation in elements 12..14.
    void transformPlane(plane3d<T>& plane) const {
        const T nx = plane.Normal.X, ny = plane.Normal.Y, nz = plane.Normal.Z;
        const T d = static_cast<T>(-plane.D);
        const T px = d * nx, py = d * ny, pz = d * nz;
        // Transformed point P' = M * (-D * N) (translation included).
        const T pX = px * M[0] + py * M[4] + pz * M[8] + M[12];
        const T pY = px * M[1] + py * M[5] + pz * M[9] + M[13];
        const T pZ = px * M[2] + py * M[6] + pz * M[10] + M[14];
        // Transformed normal: M * N minus M * origin (the translation cancels).
        const T tnX = nx * M[0] + ny * M[4] + nz * M[8];
        const T tnY = nx * M[1] + ny * M[5] + nz * M[9];
        const T tnZ = nx * M[2] + ny * M[6] + nz * M[10];
        plane.Normal.X = tnX;
        plane.Normal.Y = tnY;
        plane.Normal.Z = tnZ;
        plane.D = static_cast<T>(-(pX * tnX + pY * tnY + pZ * tnZ));
    }

    // 0x6c7cf0 setRotationRadians: writes the rotation part from angles (x, y, z) in radians. Elements
    // 3, 7, 11 and 12..15 are not written by the listing and keep their values; the flag is cleared.
    void setRotationRadians(const vector3d<T>& angles) {
        const T cx = std::cos(angles.X), sx = std::sin(angles.X);
        const T cy = std::cos(angles.Y), sy = std::sin(angles.Y);
        const T cz = std::cos(angles.Z), sz = std::sin(angles.Z);
        flag = false;
        M[0] = cy * cz;
        M[1] = cy * sz;
        M[2] = -sy;
        M[4] = sx * sy * cz - cx * sz;
        M[5] = sx * sy * sz + cx * cz;
        M[6] = sx * cy;
        M[8] = cx * sy * cz + sx * sz;
        M[9] = cx * sy * sz - sx * cz;
        M[10] = cx * cy;
    }
};
}  // namespace glitch::core

namespace glitch::video {
// Stand-in for the driver interface; only its reference counting is used here.
class IVideoDriver : public glitch::IReferenceCounted {};
// Reference-counted video objects held through boost::intrusive_ptr by the io code.
// ITexture keeps a slot sub-type at +0x38; its low two bits pick the parameter code 12..15 that a
// texture is stored under (see material_parameters.hpp, PtrTraits<ITexture>).
class ITexture : public glitch::IReferenceCounted {
public:
    std::uint32_t slotType = 0;  // +0x38
};
class CLight : public glitch::IReferenceCounted {};
// Video mode list of a device (glitch::video::CVideoModeList); only its storage is needed here.
class CVideoModeList {
public:
    CVideoModeList() = default;
};
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

// CVertexStreams is released like CMaterial: the count is at +0x00 and the last release
// deletes the object (0x5a10fc; the original calls its destructor first, not modelled).
class CVertexStreams {
public:
    mutable int referenceCounter = 1;  // +0x00
};
inline void intrusive_ptr_add_ref(const CVertexStreams* p) { ++p->referenceCounter; }
inline void intrusive_ptr_release(const CVertexStreams* p) {
    if (--p->referenceCounter == 0) delete p;
}

// Reference-counted video objects that only need the +0x04 drop() release here.
class IShader : public glitch::IReferenceCounted {};      // 0x5b9d6c
class IShaderCode : public glitch::IReferenceCounted {};  // 0x5b9dfc
class IBuffer : public glitch::IReferenceCounted {};      // 0x637b8c
class CImage : public glitch::IReferenceCounted {};       // 0x603414

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

namespace glitch::scene {
// Mesh buffer stand-in for the intrusive_ptr release at 0x637bac (drop() at +0x04).
class CMeshBuffer : public glitch::IReferenceCounted {};
}  // namespace glitch::scene
