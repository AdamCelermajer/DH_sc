#include "edit_text_definition_v1.hpp"
namespace dh2::ui::edit_text_definition_v1 {
bool remove_html(std::string&text,std::string&error){
 error.clear();const auto null=text.find('\0');const auto visible=text.substr(0,null);const auto close=visible.find("</");if(close==std::string::npos)return true;
 const auto opening=visible.rfind('>',close);if(opening==std::string::npos){error="source removeHTML scans outside storage without a preceding '>'";return false;}
 const auto begin=opening+1;const auto size=close-begin;
 if(size>511){error="source removeHTML exceeds its 512-byte stack buffer";return false;}
 text=visible.substr(begin,size);return true;
}
}
