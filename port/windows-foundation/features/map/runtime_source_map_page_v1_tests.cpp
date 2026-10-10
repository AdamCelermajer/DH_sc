#include "runtime_source_map_page_v1.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>

using namespace dh::foundation;
namespace dh::foundation::frontend::art {
const ScreenArt& original_art(Screen) noexcept {
    static const ScreenArt empty;
    return empty;
}
}
namespace dh::foundation::map_ui {
bool source_map_project_runtime_frame_v1(
    const SourceMapCameraFrameV1&,
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,
    const Point3V1&, const std::array<float, 4>&,
    std::array<float, 2>&, std::string& error) {
    error = "projection must use the actual linked camera adapter";
    return false;
}
}
namespace {
void check(bool ok, int line) {
    if (!ok) throw std::runtime_error("source map page test failed at line " +
                                      std::to_string(line));
}
#define CHECK(x) check((x), __LINE__)

frontend::art::HitRegion triangle(const char* path, float left, float top) {
    frontend::art::HitRegion hit;
    hit.button_path = path;
    hit.triangles = {{left, top, 0, 0}, {left + 10, top, 0, 0},
                     {left, top + 10, 0, 0}};
    return hit;
}
}

int main() {
    try {
        std::string error;
        unsigned show_count = 0, hide_count = 0, legend_count = 0, reset_count = 0;
        bool last_legend = false;
        auto world = std::make_shared<int>(1);
        auto level = std::make_shared<int>(2);
        auto rooms = std::make_shared<int>(3);
        auto camera = std::make_shared<int>(4);
        auto player = std::make_shared<int>(5);
        auto character = std::make_shared<int>(6);
        auto save = std::make_shared<int>(7);
        auto quests = std::make_shared<int>(8);
        auto events = std::make_shared<int>(9);

        map_page::ServicesV1 services;
        services.source_page.borrow = [&](map_ui::SourceMapBorrowV1& out,
                                           std::string&) {
            out = {world, level, rooms, camera, player, character, save, quests,
                   events, 41, 0};
            return true;
        };
        services.source_page.project = [](const map_ui::SourceMapBorrowV1&,
                                          character_menu::Frame&, std::string&) {
            return true;
        };
        services.source_page.show = [&](const map_ui::SourceMapBorrowV1&,
                                         std::string&) { ++show_count; return true; };
        services.source_page.hide = [&](const map_ui::SourceMapBorrowV1&,
                                         std::string&) { ++hide_count; return true; };
        services.source_page.legend = [&](const map_ui::SourceMapBorrowV1&, bool value,
                                           std::string&) {
            ++legend_count;
            last_legend = value;
            return true;
        };
        services.source_page.reset_zoom = [&](const map_ui::SourceMapBorrowV1&,
                                               std::string&) {
            ++reset_count;
            return true;
        };
        services.source_page.authored_resource = [](std::string& hash,
                                                     std::uint32_t& sprite,
                                                     std::string&) {
            hash = "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0";
            sprite = 655;
            return true;
        };
        services.collect_markers = [](const map_ui::SourceMapBorrowV1&,
                                      std::vector<map_page::SourceMarkerV1>& out,
                                      std::string&) { out.clear(); return true; };

        map_page::RuntimeSourceMapPageV1 page(std::move(services));
        CHECK(page.open(error));
        CHECK(page.visible() && show_count == 1);
        CHECK(!page.open(error));

        frontend::art::ScreenArt screen;
        screen.render_regions.push_back({"menu_MapSheet.RenderMap", {100, 300, 30, 190}});
        screen.hit_regions.push_back(triangle("menu_MapSheet.btn_Legend", 10, 10));
        screen.hit_regions.push_back(triangle("menu_MapSheet.btn_ResetZoom", 30, 10));
        map_page::AuthoredMapArtV1 art{
            &screen,
            "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0",
            655};

        map_page::PageInputV1 action = map_page::PageInputV1::legend;
        CHECK(page.release(art, {12, 12}, 480, 320, action, error));
        CHECK(action == map_page::PageInputV1::legend);
        CHECK(legend_count == 1 && last_legend);
        CHECK(page.release(art, {32, 12}, 480, 320, action, error));
        CHECK(action == map_page::PageInputV1::reset_zoom && reset_count == 1);
        CHECK(page.release(art, {100, 200}, 480, 320, action, error));
        CHECK(action == map_page::PageInputV1::none);

        CHECK(page.close(error));
        CHECK(!page.visible() && hide_count == 1);
        CHECK(!page.release(art, {12, 12}, 480, 320, action, error));
        CHECK(!page.close(error));
        std::cout << "source Map page lifecycle and authored controls: PASS\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
