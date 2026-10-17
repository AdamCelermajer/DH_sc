#pragma once

#include "../../../../../../port/level-world/debug_switches_persistence.hpp"
#include <cstdio>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace dh2::native::debug_files {

struct Counters {
    std::uint64_t read_attempts=0,read_opens=0,read_misses=0,read_closes=0;
    std::uint64_t save_attempts=0,save_completions=0;
    std::uint64_t write_attempts=0,write_opens=0,write_misses=0,write_closes=0;
    std::uint64_t word_writes=0,string_writes=0,byte_writes=0,bytes_written=0;
    std::uint64_t io_errors=0,rejected_requests=0;
};

// Production filesystem dependency adapter; original Runtime/save caller bodies
// remain in level-world. All identities and resources are owned by this object.
class Backend {
public:
    Backend();
    ~Backend();
    Backend(const Backend&)=delete;
    Backend& operator=(const Backend&)=delete;
    Backend(Backend&&)=delete;
    Backend& operator=(Backend&&)=delete;

    // Existing absolute app-private directory (Android Context.getFilesDir).
    // Optional exact bundled bytes install the named file by exclusive creation
    // only when absent. This asset installation is outside original load/save.
    // Existing files are never overwritten by installation/reinitialization.
    // Same-directory reinitialization is permitted; changing directory rejects.
    bool initialize(const std::filesystem::path& directory,
                    const std::uint8_t* seed,std::size_t seed_size,std::string& error);
    debug_switches::Runtime& runtime(){return runtime_;}
    debug_switches::Globals& globals(){return globals_;}
    const debug_switches::Services& services()const{return services_;}
    const Counters& counters()const{return counters_;}
    const debug_switches_persistence::Result& last_save()const{return last_save_;}
    const std::filesystem::path& filename()const{return filename_;}
    std::size_t retained_reads()const{return reads_.size();}
    std::size_t retained_writes()const{return writes_.size();}
    std::size_t active_reads()const;
    std::size_t active_writes()const;

private:
    struct ReadResource {std::FILE* file=nullptr;std::vector<std::uint8_t> bytes;};
    struct WriteResource {std::FILE* file=nullptr;};
    static std::int32_t debug_call(void*,const debug_switches::Request*,debug_switches::File*);
    static std::int32_t write_call(void*,const debug_switches_persistence::Request*,debug_switches_persistence::Stream*);
    std::int32_t debug_operation(const debug_switches::Request&,debug_switches::File&);
    std::int32_t write_operation(const debug_switches_persistence::Request&,debug_switches_persistence::Stream&);
    std::int32_t reject();
    std::int32_t io_error();

    debug_switches::Runtime runtime_;
    debug_switches::FileSystem files_;
    debug_switches::Engine engine_;
    debug_switches::Application application_;
    debug_switches::Globals globals_;
    debug_switches::Services services_;
    debug_switches_persistence::Services writer_;
    Counters counters_{};
    debug_switches_persistence::Result last_save_{};
    std::filesystem::path directory_,filename_;
    bool initialized_=false;
    std::map<std::uintptr_t,std::unique_ptr<ReadResource>> reads_;
    std::map<std::uintptr_t,std::unique_ptr<WriteResource>> writes_;
};

// One owning thread; no destruction/reinitialization during a source call. The
// owned globals/projection graph stays selected; only source Runtime recursion
// (save -> writer -> load/query) may reenter these services. No fake no-op saves.
// Every opened read retains its original byte image across nested file saves.
// Closed controls/bytes and leaked-on-error resources remain until destruction.
// Errors retain loaded/map/file effects without extra source close/rollback.
// Destruction closes residual OS handles as backend lifetime cleanup, without
// incrementing source-close counters. Runtime's partial decoding boundaries stay
// explicit; this backend does not broaden them or claim original OS/allocator
// bodies. Open ENOENT is an ordinary miss; other IO errors are port failures.
} // namespace dh2::native::debug_files
