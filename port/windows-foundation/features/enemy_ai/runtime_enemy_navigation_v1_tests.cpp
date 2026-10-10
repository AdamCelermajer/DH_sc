#include "runtime_enemy_navigation_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_navigation.hpp"
#include "../../../scene-materials/scene.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;

static void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

int main(int argc, char** argv) {
    try {
        require(argc == 2, "Supply original shared asset root");
        AssetCatalog assets(argv[1]);
        std::string error;
        CollisionScene scene;
        scene.floorInstances = 1;
        scene.collisionInstances = 1;
        scene.triangles.push_back({{0,-10,0},{10,-10,0},{10,10,0},true});
        scene.triangles.push_back({{5,-2,0},{5,2,0},{5,2,6},false});
        scene.triangles.push_back({{5,-2,0},{5,2,6},{5,-2,6},false});

        RuntimeEnemyNavigationConfigV1 config;
        config.scene_collision = &scene;
        RuntimeEnemyNavigationV1 navigation(std::move(config));
        bool visible = false;
        require(navigation.line_of_sight({0,0,3},{10,0,3},visible,error), error.c_str());
        require(!visible, "Authored _colbox_ triangle did not block the finite segment");
        require(navigation.line_of_sight({0,3,3},{10,3,3},visible,error), error.c_str());
        require(visible, "A ray outside authored collision was blocked");
        require(navigation.line_of_sight({5,0,3},{10,0,3},visible,error), error.c_str());
        require(visible, "Wall contact at the source endpoint was incorrectly treated as interior occlusion");

        scene.collisionInstances = 0;
        scene.triangles.resize(1);
        require(navigation.line_of_sight({0,0,3},{10,0,3},visible,error), error.c_str());
        require(visible, "Loaded floor-only geometry should leave LOS unobstructed");

        CollisionScene uninitialized;
        RuntimeEnemyNavigationConfigV1 missing_config;
        missing_config.scene_collision = &uninitialized;
        RuntimeEnemyNavigationV1 missing(std::move(missing_config));
        require(!missing.line_of_sight({0,0,0},{1,0,0},visible,error) && !error.empty(),
                "Uninitialized collision data must fail closed");

        const auto floor_bytes = assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");
        dh2::resources::BresView bres{};
        require(dh2_bres_open(&bres, floor_bytes.data(), floor_bytes.size()) ==
                    dh2::resources::BresError::ok,
                "Original source floor BRES did not open");
        dh2::scene::Scene floor_scene;
        require(dh2::scene::load(bres, floor_scene, error), error.c_str());
        unsigned floor_instance = UINT32_MAX;
        for (unsigned i=0; i<floor_scene.instances.size(); ++i) {
            const auto& node = floor_scene.graph[floor_scene.instances[i].node_index];
            if (node.name.find("floor") != std::string::npos) { floor_instance=i; break; }
        }
        require(floor_instance != UINT32_MAX, "Original source floor helper is absent");
        auto floors = std::make_shared<dh2::floors::World>();
        OriginalSourceFloorBinding floor_binding;
        floor_binding.instance = floor_instance;
        floor_binding.room = 0;
        floor_binding.mesh_local_quaternion = {0,0,0,1};
        floor_binding.mesh_local_scale = {1,1,1};
        require(append_original_module_floors(bres, floor_scene, {floor_binding}, *floors, error), error.c_str());
        require(dh2::floors::build_graph(*floors, error) && dh2::floors::post_load(*floors, error), error.c_str());
        require(floors->graph.edge_count != 0, "Original sewn source floor has no route edges");
        bool source_edge_slide_proved = false;
        for (const auto& record : floors->records) {
            for (const auto& triangle : record->triangles) {
                const float a[]{triangle.points[0][0],triangle.points[0][1],triangle.points[0][2]};
                const float b[]{triangle.points[1][0],triangle.points[1][1],triangle.points[1][2]};
                const float c[]{triangle.points[2][0],triangle.points[2][1],triangle.points[2][2]};
                const float ux=b[0]-a[0],uy=b[1]-a[1],uz=b[2]-a[2];
                const float vx=c[0]-a[0],vy=c[1]-a[1],vz=c[2]-a[2];
                const float nx=uy*vz-uz*vy,ny=uz*vx-ux*vz,nz=ux*vy-uy*vx;
                if (std::abs(nz)<1e-5f) continue;
                const float center_x=(a[0]+b[0]+c[0])/3.0f;
                const float center_y=(a[1]+b[1]+c[1])/3.0f;
                const float edges[3][2][3]{{{a[0],a[1],a[2]},{b[0],b[1],b[2]}},
                                            {{b[0],b[1],b[2]},{c[0],c[1],c[2]}},
                                            {{c[0],c[1],c[2]},{a[0],a[1],a[2]}}};
                for (const auto& edge : edges) {
                    const float mid_x=(edge[0][0]+edge[1][0])*0.5f;
                    const float mid_y=(edge[0][1]+edge[1][1])*0.5f;
                    float inward_x=center_x-mid_x,inward_y=center_y-mid_y;
                    const float inward_length=std::sqrt(inward_x*inward_x+inward_y*inward_y);
                    if (!(inward_length>0.01f)) continue;
                    inward_x/=inward_length; inward_y/=inward_length;
                    float position[]{mid_x+inward_x*0.1f,mid_y+inward_y*0.1f,0.0f};
                    position[2]=a[2]-(nx*(position[0]-a[0])+ny*(position[1]-a[1]))/nz;
                    float direction[]{-inward_x,-inward_y,0.0f};
                    const float before_x=direction[0],before_y=direction[1];
                    dh2::navigation::DirectionRequest request{
                        &floors->collision_world,position,0.0f,2,0};
                    unsigned valid=0;
                    if (dh2_nav_validate_direction(&valid,direction,&request)) continue;
                    if (direction[0]!=before_x || direction[1]!=before_y) {
                        source_edge_slide_proved=true;
                        break;
                    }
                }
                if (source_edge_slide_proved) break;
            }
            if (source_edge_slide_proved) break;
        }
        require(source_edge_slide_proved,
                "Original ValidateDirection did not slide a heading at an authored Swamp floor boundary");
        SourceNavigationWorldStorage storage(floors, 1);
        const auto& edge = floors->graph.edges[0];
        const auto& start = floors->graph.nodes[edge.from-1];
        const auto& finish = floors->graph.nodes[edge.to-1];
        dh2::navigation::NavigationObject source_pf{};
        require(dh2_nav_object_defaults(&source_pf) == 0, "Source PF defaults were rejected");
        dh2::navigation::PathObject path{};
        std::vector<dh2::navigation::PathSegment> segments(floors->graph.node_count+1);
        std::vector<std::uint32_t> search(floors->graph.node_count+1);
        path.route.flags = source_pf.motion.flags;
        path.route.radius = source_pf.radius;
        path.segments = segments.data();
        path.capacity = static_cast<std::uint32_t>(segments.size());
        std::copy(start.position, start.position+3, path.position);
        dh2::navigation::RouteResult route_result{};
        route_result.search.path = search.data();
        route_result.search.path_capacity = static_cast<std::uint32_t>(search.size());
        require(dh2::floors::find_path(*storage.floors, path, finish.position,
                    std::numeric_limits<unsigned>::max(), route_result) == 0,
                "Shared route kernel rejected an actual sewn-floor edge request");
        require(route_result.found && path.count != 0,
                "Actual sewn-floor neighboring nodes did not produce a source route");
        std::copy(start.position, start.position+3, path.position);
        dh2::navigation::MoveResult waypoint{};
        require(dh2_nav_move_path(&waypoint, &path, &storage.floors->graph) == 0,
                "Shared MovePath kernel rejected the source route");
        require(waypoint.target[0] == finish.position[0] &&
                waypoint.target[1] == finish.position[1] &&
                waypoint.target[2] == finish.position[2],
                "Shared MovePath did not preserve the route's authored final target");

        std::cout << "runtime enemy navigation PASS blocked=1 clear=2 source_floor_route=1 source_boundary_slide=1 missing_geometry=closed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime enemy navigation FAIL: " << ex.what() << '\n';
        return 1;
    }
}
