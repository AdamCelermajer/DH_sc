#include "visual_node_queries_v1.hpp"

#include <cmath>
namespace dh2::visual_node_queries_v1 {
namespace {

bool valid(const View* view) noexcept {
    return view && view->visual && view->scene;
}

Identity handle(const View& view, std::size_t index) noexcept {
    return reinterpret_cast<Identity>(&view.scene->graph[index]);
}

bool matches(const scene::Node& node, const char* name) noexcept {
    return node.name == name || node.id == name || node.sid == name;
}

} // namespace

Status get_specific_node(const View* view, const char* name,
                         Identity* node_out) noexcept {
    if (!valid(view) || !name || !*name || !node_out)
        return Status::invalid_argument;
    for (std::size_t index = 0; index < view->scene->graph.size(); ++index) {
        if (!matches(view->scene->graph[index], name)) continue;
        *node_out = handle(*view, index);
        return Status::complete;
    }
    return Status::not_found;
}

Status get_specific_node(const View* view, std::int32_t index,
                         Identity* node_out) noexcept {
    if (!valid(view) || !node_out || index < 0)
        return Status::invalid_argument;
    if (static_cast<std::size_t>(index) >= view->scene->graph.size())
        return Status::not_found;
    *node_out = handle(*view, static_cast<std::size_t>(index));
    return Status::complete;
}

Status absolute_position(const View* view, Identity node,
                         Position* output) noexcept {
    if (!valid(view) || !node || !output)
        return Status::invalid_argument;
    const scene::Node* found = nullptr;
    for (const auto& candidate : view->scene->graph)
        if (reinterpret_cast<Identity>(&candidate) == node) {
            found = &candidate;
            break;
        }
    if (!found) return Status::stale_handle;
    const auto& matrix = found->world;
    Position result{matrix[12], matrix[13], matrix[14]};
    if (!std::isfinite(result.x) || !std::isfinite(result.y) ||
        !std::isfinite(result.z))
        return Status::invalid_transform;
    *output = result;
    return Status::complete;
}

} // namespace dh2::visual_node_queries_v1
