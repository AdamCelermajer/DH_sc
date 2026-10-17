#include "native_debug_files.hpp"
#include "../../../../../../port/persistence/binary.h"
#include <cerrno>
#include <cstring>
#include <limits>
#include <system_error>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace dh2::native::debug_files {
namespace ds=debug_switches;
namespace dp=debug_switches_persistence;
namespace {
constexpr const char* source_filename="DebugSwitches.savegame";
constexpr std::size_t maximum_file=16u*1024u*1024u;
std::FILE* open_file(const std::filesystem::path& name,const char* mode){
#ifdef _WIN32
    return _wfopen(name.c_str(),std::filesystem::path(mode).c_str());
#else
    return std::fopen(name.c_str(),mode);
#endif
}
int exclusive_file(const std::filesystem::path& name){
#ifdef _WIN32
    return _wopen(name.c_str(),_O_WRONLY|_O_CREAT|_O_EXCL|_O_BINARY,_S_IREAD|_S_IWRITE);
#else
    return ::open(name.c_str(),O_WRONLY|O_CREAT|O_EXCL,0600);
#endif
}
int close_descriptor(int descriptor){
#ifdef _WIN32
    return _close(descriptor);
#else
    return ::close(descriptor);
#endif
}
std::ptrdiff_t write_descriptor(int descriptor,const std::uint8_t* bytes,std::size_t size){
#ifdef _WIN32
    return _write(descriptor,bytes,static_cast<unsigned>(size));
#else
    return ::write(descriptor,bytes,size);
#endif
}
}

Backend::Backend():runtime_(reinterpret_cast<std::uintptr_t>(&runtime_)),
    files_{reinterpret_cast<std::uintptr_t>(&files_)},
    engine_{reinterpret_cast<std::uintptr_t>(&engine_),&files_},
    application_{reinterpret_cast<std::uintptr_t>(&application_),&engine_},
    globals_{0,&runtime_,&application_},services_{this,debug_call},writer_{this,write_call}{}

Backend::~Backend(){
    for(auto& resource:reads_)if(resource.second->file)std::fclose(resource.second->file);
    for(auto& resource:writes_)if(resource.second->file)std::fclose(resource.second->file);
}

bool Backend::initialize(const std::filesystem::path& directory,const std::uint8_t* seed,
                         std::size_t size,std::string& error){
    error.clear();
    if(!directory.is_absolute()||(!seed&&size)||size>maximum_file){error="Invalid private directory or seed";return false;}
    std::error_code code;const auto normalized=directory.lexically_normal();
    if(!std::filesystem::is_directory(normalized,code)||code){error="Private directory is unavailable";return false;}
    if(initialized_&&directory_!=normalized){error="Debug backend directory is already selected";return false;}
    const auto name=normalized/source_filename;
    if(seed){
        const auto descriptor=exclusive_file(name);
        if(descriptor<0){if(errno!=EEXIST){error="Debug asset installation open failed";return false;}}
        else{
            std::size_t position=0;bool failed=false;
            while(position<size){
                const auto count=write_descriptor(descriptor,seed+position,size-position);
                if(count<0&&errno==EINTR)continue;
                if(count<=0){failed=true;break;}
                position+=static_cast<std::size_t>(count);
            }
            const auto closed=close_descriptor(descriptor);
            if(failed||closed){error="Debug asset installation write failed";return false;}
        }
    }
    directory_=normalized;filename_=name;initialized_=true;return true;
}

std::size_t Backend::active_reads()const{std::size_t count=0;for(const auto& resource:reads_)count+=resource.second->file!=nullptr;return count;}
std::size_t Backend::active_writes()const{std::size_t count=0;for(const auto& resource:writes_)count+=resource.second->file!=nullptr;return count;}
std::int32_t Backend::reject(){++counters_.rejected_requests;return 1;}
std::int32_t Backend::io_error(){++counters_.io_errors;return 1;}

std::int32_t Backend::debug_call(void* context,const ds::Request* request,ds::File* reply){
    if(!context||!request||!reply)return 1;
    auto& owner=*static_cast<Backend*>(context);*reply={};
    try{return owner.debug_operation(*request,*reply);}catch(...){return owner.io_error();}
}
std::int32_t Backend::write_call(void* context,const dp::Request* request,dp::Stream* reply){
    if(!context||!request||!reply)return 1;
    auto& owner=*static_cast<Backend*>(context);*reply={};
    try{return owner.write_operation(*request,*reply);}catch(...){return owner.io_error();}
}

std::int32_t Backend::debug_operation(const ds::Request& request,ds::File& reply){
    if(!initialized_||request.owner!=&runtime_)return reject();
    switch(request.operation){
    case ds::Operation::open_read:{
        if(request.filesystem!=files_.identity||!request.path||std::strcmp(request.path,source_filename))return reject();
        ++counters_.read_attempts;
        auto resource=std::make_unique<ReadResource>();const auto id=reinterpret_cast<std::uintptr_t>(resource.get());
        // Register before OS acquisition so backing remains owned on every later
        // allocation/read/parse failure. No source close is invented on errors.
        auto& retained=*reads_.emplace(id,std::move(resource)).first->second;
        retained.file=open_file(filename_,"rb");
        if(!retained.file){const auto failure=errno;reads_.erase(id);if(failure==ENOENT){++counters_.read_misses;return 0;}return io_error();}
        ++counters_.read_opens;
        if(std::fseek(retained.file,0,SEEK_END))return io_error();
        const auto size=std::ftell(retained.file);
        if(size<0||static_cast<unsigned long>(size)>maximum_file||std::fseek(retained.file,0,SEEK_SET))return io_error();
        retained.bytes.resize(static_cast<std::size_t>(size));
        if(size&&std::fread(retained.bytes.data(),1,retained.bytes.size(),retained.file)!=retained.bytes.size())return io_error();
        if(std::ferror(retained.file))return io_error();
        reply={id,retained.bytes.data(),retained.bytes.size()};return 0;
    }
    case ds::Operation::close:{
        if(request.filesystem!=files_.identity||!request.file)return reject();
        const auto found=reads_.find(request.file->identity);
        if(found==reads_.end()||!found->second->file||request.file->bytes!=found->second->bytes.data()||request.file->size!=found->second->bytes.size())return reject();
        auto* file=found->second->file;found->second->file=nullptr;
        if(std::fclose(file))return io_error();
        ++counters_.read_closes;return 0;
    }
    case ds::Operation::save:{
        ++counters_.save_attempts;
        const dp::Owner owner{runtime_.identity(),&runtime_.switches(),&runtime_.modules()};
        const auto status=dp::save(owner,globals_,services_,writer_,last_save_);
        if(status!=dp::Status::complete)return 1;
        ++counters_.save_completions;return 0;
    }
    }
    return reject();
}

std::int32_t Backend::write_operation(const dp::Request& request,dp::Stream& reply){
    if(!initialized_||request.owner!=runtime_.identity())return reject();
    if(request.operation==dp::Operation::open_write){
        if(request.filesystem!=files_.identity||request.word!=1||!request.bytes||request.size||std::strcmp(request.bytes,source_filename))return reject();
        ++counters_.write_attempts;
        auto resource=std::make_unique<WriteResource>();const auto id=reinterpret_cast<std::uintptr_t>(resource.get());
        auto& retained=*writes_.emplace(id,std::move(resource)).first->second;
        retained.file=open_file(filename_,"wb");
        if(!retained.file){const auto failure=errno;writes_.erase(id);if(failure==ENOENT){++counters_.write_misses;return 0;}return io_error();}
        ++counters_.write_opens;reply={id};return 0;
    }
    const auto found=writes_.find(request.stream);
    if(found==writes_.end()||!found->second->file)return reject();
    auto* const file=found->second->file;
    if(request.operation==dp::Operation::close){
        if(request.filesystem!=files_.identity)return reject();
        found->second->file=nullptr;if(std::fclose(file))return io_error();
        ++counters_.write_closes;return 0;
    }
    std::uint8_t bytes[4];const void* data=nullptr;std::size_t size=0;
    switch(request.operation){
    case dp::Operation::write_word:dh2_save_write32(bytes,request.word);data=bytes;size=4;++counters_.word_writes;break;
    case dp::Operation::write_byte:bytes[0]=static_cast<std::uint8_t>(request.word);data=bytes;size=1;++counters_.byte_writes;break;
    case dp::Operation::write_string:
        if(request.size>std::numeric_limits<std::uint32_t>::max()||(!request.bytes&&request.size))return reject();
        dh2_save_write32(bytes,static_cast<std::uint32_t>(request.size));++counters_.string_writes;
        {const auto written=std::fwrite(bytes,1,4,file);counters_.bytes_written+=written;if(written!=4)return io_error();}
        data=request.bytes;size=request.size;break;
    default:return reject();
    }
    if(size){const auto written=std::fwrite(data,1,size,file);counters_.bytes_written+=written;if(written!=size)return io_error();}
    // Flush each completed typed service write so retained partial effects are
    // real observable file contents, including when the next service fails.
    if(std::fflush(file)||std::ferror(file))return io_error();
    return 0;
}
} // namespace dh2::native::debug_files
