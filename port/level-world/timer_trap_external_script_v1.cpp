#include "timer_trap_external_script_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <new>
#include <utility>
#include <vector>

namespace dh2::timer_trap_external_script_v1 {
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

int provider_failure(char* output, std::size_t capacity, const std::string& detail,
                     const char* fallback) noexcept {
    if (output && capacity)
        std::snprintf(output, capacity, "%s", detail.empty() ? fallback : detail.c_str());
    return 1;
}
} // namespace

struct Owner::Impl {
    struct NativeBinding {
        Impl* owner{};
        enum class Kind : std::uint8_t {
            trace, py_oid, py_constant, deal_damages, remove, get_owner, get_damager
        } kind{};
    };

    dh2_script_vm* vm{};
    Services services{};
    std::string source_path;
    std::vector<NativeBinding> bindings;

    ~Impl() { dh2_script_vm_destroy(vm); }

    static int native(void* raw, const dh2_script_value* args, std::uint32_t count,
                      dh2_script_value* output, std::uint32_t capacity,
                      std::uint32_t* returned, char* error,
                      std::size_t error_capacity) noexcept {
        auto* binding = static_cast<NativeBinding*>(raw);
        if (!binding || !binding->owner || !returned || (!args && count))
            return fail(error, error_capacity, "invalid TriggerTrap ExternalScript callback");
        auto& self = *binding->owner;
        *returned = 0;
        using Kind = NativeBinding::Kind;
        try {
            switch (binding->kind) {
            case Kind::trace:
                return 0;
            case Kind::py_oid:
            case Kind::py_constant: {
                if (count != 2 || args[0].type != DH2_SCRIPT_STRING ||
                    args[1].type != DH2_SCRIPT_STRING) return 0;
                const auto provider = binding->kind == Kind::py_oid
                    ? self.services.get_py_oid : self.services.get_py_constant;
                if (!provider || !capacity || !output)
                    return fail(error, error_capacity, binding->kind == Kind::py_oid
                        ? "LuaScript::GetPyOID provider is unavailable"
                        : "LuaScript::GetPyCst provider is unavailable");
                const std::string category(args[0].text ? args[0].text : "", args[0].text_bytes);
                const std::string member(args[1].text ? args[1].text : "", args[1].text_bytes);
                std::int32_t value = -1;
                std::string detail;
                if (!provider(self.services.context, category.c_str(), member.c_str(),
                              value, detail))
                    return provider_failure(error, error_capacity, detail,
                                            "PyData lookup failed");
                set_number(output[0], static_cast<float>(value));
                *returned = 1;
                return 0;
            }
            case Kind::deal_damages: {
                // GameObject::_DealDamages 0x3906fc: more than two arguments,
                // an object first and a number second, the damager below
                // Arrays::GameObjectDamager::size (checked by the provider).
                if (count <= 2 || (args[0].type != DH2_SCRIPT_IDENTITY && args[0].type != 7) ||
                    args[1].type != DH2_SCRIPT_NUMBER || !std::isfinite(args[1].number) ||
                    args[1].number < 0.0f || args[0].identity == 0) return 0;
                if (!self.services.deal_damages)
                    return fail(error, error_capacity, "GameObject::_DealDamages provider is unavailable");
                bool character = false;
                std::int32_t amount = 0;
                std::string detail;
                if (!self.services.deal_damages(self.services.context, args[0].identity,
                        static_cast<std::int32_t>(args[1].number), character, amount, detail))
                    return provider_failure(error, error_capacity, detail,
                                            "GameObject::_DealDamages failed");
                if (character && capacity && output) {
                    set_number(output[0], static_cast<float>(amount));
                    *returned = 1;
                }
                return 0;
            }
            case Kind::remove: {
                if (!self.services.remove)
                    return fail(error, error_capacity, "TriggerTrap::_Remove provider is unavailable");
                std::string detail;
                if (!self.services.remove(self.services.context, detail))
                    return provider_failure(error, error_capacity, detail,
                                            "TriggerTrap::_Remove failed");
                return 0;
            }
            case Kind::get_owner:
                if (!self.services.owner || !capacity || !output)
                    return fail(error, error_capacity, "TriggerTrap::_GetOwner provider is unavailable");
                set_identity(output[0], self.services.owner(self.services.context));
                *returned = 1;
                return 0;
            case Kind::get_damager:
                if (!self.services.damager || !capacity || !output)
                    return fail(error, error_capacity, "TriggerTrap::_GetDamager provider is unavailable");
                set_number(output[0], static_cast<float>(
                    self.services.damager(self.services.context)));
                *returned = 1;
                return 0;
            }
            return 0;
        } catch (...) {
            return fail(error, error_capacity, "TriggerTrap ExternalScript provider threw");
        }
    }

    bool bind(const char* name, NativeBinding::Kind kind) {
        bindings.push_back({this, kind});
        return dh2_script_vm_bind_source_values(vm, name, native, &bindings.back()) == 0;
    }

    bool has_function(const char* name) const {
        dh2_script_value value{};
        return dh2_script_vm_get_global(vm, name, &value) == 0 &&
            value.type == DH2_SCRIPT_FUNCTION;
    }
};

Owner::Owner() = default;

Owner::~Owner() {
    busy_ = true;
    impl_.reset();
}

Status Owner::initialize(Source source, const Services& services, std::string& error,
                         std::size_t memory_limit) {
    error.clear();
    if (busy_) { error = "TriggerTrap ExternalScript is busy"; return Status::busy; }
    if (!valid_source(source) || !services.get_py_oid || !services.get_py_constant ||
        !services.deal_damages || !services.remove) {
        error = "invalid TriggerTrap ExternalScript source or providers";
        return Status::invalid_argument;
    }
    BusyScope scope(busy_);
    auto candidate = std::unique_ptr<Impl>(new (std::nothrow) Impl());
    if (!candidate) { error = "TriggerTrap ExternalScript allocation failed"; return Status::allocation_failed; }
    candidate->vm = dh2_script_vm_create_deferred(memory_limit);
    if (!candidate->vm) { error = "TriggerTrap ExternalScript Lua5.1 VM allocation failed"; return Status::allocation_failed; }
    candidate->services = services;
    try {
        candidate->source_path.assign(source.path);
        candidate->bindings.reserve(8);
    } catch (...) {
        error = "TriggerTrap ExternalScript owner storage allocation failed";
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
    using Kind = Impl::NativeBinding::Kind;
    if (!candidate->bind("Trace", Kind::trace) ||
        !candidate->bind("GetPyOID", Kind::py_oid) ||
        !candidate->bind("GetPyCst", Kind::py_constant) ||
        !candidate->bind("DealDamages", Kind::deal_damages) ||
        !candidate->bind("Remove", Kind::remove) ||
        !candidate->bind("GetOwner", Kind::get_owner) ||
        !candidate->bind("GetDamager", Kind::get_damager)) {
        error = dh2_script_vm_error(candidate->vm);
        if (error.empty()) error = "TriggerTrap ExternalScript native binding failed";
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

namespace {
struct TriggerVerdict {
    bool denied{};
};
int observe_trigger(void* raw, const dh2_script_first_return_v1* first, char*, std::size_t) {
    auto* verdict = static_cast<TriggerVerdict*>(raw);
    // CanTrigger: a non-empty ReturnValues whose first Value is a boolean false.
    if (first && first->count && first->type == DH2_SCRIPT_BOOLEAN && !first->boolean)
        verdict->denied = true;
    return 0;
}
} // namespace

Status Owner::on_trigger(std::uintptr_t victim, bool& allowed, std::string& error) {
    error.clear();
    allowed = true;
    if (busy_) { error = "TriggerTrap ExternalScript is busy"; return Status::busy; }
    if (!impl_ || !impl_->vm) { error = "TriggerTrap ExternalScript is not loaded"; return Status::service_unavailable; }
    BusyScope scope(busy_);
    if (!impl_->has_function("OnTrigger")) return Status::complete;
    dh2_script_value argument{};
    set_identity(argument, victim);
    TriggerVerdict verdict{};
    const int status = dh2_script_vm_call_first_source_v1(impl_->vm, "OnTrigger",
        &argument, 1, observe_trigger, &verdict);
    if (status != 0) {
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "TriggerTrap::CanTrigger OnTrigger failed";
        return Status::callback_failed;
    }
    allowed = !verdict.denied;
    return Status::complete;
}

Status Owner::on_damage_roll(std::uintptr_t victim, std::string& error) {
    error.clear();
    if (busy_) { error = "TriggerTrap ExternalScript is busy"; return Status::busy; }
    if (!impl_ || !impl_->vm) { error = "TriggerTrap ExternalScript is not loaded"; return Status::service_unavailable; }
    BusyScope scope(busy_);
    if (!impl_->has_function("OnDamageRoll")) return Status::complete;
    dh2_script_value argument{};
    set_identity(argument, victim);
    if (dh2_script_vm_call_discard_source(impl_->vm, "OnDamageRoll", &argument, 1) != 0) {
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "TriggerTrap::Update OnDamageRoll failed";
        return Status::callback_failed;
    }
    return Status::complete;
}

Status Owner::on_anim_event(const char* name, std::string& error) {
    error.clear();
    if (busy_) { error = "TriggerTrap ExternalScript is busy"; return Status::busy; }
    if (!impl_ || !impl_->vm || !name) { error = "TriggerTrap ExternalScript is not loaded"; return Status::service_unavailable; }
    BusyScope scope(busy_);
    if (!impl_->has_function("OnAnimEvent")) return Status::complete;
    dh2_script_value argument{};
    argument.type = DH2_SCRIPT_STRING;
    argument.text = name;
    argument.text_bytes = std::strlen(name);
    if (dh2_script_vm_call_discard_source(impl_->vm, "OnAnimEvent", &argument, 1) != 0) {
        error = dh2_script_vm_error(impl_->vm);
        if (error.empty()) error = "TriggerTrap::__EventCallback OnAnimEvent failed";
        return Status::callback_failed;
    }
    return Status::complete;
}

Status Owner::reset(std::string& error) noexcept {
    error.clear();
    if (busy_) { error = "TriggerTrap ExternalScript is busy"; return Status::busy; }
    busy_ = true;
    impl_.reset();
    busy_ = false;
    return Status::complete;
}

bool Owner::ready() const noexcept { return impl_ && impl_->vm; }
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

} // namespace dh2::timer_trap_external_script_v1
