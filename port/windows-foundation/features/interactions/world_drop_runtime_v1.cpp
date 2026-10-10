#include "world_drop_runtime_v1.hpp"
#include "../../renderer.hpp"

namespace dh::foundation::interactions {

struct WorldDropRuntimeV1::Impl {
    Renderer* renderer{};
    std::unique_ptr<SourceWorldItemDropMaterialBindingsV1> materials;
};

WorldDropRuntimeV1::WorldDropRuntimeV1() = default;
WorldDropRuntimeV1::~WorldDropRuntimeV1() = default;

bool WorldDropRuntimeV1::load(AssetCatalog& assets,
                              std::shared_ptr<loot::RuntimeWorldItemAdapterV1> store,
                              dh2::data::LootAudioVisualV8::Borrow audiovisual,
                              Renderer& renderer, std::string& error) {
    error.clear();
    renderer_.reset();
    impl_ = std::make_unique<Impl>();
    impl_->renderer = &renderer;
    SourceWorldItemDropTextureServicesV1 textures;
    textures.upload = [&renderer](const TextureImage& image, std::uint32_t& id, std::string& message) {
        id = renderer.createTexture(static_cast<int>(image.width), static_cast<int>(image.height),
                                    image.rgba.data());
        if (!id) message = "GPU texture upload failed";
        return id != 0;
    };
    textures.release = [&renderer](std::uint32_t id) { renderer.destroyTexture(id); };
    impl_->materials = std::make_unique<SourceWorldItemDropMaterialBindingsV1>(
        assets, std::move(textures), &assets);
    SourceWorldItemDropRenderServicesV1 services;
    auto* materials = impl_->materials.get();
    services.material = [materials](const SourceWorldItemDropMaterialV1& request, Material& output,
                                    std::string& message) {
        return materials->bind(request, output, message);
    };
    // Packets are drawn by main's RenderQueue from frame(); the projection's
    // own submit callback is never used.
    services.submit = [](std::shared_ptr<const SourceWorldItemDropRenderFrameV1>, std::string&) {
        return true;
    };
    if (!store) {
        error = "World-item store is required";
        return false;
    }
    return SourceWorldItemDropRenderV1::load(assets, *store, std::move(audiovisual),
                                             std::move(services), renderer_, error);
}

bool WorldDropRuntimeV1::prepare(std::string& error) {
    error.clear();
    if (!renderer_) {
        error = "World-item presentation is not loaded";
        return false;
    }
    frame_.reset();
    return renderer_->prepare(frame_, error);
}

std::vector<std::string> WorldDropRuntimeV1::resolved_visuals() const {
    return renderer_ ? renderer_->resolved_visuals() : std::vector<std::string>{};
}

const std::map<std::string, std::string>& WorldDropRuntimeV1::unresolved_visuals() const {
    static const std::map<std::string, std::string> none;
    return renderer_ ? renderer_->unresolved_visuals() : none;
}

} // namespace dh::foundation::interactions
