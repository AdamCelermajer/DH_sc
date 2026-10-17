#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::data {
struct Bytes{const std::uint8_t* data;std::size_t size;};
struct Dictionary{std::vector<std::string> names,values;};
struct CharacterTable{
 std::vector<std::string> names,fields;
 std::vector<std::array<std::int32_t,224>> rows;
 std::size_t data_consumed=0,names_consumed=0,fields_consumed=0;
};
// Leading CharacterTable section only; subsequent subclass sections are
// retained in their original files and not interpreted here.
bool load_characters(Bytes records,Bytes names,Bytes fields,CharacterTable&,std::string&);
bool load_dictionary(Bytes names,Bytes values,Dictionary&,std::string&);
const std::int32_t* property(const CharacterTable&,const std::string& character,const std::string& field);
// NativeGetPossibleClassSpec reads ClassString/ClassDescString from the two
// rows immediately following the current source class row. The returned order
// is Class1Name, Class1Desc, Class2Name, Class2Desc.
bool possible_class_specialization_text_ids(const CharacterTable&,
 std::int32_t current_class,std::array<std::int32_t,4>&,std::string& error);
const std::string* lookup(const Dictionary&,const std::string& name);
}
