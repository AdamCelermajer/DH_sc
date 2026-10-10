#include "pcm_wav_v2.hpp"

namespace dh::foundation::startup {
namespace {

void put_u32(std::vector<std::uint8_t>& out, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
}
void put_u16(std::vector<std::uint8_t>& out, std::uint16_t v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
}


} // namespace

std::vector<std::uint8_t> make_pcm16_wav(const std::vector<std::int16_t>& pcm, std::uint32_t rate, std::uint32_t channels) {
    const std::uint32_t dataBytes = static_cast<std::uint32_t>(pcm.size() * 2);
    std::vector<std::uint8_t> out;
    out.reserve(44 + dataBytes);
    out.insert(out.end(), {'R', 'I', 'F', 'F'});
    put_u32(out, 36 + dataBytes);
    out.insert(out.end(), {'W', 'A', 'V', 'E', 'f', 'm', 't', ' '});
    put_u32(out, 16);
    put_u16(out, 1);                                   // PCM
    put_u16(out, static_cast<std::uint16_t>(channels));
    put_u32(out, rate);
    put_u32(out, rate * channels * 2);                 // byte rate
    put_u16(out, static_cast<std::uint16_t>(channels * 2));  // block align
    put_u16(out, 16);                                  // bits
    out.insert(out.end(), {'d', 'a', 't', 'a'});
    put_u32(out, dataBytes);
    for (std::int16_t s : pcm) put_u16(out, static_cast<std::uint16_t>(s));
    return out;
}

} // namespace dh::foundation::startup
