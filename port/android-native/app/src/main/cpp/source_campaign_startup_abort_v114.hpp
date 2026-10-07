#pragma once
#include "source_campaign_retirement_v88.hpp"
#include <memory>
namespace dh2::android_ui {struct FrontSelectedProfileV50;}
namespace dh2::loader {
class NativeGSLevelRuntimeV27;class NativeGSLevelConnectionV26;
class NativeLevelConnectionV25;class CanonicalLevelContextV1;class LevelConstructorBindingsV4;
struct NativeGSLevelGlobalsV27;
}
namespace model_renderer {
struct SourceWorldBorrowV61;
struct SourceCampaignCandidateBorrowV55;
//SAME retained bootstrap prefix. Positive GS connection is borrowed under gs;
//other fields are original owners, never reconstructed publication/receivers.
struct SourceCampaignStartupPrefixV114 {
 std::shared_ptr<SourceWorldBorrowV61> world; //Genuine NULL if create_world failed.
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> selected;
 std::shared_ptr<dh2::loader::NativeGSLevelGlobalsV27> globals;
 std::shared_ptr<dh2::loader::NativeGSLevelRuntimeV27> gs;
 const dh2::loader::NativeGSLevelConnectionV26* gs_connection{};
 std::shared_ptr<dh2::loader::NativeLevelConnectionV25> level_connection;
 std::shared_ptr<dh2::loader::CanonicalLevelContextV1> allocated_level,published_level34;
 std::shared_ptr<dh2::loader::LevelConstructorBindingsV4> bindings;
 bool level_c1_complete{},has_loading{};
};
struct SourceCampaignStartupAbortServicesV114 {
 std::shared_ptr<void> owner; //Independent native process service packet.
 //Same actual owning-thread admission/draw/contact/menu scopes, no new barrier.
 std::function<SourceCampaignRetirementResultV88(const SourceCampaignStartupPrefixV114&,std::string&)> quiesce;
 //Authentic partial-C1/GS/Level qualified release. If ordinary release was
 //enrolled, use those SAME owners. Otherwise Main supplies actual reached
 //constructor-prefix leaves; no full D1 assumption/shared_ptr-expiry cleanup.
 //Complete means actual native slots/release prefixes finished, not readiness.
 std::function<SourceCampaignRetirementResultV88(const SourceCampaignStartupPrefixV114&,std::string&)> release_prefix;
 //Existing native render/menu/World aliases final-unpublication continuation.
 std::function<SourceCampaignRetirementResultV88(const SourceCampaignStartupPrefixV114&,std::string&)> retire_native;
};
//Only actual failed startup BEFORE source GS/loading slot adoption. Full
//adopted candidates continue to use existing V88 normal retirement.
bool borrow_source_campaign_startup_prefix_v114(SourceCampaignStartupPrefixV114&,std::string&);
bool borrow_source_campaign_startup_candidate_v114(const SourceCampaignStartupPrefixV114&,
 SourceCampaignCandidateBorrowV55&,std::string&);
bool begin_source_campaign_startup_abort_v114(SourceCampaignStartupAbortServicesV114,std::string&);
// Owns no replacement journal. Uses actual constructor cleanup before
// publication, or the already enrolled Level release for a published receiver.
SourceCampaignRetirementResultV88 release_source_campaign_startup_prefix_v114(
 const SourceCampaignStartupPrefixV114&,std::string&);
//Drain/retry through existing drain_source_campaign_retirement_v88 and explicit
//retry_source_campaign_retirement_v114; no separate lifecycle or destructor.
}
