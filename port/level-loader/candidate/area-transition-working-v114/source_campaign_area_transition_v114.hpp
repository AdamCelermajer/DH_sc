#pragma once
#include "area_transition_sequence_v114.hpp"
#include "source_campaign_retirement_v88.hpp"
namespace model_renderer {
//Installs ONLY the three existing retirement routes. Incoming native start and
//completion providers remain Main's actual V55/start/restore composition.
bool bind_source_campaign_transition_retirement_v114(SourceCampaignRetirementServicesV88,
 dh2::loader::AreaTransitionServicesV114&,std::string&);
}