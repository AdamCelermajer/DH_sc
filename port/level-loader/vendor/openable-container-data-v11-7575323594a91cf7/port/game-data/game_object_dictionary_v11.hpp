#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::data {
// Arrays::GameObjectDict: source ColladaFile records (len4,C-string8),
// distinct from OpenableContainers.visual IDs and constant enum names.
class GameObjectDictionaryV11 {
 std::vector<std::string> files_,names_;
 bool loaded_{};
public:
 bool load(const std::uint8_t* records,std::size_t,const std::uint8_t* names,std::size_t,std::string&);
 bool file(std::int32_t,const std::string*&,std::string&)const;
 bool find(const char*,std::int32_t&,std::string&)const;
 std::size_t size()const noexcept{return files_.size();}
};
}
