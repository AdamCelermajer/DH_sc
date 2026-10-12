// AccelerometerBase: last reading plus the previous sample and derived angles.
// Field offsets are from the original; the roles of the copies are inferred.
#pragma once

class AccelerometerBase {
public:
    AccelerometerBase();                         // 0x33da0c / 0x33da64
    virtual ~AccelerometerBase() = default;      // compiler-generated
    virtual void clear();                        // 0x33d720
    virtual void update(double dt);              // 0x33d744: copies the sample forward
    virtual void moved(float x, float y, float z);  // 0x33d8dc

private:
    void _calcAngles();                          // 0x33d798

    float x_ = 0;        // +0x4
    float y_ = 0;        // +0x8
    float z_ = 0;        // +0xc
    float prevX_ = 0;    // +0x10
    float prevY_ = 0;    // +0x14
    float prevZ_ = 0;    // +0x18
    float zCopy_ = 0;    // +0x1c
    float zCopy2_ = 0;   // +0x28
    float angle_ = 0;    // +0x34
    float angleSrc_ = 0; // +0x38
};

class AccelerometerIPhone : public AccelerometerBase {
public:
    AccelerometerIPhone();                       // 0x33dbf8 / 0x33dc2c
    void moved(float x, float y, float z) override;  // 0x33db64: sign-flipped sample
};
