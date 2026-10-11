#pragma once

#include "../../../../../../port/level-world/level_savegame_owner_v1.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>


namespace dh2::native::level_savegame_publisher_v1 {

// Platform file boundary for the independent LevelSavegame Savegame image.
// It publishes only the filename already produced by the source Level owner;
// it does not own, encode, or alias the PlayerSavegame profile.
class Publisher {
public:
    explicit Publisher(std::filesystem::path directory = {})
        : directory_(std::move(directory)) {}

    void bind_directory(std::filesystem::path directory) {
        directory_ = std::move(directory);
    }

    const std::filesystem::path& directory() const noexcept { return directory_; }

    bool publish(const std::string& filename,
                 const std::vector<std::uint8_t>& image,
                 std::string& error) const;

    static bool publish_callback(void* context, const std::string& filename,
                                 const std::vector<std::uint8_t>& image,
                                 std::string& error) {
        const auto* self = static_cast<const Publisher*>(context);
        if (!self) {
            error = "LevelSavegame publisher context is unavailable";
            return false;
        }
        return self->publish(filename, image, error);
    }

    level_savegame_owner_v1::PublishServices services() const noexcept {
        return {const_cast<Publisher*>(this), &publish_callback};
    }

private:
    std::filesystem::path directory_;
};

} // namespace dh2::native::level_savegame_publisher_v1
