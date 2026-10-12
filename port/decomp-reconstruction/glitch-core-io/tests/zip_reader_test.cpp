// SZipFileEntry copy and assignment, CZipReader directory helpers, and the IrrXML callback.
#include "glitch-core-io/io/irr_xml_file_read_callback.hpp"
#include "glitch-core-io/io/zip_reader.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::io;

// A read file over a string, with working position and size.
class BufferFile : public IReadFile {
public:
    explicit BufferFile(std::string bytes) : data(std::move(bytes)) {}
    void read(void* buffer, unsigned int count) override {
        std::size_t n = std::min<std::size_t>(count, data.size() - pos);
        std::memcpy(buffer, data.data() + pos, n);
        pos += n;
    }
    bool seek(long offset, bool relative) override {
        long target = relative ? static_cast<long>(pos) + offset : offset;
        if (target < 0 || static_cast<std::size_t>(target) > data.size()) return false;
        pos = static_cast<std::size_t>(target);
        return true;
    }
    long getSize() const override { return static_cast<long>(data.size()); }
    long getPos() const override { return static_cast<long>(pos); }

    std::string data;
    std::size_t pos = 0;
};

static void testEntryCopy() {
    SZipFileEntry a;
    a.path = "dir/a.txt";
    a.name = "a.txt";
    a.extra = "x";
    a.f48 = 11;
    a.f68 = 3;
    SZipFileEntry b(a);
    CHECK(b.path == "dir/a.txt" && b.name == "a.txt" && b.extra == "x");
    CHECK(b.f48 == 11 && b.f68 == 3);
    SZipFileEntry c;
    c = b;
    CHECK(c.name == "a.txt" && c.f48 == 11);
    c = c;
    CHECK(c.path == "dir/a.txt");
}

static void testDirectory() {
    CZipReader zip;
    CHECK(zip.getFileCount() == 0);
    SZipFileEntry first;
    first.name = "one";
    zip.entries.push_back(first);
    SZipFileEntry second;
    second.name = "two";
    second.f4c = 99;
    zip.entries.push_back(second);
    CHECK(zip.getFileCount() == 2);
    CHECK(zip.getFileInfo(1)->name == "two" && zip.getFileInfo(1)->f4c == 99);
}

static void testDeletePath() {
    CZipReader zip;
    std::string a = "dir/sub\\file.txt";
    zip.deletePathFromFilename(a);
    CHECK(a == "file.txt");
    std::string b = "/x";
    zip.deletePathFromFilename(b);
    CHECK(b == "/x");
    std::string c = "plain";
    zip.deletePathFromFilename(c);
    CHECK(c == "plain");
    std::string d = "trailing/";
    zip.deletePathFromFilename(d);
    CHECK(d.empty());
    std::string e;
    zip.deletePathFromFilename(e);
    CHECK(e.empty());
}

static void testValidSignature() {
    BufferFile good(std::string("PK\x03\x04rest", 8));
    CHECK(CZipReader::isValid(&good));
    CHECK(good.pos == 0);  // position restored
    BufferFile bad(std::string("ZZZZ", 4));
    CHECK(!CZipReader::isValid(&bad));
    CHECK(!CZipReader::isValid("glitch_zip_missing_file.zip"));
}

static void testIrrXmlCallback() {
    BufferFile* inner = new BufferFile("0123456789");
    {
        CIrrXMLFileReadCallBack cb(inner);  // the callback drops its reference to the file when it ends
        char buf[4] = {0};
        cb.read(buf, 3);
        CHECK(std::memcmp(buf, "012", 3) == 0);
        CHECK(inner->getPos() == 3);
        CHECK(cb.getSize() == 10);
    }
}

int main() {
    testEntryCopy();
    testDirectory();
    testDeletePath();
    testValidSignature();
    // findFile: the key is the name; with flagA it is lower-cased, and with flagB its path is removed.
    {
        CZipReader indexed;
        SZipFileEntry first;
        first.name = "a.txt";
        SZipFileEntry second;
        second.name = "b.txt";
        indexed.entries = {first, second};
        CHECK(indexed.findFile("b.txt") == 1);
        CHECK(indexed.findFile("zz.txt") == -1);
        indexed.flagA = true;
        CHECK(indexed.findFile("B.TXT") == 1);
        indexed.flagB = true;
        CHECK(indexed.findFile("dir/a.txt") == 0);
    }
    testIrrXmlCallback();
    if (failures == 0) std::printf("zip_reader_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
