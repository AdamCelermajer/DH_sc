// Owner G typed rows: IMaterialParametersTyped<SGlobalParameterOwner> (see material_parameters_bind_g.hpp).
//
// The member templates of the mixin are defined here and instantiated explicitly for G at the end of the
// file. Each body forwards to one free function of part 1a (the table view) or part 1b (the store), chosen
// by the value type. A value type with no free function (the intrusive_ptr rows 0x5bf7dc, 0x5beaa4, 0x5c2198,
// 0x5c24d8, 0x5be68c, 0x5c3f40, 0x5c0c94, 0x5c0bd4, which part 1a lists as blocked) is not instantiated, so
// it is not bound and has no body.
//
// Stride of the pointer forms: bytes, as in part 1a and 1b. Pointer slots of intrusive_ptr are read and
// written as raw ITexture* / CLight* of sizeof(void*) bytes (the port's intrusive_ptr is pointer sized).
#include "glitch-video/material_parameters_bind_g.hpp"

#include "glitch-video/light.hpp"
#include "glitch-video/texture.hpp"

namespace glitch::video::detail {

using glitch::core::vector2d;
using glitch::core::vector3d;
using glitch::core::vector4d;
using glitch::video::SColor;
using glitch::video::SColorf;

// ---- table entries of the struct (the part 1a view is rebuilt from the mirror, the part 1b store is the source)

void SGlobalParameterOwner::defineEntry(std::uint16_t id, std::uint8_t type, std::uint32_t count, std::uint32_t offset) {
    if (m_store.entries.size() <= id) {
        m_store.entries.resize(std::size_t(id) + 1);
        m_entries.resize(std::size_t(id) + 1);
    }
    part1b::ParamEntry& e = m_store.entries[id];
    e.set = true;
    e.type = type;
    e.count = count;
    e.offset = offset;

    part1a::SParamEntry& r = m_entries[id];
    r = part1a::SParamEntry{};
    r.ref = 1;
    r.type = type;
    r.count = count;
    r.offset = offset;
}

part1a::GlobalParamTable SGlobalParameterOwner::table() const {
    part1a::GlobalParamTable t;
    t.entries = m_entries.data();
    t.entryCount = static_cast<std::uint32_t>(m_entries.size());
    t.values = const_cast<std::uint8_t*>(m_store.values.data());
    return t;
}

namespace {

using Owner = SGlobalParameterOwner;
using boost::intrusive_ptr;

// ---- Pointer forms (T* out / const T* src, stride in bytes). Value types of part 1a.

template <class T>
int arrayGet(const Owner& g, std::uint16_t id, T* out, int stride) {
    return part1a::getParameter(g.table(), id, out, stride);
}
template <class T>
int arrayGetCvt(const Owner& g, std::uint16_t id, T* out, int stride) {
    return part1a::getParameterCvt(g.table(), id, out, stride);
}
template <class T>
int arraySet(Owner& g, std::uint16_t id, const T* src, int stride) {
    return part1a::setParameter(g.table(), id, src, stride);
}
template <class T>
int arraySetCvt(Owner& g, std::uint16_t id, const T* src, int stride) {
    return part1a::setParameterCvt(g.table(), id, src, stride);
}

// Pointer forms of the intrusive rows bound through part 1b (0x5c4fa8, 0x5c518c, 0x5c568c, 0x5c540c).
int arrayGetCvt(const Owner& g, std::uint16_t id, intrusive_ptr<ITexture>* out, int stride) {
    return part1b::getParameterArrayCvt(g.store(), id, reinterpret_cast<ITexture**>(out), stride);
}
int arrayGetCvt(const Owner& g, std::uint16_t id, intrusive_ptr<CLight>* out, int stride) {
    return part1b::getParameterArrayCvt(g.store(), id, reinterpret_cast<CLight**>(out), stride);
}
int arraySet(Owner& g, std::uint16_t id, const intrusive_ptr<CLight>* src, int stride) {
    return part1b::setParameterArray(g.store(), id, reinterpret_cast<CLight* const*>(src), stride);
}
int arraySetCvt(Owner& g, std::uint16_t id, const intrusive_ptr<CLight>* src, int stride) {
    return part1b::setParameterArrayCvt(g.store(), id, reinterpret_cast<CLight* const*>(src), stride);
}

// ---- Index forms (element index, T& / const T&). Value types of part 1a for the gets and the Cvt setters
// of four types; the other value types are part 1b.

template <class T>
int indexGet(const Owner& g, std::uint16_t id, std::uint32_t index, T& out) {
    return part1a::getParameter(g.table(), id, index, out);
}
template <class T>
int indexGetCvt(const Owner& g, std::uint16_t id, std::uint32_t index, T& out) {
    return part1a::getParameterCvt(g.table(), id, index, out);
}
// Own-type setters (part 1b): element index * sizeof(T).
template <class T>
int indexSet(Owner& g, std::uint16_t id, std::uint32_t index, const T& src) {
    return part1b::setParameter(g.store(), id, index, src);
}
// Cvt setters, part 1a types.
int indexSetCvt(Owner& g, std::uint16_t id, std::uint32_t index, const SColorf& src) {
    return part1a::setParameterCvt(g.table(), id, index, src);
}
int indexSetCvt(Owner& g, std::uint16_t id, std::uint32_t index, const SColor& src) {
    return part1a::setParameterCvt(g.table(), id, index, src);
}
int indexSetCvt(Owner& g, std::uint16_t id, std::uint32_t index, const vector4d<float>& src) {
    return part1a::setParameterCvt(g.table(), id, index, src);
}
int indexSetCvt(Owner& g, std::uint16_t id, std::uint32_t index, const vector3d<float>& src) {
    return part1a::setParameterCvt(g.table(), id, index, src);
}
// Cvt setters, part 1b types.
template <class T>
int indexSetCvt(Owner& g, std::uint16_t id, std::uint32_t index, const T& src) {
    return part1b::setParameterCvt(g.store(), id, index, src);
}

// Intrusive index rows bound through part 1b: 0x5c48a0, 0x5c5be0, 0x5c5a40, 0x5c58ec.
int indexSet(Owner& g, std::uint16_t id, std::uint32_t index, const intrusive_ptr<ITexture>& src) {
    return part1b::setParameter(g.store(), id, index, src.get());
}
int indexSetCvt(Owner& g, std::uint16_t id, std::uint32_t index, const intrusive_ptr<CLight>& src) {
    return part1b::setParameterCvt(g.store(), id, index, src.get());
}
int indexGet(const Owner& g, std::uint16_t id, std::uint32_t index, intrusive_ptr<CLight>& out) {
    CLight* p = out.get();
    const int ok = part1b::getParameter(g.store(), id, index, p);
    out = intrusive_ptr<CLight>(p);
    return ok;
}
int indexGetCvt(const Owner& g, std::uint16_t id, std::uint32_t index, intrusive_ptr<CLight>& out) {
    CLight* p = out.get();
    const int ok = part1b::getParameterCvt(g.store(), id, index, p);
    out = intrusive_ptr<CLight>(p);
    return ok;
}

}  // namespace

// ---- Mixin member bodies. Derived is SGlobalParameterOwner; the casts are the only owner-specific step.

#define GLITCH_BIND_G_SELF(name) const Owner& name = static_cast<const Owner&>(*this)
#define GLITCH_BIND_G_SELF_MUT(name) Owner& name = static_cast<Owner&>(*this)

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameter(uint16_t id, T* out, int stride) const {
    GLITCH_BIND_G_SELF(g);
    return arrayGet(g, id, out, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameter(uint16_t id, const T* src, int stride) {
    GLITCH_BIND_G_SELF_MUT(g);
    return arraySet(g, id, src, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameterCvt(uint16_t id, T* out, int stride) const {
    GLITCH_BIND_G_SELF(g);
    return arrayGetCvt(g, id, out, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameterCvt(uint16_t id, const T* src, int stride) {
    GLITCH_BIND_G_SELF_MUT(g);
    return arraySetCvt(g, id, src, stride) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameter(uint16_t id, uint32_t index, T& out) const {
    GLITCH_BIND_G_SELF(g);
    return indexGet(g, id, index, out) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameter(uint16_t id, uint32_t index, const T& src) {
    GLITCH_BIND_G_SELF_MUT(g);
    return indexSet(g, id, index, src) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::getParameterCvt(uint16_t id, uint32_t index, T& out) const {
    GLITCH_BIND_G_SELF(g);
    return indexGetCvt(g, id, index, out) != 0;
}

template <typename Derived>
template <typename T>
EnableIfValidMaterialParameter<T> IMaterialParametersTyped<Derived>::setParameterCvt(uint16_t id, uint32_t index, const T& src) {
    GLITCH_BIND_G_SELF_MUT(g);
    return indexSetCvt(g, id, index, src) != 0;
}

#undef GLITCH_BIND_G_SELF
#undef GLITCH_BIND_G_SELF_MUT

// ---- Explicit instantiation for G. Each line is one row of the typed table (see the address list in
// material_parameters_typed.hpp). The intrusive rows listed in the file header are not instantiated.

#define GLITCH_BIND_G_MIXIN IMaterialParametersTyped<SGlobalParameterOwner>

#define GLITCH_BIND_G_INSTANTIATE_VALUE(T)                                                          \
    template bool GLITCH_BIND_G_MIXIN::getParameter<T>(uint16_t, T*, int) const;                    \
    template bool GLITCH_BIND_G_MIXIN::getParameterCvt<T>(uint16_t, T*, int) const;                 \
    template bool GLITCH_BIND_G_MIXIN::setParameter<T>(uint16_t, const T*, int);                    \
    template bool GLITCH_BIND_G_MIXIN::setParameterCvt<T>(uint16_t, const T*, int);                 \
    template bool GLITCH_BIND_G_MIXIN::getParameter<T>(uint16_t, uint32_t, T&) const;               \
    template bool GLITCH_BIND_G_MIXIN::getParameterCvt<T>(uint16_t, uint32_t, T&) const;            \
    template bool GLITCH_BIND_G_MIXIN::setParameter<T>(uint16_t, uint32_t, const T&);               \
    template bool GLITCH_BIND_G_MIXIN::setParameterCvt<T>(uint16_t, uint32_t, const T&);

GLITCH_BIND_G_INSTANTIATE_VALUE(int)
GLITCH_BIND_G_INSTANTIATE_VALUE(vector2d<int>)
GLITCH_BIND_G_INSTANTIATE_VALUE(vector3d<int>)
GLITCH_BIND_G_INSTANTIATE_VALUE(vector4d<int>)
GLITCH_BIND_G_INSTANTIATE_VALUE(float)
GLITCH_BIND_G_INSTANTIATE_VALUE(vector2d<float>)
GLITCH_BIND_G_INSTANTIATE_VALUE(vector3d<float>)
GLITCH_BIND_G_INSTANTIATE_VALUE(vector4d<float>)
GLITCH_BIND_G_INSTANTIATE_VALUE(SColor)
GLITCH_BIND_G_INSTANTIATE_VALUE(SColorf)

// Intrusive rows bound through part 1b (8 of the 16 G intrusive rows).
template bool GLITCH_BIND_G_MIXIN::getParameterCvt<intrusive_ptr<ITexture>>(uint16_t, intrusive_ptr<ITexture>*, int) const;  // 0x5c4fa8
template bool GLITCH_BIND_G_MIXIN::getParameterCvt<intrusive_ptr<CLight>>(uint16_t, intrusive_ptr<CLight>*, int) const;      // 0x5c518c
template bool GLITCH_BIND_G_MIXIN::setParameter<intrusive_ptr<CLight>>(uint16_t, const intrusive_ptr<CLight>*, int);         // 0x5c568c
template bool GLITCH_BIND_G_MIXIN::setParameterCvt<intrusive_ptr<CLight>>(uint16_t, const intrusive_ptr<CLight>*, int);      // 0x5c540c
template bool GLITCH_BIND_G_MIXIN::setParameter<intrusive_ptr<ITexture>>(uint16_t, uint32_t, const intrusive_ptr<ITexture>&);  // 0x5c48a0
template bool GLITCH_BIND_G_MIXIN::setParameterCvt<intrusive_ptr<CLight>>(uint16_t, uint32_t, const intrusive_ptr<CLight>&);   // 0x5c5be0
template bool GLITCH_BIND_G_MIXIN::getParameter<intrusive_ptr<CLight>>(uint16_t, uint32_t, intrusive_ptr<CLight>&) const;      // 0x5c5a40
template bool GLITCH_BIND_G_MIXIN::getParameterCvt<intrusive_ptr<CLight>>(uint16_t, uint32_t, intrusive_ptr<CLight>&) const;   // 0x5c58ec

#undef GLITCH_BIND_G_INSTANTIATE_VALUE
#undef GLITCH_BIND_G_MIXIN

}  // namespace glitch::video::detail
