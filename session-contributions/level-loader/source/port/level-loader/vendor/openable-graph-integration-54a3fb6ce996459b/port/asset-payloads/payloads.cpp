#include "payloads.hpp"
#include <cstring>
#include <initializer_list>
#include <limits>

using namespace dh2::assets;
using dh2::resources::Library;
namespace {
std::uint32_t word(const std::uint8_t* p) {
    return p[0] | (std::uint32_t(p[1]) << 8) | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
std::uint16_t half(const std::uint8_t* p) { return p[0] | (std::uint16_t(p[1]) << 8); }
std::int32_t signed_word(std::uint32_t w) {
    std::int32_t v; std::memcpy(&v, &w, 4); return v;
}
bool span(const BresView& v, std::uint64_t o, std::uint64_t n) { return v.bytes && o <= v.size && n <= v.size - o; }
const std::uint8_t* at(const BresView& v, std::uint32_t o, std::uint64_t n) { return span(v, o, n) ? v.bytes + o : nullptr; }
const char* text(const BresView& v, std::uint32_t o) {
    if (!o || !span(v, o, 1)) return nullptr;
    for (std::size_t i = o; i < v.size; ++i)
        if (!v.bytes[i]) return reinterpret_cast<const char*>(v.bytes + o);
    return nullptr;
}
constexpr std::uint32_t widths[] = {1, 1, 2, 2, 4, 4, 4};
constexpr std::uint32_t primitive_map[] = {6, 4, 3, 1, 2};
const std::uint8_t* buffer(const BresView& v, std::uint32_t offset, std::uint64_t size) {
    const auto* root = at(v, v.root_offset, 192);
    if (!root) return nullptr;
    // CMesh's root+0x64 branch wraps vertices and indices in 16-byte
    // onDemand<T> records: refcount, file offset, byte size, cached pointer.
    // Complete BRES files contain those deferred bytes too; borrow them.
    if (signed_word(word(root + 100)) > 0) {
        const auto* d = at(v, offset, 16);
        if (!d || word(d + 12) || size > (word(d + 8) & ~3U)) return nullptr;
        return at(v, word(d + 4), size);
    }
    return at(v, offset, size);
}
bool scalar(const std::uint8_t* p, std::uint32_t type, float& value) {
    switch (type) {
    case 0: value = p[0] < 128 ? p[0] : int(p[0]) - 256; return true;
    case 1: value = p[0]; return true;
    case 2: value = half(p) < 32768 ? half(p) : int(half(p)) - 65536; return true;
    case 3: value = half(p); return true;
    case 4: value = static_cast<float>(signed_word(word(p))); return true;
    case 5: value = static_cast<float>(word(p)); return true;
    case 6: { const auto w = word(p); std::memcpy(&value, &w, 4); return true; }
    default: return false;
    }
}
const std::uint8_t* record(const Animation* a) { return a ? at(a->image, a->record, 32) : nullptr; }
const std::uint8_t* sampler(const Animation* a, std::int32_t i) {
    const auto* r = record(a);
    if (!r || i < 0 || std::uint32_t(i) >= word(r + 4)) return nullptr;
    const auto o = std::uint64_t(word(r + 8)) + std::uint64_t(std::uint32_t(i)) * 28;
    return span(a->image, o, 28) ? a->image.bytes + o : nullptr;
}
const std::uint8_t* optional_value(const Animation* a, std::uint32_t slot, std::uint32_t field) {
    const auto* r = record(a);
    if (!r || !word(r + slot)) return nullptr;
    const auto* v = at(a->image, word(r + slot), 12);
    return v && word(v + field) ? at(a->image, word(v + field), 1) : nullptr;
}
bool vector(const Animation* a, const std::uint8_t* s, bool output, Vector& v) {
    if (!a || !s) return false;
    const auto i = word(s + (output ? 24 : 12));
    if (i >= a->entries) return false;
    const auto slot = std::uint64_t(a->data) + 8 + std::uint64_t(i) * 8;
    if (!span(a->image, slot - 4, 8)) return false;
    const auto* e = a->image.bytes + slot;
    const auto target = std::int64_t(slot) + signed_word(word(e));
    const auto type = word(s + (output ? 16 : 4));
    const auto components = word(s + (output ? 20 : 8));
    const auto count = word(e - 4);
    if (type > 6 || !components || components > 16) return false;
    const auto size = std::uint64_t(count) * components * widths[type];
    if (target < a->data || std::uint64_t(target) > std::uint64_t(a->data) + a->data_size
        || size > std::uint64_t(a->data) + a->data_size - std::uint64_t(target)) return false;
    v = {a->image.bytes + target, count, type, components};
    return true;
}
std::int32_t raw_time(const Vector& v, std::uint32_t i) {
    if (v.type == 1) return v.data[i];
    if (v.type == 3) return half(v.data + std::size_t(i) * 2);
    return signed_word(word(v.data + std::size_t(i) * 4));
}
// The original uses float-to-int truncation here. Reject an unrepresentable
// conversion rather than invoking C++ undefined behavior on hostile input.
bool truncated(float f, std::int32_t& result) {
    if (!(f >= -2147483648.0f && f < 2147483648.0f)) return false;
    result = static_cast<std::int32_t>(f); return true;
}
std::int32_t difference(std::int32_t a, std::int32_t b) { return signed_word(std::uint32_t(a) - std::uint32_t(b)); }
}

extern "C" {
Error dh2_mesh_open(Mesh* out, const BresView* image, std::int32_t geometry) {
    if (!out) return Error::argument;
    *out = {};
    if (!image) return Error::argument;
    const auto* g = dh2_bres_library_item(image, Library::geometry, geometry);
    if (!g) return Error::range;
    if (word(g + 8) != 0) return Error::geometry_type;
    const auto* m = at(*image, word(g + 12), 44);
    if (!m) return Error::range;
    if (word(m) != 1) return Error::stream_layout;
    const auto* s = at(*image, word(m + 8), 44);
    if (!s) return Error::range;
    const auto n = word(s + 4);
    const std::uint32_t stride = half(s);
    if (!stride || !n || n > 128 || n != word(s + 12) || n != word(s + 20) || n != word(s + 28)) return Error::stream_layout;
    for (auto offset : {8U, 16U, 24U, 32U})
        if (!at(*image, word(s + offset), std::uint64_t(n) * 4)) return Error::range;
    if (!buffer(*image, word(s + 36), std::uint64_t(word(m + 4)) * stride)
        || !at(*image, word(m + 16), std::uint64_t(word(m + 12)) * 56)) return Error::range;
    Mesh candidate{*image, text(*image, word(g)), text(*image, word(g + 4)), word(m + 4), stride, n, word(m + 12), word(m + 8), word(m + 16), {}, {}};
    if (!candidate.id || !candidate.name) return Error::string;
    for (std::uint32_t i = 0; i < 3; ++i) {
        scalar(m + 20 + i * 4, 6, candidate.minimum[i]);
        scalar(m + 32 + i * 4, 6, candidate.maximum[i]);
    }
    for (std::uint32_t i = 0; i < n; ++i) {
        Attribute a{}; const auto e = dh2_mesh_attribute(&candidate, i, &a);
        if (e != Error::ok) return e;
    }
    for (std::uint32_t i = 0; i < candidate.primitives; ++i) {
        Primitive p{}; const auto e = dh2_mesh_primitive(&candidate, i, &p);
        if (e != Error::ok) return e;
        for (std::uint32_t k = 0; k < p.index_count; ++k) {
            std::uint32_t index; dh2_index_read(&p, k, &index);
            if (index >= candidate.vertices || index < p.minimum_index || index > p.maximum_index) return Error::index;
        }
    }
    *out = candidate; return Error::ok;
}
Error dh2_mesh_attribute(const Mesh* m, std::int32_t i, Attribute* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!m || i < 0 || std::uint32_t(i) >= m->attributes) return Error::attribute;
    const auto* s = at(m->image, m->stream, 44);
    if (!s) return Error::range;
    std::uint32_t values[3];
    for (std::uint32_t j = 0; j < 3; ++j) {
        const auto offset = std::uint64_t(word(s + 8 + j * 8)) + std::uint32_t(i) * 4;
        if (!span(m->image, offset, 4)) return Error::range;
        values[j] = word(m->image.bytes + offset);
    }
    const auto offset = values[0], type = values[1], components = values[2];
    if (type > 6 || !components || components > 4 || offset >= m->stride
        || components * widths[type] > m->stride - offset) return Error::attribute;
    const auto* data = buffer(m->image, word(s + 36), std::uint64_t(m->vertices) * m->stride);
    if (!data) return Error::range;
    *out = {data + offset, type, components, m->stride, m->vertices}; return Error::ok;
}
Error dh2_mesh_primitive(const Mesh* m, std::int32_t i, Primitive* out) {
    if (!out) return Error::argument;
    *out = {};
    if (!m || i < 0 || std::uint32_t(i) >= m->primitives) return Error::primitive;
    const auto offset = std::uint64_t(m->buffers) + std::uint32_t(i) * 56;
    if (!span(m->image, offset, 56)) return Error::range;
    const auto* p = m->image.bytes + offset;
    const auto type = word(p), maximum = word(p + 36), count = word(p + 40), width = maximum < 65536 ? 2U : 4U;
    if (type >= 5) return Error::primitive;
    const auto* indices = buffer(m->image, word(p + 44), std::uint64_t(count) * width);
    const auto* material = text(m->image, word(p + 4));
    if (!indices) return Error::range;
    if (!material) return Error::string;
    *out = {material, indices, type, primitive_map[type], word(p + 8), count, width, word(p + 32), maximum, {}};
    for (std::uint32_t j = 0; j < 18; ++j) {
        const auto v = p[12 + j]; out->attributes[j] = v < 128 ? v : int(v) - 256;
        if (out->attributes[j] < -1 || out->attributes[j] >= std::int32_t(m->attributes)) return Error::attribute;
    }
    return Error::ok;
}
bool dh2_attribute_read(const Attribute* a, std::uint32_t vertex, float* out) {
    if (!a || !out || !a->data || vertex >= a->vertices || a->type > 6 || !a->components || a->components > 4) return false;
    if (a->components * widths[a->type] > a->stride) return false;
    for (std::uint32_t j = 0; j < a->components; ++j)
        scalar(a->data + std::size_t(vertex) * a->stride + j * widths[a->type], a->type, out[j]);
    return true;
}
bool dh2_index_read(const Primitive* p, std::uint32_t i, std::uint32_t* out) {
    if (!p || !out || !p->indices || i >= p->index_count || (p->index_width != 2 && p->index_width != 4)) return false;
    const auto* b = p->indices + std::size_t(i) * p->index_width;
    *out = p->index_width == 2 ? half(b) : word(b); return true;
}
std::uint32_t dh2_animation_segments(const BresView* image) {
    if (!image) return 0;
    const auto* r = at(*image, image->root_offset, 192);
    if (!r || !word(r + 48)) return 0;
    const auto* l = at(*image, word(r + 48), 8);
    if (!l || !at(*image, word(l + 4), std::uint64_t(word(l)) * 24)) return 0;
    return word(l);
}
Error dh2_animation_open(Animation* out, const BresView* image, std::int32_t animation, std::int32_t segment) {
    if (!out) return Error::argument;
    *out = {};
    if (!image || segment < 0 || std::uint32_t(segment) >= dh2_animation_segments(image)) return Error::range;
    const auto* a = dh2_bres_library_item(image, Library::animation, animation);
    if (!a) return Error::range;
    const auto* l = image->bytes + word(image->bytes + image->root_offset + 48);
    const auto* s = image->bytes + word(l + 4) + std::size_t(std::uint32_t(segment)) * 24;
    const auto state = word(s + 8);
    if (state > 1) return Error::segment_state;
    const auto data = word(s + (state == 0 ? 12 : 20));
    const auto size64 = state == 0 ? word(s + 16) & ~3U : (data <= image->size ? image->size - data : 0);
    if (size64 > std::numeric_limits<std::uint32_t>::max() || !span(*image, data, size64) || size64 < 4) return Error::range;
    const auto count = word(image->bytes + data);
    if (4 + std::uint64_t(count) * 8 > size64) return Error::vector;
    if (state == 1 && word(s + 16)) return Error::segment_state; // Already inner-relocated is unsupported.
    Animation candidate{*image, std::uint32_t(a - image->bytes), data, std::uint32_t(size64), count, signed_word(word(s)), signed_word(word(s + 4))};
    if (!text(*image, word(a))) return Error::string;
    if (!at(*image, word(a + 8), std::uint64_t(word(a + 4)) * 28)
        || !at(*image, word(a + 16), std::uint64_t(word(a + 12)) * 16)) return Error::range;
    for (std::uint32_t i = 0; i < word(a + 12); ++i) {
        const auto* channel = image->bytes + word(a + 16) + std::size_t(i) * 16;
        if (!text(*image, word(channel + 4))) return Error::string;
    }
    for (auto offset : {24U, 28U}) {
        if (word(a + offset)) {
            const auto* value = at(*image, word(a + offset), 12);
            if (!value || !at(*image, word(value + 8), 1)) return Error::range;
            if (offset == 28 && !at(*image, word(value + 4), 1)) return Error::range;
        }
    }
    for (std::uint32_t i = 0; i < word(a + 4); ++i) {
        Vector times{}, values{}; const auto* sm = sampler(&candidate, i);
        if (!vector(&candidate, sm, false, times) || !vector(&candidate, sm, true, values)) return Error::vector;
        if (times.components != 1) return Error::animation;
        // getKeyTime() and generic findKeyFrameNo() use sampler zero's time
        // type even when passed a different sampler. Validate that access too.
        Vector first{};
        if (!vector(&candidate, sampler(&candidate, 0), false, first)) return Error::vector;
        const auto bytes = std::uint64_t(times.count) * widths[first.type];
        const auto pos = std::uint64_t(times.data - image->bytes);
        if (pos < data || pos > std::uint64_t(data) + size64 || bytes > std::uint64_t(data) + size64 - pos) return Error::vector;
    }
    *out = candidate; return Error::ok;
}
const char* dh2_animation_target(const Animation* a) {
    const auto* c = dh2_animation_channel(a, 0); return c ? text(a->image, word(c + 4)) : nullptr;
}
const std::uint8_t* dh2_animation_channel(const Animation* a, std::int32_t i) {
    const auto* r = record(a);
    if (!r || i < 0 || std::uint32_t(i) >= word(r + 12)) return nullptr;
    const auto offset = std::uint64_t(word(r + 16)) + std::uint32_t(i) * 16;
    return span(a->image, offset, 16) ? a->image.bytes + offset : nullptr;
}
std::uint32_t dh2_animation_type(const Animation* a, std::int32_t i) { const auto* p = dh2_animation_channel(a, i); return p ? word(p + 8) : 0; }
std::uint32_t dh2_animation_channels(const Animation* a) { const auto* r = record(a); return r ? word(r + 12) : 0; }
std::uint32_t dh2_animation_samplers(const Animation* a) { const auto* r = record(a); return r ? word(r + 4) : 0; }
std::uint32_t dh2_animation_time_type(const Animation* a, std::int32_t i) { const auto* p = sampler(a, i); return p ? word(p + 4) : 0; }
std::uint32_t dh2_animation_interpolation(const Animation* a, std::int32_t i) { const auto* p = sampler(a, i); return p ? word(p) : 0; }
std::uint32_t dh2_animation_animator(const Animation* a) { const auto* r = record(a); return r ? word(r + 20) : 0; }
bool dh2_animation_has_default(const Animation* a) { const auto* r = record(a); return r && word(r + 24); }
const std::uint8_t* dh2_animation_default(const Animation* a) { return optional_value(a, 24, 8); }
const std::uint8_t* dh2_animation_offsets(const Animation* a) { return optional_value(a, 28, 8); }
const std::uint8_t* dh2_animation_scales(const Animation* a) { return optional_value(a, 28, 4); }
std::uint32_t dh2_animation_scale_type(const Animation* a) {
    const auto* r = record(a); if (!r || !word(r + 28)) return 2;
    const auto* p = at(a->image, word(r + 28), 4); return p ? word(p) : 2;
}
bool dh2_animation_vector(const Animation* a, std::int32_t i, bool output, Vector* out) {
    if (!out) return false;
    *out = {}; return vector(a, sampler(a, i), output, *out);
}
bool dh2_vector_read(const Vector* v, std::uint32_t i, float* out) {
    if (!v || !out || !v->data || i >= v->count || v->type > 6 || !v->components || v->components > 16) return false;
    for (std::uint32_t j = 0; j < v->components; ++j)
        scalar(v->data + (std::size_t(i) * v->components + j) * widths[v->type], v->type, out[j]);
    return true;
}
std::int32_t dh2_animation_key_time(const Animation* a, std::int32_t i, std::int32_t key) {
    Vector v{};
    if (key < 0 || !dh2_animation_vector(a, i, false, &v) || std::uint32_t(key) >= v.count) return 0;
    v.type = dh2_animation_time_type(a, 0); // Original sampler-zero behavior.
    if (v.type == 4) return raw_time(v, key);
    if (v.type == 1 || v.type == 3) return static_cast<std::int32_t>(double(raw_time(v, key)) * 0x1.0aaaa9f7b5aeap+5);
    return 0;
}
std::int32_t dh2_animation_start(const Animation* a, std::int32_t i) { return dh2_animation_key_time(a, i, 0); }
std::int32_t dh2_animation_end(const Animation* a, std::int32_t i) {
    Vector v{}; if (!dh2_animation_vector(a, i, false, &v) || !v.count || v.count > 2147483647U) return 0;
    return dh2_animation_key_time(a, i, std::int32_t(v.count) - 1);
}
std::int32_t dh2_animation_length(const Animation* a, std::int32_t i) { return difference(dh2_animation_end(a, i), dh2_animation_start(a, i)); }
bool dh2_animation_find_raw_index(const Vector* v, std::int32_t ms, std::int32_t* key) {
    if (!v || !key || !v->data || !v->count || v->count > 2147483647U || v->components != 1 || (v->type != 1 && v->type != 3 && v->type != 4)) return false;
    const auto query = static_cast<float>(ms);
    const auto frame = v->type == 4 ? query : query / 0x1.0aaaaap+5f;
    std::int32_t low = 1, high = std::int32_t(v->count) - 1;
    while (low <= high) {
        const auto middle = std::int32_t((std::int64_t(low) + high) / 2);
        if (frame < static_cast<float>(raw_time(*v, middle))) high = middle - 1;
        else low = middle + 1;
    }
    *key = high;
    const auto t = static_cast<float>(raw_time(*v, high));
    const auto start = v->type == 4 ? t : t * 0x1.0aaaaap+5f;
    // The imported helper is __aeabi_fcmpeq: exact keys return false,
    // while a query before the first key can return true with key zero.
    // The fraction overload then clamps that negative fraction to zero.
    return !(query == start) && high != std::int32_t(v->count) - 1;
}
bool dh2_animation_find_index(const Animation* a, std::int32_t i, std::int32_t ms, std::int32_t* key) {
    Vector v{}; if (!dh2_animation_vector(a, i, false, &v)) return false;
    v.type = dh2_animation_time_type(a, 0);
    const bool active = dh2_animation_find_raw_index(&v, ms, key);
    return dh2_animation_interpolation(a, i) != 0 && active;
}
bool dh2_animation_find(const Animation* a, std::int32_t i, std::int32_t ms, std::int32_t* key, float* fraction) {
    if (!fraction || !dh2_animation_find_index(a, i, ms, key)) return false;
    Vector v{}; if (!dh2_animation_vector(a, i, false, &v)) return false;
    v.type = dh2_animation_time_type(a, 0);
    const auto factor = v.type == 4 ? 1.0f : 0x1.0aaaaap+5f;
    std::int32_t start, end;
    if (!truncated(static_cast<float>(raw_time(v, *key)) * factor, start)
        || !truncated(static_cast<float>(raw_time(v, *key + 1)) * factor, end)) return false;
    float ratio = static_cast<float>(difference(ms, start)) / static_cast<float>(difference(end, start));
    if (ratio < 0.0f) ratio = 0.0f;
    else if (!(ratio < 1.0f)) ratio = 1.0f;
    *fraction = ratio; return true;
}
}
