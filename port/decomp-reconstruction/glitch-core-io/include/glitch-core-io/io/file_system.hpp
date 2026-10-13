// glitch::io::CFileSystem: the file system facade. The object is 0x2c bytes (operator new in
// createFileSystem): vtable, refcount, and the three archive lists (zip, pak, unzip). Not
// modelled yet: the archive add/remove functions, the obfuscation map and the XML reader
// factories (they need the IrrXML reader interface, which the port does not have).
#pragma once

#include <string>
#include <vector>

#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/file_list.hpp"
#include "glitch-core-io/io/interfaces.hpp"
#include "glitch-core-io/io/read_file.hpp"
#include "glitch-core-io/io/write_file.hpp"
#include "glitch-core-io/io/xml_writer.hpp"

namespace glitch::io {

class CZipReader;
class CPakReader;
class CUnZipReader;

class CFileSystem : public IFileSystem {
public:
    // 0x56c058 (C2), 0x56c0a8 (C1): zeroes the refcount (+0x04, so createFileSystem returns an
    // object with count 0) and the three archive lists; the vtable is implicit.
    CFileSystem();
    // 0x56cc44 (D1), 0x56ccc4 (D2): clear() drops the archive readers; the lists are then freed.
    ~CFileSystem();

    // 0x56cb78: drops every reader in the three archive lists and empties them.
    void clear();
    // 0x56df38: 1 when a zip, pak or unzip archive lists the name, or when the file can be opened
    // for reading; 0 otherwise. PARTIAL: the last step uses fopen, not CFileSystem::open (0x56dc40).
    int existFile(const char* name) const;
    // 0x34df7c: forwards to the name overload.
    int existFile(const std::string& name) const;

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
    // 0x56c6b4: the part before the last '/' or '\\', or "." when there is none.
    std::string getFileDir(const std::string& path) const;
    // 0x56c740: the file name after the last separator; without keepExtension the text from the
    // last '.' is dropped too. A dot before the separator gives the rest after the separator, as
    // the original's unsigned count does.
    std::string getFileBasename(const std::string& path, bool keepExtension) const;

    // The working directory is a process-wide static in the original (char[1024], symbols.csv).
    static std::string WorkingDirectory;  // _ZN6glitch2io11CFileSystem16WorkingDirectoryE

protected:
    // The archive lists, 12 bytes each in the original (begin, end, capacity). Each element is
    // referenced; clear() drops them. Protected so a derived test can fill them.
    std::vector<CZipReader*> zipReaders;      // +0x08
    std::vector<CPakReader*> pakReaders;      // +0x14
    std::vector<CUnZipReader*> unZipReaders;  // +0x20
};

// 0x56c2d8 _ZN6glitch2io16createFileSystemEv
IFileSystem* createFileSystem();

}  // namespace glitch::io
