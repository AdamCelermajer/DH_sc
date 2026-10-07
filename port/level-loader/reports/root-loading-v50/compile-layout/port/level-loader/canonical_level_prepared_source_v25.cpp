#include "canonical_level_context_v1.hpp"
namespace dh2::loader {
// Same existing method, separated only to let native C1/GSLevel link without
// falsely claiming the procedural/fixed-map preparation pipeline is active.
bool CanonicalLevelContextV1::retain_prepared_source(LevelPreparationV1::Borrow source,std::string& error){
 error.clear();if(!source){error="required completed Level source borrow";return false;}
 const auto& actual=source.request();
 if(actual.identity!=request_.identity||actual.definition!=request_.definition||actual.kind!=request_.kind||
    actual.seed!=request_.seed||actual.repair_known_references!=request_.repair_known_references||
    actual.allow_original_backup!=request_.allow_original_backup){
  error="prepared source belongs to a different canonical Level request";return false;
 }
 prepared_=std::move(source);return true;
}
}
