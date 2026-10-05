#include "procedural_random_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
namespace {
std::int32_t signed_bits(std::uint32_t bits)noexcept {
    if(bits<=0x7fffffffU)return static_cast<std::int32_t>(bits);
    return -1-static_cast<std::int32_t>(0xffffffffU-bits);
}
}
std::uint32_t ProceduralRandomV1::next()noexcept {
    // Original 0x483a94: 32-bit wrapping increment, UMULL by 0x10a860c1,
    // unsigned 64-bit remainder with modulus 0xfffffffb. Wrapping the
    // multiplication before the remainder would change the sequence.
    const std::uint32_t increment=state_+1U;
    state_=static_cast<std::uint32_t>((std::uint64_t(increment)*0x10a860c1U)%0xfffffffbULL);
    return state_;
}
std::int32_t ProceduralRandomV1::between(std::int32_t minimum,std::int32_t maximum)noexcept {
    if(minimum>=maximum)return minimum;
    const std::uint32_t width=std::uint32_t(maximum)-std::uint32_t(minimum);
    return signed_bits(next()%width+std::uint32_t(minimum));
}
std::uint32_t ProceduralRandomV1::bounded(std::uint32_t exclusive) {
    if(!exclusive)throw std::invalid_argument("Zero procedural shuffle bound");
    return next()%exclusive;
}
std::uint32_t ProceduralRandomV1::hash(std::string_view bytes)noexcept {
    std::uint32_t value=5381;
    for(unsigned char byte:bytes){if(!byte)break;value=value*33U+byte;}
    return value;
}
}
