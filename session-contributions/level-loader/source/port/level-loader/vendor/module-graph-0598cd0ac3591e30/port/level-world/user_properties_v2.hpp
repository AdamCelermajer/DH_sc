#pragma once
#include <map>
#include <string>
namespace dh2::world {
// UserProperties319158/31903c/318fe4/318e5c. ASCII key domain of the actual
// cache (source libc ctype flags7); non-ASCII input remains explicit required.
// Values retain their source whitespace unless enclosed by literal %22 pairs.
bool user_properties_v2(const char*,std::map<std::string,std::string>&,std::string&);
}
