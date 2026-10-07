#pragma once
#include "canonical_character_candidate_v60.hpp"
namespace dh2::world {
//1 handled by source-owned fields/VM,0 another endpoint,-1 reached failure.
int canonical_character_hud_actor_v62(CanonicalCharacterCandidateRecordV60&,
 const ui::HudManagerRequest&,ui::HudManagerResponse&,std::string&);
}
