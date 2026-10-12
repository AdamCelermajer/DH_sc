// CFileSystem factories and working-directory helpers.
#include "glitch-core-io/io/file_system.hpp"
#include "glitch-core-io/io/interfaces.hpp"

#include <cstdio>
#include <cstring>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::io;

int main() {
    CHECK(std::strcmp(CFileSystem::getWorkingDirectory(), "") == 0);

    CFileSystem fs;
    CHECK(!fs.changeWorkingDirectoryTo("glitch_no_such_directory_xyz"));
    CHECK(fs.changeWorkingDirectoryTo("."));
    CHECK(CFileSystem::WorkingDirectory == ".");

    CHECK(fs.getAbsolutePath("a/b.txt") == "a/b.txt");

    CFileList* list = fs.createFileList();
    CHECK(list != nullptr && list->getFileCount() == 0);
    if (list) list->drop();

    CHECK(fs.createMemoryReadFile(nullptr, 4, "none", false) == nullptr);
    char block[3] = {'a', 'b', 'c'};
    IReadFile* mem = fs.createMemoryReadFile(block, 3, "block", false);
    CHECK(mem != nullptr);
    if (mem) mem->drop();

    const char* name = "glitch_fs_factory_test.bin";
    IWriteFile* written = fs.createAndWriteFile(name, false);
    CHECK(written != nullptr);
    if (written) {
        CHECK(written->write("xy", 2) == 2);
        written->drop();
    }
    std::remove(name);

    IXMLWriter* writer = fs.createXMLWriter(static_cast<IWriteFile*>(nullptr));
    CHECK(writer != nullptr);
    if (writer) writer->drop();

    CAttributes* attributes = fs.createEmptyAttributes(nullptr);
    CHECK(attributes != nullptr);
    if (attributes) attributes->drop();

    IFileSystem* created = createFileSystem();
    CHECK(created != nullptr);
    if (created) created->drop();

    if (failures == 0) std::printf("file_system_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
