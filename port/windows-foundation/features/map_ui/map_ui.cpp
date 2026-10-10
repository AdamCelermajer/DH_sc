#include "map_ui.hpp"

namespace dh::foundation::map_ui {
namespace {
constexpr const char* kMapSwfSha256 =
    "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0";
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}
bool complete(const SourceMapBorrowV1& q) {
    return q.world && q.level && q.room_zones && q.camera && q.local_player &&
           q.character && q.save && q.quests && q.events && q.level_generation;
}
bool same_level(const SourceMapBorrowV1& a, const SourceMapBorrowV1& b) {
    return a.world.get() == b.world.get() && a.level.get() == b.level.get() &&
           a.level_generation == b.level_generation &&
           a.local_player.get() == b.local_player.get() &&
           a.character.get() == b.character.get() &&
           a.room_zones.get() == b.room_zones.get() &&
           a.camera.get() == b.camera.get() && a.save.get() == b.save.get() &&
           a.quests.get() == b.quests.get() && a.events.get() == b.events.get() &&
           a.local_player_index == b.local_player_index;
}
}

bool PresenterV1::borrow_current(SourceMapBorrowV1& out, std::string& error) const {
    if (!services_.borrow) return fail(error, "Map UI: required same-world live owner borrow service");
    SourceMapBorrowV1 next;
    if (!services_.borrow(next, error)) return false;
    if (!complete(next)) return fail(error,
        "Map UI: incomplete live Level/RoomZone/camera/player/Character/save/quest/event owners");
    out = std::move(next);
    error.clear();
    return true;
}

bool PresenterV1::ready(std::string& error) const {
    if (!services_.project || !services_.show || !services_.hide ||
        !services_.legend || !services_.reset_zoom || !services_.authored_resource)
        return fail(error, "Map UI: required source Show/RenderMap/camera/action/resource adapters");
    std::string hash;
    std::uint32_t sprite{};
    if (!services_.authored_resource(hash, sprite, error)) return false;
    if (hash != kMapSwfSha256 || sprite != 655)
        return fail(error, "Map UI: resource is not original dqcharmenu_droid.swf sprite655 menu_MapSheet");
    SourceMapBorrowV1 current;
    return borrow_current(current, error);
}

bool PresenterV1::show(std::string& error) {
    if (visible_) return fail(error, "Map UI: source Map.Show cannot replay while already visible");
    if (!ready(error)) return false;
    SourceMapBorrowV1 current;
    if (!borrow_current(current, error) || !services_.show(current, error)) return false;
    shown_ = std::move(current);
    visible_ = true;
    legend_visible_ = false;
    error.clear();
    return true;
}

bool PresenterV1::hide(std::string& error) {
    if (!visible_) return fail(error, "Map UI: source Map.Hide requires the shown Map owner");
    // Source Hide restores the camera/target captured by Show. Keep that
    // exact lease until the source hide callback completes.
    if (!services_.hide || !services_.hide(shown_, error)) return false;
    shown_ = {};
    visible_ = false;
    legend_visible_ = false;
    error.clear();
    return true;
}

bool PresenterV1::frame(character_menu::Frame& out, std::string& error) {
    if (!visible_) return fail(error, "Map UI: RenderMap projection requires visible source Map owner");
    SourceMapBorrowV1 current;
    if (!borrow_current(current, error)) return false;
    if (!same_level(shown_, current))
        return fail(error, "Map UI: live campaign owners changed during source Map lifetime");
    if (!services_.project) return fail(error, "Map UI: required exact source RenderMap projection");
    auto next = out;
    if (!services_.project(current, next, error)) return false;
    out = std::move(next);
    error.clear();
    return true;
}

bool PresenterV1::set_legend(bool visible, std::string& error) {
    if (!visible_) return fail(error, "Map UI: legend action requires visible source Map owner");
    SourceMapBorrowV1 current;
    if (!borrow_current(current, error)) return false;
    if (!same_level(shown_, current))
        return fail(error, "Map UI: live campaign owners changed during source Map lifetime");
    if (!services_.legend || !services_.legend(current, visible, error)) return false;
    legend_visible_ = visible;
    error.clear();
    return true;
}

bool PresenterV1::reset_zoom(std::string& error) {
    if (!visible_) return fail(error, "Map UI: ResetZoom requires visible source Map owner");
    SourceMapBorrowV1 current;
    if (!borrow_current(current, error)) return false;
    if (!same_level(shown_, current))
        return fail(error, "Map UI: live campaign owners changed during source Map lifetime");
    if (!services_.reset_zoom || !services_.reset_zoom(current, error)) return false;
    error.clear();
    return true;
}

} // namespace dh::foundation::map_ui
