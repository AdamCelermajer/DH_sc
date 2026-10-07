#pragma once
#include <area_transition_request_v114.hpp>
namespace model_renderer {
// Runs the existing live SaveAllPlayers prefix, then copies the resulting
// serialized cache. The returned bytes retain no old actor/index/World loan.
// This is serialization/queue acceptance, not a disk-persistence receipt.
bool capture_source_campaign_transition_restore_v114(const std::shared_ptr<void>&,
 dh2::loader::AreaTransitionRestoreReceiptV114&,std::string&);
}
