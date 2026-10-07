#pragma once
#include "model_renderer.hpp"
#include "source_campaign_runtime_v61.hpp"
#include <native_conditions_v69.hpp>
#include <condition_scoped_binding_v72.hpp>
#include <world_item_graph_v3.hpp>
namespace model_renderer {
// Exact Main API. The production definition/sole cache decoder/runtime is Main's.
// This repeated declaration can be removed when model_renderer.hpp publishes it.
bool borrow_source_campaign_conditions_v70(const SourceCampaignCandidateBorrowV55&,
 dh2::world::ConditionDataInitServicesV3&,std::shared_ptr<dh2::world::NativeConditionRuntimeV69>&,std::string&);
// No candidate receipt is retained. The bag owns only the independent native
// ConditionList arena; Main providers borrow actual App/World/PM/Level weakly.
bool lend_campaign_condition_bag_v75(const SourceCampaignCandidateBorrowV55&,
 dh2::world::ConditionDataInitServicesV3&,std::shared_ptr<dh2::world::NativeConditionRuntimeV69>&,std::string&);
// Receives the RETURNED same bag, never calls create/decode or regenerates it.
bool bind_campaign_condition_base_v75(std::uintptr_t,dh2::world::CanonicalBaseBorrowV68,
 const dh2::world::ConditionDataInitServicesV3&,const std::shared_ptr<dh2::world::NativeConditionRuntimeV69>&,
 std::shared_ptr<dh2::world::ConditionDataBindingV72>&,std::string&);
bool bind_campaign_item_conditions_v75(const SourceCampaignCandidateBorrowV55&,
 const std::shared_ptr<dh2::character::RetainedWorldItemObjectV1>&,
 dh2::character::WorldItemGraphServicesV3&,std::string&);
// Implemented beside existing source_campaign_v55, before any XML factory.
bool bind_source_campaign_module_conditions_runtime_v75(const SourceCampaignCandidateBorrowV55&,
 const dh2::world::ConditionDataInitServicesV3&,std::string&);
}
