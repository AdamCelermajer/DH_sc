#pragma once

// PyDataArrays and PyDataConstants (slice pydata12, wave 7). No existing header declares these two
// classes, so they are declared here with the members the listing uses. Layouts are internal to the
// port (NOTES.md, "std::map stand-in (decided, wave 7)"): the byte offsets of the original are noted
// on each member, but the port does not reproduce them.

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "pydata-accessors/deps/stream_reader.hpp"

class DataReloaderManager;  // Type not recovered; the constructors only store the pointer.

// Loader used by PyDataArrays::Load. The listing reaches it through the engine global dword_99F73C
// (its +52 word) and makes two virtual calls: vtable +144 opens "pydata/<name>" and returns a stream or
// null; vtable +120 releases the stream slot. The loader class is not recovered, so this interface is a
// stand-in that names only what the listing calls.
class PyDataFileLoader {
public:
    virtual ~PyDataFileLoader() = default;
    virtual IStreamBase* OpenFile(const char* path) = 0;  // original vtable +0x90
    virtual void ReleaseFile(IStreamBase** slot) = 0;      // original vtable +0x78
};

extern PyDataFileLoader* g_pyDataLoader;  // dword_99F73C + 52; defined by the owning slice
extern int gAssertLevel;                  // engine assert level read by registerClassByName

class PyDataArrays {
public:
    using ClassFactory = int (*)(const char*);
    using FileRead = void (*)(IStreamBase*);
    using FileFinalize = void (*)();

    // One entry per file: the reader and its finalizer. The destructor calls the finalizer (original +4).
    struct _Funcs {
        FileRead read;
        FileFinalize finalize;
    };

    explicit PyDataArrays(DataReloaderManager* reloader);
    virtual ~PyDataArrays();

    // Vtable slot +8 in the original. Defined by another slice; Load calls it for each file.
    virtual void reloadData(IStreamBase* stream, const char* fileName);

    int Load();
    ClassFactory& registerClassByName(const char* name, ClassFactory factory);

private:
    std::map<std::string, std::vector<_Funcs>> m_files;  // original this+4 (header node) and the tree
    std::map<std::string, ClassFactory> m_classes;       // original this+28 (registry stand-in)
    DataReloaderManager* m_reloader = nullptr;           // original this+52
    int m_state = 0;                                     // original this+56, Load's file index
};

// Deleting destructor (D0 in the listing): destroys the object and frees its storage.
void DeletingDestructor(PyDataArrays* self);

class PyDataConstants {
public:
    explicit PyDataConstants(DataReloaderManager* reloader);
    virtual ~PyDataConstants();

    // Returns the first constant name, in key order, whose value equals `value` within `className`,
    // or "" when the class is absent or no entry matches. The original returned a char* into the node.
    const char* getConstantName(const char* className, int value) const;

    // Returns true when the whole stream was parsed. The original returns its last intermediate value.
    bool reloadData(IStreamBase* stream, const char* fileName);

private:
    std::map<std::string, std::map<std::string, int>> m_classes;  // original this+4 tree
    DataReloaderManager* m_reloader = nullptr;                    // original this+28
};

// Deleting destructor (D0 in the listing).
void DeletingDestructor(PyDataConstants* self);
