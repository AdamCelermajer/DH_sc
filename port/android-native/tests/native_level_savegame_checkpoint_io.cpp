#include "../app/src/main/cpp/native_level_savegame_publisher_v1.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}
void write(const std::filesystem::path& path,
           const std::vector<std::uint8_t>& bytes) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    check(bool(file), "checkpoint fixture open failed");
    file.write(reinterpret_cast<const char*>(bytes.data()),
               static_cast<std::streamsize>(bytes.size()));
    check(bool(file), "checkpoint fixture write failed");
}
}

int main() {
    const auto root = std::filesystem::temp_directory_path() /
        ("dh2-checkpoint-io-" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(root);
    struct Cleanup {
        std::filesystem::path path;
        ~Cleanup() { std::error_code ignored; std::filesystem::remove_all(path, ignored); }
    } cleanup{root};
    const std::vector<std::uint8_t> level{1, 2, 3, 4};
    const std::vector<std::uint8_t> player{5, 6, 7};
    write(root / "dh2_002_1_single_level.checkpoint", level);
    write(root / "dh2_002_single.checkpoint", player);

    dh2::native::level_savegame_publisher_v1::Publisher reader(root);
    std::vector<std::uint8_t> image;
    std::string error;
    check(reader.read_checkpoint("dh2_002_1_single_level.checkpoint", image, error) &&
          image == level, "LevelSavegame checkpoint image was not read from the bound directory");
    check(reader.read_player_checkpoint("dh2_002_single.checkpoint", image, error) &&
          image == player, "PlayerSavegame checkpoint image was not read from the bound directory");
    const auto retained = image;
    check(!reader.read_checkpoint("../dh2_002_1_single_level.checkpoint", image, error) &&
          image == retained && !error.empty(), "checkpoint reader accepted traversal or changed output on failure");
    check(!reader.read("dh2_002_single.checkpoint", image, error) &&
          image == retained && !error.empty(), "normal LevelSavegame reader accepted Player checkpoint filename");
    std::cout << "PASS native_level_savegame_checkpoint_io checks=4\n";
}
