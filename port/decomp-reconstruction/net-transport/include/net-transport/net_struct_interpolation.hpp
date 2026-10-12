// NetStructInterpolation<float> and NetStructInterpolatedRangedFloat<Bits, Min, Max>:
// replicated floats that smooth received values over time. AddValue keeps a ring of
// (time, value) samples and a running average; the wrap-around boundary fields of the original
// are not recovered (see AddValue), so the ring is kept by count only.
#pragma once

#include <cstdint>

#include "net-transport/net_struct.hpp"

template <typename T>
class NetStructInterpolation : public NetStructMember {
public:
    ~NetStructInterpolation() override = default;   // 0x3a3578 _ZN22NetStructInterpolationIfED1Ev (float instance)

    // 0x3a3e64 _ZN22NetStructInterpolationIfE8AddValueEfj (partial, see the body).
    // Adds a sample and returns 20, the constant the original returns on every path.
    int AddValue(T value, unsigned int time) {
        const unsigned int frame_time = GetFrameTime();
        unsigned int count = count_;
        int total = total_;
        // The window (+0xbc, 40 by default) limits the count: the oldest share is taken out of the
        // total until the count is below the window.
        if (count >= window_) {
            do {
                const int share = total / static_cast<int>(count--);
                total -= share;
            } while (window_ <= count);
            total_ = total;
            count_ = count;
        }
        // The sample's age: frame time minus its time stamp.
        const int age = static_cast<int>(frame_time - time);
        ++count;
        count_ = count;
        total_ = age + total;
        average_ = total_ / static_cast<int>(count);
        // The new sample goes into the ring at the head; the head then moves forward (20 slots).
        samples_[head_] = Sample{time, value};
        head_ = (head_ + 1) % kSamples;
        return 20;
    }

protected:
    static constexpr unsigned kSamples = 20;   // ring length (40 words of (time, value) pairs)

    struct Sample {
        unsigned int time;                     // word 2k+1 of the ring
        T value;                               // word 2k+2 of the ring
    };

    // NetStructMember::GetFrameTime (not reconstructed in this category). The stand-in returns 0.
    unsigned int GetFrameTime() const { return 0; }

    Sample samples_[kSamples] = {};            // +0x04..+0x9f (relative to the NetStructInterpolation base)
    unsigned int window_ = 40;                 // word 47 (set to 40 by the constructor)
    unsigned int count_ = 0;                   // word 49
    int total_ = 0;                            // word 48
    int average_ = 0;                          // word 50 (read by the interpolation getter, not in this slice)
    unsigned int head_ = 0;                    // word 46
};

template <unsigned Bits, int Min, int Max>
class NetStructInterpolatedRangedFloat : public NetStructMemberType<float>, public NetStructInterpolation<float> {
public:
    // 0x3a681c / 0x3a6920 _ZN32NetStructInterpolatedRangedFloat...C1Ef (partial: the change
    // notification of the base, NetStructMember::SetChanged, is not in this port).
    explicit NetStructInterpolatedRangedFloat(float value);
    ~NetStructInterpolatedRangedFloat() override = default;

    // 0x3a3f50 / 0x3a3f60 _ZN32NetStructInterpolatedRangedFloat...8PostLoadEj: feeds the current
    // value to the interpolation with the given time. Returns AddValue's result (20).
    int PostLoad(unsigned int time);
};

template <unsigned Bits, int Min, int Max>
NetStructInterpolatedRangedFloat<Bits, Min, Max>::NetStructInterpolatedRangedFloat(float value) {
    // The original sets the value and calls SetChanged when it differs from the stored value
    // (the stored value starts at zero), then zeroes the sample ring and sets the window to 40.
    // SetChanged is not reconstructed, so only the value is set here.
    this->value_ = value;
}

template <unsigned Bits, int Min, int Max>
int NetStructInterpolatedRangedFloat<Bits, Min, Max>::PostLoad(unsigned int time) {
    return this->AddValue(this->value_, time);
}

// The two instantiations the binary uses. Their destructors are the empty virtual
// destructors at the addresses below (the deleting variants are compiler-generated).
//   0x3a2f24 _ZN32NetStructInterpolatedRangedFloatILj18ELin200000ELi200000EED1Ev
//   0x3a2f34 _ZN32NetStructInterpolatedRangedFloatILj16ELin50000ELi50000EED1Ev
using NetStructInterpolatedRangedFloat18 = NetStructInterpolatedRangedFloat<18, -200000, 200000>;
using NetStructInterpolatedRangedFloat16 = NetStructInterpolatedRangedFloat<16, -50000, 50000>;
