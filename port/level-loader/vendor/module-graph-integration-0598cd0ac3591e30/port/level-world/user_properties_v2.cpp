#include "user_properties_v2.hpp"
#include <cstring>
namespace dh2::world {
namespace {
bool alnum(unsigned char c){return (c>='A'&&c<='Z')||(c>='a'&&c<='z')||(c>='0'&&c<='9');}
void line(const std::string& input,std::map<std::string,std::string>& result){
 auto equal=input.find('=');const auto key=input.substr(0,equal);
 std::size_t start=0;while(start<key.size()&&!alnum(static_cast<unsigned char>(key[start])))++start;
 if(start==key.size())return;
 auto end=start+1;while(end<key.size()&&alnum(static_cast<unsigned char>(key[end])))++end;
 std::string value;
 if(equal!=std::string::npos){value=input.substr(equal+1);auto first=value.find("%22");
  if(first!=std::string::npos){auto second=value.find("%22",first+3);if(second!=std::string::npos)value=value.substr(first+3,second-first-3);}}
 result[key.substr(start,end-start)]=std::move(value);
}
}
bool user_properties_v2(const char* source,std::map<std::string,std::string>& out,std::string& e){
 out.clear();e.clear();if(!source){e="Required original UserProperties CString";return false;}
 std::size_t n=0;while(source[n]){if(n>=1048576){e="UserProperties exceeds retained input capacity";return false;}
  if(static_cast<unsigned char>(source[n])>=128){e="Required source libc non-ASCII ctype producer";return false;}++n;}
 std::size_t first=0;for(std::size_t i=0;i<=n;++i)if(i==n||source[i]=='\n'){line(std::string(source+first,i-first),out);first=i+1;}
 return true;
}
}
