#pragma once
#include <area_transition_sequence_v114.hpp>
#include <area_transition_request_v114.hpp>
namespace model_renderer {
// Actual confirmation callbacks supply the immutable authored request and
// native start/restore services. Touching an ExitZone does not call this API.
bool enqueue_confirmed_source_area_transition_v114(
 std::shared_ptr<const dh2::loader::AreaTransitionRequestV114>,
 dh2::loader::AreaTransitionServicesV114,std::string&);
// Explicit retry resumes only the retained old-level retirement continuation.
bool resume_source_area_transition_retirement_v114(std::string&);
bool request_confirmed_source_area_load_v114(const dh2::loader::ApplicationLoadLevelArgumentsV114&,std::string&);
}
