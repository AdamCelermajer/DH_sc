#include "runtime_source_map_page_v1.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

namespace dh::foundation::map_page {
namespace {
constexpr const char* kMapSwfSha256 =
    "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0";

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool same_level(const map_ui::SourceMapBorrowV1& a,
                const map_ui::SourceMapBorrowV1& b) {
    return a.world.get() == b.world.get() && a.level.get() == b.level.get() &&
           a.room_zones.get() == b.room_zones.get() &&
           a.camera.get() == b.camera.get() &&
           a.local_player.get() == b.local_player.get() &&
           a.character.get() == b.character.get() && a.save.get() == b.save.get() &&
           a.quests.get() == b.quests.get() && a.events.get() == b.events.get() &&
           a.level_generation == b.level_generation &&
           a.local_player_index == b.local_player_index;
}

bool known_source_family(unsigned family) {
    switch (family) {
    case 0: case 1: case 2: case 3: case 4: case 7:
    case 10: case 11: case 12: case 14: case 15: case 16: case 17:
        return true;
    default:
        // Reserved families 5, 6, 8, 9, and 13 have no recovered source
        // producer. Unknown IDs likewise remain absent; no default glyph.
        return false;
    }
}

bool finite_position(const map_ui::Point3V1& p) {
    return std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z);
}

std::string tail_symbol(const std::string& path) {
    const auto separator = path.find_last_of("/.");
    return separator == std::string::npos ? path : path.substr(separator + 1);
}

bool authored_page(const AuthoredMapArtV1& art, std::array<float, 4>& render_rect,
                   std::string& error) {
    if (!art.screen || art.swf_sha256 != kMapSwfSha256 ||
        art.map_sheet_sprite != 655)
        return fail(error, "Map page: expected retained original Droid SWF MapSheet sprite655");
    if (art.screen->bitmap_ids.size() != art.screen->batches.size() ||
        art.screen->batch_colors.size() != art.screen->batches.size())
        return fail(error, "Map page: ScreenArt bitmap/color rows are not parallel to authored batches");

    bool found_render = false;
    bool found_legend = false;
    bool found_reset = false;
    for (const auto& region : art.screen->render_regions) {
        if (tail_symbol(region.path) != "RenderMap") continue;
        render_rect = region.bounds;
        found_render = true;
        break;
    }
    for (const auto& hit : art.screen->hit_regions) {
        const auto name = tail_symbol(hit.button_path);
        found_legend |= name == "btn_Legend";
        found_reset |= name == "btn_ResetZoom";
    }
    if (!found_render)
        return fail(error, "Map page: actual retained MapSheet RenderMap region is missing");
    if (!found_legend || !found_reset)
        return fail(error, "Map page: actual retained Legend/ResetZoom hit contours are missing");
    const auto& r = render_rect;
    if (!std::isfinite(r[0]) || !std::isfinite(r[1]) || !std::isfinite(r[2]) ||
        !std::isfinite(r[3]) || r[1] < r[0] || r[3] < r[2])
        return fail(error, "Map page: invalid authored RenderMap source rectangle");
    return true;
}

bool same_retained_resource(const map_ui::ServicesV1& services,
                            const AuthoredMapArtV1& art,
                            std::string& error) {
    if (!services.authored_resource)
        return fail(error, "Map page: retained original SWF identity provider is unavailable");
    std::string hash;
    std::uint32_t sprite{};
    if (!services.authored_resource(hash, sprite, error)) return false;
    if (hash != art.swf_sha256 || sprite != art.map_sheet_sprite)
        return fail(error, "Map page: supplied ScreenArt differs from the retained original SWF identity");
    return true;
}

const frontend::art::HitRegion* find_control(const frontend::art::ScreenArt& screen,
                                             const char* symbol,
                                             float x, float y) {
    for (auto it = screen.hit_regions.rbegin(); it != screen.hit_regions.rend(); ++it) {
        if (tail_symbol(it->button_path) == symbol &&
            frontend::art::contains(*it, x, y))
            return &*it;
    }
    return nullptr;
}
} // namespace

struct RuntimeSourceMapPageV1::Impl {
    explicit Impl(ServicesV1 value)
        : services(std::move(value)), presenter(services.source_page) {}

    ServicesV1 services;
    map_ui::PresenterV1 presenter;
};

RuntimeSourceMapPageV1::RuntimeSourceMapPageV1(ServicesV1 services)
    : impl_(std::make_unique<Impl>(std::move(services))) {}

RuntimeSourceMapPageV1::~RuntimeSourceMapPageV1() = default;

bool RuntimeSourceMapPageV1::open(std::string& error) {
    return impl_->presenter.show(error);
}

bool RuntimeSourceMapPageV1::close(std::string& error) {
    return impl_->presenter.hide(error);
}

bool RuntimeSourceMapPageV1::visible() const noexcept {
    return impl_->presenter.visible();
}

bool RuntimeSourceMapPageV1::frame(
    const AuthoredMapArtV1& art, const map_ui::SourceMapCameraFrameV1& camera_frame,
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,
    int width, int height, SourceFrameV1& output, std::string& error) {
    if (!visible()) return fail(error, "Map page: frame requested while the source page is closed");
    if (width <= 0 || height <= 0)
        return fail(error, "Map page: viewport must have positive dimensions");
    std::array<float, 4> rect{};
    if (!authored_page(art, rect, error) ||
        !same_retained_resource(impl_->services.source_page, art, error)) return false;
    if (!camera || camera_frame.expected_camera !=
            reinterpret_cast<std::uintptr_t>(camera.get()))
        return fail(error, "Map page: camera runtime differs from the retained source frame identity");

    character_menu::Frame native_page;
    if (!impl_->presenter.frame(native_page, error)) return false;

    map_ui::SourceMapBorrowV1 owner;
    if (!impl_->services.source_page.borrow ||
        !impl_->services.source_page.borrow(owner, error))
        return false;
    if (!impl_->services.collect_markers)
        return fail(error, "Map page: live source marker producers are unavailable");

    std::vector<SourceMarkerV1> source_markers;
    if (!impl_->services.collect_markers(owner, source_markers, error)) return false;

    SourceFrameV1 next;
    next.character_menu = std::move(native_page);
    next.render_map_rect = rect;
    next.level_generation = owner.level_generation;
    next.markers.reserve(source_markers.size());
    for (const auto& marker : source_markers) {
        if (!known_source_family(marker.family)) continue;
        if (!marker.owner || !finite_position(marker.world_position))
            return fail(error, "Map page: source marker lacks its live owner or finite source position");
        std::array<float, 2> projected{};
        if (!map_ui::source_map_project_runtime_frame_v1(
                camera_frame, camera, marker.world_position, rect, projected, error))
            return false;
        ProjectedMarkerV1 result;
        result.owner = marker.owner;
        result.source_id = marker.source_id;
        result.family = marker.family;
        result.world_position = marker.world_position;
        result.authored_screen_position = projected;
        next.markers.push_back(std::move(result));
    }

    // Reborrow after projection: do not publish a mixed Level/camera frame if
    // the campaign changed while source marker owners were sampled.
    map_ui::SourceMapBorrowV1 after;
    if (!impl_->services.source_page.borrow(after, error)) return false;
    if (!same_level(owner, after))
        return fail(error, "Map page: campaign owner lease changed while projecting source markers");
    if (!map_ui::source_map_camera_frame_current_v1(camera_frame, error)) return false;

    output = std::move(next);
    error.clear();
    return true;
}

bool RuntimeSourceMapPageV1::release(
    const AuthoredMapArtV1& art, frontend::input::Point point,
    int width, int height, PageInputV1& output, std::string& error) {
    if (!visible()) return fail(error, "Map page: release received while source page is closed");
    if (width <= 0 || height <= 0)
        return fail(error, "Map page: release viewport must have positive dimensions");
    std::array<float, 4> unused{};
    if (!authored_page(art, unused, error) ||
        !same_retained_resource(impl_->services.source_page, art, error)) return false;
    // Match the frontend's current source_point inverse: the retained CUI
    // FlashCamera stretches the 480x320 logical stage independently per axis.
    const std::array<float, 2> source_point{
        static_cast<float>(point.x) * 480.0f / static_cast<float>(width),
        static_cast<float>(point.y) * 320.0f / static_cast<float>(height)};

    PageInputV1 action = PageInputV1::none;
    if (find_control(*art.screen, "btn_Legend", source_point[0], source_point[1])) {
        if (!impl_->presenter.set_legend(!impl_->presenter.legend_visible(), error)) return false;
        action = PageInputV1::legend;
    } else if (find_control(*art.screen, "btn_ResetZoom", source_point[0], source_point[1])) {
        if (!impl_->presenter.reset_zoom(error)) return false;
        action = PageInputV1::reset_zoom;
    }
    output = action;
    error.clear();
    return true;
}

} // namespace dh::foundation::map_page
