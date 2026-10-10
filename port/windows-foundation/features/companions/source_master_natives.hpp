#pragma once
#include "source_master.hpp"
#include <memory>
namespace dh::foundation::companions {
struct SourceMasterNativeOwner {
    std::shared_ptr<void> lease;
    std::function<bool(SourceMasterBorrow&,std::string&)> borrow;
};
// CharacterScriptSessionInput[V3].gameplay_binding contract. Installs only
// original companion callbacks not already handled by that retained Session.
// Original commands/FSM/property/design/host/timers retain existing bindings.
int select_source_master_native(void*,std::uint32_t,dh2_script_function*,void**);
}
