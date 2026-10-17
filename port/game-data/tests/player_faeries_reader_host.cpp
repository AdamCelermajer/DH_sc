#include "../player_savegame_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

using dh2::data::Bytes;
using dh2::data::PlayerSavegameV1;

namespace {
void require(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

bool read_exact(std::FILE* file, void* data, std::size_t size) {
    return size == 0 || std::fread(data, 1, size, file) == size;
}

void write_u8(std::FILE* file, std::uint8_t value) {
    require(std::fwrite(&value, 1, 1, file) == 1, "result write failed");
}

void write_u16(std::FILE* file, std::uint16_t value) {
    const std::uint8_t bytes[]{std::uint8_t(value), std::uint8_t(value >> 8)};
    require(std::fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes),
            "result write failed");
}

void write_u32(std::FILE* file, std::uint32_t value) {
    const std::uint8_t bytes[]{std::uint8_t(value), std::uint8_t(value >> 8),
                               std::uint8_t(value >> 16), std::uint8_t(value >> 24)};
    require(std::fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes),
            "result write failed");
}

unsigned policy_checks() {
    unsigned checks = 0;
    const auto check = [&](bool value) {
        if (!value) throw std::runtime_error(
            "FAES native policy check failed #" + std::to_string(checks + 1));
        ++checks;
    };
    std::string error;
    std::size_t consumed = 99;
    bool mismatch = true;
    dh2::data::PlayerSavegameV1 fresh;
    std::uint8_t valid_zeroes[69]{};
    valid_zeroes[4] = 5;
    valid_zeroes[27] = 5;
    valid_zeroes[50] = 5;
    check(!fresh.load_faeries({valid_zeroes, sizeof(valid_zeroes)}, consumed,
                              mismatch, error));
    check(consumed == 0 && !mismatch);
    check(!fresh.faeries_initialized()[0] && !fresh.faeries_initialized()[1] &&
          !fresh.faeries_initialized()[2]);

    dh2::data::PlayerSavegameV1 initialized;
    initialized.initialize_faeries();
    check(initialized.faeries_initialized()[0] &&
          initialized.faeries_initialized()[1] &&
          initialized.faeries_initialized()[2]);
    check(initialized.load_faeries({valid_zeroes, sizeof(valid_zeroes)}, consumed,
                                   mismatch, error));
    check(consumed == sizeof(valid_zeroes) && !mismatch);

    // Original __LoadFaeries returns at the mismatching difficulty after its
    // ID and count; later difficulties must remain untouched.
    std::vector<std::uint8_t> bad_count(69, 0);
    bad_count[0] = 0x34;
    bad_count[4] = 4;
    consumed = 0;
    mismatch = false;
    check(initialized.load_faeries({bad_count.data(), bad_count.size()},
                                   consumed, mismatch, error));
    check(consumed == 8 && mismatch &&
          initialized.current_faery(0) == 0x34 &&
          initialized.current_faery(1) == 0);

    check(initialized.set_current_faery(4, 1, error));
    check(initialized.current_faery(1) == 4 && initialized.current_faery(0) == 0x34);
    check(!initialized.set_current_faery(5, 1, error));
    check(initialized.current_faery(1) == 4);
    check(!initialized.set_current_faery(0, 3, error));
    PlayerSavegameV1 uninitialized;
    check(!uninitialized.set_current_faery(0, 0, error));
    return checks;
}
}  // namespace

int main(int argc, char** argv) {
    try {
        require(argc == 3, "expected input and output paths");
        std::FILE* input = std::fopen(argv[1], "rb");
        std::FILE* output = std::fopen(argv[2], "wb");
        require(input && output, "could not open fixture files");
        std::uint32_t count = 0;
        require(read_exact(input, &count, sizeof(count)), "missing fixture count");

        for (std::uint32_t i = 0; i < count; ++i) {
            std::uint32_t initialized = 0, size = 0;
            require(read_exact(input, &initialized, sizeof(initialized)) &&
                    read_exact(input, &size, sizeof(size)), "truncated fixture header");
            require(initialized <= 1 && size <= 4096, "invalid fixture bounds");
            std::vector<std::uint8_t> payload(size);
            require(read_exact(input, payload.data(), payload.size()),
                    "truncated fixture payload");

            PlayerSavegameV1 save;
            if (initialized) save.initialize_faeries();
            std::size_t consumed = 0;
            bool mismatch = false;
            std::string error;
            const bool ok = save.load_faeries(
                {payload.data(), payload.size()}, consumed, mismatch, error);

            write_u32(output, ok ? 0u : 1u);
            write_u32(output, static_cast<std::uint32_t>(consumed));
            write_u32(output, mismatch ? 1u : 0u);
            for (const auto id : save.current_faeries())
                write_u32(output, static_cast<std::uint32_t>(id));
            for (const auto& difficulty : save.faeries())
                for (const auto& row : difficulty) {
                    write_u8(output, row.state);
                    write_u16(output, row.level);
                }
        }
        require(std::fclose(input) == 0 && std::fclose(output) == 0,
                "fixture close failed");
        std::printf("%u\n", policy_checks());
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 9;
    }
}
