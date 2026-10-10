#pragma once

#include "../../../android-native/app/src/main/cpp/source_campaign_script_execution_v96.hpp"

namespace dh::foundation::cinematics {

// Native V59 hooks receive the actual checked parse borrow. They run before
// the binder's already-composed source body; handled=false delegates to that
// same body. No OriginalCampaignRuntime command snapshot is involved.
struct SourceCampaignNativeCommandHooksV96 {
    std::shared_ptr<void> owner;
    std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,bool skip,
        std::int32_t module,bool& handled,std::string&)> execute;
    std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,
        bool& handled,bool& blocking,std::string&)> blocking;
    std::function<bool(const dh2::loader::CheckedCommandBorrowV59&,
        bool& handled,std::string&)> update;
};

// Suitable for SourceScriptBehaviorDecoratorV96. The manager and scheduler
// are borrowed from the native binder and must remain the owners it later
// binds. Captured command closures retain the provider but weakly reference
// those owners, avoiding an Application/ScriptManager ownership cycle.
bool decorate_source_campaign_native_behavior_v96(
    const std::shared_ptr<dh2::loader::ScriptManagerOwnerV52>&,
    const dh2::loader::ScriptSchedulerServicesV96&,
    const SourceCampaignNativeCommandHooksV96&,
    dh2::loader::ScriptCommandBehaviorV59&,
    std::string&);

dh2::android_ui::SourceScriptBehaviorDecoratorV96
source_campaign_native_behavior_decorator_v96(
    SourceCampaignNativeCommandHooksV96);

} // namespace dh::foundation::cinematics
