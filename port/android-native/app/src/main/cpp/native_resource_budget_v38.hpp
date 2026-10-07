#pragma once
#include "../../../../../engine-resources/resource_budget_v37.hpp"
namespace dh2::android_resources {
// Single ledger for this real native GL context. Owners are migrated explicitly;
// the ledger measures admitted storage, not GPU driver/private host residency.
resources::ContextResourceBudgetV37& budget_v38();
// Retained CPU owners keep this lease and release through it. They must not
// call the singleton factory from namespace-static destruction.
std::shared_ptr<resources::ContextResourceBudgetV37> budget_lease_v39();
void begin_context_v38();
void context_lost_v38()noexcept;
void release_v38(resources::ResourceTokenV37&);
// Explicit diagnostics only; never sampled on the rendering hot path.
std::string budget_report_v46();
}
