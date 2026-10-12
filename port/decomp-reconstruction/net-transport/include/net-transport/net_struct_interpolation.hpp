// NetStructInterpolation<float> and NetStructInterpolatedRangedFloat<Bits, Min, Max>:
// replicated floats that smooth received values over time. Only the destructors are
// reconstructed here; AddValue (the decompile is cut short by a noreturn __aeabi_idiv
// call) and the constructors and PostLoad are still open.
#pragma once

#include "net-transport/net_struct.hpp"

template <typename T>
class NetStructInterpolation : public NetStructMember {
public:
    ~NetStructInterpolation() override = default;   // 0x3a3578 _ZN22NetStructInterpolationIfED1Ev (float instance)
};

template <unsigned Bits, int Min, int Max>
class NetStructInterpolatedRangedFloat : public NetStructMemberType<float>, public NetStructInterpolation<float> {
public:
    ~NetStructInterpolatedRangedFloat() override = default;
};

// The two instantiations the binary uses. Their destructors are the empty virtual
// destructors at the addresses below (the deleting variants are compiler-generated).
//   0x3a2f24 _ZN32NetStructInterpolatedRangedFloatILj18ELin200000ELi200000EED1Ev
//   0x3a2f34 _ZN32NetStructInterpolatedRangedFloatILj16ELin50000ELi50000EED1Ev
using NetStructInterpolatedRangedFloat18 = NetStructInterpolatedRangedFloat<18, -200000, 200000>;
using NetStructInterpolatedRangedFloat16 = NetStructInterpolatedRangedFloat<16, -50000, 50000>;
