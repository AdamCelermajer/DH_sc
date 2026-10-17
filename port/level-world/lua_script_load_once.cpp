#include "lua_script_load_once.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <new>
#include <utility>

namespace dh2::lua_script_load_once {
namespace {
constexpr std::size_t kMaximumPathBytes = 4096;
constexpr std::size_t kMaximumSourceBytes = 8u * 1024u * 1024u;
constexpr std::int32_t kUnknownVmStatus = std::numeric_limits<std::int32_t>::min();

struct AddressRange {
    std::uintptr_t first;
    std::uintptr_t end;
};

template <typename T>
bool object_range(const T* pointer, AddressRange& output) noexcept {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || first % alignof(T) != 0 ||
        first > std::numeric_limits<std::uintptr_t>::max() - sizeof(T))
        return false;
    output = {first, first + sizeof(T)};
    return true;
}

bool byte_range(const void* pointer, std::size_t bytes, AddressRange& output) noexcept {
    const auto first = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || bytes == 0 ||
        first > std::numeric_limits<std::uintptr_t>::max() - bytes)
        return false;
    output = {first, first + bytes};
    return true;
}

bool overlaps(AddressRange left, AddressRange right) noexcept {
    return left.first < right.end && right.first < left.end;
}

bool path_length(const char* path, std::size_t& size) noexcept {
    if (!path) return false;
    for (size = 0; size < kMaximumPathBytes; ++size)
        if (path[size] == '\0') return size != 0;
    return false;
}

bool path_range(const char* path, std::size_t size, AddressRange& output) noexcept {
    return byte_range(path, size + 1, output);
}

bool contains_path_unchecked(const std::vector<std::string>& paths,
                             const char* path, std::size_t size) noexcept {
    return std::any_of(paths.begin(), paths.end(),
        [path, size](const std::string& entry) {
            return entry.size() == size &&
                   std::memcmp(entry.data(), path, size) == 0;
        });
}

void clear_result(Result& result) noexcept {
    result.vm_status = 0;
    result.source_success = 0;
    result.cache_hit = 0;
    result.load_called = 0;
}

struct BusyScope {
    bool& busy;
    const Result*& active_result;
    BusyScope(bool& value, const Result*& active, const Result* result) noexcept
        : busy(value), active_result(active) {
        busy = true;
        active_result = result;
    }
    ~BusyScope() {
        active_result = nullptr;
        busy = false;
    }
};

}  // namespace

Status reset(State* state, std::uintptr_t vm_identity) {
    AddressRange state_range{};
    if (!object_range(state, state_range) || !vm_identity)
        return Status::invalid_argument;
    if (state->busy_) return Status::busy;
    state->loaded_paths_.clear();
    state->vm_identity_ = vm_identity;
    return Status::complete;
}

Status load_once(State* state, const Source* source, const Services* services,
                 Result* result) {
    AddressRange controls[4]{};
    if (!object_range(state, controls[0]) || !object_range(source, controls[1]) ||
        !object_range(result, controls[3]) ||
        (services && !object_range(services, controls[2])))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 4; ++i) {
        if (i == 2 && !services) continue;
        for (unsigned j = 0; j < i; ++j) {
            if (j == 2 && !services) continue;
            if (overlaps(controls[i], controls[j])) return Status::invalid_argument;
        }
    }

    if (state->busy_) {
        // A recursive callback must not overwrite the outer call's Result.
        if (state->active_result_ != result) clear_result(*result);
        return Status::busy;
    }
    clear_result(*result);

    if (!state->vm_identity_ || source->vm_identity == 0)
        return Status::invalid_argument;
    if (source->vm_identity != state->vm_identity_)
        return Status::vm_mismatch;

    std::size_t name_size = 0;
    if (!path_length(source->resolved_path, name_size))
        return Status::invalid_argument;
    AddressRange name_range{};
    if (!path_range(source->resolved_path, name_size, name_range))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 4; ++i) {
        if (i == 2 && !services) continue;
        if (overlaps(name_range, controls[i])) return Status::invalid_argument;
    }

    if (contains_path_unchecked(state->loaded_paths_, source->resolved_path, name_size)) {
        result->source_success = 1;
        result->cache_hit = 1;
        return Status::complete;
    }

    if (!services || !services->load) return Status::service_unavailable;
    if (!source->bytes || source->byte_count == 0 ||
        source->byte_count > kMaximumSourceBytes)
        return Status::invalid_source;
    AddressRange bytes_range{};
    if (!byte_range(source->bytes, source->byte_count, bytes_range))
        return Status::invalid_source;
    if (overlaps(name_range, bytes_range)) return Status::invalid_source;
    for (unsigned i = 0; i < 4; ++i) {
        if (i == 2 && !services) continue;
        if (overlaps(bytes_range, controls[i])) return Status::invalid_source;
    }

    // Allocate the immutable set key and vector slot before calling the VM.
    // If those allocations fail, avoid executing the script without being
    // able to remember that it ran.
    std::string key;
    try {
        key.assign(source->resolved_path, name_size);
        if (state->loaded_paths_.size() == state->loaded_paths_.max_size())
            return Status::allocation_failed;
        state->loaded_paths_.reserve(state->loaded_paths_.size() + 1);
    } catch (const std::bad_alloc&) {
        return Status::allocation_failed;
    } catch (...) {
        return Status::allocation_failed;
    }

    const auto vm_identity = source->vm_identity;
    const auto* const bytes = source->bytes;
    const auto byte_count = source->byte_count;
    const auto loader = services->load;
    void* const context = services->context;
    BusyScope busy(state->busy_, state->active_result_, result);
    result->load_called = 1;
    std::int32_t vm_status = kUnknownVmStatus;
    try {
        vm_status = loader(context, vm_identity, bytes, byte_count, key.c_str());
    } catch (...) {
        result->vm_status = kUnknownVmStatus;
        return Status::service_failed;
    }
    result->vm_status = vm_status;
    if (vm_status != 0) return Status::load_failed;

    // Capacity was reserved before executing Lua; moving the prepared key is
    // non-allocating for std::string's standard allocator.
    state->loaded_paths_.push_back(std::move(key));
    result->source_success = 1;
    return Status::complete;
}

std::size_t loaded_path_count(const State* state) noexcept {
    return state ? state->loaded_paths_.size() : 0;
}

bool contains_path(const State* state, const char* resolved_path) noexcept {
    if (!state) return false;
    std::size_t size = 0;
    return path_length(resolved_path, size) &&
           contains_path_unchecked(state->loaded_paths_, resolved_path, size);
}

}  // namespace dh2::lua_script_load_once
