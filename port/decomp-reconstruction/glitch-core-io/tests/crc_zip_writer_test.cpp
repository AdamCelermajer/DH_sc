// CRC-32 table (reflected polynomial, checked against the standard values) and CZipWriter state.
#include "glitch-core-io/boost/crc_table.hpp"
#include "glitch-core-io/io/zip_writer.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using boost::detail::crc_table_t_32_79764919_true;

int main() {
    crc_table_t_32_79764919_true::init_table();
    crc_table_t_32_79764919_true::init_table();  // second call does nothing
    const std::uint32_t* table = crc_table_t_32_79764919_true::table_;
    CHECK(table[0] == 0x00000000u);
    CHECK(table[1] == 0x77073096u);
    CHECK(table[2] == 0xEE0E612Cu);
    CHECK(table[128] == 0xEDB88320u);
    CHECK(table[255] == 0x2D02EF8Du);

    glitch::io::CZipWriter* zip = new glitch::io::CZipWriter(nullptr);
    CHECK(zip->referenceCounter == 1);
    CHECK(zip->writeFile == nullptr && !zip->flag && zip->names.empty());
    zip->drop();

    if (failures == 0) std::printf("crc_zip_writer_test: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
