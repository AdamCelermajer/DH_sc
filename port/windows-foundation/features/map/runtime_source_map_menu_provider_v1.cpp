#include "runtime_source_map_menu_provider_v1.hpp"

#include "runtime_source_map_actor_markers_v1.hpp"

namespace dh::foundation::map_page {
namespace {
bool fail(std::string& error, const char* reason) {
    error = reason;
    return false;
}
}

RuntimeSourceMapMenuProviderV1::RuntimeSourceMapMenuProviderV1(
    RuntimeSourceMapPageV1& page)
    : page_(&page) {
    callbacks_.open = [page = &page](std::string& error) {
        return page ? page->open(error)
                     : fail(error, "MapSheet provider: page is unavailable");
    };
    callbacks_.close = [page = &page](std::string& error) {
        return page ? page->close(error)
                     : fail(error, "MapSheet provider: page is unavailable");
    };
    callbacks_.visible = [page = &page] { return page && page->visible(); };
    callbacks_.frame = [page = &page](const AuthoredMapArtV1& art,
                              const map_ui::SourceMapCameraFrameV1& camera_frame,
                              const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,
                              int width, int height, SourceFrameV1& output,
                              std::string& error) {
        return page ? page->frame(art, camera_frame, camera, width, height,
                                    output, error)
                     : fail(error, "MapSheet provider: page is unavailable");
    };
    callbacks_.release = [page = &page](const AuthoredMapArtV1& art,
                                frontend::input::Point point,
                                int width, int height, PageInputV1& action,
                                std::string& error) {
        return page ? page->release(art, point, width, height, action, error)
                     : fail(error, "MapSheet provider: page is unavailable");
    };
    callbacks_.append_current_player_marker =
        [](const CombatSession& session, std::vector<SourceMarkerV1>& markers,
           std::string& error) {
            return source_map_append_current_player_marker_v1(session, markers, error);
        };
}

bool RuntimeSourceMapMenuProviderV1::register_with(
    const RegisterSourcePushedMenuV1& register_menu, std::string& error) {
    if (registered_)
        return fail(error, "MapSheet provider: exact pushed-menu symbol was already registered");
    if (!page_ || !register_menu)
        return fail(error, "MapSheet provider: page or pushed-menu registrar is unavailable");
    if (!register_menu(kSourceMapPushedMenuSymbolV1, callbacks_, error))
        return false;
    registered_ = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::map_page
