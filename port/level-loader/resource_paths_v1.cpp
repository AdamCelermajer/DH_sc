#include "resource_paths_v1.hpp"
#include <stdexcept>
namespace dh2::loader {
std::vector<std::string> compiled_level_paths_v1(const std::string& authored) {
    if(authored.find('\0')!=std::string::npos)
        throw std::invalid_argument("Embedded NUL in resource name");
    std::vector<std::string> result;
    for(const char* folder:{"","data/","data/scene/","data/3d/modules/"}) {
        std::string candidate=std::string(folder)+authored;
        for(const char* marker:{"old/","debug/","ps3/","iphone/"}) {
            const auto found=candidate.find(marker);
            if(found!=std::string::npos) {
                candidate.erase(found,std::char_traits<char>::length(marker));
                break;
            }
        }
        result.push_back(std::move(candidate));
    }
    return result;
}
}
