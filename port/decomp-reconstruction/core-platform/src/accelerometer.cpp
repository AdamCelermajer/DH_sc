#include "core-platform/accelerometer.hpp"

// 0x33da0c / 0x33da64 AccelerometerBase::AccelerometerBase()
// Partial: the original zeroes the members, calls _calcAngles() and clear(); the
// exact order of the zeroing stores is not fully checked.
AccelerometerBase::AccelerometerBase() {
    _calcAngles();
    clear();
}

// 0x33d720 AccelerometerBase::clear()
void AccelerometerBase::clear() {
    zCopy_ = z_;
    prevX_ = x_;
    prevY_ = y_;
    prevZ_ = z_;
    zCopy2_ = z_;
}

// 0x33d744 AccelerometerBase::update(double)
void AccelerometerBase::update(double) {
    zCopy_ = z_;
    prevX_ = x_;
    prevY_ = y_;
    prevZ_ = z_;
    zCopy2_ = z_;
}

// 0x33d8dc AccelerometerBase::moved(float, float, float)
// Partial: stores the sample, then tail-calls a float comparison whose result
// is not recovered.
void AccelerometerBase::moved(float x, float y, float z) {
    z_ = z;
    x_ = x;
    y_ = y;
}

// 0x33d798 AccelerometerBase::_calcAngles()
// Partial: only the first store and a tail call to a float comparison are visible.
void AccelerometerBase::_calcAngles() {
    angle_ = angleSrc_;
}

// 0x33dbf8 / 0x33dc2c AccelerometerIPhone::AccelerometerIPhone()
AccelerometerIPhone::AccelerometerIPhone() : AccelerometerBase() {}

// 0x33db64 AccelerometerIPhone::moved(float, float, float)
// The original adds 0x80000000 to each float's bit pattern, which flips the sign.
void AccelerometerIPhone::moved(float x, float y, float z) {
    AccelerometerBase::moved(-x, -y, -z);
}
