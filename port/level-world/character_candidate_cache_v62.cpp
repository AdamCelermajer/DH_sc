#include "character_candidate_cache_v62.hpp"
namespace dh2::character {
bool CharacterCandidateCacheV62::read(const char* name,std::vector<std::uint8_t>& out,std::string& e){
 bool found{};if(!files_.read||!files_.read(std::string("data/pydata/")+name,found,out,e))return false;
 if(!found){e=std::string("Missing actual Character cache table: ")+name;return false;}return true;
}
bool CharacterCandidateCacheV62::load(std::string& e){
 if(attempted_){e="Character cache initialization cannot replay a reached prefix";return false;}attempted_=true;
 auto span=[](const auto& b){return data::Bytes{b.data(),b.size()};};
 std::vector<std::uint8_t> names,values,schema;
 if(!read("character_models_dictionary_pyarraynames.bin",names,e)||!read("character_models_dictionary_pyarray.bin",values,e)||!data::load_dictionary(span(names),span(values),models_,e))return false;
 if(!read("animations_dictionary_pyarraynames.bin",names,e)||!read("animations_dictionary_pyarray.bin",values,e)||!data::load_dictionary(span(names),span(values),animations_,e))return false;
 if(!read("animations_pyarraynames.bin",names,e)||!read("animations_pyarray.bin",values,e)||!read("animations_pystructnames.bin",schema,e)||!data::load_animation_tables(span(values),span(names),span(schema),animations_,tables_,e))return false;
 if(!read("loot_table_pyarraynames.bin",names,e)||!read("loot_table_pyarray.bin",values,e)||!read("loot_table_pystructnames.bin",schema,e)||!loot_.load(span(values),span(names),span(schema),e))return false;
 auto templates=std::make_shared<data::CharacterTemplateTableV78>();
 if(!read("character_templates_pyarraynames.bin",names,e)||!read("character_templates_pyarray.bin",values,e)||!read("character_templates_pystructnames.bin",schema,e)||
    !templates->load(span(values),span(names),span(schema),e))return false;
 templates_=std::move(templates);
 if(!read("sounds_pyarray.bin",values,e)||!sounds_v70_.load(values,e))return false;
 ready_=true;e.clear();return true;
}
}
