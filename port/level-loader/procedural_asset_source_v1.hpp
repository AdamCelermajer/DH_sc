#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::loader {

// Byte source used by the procedural preparation stages (rule file, MGX list,
// MGX blocks, MVX modules). Replaces the zip-only dependency so the same stages
// run over the Windows loose-file asset catalog and over a cache archive.
//
// read(): found=false is a missing resource (not an error); error is set only on
// a read failure. Keys follow ZipAssetPackV1::key: '\\' becomes '/', leading
// "./" is removed, names with ".." or absolute roots are rejected, and the key is
// lower-case. Identity only; reads use the authored URI.
class ProceduralAssetSourceV1 {
public:
    virtual ~ProceduralAssetSourceV1() = default;
    virtual bool read(const std::string& uri, bool& found, std::vector<std::uint8_t>& bytes,
                      std::string& error) const = 0;
    static bool key(const std::string& input, std::string& out, std::string& error);
};

} // namespace dh2::loader
