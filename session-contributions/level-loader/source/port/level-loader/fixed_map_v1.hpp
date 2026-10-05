#pragma once
#include "fixed_sources_v1.hpp"
#include "visual_transform_v1.hpp"
#include "../scene-materials/scene.hpp"
#include "../level-world/floors.hpp"
#include <map>
namespace dh2::loader {
// Internal assembly stage; this is not an agreed gameplay/factory/save ABI.
// BRES views always retain their immutable source buffer through owner.
struct MapAssetV1 {
    std::string uri;
    std::shared_ptr<const std::vector<std::uint8_t>> owner;
    resources::BresView view{};
    scene::Scene source;
    std::uint32_t material_offset{};
};
struct MapModuleV1 {
    std::uint32_t document{},element{},asset{},root{},parent{no_source_v1};
    std::string authored_name,xrefobject;
    VisualTransformV1 transform;
    // Retained independently of parsed source defaults. Main evaluates these
    // before publishing runtime objects; assembly does not activate modules.
    std::string activate_cond,condition_desc;
    bool authored_visible{true};
};
enum class MapGeometryKindV1 {mesh,floor,exit,minimap,module_root,unclassified};
struct MapInstanceV1 {
    std::uint32_t scene_instance{},asset{},module{};
    MapGeometryKindV1 kind{MapGeometryKindV1::unclassified};
};
struct MapFloorMetadataV1 {
    std::uint32_t scene_instance{},floor_record{};
    std::map<std::string,std::string> properties;
    floor_source::Flags flags{0,1};
};
class FixedMapV1 {
    struct Snapshot;
    std::shared_ptr<const Snapshot> snapshot_;
public:
    class Borrow {
        friend class FixedMapV1;
        std::shared_ptr<const Snapshot> snapshot_;
        explicit Borrow(std::shared_ptr<const Snapshot> p):snapshot_(std::move(p)){}
    public:
        Borrow()=default;
        explicit operator bool()const noexcept{return bool(snapshot_);}
        const FixedSourcesV1::Borrow& sources()const;
        const std::vector<MapAssetV1>& assets()const;
        const std::vector<MapModuleV1>& modules()const;
        const scene::Scene& scene()const;
        const std::vector<MapInstanceV1>& instances()const;
        const floors::World& navigation()const;
        const std::vector<MapFloorMetadataV1>& floor_metadata()const;
    };
    // Prepare owned module geometry/materials and feed authored floors into the
    // existing native floor/navigation adapter. Failed candidates preserve the
    // previous map. Other declarations remain retained in sources, uncreated.
    bool prepare(const assets::ZipAssetPackV1&,FixedSourcesV1::Borrow,std::string&);
    Borrow borrow()const{return Borrow(snapshot_);}
};
}
