#include "runtime_effects_renderer_v1.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::effects;

namespace {
unsigned checks{};
void check(bool ok, const char* reason) {
    ++checks;
    if (!ok) throw std::runtime_error(std::string("check ") +
                                      std::to_string(checks) + ": " + reason);
}
std::shared_ptr<const EffectRenderFrame> frame(std::shared_ptr<const void> owner) {
    auto result = std::make_shared<EffectRenderFrame>();
    EffectRenderPacket packet;
    packet.source.kind = EffectDrawKind::authored_mesh;
    packet.source.resource_uri = "data/3d/interface/source-authored-resource.bdae";
    packet.source.resource_bytes =
        std::make_shared<const std::vector<std::uint8_t>>(16, 0x5a);
    packet.source_retention = std::move(owner);
    packet.mesh.vertices.resize(3);
    packet.mesh.indices = {0, 1, 2};
    DrawRange range;
    range.indexCount = 3;
    range.material.sourcePass = SourceMaterialPass{};
    packet.mesh.ranges.push_back(range);
    result->packets.push_back(std::move(packet));
    return result;
}
}

int main() {
    try {
        Renderer renderer; // No WGL context; this test never invokes GPU methods.
        std::weak_ptr<const EffectRenderFrame> first_weak;
        std::weak_ptr<const void> first_owner_weak;
        {
            RuntimeEffectsRendererV1 queue(renderer);
            RuntimeEffectsFactoryBindingsV1 bindings;
            queue.bind_factory_services(bindings);
            check(bool(bindings.textures.upload) && bool(bindings.textures.release) &&
                  bool(bindings.submit), "factory received renderer-owned services");

            std::string error;
            auto first_owner = std::make_shared<const std::uint32_t>(7);
            first_owner_weak = first_owner;
            auto first = frame(first_owner);
            first_weak = first;
            first_owner.reset();
            check(bindings.submit(first, error), "exact source frame enqueue");
            check(queue.pending_frames() == 1 && queue.pending_packets() == 1,
                  "queue counts exact accepted frame and packet");
            first.reset();
            check(!first_weak.expired() && !first_owner_weak.expired(),
                  "queue retains exact frame and source packet lease");

            auto invalid = std::make_shared<EffectRenderFrame>();
            invalid->packets.emplace_back();
            check(!bindings.submit(invalid, error) && !error.empty(),
                  "malformed/unretained source packet rejected before Renderer");
            check(queue.pending_frames() == 1 && queue.pending_packets() == 1,
                  "rejected packet did not mutate retained queue");

            auto empty = std::make_shared<EffectRenderFrame>();
            check(bindings.submit(empty, error), "empty source frame accepted as no-op");
            check(queue.pending_frames() == 1 && queue.pending_packets() == 1,
                  "empty frame did not create a queue loan");
        }
        check(first_weak.expired() && first_owner_weak.expired(),
              "queue teardown releases frame and source lease when no GPU work ran");
        std::cout << "{\"validation\":\"PASS\",\"checks\":" << checks
                  << ",\"queued_packets\":1,\"gpu_calls\":0,\"wgl_created\":false}\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
