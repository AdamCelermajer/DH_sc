#include "../visual_node_queries_v1.hpp"

#include <cassert>
#include <cmath>
#include <string>

int main() {
    using namespace dh2;
    using namespace visual_node_queries_v1;

    scene::Scene first;
    scene::Node root;
    root.id = "root";
    root.name = "Root";
    root.world = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 10, 20, 30, 1};
    scene::Node socket;
    socket.id = "socket_id";
    socket.sid = "interaction_position";
    socket.name = "Socket";
    socket.parent = 0;
    socket.world = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 11, 22, 33, 1};
    first.graph = {root, socket};
    View first_view{101, &first};

    Identity by_name = 0;
    assert(get_specific_node(&first_view, "interaction_position", &by_name) ==
           Status::complete);
    Position position{7, 8, 9};
    assert(absolute_position(&first_view, by_name, &position) == Status::complete);
    assert(position.x == 11 && position.y == 22 && position.z == 33);

    Identity by_index = 0;
    assert(get_specific_node(&first_view, std::int32_t{1}, &by_index) ==
           Status::complete);
    assert(by_index == by_name);
    assert(get_specific_node(&first_view, std::int32_t{-1}, &by_index) ==
           Status::invalid_argument);
    assert(get_specific_node(&first_view, "missing", &by_index) == Status::not_found);

    scene::Scene other;
    other.graph.push_back(root);
    View other_view{202, &other};
    Position unchanged{3, 4, 5};
    assert(absolute_position(&other_view, by_name, &unchanged) == Status::stale_handle);
    assert(unchanged.x == 3 && unchanged.y == 4 && unchanged.z == 5);
    assert(get_specific_node(nullptr, "root", &by_index) == Status::invalid_argument);

    first.graph[1].world[12] = NAN;
    assert(absolute_position(&first_view, by_name, &unchanged) ==
           Status::invalid_transform);
    assert(unchanged.x == 3 && unchanged.y == 4 && unchanged.z == 5);
}
