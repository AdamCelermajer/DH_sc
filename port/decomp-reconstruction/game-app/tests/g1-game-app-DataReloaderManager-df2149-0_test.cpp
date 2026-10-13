// Test for batch g1-game-app-DataReloaderManager-df2149-0. Covers the constructor, the two empty methods, the
// _MonitoredFiles destructor (block release order and the skip rules) and the DataReloaderManager destructor
// (element release order, null entries). This test links without core-platform, so CustomFree is replaced by a
// logging stub that frees with std::free. Exit status 0 means every check passed.
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <vector>

#include "core-platform/memory.hpp"
#include "dh2/DataReloaderManager.hpp"

namespace {

int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// Every pointer passed to CustomFree by the code under test, in call order.
std::vector<void*> g_freed;

// Allocates a _MonitoredFiles with malloc (CustomFree frees with std::free) and clears its pointers.
DataReloaderManager::_MonitoredFiles* NewFiles() {
    void* mem = std::malloc(sizeof(DataReloaderManager::_MonitoredFiles));
    auto* files = new (mem) DataReloaderManager::_MonitoredFiles();
    files->unk_04 = nullptr;
    files->unk_18 = nullptr;
    files->unk_1c = nullptr;
    files->unk_30 = nullptr;
    return files;
}

char* NewBlock(std::size_t bytes) {
    return static_cast<char*>(std::malloc(bytes));
}

}  // namespace

// Replaces core-platform's CustomFree for this test binary. Records the pointer, then frees it.
void CustomFree(void* p) {
    g_freed.push_back(p);
    std::free(p);
}

int main() {
    // 1. A fresh manager is empty, and its no-op methods free nothing.
    {
        DataReloaderManager mgr;
        Check(mgr.m_monitoredFiles.empty(), "constructor leaves the monitored list empty");
        mgr.checkFiles();
        mgr.monitorFiles("data", "file.lua", nullptr);
        Check(g_freed.empty(), "empty methods free nothing");
    }
    Check(g_freed.empty(), "destroying an empty manager frees nothing");

    // 2. A group with no blocks is released once; null entries are skipped.
    g_freed.clear();
    void* expectGroup2 = nullptr;
    {
        DataReloaderManager mgr;
        DataReloaderManager::_MonitoredFiles* files = NewFiles();
        mgr.m_monitoredFiles.push_back(nullptr);
        mgr.m_monitoredFiles.push_back(files);
        mgr.m_monitoredFiles.push_back(nullptr);
        expectGroup2 = files;
    }
    Check(g_freed.size() == 1, "a group with no blocks is freed once and null entries are skipped");
    if (g_freed.size() == 1) {
        Check(g_freed[0] == expectGroup2, "the freed pointer is the group");
    }

    // 3. Only the +0x30 block: it is released first, then the group.
    g_freed.clear();
    void* expect3Block = nullptr;
    void* expect3Group = nullptr;
    {
        DataReloaderManager mgr;
        DataReloaderManager::_MonitoredFiles* files = NewFiles();
        char* block30 = NewBlock(16);
        files->unk_30 = block30;
        files->unk_1c = block30 + 16;
        mgr.m_monitoredFiles.push_back(files);
        expect3Block = block30;
        expect3Group = files;
    }
    Check(g_freed.size() == 2, "block30 and the group are released");
    if (g_freed.size() == 2) {
        Check(g_freed[0] == expect3Block, "block30 is released before the group");
        Check(g_freed[1] == expect3Group, "the group is released last");
    }

    // 4. Only the +0x18 block (end at +0x04). The +0x30 field is the inline slot (&unk_1c), so it is not freed.
    g_freed.clear();
    void* expect4Block = nullptr;
    void* expect4Group = nullptr;
    {
        DataReloaderManager mgr;
        DataReloaderManager::_MonitoredFiles* files = NewFiles();
        char* block18 = NewBlock(8);
        files->unk_18 = block18;
        files->unk_04 = block18 + 8;
        files->unk_30 = reinterpret_cast<char*>(&files->unk_1c);
        mgr.m_monitoredFiles.push_back(files);
        expect4Block = block18;
        expect4Group = files;
    }
    Check(g_freed.size() == 2, "block18 and the group are released when block30 is the inline slot");
    if (g_freed.size() == 2) {
        Check(g_freed[0] == expect4Block, "block18 is released before the group");
        Check(g_freed[1] == expect4Group, "the group is released last");
    }

    // 5. Both blocks: the +0x30 block first, then the +0x18 block, then the group.
    g_freed.clear();
    void* expect5Block30 = nullptr;
    void* expect5Block18 = nullptr;
    void* expect5Group = nullptr;
    {
        DataReloaderManager mgr;
        DataReloaderManager::_MonitoredFiles* files = NewFiles();
        char* block30 = NewBlock(16);
        char* block18 = NewBlock(8);
        files->unk_30 = block30;
        files->unk_1c = block30 + 16;
        files->unk_18 = block18;
        files->unk_04 = block18 + 8;
        mgr.m_monitoredFiles.push_back(files);
        expect5Block30 = block30;
        expect5Block18 = block18;
        expect5Group = files;
    }
    Check(g_freed.size() == 3, "both blocks and the group are released");
    if (g_freed.size() == 3) {
        Check(g_freed[0] == expect5Block30, "block30 is released first");
        Check(g_freed[1] == expect5Block18, "block18 is released second");
        Check(g_freed[2] == expect5Group, "the group is released last");
    }

    if (g_failures == 0) {
        std::printf("all checks passed\n");
        return 0;
    }
    std::printf("%d check(s) failed\n", g_failures);
    return 1;
}
