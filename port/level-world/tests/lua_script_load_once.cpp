#include "../lua_script_load_once.hpp"

#include "../../adam-script-runtime/script_runtime.h"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <new>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace dh2::lua_script_load_once;

namespace {
unsigned checks = 0;

void check(bool condition, const char* message) {
    ++checks;
    if (!condition) {
        std::fprintf(stderr, "failed check %u: %s\n", checks, message);
        std::abort();
    }
}

std::vector<unsigned char> read_bytes(const char* path) {
    std::ifstream input(path, std::ios::binary);
    check(static_cast<bool>(input), "could not open skill common fixture");
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

float number(dh2_script_vm* vm, const char* name) {
    dh2_script_value value{};
    check(dh2_script_vm_get_global(vm, name, &value) == 0, "global lookup failed");
    check(value.type == DH2_SCRIPT_NUMBER, "expected a Lua number global");
    return value.number;
}

unsigned function_or_nil(dh2_script_vm* vm, const char* name) {
    dh2_script_value value{};
    check(dh2_script_vm_get_global(vm, name, &value) == 0, "function lookup failed");
    return value.type;
}

struct VmContext {
    dh2_script_vm* vm = nullptr;
    State* cache = nullptr;
    std::uintptr_t identity = 0;
    std::uint32_t calls = 0;
    bool trigger_reentry = false;
    bool trigger_reset = false;
    bool throw_on_load = false;
    Status nested_status = Status::invalid_argument;
    Status reset_status = Status::invalid_argument;
    Source nested_source{};
    Services nested_services{};
    Result nested_result{};
};

std::int32_t vm_load(void* opaque, std::uintptr_t identity, const void* bytes,
                    std::size_t byte_count, const char* chunk_name) {
    auto& context = *static_cast<VmContext*>(opaque);
    if (!context.vm || identity != context.identity) return -81;
    ++context.calls;
    if (context.throw_on_load) {
        context.throw_on_load = false;
        throw std::runtime_error("host loader exception");
    }
    if (context.trigger_reentry) {
        context.trigger_reentry = false;
        context.nested_status = load_once(context.cache, &context.nested_source,
                                          &context.nested_services,
                                          &context.nested_result);
    }
    if (context.trigger_reset) {
        context.trigger_reset = false;
        context.reset_status = reset(context.cache, context.identity);
    }
    return dh2_script_vm_load(context.vm, bytes, byte_count, chunk_name);
}

void set_number(dh2_script_value& value, float number_value) {
    value = {};
    value.type = DH2_SCRIPT_NUMBER;
    value.number = number_value;
}

void set_string(dh2_script_value& value, const char* text) {
    value = {};
    value.type = DH2_SCRIPT_STRING;
    value.text = text;
    value.text_bytes = std::strlen(text);
}

Status load_text(State& state, VmContext& context, const char* name,
                 const char* text, Result& result) {
    const Source source{context.identity, name, text, std::strlen(text)};
    const Services services{&context, vm_load};
    return load_once(&state, &source, &services, &result);
}

void test_common_path_cache(const std::vector<unsigned char>& commons) {
    auto* vm = dh2_script_vm_create(8 * 1024 * 1024);
    check(vm != nullptr, "first Lua VM allocation failed");
    State state;
    const auto identity = reinterpret_cast<std::uintptr_t>(vm);
    check(reset(&state, identity) == Status::complete, "initial VM bind failed");
    VmContext context{vm, &state, identity};
    const Services services{&context, vm_load};
    Result result{};
    const Source common{identity, "data/scripts/skills/_commons.luac",
                        commons.data(), commons.size()};
    check(load_once(&state, &common, &services, &result) == Status::complete &&
          result.source_success == 1 && result.cache_hit == 0 && result.load_called == 1,
          "skill _commons first load failed");

    // DeclareSkill receives its loop slot through a real float32 Lua number.
    // The following loaded skill chunk registers into _commons' local SKILLS
    // closure, then selection runs that stored callback.
    dh2_script_value declaration[2]{};
    set_string(declaration[0], "AUDIT_SKILL");
    set_number(declaration[1], 3.25f);
    check(dh2_script_vm_call_discard_source(vm, "DeclareSkill", declaration, 2) == 0,
          "real float32 DeclareSkill call failed");
    const char* skill_source =
        "RegisterSkill(function() hits=hits+1 end,"
        "function() return true,false end,function() end,function() end);"
        "SetSkill('AUDIT_SKILL',0); hits=0";
    Result skill_result{};
    check(load_text(state, context, "data/scripts/skills/audit_skill.luac",
                    skill_source, skill_result) == Status::complete &&
          skill_result.source_success == 1 && skill_result.load_called == 1,
          "second distinct Lua path did not execute");
    check(dh2_script_vm_call_discard_source(vm, "OnSkillUpdate", nullptr, 0) == 0 &&
          number(vm, "hits") == 1.0f, "registered skill callback did not run");

    // The original AddFile per-LuaScript path set returns true before touching
    // StreamBuffer/Instance::loadFile. The same is tested with unavailable
    // bytes and no loader service: the custom registry must remain intact.
    const Source duplicate{identity, "data/scripts/skills/_commons.luac", nullptr, 0};
    Result duplicate_result{};
    check(load_once(&state, &duplicate, nullptr, &duplicate_result) == Status::complete &&
          duplicate_result.source_success == 1 && duplicate_result.cache_hit == 1 &&
          duplicate_result.load_called == 0 && context.calls == 2,
          "same-VM duplicate path called the VM loader");
    check(dh2_script_vm_call_discard_source(vm, "OnSkillUpdate", nullptr, 0) == 0 &&
          number(vm, "hits") == 2.0f,
          "duplicate _commons path reset the source SKILLS registration");
    check(loaded_path_count(&state) == 2 &&
          contains_path(&state, "data/scripts/skills/_commons.luac") &&
          contains_path(&state, "data/scripts/skills/audit_skill.luac"),
          "successful paths were not retained exactly");

    Result wrong_vm_result{};
    const Source wrong_vm{identity + 16, "data/scripts/skills/_commons.luac",
                          commons.data(), commons.size()};
    check(load_once(&state, &wrong_vm, &services, &wrong_vm_result) == Status::vm_mismatch &&
          context.calls == 2, "cache accepted a different VM without reset");
    dh2_script_vm_destroy(vm);
}

void test_failures_and_retry() {
    auto* vm = dh2_script_vm_create(8 * 1024 * 1024);
    check(vm != nullptr, "failure-test VM allocation failed");
    State state;
    const auto identity = reinterpret_cast<std::uintptr_t>(vm);
    check(reset(&state, identity) == Status::complete, "failure-test VM bind failed");
    VmContext context{vm, &state, identity};

    Result syntax_result{};
    check(load_text(state, context, "data/scripts/skills/bad_syntax.luac",
                    "local = 1", syntax_result) == Status::load_failed &&
          syntax_result.vm_status == -2 && syntax_result.source_success == 0 &&
          !contains_path(&state, "data/scripts/skills/bad_syntax.luac"),
          "syntax failure was cached or lost its VM status");
    check(load_text(state, context, "data/scripts/skills/bad_syntax.luac",
                    "syntax_retry=1", syntax_result) == Status::complete &&
          number(vm, "syntax_retry") == 1.0f,
          "syntax-failed path could not be retried");

    Result runtime_result{};
    check(load_text(state, context, "data/scripts/skills/partial_error.luac",
                    "partial_effect=(partial_effect or 0)+1; error('audit failure')",
                    runtime_result) == Status::load_failed,
          "runtime failure did not propagate");
    const bool runtime_error_retained =
        std::strstr(dh2_script_vm_error(vm), "audit failure") != nullptr;
    check(runtime_result.vm_status == -2 && number(vm, "partial_effect") == 1.0f &&
          !contains_path(&state, "data/scripts/skills/partial_error.luac"),
          "runtime error rolled back effects or was cached");
    check(runtime_error_retained,
          "VM's original error text was not retained");
    Result retry_result{};
    check(load_text(state, context, "data/scripts/skills/partial_error.luac",
                    "runtime_retry=(runtime_retry or 0)+1", retry_result) == Status::complete &&
          retry_result.source_success == 1 &&
          number(vm, "runtime_retry") == 1.0f &&
          number(vm, "partial_effect") == 1.0f,
          "runtime-failed path retry did not preserve earlier effects");

    Result service_exception{};
    context.throw_on_load = true;
    check(load_text(state, context, "data/scripts/skills/service_exception.luac",
                    "service_retry=1", service_exception) == Status::service_failed &&
          service_exception.load_called == 1 &&
          service_exception.vm_status == std::numeric_limits<std::int32_t>::min() &&
          !contains_path(&state, "data/scripts/skills/service_exception.luac"),
          "thrown loader service was cached or misreported");
    Result service_retry{};
    check(load_text(state, context, "data/scripts/skills/service_exception.luac",
                    "service_retry=1", service_retry) == Status::complete &&
          number(vm, "service_retry") == 1.0f,
          "loader cache did not recover after service exception");
    check(context.calls == 6, "failure/retry VM call count differs");
    dh2_script_vm_destroy(vm);
}

void test_vm_reset_and_reentry(const std::vector<unsigned char>& commons) {
    auto* first = dh2_script_vm_create(8 * 1024 * 1024);
    auto* second = dh2_script_vm_create(8 * 1024 * 1024);
    check(first != nullptr && second != nullptr, "reset-test VM allocation failed");
    State state;
    const auto first_identity = reinterpret_cast<std::uintptr_t>(first);
    const auto second_identity = reinterpret_cast<std::uintptr_t>(second);
    check(reset(&state, first_identity) == Status::complete, "reset-test first bind failed");
    VmContext context{first, &state, first_identity};
    Services services{&context, vm_load};
    Result result{};
    const Source first_common{first_identity, "data/scripts/skills/_commons.luac",
                              commons.data(), commons.size()};
    check(load_once(&state, &first_common, &services, &result) == Status::complete,
          "first VM did not execute common path");
    check(loaded_path_count(&state) == 1 &&
          contains_path(&state, "data/scripts/skills/_commons.luac"),
          "first VM path set did not retain its successful load");

    // Replace the VM, then explicitly reset the per-VM path set. The exact same
    // source path must execute again in the replacement VM.
    dh2_script_vm_destroy(first);
    check(reset(&state, second_identity) == Status::complete,
          "reset did not bind replacement VM");
    context.vm = second;
    context.identity = second_identity;
    const Services second_services{&context, vm_load};
    const Source second_common{second_identity, "data/scripts/skills/_commons.luac",
                               commons.data(), commons.size()};
    check(load_once(&state, &second_common, &second_services, &result) == Status::complete &&
          result.load_called == 1 && context.calls == 2 &&
          function_or_nil(second, "DeclareSkill") == DH2_SCRIPT_FUNCTION,
          "replacement VM did not execute the common path anew");
    context.nested_source = {second_identity, "data/scripts/skills/nested.luac",
                             "nested=1", 8};
    context.nested_services = {&context, vm_load};
    context.trigger_reentry = true;
    Result outer_result{};
    check(load_text(state, context, "data/scripts/skills/outer.luac",
                    "outer=1", outer_result) == Status::complete &&
          context.nested_status == Status::busy && context.calls == 3 &&
          contains_path(&state, "data/scripts/skills/outer.luac") &&
          !contains_path(&state, "data/scripts/skills/nested.luac"),
          "synchronous re-entry executed or cached the nested path");
    dh2_script_vm_destroy(second);
}

void test_control_guards() {
    auto* vm = dh2_script_vm_create(8 * 1024 * 1024);
    check(vm != nullptr, "guard-test VM allocation failed");
    State state;
    const auto identity = reinterpret_cast<std::uintptr_t>(vm);
    check(reset(&state, identity) == Status::complete, "guard-test VM bind failed");
    VmContext context{vm, &state, identity};
    Services services{&context, vm_load};
    Result result{};
    const char shared[] = "x=1";
    const Source overlap_source{identity, shared, shared, sizeof(shared)};
    check(load_once(&state, &overlap_source, &services, &result) == Status::invalid_source &&
          context.calls == 0, "overlapping path/script byte ranges were accepted");

    alignas(Source) unsigned char unaligned_storage[sizeof(Source) + alignof(Source)]{};
    const auto* unaligned_source = reinterpret_cast<const Source*>(unaligned_storage + 1);
    check(load_once(&state, unaligned_source, &services, &result) ==
          Status::invalid_argument && context.calls == 0,
          "misaligned source control was dereferenced or accepted");

    union SourceServices {
        Source source;
        Services services;
        SourceServices() : source{} {}
        ~SourceServices() {}
    } source_services_alias;
    const auto calls_before_alias = context.calls;
    check(load_once(&state, &source_services_alias.source,
                    reinterpret_cast<const Services*>(&source_services_alias.source),
                    &result) == Status::invalid_argument &&
          context.calls == calls_before_alias,
          "overlapping source/service controls were accepted");

    const Source missing_bytes{identity, "data/scripts/skills/missing.luac", nullptr, 0};
    check(load_once(&state, &missing_bytes, &services, &result) == Status::invalid_source &&
          context.calls == 0, "cache miss accepted absent bytes");

    // A Request and Result sharing the same storage must be rejected before
    // either record is read or written.
    union RequestResult {
        Source source;
        Result result;
        RequestResult() : source{} {}
        ~RequestResult() {}
    } aliased;
    const auto before = context.calls;
    check(load_once(&state, &aliased.source, &services,
                    reinterpret_cast<Result*>(&aliased.source)) == Status::invalid_argument &&
          context.calls == before,
          "overlapping request/result controls were accepted");

    check(reset(&state, identity) == Status::complete, "guard-test reset failed");
    auto* reentrant_vm = dh2_script_vm_create(8 * 1024 * 1024);
    check(reentrant_vm != nullptr, "reentry-test VM allocation failed");
    dh2_script_vm_destroy(vm);
    const auto reentrant_identity = reinterpret_cast<std::uintptr_t>(reentrant_vm);
    check(reset(&state, reentrant_identity) == Status::complete,
          "reentry-test VM bind failed");
    context.vm = reentrant_vm;
    context.identity = reentrant_identity;
    context.calls = 0;
    context.trigger_reentry = true;
    context.nested_source = {reentrant_identity, "nested-reset-target",
                             "nested=1", 8};
    context.nested_services = {&context, vm_load};
    const char outer_code[] = "outer=1";
    const Source outer{reentrant_identity, "outer-reset-target",
                       outer_code, sizeof(outer_code) - 1};
    const Services outer_services{&context, vm_load};
    Result outer_result{};
    context.trigger_reset = true;
    check(load_once(&state, &outer, &outer_services, &outer_result) == Status::complete,
          "outer reentrant test load failed");
    check(context.nested_status == Status::busy,
          "nested loader call did not report busy");
    check(context.reset_status == Status::busy,
          "cache reset from active VM callback was not rejected");
    check(reset(&state, reentrant_identity) == Status::complete,
          "reset after synchronous callback did not recover");
    dh2_script_vm_destroy(reentrant_vm);
}
}  // namespace

int main(int argc, char** argv) {
    static_assert(sizeof(float) == 4, "Lua skill values must be float32");
    static_assert(std::is_same<decltype(dh2_script_value::number), float>::value,
                  "the runtime ABI must preserve float32 Lua numbers");
    check(argc == 2, "expected path to recovered skills _commons");
    const auto commons = read_bytes(argv[1]);
    check(commons.size() == 11291, "unexpected skills _commons byte count");
    test_common_path_cache(commons);
    test_failures_and_retry();
    test_vm_reset_and_reentry(commons);
    test_control_guards();
    std::printf("{\"validation\":\"PASS\",\"checks\":%u,\"source_commons_loads\":2,"
                "\"distinct_paths\":2,\"failure_retries\":2,\"runtime_partial_effect_preserved\":true,"
                "\"service_exception_retry\":true,\"reset_executes_in_new_vm\":true,"
                "\"duplicate_skills_commons_preserved_registry\":true,"
                "\"reentry_rejected\":true,\"float32\":true,\"mismatches\":0}\n", checks);
    return 0;
}
