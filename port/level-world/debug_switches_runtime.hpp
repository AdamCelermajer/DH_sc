#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace dh2::debug_switches {
class Runtime;
struct FileSystem {std::uintptr_t identity;};
struct Engine {std::uintptr_t identity;const FileSystem* files;};
struct Application {std::uintptr_t identity;const Engine* engine;};
// loaded is the original shared one-byte DebugSwitches::s_loaded guard, not a
// per-Runtime cache. Retired selected owners remain live through synchronous use.
struct Globals {std::uint8_t loaded;Runtime* singleton;const Application* application;};
struct File {std::uintptr_t identity;const std::uint8_t* bytes;std::size_t size;};
enum class Operation : std::uint8_t {open_read,close,save};
struct Request {Operation operation;Runtime* owner;std::uintptr_t filesystem;const File* file;const char* path;};
struct Services {
    void* context;
    // Zero success; open_read returns a retained live stream/byte view or a
    // zero identity for an ordinary missing file. close uses the captured FS.
    // save implements the real original save dependency for the supplied owner,
    // including persistent effects. Successful no-ops are not substitutes.
    std::int32_t (*invoke)(void*,const Request*,File* reply);
};
enum class Status : std::int32_t {complete,invalid_argument,service_unavailable,service_failed,unsupported_configuration};
// Source owned maps, rather than borrowed fabricated query facts. The first
// map is switches and the second is modules; original ctor initializes both.
class Runtime {
public:
    explicit Runtime(std::uintptr_t identity):identity_(identity){}
    std::uintptr_t identity()const{return identity_;}
    const std::map<std::string,std::uint8_t>& switches()const{return switches_;}
    const std::map<std::string,std::uint8_t>& modules()const{return modules_;}
    std::size_t last_file_position()const{return last_file_position_;}
    Status load(Globals&,const Services&);
    Status get_switch(const std::string& key,Globals&,const Services&,std::uint8_t& value);
    Status set_switch(const std::string& key,std::uint8_t value,Globals&,const Services&);
private:
    Status load_impl(Globals&,const Services&,unsigned);
    Status get_impl(const std::string&,Globals&,const Services&,std::uint8_t&,unsigned);
    Status set_impl(const std::string&,std::uint8_t,Globals&,const Services&,unsigned);
    Status trace(const char*,Globals&,const Services&,unsigned);
    Status read_configuration(const File&,Globals&,const Services&,unsigned);
    std::uintptr_t identity_;
    std::map<std::string,std::uint8_t> switches_,modules_;
    std::size_t last_file_position_=0;
};

// Complete load512B/GetSwitch196B/SetSwitch236B source callers. File decoding
// covers the original WSBD version>=0x10000 switch loop and version>=0x20000
// zero/nonpositive-module-count prefix. Positive module lists, old/text/bad magic
// formats remain explicit unsupported boundaries, not successful empty loads.
// Data-read/save/string/map allocator dependency bodies are not whole-body claims.
// The shared loaded guard is set BEFORE file access, and stays set after failure.
// Existing keys are read freshly; missing GetSwitch inserts false BEFORE trace;
// missing SetSwitch traces BEFORE insertion, then saves only when value changes.
// Borrowed key content may change via providers and is reread where the source
// does. Internal source recursion is permitted; its direct caller chain is
// bounded. The real save provider may perform the original writer's load/query
// tracing through these retained owners. Other external provider reentry into
// these controls is forbidden. Independent runtimes may nest.
// One owning thread retains Globals/application/engine/filesystem/File backing,
// Runtime and service context until return; identities remain stable. Services
// are captured per public entry. Providers may change Globals selections and
// borrowed file bytes/key contents. Returned File controls are copied, bytes
// remain live/fresh. No provider-driven map erase/destruction of active owners.
// Errors/throws retain prior guard/map/provider effects, without extra close,
// save, rollback or repair. The provider must own leaked-on-error file resources.
} // namespace dh2::debug_switches
