#pragma once
#include <canonical_cached_file_v1.hpp>
#include "level_lightset_stage9_v53.hpp"
namespace dh2::loader {
struct CanonicalLightsetFileInputsV53 {
 assets::ZipAssetPackV1 archive;
 std::shared_ptr<CanonicalLevelContextV1> level;
 std::shared_ptr<void> candidate_owner;
 world::CanonicalObjectManagerV1* manager{};
 world::CanonicalClassServicesV1 classes;CanonicalFileSourceServicesV1 source;
 std::shared_ptr<void> actual_filesystem_owner;
 std::function<bool(const std::string&,std::string&,std::string&)> resolve_filename;
 std::function<bool(std::string&)> validate_current;
};
// Same parser/factory/registry producer as root/module load, one retained XML
// occurrence. Actual LightPoint dispatch is REQUIRED; never skip gametype19.
class CanonicalLightsetFileV53 final {
 CanonicalLightsetFileInputsV53 input_;std::unique_ptr<CanonicalCachedFileV1> file_;
 CanonicalLevelContextV1::ModuleLoadFields fields_;std::string raw_,uri_,error_;
 std::array<float,3> position_{};std::int32_t room_{};bool started_{},failed_{},busy_{};
public:
 explicit CanonicalLightsetFileV53(CanonicalLightsetFileInputsV53);
 LifecycleStepV36 step(const std::string&,const char*,std::string&);
 const CanonicalCachedFileV1* actual_file()const noexcept{return file_.get();}
 bool discard_after_owner_release(const std::function<bool(std::string&)>&,std::string&);
};
}
