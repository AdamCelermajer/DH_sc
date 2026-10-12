// IMaterialParameters<Owner, Header> member bodies. Each body is shared by the
// three instances; the address comments name the instance that each address
// belongs to.
#include "glitch-video/material_parameters.hpp"

#include <type_traits>

namespace glitch::video::detail {

// setDirty. G 0x5ba0d0 and R 0x5cef90 are empty; M 0x5c60c8 writes the dirty words.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::setDirty() {
    if constexpr (kTrackDirty) {
        m_dirty0 = 0xffffffffu;
        m_dirty1 = 0xffffffffu;
    }
}

// setParameterAt(int*, float). G 0x5ba0d4 and R 0x5cef94 store only.
// M 0x5c60d8 marks the dirty words when the stored int changes.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::setParameterAt(int32_t* dst, float value) {
    int32_t next = static_cast<int32_t>(value);
    if constexpr (kTrackDirty) {
        if (next != *dst) setDirty();
    }
    *dst = next;
}

// setParameterAt(float*, int). G 0x5ba0ec and R 0x5cefac store only.
// M 0x5c6108 marks the dirty words when the stored float changes.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::setParameterAt(float* dst, int32_t value) {
    float next = static_cast<float>(value);
    if constexpr (kTrackDirty) {
        if (!(next == *dst)) setDirty();
    }
    *dst = next;
}

// setParameterAt(vector4d*, SColorf const&). G 0x5ba114 and R 0x5cefd4 copy only.
// M 0x5c6140 compares the four words exactly, marks the dirty words on any change,
// then copies.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::setParameterAt(core::vector4d<float>* dst, const SColorf& value) {
    if constexpr (kTrackDirty) {
        if (!(dst->X == value.r && dst->Y == value.g && dst->Z == value.b && dst->W == value.a)) setDirty();
    }
    dst->X = value.r;
    dst->Y = value.g;
    dst->Z = value.b;
    dst->W = value.a;
}

// setParameterAt(SColorf*, vector4d const&). G 0x5ba104 and R 0x5cefc4 copy only.
// M 0x5caf10: partial. It compares with SColorf::equals (an epsilon compare whose
// extra argument is not recovered); this port compares exactly.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::setParameterAt(SColorf* dst, const core::vector4d<float>& value) {
    if constexpr (kTrackDirty) {
        if (!(dst->r == value.X && dst->g == value.Y && dst->b == value.Z && dst->a == value.W)) setDirty();
    }
    dst->r = value.X;
    dst->g = value.Y;
    dst->b = value.Z;
    dst->a = value.W;
}

// getParameterAt(int const*, float&). G 0x5ba138, M 0x5c61d8, R 0x5ceff8.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::getParameterAt(const int32_t* src, float& value) const {
    value = static_cast<float>(*src);
}

// getParameterAt(float const*, int&). G 0x5ba150, M 0x5c61f0, R 0x5cf010.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::getParameterAt(const float* src, int32_t& value) const {
    value = static_cast<int32_t>(*src);
}

// getParameterAt(SColorf const*, vector4d&). G 0x5ba168, M 0x5c6208, R 0x5cf028.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::getParameterAt(const SColorf* src, core::vector4d<float>& value) const {
    value.X = src->r;
    value.Y = src->g;
    value.Z = src->b;
    value.W = src->a;
}

// getParameterAt(vector4d const*, SColorf&). G 0x5ba18c, M 0x5c622c, R 0x5cf04c.
template <typename Owner, bool kTrackDirty>
void IMaterialParameters<Owner, kTrackDirty>::getParameterAt(const core::vector4d<float>* src, SColorf& value) const {
    value.r = src->X;
    value.g = src->Y;
    value.b = src->Z;
    value.a = src->W;
}

// Instances: G = CGlobalMaterialParameterManager (no tracking), M = CMaterial (tracking),
// R = CMaterialRenderer (no tracking).
template class IMaterialParameters<CGlobalMaterialParameterManager, false>;
template class IMaterialParameters<CMaterial, true>;
template class IMaterialParameters<CMaterialRenderer, false>;

}  // namespace glitch::video::detail
