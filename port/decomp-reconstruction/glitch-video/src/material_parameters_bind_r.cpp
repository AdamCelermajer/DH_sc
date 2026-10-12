// Owner R typed rows: IMaterialParametersTyped<IMaterialParameters<CMaterialRenderer, false>>.
//
// The member templates and dispatchers of the mixin (material_parameters_typed.hpp) are defined here for the
// CMaterialRenderer instance and instantiated explicitly at the end of the file. Each body forwards to a
// free function of part d (SRendererValueTable view) or part 1c (SParameterTable view):
//   pointer forms, indexed Cvt getter, all array and indexed Cvt setters  -> part d, see below
//   indexed setters                                                       -> part 1c (instances shared with M)
//   indexed getParameter: int, vector2/3/4 int, float, vector2 float       -> part 1c
//                         vector3/4 float, SColor, SColorf                -> part d
// The owner has no dirty words, so the part 1c views carry no dirty pointers.
//
// Stride of the pointer forms: bytes (0 means contiguous), as in parts d and 1c.
//
// Intrusive pointer rows (codes 12..15 and 18) and the matrix code 11 are not bound. The dispatchers return 0
// for them, so the dispatchers are partial (see the ledger notes).
#include <cstdint>
#include <type_traits>

#include "glitch-video/material_parameters.hpp"
#include "glitch-video/material_parameters_part_1c.hpp"
#include "glitch-video/material_parameters_part_d.hpp"
#include "glitch-video/material_parameters_typed.hpp"

namespace glitch::video::detail {
namespace {

using RParams = IMaterialParameters<CMaterialRenderer, false>;
using glitch::core::vector2d;
using glitch::core::vector3d;
using glitch::core::vector4d;
using glitch::video::SColor;
using glitch::video::SColorf;

// Table views of the CMaterialRenderer instance. The typed getters are const, but no part d or part 1c getter
// writes through the matrix map or the value buffer, so the map is taken through const_cast in one place.
rparams::SRendererValueTable rendererTable(const RParams& r) {
    rparams::SRendererValueTable t;
    t.count = r.parameterHeader().count;
    t.entries = r.parameterHeader().entries;
    t.values = r.valueBuffer();
    t.matrices = &const_cast<RParams&>(r).matrixSlots();
    return t;
}

part1c::SParameterTable paramTable(const RParams& r) {
    part1c::SParameterTable t;
    t.header = &r.parameterHeader();
    t.values = r.valueBuffer();
    return t;  // dirtyFirst, dirtyLast and pointers stay null: R has no dirty words and no pointer rows here
}

// Value-type tag for the dispatchers: withNumericType calls fn(TypeTag<T>{}) for the stored type of a code.
template <class T>
struct TypeTag {
    using type = T;
};

using V2i = vector2d<int>;
using V3i = vector3d<int>;
using V4i = vector4d<int>;
using V2f = vector2d<float>;
using V3f = vector3d<float>;
using V4f = vector4d<float>;

// Codes 1..8, 16 and 17 (shader_value_type). Codes 11..15 and 18 are not numeric and are not bound.
template <class Fn>
int withNumericType(int32_t code, Fn&& fn) {
    switch (code) {
        case shader_value_type::kInt: return fn(TypeTag<int>{});
        case shader_value_type::kVector2dInt: return fn(TypeTag<V2i>{});
        case shader_value_type::kVector3dInt: return fn(TypeTag<V3i>{});
        case shader_value_type::kVector4dInt: return fn(TypeTag<V4i>{});
        case shader_value_type::kFloat: return fn(TypeTag<float>{});
        case shader_value_type::kVector2dFloat: return fn(TypeTag<V2f>{});
        case shader_value_type::kVector3dFloat: return fn(TypeTag<V3f>{});
        case shader_value_type::kVector4dFloat: return fn(TypeTag<V4f>{});
        case shader_value_type::kColor: return fn(TypeTag<SColor>{});
        case shader_value_type::kColorf: return fn(TypeTag<SColorf>{});
        default: return 0;
    }
}

// Indexed getParameter routes: part 1c instances for the first six types, part d for the rest.
template <class T>
constexpr bool kIndexedGetInPart1c = std::is_same_v<T, int> || std::is_same_v<T, V2i> || std::is_same_v<T, V3i> ||
                                     std::is_same_v<T, V4i> || std::is_same_v<T, float> || std::is_same_v<T, V2f>;

}  // namespace

// ---- Pointer forms (stride in bytes). All ten numeric types are in part d.

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameter(uint16_t id, T* out, int stride) const {
    const RParams& r = static_cast<const Derived&>(*this);
    return rparams::getParameter(rendererTable(r), id, out, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameter(uint16_t id, const T* src, int stride) {
    const RParams& r = static_cast<const Derived&>(*this);
    return rparams::setParameter(rendererTable(r), id, src, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameterCvt(uint16_t id, T* out, int stride) const {
    const RParams& r = static_cast<const Derived&>(*this);
    return rparams::getParameterCvt(rendererTable(r), id, out, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameterCvt(uint16_t id, const T* src, int stride) {
    const RParams& r = static_cast<const Derived&>(*this);
    return rparams::setParameterCvt(rendererTable(r), id, src, stride) != 0;
}

// ---- Indexed forms.

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameter(uint16_t id, uint32_t index, T& out) const {
    const RParams& r = static_cast<const Derived&>(*this);
    if constexpr (kIndexedGetInPart1c<T>) {
        return part1c::getParameter<T>(paramTable(r), id, index, out) != 0;
    } else {
        return rparams::getParameter(rendererTable(r), id, index, out) != 0;
    }
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameter(uint16_t id, uint32_t index, const T& src) {
    const RParams& r = static_cast<const Derived&>(*this);
    return part1c::setParameter<T>(paramTable(r), id, index, src) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameterCvt(uint16_t id, uint32_t index, T& out) const {
    const RParams& r = static_cast<const Derived&>(*this);
    return rparams::getParameterCvt(rendererTable(r), id, index, out) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameterCvt(uint16_t id, uint32_t index, const T& src) {
    const RParams& r = static_cast<const Derived&>(*this);
    return part1c::setParameterCvt<T>(paramTable(r), id, index, src) != 0;
}

// ---- Dispatchers. Numeric codes forward to the typed members above. Codes 11..15 and 18 return 0 (not bound).

template <typename Derived>
int IMaterialParametersTyped<Derived>::getParameter(uint16_t id, E_SHADER_PARAMETER_VALUE_TYPE type, void* out,
                                                    int stride) const {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->getParameter(id, static_cast<T*>(out), stride) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::getParameterCvt(uint16_t id, E_SHADER_PARAMETER_VALUE_TYPE type, void* out,
                                                       int stride) const {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->getParameterCvt(id, static_cast<T*>(out), stride) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::setParameter(uint16_t id, E_SHADER_PARAMETER_VALUE_TYPE type, const void* src,
                                                    int stride) {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->setParameter(id, static_cast<const T*>(src), stride) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::setParameterCvt(uint16_t id, E_SHADER_PARAMETER_VALUE_TYPE type, const void* src,
                                                       int stride) {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->setParameterCvt(id, static_cast<const T*>(src), stride) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::getParameter(uint16_t id, uint32_t index, E_SHADER_PARAMETER_VALUE_TYPE type,
                                                    void* out) const {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->getParameter(id, index, *static_cast<T*>(out)) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::getParameterCvt(uint16_t id, uint32_t index, E_SHADER_PARAMETER_VALUE_TYPE type,
                                                       void* out) const {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->getParameterCvt(id, index, *static_cast<T*>(out)) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::setParameter(uint16_t id, uint32_t index, E_SHADER_PARAMETER_VALUE_TYPE type,
                                                    const void* src) {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->setParameter(id, index, *static_cast<const T*>(src)) ? 1 : 0;
    });
}

template <typename Derived>
int IMaterialParametersTyped<Derived>::setParameterCvt(uint16_t id, uint32_t index, E_SHADER_PARAMETER_VALUE_TYPE type,
                                                       const void* src) {
    return withNumericType(type, [&](auto tag) -> int {
        using T = typename decltype(tag)::type;
        return this->setParameterCvt(id, index, *static_cast<const T*>(src)) ? 1 : 0;
    });
}

// ---- Explicit instantiation for R. Each line is one row of the typed table (addresses in
// material_parameters_typed.hpp). The explicit instantiation of the class adds the eight dispatchers.

#define GLITCH_BIND_R_MIXIN IMaterialParametersTyped<RParams>

#define GLITCH_BIND_R_INSTANTIATE_VALUE(T)                                                     \
    template bool GLITCH_BIND_R_MIXIN::getParameter<T>(uint16_t, T*, int) const;               \
    template bool GLITCH_BIND_R_MIXIN::getParameterCvt<T>(uint16_t, T*, int) const;            \
    template bool GLITCH_BIND_R_MIXIN::setParameter<T>(uint16_t, const T*, int);               \
    template bool GLITCH_BIND_R_MIXIN::setParameterCvt<T>(uint16_t, const T*, int);            \
    template bool GLITCH_BIND_R_MIXIN::getParameter<T>(uint16_t, uint32_t, T&) const;          \
    template bool GLITCH_BIND_R_MIXIN::getParameterCvt<T>(uint16_t, uint32_t, T&) const;       \
    template bool GLITCH_BIND_R_MIXIN::setParameter<T>(uint16_t, uint32_t, const T&);          \
    template bool GLITCH_BIND_R_MIXIN::setParameterCvt<T>(uint16_t, uint32_t, const T&);

GLITCH_BIND_R_INSTANTIATE_VALUE(int)
GLITCH_BIND_R_INSTANTIATE_VALUE(V2i)
GLITCH_BIND_R_INSTANTIATE_VALUE(V3i)
GLITCH_BIND_R_INSTANTIATE_VALUE(V4i)
GLITCH_BIND_R_INSTANTIATE_VALUE(float)
GLITCH_BIND_R_INSTANTIATE_VALUE(V2f)
GLITCH_BIND_R_INSTANTIATE_VALUE(V3f)
GLITCH_BIND_R_INSTANTIATE_VALUE(V4f)
GLITCH_BIND_R_INSTANTIATE_VALUE(SColor)
GLITCH_BIND_R_INSTANTIATE_VALUE(SColorf)

template class GLITCH_BIND_R_MIXIN;

#undef GLITCH_BIND_R_INSTANTIATE_VALUE
#undef GLITCH_BIND_R_MIXIN

}  // namespace glitch::video::detail
