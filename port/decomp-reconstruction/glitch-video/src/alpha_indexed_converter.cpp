#include "glitch-video/alpha_indexed_converter.hpp"

namespace glitch::video::pixel_format {

namespace {

// ARM register shifts of 32 or more give 0 (NOTES, Wave 5). C++ shifts that large are undefined.
uint32_t lsrOrZero(uint32_t value, uint8_t amount) {
    return amount >= 32 ? 0u : (value >> amount);
}

uint32_t lslOrZero(uint32_t value, uint8_t amount) {
    return amount >= 32 ? 0u : (value << amount);
}

}  // namespace

// 0x5eeae4 _ZN6glitch5video12pixel_format12_GLOBAL__N_1...SAlphaIndexedPackedConverterC2ENS0_14E_PIXEL_FORMATESB_
// Partial: see the header. The table-driven setup is not translated.
SAlphaIndexedPackedConverter::SAlphaIndexedPackedConverter(E_PIXEL_FORMAT /*source*/,
                                                           E_PIXEL_FORMAT /*target*/) {}

// 0x5ededc _ZNK6glitch5video12pixel_format12_GLOBAL__N_1...SAlphaIndexedPackedConverterclEj
uint32_t SAlphaIndexedPackedConverter::operator()(uint32_t pixel) const {
    // Each of the three output terms combines a group-0 and a group-1 channel, then applies its mask.
    uint32_t terms[3];
    for (int k = 0; k < 3; ++k) {
        const uint32_t group0 = lslOrZero(lsrOrZero(pixel & m_inputMask[0][k], m_rightShift[0][k]),
                                          m_leftShift[0][k]);
        const uint32_t group1 = lslOrZero(lsrOrZero(pixel & m_inputMask[1][k], m_rightShift[1][k]),
                                          m_leftShift[1][k]);
        terms[k] = (group0 | group1) & m_resultMask[k];
    }

    // Lookup term: the masked pixel indexes a 16-bit table; the entry is shifted and masked.
    const uint32_t index = lsrOrZero(pixel & m_lookupMask, m_lookupIndexShift);
    const int32_t entry = static_cast<int32_t>(m_lookupTable[index]);
    const uint32_t lookup = static_cast<uint32_t>(entry >> m_lookupResultShift) << m_lookupResultLeftShift;

    return terms[0] | terms[1] | m_orValue | terms[2] | (m_fixedBits & lookup);
}

}  // namespace glitch::video::pixel_format
