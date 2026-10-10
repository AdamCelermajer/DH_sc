#pragma once

#include "runtime_source_map_page_v1.hpp"
#include "../../combat_session.hpp"

#include <functional>

namespace dh::foundation::map_page {

inline constexpr const char* kSourceMapPushedMenuSymbolV1 = "menu_MapSheet";

// Callable surface installed for the exact authored NativePushMenu symbol.
// The registry/caller owns the page and must not retain these callbacks beyond
// that page's lifetime.
struct SourceMapPushedMenuCallbacksV1 {
    std::function<bool(std::string&)> open;
    std::function<bool(std::string&)> close;
    std::function<bool()> visible;
    std::function<bool(const AuthoredMapArtV1&,
                       const map_ui::SourceMapCameraFrameV1&,
                       const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,
                       int, int, SourceFrameV1&, std::string&)> frame;
    std::function<bool(const AuthoredMapArtV1&, frontend::input::Point,
                       int, int, PageInputV1&, std::string&)> release;

    // This is deliberately an explicit same-session seam. The caller supplies
    // the CombatSession that owns the visible gameplay frame; the adapter does
    // not infer an alias to SourceMapBorrowV1.local_player.
    std::function<bool(const CombatSession&, std::vector<SourceMarkerV1>&,
                       std::string&)> append_current_player_marker;
};

using RegisterSourcePushedMenuV1 = std::function<bool(
    const char* exact_symbol, const SourceMapPushedMenuCallbacksV1&,
    std::string& error)>;

// Feature-owned binding from the source-authored pushed-menu symbol to the
// existing Map page leaf. It does not create missing Level/RoomZone/camera
// services or own/advance a gameplay session.
class RuntimeSourceMapMenuProviderV1 {
    RuntimeSourceMapPageV1* page_{};
    bool registered_{};
    SourceMapPushedMenuCallbacksV1 callbacks_;

public:
    explicit RuntimeSourceMapMenuProviderV1(RuntimeSourceMapPageV1& page);

    bool register_with(const RegisterSourcePushedMenuV1&,
                       std::string& error);
    bool registered() const noexcept { return registered_; }
};

} // namespace dh::foundation::map_page
