#pragma once
#include "source_process_pydata_files_v101.hpp"
#include "source_process_array_registration_v101.hpp"
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <cstdint>
namespace dh2::android_ui {
struct ProcessArrayValueV101;
struct ProcessArrayRowV101 {std::vector<ProcessArrayValueV101> fields;};
struct ProcessArrayValueV101 {
 enum class Kind {word,byte,half,float32,string,vector};Kind kind{Kind::word};
 std::uint32_t bits{};std::string text;std::vector<ProcessArrayRowV101> elements;
};
struct ProcessArrayGroupV101 {
 std::uint32_t declared_rows{},declared_names{};
 std::vector<ProcessArrayRowV101> rows;
 std::vector<std::string> names,members;
 std::map<std::string,std::int32_t> name_index;
 bool records_loaded{},names_loaded{},members_loaded{};
};
//One source C1 registration map, source38 load counter and actual decoded
//group storage. Compiled Structs member CStrings come from their original
//process global constructor, independently of array/name file load phases.
//No World, movie, GL generation or parallel table cache.
class SourceProcessArraysV101 {
 std::map<std::string,ProcessArrayGroupV101> groups_;
 std::map<std::string,std::string> classes_,member_types_;
 std::uint32_t stage38_{};bool failed_{};std::string error_;
public:
 using Read=std::function<bool(const std::string&,std::vector<std::uint8_t>&,std::string&)>;
 SourceProcessArraysV101();
 bool load_stage(const Read&,bool& complete,std::string&);
 const ProcessArrayGroupV101* group(const char*)const noexcept;
 //Actual registerClassByName dispatch: struct field index or array name ID.
 bool get_member_id(const char* type,const char* name,std::int32_t&,std::string&)const;
 std::uint32_t stage()const noexcept{return stage38_;}
 bool ready()const noexcept{return stage38_==70&&!failed_;}
};
}
