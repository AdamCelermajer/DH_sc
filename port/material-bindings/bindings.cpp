#include "bindings.hpp"

#include <cstring>
#include <cstdint>

namespace {
using dh2::resources::BresView;
using dh2::resources::Library;
using dh2::materials::Error;

bool span(const BresView* view, std::uint32_t offset, std::uint64_t length) {
    return view && view->bytes && offset <= view->size && length <= view->size - offset;
}

bool borrowed(const BresView* view, const std::uint8_t* pointer, std::uint64_t length) {
    if (!view || !view->bytes || !pointer) return false;
    const auto base = reinterpret_cast<std::uintptr_t>(view->bytes);
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);
    return address >= base && address - base <= view->size &&
           length <= view->size - (address - base);
}

bool array_span(const BresView* view, std::uint32_t count,
                std::uint32_t offset, std::uint32_t stride) {
    return (!count || offset != 0) &&
           span(view, offset, std::uint64_t(count) * stride);
}

std::uint32_t word(const std::uint8_t* p) {
    return std::uint32_t(p[0]) | std::uint32_t(p[1]) << 8 |
           std::uint32_t(p[2]) << 16 | std::uint32_t(p[3]) << 24;
}

const char* string(const BresView* view, std::uint32_t offset) {
    if (offset == 0 || offset >= view->size) return nullptr;
    for (std::size_t i = offset; i < view->size; ++i) {
        if (view->bytes[i] == 0)
            return reinterpret_cast<const char*>(view->bytes + offset);
    }
    return nullptr;
}

template <typename T>
bool prepare(T* out, const BresView* view) {
    if (!out) return false;
    *out = {};
    return view && view->bytes;
}

Error parameter(dh2::materials::Parameter* out, const BresView* view,
                const std::uint8_t* records, std::uint32_t count,
                std::int32_t index) {
    if (!out) return Error::argument;
    *out = {};
    if (!view || !view->bytes) return Error::argument;
    if (index < 0 || static_cast<std::uint32_t>(index) >= count) return Error::index;
    if (!borrowed(view, records, std::uint64_t(count) * 24)) return Error::layout;
    const auto* record = records + std::size_t(index) * 24;
    const auto id = string(view, word(record));
    const auto semantic = string(view, word(record + 4));
    if (!id || !semantic) return Error::string;
    const auto header = word(record + 16), values = word(record + 20);
    if (!span(view, header, 4) || !span(view, values, 4) ||
        word(view->bytes + header) != word(record + 12))
        return Error::layout;
    *out = {*view, id, semantic, word(record + 8), word(record + 12),
            view->bytes + values};
    return Error::ok;
}

Error image_ref(dh2::materials::ImageRef* out, const BresView* view,
                std::uint32_t serialized_index) {
    if (!out) return Error::argument;
    *out = {};
    out->index = -1;
    if (!view || !view->bytes) return Error::argument;
    if (serialized_index == 0xffffffffU) return Error::ok;
    if (serialized_index >= dh2_bres_library_count(view, Library::image))
        return Error::layout;
    dh2::materials::Image image{};
    const auto error = dh2_image_record(&image, view,
                                         static_cast<std::int32_t>(serialized_index));
    if (error != Error::ok) return error;
    *out = {static_cast<std::int32_t>(serialized_index), image.id,
            image.name, image.source_path};
    return Error::ok;
}
}

extern "C" {
Error dh2_image_record(dh2::materials::Image* out, const BresView* view, std::int32_t index) {
    if (!prepare(out, view)) return Error::argument;
    const auto* record = dh2_bres_library_item(view, Library::image, index);
    if (!record) return Error::index;
    out->id = string(view, word(record));
    out->name = string(view, word(record + 4));
    out->source_path = string(view, word(record + 8));
    if (!out->id || !out->name || !out->source_path) return Error::string;
    out->image = *view;
    out->raw_word_12 = word(record + 12);
    out->raw_word_16 = word(record + 16);
    return Error::ok;
}

Error dh2_effect_record(dh2::materials::Effect* out, const BresView* view, std::int32_t index) {
    if (!prepare(out, view)) return Error::argument;
    const auto* record = dh2_bres_library_item(view, Library::effect, index);
    if (!record) return Error::index;
    out->id = string(view, word(record));
    out->name = string(view, word(record + 4));
    if (!out->id || !out->name) return Error::string;
    out->image = *view;
    out->record = record;
    return Error::ok;
}

Error dh2_effect_group(dh2::materials::EffectGroup* out,
                       const dh2::materials::Effect* effect, std::int32_t group) {
    if (!out) return Error::argument;
    *out = {};
    if (!effect || !effect->image.bytes) return Error::argument;
    if (group < 0 || group > 1) return Error::index;
    const auto* view = &effect->image;
    if (!borrowed(view, effect->record, 116)) return Error::layout;
    const auto* base = effect->record + 8 + std::size_t(group) * 24;
    const auto named_count = word(base), named_offset = word(base + 4);
    const auto parameter_count = word(base + 8), parameter_offset = word(base + 12);
    const auto image_count = word(base + 16), image_offset = word(base + 20);
    if (!array_span(view, named_count, named_offset, 12) ||
        !array_span(view, parameter_count, parameter_offset, 24) ||
        !array_span(view, image_count, image_offset, 4)) return Error::layout;
    *out = {*view, named_count, parameter_count, image_count,
            named_count ? view->bytes + named_offset : nullptr,
            parameter_count ? view->bytes + parameter_offset : nullptr,
            image_count ? view->bytes + image_offset : nullptr};
    return Error::ok;
}

Error dh2_effect_parameter(dh2::materials::EffectParameter* out,
                           const dh2::materials::EffectGroup* group, std::int32_t index) {
    if (!out) return Error::argument;
    *out = {};
    if (!group || !group->image.bytes) return Error::argument;
    if (index < 0 || static_cast<std::uint32_t>(index) >= group->parameter_count)
        return Error::index;
    const auto* view = &group->image;
    if (!borrowed(view, group->parameter_records,
                  std::uint64_t(group->parameter_count) * 24)) return Error::layout;
    const auto* record = group->parameter_records + std::size_t(index) * 24;
    const auto id = string(view, word(record));
    if (!id) return Error::string;
    const auto header = word(record + 16), values = word(record + 20);
    if (!span(view, header, 4) || !span(view, values, 4)) return Error::layout;
    *out = {*view, id, word(record + 4), word(record + 8),
            word(record + 12), view->bytes + values};
    return Error::ok;
}

Error dh2_effect_group_image(dh2::materials::ImageRef* out,
                             const dh2::materials::EffectGroup* group, std::int32_t index) {
    if (!out) return Error::argument;
    *out = {};
    if (!group || !group->image.bytes) return Error::argument;
    if (index < 0 || static_cast<std::uint32_t>(index) >= group->image_count)
        return Error::index;
    if (!borrowed(&group->image, group->image_indices,
                  std::uint64_t(group->image_count) * 4)) return Error::layout;
    return image_ref(out, &group->image,
                     word(group->image_indices + std::size_t(index) * 4));
}

Error dh2_material_record(dh2::materials::Material* out, const BresView* view, std::int32_t index) {
    if (!prepare(out, view)) return Error::argument;
    const auto* record = dh2_bres_library_item(view, Library::material, index);
    if (!record) return Error::index;
    out->id = string(view, word(record));
    out->name = string(view, word(record + 4));
    out->external_effect_file = string(view, word(record + 8));
    out->effect_url = string(view, word(record + 12));
    if (!out->id || !out->name || !out->effect_url ||
        (word(record + 8) && !out->external_effect_file)) return Error::string;
    const auto count = word(record + 16), offset = word(record + 20);
    if (!array_span(view, count, offset, 24)) return Error::layout;
    out->image = *view;
    out->record = record;
    out->parameter_count = count;
    out->parameter_records = count ? view->bytes + offset : nullptr;
    return Error::ok;
}

Error dh2_material_parameter(dh2::materials::Parameter* out,
                             const dh2::materials::Material* material, std::int32_t index) {
    if (!out) return Error::argument;
    *out = {};
    if (!material) return Error::argument;
    return parameter(out, &material->image, material->parameter_records,
                     material->parameter_count, index);
}

Error dh2_material_sampler_image(dh2::materials::ImageRef* out,
                                 const dh2::materials::Material* material,
                                 std::int32_t parameter_index) {
    if (!out) return Error::argument;
    *out = {};
    if (!material) return Error::argument;
    dh2::materials::Parameter value{};
    const auto error = dh2_material_parameter(&value, material, parameter_index);
    if (error != Error::ok) return error;
    if (value.type_code != 11) return Error::kind;
    if (value.value_count != 1 || !borrowed(&value.image, value.raw_value, 4))
        return Error::layout;
    const auto index_offset = word(value.raw_value);
    if (!span(&value.image, index_offset, 4)) return Error::layout;
    return image_ref(out, &value.image, word(value.image.bytes + index_offset));
}

std::int32_t dh2_material_local_effect(const dh2::materials::Material* material) {
    if (!material || !material->image.bytes || material->external_effect_file ||
        !material->effect_url || material->effect_url[0] != '#') return -1;
    const char* wanted = material->effect_url + 1;
    const auto count = dh2_bres_library_count(&material->image, Library::effect);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto* record = dh2_bres_library_item(&material->image, Library::effect, i);
        const char* id = string(&material->image, word(record));
        if (!id) return -1;
        if (std::strcmp(id, wanted) == 0) return static_cast<std::int32_t>(i);
    }
    return -1;
}
}
