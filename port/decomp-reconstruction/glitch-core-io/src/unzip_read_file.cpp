// glitch::io::CUnzipReadFile. See include/glitch-core-io/io/unzip_read_file.hpp.
#include "glitch-core-io/io/unzip_read_file.hpp"

namespace glitch::io {

// 0x5779cc _ZN6glitch2io14CUnzipReadFileC1ERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEEPKc
CUnzipReadFile::CUnzipReadFile(const std::string& folderPath, const char* entryName)
    : CReadFile(folderPath.c_str(), false), name(entryName ? entryName : "") {}

// 0x576db8 _ZNK6glitch2io14CUnzipReadFile11getFileNameEv
const std::string& CUnzipReadFile::getFileName() const { return name; }

}  // namespace glitch::io
