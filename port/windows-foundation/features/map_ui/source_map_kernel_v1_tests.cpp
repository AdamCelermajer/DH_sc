#include "source_map_kernel_v1.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation::map_ui;
namespace {
void check(bool ok, int line) {
    if (!ok) throw std::runtime_error("map source-kernel test failed at line " + std::to_string(line));
}
#define CHECK(x) check((x), __LINE__)
bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
}

int main() {
    try {
        const MapCameraSourceConfigV1 source_camera;
        CHECK(std::string(source_camera.asset_path) == "data/3D/camera/minimapcameras.bdae");
        CHECK(std::string(source_camera.animation_set) == "Default");
        CHECK(std::string(source_camera.clip) == "PlayerCamera_Default_minimap");
        CHECK(near(source_camera.set_data_0, 0.42963f) && near(source_camera.set_data_1, 1.5f));
        CHECK(near(source_camera.set_data_2, 0) && near(source_camera.set_data_3, 100000));
        CHECK(near(source_camera.set_data_4, 0) && !source_camera.damping_enabled);
        CHECK(source_camera.camera_byte_133 == 1 && source_camera.camera_word_136 == 0);
        CHECK(near(source_camera.camera_float_140, 1));
        CHECK((source_camera.set_data_camera_vector == std::array<float, 3>{0, 0, 1}));
        CHECK((source_camera.post_set_data_camera_vector == std::array<float, 3>{-1, 1, 0}));
        CHECK(source_camera.initial_camera_virtual_byte_offset == 276);
        auto visited = std::make_shared<int>(1), hidden = std::make_shared<int>(2);
        std::vector<RoomZoneV1> zones{{visited, true}, {hidden, false}};
        int visible_inside = -1, visible_outside = -1;
        std::vector<MapObjectV1> objects{
            {std::make_shared<int>(3), {1, -2, 0, 3, 2, 4},
             [&](bool v, std::string&) { visible_inside = v; return true; }},
            {std::make_shared<int>(4), {8, 9, 0, 10, 11, 4},
             [&](bool v, std::string&) { visible_outside = v; return true;}}
        };
        RoomServicesV1 rooms;
        rooms.has_inside = [&](const std::shared_ptr<void>& room, const Point3V1& p,
                               bool& inside, std::string&) {
            CHECK(room.get() == visited.get());
            inside = p.x >= 0 && p.x <= 5 && p.y >= -3 && p.y <= 3;
            return true;
        };
        std::vector<std::shared_ptr<void>> visited_list, unvisited_list;
        MapBoundsV1 bounds{};
        std::string error;
        CHECK(source_map_show_bounds_v1(zones, objects, rooms, visited_list,
                                        unvisited_list, bounds, error));
        CHECK(visited_list.size() == 1 && visited_list.front() == visited);
        CHECK(unvisited_list.size() == 1 && unvisited_list.front() == hidden);
        CHECK(visible_inside == 1 && visible_outside == 0);
        // Source expands the union by one complete original span on each side.
        CHECK(near(bounds.min_x, -1) && near(bounds.max_x, 5));
        CHECK(near(bounds.min_y, -6) && near(bounds.max_y, 6));

        Point3V1 offset{20, -20, 7};
        CHECK(source_map_camera_offset_v1(bounds, {0, 0, 0}, offset, error));
        CHECK(near(offset.x, 5) && near(offset.y, -6) && near(offset.z, 7));
        offset = {-20, 20, -3};
        CHECK(source_map_camera_offset_v1(bounds, {0, 0, 0}, offset, error));
        CHECK(near(offset.x, -1) && near(offset.y, 6) && near(offset.z, -3));

        std::array<float, 2> point{};
        CHECK(source_map_screen_coord_v1({10, 110, 20, 220}, {1, -1, 0}, point, error));
        CHECK(near(point[0], 110) && near(point[1], 220));
        CHECK(source_map_screen_coord_v1({10, 110, 20, 220}, {-1, 1, 0}, point, error));
        CHECK(near(point[0], 10) && near(point[1], 20));
        Point3V1 received{};
        CHECK(source_map_project_marker_v1(
            [&](const Point3V1& world, Point3V1& screen, std::string&) {
                received = world;
                screen = {world.x * 0.5f, world.y * 0.25f, 0};
                return true;
            }, {2, -4, 17}, {10, 110, 20, 220}, point, error));
        CHECK(near(received.x, 2) && near(received.y, -4) && near(received.z, 17));
        CHECK(near(point[0], 110) && near(point[1], 220));

        SourceCameraMatrixV1 view{};
        view[0] = view[5] = view[10] = view[15] = 1.0f;
        view[12] = 10.0f; view[13] = 20.0f;
        SourceCameraMatrixV1 projection{};
        projection[0] = 2.0f; projection[5] = 3.0f;
        projection[10] = projection[15] = 1.0f;
        std::vector<std::uint32_t> modes;
        const SourceCameraMatricesV1 matrices = [&](std::uint32_t mode,
                SourceCameraMatrixV1& out, std::string&) {
            modes.push_back(mode);
            out = mode == 0 ? view : projection;
            return true;
        };
        Point3V1 screen{};
        CHECK(source_camera_base_screen_coord_v1(matrices, {1, 2, 3}, screen, error));
        CHECK(modes == std::vector<std::uint32_t>({0, 2}));
        CHECK(near(screen.x, 22) && near(screen.y, 66) && near(screen.z, 0));

        SourceCameraMatrixV1 zero_w = projection;
        zero_w[15] = 0.0f;
        zero_w[3] = zero_w[7] = zero_w[11] = 0.0f;
        const SourceCameraMatricesV1 singular = [&](std::uint32_t mode,
                SourceCameraMatrixV1& out, std::string&) {
            out = mode == 0 ? view : zero_w;
            return true;
        };
        CHECK(!source_camera_base_screen_coord_v1(singular, {1, 2, 3}, screen, error));

        auto camera_frame_lease = std::make_shared<int>(5);
        SourceMapCameraFrameV1 camera_frame;
        camera_frame.frame_lease = camera_frame_lease;
        camera_frame.generation = 7;
        camera_frame.expected_scene_manager = 0x1000;
        camera_frame.expected_camera = 0x2000;
        unsigned frame_checks = 0;
        camera_frame.read_active = [&](const std::shared_ptr<void>& lease,
                std::uint64_t generation, std::uintptr_t& manager,
                std::uintptr_t& camera, std::string&) {
            CHECK(lease == camera_frame_lease && generation == 7);
            manager = 0x1000; camera = 0x2000; ++frame_checks;
            return true;
        };
        CHECK(source_map_camera_frame_current_v1(camera_frame, error));
        CHECK(frame_checks == 1);
        camera_frame.read_active = [&](const std::shared_ptr<void>&,
                std::uint64_t, std::uintptr_t& manager,
                std::uintptr_t& camera, std::string&) {
            manager = 0x1001; camera = 0x2000;
            return true;
        };
        CHECK(!source_map_camera_frame_current_v1(camera_frame, error));
        camera_frame.expected_scene_manager = 0x1001;
        camera_frame.expected_camera = 0x2001;
        CHECK(!source_map_camera_frame_current_v1(camera_frame, error));

        auto old = point;
        CHECK(!source_map_screen_coord_v1({10, 110, 220, 20}, {0, 0, 0}, point, error));
        CHECK(point == old && !error.empty());
        std::cout << "MenuCharMenu_Map source bounds, camera clamp and icon screen projection passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
