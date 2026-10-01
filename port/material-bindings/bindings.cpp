#include "bindings.hpp"

#include <cstring>

namespace {
using dh2::resources::BresView;
using dh2::resources::Library;
using dh2::materials::Error;

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
    out->image = *view;
    out->record = record;
    return Error::ok;
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
