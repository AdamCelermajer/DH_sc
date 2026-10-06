#include "native_resource_budget_v38.hpp"
#include <stdexcept>
namespace dh2::android_resources {
std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39(){static auto ledger=std::make_shared<resources::ContextResourceBudgetV37>();return ledger;}
resources::ContextResourceBudgetV37& budget_v38(){return *budget_lease_v39();}
void begin_context_v38(){std::string error;if(!budget_v38().begin_context(error))throw std::runtime_error("Native resource context: "+error);}
void context_lost_v38()noexcept{budget_v38().context_lost();}
void release_v38(resources::ResourceTokenV37& token){if(!token)return;std::string error;if(!budget_v38().release(token,error))throw std::runtime_error("Native resource release: "+error);}
}
