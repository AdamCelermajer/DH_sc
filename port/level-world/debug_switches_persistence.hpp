#pragma once
#include "debug_switches_runtime.hpp"

namespace dh2::debug_switches_persistence {
using Map=std::map<std::string,std::uint8_t>;
// These are live owned maps, not copied serialization snapshots. A Runtime's
// const map accessors can provide both views. Map controls/identity stay fixed.
struct Owner {std::uintptr_t identity;const Map* switches;const Map* modules;};
struct Stream {std::uintptr_t identity;};
enum class Operation : std::uint8_t {open_write,write_word,write_string,write_byte,close};
struct Request {
    Operation operation;std::uintptr_t owner,filesystem,stream;
    std::uint32_t word;const char* bytes;std::size_t size;
};
struct Services {
    void* context;
    // Zero means success. open_write returns a retained real writable stream,
    // or zero for an ordinary open miss; mode is source word1. write_string
    // receives captured live bytes/length, and encodes length plus bytes without
    // a NUL terminator. Writes must have real stream effects; no successful no-op.
    std::int32_t (*invoke)(void*,const Request*,Stream* reply);
};
using Status=debug_switches::Status;
struct Result {
    std::uint32_t calls,module_count,switch_count,modules_written,switches_written;
    Operation last_operation;
};
Status write(const Owner&,Stream,debug_switches::Globals&,
             const debug_switches::Services& debug_services,const Services&,Result&);
Status save(const Owner&,debug_switches::Globals&,
            const debug_switches::Services& debug_services,const Services&,Result&);

// Complete original _saveSwitches520B and save136B callers. Standard owned map
// iteration/string storage replace original STL ABI; native stream-write bodies
// remain explicit dependencies. The real Runtime performs nested load/query.
// Capture counts at their source points, then traverse live ordered nodes. Each
// switch traces BEFORE reading its name/value; its byte is reread AFTER string
// write. The tracing Runtime is captured once before the nonempty switch loop.
// Insertion can make emitted rows exceed the previously written count. Do not
// repair counts, snapshot rows or change traversal into a fixed-count loop.
// One owning thread retains all controls, maps, nodes, captured Runtime owners,
// stream/filesystem/backend and borrowed string bytes through return. Providers
// may insert keys/update bytes/change Globals selections, but cannot erase or
// destroy retained maps/nodes, mutate keys, clear the loaded guard, or arbitrarily
// reenter the same writer. The original save->writer->load/query recursion is
// allowed; nested independent writers/outputs are allowed. Result is separate
// from all controls/backing. Service records are captured per public entry.
// Errors retain previous writes/map effects and do not add close/rollback/repair;
// the backend owns stream cleanup after port failure. C++ allocator/unwinding
// is not a claim of original STL exception behavior.
} // namespace dh2::debug_switches_persistence
