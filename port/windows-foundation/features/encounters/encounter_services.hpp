#pragma once
#include "../../original_campaign_world_adapter.hpp"
#include "../../../level-world/native_conditions_v69.hpp"
#include "../../../level-world/object_enable_condition_v2.hpp"
#include "../../../level-world/canonical_trigger_zone_v22.hpp"
namespace dh::foundation::encounters {
struct AdmissionServices {
 std::shared_ptr<void> owner;
 std::function<bool(bool&character,bool&save,std::uint8_t&quest_sync14,std::string&)> local_save;
 std::function<bool(bool&present,std::int32_t&difficulty118,std::string&)> current_level;
 // Actual selected source virtual44/48 effects on SAME receiver.
 std::function<bool(dh2::world::CanonicalGameObjectBaseOwnerV1&,bool,std::string&)> enabled_event;
};
class Admission {
public:
 Admission(std::shared_ptr<dh2::world::NativeConditionRuntimeV69>,AdmissionServices);
 bool initialize(dh2::world::CanonicalGameObjectBaseOwnerV1&,unsigned offset,std::string&);
 bool test_enable(dh2::world::CanonicalGameObjectBaseOwnerV1&,bool mark_tested,bool&enabled,std::string&);
 bool test_disable(dh2::world::CanonicalGameObjectBaseOwnerV1&,bool mark_tested,bool&enabled,std::string&);
private:
 std::shared_ptr<dh2::world::NativeConditionRuntimeV69> conditions_;AdmissionServices services_;
 bool evaluate(dh2::world::CanonicalGameObjectBaseOwnerV1&,bool disable,bool mark,bool&,std::string&);
};
struct CampaignSourceServices {
 std::shared_ptr<void> owner;
 std::function<bool(int global_script,int module,bool received,bool&admitted,std::string&)> admit_start;
 std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> random;
 // Flags produced by original command initialization, not assumed zero.
 std::function<bool(int global_script,std::uint8_t&,std::string&)> script_flags;
 std::function<bool(const OriginalCampaignCommand&,bool&skip,std::string&)> command_skip;
};
class CampaignBinding {
public:
 CampaignBinding(OriginalCampaignRuntime&,OriginalCampaignWorldAdapter&,CampaignSourceServices);
 bool bind(std::string&);
 // Call before actual Trigger constructor/InitPost. Source Trigger script IDs
 // are level-array local; translation to common+level runtime IDs stays here.
 bool trigger_script_services(dh2::world::TriggerZoneServicesV22&,std::string&);
 bool tick(std::int32_t actual_delta_ms,std::string&);
private:
 OriginalCampaignRuntime* runtime_;OriginalCampaignWorldAdapter* world_;CampaignSourceServices source_;
 bool bound_=false;
 int common_count()const;
 bool global_from_level(int,int&,std::string&)const;
};
}
