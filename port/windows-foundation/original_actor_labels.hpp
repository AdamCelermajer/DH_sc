#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh::foundation {
class AssetCatalog;
struct OriginalActorLabelInput {
    // Original resolved Character property18 Name, property19 Level.
    std::int32_t name_id = -1, level_raw = -1;
    bool player = false, boss = false;
    // Delivered by the current player's save state, never a profile ID.
    std::string saved_player_name;
};
struct OriginalActorLabel { std::string name, level; std::int32_t numeric_level = -1; };
class OriginalActorLabels {
public:
    OriginalActorLabels();
    ~OriginalActorLabels();
    OriginalActorLabels(OriginalActorLabels&&) noexcept;
    OriginalActorLabels& operator=(OriginalActorLabels&&) noexcept;
    bool load(const AssetCatalog&, const std::string& data_root,
              std::int32_t language_pack, std::string& error);
    bool display_name(const OriginalActorLabelInput&, std::string&, std::string& error);
    bool label(const OriginalActorLabelInput&, OriginalActorLabel&, std::string& error);
    bool string_id(std::int32_t id, std::string&, std::string& error);
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
