// Point3D<T>: a three-component point or vector, X, Y and Z. Point3D<float> is the game's world-space position
// and direction type (GameObject, Character, PFWorld, PFObject). Point3D<int> is also instantiated (the
// StrToObj and ObjToStr overloads).
// Original 32-bit size 0xc (12) for Point3D<float>.
// Reconstructed from libDungeonHunter2.so pseudocode (operator== 0x312b6c, transform 0x312da8, rotateXY 0x3e5740).
#pragma once

namespace glitch {
namespace core {
template <typename T>
class CMatrix4;
}  // namespace core
}  // namespace glitch

template <typename T>
struct Point3D {
    bool operator==(const Point3D& other) const;                            // 0x312b6c, each component within 0.0001
    void transform(const glitch::core::CMatrix4<float>& matrix);            // 0x312da8, multiplies (X, Y, Z, 1) by the matrix; X and Y take the result and Z is restored
    float angleCos(const Point3D& other) const;                             // 0x312f40 cosine of the angle between the two vectors
    float angle(const Point3D& other) const;                                // 0x313058 acos(angleCos)
    Point3D& operator/=(const float& divisor);                              // 0x34d04c divides each component
    void normalize();                                                       // 0x34d0b0 divides by the length (through operator/=)
    float distance(const Point3D& other) const;                             // 0x3a4118 Euclidean distance
    Point3D& rotateXY(float angle);                                         // 0x3e5740 rotates the X and Y components, angle in radians
    float rotateXYBy(float degrees, const Point3D& pivot);                  // 0x418c48 rotates about pivot in degrees, returns the new Y

    T X;                                                                    // +0x00
    T Y;                                                                    // +0x04
    T Z;                                                                    // +0x08
};
