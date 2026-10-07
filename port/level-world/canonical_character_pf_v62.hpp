#pragma once
#include "canonical_character_candidate_v60.hpp"
namespace dh2::world {
// Original Character virtual PF policy over its actual retained runtime.
// Registry/header/buffer storage comes from the source campaign candidate.
bool canonical_character_update_pf_v62(CanonicalCharacterCandidateRecordV60&,
 const navigation::CollisionWorld&,navigation::ObstacleRegistry&,std::string&);
bool canonical_character_init_pf_v62(CanonicalCharacterCandidateRecordV60&,
 const navigation::CollisionWorld&,const float*,float,std::string&);
}
