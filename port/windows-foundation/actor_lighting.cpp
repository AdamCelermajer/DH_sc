#include "actor_lighting.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace dh::foundation {
namespace {
bool triple(const TiXmlElement& element, const char* key, std::array<float,3>& output,
            std::string& error) {
    const char* text=element.Attribute(key);
    if (!text) return true;
    std::istringstream input(text);
    char first=0,second=0;
    std::array<float,3> result;
    if (!(input>>result[0]>>first>>result[1]>>second>>result[2]) || first!=',' || second!=',') {
        error=std::string("Invalid source light vector: ")+key; return false;
    }
    input>>std::ws;
    if (!input.eof() || !std::all_of(result.begin(),result.end(),[](float value) { return std::isfinite(value); })) {
        error=std::string("Nonfinite or trailing source light vector: ")+key; return false;
    }
    output=result; return true;
}
bool scalar(const TiXmlElement& element, const char* key, float& output, std::string& error) {
    const char* text=element.Attribute(key);
    if (!text) return true;
    std::istringstream input(text); float value;
    if (!(input>>value) || !std::isfinite(value)) { error=std::string("Invalid source light scalar: ")+key; return false; }
    input>>std::ws;
    if (!input.eof()) { error=std::string("Trailing source light scalar: ")+key; return false; }
    output=value; return true;
}
std::optional<bool> definesLighting(std::string_view text) {
    // Conditional preambles need the actual driver/config macro environment.
    // Reject that unresolved domain rather than guess their effective branch.
    if (text.find("/*")!=std::string_view::npos) return std::nullopt;
    bool lighting=false;
    std::istringstream lines{std::string(text)}; std::string line;
    while (std::getline(lines,line)) {
        const auto comment=line.find("//"); if (comment!=std::string::npos) line.resize(comment);
        std::istringstream tokens(line); std::string directive,name;
        if (!(tokens>>directive)) continue;
        if (directive=="#") { tokens>>directive; directive="#"+directive; }
        if (directive=="#if" || directive=="#ifdef" || directive=="#ifndef"
            || directive=="#elif" || directive=="#else" || directive=="#endif") return std::nullopt;
        if ((directive=="#define" || directive=="#undef") && tokens>>name && name=="LIGHTING")
            lighting=directive=="#define";
    }
    return lighting;
}
template<std::size_t N> bool finite(const std::array<float,N>& values) {
    return std::all_of(values.begin(),values.end(),[](float value) { return std::isfinite(value); });
}
}

bool parseAuthoredPointLights(std::string_view xml, std::vector<AuthoredPointLight>& output, std::string& error) {
    if (xml.empty() || xml.size()>1024*1024 || xml.find('\0')!=std::string_view::npos) {
        error="Light module outside bounded UTF8/XML domain"; return false;
    }
    TiXmlDocument document; const std::string owned(xml); document.Parse(owned.c_str());
    const auto* root=document.RootElement();
    if (document.Error() || !root || std::string(root->Value())!="Module") {
        error="Malformed authored light Module"; return false;
    }
    std::vector<AuthoredPointLight> result;
    for (const auto* element=root->FirstChildElement(); element; element=element->NextSiblingElement()) {
        const auto* type=element->Attribute("type");
        if (std::string(element->Value())!="GameObject" || !type || std::string(type)!="Light") continue;
        const auto* gameType=element->Attribute("gametype");
        if (!gameType || std::string(gameType)!="LightPoint") { error="Unsupported authored light kind"; return false; }
        AuthoredPointLight light;
        const auto* name=element->Attribute("name");
        if (!name || !*name || result.size()>=4096) { error="Missing source light name or excessive light count"; return false; }
        light.name=name; if (const auto* attachment=element->Attribute("attachedTo")) light.attachedTo=attachment;
        if (!triple(*element,"position",light.position,error) || !triple(*element,"attachedOffset",light.attachedOffset,error)
            || !triple(*element,"attenuation",light.attenuation,error) || !triple(*element,"ambientColor",light.ambient,error)
            || !triple(*element,"diffuseColor",light.diffuse,error) || !triple(*element,"specularColor",light.specular,error)
            || !scalar(*element,"radius",light.radius,error)) return false;
        float automatic=0; if (!scalar(*element,"automatic",automatic,error)) return false;
        if (automatic!=0 && automatic!=1) { error="Source automatic flag outside boolean domain"; return false; }
        light.automatic=automatic!=0;
        result.push_back(std::move(light));
    }
    output=std::move(result); error.clear(); return true;
}

SourceLight normalizeSourceLight(const AuthoredPointLight& authored) {
    SourceLight result;
    std::copy(authored.position.begin(),authored.position.end(),result.position.begin());
    result.radius=authored.radius;
    result.attenuation={authored.attenuation[0],authored.attenuation[1]/1000.0f,authored.attenuation[2]/1000000.0f};
    for (std::size_t i=0;i<3;++i) {
        result.ambient[i]=authored.ambient[i]/255.0f;
        result.diffuse[i]=authored.diffuse[i]/255.0f;
        result.specular[i]=authored.specular[i]/255.0f;
    }
    return result;
}

bool planSourceLightBindings(const SourceLightSets& source, SourceLightSetId setId,
                             const std::array<bool,5>& filter, const std::array<bool,4>& present,
                             std::vector<SourceLightBinding>& output, std::string& error) {
    const auto id=static_cast<std::uint32_t>(setId);
    if (id>=source.sets.size()) { error="Source LightSet id outside four-set domain"; return false; }
    std::vector<SourceLightBinding> result; std::uint32_t target=0;
    for (std::uint32_t slot=0;slot<4;++slot)
        if (filter[slot] && present[target] && source.sets[id][slot])
            result.push_back({target++,slot,false,source.sets[id][slot]});
    for (std::uint32_t slot=0;slot<4;++slot)
        if (!filter[slot] && present[target]) result.push_back({target++,slot,true,source.dummyOff[slot]});
    output=std::move(result); error.clear(); return true;
}

SourceVertexLighting classifySourceVertexLighting(std::string_view filename, std::string_view preamble) {
    if (filename=="ProfileCOMMON_emul_VS.glsl") {
        const auto lighting=definesLighting(preamble);
        if (!lighting) return SourceVertexLighting::Unsupported;
        return *lighting ? SourceVertexLighting::CommonLit : SourceVertexLighting::CommonUnlit;
    }
    if (filename=="GL_Diffuse_L1_iPhone_VS.glsl") return SourceVertexLighting::DiffuseL1VertexColor;
    return SourceVertexLighting::Unsupported;
}

bool evaluateSourceCommonVertexLight(const SourceLight& light, const SourceCommonMaterialLighting& material,
                                    const std::array<float,3>& position, const std::array<float,3>& normal,
                                    std::array<float,4>& output, std::string& error) {
    if (!finite(position) || !finite(normal) || !finite(light.position) || !finite(light.attenuation)
        || !finite(light.ambient) || !finite(light.diffuse) || !finite(light.specular)
        || !finite(material.emission) || !finite(material.ambient) || !finite(material.diffuse)
        || !finite(material.specular) || !finite(material.sceneAmbient) || !std::isfinite(material.shininess)) {
        error="Nonfinite source lighting input"; return false;
    }
    std::array<float,3> vector;
    float squared=0;
    for (std::size_t i=0;i<3;++i) { vector[i]=light.position[i]-position[i]*light.position[3]; squared+=vector[i]*vector[i]; }
    const float distance=std::sqrt(squared);
    const float denominator=light.attenuation[0]+light.attenuation[1]*distance+light.attenuation[2]*distance*distance;
    if (!std::isfinite(distance) || distance<=0 || !std::isfinite(denominator) || denominator<=0) {
        error="Source shader normalization/attenuation outside finite evaluation domain"; return false;
    }
    for (auto& value:vector) value/=distance;
    float nDot=0; for (std::size_t i=0;i<3;++i) nDot+=normal[i]*vector[i]; nDot=std::max(0.0f,nDot);
    std::array<float,3> half{vector[0],vector[1],vector[2]+1.0f};
    const float halfSquared=half[0]*half[0]+half[1]*half[1]+half[2]*half[2];
    if (halfSquared<=0) { error="Source half-vector normalization undefined"; return false; }
    float nDotHalf=0; for (std::size_t i=0;i<3;++i) nDotHalf+=normal[i]*half[i]/std::sqrt(halfSquared);
    const float power=nDot>0 ? std::pow(std::max(0.0f,nDotHalf),std::max(material.shininess,0.0001f)) : 0;
    const float attenuation=1.0f/denominator;
    std::array<float,4> result;
    for (std::size_t i=0;i<4;++i)
        result[i]=std::clamp(material.emission[i]+(material.sceneAmbient[i]+light.ambient[i]*attenuation)*material.ambient[i]
            + light.diffuse[i]*nDot*attenuation*material.diffuse[i]
            + light.specular[i]*power*attenuation*material.specular[i],0.0f,1.0f);
    if (!finite(result)) { error="Nonfinite source lighting output"; return false; }
    output=result; error.clear(); return true;
}

} // namespace dh::foundation
