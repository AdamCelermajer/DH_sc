// Batch g1-game-app-DataReloaderManager-df2149-0 (DataReloaderManager, part 1 of 1).
// The constructor, the two empty methods, the _MonitoredFiles destructor and the DataReloaderManager destructor.
// The C1/C2 constructor pair and the D1/D2 destructor pair have identical bodies, so each pair has one
// definition here. The scalar deleting destructor (D0) is compiler-generated and is not written.
#include "dh2/DataReloaderManager.hpp"

#include "core-platform/memory.hpp"

// 0x339094 DataReloaderManager::DataReloaderManager()
// 0x3390c8 DataReloaderManager::DataReloaderManager()  (C1 variant, same body)
// Asm (0x339094): stores 0 to +0x0c, +0x04 and +0x08, then the vtable address point (_ZTV19DataReloaderManager + 8)
// to +0x00. The three zero stores are the empty STLport vector (begin, end, end of storage), which the default
// constructor of m_monitoredFiles produces. The vptr store is done by the compiler.
DataReloaderManager::DataReloaderManager() {
}

// 0x3390fc DataReloaderManager::monitorFiles(char const*, char const*, IDataReloader*)
// Empty: the binary has one BX LR here (bytes 1e ff 2f e1). The ELF symbol size (155) runs into unrelated code
// at 0x339104, which is not part of this function.
void DataReloaderManager::monitorFiles(const char* dir, const char* file, IDataReloader* reloader) {
    (void)dir;
    (void)file;
    (void)reloader;
}

// 0x339100 DataReloaderManager::checkFiles() const
// Empty: the binary has one BX LR here (bytes 1e ff 2f e1).
void DataReloaderManager::checkFiles() const {
}

// 0x339124 DataReloaderManager::_MonitoredFiles::~_MonitoredFiles()
// PARTIAL: the tests and the order of the two releases follow the asm. The block at +0x30 is released before the
// block at +0x18. Each block is skipped when it is null or when it is the inline slot whose address is the field
// itself (+0x1c for the first, +0x04 for the second). The original releases a block with the node allocator
// (__node_alloc::_M_deallocate) when its size (end - start) is 0x80 bytes or less, and with CustomFree above that.
// The node allocator is not recovered, so both sizes go to CustomFree here, the same gap as ClearSubtitles in
// core-platform/src/w7_global_00.cpp.
DataReloaderManager::_MonitoredFiles::~_MonitoredFiles() {
    // Block at +0x30 (unk_30). Its size is (+0x1c) - (+0x30).
    char* block30 = unk_30;
    if (block30 != nullptr && reinterpret_cast<void*>(block30) != reinterpret_cast<void*>(&unk_1c)) {
        CustomFree(block30);
    }

    // Block at +0x18 (unk_18). Its size is (+0x04) - (+0x18).
    char* block18 = unk_18;
    if (block18 != nullptr && reinterpret_cast<void*>(block18) != reinterpret_cast<void*>(&unk_04)) {
        CustomFree(block18);
    }
}

// 0x3391a0 DataReloaderManager::~DataReloaderManager()  (D1, complete-object destructor)
// 0x33925c DataReloaderManager::~DataReloaderManager()  (D2, same body)
// PARTIAL, for the release of the vector's own storage. The element loop follows the asm: each non-null entry is
// destroyed (its ~_MonitoredFiles, then CustomFree) in vector order. The original re-reads the end pointer after
// each free, but the vector is not changed by the loop, so one range loop covers it. The original then releases
// the vector's storage itself, with the node allocator when the size ((end of storage - begin) & ~3, 4-byte
// pointers) is 0x80 bytes or less and with CustomFree otherwise. On the host the storage is released by the
// std::vector member's destructor, which runs after this body. The 32-bit size arithmetic and the node allocator
// are not reproduced. The vptr store at entry is done by the compiler.
DataReloaderManager::~DataReloaderManager() {
    for (_MonitoredFiles* files : m_monitoredFiles) {
        if (files != nullptr) {
            files->~_MonitoredFiles();
            CustomFree(files);
        }
    }
}

// 0x339240 DataReloaderManager::~DataReloaderManager()  (D0, scalar deleting destructor)
// Not written: compiler-generated from the virtual destructor. Asm: BL D1 (0x3391a0), then BL CustomFree(this).
