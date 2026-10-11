#include "resources.hpp"
#include <cstring>
#include <new>

namespace dh2::resources {
namespace {
std::int32_t signed32(std::uint32_t v) {
    return v <= 0x7fffffffU ? static_cast<std::int32_t>(v)
                           : -1 - static_cast<std::int32_t>(~v);
}
std::int32_t add(std::int32_t a, std::uint32_t b) {
    return signed32(static_cast<std::uint32_t>(a) + b);
}
std::int32_t sub(std::int32_t a, std::int32_t b) {
    return signed32(static_cast<std::uint32_t>(a) - static_cast<std::uint32_t>(b));
}
std::uint32_t word(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8)
         | (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
}
bool span(std::size_t offset, std::size_t amount, std::size_t size) {
    return offset <= size && amount <= size - offset;
}
}

MemoryReader::MemoryReader(void* p, std::int32_t n, const char* s)
    : data(static_cast<std::uint8_t*>(p)), length(n), cursor(0), filename(s) {}
std::int32_t MemoryReader::read(void* dst, std::uint32_t requested) {
    std::int32_t amount = signed32(requested);
    if (add(cursor, requested) > length) amount = sub(length, cursor);
    if (amount <= 0) return 0;
    // The old seek admits negative positions. Retain that state behavior but
    // refuse copies outside the borrowed buffer instead of reproducing UB.
    if (!data || !dst || cursor < 0 || length < 0 || cursor > length
        || amount > length - cursor) return 0;
    std::memcpy(dst, data + cursor, static_cast<std::uint32_t>(amount));
    cursor = add(cursor, static_cast<std::uint32_t>(amount));
    return amount;
}
bool MemoryReader::seek(std::int32_t requested, bool relative) {
    const auto candidate = relative ? add(cursor, static_cast<std::uint32_t>(requested)) : requested;
    if (candidate > length) return false;
    cursor = candidate;
    return true;
}
bool MemoryReader::read_async(void* dst, std::uint32_t n, Completion fn, void* user) {
    const auto amount = read(dst, n);
    fn(amount, amount == 0 ? 1 : 0, this, user);
    return true;
}
bool MemoryReader::read_async_at(void* dst, std::uint32_t n, std::int32_t p,
                                Completion fn, void* user) {
    seek(p, false); // Original ignores failure and reads from the old cursor.
    return read_async(dst, n, fn, user);
}
std::int32_t MemoryReader::size() const { return length; }
std::int32_t MemoryReader::position() const { return cursor; }
const char* MemoryReader::name() const { return filename; }
const char* MemoryReader::full_path() const { return filename; }
void* MemoryReader::buffer(std::int32_t* p) { if (p) *p = cursor; return data; }
bool MemoryReader::valid() const { return data && length >= 0; }
bool MemoryReader::all_in_memory() const { return true; }

LimitReader::LimitReader(ReadFile* file, std::int32_t n, const char* s, const char* path)
    : underlying(file), length(n), start(file->position()),
      end(add(start, static_cast<std::uint32_t>(n))), cursor(start), filename(s), fullpath(path) {}
std::int32_t LimitReader::read(void* dst, std::uint32_t requested) {
    auto position = underlying->position();
    if (position != cursor) { underlying->seek(cursor, false); position = cursor; }
    if (position >= end) return 0;
    auto amount = requested;
    if (end <= add(position, requested)) amount = static_cast<std::uint32_t>(sub(end, position));
    const auto result = underlying->read(dst, amount);
    cursor = add(cursor, static_cast<std::uint32_t>(result));
    return result;
}
bool LimitReader::read_async(void* dst, std::uint32_t n, Completion fn, void* user) {
    return read_async_at(dst, n, position(), fn, user);
}
bool LimitReader::read_async_at(void* dst, std::uint32_t requested, std::int32_t p,
                               Completion fn, void* user) {
    cursor = add(start, static_cast<std::uint32_t>(p));
    if (cursor >= end) return false; // Cursor changes even when no callback fires.
    auto amount = requested;
    if (end <= add(cursor, requested)) amount = static_cast<std::uint32_t>(sub(end, cursor));
    const auto result = underlying->read_async_at(dst, amount, cursor, fn, user);
    // Uses the clamped requested count, even after short reads or a failure.
    // Callback receives the underlying file, not this LimitReader.
    cursor = add(cursor, amount);
    return result;
}
bool LimitReader::seek(std::int32_t requested, bool relative) {
    const auto position = underlying->position();
    auto argument = add(sub(requested, cursor), static_cast<std::uint32_t>(position));
    if (relative) {
        if (add(argument, static_cast<std::uint32_t>(cursor)) > end) argument = sub(end, position);
        cursor = add(argument, static_cast<std::uint32_t>(position));
    } else {
        argument = add(argument, static_cast<std::uint32_t>(start));
        if (argument > end) return false;
        cursor = argument;
    }
    return underlying->seek(argument, relative);
}
std::int32_t LimitReader::size() const { return length; }
std::int32_t LimitReader::position() const { return sub(cursor, start); }
const char* LimitReader::name() const { return filename; }
const char* LimitReader::full_path() const { return fullpath; }

const LibraryLayout libraries[] = {
    {0x24, 0x28, 0x20, "animation"}, {0x34, 0x38, 0x0c, "animation_clip"},
    {0x3c, 0x40, 0x1c, "camera"}, {0x44, 0x48, 0x18, "light"},
    {0x4c, 0x50, 0x14, "image"}, {0x54, 0x58, 0x74, "effect"},
    {0x5c, 0x60, 0x24, "material"}, {0x68, 0x6c, 0x10, "geometry"},
    {0x70, 0x74, 0x0c, "controller"}, {0x78, 0x7c, 0x90, "emitter"},
    {0x80, 0x84, 0xe8, "gnps_emitter"}, {0x88, 0x8c, 0x10, "force"},
    {0x90, 0x94, 0x24, "coronas"}
};

namespace {
bool library_span(const BresView* v, Library kind, std::uint32_t& count,
                  std::uint32_t& offset, std::uint32_t& stride) {
    const auto index = static_cast<std::uint32_t>(kind);
    if (!v || !v->bytes || index >= static_cast<unsigned>(Library::count)
        || !span(v->root_offset, 192, v->size)) return false;
    const auto& l = libraries[index];
    count = word(v->bytes + v->root_offset + l.count_offset);
    offset = word(v->bytes + v->root_offset + l.pointer_offset);
    stride = l.stride;
    return (!count || offset != 0) && span(offset, std::uint64_t(count) * stride, v->size);
}
}
}

using namespace dh2::resources;
extern "C" {
void dh2_memory_init(MemoryReader* r, void* p, std::int32_t n, const char* s) { new (r) MemoryReader(p, n, s); }
void dh2_limit_init(LimitReader* r, ReadFile* f, std::int32_t n, const char* s, const char* p) { new (r) LimitReader(f, n, s, p); }
std::int32_t dh2_memory_read(MemoryReader* r, void* p, std::uint32_t n) { return r->read(p, n); }
bool dh2_memory_seek(MemoryReader* r, std::int32_t p, bool b) { return r->seek(p, b); }
bool dh2_memory_async(MemoryReader* r, void* p, std::uint32_t n, Completion f, void* u) { return r->read_async(p, n, f, u); }
bool dh2_memory_async_at(MemoryReader* r, void* p, std::uint32_t n, std::int32_t at, Completion f, void* u) { return r->read_async_at(p, n, at, f, u); }
void* dh2_memory_buffer(MemoryReader* r, std::int32_t* p) { return r->buffer(p); }
bool dh2_memory_valid(const MemoryReader* r) { return r->valid(); }
bool dh2_memory_all_in_memory(const MemoryReader* r) { return r->all_in_memory(); }
std::int32_t dh2_memory_size(const MemoryReader* r) { return r->size(); }
std::int32_t dh2_memory_position(const MemoryReader* r) { return r->position(); }
const char* dh2_memory_name(const MemoryReader* r) { return r->name(); }
const char* dh2_memory_full_path(const MemoryReader* r) { return r->full_path(); }
std::int32_t dh2_limit_read(LimitReader* r, void* p, std::uint32_t n) { return r->read(p, n); }
bool dh2_limit_seek(LimitReader* r, std::int32_t p, bool b) { return r->seek(p, b); }
bool dh2_limit_async(LimitReader* r, void* p, std::uint32_t n, Completion f, void* u) { return r->read_async(p, n, f, u); }
bool dh2_limit_async_at(LimitReader* r, void* p, std::uint32_t n, std::int32_t at, Completion f, void* u) { return r->read_async_at(p, n, at, f, u); }
std::int32_t dh2_limit_size(const LimitReader* r) { return r->size(); }
std::int32_t dh2_limit_position(const LimitReader* r) { return r->position(); }
const char* dh2_limit_name(const LimitReader* r) { return r->name(); }
const char* dh2_limit_full_path(const LimitReader* r) { return r->full_path(); }

BresError dh2_bres_open(BresView* out, const void* input, std::size_t size) {
    if (!out) return BresError::null_input;
    *out = {};
    if (!input) return BresError::null_input;
    if (size < 60) return BresError::short_header;
    const auto* b = static_cast<const std::uint8_t*>(input);
    if (word(b) != 0x53455242U) return BresError::magic;
    if (b[4] != 0xfe || b[5] != 0xff) return BresError::byte_order;
    if (b[7] & 0x80) return BresError::already_relocated;
    if (word(b + 8) != 60) return BresError::header_size;
    if (word(b + 12) != size) return BresError::file_size;
    if (word(b + 20) != 0) return BresError::external_base;
    const auto count = word(b + 16), table = word(b + 24);
    if (!count || table != 60 || !span(table, std::uint64_t(count) * 4, size)
        || word(b + 28) != 60 + std::uint64_t(count) * 4 || word(b + table) != 24)
        return BresError::fixup_table;
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto field = word(b + table + std::size_t(i) * 4);
        if (field % 4 || !span(field, 4, size)
            || (field >= table && field < table + std::uint64_t(count) * 4))
            return BresError::fixup_field;
        if (word(b + field) > size) return BresError::fixup_target;
    }
    const auto root = word(b + 32);
    if (!span(root, 192, size)) return BresError::root;
    if (!span(word(b + 40), word(b + 44), size)
        || !span(word(b + 36), 0, size) || word(b + 56) > size) return BresError::file_size;
    *out = {b, size, count, table, root, word(b + 36), word(b + 44), word(b + 48), word(b + 56)};
    return BresError::ok;
}
BresError dh2_bres_fixups(const BresView* v, Fixup* out, std::size_t capacity) {
    if (!v || !v->bytes || !out) return BresError::null_input;
    if (capacity < v->fixup_count) return BresError::capacity;
    for (std::uint32_t i = 0; i < v->fixup_count; ++i) {
        const auto field = word(v->bytes + v->fixup_offset + std::size_t(i) * 4);
        out[i] = {v->bytes + field, v->bytes + word(v->bytes + field)};
    }
    return BresError::ok;
}
std::uint32_t dh2_bres_library_count(const BresView* v, Library kind) {
    std::uint32_t n, p, stride;
    return library_span(v, kind, n, p, stride) ? n : 0;
}
const std::uint8_t* dh2_bres_library_item(const BresView* v, Library kind, std::int32_t index) {
    std::uint32_t n, p, stride;
    if (index < 0 || !library_span(v, kind, n, p, stride) || static_cast<std::uint32_t>(index) >= n) return nullptr;
    return v->bytes + p + std::size_t(index) * stride;
}
const std::uint8_t* dh2_bres_root_part(const BresView* v, RootPart kind) {
    if (!v || !v->bytes || !span(v->root_offset, 192, v->size)) return nullptr;
    if (kind == RootPart::animation_clip_library) return v->bytes + v->root_offset + 0x34;
    if (kind == RootPart::scene) return v->bytes + v->root_offset + 0xb8;
    return nullptr;
}
const char* dh2_bres_version(const BresView* v) {
    if (!v || !v->bytes || !span(v->root_offset, 4, v->size)) return nullptr;
    const auto offset = word(v->bytes + v->root_offset);
    if (!span(offset, 1, v->size)) return nullptr;
    for (std::size_t i = offset; i < v->size; ++i)
        if (!v->bytes[i]) return reinterpret_cast<const char*>(v->bytes + offset);
    return nullptr;
}
EmitterError dh2_bres_emitter(const BresView* view, std::int32_t index,
                              Emitter* out) {
    if (!view || !view->bytes || !out) return EmitterError::null_input;
    *out = {};
    if (index < 0) return EmitterError::index;
    std::uint32_t count = 0, table = 0, stride = 0;
    if (!library_span(view, Library::emitter, count, table, stride)
        || stride != 144) return EmitterError::library;
    if (static_cast<std::uint32_t>(index) >= count) return EmitterError::index;
    const auto offset = table + static_cast<std::uint32_t>(index) * stride;
    if (!span(offset, stride, view->size)) return EmitterError::row;
    const auto* row = view->bytes + offset;
    const auto read_float = [view](std::uint32_t at) {
        const auto bits = word(view->bytes + at);
        float value = 0;
        std::memcpy(&value, &bits, sizeof(value));
        return value;
    };
    const auto pointer_target = [view,row](std::uint32_t field,
                                            std::uint32_t bytes,
                                            std::uint32_t& target) {
        target = word(row + field);
        return target != 0 && span(target, bytes, view->size);
    };
    out->row_index = static_cast<std::uint32_t>(index);
    out->row_offset = offset;
    out->emitter_type = word(row + 8);
    out->max_particles = signed32(word(row + 24));
    out->birth_rate = read_float(offset + 32);
    out->life = read_float(offset + 40);
    out->life_variation = read_float(offset + 44);
    out->target_size = read_float(offset + 48);
    out->size_variation = read_float(offset + 52);
    out->size_growth_time = read_float(offset + 56);
    out->size_fade_time = read_float(offset + 60);
    out->speed = read_float(offset + 64);
    out->speed_variation = read_float(offset + 68);
    out->direction_type = word(row + 72);
    out->geometry_mode = word(row + 80);
    out->shape_data_offset = word(row + 84);
    out->animation_offset = read_float(offset + 92);
    out->animation_offset_variation = read_float(offset + 96);
    out->animation_key_mapping_type = word(row + 100);
    out->animation_length = read_float(offset + 104);
    out->animation_length_variation = read_float(offset + 108);
    out->animation_scale_multiplier = read_float(offset + 112);
    out->animation_scale_variation = read_float(offset + 116);
    out->spin_time = read_float(offset + 120);
    out->spin_variation = read_float(offset + 124);
    out->spin_phase = read_float(offset + 128);
    out->spin_phase_variation = read_float(offset + 132);
    out->spin_axis_type = word(row + 136);
    out->spin_axis_data_offset = word(row + 140);

    // initParticleSystem reads RadiusLength and dimensions through +0x0c:
    // type 0 uses radius/width/height; types 1 and 2 use radius/height.
    // Preserve the serialized target as a file offset.
    if (out->emitter_type <= 2) {
        const std::uint32_t shape_bytes = out->emitter_type == 0 ? 12U : 8U;
        if (!pointer_target(12, shape_bytes, out->radius_data_offset))
            return EmitterError::radius_pointer;
        out->radius_length = read_float(out->radius_data_offset);
        if (out->emitter_type == 0) {
            out->shape_width = read_float(out->radius_data_offset + 4);
            out->shape_height = read_float(out->radius_data_offset + 8);
            out->shape_width_present = out->shape_height_present = true;
        } else {
            out->shape_height = read_float(out->radius_data_offset + 4);
            out->shape_height_present = true;
        }
    }
    if (out->direction_type == 1 || out->direction_type == 2) {
        const auto need = out->direction_type == 1 ? 16U : 8U;
        if (!pointer_target(76, need, out->direction_data_offset))
            return EmitterError::direction_pointer;
        if (out->direction_type == 1) {
            for (std::uint32_t i = 0; i < 3; ++i)
                out->direction[i] = read_float(out->direction_data_offset + i * 4);
            out->direction_variation = read_float(out->direction_data_offset + 12);
            out->direction_variation_present = true;
        } else {
            // Source direction type 2 reads the scalar at vector+4; direction
            // itself remains at the particle-context default.
            out->direction_variation = read_float(out->direction_data_offset + 4);
            out->direction_variation_present = true;
        }
    }
    if (out->spin_axis_type == 1) {
        if (!pointer_target(140, 16, out->spin_axis_data_offset))
            return EmitterError::spin_axis_pointer;
        for (std::uint32_t i = 0; i < 3; ++i)
            out->spin_axis[i] = read_float(out->spin_axis_data_offset + i * 4);
        out->spin_axis_variation = read_float(out->spin_axis_data_offset + 12);
    }

    // createParticleSystem only reads the shape pointer for mode 0. Tags 3/4
    // use flags at +4 and subtype at +8; the lock-axis branches consume vec3
    // data at +0x0c and/or +0x18. Other tags are selectors, not dimensions.
    if (out->geometry_mode == 0) {
        if (!out->shape_data_offset
            || !span(out->shape_data_offset, 4, view->size))
            return EmitterError::shape_pointer;
        const auto* shape = view->bytes + out->shape_data_offset;
        out->shape_source_tag = word(shape);
        if (out->shape_source_tag == 3 || out->shape_source_tag == 4) {
            if (!span(out->shape_data_offset, 12, view->size))
                return EmitterError::shape_pointer;
            out->shape_flags = word(shape + 4);
            out->shape_subtype = word(shape + 8);
            const bool use_axis_0 = out->shape_subtype == 1
                && (out->shape_flags & 2U) != 0;
            const bool use_axis_1 = (out->shape_subtype == 2)
                || (out->shape_subtype == 1 && (out->shape_flags & 4U) != 0);
            if (use_axis_0) {
                if (!span(out->shape_data_offset + 12, 12, view->size))
                    return EmitterError::shape_pointer;
                for (std::uint32_t i = 0; i < 3; ++i)
                    out->shape_lock_axis_0[i] = read_float(out->shape_data_offset + 12 + i * 4);
            }
            if (use_axis_1) {
                if (!span(out->shape_data_offset + 24, 12, view->size))
                    return EmitterError::shape_pointer;
                for (std::uint32_t i = 0; i < 3; ++i)
                    out->shape_lock_axis_1[i] = read_float(out->shape_data_offset + 24 + i * 4);
            }
        }
    }
    return EmitterError::ok;
}
EmitterError dh2_bres_visual_content(const BresView* view, VisualContent* out) {
    if (!view || !view->bytes || !out) return EmitterError::null_input;
    *out = {};
    const auto read_count = [view](Library kind, std::uint32_t& count) {
        std::uint32_t offset = 0, stride = 0;
        return library_span(view, kind, count, offset, stride);
    };
    if (!read_count(Library::geometry, out->geometry_count)
        || !read_count(Library::emitter, out->emitter_count)
        || !read_count(Library::gnps_emitter, out->gnps_emitter_count)
        || !read_count(Library::material, out->material_count)
        || !read_count(Library::effect, out->effect_count)
        || !read_count(Library::force, out->force_count)
        || !read_count(Library::coronas, out->coronas_count))
        return EmitterError::library;
    const bool geometry = out->geometry_count != 0;
    const bool particles = out->emitter_count != 0 || out->gnps_emitter_count != 0;
    if (geometry && particles) out->kind = VisualContentKind::geometry_and_particles;
    else if (geometry) out->kind = VisualContentKind::geometry;
    else if (particles) out->kind = VisualContentKind::particles_without_geometry;
    else if (out->force_count || out->coronas_count)
        out->kind = VisualContentKind::auxiliary_only;
    else out->kind = VisualContentKind::no_particles_or_geometry;
    return EmitterError::ok;
}
}
