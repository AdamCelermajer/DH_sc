#include "container_open_script_v1.hpp"

#include "script_runtime.h"

#include <cctype>
#include <cstdio>
#include <cstring>
#include <memory>
#include <set>
#include <utility>

namespace dh::foundation::containers {
namespace {

// Callback context: borrowed by the VM for the duration of one OnOpen call.
struct RunContext {
    const OpenScriptServicesV1* services = nullptr;
    OpenScriptRunReportV1* report = nullptr;
    std::string failure; // diagnostic text of the first failed callback
};

void set_error(RunContext& context, char* text, std::size_t capacity, const std::string& message) {
    context.failure = message;
    if (text && capacity) std::snprintf(text, capacity, "%s", message.c_str());
}

dh2_script_value number_value(double value) {
    dh2_script_value out{};
    out.type = DH2_SCRIPT_NUMBER;
    out.number = static_cast<float>(value);
    return out;
}

dh2_script_value boolean_value(bool value) {
    dh2_script_value out{};
    out.type = DH2_SCRIPT_BOOLEAN;
    out.boolean = value ? 1u : 0u;
    return out;
}

dh2_script_value text_value(const std::string& text) {
    dh2_script_value out{};
    out.type = DH2_SCRIPT_STRING;
    out.text = text.c_str();
    out.text_bytes = text.size();
    return out;
}

bool is_number(const dh2_script_value& v) { return v.type == DH2_SCRIPT_NUMBER; }
bool is_boolean(const dh2_script_value& v) { return v.type == DH2_SCRIPT_BOOLEAN; }
bool is_string(const dh2_script_value& v) { return v.type == DH2_SCRIPT_STRING; }

// GetRand(lo, hi)
int lua_get_rand(void* opaque, const dh2_script_value* args, std::uint32_t count, dh2_script_value* results,
                 std::uint32_t capacity, std::uint32_t* returned, char* error, std::size_t error_capacity) {
    auto& context = *static_cast<RunContext*>(opaque);
    if (!args || count != 2 || !is_number(args[0]) || !is_number(args[1]) || !results || !capacity || !returned) {
        set_error(context, error, error_capacity, "GetRand expects (lo, hi) numbers");
        return 1;
    }
    const auto lo = static_cast<std::int32_t>(args[0].number);
    const auto hi = static_cast<std::int32_t>(args[1].number);
    std::int32_t value = 0;
    std::string reason;
    if (!context.services->random || !context.services->random(lo, hi, value, reason)) {
        set_error(context, error, error_capacity, "GetRand failed: " + reason);
        return 1;
    }
    ++context.report->draws;
    results[0] = number_value(value);
    *returned = 1;
    return 0;
}

// GetPyOID("CharacterTable", row) -> row index
int lua_get_py_oid(void* opaque, const dh2_script_value* args, std::uint32_t count, dh2_script_value* results,
                   std::uint32_t capacity, std::uint32_t* returned, char* error, std::size_t error_capacity) {
    auto& context = *static_cast<RunContext*>(opaque);
    if (!args || count != 2 || !is_string(args[0]) || !is_string(args[1]) || !results || !capacity || !returned) {
        set_error(context, error, error_capacity, "GetPyOID expects (group, row) strings");
        return 1;
    }
    const std::string group(args[0].text, args[0].text_bytes);
    const std::string row(args[1].text, args[1].text_bytes);
    if (group != "CharacterTable" || !context.services->character_rows) {
        set_error(context, error, error_capacity, "GetPyOID group not bound: " + group);
        return 1;
    }
    const auto& rows = *context.services->character_rows;
    for (std::size_t i = 0; i < rows.size(); ++i) {
        if (rows[i] == row) {
            results[0] = number_value(static_cast<double>(i));
            *returned = 1;
            return 0;
        }
    }
    set_error(context, error, error_capacity, "GetPyOID CharacterTable row not found: " + row);
    return 1;
}

// Summon(oid, spawn, x, y, z, relative)
int lua_summon(void* opaque, const dh2_script_value* args, std::uint32_t count, dh2_script_value* results,
               std::uint32_t capacity, std::uint32_t* returned, char* error, std::size_t error_capacity) {
    auto& context = *static_cast<RunContext*>(opaque);
    if (!args || count != 6 || !is_number(args[0]) || !is_boolean(args[1]) || !is_number(args[2]) ||
        !is_number(args[3]) || !is_number(args[4]) || !is_boolean(args[5]) || !results || !capacity || !returned) {
        set_error(context, error, error_capacity, "Summon expects (oid, spawn, x, y, z, relative)");
        return 1;
    }
    const auto& rows = context.services->character_rows ? *context.services->character_rows : std::vector<std::string>{};
    const auto index = static_cast<std::int64_t>(args[0].number);
    if (index < 0 || static_cast<std::size_t>(index) >= rows.size()) {
        set_error(context, error, error_capacity, "Summon oid is not a CharacterTable row");
        return 1;
    }
    OpenScriptSummonRequestV1 request;
    request.character = rows[static_cast<std::size_t>(index)];
    request.spawn = args[1].boolean != 0;
    request.offset[0] = args[2].number;
    request.offset[1] = args[3].number;
    request.offset[2] = args[4].number;
    request.relative = args[5].boolean != 0;
    if (!context.services->summon) {
        set_error(context, error, error_capacity, "Summon owner is not bound");
        return 1;
    }
    std::string reason;
    ++context.report->summon_calls;
    const bool spawned = context.services->summon(request, reason);
    if (spawned) ++context.report->summons_spawned;
    context.report->summon_lines.push_back("Summon character=" + request.character +
                                           " spawn=" + (request.spawn ? "1" : "0") +
                                           " outcome=" + (spawned ? std::string("spawned")
                                                                  : "refused reason=" + reason));
    results[0] = boolean_value(spawned);
    *returned = 1;
    return 0;
}

// Trace(text)
int lua_trace(void* opaque, const dh2_script_value* args, std::uint32_t count, dh2_script_value*, std::uint32_t,
              std::uint32_t* returned, char* error, std::size_t error_capacity) {
    auto& context = *static_cast<RunContext*>(opaque);
    if (!args || count != 1 || !is_string(args[0]) || !returned) {
        set_error(context, error, error_capacity, "Trace expects one string");
        return 1;
    }
    context.report->trace_lines.emplace_back(args[0].text, args[0].text_bytes);
    *returned = 0;
    return 0;
}

// GetName()
int lua_get_name(void* opaque, const dh2_script_value*, std::uint32_t count, dh2_script_value* results,
                 std::uint32_t capacity, std::uint32_t* returned, char* error, std::size_t error_capacity) {
    auto& context = *static_cast<RunContext*>(opaque);
    if (count != 0 || !results || !capacity || !returned) {
        set_error(context, error, error_capacity, "GetName expects no arguments");
        return 1;
    }
    results[0] = text_value(context.services->object_name);
    *returned = 1;
    return 0;
}

// Text of a Lua source with `--` comments and string literals handled, for the reservation scan.
std::string without_line_comments(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size();) {
        if (text[i] == '"') {
            out.push_back(text[i++]);
            while (i < text.size() && text[i] != '"' && text[i] != '\n') out.push_back(text[i++]);
            if (i < text.size() && text[i] == '"') out.push_back(text[i++]);
            continue;
        }
        if (text.compare(i, 2, "--") == 0) {
            while (i < text.size() && text[i] != '\n') ++i;
            continue;
        }
        out.push_back(text[i++]);
    }
    return out;
}

} // namespace

bool run_authored_open_script_v1(const std::string& source, const std::string& chunk_name,
                                 const OpenScriptServicesV1& services, OpenScriptRunReportV1& report,
                                 std::string& error) {
    error.clear();
    if (!services.random || !services.summon) {
        error = "Open script requires the GetRand and Summon services";
        return false;
    }
    std::unique_ptr<dh2_script_vm, void (*)(dh2_script_vm*)> vm(dh2_script_vm_create(1024u * 1024u),
                                                                 dh2_script_vm_destroy);
    if (!vm) {
        error = "Open script VM allocation failed: " + chunk_name;
        return false;
    }
    if (dh2_script_vm_open_source_libraries(vm.get()) != 0) {
        error = "Open script libraries failed: " + chunk_name;
        return false;
    }
    RunContext context;
    context.services = &services;
    context.report = &report;
    const std::pair<const char*, dh2_script_function> bindings[] = {
        {"GetRand", lua_get_rand}, {"GetPyOID", lua_get_py_oid}, {"Summon", lua_summon},
        {"Trace", lua_trace}, {"GetName", lua_get_name},
    };
    for (const auto& binding : bindings) {
        if (dh2_script_vm_bind_source_values(vm.get(), binding.first, binding.second, &context) != 0) {
            error = std::string("Open script could not bind ") + binding.first + ": " + chunk_name;
            return false;
        }
    }
    if (dh2_script_vm_load_source_file(vm.get(), source.data(), source.size()) != 0) {
        const char* message = dh2_script_vm_error(vm.get());
        error = "Authored open script failed to load (" + chunk_name + "): " + (message ? message : "");
        return false;
    }
    // OnOpen(actor) with a nil actor (see the header note).
    dh2_script_value nil_actor{};
    nil_actor.type = DH2_SCRIPT_NIL;
    if (dh2_script_vm_call_discard_source(vm.get(), "OnOpen", &nil_actor, 1) != 0) {
        const char* message = dh2_script_vm_error(vm.get());
        error = "Authored OnOpen failed (" + chunk_name + "): " +
                (context.failure.empty() ? (message ? message : "") : context.failure);
        return false;
    }
    return true;
}

std::vector<std::string> authored_open_summon_rows_v1(const std::string& source) {
    const std::string s = without_line_comments(source);
    std::set<std::string> rows;
    const std::string summon = "Summon(";
    for (std::size_t at = s.find(summon); at != std::string::npos; at = s.find(summon, at + 1)) {
        std::size_t p = at + summon.size();
        auto skip = [&] { while (p < s.size() && std::isspace(static_cast<unsigned char>(s[p]))) ++p; };
        auto literal = [&](const char* text) {
            skip();
            const std::size_t n = std::strlen(text);
            if (s.compare(p, n, text) != 0) return false;
            p += n;
            return true;
        };
        skip();
        if (!literal("GetPyOID") || !literal("(") || !literal("\"CharacterTable\"") || !literal(",")) continue;
        skip();
        if (p >= s.size() || s[p] != '"') continue;
        const std::size_t close = s.find('"', p + 1);
        if (close == std::string::npos) continue;
        rows.insert(s.substr(p + 1, close - p - 1));
    }
    return {rows.begin(), rows.end()};
}

} // namespace dh::foundation::containers
