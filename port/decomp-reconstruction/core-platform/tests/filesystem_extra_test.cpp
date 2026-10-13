#include "core-platform/deps/io_stream.hpp"
#include "core-platform/filesystem_base.hpp"
#include "core-platform/filesystem_win32.hpp"

#include <cstdio>
#include <string>
#include <vector>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// A stream that counts how many instances are alive.
struct CountedStream : IFileStream {
    static int alive;
    CountedStream() { ++alive; }
    ~CountedStream() override { --alive; }
    // The IStreamBase interface is not used by this test.
    std::uint32_t size() const override { return 0; }
    bool canRead() const override { return false; }
    bool canWrite() const override { return false; }
    std::uint64_t peek(void*, std::uint64_t) const override { return 0; }
    std::uint64_t read(void*, std::uint64_t) override { return 0; }
    std::uint64_t write(const void*, std::uint64_t) override { return 0; }
    void seek(std::uint64_t) override {}
    std::uint64_t tell() const override { return 0; }
};
int CountedStream::alive = 0;

int main() {
    FileSystemBase base;
    CHECK(base.formatFilePath("a/b\\c") == "a/b\\c");  // the base copies the path
    CHECK(base.getAbsolutePath("x/y") == "x/y");
    CHECK(base.ApplyFilenameHacks("save.dat") == "save.dat");

    // Existence checks: a file written here exists, a missing one does not.
    {
        std::FILE* f = std::fopen("dh2_fs_extra_exists.bin", "wb");
        CHECK(f != nullptr);
        if (f) {
            std::fputs("x", f);
            std::fclose(f);
        }
    }
    CHECK(base.doesFileExists("dh2_fs_extra_exists.bin"));
    CHECK(base.doesResourceExists("dh2_fs_extra_exists.bin"));
    CHECK(base.doesSavefileExists("dh2_fs_extra_exists.bin"));
    CHECK(!base.doesFileExists("dh2_fs_extra_missing.bin"));
    std::remove("dh2_fs_extra_exists.bin");

    // createAndOpenFile: a plain name gets the RES_PATH prefix (default "./"); a name
    // with ".config" is used as given.
    base.createAndOpenFile("dh2_fs_extra_created.txt");
    {
        std::FILE* f = std::fopen("dh2_fs_extra_created.txt", "rb");
        CHECK(f != nullptr);
        if (f) {
            std::fclose(f);
        }
    }
    std::remove("dh2_fs_extra_created.txt");

    // Win32 wrappers: closeFile deletes the stream and clears the pointer; getFolders
    // leaves the list alone.
    FileSystemWin32 fs;
    IFileStream* stream = new CountedStream();
    CHECK(CountedStream::alive == 1);
    fs.closeFile(stream);
    CHECK(stream == nullptr && CountedStream::alive == 0);
    fs.closeFile(stream);  // a null stream is ignored
    CHECK(stream == nullptr);

    std::vector<std::string> folders{"keep"};
    fs.getFolders(".", folders);
    CHECK(folders.size() == 1 && folders[0] == "keep");

    // Win32 formatFilePath rewrites forward slashes.
    CHECK(fs.formatFilePath("a/b") == "a\\b");

    std::printf("%s\n", failures ? "filesystem_extra: FAILED" : "filesystem_extra: ok");
    return failures != 0;
}
