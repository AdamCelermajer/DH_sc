#pragma once
#include <level_source_loading_v43.hpp>
namespace model_renderer {
struct SourceWorldBorrowV61;
//Call after the sole candidate.lend_loading_inputs, before SourceLoading C1.
//Only callback transport is retained; source Module/InitPost remain V43-owned.
bool bind_deferred_campaign_object_loading_v106(const std::shared_ptr<SourceWorldBorrowV61>&,
 dh2::loader::SourceLoadingInputsV43&,std::string&);
}
