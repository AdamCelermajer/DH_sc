// glitch::io::CFileSystem: the file system facade. Only the members used by the reconstructed
// functions are declared. Not modelled yet: the constructor's archive containers (+0x08 ..),
// the archive functions, the obfuscation map and the XML reader factories (they need the
// IrrXML reader interface, which the port does not have).
#pragma once

#include <string>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/file_list.hpp"
#include "glitch-core-io/io/interfaces.hpp"
#include "glitch-core-io/io/read_file.hpp"
#include "glitch-core-io/io/write_file.hpp"
#include "glitch-core-io/io/xml_writer.hpp"

namespace glitch::io {

class CFileSystem : public IFileSystem {
public:
    CFileSystem() = default;  // the original constructor (0x56c058, 0x56c0a8) is not reconstructed

    // 0x56c214: the working directory as last set by changeWorkingDirectoryTo.
    static const char* getWorkingDirectory();
    // 0x56c528: changes the process working directory; true when the change succeeded, and then
    // the new directory is kept for getWorkingDirectory.
    bool changeWorkingDirectoryTo(const char* directory);

    // 0x56c4a4 (const): a new, empty directory listing.
    CFileList* createFileList() const;
    // 0x56c570: opens a write file (see createWriteFile).
    IWriteFile* createAndWriteFile(const char* name, bool append);
    // 0x56c57c: a memory read file; null when the data pointer is null.
    IReadFile* createMemoryReadFile(void* data, int length, const char* name, bool takeOwnership);
    // 0x56c414: an XML writer over the given write file.
    IXMLWriter* createXMLWriter(IWriteFile* file);
    // 0x56c278: an XML writer over a write file opened by name.
    IXMLWriter* createXMLWriter(const char* name);
    // 0x56c3ec: an attribute container bound to the driver.
    CAttributes* createEmptyAttributes(glitch::video::IVideoDriver* driver);
    // 0x56c348: returns a copy of the name (the original does no path resolution here).
    std::string getAbsolutePath(const std::string& path) const;

    // The working directory is a process-wide static in the original.
    static std::string WorkingDirectory;  // _ZN6glitch2io11CFileSystem16WorkingDirectoryE
};

// 0x56c2d8 _ZN6glitch2io16createFileSystemEv
IFileSystem* createFileSystem();

}  // namespace glitch::io
