#include "core-platform/accelerometer.hpp"

#include <cmath>

namespace {
// The listing stores the float bit patterns 0x3FC90FDB (+pi/2) and 0xBFC90FDB
// (-pi/2) when the other component is too small for atan2. The sign comes from
// the reference angle: <= 0 gives -pi/2.
constexpr float kHalfPi = 1.57079637f;
constexpr float kMinusHalfPi = -1.57079637f;

float QuarterTurnFor(float reference) { return reference <= 0.0f ? kMinusHalfPi : kHalfPi; }
}  // namespace

// 0x33da0c / 0x33da64 AccelerometerBase::AccelerometerBase()
// Zeroes the members (the default initialisers), calls _calcAngles() and clear().
AccelerometerBase::AccelerometerBase() {
    _calcAngles();
    clear();
}

// 0x33d720 AccelerometerBase::clear()
void AccelerometerBase::clear() {
    prevX_ = x_;
    prevY_ = y_;
    prevZ_ = z_;
    minX_ = z_;
    maxX_ = z_;
}

// 0x33d744 AccelerometerBase::update(double): a thunk to clear().
void AccelerometerBase::update(double) { clear(); }

// 0x33d8dc AccelerometerBase::moved(float, float, float)
// Stores the sample, lowers the min and raises the max of each axis, and
// recomputes the angles. Partial: the listing then raises an event (type 6 with
// the three components) through EventManager; that call is not reproduced here.
void AccelerometerBase::moved(float x, float y, float z) {
    x_ = x;
    y_ = y;
    z_ = z;
    minX_ = minX_ > x ? x : minX_;
    maxX_ = maxX_ >= x ? maxX_ : x;
    minY_ = minY_ > y ? y : minY_;
    maxY_ = maxY_ >= y ? maxY_ : y;
    minZ_ = minZ_ > z ? z : minZ_;
    maxZ_ = maxZ_ >= z ? maxZ_ : z;
    _calcAngles();
}

// 0x33d798 AccelerometerBase::_calcAngles()
// Each angle is atan2 of its pair when the first component is at least 0.001 in
// magnitude; otherwise it is +-pi/2 by the sign of the reference angle (the previous
// X angle for the first angle, the new X angle for the last one).
void AccelerometerBase::_calcAngles() {
    prevAngleX_ = angleX_;
    if (std::fabs(x_) >= 0.001) {
        angleX_ = std::atan2(y_, x_);
    } else {
        angleX_ = QuarterTurnFor(prevAngleX_);
    }

    prevAngleY_ = angleY_;
    if (std::fabs(y_) >= 0.001) {
        angleY_ = std::atan2(z_, y_);
    } else {
        angleY_ = QuarterTurnFor(prevAngleY_);
    }

    angleXCopy_ = angleX_;
    if (std::fabs(z_) >= 0.001) {
        angleZ_ = std::atan2(x_, z_);
    } else {
        angleZ_ = QuarterTurnFor(angleXCopy_);
    }
}

// 0x33dbf8 / 0x33dc2c AccelerometerIPhone::AccelerometerIPhone()
AccelerometerIPhone::AccelerometerIPhone() : AccelerometerBase() {}

// 0x33db64 AccelerometerIPhone::moved(float, float, float)
// The original adds 0x80000000 to each float's bit pattern, which flips the sign.
void AccelerometerIPhone::moved(float x, float y, float z) {
    AccelerometerBase::moved(-x, -y, -z);
}
