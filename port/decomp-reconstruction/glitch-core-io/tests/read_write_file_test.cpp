#include "glitch-core-io/io/read_file.hpp"
#include "glitch-core-io/io/write_file.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    const char* name = "glitch_io_rw_test.bin";

    // Write, then flush and check size and position.
    {
        glitch::io::CWriteFile w(name, false);
        w.openFile(false);
        CHECK(w.write("abcdefgh", 8) == 8);
        w.flush();
        CHECK(w.getPos() == 8);
        CHECK(w.seek(2, false) && w.getPos() == 2);
        CHECK(w.getFileName() == name);
    }

    // Read back.
    {
        glitch::io::CReadFile r(name, false);
        CHECK(r.isOpen());
        CHECK(r.getSize() == 8);
        CHECK(r.getFileName() == name);
        char buf[4] = {0};
        r.read(buf, 3);
        CHECK(std::memcmp(buf, "abc", 3) == 0);
        CHECK(r.getPos() == 3);
        CHECK(r.seek(1, true) && r.getPos() == 4);
        r.read(buf, 4);
        CHECK(std::memcmp(buf, "efgh", 4) == 0);

        glitch::io::CReadFile* c = r.clone();
        CHECK(c->getPos() == r.getPos() && c->getSize() == 8);
        delete c;
    }

    // Obfuscated read: the first bytes are shifted by (~pos - i).
    {
        glitch::io::CReadFile r(name, true);
        char buf[2] = {0};
        r.read(buf, 2);
        CHECK(static_cast<unsigned char>(buf[0]) == static_cast<unsigned char>('a' + static_cast<unsigned char>(~0u)));
    }

    // Missing file: not open, size 0.
    {
        glitch::io::CReadFile missing("glitch_io_missing_file.bin", false);
        CHECK(!missing.isOpen() && missing.getSize() == 0);
        CHECK(!missing.seek(0, false));
    }

    std::remove(name);
    if (failures) return 1;
    std::printf("read_write_file_test ok\n");
    return 0;
}
