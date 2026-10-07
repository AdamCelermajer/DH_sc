#pragma once
#include <native_physical_filter_v1.hpp>
#include <memory>
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
// Root's actual V69 physical associations choose the already constructed
// POZone/PODecor receiver. No cast from NativeBody to a source owner.
bool borrow_source_campaign_physical_filter_v105(const SourceCampaignCandidateBorrowV55&,
 std::uintptr_t,std::shared_ptr<void>&,dh2::physical::NativePhysicalFilterBorrowV1&,std::string&);
}
