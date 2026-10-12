#pragma once
#include <level_source_loading_v43.hpp>
#include <canonical_lightset_file_v53.hpp>
#include <canonical_light_point_factory_v53.hpp>
#include <canonical_level_module_bindings_v2.hpp>
namespace model_renderer {
struct MainStage9ActualInputsV54 {
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> level;
 // Alias only the actual source manager inside its real retained owner facet.
 std::shared_ptr<dh2::world::CanonicalObjectManagerV1> manager;
 std::shared_ptr<dh2::world::CanonicalLevelModuleBindingsV2> modules;
 std::shared_ptr<void> actual_device_owner;
 dh2::loader::CanonicalLightsetFileInputsV53 files;
 // Main's weak-World/current renderer manager+candidate+application guard.
 std::function<bool(std::string&)> validate_scope;
};
class MainStage9LightsetBindingV54 final : public std::enable_shared_from_this<MainStage9LightsetBindingV54> {
 MainStage9ActualInputsV54 input_;std::unique_ptr<dh2::loader::Stage9BodyV53> body_;
 std::shared_ptr<dh2::loader::CanonicalLightsetFileV53> file_;
 std::uintptr_t config_id_{};std::int32_t config_key_{};
 bool current_config(std::shared_ptr<dh2::world::CanonicalLevelConfigV1>&,std::string&);
 bool validate(std::string&);
public:
 explicit MainStage9LightsetBindingV54(MainStage9ActualInputsV54 in):input_(std::move(in)){}
 dh2::loader::LifecycleStepV36 step(std::string&);
 static bool bind(MainStage9ActualInputsV54,dh2::loader::SourceLoadingInputsV43&,
  std::shared_ptr<MainStage9LightsetBindingV54>&,std::string&);
 const auto& actual_file()const noexcept{return file_;}
};
}
