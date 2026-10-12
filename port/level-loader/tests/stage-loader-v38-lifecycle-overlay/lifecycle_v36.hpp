#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh2::loader {
struct LifecycleStageV36 { std::int32_t state;std::uint32_t original_entry;const char* service;bool source_has_body; };
const std::array<LifecycleStageV36,38>& lifecycle_stages_v36() noexcept;
// These pointers MUST alias the retained actual Level receiver. No copied
// progress field, secondary Level global, campaign authority or ready flag.
struct LifecycleFieldsV36 {
 std::uint32_t *progress30{},*state130{},*counter134{},*current138{};
};
// Original scalar tail 3f6e9c..3f6ef4 only. Does not invoke UI/license callbacks.
void lifecycle_progress_tail_v36(LifecycleFieldsV36 fields) noexcept;
enum class LifecycleStepV36 { pending,complete,dependency_missing,failed };
enum class LifecycleStatusV36 { loading,awaiting_end_loading,source_finished,cancelling,cancelled,failed };
struct LifecycleResourceCountsV36 { std::uint64_t objects{},visuals{},meshes{},textures{},navigation{},bytes{}; };
struct LifecycleDiagnosticsV36 {
 LifecycleStatusV36 status{LifecycleStatusV36::loading};
 std::int32_t source_state{},source_progress{},failed_state{-1};
 std::uint64_t service_calls{},completed_stages{},elapsed_nanoseconds{},cpu_ticks{},owned_pins{};
 LifecycleResourceCountsV36 actual_resources;
 bool actual_resources_available{},whole_level_init_verified{},gameplay_ready{};
 std::string required_service,error;
};
struct LifecycleServicesV36 {
 // Each body performs the authentic side effects for the numbered source
 // stage, excluding source field130 increment/progress-tail/UI callback.
 // pending preserves its own cursor. The map/InitPost internal source spins
 // become ONE call per tick. No partial body may return complete.
 std::array<std::function<LifecycleStepV36(std::string&)>,38> stage_body;
 // Source stage7 increments file13c AFTER dispatcher130 increment and
 // BEFORE progress tail. Required for7; optional reached hooks for others.
 std::array<std::function<bool(std::string&)>,38> after_source_increment;
 // Original post-tail onProgress/license/menu effects, owned by main/menu.
 std::function<bool(std::int32_t,std::int32_t,std::string&)> publish_progress;
 // Real retained resource counters, not elapsed-time-derived work estimates.
 std::function<bool(LifecycleResourceCountsV36&,std::string&)> resource_counts;
 // Cancellation is an explicit native safety adapter; source has no cancel
 // branch in _LoadProcess. Must complete genuine Level::Unload/teardown before
 // resource pins are dropped. May yield pending for bounded cleanup.
 std::function<LifecycleStepV36(std::string&)> cancel_and_unload;
};
class LifecycleV36 {
 LifecycleFieldsV36 fields_;
 std::shared_ptr<void> actual_level_pin_;
 std::vector<std::shared_ptr<const void>> resource_pins_;
 LifecycleServicesV36 services_;
 LifecycleDiagnosticsV36 diagnostics_;
 std::int32_t expected_state_{};
 bool busy_{},cancel_requested_{};
 void fail(std::int32_t,const std::string&,const std::string&);
 bool publish();
public:
 LifecycleV36(LifecycleFieldsV36,std::shared_ptr<void> actual_level_pin,
              std::vector<std::shared_ptr<const void>> retained_resource_pins,
              LifecycleServicesV36);
 LifecycleV36(const LifecycleV36&)=delete;
 LifecycleV36& operator=(const LifecycleV36&)=delete;
 // One original stage-body call maximum; no poll loop or timer progress.
 LifecycleStatusV36 tick();
 void request_cancel() noexcept {cancel_requested_=true;}
 const LifecycleDiagnosticsV36& diagnostics() const noexcept {return diagnostics_;}
};
}
