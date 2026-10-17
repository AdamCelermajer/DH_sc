#include "../game_object_visual_attachment_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <vector>

namespace attach = dh2::game_object_visual_attachment_v1;
namespace object = dh2::object_manager_runtime_owner_v1;
namespace zoning = dh2::game_object_zoning_visibility;
namespace set_visible = dh2::game_object_set_visible;

void require(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "Visual attachment regression: %s\n", message);
        std::abort();
    }
}

struct Fixture { std::vector<std::uintptr_t> destroyed; };

int destroy_visual(void* raw, std::uintptr_t visual) {
    if (!raw || !visual) return 1;
    static_cast<Fixture*>(raw)->destroyed.push_back(visual);
    return 0;
}
int reject_destroy(void*, std::uintptr_t) { return 1; }

int main() {
    Fixture fixture;
    object::GameObject game{};
    game.identity = 0x100;
    std::uintptr_t live_visual = 0x10;
    game.live_fields.visual_object_2d8 = &live_visual;
    zoning::VisualObject rejected{0x20, game.identity};
    zoning::VisualObject replacement{0x30, game.identity};
    zoning::VisualObject foreign{0x40, 0x200};
    std::uintptr_t live_root_owner = 0;
    set_visible::SceneNode replacement_root{0x300, 0x301, 0};
    replacement_root.owner_game_object_204_live = &live_root_owner;
    set_visible::SceneNode foreign_root{0x400, 0x401, 0};
    set_visible::SceneNode providerless_root{0x500, 0x501, 0};
    const attach::Services services{&fixture, destroy_visual};
    attach::Result result{};
    std::string error;

    require(attach::set_visual_object(&game, {&rejected, nullptr}, &services,
            &result, error) == attach::Status::missing_scene_node,
        "candidate without a SceneNode was not source-rejected");
    require(live_visual == 0x10 && game.visual_object_2d8 == 0 &&
            fixture.destroyed.size() == 1 &&
            fixture.destroyed[0] == 0x20 && result.rejected_missing_scene_node,
        "failed construction did not destroy only the new visual and preserve old attachment");

    require(attach::set_visual_object(&game, {&replacement, &replacement_root}, &services,
            &result, error) == attach::Status::complete,
        "replacement attachment failed");
    require(fixture.destroyed.size() == 2 && fixture.destroyed[1] == 0x10 &&
            live_visual == 0x30 && game.visual_object_2d8 == 0 &&
            replacement.owner_identity == game.identity &&
            live_root_owner == game.identity &&
            replacement_root.owner_game_object_204 == 0 &&
            result.game_object_stores == 2 && result.root_owner_stores == 1,
        "replacement order or canonical owner backlink differs");

    require(attach::set_visual_object(&game, {&replacement, &replacement_root}, &services,
            &result, error) == attach::Status::complete &&
            fixture.destroyed.size() == 2 && live_visual == 0x30 &&
            result.game_object_stores == 0 &&
            replacement.owner_identity == game.identity,
        "same-pointer source attach performed replacement work");

    require(attach::set_visual_object(&game, {}, &services, &result, error) ==
            attach::Status::complete && live_visual == 0 &&
            fixture.destroyed.size() == 3 && fixture.destroyed[2] == 0x30 &&
            result.current_visual == 0,
        "source detach did not destroy and clear the attached visual");

    require(attach::set_visual_object(&game, {&foreign, &foreign_root}, &services,
            &result, error) == attach::Status::invalid_visual_owner &&
            live_visual == 0 && fixture.destroyed.size() == 3,
        "candidate linked to another GameObject was accepted");

    zoning::VisualObject without_provider{0x50, game.identity};
    require(attach::set_visual_object(&game, {&without_provider, &providerless_root},
            nullptr, &result, error) == attach::Status::invalid_argument &&
            live_visual == 0,
        "missing service bundle mutated the source GameObject");

    live_visual = 0x60;
    const attach::Services no_destructor{};
    require(attach::set_visual_object(&game, {}, &no_destructor, &result,
            error) == attach::Status::service_unavailable &&
            live_visual == 0x60,
        "missing old-visual destructor was not fail-closed");
    const attach::Services failing_destructor{nullptr, reject_destroy};
    require(attach::set_visual_object(&game, {}, &failing_destructor, &result,
            error) == attach::Status::service_failed &&
            live_visual == 0x60,
        "failed destructor provider changed source attachment");

    std::puts("{\"failed_scene_preserves_previous\":true,\"replace_destroys_old_first\":true,\"owner_backlink\":true,\"same_pointer_no_replace\":true,\"detach_destroys\":true,\"foreign_owner_rejected\":true,\"missing_destructor_fails_closed\":true,\"failed_destructor_preserves_attachment\":true}");
    return 0;
}
