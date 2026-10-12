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
