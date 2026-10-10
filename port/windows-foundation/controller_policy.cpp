#include "controller_policy.hpp"
#include "content_paths.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"
#include <cmath>
#include <stdexcept>
namespace dh::foundation {
bool load_controller_policy(const AssetCatalog& assets,const std::string& uri,
                            ActorMovementConfig& output,std::string& provenance,std::string& error) {
    try {
        auto bytes=read_content(assets,uri);if(bytes.empty()||bytes.size()>65536)throw std::runtime_error("Controller policy size invalid");
        std::string text(bytes.begin(),bytes.end());if(text.find('\0')!=std::string::npos)throw std::runtime_error("NUL in controller policy");
        TiXmlDocument document;document.Parse(text.c_str());auto* root=document.RootElement();
        if(document.Error()||!root||std::string(root->Value())!="controllerPolicy")throw std::runtime_error("Invalid controller policy XML");
        const auto* source=root->Attribute("provenance");if(!source||!*source)throw std::runtime_error("Controller policy needs explicit provenance");
        auto next=output;
        for(auto field:{std::pair<const char*,float*>{"maxStepUp",&next.maxStepUp},{"maxStepDown",&next.maxStepDown},{"maxSlopeDegrees",&next.maxSlopeDegrees}}) {
            double value;
            if(root->QueryDoubleAttribute(field.first,&value)!=TIXML_SUCCESS||!std::isfinite(value)||value<0||value>1000000)throw std::runtime_error(std::string("Invalid policy field ")+field.first);
            *field.second=float(value);
        }
        output=next;provenance=source;error.clear();return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
