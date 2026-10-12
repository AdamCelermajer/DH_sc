#include "glitch-core-io/io/zip_writer.hpp"

namespace glitch::io {

// 0x6b4f34 _ZN6glitch2io10CZipWriterC2EPNS0_10IWriteFileE (C1 at 0x6b4f90 is the same body)
// The write file is not reference counted by the writer in the original.
CZipWriter::CZipWriter(IWriteFile* file) : writeFile(file) {}

// 0x6b5238 _ZN6glitch2io10CZipWriterD1Ev. PARTIAL: the original calls close() first, which
// writes the central directory and is not reconstructed here.
CZipWriter::~CZipWriter() = default;

}  // namespace glitch::io
