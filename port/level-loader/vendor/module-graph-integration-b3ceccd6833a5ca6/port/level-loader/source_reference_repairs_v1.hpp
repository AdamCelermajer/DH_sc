#pragma once
#include "procedural_modules_v1.hpp"
namespace dh2::loader {
// Explicit cache-reference overlay. Never edits authored XML or generator inputs.
// Applies only documented exact path corrections when the authored path is absent
// and the replacement exists. Failure leaves the entire module plan unchanged.
bool repair_procedural_references_v1(const assets::ZipAssetPackV1&,
    ProceduralModulePlanV1&,std::string& error);
// Recovered original _LoadProcess fallback naming, scoped to the resource basename.
std::string original_backup_definition_v1(const std::string& definition);
}
