#include "world.hpp"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
using namespace dh2::world;
constexpr std::size_t max_xml = 8U * 1024U * 1024U;
constexpr std::size_t max_value = 4096, max_path = 1024;
constexpr std::uint32_t max_objects = 4096, max_fields = 128, max_modules = 256;
constexpr std::uint32_t max_entities = 32768;

Error fail(Diagnostic* d, Error error, const char* message,
           const std::uint8_t* text = nullptr, std::size_t offset = 0) {
    if (d) {
        *d = {}; d->error = error; d->byte_offset = static_cast<std::uint32_t>(offset);
        d->line = 1; d->column = 1;
        for (std::size_t i = 0; text && i < offset; ++i) {
            if (text[i] == '\n') { ++d->line; d->column = 1; }
            else ++d->column;
        }
        std::snprintf(d->message, sizeof d->message, "%s", message);
    }
    return error;
}
Error done(Diagnostic* d) { if (d) *d = {}; return Error::ok; }
char* copy(const char* p, std::size_t n) {
    auto* out = static_cast<char*>(std::malloc(n + 1));
    if (out) { std::memcpy(out, p, n); out[n] = 0; }
    return out;
}
char* copy(const char* p) { return p ? copy(p, std::strlen(p)) : nullptr; }
void free_object(Object& o) {
    std::free(o.source_path);
    for (std::uint32_t i = 0; i < o.field_count; ++i) {
        std::free(o.fields[i].name); std::free(o.fields[i].value);
    }
    std::free(o.fields); o = {};
}
void free_objects(Object* p, std::uint32_t count) {
    for (std::uint32_t i = 0; i < count; ++i) free_object(p[i]);
    std::free(p);
}
bool whitespace(unsigned c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
bool name_first(unsigned c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool name_next(unsigned c) { return name_first(c) || (c >= '0' && c <= '9') || c == '-' || c == '.'; }
bool translation_only(const Transform& t) {
    for (unsigned i = 0; i < 3; ++i)
        if (t.rotation_degrees[i] != 0 || t.scale[i] != 1) return false;
    return true;
}

struct Parser {
    const std::uint8_t* p;
    std::size_t size, pos = 0;
    Diagnostic* diagnostic;
    Error error = Error::ok;
    bool reject(Error e, const char* message) {
        error = fail(diagnostic, e, message, p, pos); return false;
    }
    bool starts(const char* s) const {
        const auto n = std::strlen(s);
        return n <= size - pos && std::memcmp(p + pos, s, n) == 0;
    }
    void space() { while (pos < size && whitespace(p[pos])) ++pos; }
    bool misc() {
        while (true) {
            space();
            if (!starts("<!--")) return true;
            pos += 4;
            while (pos < size && !starts("-->")) {
                if (starts("--")) return reject(Error::xml, "Invalid XML comment");
                ++pos;
            }
            if (pos == size) return reject(Error::xml, "Unterminated XML comment");
            pos += 3;
        }
    }
    bool consume(const char* s) {
        if (!starts(s)) return reject(Error::xml, "Unexpected XML token");
        pos += std::strlen(s); return true;
    }
    bool identifier(char* output, std::size_t capacity) {
        const auto begin = pos;
        if (pos >= size || !name_first(p[pos])) return reject(Error::xml, "Expected XML name");
        while (pos < size && name_next(p[pos])) ++pos;
        const auto n = pos - begin;
        if (n >= capacity) return reject(Error::limit, "XML name too long");
        std::memcpy(output, p + begin, n); output[n] = 0; return true;
    }
    bool codepoint(char* value, std::size_t& n, std::uint32_t cp) {
        if (!((cp >= 0x20 && cp <= 0xd7ff) || (cp >= 0xe000 && cp <= 0xfffd) ||
              (cp >= 0x10000 && cp <= 0x10ffff) || cp == 9 || cp == 10 || cp == 13))
            return reject(Error::xml, "Invalid XML character");
        unsigned bytes = cp < 0x80 ? 1 : cp < 0x800 ? 2 : cp < 0x10000 ? 3 : 4;
        if (n + bytes > max_value) return reject(Error::limit, "Attribute value too long");
        if (bytes == 1) value[n++] = static_cast<char>(cp);
        else {
            value[n++] = static_cast<char>((bytes == 2 ? 0xc0 : bytes == 3 ? 0xe0 : 0xf0) |
                                           (cp >> (6 * (bytes - 1))));
            for (unsigned i = bytes - 1; i > 0; --i)
                value[n++] = static_cast<char>(0x80 | ((cp >> (6 * (i - 1))) & 0x3f));
        }
        return true;
    }
    bool entity(char* value, std::size_t& n) {
        ++pos;
        const auto begin = pos;
        while (pos < size && p[pos] != ';' && pos - begin <= 16) ++pos;
        if (pos >= size || p[pos] != ';') return reject(Error::xml, "Invalid XML entity");
        const auto length = pos++ - begin;
        const char* s = reinterpret_cast<const char*>(p + begin);
        std::uint32_t cp = 0;
        if (length == 3 && !std::memcmp(s, "amp", 3)) cp = '&';
        else if (length == 2 && !std::memcmp(s, "lt", 2)) cp = '<';
        else if (length == 2 && !std::memcmp(s, "gt", 2)) cp = '>';
        else if (length == 4 && !std::memcmp(s, "quot", 4)) cp = '"';
        else if (length == 4 && !std::memcmp(s, "apos", 4)) cp = '\'';
        else if (length > 1 && s[0] == '#') {
            std::size_t i = 1; unsigned base = 10;
            if (i < length && s[i] == 'x') { base = 16; ++i; }
            if (i == length) return reject(Error::xml, "Empty XML character reference");
            for (; i < length; ++i) {
                unsigned digit = s[i] >= '0' && s[i] <= '9' ? s[i] - '0' :
                    s[i] >= 'a' && s[i] <= 'f' ? s[i] - 'a' + 10 :
                    s[i] >= 'A' && s[i] <= 'F' ? s[i] - 'A' + 10 : 99;
                if (digit >= base || cp > (0x10ffffU - digit) / base)
                    return reject(Error::xml, "Invalid XML character reference");
                cp = cp * base + digit;
            }
        } else return reject(Error::unsupported_xml, "Custom XML entities are unsupported");
        return codepoint(value, n, cp);
    }
    bool quoted(char* value, std::size_t& n) {
        n = 0;
        if (pos >= size || (p[pos] != '"' && p[pos] != '\''))
            return reject(Error::xml, "Expected quoted attribute");
        const unsigned quote = p[pos++];
        while (pos < size && p[pos] != quote) {
            if (p[pos] == '<') return reject(Error::xml, "Unescaped attribute delimiter");
            if (p[pos] == '&') { if (!entity(value, n)) return false; continue; }
            std::uint32_t cp = p[pos++];
            if (cp >= 0x80) {
                const unsigned length = cp >= 0xc2 && cp <= 0xdf ? 2 :
                    cp >= 0xe0 && cp <= 0xef ? 3 : cp >= 0xf0 && cp <= 0xf4 ? 4 : 0;
                if (!length || length - 1 > size - pos) return reject(Error::xml, "Invalid UTF-8");
                cp &= length == 2 ? 0x1f : length == 3 ? 0xf : 7;
                for (unsigned i = 1; i < length; ++i) {
                    const auto c = p[pos++];
                    if ((c & 0xc0) != 0x80) return reject(Error::xml, "Invalid UTF-8");
                    cp = (cp << 6) | (c & 0x3f);
                }
                if (cp < (length == 2 ? 0x80U : length == 3 ? 0x800U : 0x10000U))
                    return reject(Error::xml, "Overlong UTF-8");
            }
            // XML attribute whitespace normalization. The original byte span
            // remains available; numeric character references are not changed.
            if (cp == 9 || cp == 10 || cp == 13) cp = ' ';
            if (!codepoint(value, n, cp)) return false;
        }
        if (pos >= size) return reject(Error::xml, "Unterminated XML attribute");
        ++pos; value[n] = 0; return true;
    }
    bool object(Object& o) {
        o = {}; o.module_index = no_module;
        o.source_begin = static_cast<std::uint32_t>(pos);
        if (!consume("<GameObject")) return false;
        while (true) {
            const auto before_space = pos; space();
            if (starts("/>")) { pos += 2; break; }
            if (before_space == pos) return reject(Error::xml, "Missing attribute separator");
            if (starts(">")) return reject(Error::unsupported_xml, "GameObject children are unsupported");
            char key[96], value[max_value + 1]; std::size_t n = 0;
            if (!identifier(key, sizeof key)) return false;
            if (dh2_world_field(&o, key)) return reject(Error::duplicate_field, "Duplicate XML attribute");
            space(); if (!consume("=")) return false; space();
            if (!quoted(value, n)) return false;
            if (o.field_count == max_fields) return reject(Error::limit, "Too many object attributes");
            auto* grown = static_cast<Field*>(std::realloc(o.fields, (o.field_count + 1) * sizeof(Field)));
            if (!grown) return reject(Error::allocation, "Attribute allocation failed");
            o.fields = grown; auto& field = o.fields[o.field_count++];
            field = {copy(key), copy(value, n)};
            if (!field.name || !field.value) return reject(Error::allocation, "Attribute text allocation failed");
        }
        o.source_end = static_cast<std::uint32_t>(pos); return true;
    }
    bool document(const char* root, Object*& objects, std::uint32_t& count) {
        if (starts("\xef\xbb\xbf")) pos += 3;
        if (starts("<?xml")) {
            pos += 5;
            while (pos < size && !starts("?>")) ++pos;
            if (pos == size) return reject(Error::xml, "Unterminated XML declaration");
            pos += 2;
        }
        if (!misc() || !consume("<")) return false;
        if (starts("!")) return reject(Error::unsupported_xml, "DTD and declarations are unsupported");
        char tag[32];
        if (!identifier(tag, sizeof tag)) return false;
        if (std::strcmp(tag, root)) return reject(Error::record, "Wrong document root");
        space(); if (!consume(">")) return false;
        while (true) {
            if (!misc()) return false;
            if (starts("</")) break;
            if (!starts("<GameObject")) return reject(Error::unsupported_xml, "Only exported GameObject records are supported");
            if (count == max_objects) return reject(Error::limit, "Too many GameObjects");
            auto* grown = static_cast<Object*>(std::realloc(objects, (count + 1) * sizeof(Object)));
            if (!grown) return reject(Error::allocation, "Object allocation failed");
            objects = grown; auto& record = objects[count++];
            if (!object(record)) return false;
            record.source_record = count - 1;
        }
        pos += 2;
        if (!identifier(tag, sizeof tag)) return false;
        if (std::strcmp(tag, root)) return reject(Error::xml, "Mismatched document close");
        space(); if (!consume(">") || !misc()) return false;
        if (pos != size) return reject(Error::xml, "Trailing XML content");
        return true;
    }
};

bool decimal(const char*& p, float& value) {
    while (whitespace(static_cast<unsigned char>(*p))) ++p;
    const auto* begin = p;
    if (*p == '-' || *p == '+') ++p;
    unsigned digits = 0;
    while (*p >= '0' && *p <= '9') { ++digits; ++p; }
    if (*p == '.') { ++p; while (*p >= '0' && *p <= '9') { ++digits; ++p; } }
    if (!digits) return false;
    if (*p == 'e' || *p == 'E') {
        ++p; if (*p == '+' || *p == '-') ++p;
        unsigned exponent = 0;
        while (*p >= '0' && *p <= '9') { ++exponent; ++p; }
        if (!exponent) return false;
    }
    errno = 0; char* end = nullptr; value = std::strtof(begin, &end);
    if (end != p || errno == ERANGE || !std::isfinite(value)) return false;
    while (whitespace(static_cast<unsigned char>(*p))) ++p;
    return true;
}
bool vector(const char* p, float* values) {
    if (!p) return false;
    for (unsigned i = 0; i < 3; ++i) {
        if (!decimal(p, values[i])) return false;
        if (i != 2) { if (*p != ',') return false; ++p; }
    }
    return *p == 0;
}
Error annotate(Object& o, const char* path, std::uint32_t module, RecordKind kind,
               const float* origin, Diagnostic* d, const std::uint8_t* bytes) {
    o.name = dh2_world_field(&o, "name"); o.gametype = dh2_world_field(&o, "gametype");
    if (!o.name || !*o.name || !o.gametype || !*o.gametype)
        return fail(d, Error::missing_field, "Object needs name and gametype", bytes, o.source_begin);
    if (!vector(dh2_world_field(&o, "position"), o.local.position) ||
        !vector(dh2_world_field(&o, "rotation"), o.local.rotation_degrees) ||
        !vector(dh2_world_field(&o, "scale"), o.local.scale))
        return fail(d, Error::number, "Object transform must contain three finite decimal values", bytes, o.source_begin);
    for (unsigned i = 0; i < 3; ++i) {
        o.world_position[i] = o.local.position[i] + (origin ? origin[i] : 0);
        if (!std::isfinite(o.world_position[i]))
            return fail(d, Error::number, "World position overflow", bytes, o.source_begin);
    }
    o.source_path = copy(path); o.module_index = module; o.kind = kind;
    return o.source_path ? Error::ok : fail(d, Error::allocation, "Provenance allocation failed");
}
Error normalized_owned(char*& output, const char* original, Diagnostic* d) {
    char normalized[max_path + 1];
    auto result = dh2_world_cache_path(normalized, sizeof normalized, original, d);
    if (result != Error::ok) return result;
    output = copy(normalized);
    return output ? Error::ok : fail(d, Error::allocation, "Path allocation failed");
}
}

extern "C" const char* dh2_world_field(const Object* object, const char* name) {
    if (!object || !name) return nullptr;
    for (std::uint32_t i = 0; i < object->field_count; ++i)
        if (object->fields[i].name && !std::strcmp(object->fields[i].name, name))
            return object->fields[i].value;
    return nullptr;
}
extern "C" Error dh2_world_cache_path(char* output, std::size_t capacity,
                                        const char* original, Diagnostic* d) {
    if (!output || !capacity || !original) return fail(d, Error::argument, "Missing path input/output");
    const auto n = std::strlen(original);
    if (!n || n > max_path) {
        output[0] = 0;
        return fail(d, Error::path, "Empty or overlong cache path");
    }
    // Preserve aliased input before clearing the caller's output on failures.
    char original_copy[max_path + 1];
    std::memcpy(original_copy, original, n + 1); original = original_copy;
    output[0] = 0;
    char buffer[max_path + 1];
    for (std::size_t i = 0; i < n; ++i) {
        unsigned c = static_cast<unsigned char>(original[i]);
        if (c < 0x20 || c >= 0x7f || c == ':' || c == '?' || c == '#')
            return fail(d, Error::path, "Invalid cache path character");
        if (c == '\\') c = '/';
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        buffer[i] = static_cast<char>(c);
    }
    buffer[n] = 0;
    if (std::strncmp(buffer, "data/", 5)) return fail(d, Error::path, "Cache path must start with data/");
    const char* begin = buffer;
    for (const char* p = buffer;; ++p) {
        if (*p != '/' && *p != 0) continue;
        const auto length = p - begin;
        if (!length || (length == 1 && *begin == '.') ||
            (length == 2 && begin[0] == '.' && begin[1] == '.'))
            return fail(d, Error::path, "Empty or traversal path segment");
        begin = p + 1; if (!*p) break;
    }
    if (!std::strncmp(buffer, "data/iphone/", 12))
        std::memmove(buffer + 5, buffer + 12, n - 12 + 1);
    const auto length = std::strlen(buffer);
    if (length >= capacity) return fail(d, Error::limit, "Cache path output too small");
    std::memcpy(output, buffer, length + 1); return done(d);
}
extern "C" void dh2_world_free(Level* level) {
    if (!level) return;
    std::free(level->name); std::free(level->source_path); free_object(level->config);
    for (std::uint32_t i = 0; i < level->module_count; ++i) {
        auto& m = level->modules[i]; free_object(m.record);
        std::free(m.cache_dae); std::free(m.cache_mgp); std::free(m.cache_mvp);
        std::free(m.catalogue_node_id);
    }
    std::free(level->modules); free_objects(level->entities, level->entity_count); *level = {};
}
extern "C" Error dh2_world_import_level(Level* output, const char* name, const char* path,
    const std::uint8_t* bytes, std::size_t size, Diagnostic* d) {
    if (!output || !name || !*name || std::strlen(name) > 128 || !bytes || !size)
        return fail(d, Error::argument, "Missing level input");
    if (size > max_xml) return fail(d, Error::limit, "Level XML too large");
    Level candidate{}; Object* records = nullptr; std::uint32_t count = 0;
    Error result = normalized_owned(candidate.source_path, path, d);
    if (result != Error::ok) return result;
    candidate.name = copy(name);
    if (!candidate.name) result = fail(d, Error::allocation, "Level name allocation failed");
    Parser parser{bytes, size, 0, d};
    if (result == Error::ok && !parser.document("Level", records, count)) result = parser.error;
    bool has_config = false;
    for (std::uint32_t i = 0; result == Error::ok && i < count; ++i) {
        auto& record = records[i];
        result = annotate(record, candidate.source_path, no_module, RecordKind::level, nullptr, d, bytes);
        if (result != Error::ok) break;
        if (!std::strcmp(record.gametype, "LevelConfig")) {
            if (has_config) { result = fail(d, Error::record, "Duplicate LevelConfig"); break; }
            candidate.config = record; record = {}; has_config = true;
        } else if (!std::strcmp(record.gametype, "Module")) {
            if (candidate.module_count == max_modules) { result = fail(d, Error::limit, "Too many modules"); break; }
            auto* grown = static_cast<Module*>(std::realloc(candidate.modules,
                (candidate.module_count + 1) * sizeof(Module)));
            if (!grown) { result = fail(d, Error::allocation, "Module allocation failed"); break; }
            candidate.modules = grown; auto& module = grown[candidate.module_count++]; module = {};
            module.record = record; record = {};
            module.record.module_index = candidate.module_count - 1;
            for (std::uint32_t j = 0; j + 1 < candidate.module_count; ++j)
                if (!std::strcmp(candidate.modules[j].record.name, module.record.name))
                    result = fail(d, Error::record, "Duplicate module instance name");
            if (result != Error::ok) break;
            const char* fields[] = {"dae", "mgp", "mvp"};
            char** destinations[] = {&module.cache_dae, &module.cache_mgp, &module.cache_mvp};
            for (unsigned j = 0; j < 3 && result == Error::ok; ++j) {
                const auto* value = dh2_world_field(&module.record, fields[j]);
                if (!value || !*value) result = fail(d, Error::missing_field, "Module asset reference missing");
                else result = normalized_owned(*destinations[j], value, d);
            }
            if (result != Error::ok) break;
            const auto* xref = dh2_world_field(&module.record, "xrefobject");
            if (!xref || !*xref || std::strlen(xref) > 240) {
                result = fail(d, Error::missing_field, "Module xrefobject missing/overlong"); break;
            }
            const auto length = std::strlen(xref);
            module.catalogue_node_id = static_cast<char*>(std::malloc(length + 6));
            if (!module.catalogue_node_id) { result = fail(d, Error::allocation, "Node ID allocation failed"); break; }
            std::memcpy(module.catalogue_node_id, xref, length);
            std::memcpy(module.catalogue_node_id + length, "-node", 6);
        } else { result = fail(d, Error::record, "Level contains unsupported non-module object"); }
    }
    free_objects(records, count);
    if (result == Error::ok && (!has_config || !candidate.module_count))
        result = fail(d, Error::record, "Level needs LevelConfig and modules");
    if (result != Error::ok) { dh2_world_free(&candidate); return result; }
    dh2_world_free(output); *output = candidate; return done(d);
}
extern "C" Error dh2_world_import_module_objects(Level* level, std::uint32_t index,
    RecordKind kind, const char* path, const std::uint8_t* bytes, std::size_t size,
    Diagnostic* d) {
    if (!level || index >= level->module_count || !bytes || !size ||
        (kind != RecordKind::mgp && kind != RecordKind::mvp))
        return fail(d, Error::argument, "Invalid module input");
    if (size > max_xml) return fail(d, Error::limit, "Module XML too large");
    auto& module = level->modules[index];
    auto& loaded = kind == RecordKind::mgp ? module.mgp_loaded : module.mvp_loaded;
    if (loaded) return fail(d, Error::already_loaded, "Module file already imported");
    if (!translation_only(module.record.local))
        return fail(d, Error::unsupported_transform, "Rotated/scaled module entity placement is not reconstructed");
    char canonical[max_path + 1];
    auto result = dh2_world_cache_path(canonical, sizeof canonical, path, d);
    if (result != Error::ok) return result;
    if (std::strcmp(canonical, kind == RecordKind::mgp ? module.cache_mgp : module.cache_mvp))
        return fail(d, Error::path, "Module source path does not match its reference");
    Object* records = nullptr; std::uint32_t count = 0; Parser parser{bytes, size, 0, d};
    if (!parser.document("Module", records, count)) result = parser.error;
    for (std::uint32_t i = 0; result == Error::ok && i < count; ++i)
        result = annotate(records[i], canonical, index, kind, module.record.local.position, d, bytes);
    if (result == Error::ok && count > max_entities - level->entity_count)
        result = fail(d, Error::limit, "Too many level entities");
    if (result != Error::ok) { free_objects(records, count); return result; }
    if (count) {
        auto* grown = static_cast<Object*>(std::realloc(level->entities,
            (level->entity_count + count) * sizeof(Object)));
        if (!grown) { free_objects(records, count); return fail(d, Error::allocation, "Entity allocation failed"); }
        level->entities = grown;
        std::memcpy(grown + level->entity_count, records, count * sizeof(Object));
        level->entity_count += count;
    }
    std::free(records); loaded = true; return done(d);
}
