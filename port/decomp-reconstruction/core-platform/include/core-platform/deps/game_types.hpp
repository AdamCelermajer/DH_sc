// Game-side value types referenced by the text helpers (owned by other categories).
#pragma once

template <typename T>
struct Point2D {
    T X, Y;
};

template <typename T>
struct Point3D {
    T X, Y, Z;
};

// Rct<short>: the original object starts with a vtable pointer (+0x0) that is
// omitted here; the four shorts sit at +0x4..+0xa as in the original.
template <typename T>
struct Rct {
    T X1, Y1, X2, Y2;
};
