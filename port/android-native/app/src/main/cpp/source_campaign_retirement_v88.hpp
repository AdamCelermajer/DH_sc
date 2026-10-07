#pragma once
#include <functional>
#include <memory>
#include <string>
#include <cstdint>
namespace model_renderer {
class SourceCampaignReleaseV88;
bool source_campaign_release_complete_v88(const std::shared_ptr<SourceCampaignReleaseV88>&,bool& unload,bool& destroy,std::string&);
enum class SourceCampaignRetirementResultV88:std::uint8_t {pending,complete,failed};
struct SourceCampaignRetirementServicesV88 {
 std::shared_ptr<void> owner; //independent native provider, World/App weak
 //Actual admission closure: pending while an existing owning-thread source
 //frame/draw/event/contact scope remains. No storage/resource drop here.
 std::function<SourceCampaignRetirementResultV88(const std::shared_ptr<void>&,std::string&)> quiesce;
 //Only after original GS Dtor clears34/s_level. Pending for real callback
 //receipts; complete requires actual renderer aliases unpublished/retired.
 std::function<SourceCampaignRetirementResultV88(const std::shared_ptr<void>&,std::string&)> retire_world;
};
//True means a request was retained, not that source destruction completed.
bool begin_source_campaign_retirement_v88(SourceCampaignRetirementServicesV88,std::string&);
SourceCampaignRetirementResultV88 drain_source_campaign_retirement_v88(std::string&);
bool source_campaign_retirement_requested_v88()noexcept;
//Explicit resume of the SAME failed continuation; never starts new teardown.
bool retry_source_campaign_retirement_v114(std::string&);
}
