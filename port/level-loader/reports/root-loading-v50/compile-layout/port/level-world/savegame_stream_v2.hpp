#pragma once
#include "../game-data/data.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::level {
// Source StreamBuffer's independent read/write cursors and 0x800-byte blocks.
// Contiguous native storage is an ABI adapter; positions/bytes remain source.
class SavegameStreamV2 {
 std::vector<std::uint8_t> bytes_;std::uint64_t read_{},write_{};
public:
 SavegameStreamV2()=default;
 explicit SavegameStreamV2(data::Bytes b){if(b.size)bytes_.assign(b.data,b.data+b.size);}
 const std::vector<std::uint8_t>& bytes()const noexcept{return bytes_;}
 std::uint64_t tell()const noexcept{return read_;}
 std::uint64_t tell_write()const noexcept{return write_;}
 std::uint64_t size()const noexcept{return bytes_.size();}
 void seek(std::uint64_t)noexcept;void seek_write(std::uint64_t)noexcept;
 bool read(void*,std::size_t,std::string&);bool write(data::Bytes,std::string&);
 bool read_u32(std::uint32_t&,std::string&);bool read_u64(std::uint64_t&,std::string&);
 bool write_u32(std::uint32_t,std::string&);bool write_u64(std::uint64_t,std::string&);
 bool read_string(std::string&,std::string&);bool write_string(const std::string&,std::string&);
};
}
