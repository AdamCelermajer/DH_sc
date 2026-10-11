#include "native_level_savegame_publisher_v1.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace dh2::native::level_savegame_publisher_v1 {

bool Publisher::read(const std::string& filename,
                     std::vector<std::uint8_t>& image,
                     std::string& error) const {
    return read_named(filename, "_level.savegame", image, error);
}

bool Publisher::read_checkpoint(const std::string& filename,
                                std::vector<std::uint8_t>& image,
                                std::string& error) const {
    return read_named(filename, "_single_level.checkpoint", image, error);
}

bool Publisher::read_player_checkpoint(const std::string& filename,
                                       std::vector<std::uint8_t>& image,
                                       std::string& error) const {
    return read_named(filename, "_single.checkpoint", image, error);
}

bool Publisher::read_named(const std::string& filename, const char* suffix,
                           std::vector<std::uint8_t>& image,
                           std::string& error) const {
    constexpr std::uintmax_t max_image_bytes = 16u * 1024u * 1024u;
    if (!suffix || directory_.empty() || !directory_.is_absolute() || filename.empty() ||
        filename.find_first_of("/\\") != std::string::npos ||
        filename.find('\0') != std::string::npos || filename.size() < std::strlen(suffix) ||
        filename.compare(filename.size() - std::strlen(suffix), std::strlen(suffix), suffix) != 0) {
        error = "LevelSavegame reader requires the bound private directory and exact source filename";
        return false;
    }
    std::error_code ec;
    const auto target = directory_ / std::filesystem::u8path(filename);
    const auto bound_directory = std::filesystem::weakly_canonical(directory_, ec);
    if (ec) {
        error = "LevelSavegame bound directory could not be resolved";
        return false;
    }
    const auto resolved_target = std::filesystem::weakly_canonical(target, ec);
    if (ec || resolved_target.parent_path() != bound_directory ||
        !std::filesystem::is_regular_file(resolved_target, ec) || ec) {
        error = "LevelSavegame selected file is absent or outside its bound directory";
        return false;
    }
    const auto size = std::filesystem::file_size(resolved_target, ec);
    if (ec || size == 0 || size > max_image_bytes ||
        size > std::numeric_limits<std::size_t>::max()) {
        error = "LevelSavegame selected file is empty or exceeds the bounded reader";
        return false;
    }
    std::ifstream file(resolved_target, std::ios::binary);
    if (!file) {
        error = "LevelSavegame selected file could not be opened";
        return false;
    }
    std::vector<std::uint8_t> staged(static_cast<std::size_t>(size));
    if (!file.read(reinterpret_cast<char*>(staged.data()),
                   static_cast<std::streamsize>(staged.size())) ||
        file.peek() != std::char_traits<char>::eof()) {
        error = "LevelSavegame selected file changed or was truncated while reading";
        return false;
    }
    image = std::move(staged);
    error.clear();
    return true;
}

bool Publisher::publish(const std::string& filename,
                 const std::vector<std::uint8_t>& image,
                 std::string& error) const {
        if (preserve_existing_image_) {
            error = "LevelSavegame replacement is blocked while saved non-Player rows remain unapplied";
            return false;
        }
        if (directory_.empty() || !directory_.is_absolute() || filename.empty() ||
            filename.find_first_of("/\\") != std::string::npos ||
            filename.find('\0') != std::string::npos ||
            filename.size() < 15 ||
            filename.compare(filename.size() - 15, 15, "_level.savegame") != 0) {
            error = "LevelSavegame publisher requires the bound private directory and source filename";
            return false;
        }

        const auto target = directory_ / std::filesystem::u8path(filename);
        const auto temporary = std::filesystem::path(target.string() + ".writing");
        std::FILE* file = std::fopen(temporary.string().c_str(), "wb");
        if (!file) {
            error = "LevelSavegame temporary file open failed";
            return false;
        }
        bool okay = image.empty() ||
            std::fwrite(image.data(), 1, image.size(), file) == image.size();
        okay = okay && std::fflush(file) == 0;
#ifdef _WIN32
        if (okay) okay = ::_commit(::_fileno(file)) == 0;
#else
        if (okay) okay = ::fsync(::fileno(file)) == 0;
#endif
        if (std::fclose(file) != 0) okay = false;
        if (!okay) {
            std::error_code ignored;
            std::filesystem::remove(temporary, ignored);
            error = "LevelSavegame temporary file write or sync failed";
            return false;
        }

#ifdef _WIN32
        if (!::MoveFileExW(temporary.c_str(), target.c_str(),
                           MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
            std::error_code ignored;
            std::filesystem::remove(temporary, ignored);
            error = "LevelSavegame file replacement failed";
            return false;
        }
#else
        std::error_code ec;
        std::filesystem::rename(temporary, target, ec);
        if (ec) {
            std::error_code ignored;
            std::filesystem::remove(temporary, ignored);
            error = "LevelSavegame file replacement failed: " + ec.message();
            return false;
        }
        const auto parent = target.parent_path();
        const int directory_fd = ::open(parent.c_str(), O_RDONLY);
        if (directory_fd < 0) {
            error = "LevelSavegame directory open for sync failed";
            return false;
        }
        const bool directory_synced = ::fsync(directory_fd) == 0;
        const int close_result = ::close(directory_fd);
        if (!directory_synced || close_result != 0) {
            error = "LevelSavegame directory sync failed";
            return false;
        }
#endif
        error.clear();
        return true;
    }

} // namespace dh2::native::level_savegame_publisher_v1
