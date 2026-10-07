#pragma once
#include "../game-data/data.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
namespace dh2::level {
// Source StreamBuffer's independent read/write cursors and 0x800-byte blocks.
// Contiguous native storage is an ABI adapter; positions/bytes remain source.
class SavegameStreamV2 {
 std::vector<std::uint8_t> bytes_;std::uint64_t read_{},write_{};
public:
 SavegameStreamV2()=default;
 // Adopt the native loader's actual Grow storage without another payload copy.
 explicit SavegameStreamV2(std::vector<std::uint8_t>&& bytes):bytes_(std::move(bytes)){}
 std::uint8_t* source_copy_destination_v65()noexcept{return bytes_.data();}
 explicit SavegameStreamV2(data::Bytes b){if(b.size)bytes_.assign(b.data,b.data+b.size);}
 const std::vector<std::uint8_t>& bytes()const noexcept{return bytes_;}
 std::uint64_t tell()const noexcept{return read_;}
 std::uint64_t tell_write()const noexcept{return write_;}
 std::uint64_t size()const noexcept{return bytes_.size();}
 // StreamBuffer.clear316a48: Buffer.clear first releases storage, then
 // zeroes both independent cursors. Used by the SAME PM embedded6e0.
 void source_clear_v70()noexcept{std::vector<std::uint8_t>().swap(bytes_);read_=write_=0;}
 void seek(std::uint64_t)noexcept;void seek_write(std::uint64_t)noexcept;
 bool read(void*,std::size_t,std::string&);bool write(data::Bytes,std::string&);
 bool read_u32(std::uint32_t&,std::string&);bool read_u64(std::uint64_t&,std::string&);
 bool write_u32(std::uint32_t,std::string&);bool write_u64(std::uint64_t,std::string&);
 bool read_string(std::string&,std::string&);bool write_string(const std::string&,std::string&);
};
}
