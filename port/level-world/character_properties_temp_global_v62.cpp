#include "character_properties_temp_global_v62.hpp"
namespace dh2::character {
const std::shared_ptr<data::PropertySheet>& character_properties_temp_global_v62(){
 static const auto payload=std::make_shared<data::PropertySheet>();return payload;
}
}
