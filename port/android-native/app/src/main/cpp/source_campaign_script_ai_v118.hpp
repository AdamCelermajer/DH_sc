#pragma once
#include <cstdint>
#include <string>
namespace dh2::loader {struct CheckedCommandBorrowV59;}
namespace dh2::world {struct CanonicalCharacterCandidateRecordV60;}
namespace dh2::character {struct StateOwnerRequest48;}
namespace model_renderer {
struct SourceCampaignCandidateBorrowV55;
bool execute_source_campaign_script_ai_v118(const SourceCampaignCandidateBorrowV55&,
 const dh2::loader::CheckedCommandBorrowV59&,bool,std::int32_t,bool&,std::string&);
int source_campaign_limbus_body_v118(dh2::world::CanonicalCharacterCandidateRecordV60&,
 const dh2::character::StateOwnerRequest48&,std::string&);
}
