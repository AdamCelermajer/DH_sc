#include "procedural_file_list_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
std::vector<std::string> procedural_file_list_v1(std::string_view bytes) {
    if(bytes.size()>1048576)throw std::invalid_argument("Procedural file list exceeds checked domain");
    const auto nul=bytes.find('\0');if(nul!=bytes.npos)bytes=bytes.substr(0,nul);
    std::vector<std::string> result;
    while(true){
        const auto newline=bytes.find('\n');if(newline==bytes.npos)break;
        // Original unsigned substring length underflows at LF position zero;
        // std::string::substr clamps that length to the remaining string size.
        result.emplace_back(bytes.substr(0,newline?newline-1:bytes.size()));
        bytes.remove_prefix(newline+1);
    }
    return result;
}
}
