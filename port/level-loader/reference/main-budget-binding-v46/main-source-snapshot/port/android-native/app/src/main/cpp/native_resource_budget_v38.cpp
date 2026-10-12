#include "native_resource_budget_v38.hpp"
#include <stdexcept>
namespace dh2::android_resources {
std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39(){
 // Combined authored main-menu/HUD/character-menu plus world textures exceed
 // the initial128MiB engineering texture ceiling in real menu-flow testing.
 // Keep a finite Android profile; portable ledger defaults and byte/count
 // checks are unchanged. This measures requested storage, not driver residency.
 static auto ledger=[](){resources::ResourceBudgetLimitsV37 limits;
  limits.gpu_bytes=512*resources::mib_v37;
  limits.texture_gpu_bytes=256*resources::mib_v37;
  return std::make_shared<resources::ContextResourceBudgetV37>(limits);
 }();return ledger;
}
resources::ContextResourceBudgetV37& budget_v38(){return *budget_lease_v39();}
void begin_context_v38(){std::string error;if(!budget_v38().begin_context(error))throw std::runtime_error("Native resource context: "+error);}
void context_lost_v38()noexcept{budget_v38().context_lost();}
void release_v38(resources::ResourceTokenV37& token){if(!token)return;std::string error;if(!budget_v38().release(token,error))throw std::runtime_error("Native resource release: "+error);}
}
