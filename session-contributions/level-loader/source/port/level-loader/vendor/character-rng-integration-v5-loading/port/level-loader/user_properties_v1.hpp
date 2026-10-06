#pragma once
#include <map>
#include <string>
namespace dh2::loader {
// Source-owned user metadata, separate from gameplay CStrProps overrides.
// Original UserProperties splits LF lines, takes the first alphanumeric key
// run and extracts literal %22 pairs. It is not a general URL decoder.
bool decode_user_properties_v1(const std::string&,std::map<std::string,std::string>&,std::string&);
}
