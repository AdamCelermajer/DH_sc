#include "native_level_savegame_publisher_v1.hpp"

#include <cstdio>

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

bool Publisher::publish(const std::string& filename,
                 const std::vector<std::uint8_t>& image,
                 std::string& error) const {
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
