// glitch::io::CFileSystem: the file system facade, the implementation of IFileSystem (io/interfaces.hpp,
// which lists the 24 slots). The object is 0x2c bytes (operator new in createFileSystem): vtable, refcount,
// and the three archive lists (zip, pak, unzip). Not modelled yet: the archive containers' add/remove
// functions, the obfuscation map and the nested-archive branch of createAndOpenFile. Their slots are declared
// (the interface is complete) and answer as the port's stubs do; the ledger notes say which.
#pragma once

#include <cstdint>
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
class CFile;

class CFileSystem : public IFileSystem {
public:
    // 0x56c058 (C2), 0x56c0a8 (C1): zeroes the refcount (+0x04, so createFileSystem returns an
    // object with count 0) and the three archive lists; the vtable is implicit.
    CFileSystem();
    // 0x56cc44 (D1), 0x56ccc4 (D2): clear() drops the archive readers; the lists are then freed.
    ~CFileSystem() override;

    // Vtable order (tools/decomp_vtable.py glitch::io::CFileSystem), +0x0c onwards:
    // +0x0c, 0x56d4e0: the archives first (createAndOpenFileFromArchives), then the plain file
    // (createReadFile). PARTIAL: the nested-archive branch (a path whose prefix is itself a zip entry) is
    // not reconstructed.
    IReadFile* createAndOpenFile(const char* name) override;
    // +0x10, 0x56c57c: a memory read file; null when the data pointer is null.
    IReadFile* createMemoryReadFile(void* data, int length, const char* name, bool takeOwnership) override;
    // +0x14, 0x56c570: opens a write file (see createWriteFile).
    IWriteFile* createAndWriteFile(const char* name, bool append) override;
    // +0x18 0x56d2c0, +0x1c 0x56d180, +0x20 0x56ca70, +0x24 0x56e6dc, +0x28 0x56ce60: the archive containers and
    // the obfuscation map are not reconstructed. PARTIAL: these answer false and change nothing.
    bool addZipFileArchive(const char* name, bool ignoreCase, bool ignorePaths) override;
    bool addFolderFileArchive(const char* name, bool ignoreCase, bool ignorePaths) override;
    bool addPakFileArchive(const char* name, bool ignoreCase, bool ignorePaths) override;
    bool addObfuscationFileMap(const char* name, unsigned char key, std::string path) override;
    bool removeFileArchive(const char* name) override;
    // +0x2c, 0x56c214: the working directory as last set by changeWorkingDirectoryTo (a static buffer).
    const char* getWorkingDirectory() override;
    // +0x30, 0x56c528: changes the process working directory; true when the change succeeded, and then
    // the new directory is kept for getWorkingDirectory.
    bool changeWorkingDirectoryTo(const char* directory) override;
    // +0x34, 0x56c348 (const): returns a copy of the name (the original does no path resolution here).
    std::string getAbsolutePath(const std::string& path) const override;
    // +0x38, 0x56c6b4: the part before the last '/' or '\', or "." when there is none.
    std::string getFileDir(const std::string& path) const override;
    // +0x3c, 0x56c740: the file name after the last separator; without keepExtension the text from the
    // last '.' is dropped too. A dot before the separator gives the rest after the separator, as
    // the original's unsigned count does.
    std::string getFileBasename(const std::string& path, bool keepExtension) const override;
    // +0x40, 0x56c4a4 (const): a new, empty directory listing.
    CFileList* createFileList() const override;
    // +0x44, 0x56df38: true when a zip, pak or unzip archive lists the name, or when the file can be opened
    // for reading; false otherwise. PARTIAL: the last step uses fopen, not CFileSystem::open (0x56dc40).
    bool existFile(const char* name) const override;
    // +0x48, 0x34df7c: forwards to the name overload.
    bool existFile(const std::string& name) const override;
    // +0x4c, 0x56c230: opens the file (slot +0x0c), builds the wide reader over it (slot +0x50) and drops the file.
    IXMLReader* createXMLReader(const char* name) override;
    // +0x50, 0x56c480: the wide reader over the file, or null for a null file.
    IXMLReader* createXMLReader(IReadFile* file) override;
    // +0x54, 0x56c44c; +0x58, 0x56c43c: the same for the narrow (UTF-8) reader.
    IXMLReaderUTF8* createXMLReaderUTF8(const char* name) override;
    IXMLReaderUTF8* createXMLReaderUTF8(IReadFile* file) override;
    // +0x5c, 0x56c278: an XML writer over a write file opened by name.
    IXMLWriter* createXMLWriter(const char* name) override;
    // +0x60, 0x56c414: an XML writer over the given write file.
    IXMLWriter* createXMLWriter(IWriteFile* file) override;
    // +0x64, 0x56c3ec: an attribute container bound to the driver.
    IAttributes* createEmptyAttributes(glitch::video::IVideoDriver* driver) override;
    // +0x68, 0x56cb78: drops every reader in the three archive lists and empties them.
    void clear() override;

    // Not virtual, not reconstructed (queue rows left `skipped`): 0x56dc40 open(name, mode) returns the shared CFile handle
    // (applying the obfuscation map); 0x56e064 getObfuscatedFileName(name) looks the name up in the obfuscation map
    // (its return type is not visible in the listing, so it is not declared). Both need the map, which is not modelled.
    CFile* open(const char* name, const char* mode);

    // Not virtual: 0x56c0f8 asks each reader of the zip, pak and unzip lists in turn (slot +0x0c of the reader).
    IReadFile* createAndOpenFileFromArchives(const char* name);

    // The working directory is a process-wide static buffer in the original (char[1024],
    // _ZN6glitch2io11CFileSystem16WorkingDirectoryE in .data). core-platform's stand-in
    // CFileSystem_WorkingDirectory[512] is the same global.
    static char WorkingDirectory[1024];

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
