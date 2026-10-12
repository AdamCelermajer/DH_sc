#include "glitch-video/material_technique_maps_reader.hpp"

#include "glitch-video/material_renderer_manager.hpp"

namespace glitch::video {

// 0x5df8ec _ZN6glitch5video28IMaterialTechniqueMapsReaderC1ERNS0_24CMaterialRendererManagerE
// 0x5df94c _ZN6glitch5video28IMaterialTechniqueMapsReaderC2ERNS0_24CMaterialRendererManagerE
IMaterialTechniqueMapsReader::IMaterialTechniqueMapsReader(CMaterialRendererManager& manager)
    : m_manager(&manager),
      m_unknown10(0xFFFF),
      m_unknown15(true) {
    m_mapIds[0] = -1;
    m_mapIds[1] = -1;
    m_mapGroupActive[0] = false;
    m_mapGroupActive[1] = false;
    m_unknown14 = false;
    m_error = false;
    m_unknown18 = nullptr;
}

// 0x5df8c8 _ZN6glitch5video28IMaterialTechniqueMapsReader8setErrorEPKcS3_
// Partial: the error hook (vtable +0x28) is not modelled; the flag is set and the strings are dropped.
void IMaterialTechniqueMapsReader::setError(const char*, const char*) {
    m_error = true;
}

}  // namespace glitch::video
