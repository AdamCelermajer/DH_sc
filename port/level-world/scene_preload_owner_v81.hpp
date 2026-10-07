#pragma once
#include "../scene-materials/scene.hpp"
#include "../engine-skinning/skinning.hpp"
#include "../engine-resources/admitted_cpu_bytes_v40.hpp"
#include <functional>
namespace dh2::world {
struct ScenePreloadAssetsV81 {
 std::shared_ptr<void> provider;
 std::shared_ptr<resources::ContextResourceBudgetV37> budget;
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,
  const std::function<bool(std::uint32_t,std::string&)>&,std::string&)> read;
 //Native engineering ceilings; not original game constants/whole heap bound.
 std::uint32_t maximum_roots=4096;
 std::uint64_t maximum_encoded_bytes=64*resources::mib_v37;
};
struct ScenePreloadedRootFieldsV81 {
 //RootSceneNodeC1 stores35d874..35d8f8, followed by LoadScene35976c..3597ec.
 std::string file1bc,xref1d4;
 std::uint8_t byte208{1},byte20a{},displacement1ec{},byte200{1},byte209{};
 std::array<float,4> displacement1f0_1fc{};
 std::uintptr_t game_object204{},parentec{};
 std::uint32_t automatic_culling118{2};
};
//CPU/native scene resource successor, not a GameObject/Character/VisualObject.
//NULLxref constructs the whole authored root and never installs an animator,
//resets root from file, adds visible membership, or advances a scene clock.
class ScenePreloadedRootV81 {
 friend class ScenePreloadCollectionV81;
 std::shared_ptr<resources::AdmittedVectorV40> bytes_;
 resources::BresView image_{};
 scene::Scene scene_;
 scene::AuthoredVisibilityV76 visibility_;
 std::vector<skinning::Skin> skins_;
 ScenePreloadedRootFieldsV81 fields_;
 bool complete_{};
public:
 std::uintptr_t identity()const noexcept{return reinterpret_cast<std::uintptr_t>(this);}
 const auto& fields()const noexcept{return fields_;}
 const auto& scene()const noexcept{return scene_;}
 const auto& visibility()const noexcept{return visibility_;}
 const auto& skins()const noexcept{return skins_;}
 const auto& image()const noexcept{return image_;}
 bool complete()const noexcept{return complete_;}
};
class ScenePreloadCollectionV81 {
 ScenePreloadAssetsV81 assets_;
 //Original vector44c contains only successfully returned positive roots.
 std::vector<std::shared_ptr<ScenePreloadedRootV81>> roots_;
 //Keep the native failed construction prefix rather than replaying allocation.
 std::shared_ptr<ScenePreloadedRootV81> failed_prefix_;
 bool busy_{},failed_{};
 std::string failure_;
public:
 explicit ScenePreloadCollectionV81(ScenePreloadAssetsV81 s):assets_(std::move(s)){}
 bool preload(const std::string& filename,std::string&);
 void clear_at_scene_manager_clear()noexcept;
 const auto& source_roots44c()const noexcept{return roots_;}
 const auto& failed_prefix()const noexcept{return failed_prefix_;}
};
}
