#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh2::assets {
struct ZipBackingV1 {
    std::shared_ptr<void> owner;
    std::uint64_t bytes{};
    // Exact synchronous positional read. Owner keeps the APK/file descriptor alive.
    std::function<bool(std::uint64_t,void*,std::size_t,std::string&)> read;
};
struct ZipEntryV1 {
    std::string uri;
    std::uint32_t compressed{},bytes{},crc{};
};
// Native resource filesystem over the complete supplied cache ZIP. No cache
// extraction, executable emulation or external storage installation is involved.
// Immutable directory/backing permits independent concurrent reads.
class ZipAssetPackV1 {
    struct Impl;
    std::shared_ptr<const Impl> impl_;
public:
    bool mount(ZipBackingV1,const std::string& prefix,std::string&);
    bool read(const std::string& uri,bool& found,std::vector<std::uint8_t>&,std::string&) const;
    bool read_admitted(const std::string& uri,bool& found,std::vector<std::uint8_t>&,
        const std::function<bool(std::uint32_t,std::string&)>& before_payload,std::string&) const;
    bool mounted() const noexcept {return bool(impl_);}
    std::vector<ZipEntryV1> entries() const;
    // Lookup the SAME mounted directory without acquiring/decompressing the
    // payload. Missing is successful found=false; canonical uri is metadata.
    bool entry(const std::string& uri,bool& found,ZipEntryV1&,std::string&) const;
    static bool key(const std::string&,std::string&,std::string&);
};
}
