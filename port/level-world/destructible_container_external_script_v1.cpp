#include "destructible_container_external_script_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <new>
#include <utility>
#include <vector>

namespace dh2::destructible_container_external_script_v1 {
namespace {
struct BusyScope {
    bool& busy;
    explicit BusyScope(bool& value) : busy(value) { busy = true; }
    ~BusyScope() { busy = false; }
};

bool valid_source(Source source) {
    return source.bytes && source.size && source.size <= 8U * 1024U * 1024U &&
        source.path && source.path[0] && std::strlen(source.path) < 4096;
}

std::uint32_t source_uinteger(float value) noexcept {
    if (!std::isfinite(value) || value <= 0.0f) return 0;
    if (static_cast<double>(value) >= 4294967296.0) return UINT32_MAX;
    return static_cast<std::uint32_t>(value);
}

void set_number(dh2_script_value& output, float value) noexcept {
    output = {};
    output.type = DH2_SCRIPT_NUMBER;
    output.number = value;
}

void set_identity(dh2_script_value& output, std::uintptr_t identity) noexcept {
    output = {};
    output.type = DH2_SCRIPT_IDENTITY;
    output.identity = identity;
}

int fail(char* output, std::size_t capacity, const char* message) noexcept {
    if (output && capacity) std::snprintf(output, capacity, "%s", message);
    return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
} // namespace

struct Owner::Impl {
    struct NativeBinding {
        Impl* owner{};
        enum class Kind : std::uint8_t { random, py_oid, summon, trace } kind{};
    };

    dh2_script_vm* vm{};
    Services services{};
    std::uintptr_t caller{};
    std::string source_path;
    std::vector<NativeBinding> bindings;
    std::uint32_t open_calls{};

    ~Impl() { dh2_script_vm_destroy(vm); }

    static int native(void* raw, const dh2_script_value* args,
                      std::uint32_t count, dh2_script_value* output,
                      std::uint32_t capacity, std::uint32_t* returned,
                      char* error, std::size_t error_capacity) noexcept {
        auto* binding = static_cast<NativeBinding*>(raw);
        if (!binding || !binding->owner || !returned || (!args && count))
            return fail(error, error_capacity, "invalid container ExternalScript callback");
        auto& self = *binding->owner;
        *returned = 0;
        try {
            if (binding->kind == NativeBinding::Kind::trace) return 0;
            if (binding->kind == NativeBinding::Kind::random) {
                if (count > 2 || (count && args[0].type != DH2_SCRIPT_NUMBER) ||
                    (count == 2 && args[1].type != DH2_SCRIPT_NUMBER)) return 0;
                if (!self.services.get_random || !capacity || !output)
                    return fail(error, error_capacity, "GameObject::GetRand provider is unavailable");
                std::uint32_t base = 0;
                std::uint32_t bound = 100;
                if (count == 1) {
                    bound = source_uinteger(args[0].number);
                } else if (count == 2) {
                    base = source_uinteger(args[0].number);
                    bound = source_uinteger(args[1].number) - base;
                }
                std::int32_t draw = 0;
                std::string callback_error;
                if (!self.services.get_random(self.services.context, bound, draw,
                                              callback_error))
                    return fail(error, error_capacity, callback_error.empty()
                        ? "GameObject::GetRand source RNG failed" : callback_error.c_str());
                set_number(output[0], static_cast<float>(static_cast<std::uint32_t>(draw) + base));
                *returned = 1;
                return 0;
            }
            if (binding->kind == NativeBinding::Kind::py_oid) {
                if (count != 2 || args[0].type != DH2_SCRIPT_STRING ||
                    args[1].type != DH2_SCRIPT_STRING) return 0;
                if (!self.services.get_py_oid || !capacity || !output)
                    return fail(error, error_capacity, "LuaScript::GetPyOID provider is unavailable");
                const std::string category(args[0].text ? args[0].text : "",
                                           args[0].text_bytes);
                const std::string member(args[1].text ? args[1].text : "",
                                         args[1].text_bytes);
                std::int32_t value = -1;
                std::string callback_error;
                if (!self.services.get_py_oid(self.services.context, category.c_str(),
                        member.c_str(), value, callback_error))
                    return fail(error, error_capacity, callback_error.empty()
                        ? "LuaScript::GetPyOID lookup failed" : callback_error.c_str());
                set_number(output[0], static_cast<float>(value));
                *returned = 1;
                return 0;
            }
            if (!self.services.summon || !capacity || !output)
                return fail(error, error_capacity, "GameObject::_Summon canonical factory is unavailable");
            SummonResult result{};
            std::string callback_error;
            if (!self.services.summon(self.services.context, self.caller, args,
                                      count, result, callback_error))
                return fail(error, error_capacity, callback_error.empty()
                    ? "GameObject::_Summon canonical factory failed" : callback_error.c_str());
            if (result.returned_object) {
                if (!result.identity)
                    return fail(error, error_capacity, "GameObject::_Summon returned an invalid ObjectManager identity");
                set_identity(output[0], result.identity);
                *returned = 1;
            }
            return 0;
        } catch (...) {
            return fail(error, error_capacity, "container ExternalScript provider threw");
        }
    }

    bool bind(const char* name, NativeBinding::Kind kind) {
        bindings.push_back({this, kind});
        return dh2_script_vm_bind_source_values(vm, name, native, &bindings.back()) == 0;
    }
};

Owner::Owner() = default;

Owner::~Owner() {
    busy_ = true;
    impl_.reset();
}

Status Owner::initialize(Source source, std::uintptr_t caller,
                         const Services& services, std::string& error,
                         std::size_t memory_limit) {
    error.clear();
    if (busy_) { error = "container ExternalScript is busy"; return Status::busy; }
    if (!valid_source(source) || !caller || !services.get_random ||
        !services.get_py_oid) {
        error = "invalid per-GameObject ExternalScript source or providers";
        return Status::invalid_argument;
    }
    BusyScope scope(busy_);
    auto candidate = std::unique_ptr<Impl>(new (std::nothrow) Impl());
    if (!candidate) { error = "container ExternalScript allocation failed"; return Status::allocation_failed; }
    candidate->vm = dh2_script_vm_create_deferred(memory_limit);
    if (!candidate->vm) { error = "container ExternalScript Lua5.1 VM allocation failed"; return Status::allocation_failed; }
    candidate->services = services;
    candidate->caller = caller;
    try {
        candidate->source_path.assign(source.path);
        candidate->bindings.reserve(4);
    } catch (...) {
        error = "container ExternalScript owner storage allocation failed";
        return Status::allocation_failed;
    }
    for (const auto library : {DH2_SCRIPT_LIBRARY_BASE, DH2_SCRIPT_LIBRARY_MATH,
                               DH2_SCRIPT_LIBRARY_TABLE, DH2_SCRIPT_LIBRARY_STRING}) {
        if (dh2_script_vm_open_library(candidate->vm, library) != 0) {
            error = dh2_script_vm_error(candidate->vm);
            if (error.empty()) error = "LuaScript::BindFunction library initialization failed";
            return Status::source_load_failed;
        }
    }
    if (!candidate->bind("GetRand", Impl::NativeBinding::Kind::random) ||
        !candidate->bind("Trace", Impl::NativeBinding::Kind::trace) ||
        !candidate->bind("GetPyOID", Impl::NativeBinding::Kind::py_oid) ||
        !candidate->bind("Summon", Impl::NativeBinding::Kind::summon)) {
        error = dh2_script_vm_error(candidate->vm);
        if (error.empty()) error = "container ExternalScript native binding failed";
        return Status::source_load_failed;
    }
    if (dh2_script_vm_load_source_file(candidate->vm, source.bytes, source.size) != 0) {
        error = dh2_script_vm_error(candidate->vm);
        if (error.empty()) error = "GameObject::LoadExternalScript source load failed";
        return Status::source_load_failed;
    }
    impl_ = std::move(candidate);
    error.clear();
    return Status::complete;
}

Status Owner::on_open(std::uintptr_t opener, std::string& error) {
    error.clear();
    if (busy_) { error = "container ExternalScript is busy"; return Status::busy; }
    if (!impl_ || !impl_->vm) { error = "per-GameObject ExternalScript is not loaded"; return Status::service_unavailable; }
    BusyScope scope(busy_);
    int status = 0;
    if (!opener) {
        status = dh2_script_vm_call_discard_source(impl_->vm,
            "OnOpen", nullptr, 0);
    } else {
        dh2_script_value argument{};
        argument.type = DH2_SCRIPT_IDENTITY;
        argument.identity = opener;
        status = dh2_script_vm_call_discard_source(impl_->vm,
            "OnOpen", &argument, 1);
    }
    if (status != 0) {
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "Container::DoOpen ExternalScript::OnOpen failed";
        return Status::callback_failed;
    }
    ++impl_->open_calls;
    error.clear();
    return Status::complete;
}

Status Owner::reset(std::string& error) noexcept {
    error.clear();
    if (busy_) { error = "container ExternalScript is busy"; return Status::busy; }
    busy_ = true;
    impl_.reset();
    busy_ = false;
    return Status::complete;
}

bool Owner::ready() const noexcept { return impl_ && impl_->vm; }
std::uintptr_t Owner::caller_identity() const noexcept { return impl_ ? impl_->caller : 0; }
std::uintptr_t Owner::vm_identity() const noexcept {
    return impl_ ? reinterpret_cast<std::uintptr_t>(impl_->vm) : 0;
}

const char* status_name(Status status) noexcept {
    switch (status) {
    case Status::complete: return "complete";
    case Status::invalid_argument: return "invalid_argument";
    case Status::allocation_failed: return "allocation_failed";
    case Status::service_unavailable: return "service_unavailable";
    case Status::source_load_failed: return "source_load_failed";
    case Status::callback_failed: return "callback_failed";
    case Status::busy: return "busy";
    }
    return "unknown";
}

} // namespace dh2::destructible_container_external_script_v1
