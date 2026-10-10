#include "native_trophy_runtime.hpp"

#include <array>
#include <filesystem>
#include <fstream>

namespace dh2::native::trophies {

bool OwnerV1::initialize(data::Bytes records, data::Bytes names,
                         data::Bytes fields, std::string& error) {
    error.clear();
    if (manager_.initialized()) return true;

    data::TrophyTableV1 table;
    if (!table.load(records, names, fields, error)) return false;
    return manager_.initialize(table, error);
}

bool OwnerV1::load_savegame(const std::string& runtime_directory,
                            std::string& error) {
    if (!initialized() || runtime_directory.empty()) {
        error = "Trophy sidecar load requires the initialized manager and runtime directory";
        return false;
    }
    const auto path = std::filesystem::path(runtime_directory) / "achievements.savegame";
    std::error_code filesystem_error;
    const bool exists = std::filesystem::exists(path, filesystem_error);
    if (filesystem_error) {
        error = "Could not inspect achievements.savegame: " + filesystem_error.message();
        return false;
    }
    if (!exists) { error.clear(); return true; }

    std::ifstream input(path, std::ios::binary);
    if (!input) { error = "Could not open achievements.savegame"; return false; }
    std::array<std::uint8_t, 16> payload{};
    input.read(reinterpret_cast<char*>(payload.data()),
               static_cast<std::streamsize>(payload.size()));
    if (input.gcount() != static_cast<std::streamsize>(payload.size()) ||
        input.peek() != std::char_traits<char>::eof()) {
        error = "achievements.savegame must contain exactly 16 source bitset bytes";
        return false;
    }
    return manager_.load_savegame_bits({payload.data(), payload.size()}, error);
}

bool OwnerV1::save_savegame(const std::string& runtime_directory,
                            std::string& error) const {
    if (!initialized() || runtime_directory.empty()) {
        error = "Trophy sidecar save requires the initialized manager and runtime directory";
        return false;
    }
    std::array<std::uint8_t, 16> payload{};
    if (!manager_.encode_savegame_bits(payload, error)) return false;
    const auto path = std::filesystem::path(runtime_directory) / "achievements.savegame";
    std::error_code filesystem_error;
    std::filesystem::create_directories(path.parent_path(), filesystem_error);
    if (filesystem_error) {
        error = "Could not create Trophy runtime directory: " + filesystem_error.message();
        return false;
    }
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) { error = "Could not open achievements.savegame for writing"; return false; }
    output.write(reinterpret_cast<const char*>(payload.data()),
                 static_cast<std::streamsize>(payload.size()));
    output.flush();
    if (!output) { error = "Could not write the complete achievements.savegame bitset"; return false; }
    error.clear();
    return true;
}

} // namespace dh2::native::trophies
