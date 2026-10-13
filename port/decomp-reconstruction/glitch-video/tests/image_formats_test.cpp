// Format probes for PNG, TGA, JPG and PNG writers, the DDS header reader, the
// texture data reader sizes, and the loader factories.
#include "glitch-video/image_loaders.hpp"
#include "glitch-video/texture_loaders.hpp"
#include "video_test_types.hpp"

#include <cstdio>
#include <cstring>
#include <string>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {

// Seekable in-memory IReadFile. seek(pos, relative) and getSize are used by the probes.
class MemoryFile : public glitch::io::IReadFile {
public:
    explicit MemoryFile(std::string bytes) : m_bytes(std::move(bytes)) {}
    void unidentifiedBaseSlot() override {}
    int32_t read(void* buffer, uint32_t size) override {
        uint32_t n = 0;
        while (n < size && m_pos < m_bytes.size()) {
            static_cast<unsigned char*>(buffer)[n++] = static_cast<unsigned char>(m_bytes[m_pos++]);
        }
        return static_cast<int32_t>(n);
    }
    bool seek(int32_t position, bool relative) override {
        int64_t target = relative ? static_cast<int64_t>(m_pos) + position : position;
        if (target < 0 || target > static_cast<int64_t>(m_bytes.size())) return false;
        m_pos = static_cast<size_t>(target);
        return true;
    }
    uint32_t getSize() const override { return static_cast<uint32_t>(m_bytes.size()); }
    int32_t getPos() const override { return static_cast<int32_t>(m_pos); }

private:
    std::string m_bytes;
    size_t m_pos = 0;
};

const char kPngSig[8] = {'\x89', 'P', 'N', 'G', '\r', '\n', '\x1a', '\n'};

std::string ddsFile(uint32_t size, uint32_t pixelFormatSize) {
    std::string file = "DDS ";
    unsigned char header[0x7c] = {};
    auto put = [&](size_t off, uint32_t v) {
        for (int i = 0; i < 4; ++i) header[off + i] = static_cast<unsigned char>(v >> (8 * i));
    };
    put(0x00, size);
    put(0x48, pixelFormatSize);
    file.append(reinterpret_cast<char*>(header), sizeof(header));
    return file;
}

}  // namespace

int main() {
    using namespace glitch::video;

    // PNG format probe reads the 8-byte signature.
    CImageLoaderPng png;
    { MemoryFile f(std::string(kPngSig, 8)); CHECK(png.isALoadableFileFormat(&f)); }
    { MemoryFile f(std::string("GIF89a", 6)); CHECK(!png.isALoadableFileFormat(&f)); }
    { MemoryFile f(std::string(kPngSig, 4)); CHECK(!png.isALoadableFileFormat(&f)); }

    // PNG extension: text after the last dot only.
    CHECK(png.isALoadableFileExtension("a.png"));
    CHECK(png.isALoadableFileExtension("dir.x/A.PNG"));
    CHECK(!png.isALoadableFileExtension("a.Png"));
    CHECK(!png.isALoadableFileExtension("a.png.bak"));
    CHECK(!png.isALoadableFileExtension("noext"));

    // TGA 2.0 footer: 26 bytes, signature at footer offset 8.
    CImageLoaderTGA tga;
    {
        std::string body(40, 'x');
        std::string footer(26, '\0');
        std::memcpy(&footer[8], "TRUEVISION-XFILE.", 17);
        MemoryFile withFooter(body + footer);
        CHECK(tga.isALoadableFileFormat(&withFooter));
        MemoryFile noFooter(body);
        CHECK(!tga.isALoadableFileFormat(&noFooter));
        MemoryFile tiny(std::string(25, '\0'));
        CHECK(!tga.isALoadableFileFormat(&tiny));
    }

    // Writers: JPG matches the last-dot extension, PNG matches a substring.
    CImageWriterJPG jpg;
    CHECK(jpg.isAWriteableFileExtension("out.jpeg"));
    CHECK(jpg.isAWriteableFileExtension("out.JPEG"));
    CHECK(!jpg.isAWriteableFileExtension("out.jpg.png"));
    CImageWriterPNG pngWriter;
    CHECK(pngWriter.isAWriteableFileExtension("out.png"));
    CHECK(pngWriter.isAWriteableFileExtension("out.png.bak"));
    CHECK(!pngWriter.isAWriteableFileExtension("out.bmp"));

    // DDS header: size word must equal the bytes read, pixel format size must be 32.
    SDDSSurfaceFormatHeader header;
    MemoryFile dds(ddsFile(0x7c, 0x20));
    CHECK(readDDSHeader(&dds, header));
    CHECK(header.size == 0x7c);
    MemoryFile ddsBadFormat(ddsFile(0x7c, 0x1f));
    CHECK(!readDDSHeader(&ddsBadFormat, header));
    MemoryFile ddsShort(ddsFile(0x7c, 0x20).substr(0, 40));
    CHECK(!readDDSHeader(&ddsShort, header));

    // Texture data reader: source step without the pitch flag uses the header dimensions.
    STextureHeaderBlock dims;
    dims.width = 64;
    dims.height = 32;
    STextureDataInfo info;
    info.header = &dims;
    glitch::video::test::TestTextureDataLoading loading;
    loading.m_info = &info;
    loading.m_sourceStep = 4;
    CHECK(loading.getSourceStep(0) == 32 * 4 * 64);
    CHECK(loading.getSourceStep(1) == 16 * 4 * 32);
    CHECK(loading.getSourceStep(7) == 1 * 4 * 1);   // clamped to 1 per axis
    CHECK(loading.getSourceStep(40) == 1 * 4 * 1);  // shifts of 32 or more clamp too

    // read returns 1 when the full count is read, otherwise sets the error flag.
    MemoryFile small(std::string(10, 'a'));
    loading.m_file = &small;
    char buf[32] = {};
    CHECK(loading.read(buf, 10) == 1);
    CHECK(!loading.m_flag20);
    CHECK(loading.read(buf, 4) == 0);
    CHECK(loading.m_flag20);

    // Factories: new loader with the reference word at 1.
    IImageLoader* atc = createImageLoaderATC();
    CHECK(atc != nullptr && atc->referenceCounter == 1);
    delete atc;
    IImageLoader* pngLoader = createImageLoaderPNG();
    CHECK(pngLoader != nullptr && pngLoader->referenceCounter == 1);
    delete pngLoader;
    IImageLoader* tgaLoader = createImageLoaderTGA();
    CHECK(tgaLoader != nullptr && tgaLoader->referenceCounter == 1);
    delete tgaLoader;

    std::printf("%s\n", failures ? "image_formats: FAILED" : "image_formats: ok");
    return failures != 0;
}
