#pragma once
#include "stage_loader_v51_config_scripts.hpp"
#include "canonical_level_config_module_v1.hpp"
namespace dh2::loader {
// SAME actual receiver; no copy/another registry. Getter proposal only exposes
// existing produced float map, never inserts a default or shadows fog state.
inline Stage8ConfigBorrowV51 borrow_canonical_stage8_config_v51(const std::shared_ptr<world::CanonicalLevelConfigV1>& actual){
 if(!actual)return {};auto fields=actual->properties().fields;
 return {actual,actual->identity(),actual->string(0x150),&actual->dfog_colors(),actual->scalar_float_v51(0x210),
  [actual,fields](const std::string& value,std::string& error){if(!fields.write_string){error="Required actual LevelConfig string150 writer";return false;}return fields.write_string(fields.context,0x150,value,error);}};
}
}
