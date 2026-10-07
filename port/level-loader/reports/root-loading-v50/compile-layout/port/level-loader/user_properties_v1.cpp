#include "user_properties_v1.hpp"
namespace dh2::loader {
namespace {bool alnum(unsigned char c){return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9');}}
bool decode_user_properties_v1(const std::string& raw,std::map<std::string,std::string>& out,std::string& error) {
    error.clear();
    if(raw.size()>4096||raw.find('\0')!=std::string::npos){error="User property source exceeds checked domain";return false;}
    // Only C-locale ASCII is currently verified; reject high-byte keys/values
    // rather than relying on the host's locale or signed-char table indexing.
    for(unsigned char c:raw)if(c>=128){error="User property encoding outside checked ASCII domain";return false;}
    std::map<std::string,std::string> candidate;
    for(std::size_t start=0;start<=raw.size();) {
        const auto nl=raw.find('\n',start);const auto line=raw.substr(start,nl==std::string::npos?std::string::npos:nl-start);
        const auto equal=line.find('=');const auto left=line.substr(0,equal);
        std::size_t first=0;while(first<left.size()&&!alnum(static_cast<unsigned char>(left[first])))++first;
        if(first<left.size()) {
            std::size_t end=first;while(end<left.size()&&alnum(static_cast<unsigned char>(left[end])))++end;
            // _ParseKeyValue's no-equals branch is confirmed separately by
            // original-call comparisons; preserve the literal empty value.
            std::string value=equal==std::string::npos?std::string{}:line.substr(equal+1);
            const auto open=value.find("%22");
            if(open!=std::string::npos) {
                const auto close=value.find("%22",open+3);
                if(close!=std::string::npos)value=value.substr(open+3,close-open-3);
            }
            candidate[left.substr(first,end-first)]=std::move(value);
        }
        if(nl==std::string::npos)break;
        start=nl+1;
    }
    out=std::move(candidate);return true;
}
}
