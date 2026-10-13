#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::lua_script_load_once {

struct Source {
    std::uintptr_t vm_identity;
    const char* resolved_path;
    const void* bytes;
    std::size_t byte_count;
};

struct Services {
    void* context;
    // Must synchronously execute the supplied retained bytes in the VM named
    // by vm_identity, using resolved_path as the chunk name. It returns the
    // underlying VM load status unchanged (zero succeeds; nonzero fails).
    std::int32_t (*load)(void* context, std::uintptr_t vm_identity,
                         const void* bytes, std::size_t byte_count,
                         const char* resolved_path);
};

struct Result {
    std::int32_t vm_status;
    std::uint32_t source_success;
    std::uint32_t cache_hit;
    std::uint32_t load_called;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    invalid_source = 2,
    vm_mismatch = 3,
    busy = 4,
    service_unavailable = 5,
    load_failed = 6,
    service_failed = 7,
    allocation_failed = 8,
};

struct State {
    State() = default;
    State(const State&) = delete;
    State& operator=(const State&) = delete;
    State(State&&) = delete;
    State& operator=(State&&) = delete;

private:
    friend Status reset(State*, std::uintptr_t);
    friend Status load_once(State*, const Source*, const Services*, Result*);
    friend std::size_t loaded_path_count(const State*) noexcept;
    friend bool contains_path(const State*, const char*) noexcept;
    friend Status mark_loaded(State*, const char*);

    std::uintptr_t vm_identity_ = 0;
    std::vector<std::string> loaded_paths_;
    bool busy_ = false;
    const Result* active_result_ = nullptr;
};

// Bind/reset this path set to a newly created VM before loading files. Reset
// must happen after replacing/destroying the old VM and before using the new
// one. It cannot run from inside a synchronous loader callback.
Status reset(State*, std::uintptr_t vm_identity);

// Mirrors LuaScript's loaded-path shortcut only: an exact path hit reports a
// source-success result without touching the supplied bytes or VM. A miss
// invokes the mandatory real VM loader and records the exact path only after
// a zero VM status. The byte source and callback context are borrowed only
// until the synchronous call returns; failures preserve prior Lua side effects
// and the VM's error text, and do not mark that path loaded.
Status load_once(State*, const Source*, const Services*, Result*);

std::size_t loaded_path_count(const State*) noexcept;
bool contains_path(const State*, const char* resolved_path) noexcept;

// Records a path whose chunk already ran outside load_once: the AIS main script
// and a nested LuaScript::Load (Include) executed from inside another chunk.
// LuaManager::AddFile (0x37b23c) inserts the path into the same per-script set
// after a successful loadFile, so a later Include of that name is a hit.
// Idempotent. It never touches the VM and may run from a loader callback.
Status mark_loaded(State*, const char* resolved_path);

}  // namespace dh2::lua_script_load_once
