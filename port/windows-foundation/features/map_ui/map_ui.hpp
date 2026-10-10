#pragma once

#include "../character_menu/character_menu.hpp"
#include <functional>
#include <memory>
#include <string>

namespace dh::foundation::map_ui {

// A borrow receipt from the canonical campaign graph. Every handle is a
// lease to the existing owner; this feature never constructs a second Level,
// player, save, quest or event graph.
struct SourceMapBorrowV1 {
    std::shared_ptr<void> world;
    std::shared_ptr<void> level;
    std::shared_ptr<void> room_zones;
    std::shared_ptr<void> camera;
    std::shared_ptr<void> local_player;
    std::shared_ptr<void> character;
    std::shared_ptr<void> save;
    std::shared_ptr<void> quests;
    std::shared_ptr<void> events;
    std::uint64_t level_generation{};
    std::uint32_t local_player_index{};
};

struct ServicesV1 {
    // Must freshly borrow the SAME currently published campaign owners on
    // every call. A copied snapshot or fixture graph is not an implementation.
    std::function<bool(SourceMapBorrowV1&, std::string&)> borrow;
    // Exact original MenuCharMenu_Map/RenderMap projection. The callback owns
    // source room bounds/visitation, camera transform, marker and visibility
    // rules and may append only their actual projected output.
    std::function<bool(const SourceMapBorrowV1&,
                       character_menu::Frame&, std::string&)> project;
    // Source MenuCharMenu_Map camera lifecycle and action owners.
    std::function<bool(const SourceMapBorrowV1&, std::string&)> show;
    std::function<bool(const SourceMapBorrowV1&, std::string&)> hide;
    std::function<bool(const SourceMapBorrowV1&, bool, std::string&)> legend;
    std::function<bool(const SourceMapBorrowV1&, std::string&)> reset_zoom;
    // The SWF identity and sprite id are verified against the original movie
    // by the caller which owns its actual retained-resource lease.
    std::function<bool(std::string& sha256, std::uint32_t& map_sheet_sprite,
                       std::string&)> authored_resource;
};

class PresenterV1 {
    ServicesV1 services_;
    SourceMapBorrowV1 shown_;
    bool visible_{};
    bool legend_visible_{};

    bool borrow_current(SourceMapBorrowV1&, std::string&) const;
public:
    explicit PresenterV1(ServicesV1 services) : services_(std::move(services)) {}

    // Readiness checks the exact authored SWF identity and every source owner
    // required by Map.Show. Missing providers stay a hard failure.
    bool ready(std::string&) const;
    bool show(std::string&);
    bool hide(std::string&);
    bool frame(character_menu::Frame&, std::string&);
    bool set_legend(bool, std::string&);
    bool reset_zoom(std::string&);
    bool visible() const noexcept { return visible_; }
    bool legend_visible() const noexcept { return legend_visible_; }
};

} // namespace dh::foundation::map_ui
