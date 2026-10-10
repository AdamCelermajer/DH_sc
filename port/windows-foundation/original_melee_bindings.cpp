#include "original_melee_bindings.hpp"
#include "content_paths.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <locale>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace dh::foundation {
namespace {
constexpr std::size_t maxBytes = 16 * 1024 * 1024;
constexpr std::size_t maxElements = 200000;
constexpr std::size_t maxDepth = 128;
std::string clip_key(const std::string& uri) {
    auto key=normalize_content_uri(uri);
    std::transform(key.begin(),key.end(),key.begin(),[](unsigned char c){return char(std::tolower(c));});
    return key;
}

OriginalBindingProperties attributes(const TiXmlElement& node) {
    OriginalBindingProperties result;
    for (const auto* property = node.FirstAttribute(); property; property = property->Next())
        result.emplace(property->Name(), property->Value());
    return result;
}
std::string field(const TiXmlElement& node, const char* key, bool required = true) {
    const char* value = node.Attribute(key);
    if (required && (!value || !*value)) throw std::runtime_error(std::string("Missing melee binding field: ") + key);
    return value ? value : "";
}
std::int64_t integer(const TiXmlElement& node, const char* key) {
    const auto text = field(node, key);
    std::size_t consumed = 0;
    const auto value = std::stoll(text, &consumed);
    if (consumed != text.size()) throw std::runtime_error(std::string("Invalid melee integer: ") + key);
    return value;
}
double real(const TiXmlElement& node, const char* key) {
    std::istringstream stream(field(node, key)); stream.imbue(std::locale::classic());
    double result = 0;
    if (!(stream >> result) || !std::isfinite(result)) throw std::runtime_error(std::string("Invalid melee scalar: ") + key);
    stream >> std::ws;
    if (!stream.eof()) throw std::runtime_error(std::string("Trailing melee scalar: ") + key);
    return result;
}
void preflight(std::string_view text) {
    std::size_t nodes = 0, depth = 0;
    for (std::size_t at = 0; at < text.size();) {
        const auto start = text.find('<', at);
        if (start == std::string_view::npos) break;
        std::string_view ending;
        std::size_t skip = 0;
        if (text.substr(start,4) == "<!--") { skip=4; ending="-->"; }
        else if (text.substr(start,9) == "<![CDATA[") { skip=9; ending="]]>"; }
        else if (text.substr(start,2) == "<?") { skip=2; ending="?>"; }
        else if (text.substr(start,2) == "<!") throw std::runtime_error("Melee bindings DTD is unsupported");
        if (skip) {
            const auto end = text.find(ending,start+skip);
            if (end == std::string_view::npos) throw std::runtime_error("Unterminated melee XML declaration/comment");
            at=end+ending.size(); continue;
        }
        std::size_t end=start+1;
        char quote=0;
        for (;end<text.size();++end) {
            const char c=text[end];
            if (quote) { if(c==quote) quote=0; }
            else if(c=='\''||c=='"') quote=c;
            else if(c=='>') break;
        }
        if(end==text.size()) throw std::runtime_error("Unterminated melee XML tag");
        if(text[start+1]=='/') {
            if(!depth) throw std::runtime_error("Unexpected melee XML closing tag");
            --depth;
        } else {
            if(++nodes>maxElements||++depth>maxDepth) throw std::runtime_error("Melee XML element/depth limit exceeded");
            std::size_t previous=end;
            while(previous>start&&std::isspace(static_cast<unsigned char>(text[previous-1]))) --previous;
            if(text[previous-1]=='/') --depth;
        }
        at=end+1;
    }
}
OriginalBindingNode preserve(const TiXmlElement& node) {
    OriginalBindingNode result;
    result.tag=node.Value(); result.properties=attributes(node);
    for (auto* child=node.FirstChild();child;child=child->NextSibling()) {
        if (const auto* element=child->ToElement()) result.children.push_back(preserve(*element));
        else if (const auto* text=child->ToText()) result.text+=text->Value();
    }
    return result;
}
OriginalMeleeStep step(const TiXmlElement& node) {
    OriginalMeleeStep result;
    result.properties=attributes(node);
    result.index=integer(node,"index"); result.animationId=integer(node,"animationId");
    result.redirect=integer(node,"redirect"); result.speed=real(node,"speed");
    result.blendOut=integer(node,"blendOut"); result.moveGO=integer(node,"moveGO");
    result.uri=field(node,"uri",false);
    if(result.index<0) throw std::runtime_error("Negative melee step index");
    std::set<std::int64_t> indices;
    for(auto* child=node.FirstChildElement();child;child=child->NextSiblingElement()) {
        if(std::string(child->Value())=="step") {
            auto nested=step(*child);
            if(!indices.insert(nested.index).second) throw std::runtime_error("Duplicate nested melee step index");
            result.children.push_back(std::move(nested));
        } else result.extraNodes.push_back(preserve(*child));
    }
    return result;
}
OriginalMeleeSequence sequence(const TiXmlElement& node) {
    OriginalMeleeSequence result;
    result.properties=attributes(node);
    result.id=integer(node,"id"); result.name=field(node,"name");
    result.loop=integer(node,"loop"); result.type=integer(node,"type");
    std::set<std::int64_t> indices;
    for(auto* child=node.FirstChildElement();child;child=child->NextSiblingElement()) {
        if(std::string(child->Value())=="step") {
            auto item=step(*child);
            if(!indices.insert(item.index).second) throw std::runtime_error("Duplicate melee sequence step index");
            result.steps.push_back(std::move(item));
        } else result.extraNodes.push_back(preserve(*child));
    }
    return result;
}
}

bool OriginalMeleeBindings::load(const AssetCatalog& assets, const std::string& uri, std::string& error) {
    try {
        const auto path=resolve_content_path(assets,uri);
        if(std::filesystem::file_size(path)>maxBytes) throw std::runtime_error("Melee bindings exceed byte limit");
        return decode(read_content(assets,uri),error);
    } catch(const std::exception& exception) { error=exception.what(); return false; }
}
bool OriginalMeleeBindings::decode(const std::vector<std::uint8_t>& bytes, std::string& error) {
    try {
        if(bytes.empty()||bytes.size()>maxBytes) throw std::runtime_error("Melee bindings XML size outside limits");
        if(std::find(bytes.begin(),bytes.end(),0)!=bytes.end()) throw std::runtime_error("Melee bindings contain a NUL byte");
        const std::string xml(bytes.begin(),bytes.end()); preflight(xml);
        TiXmlDocument document; document.Parse(xml.c_str());
        if(document.Error()) throw std::runtime_error(document.ErrorDesc());
        auto* root=document.RootElement();
        if(!root||root->NextSiblingElement()||std::string(root->Value())!="meleeBindings"||field(*root,"version")!="1")
            throw std::runtime_error("Expected meleeBindings version 1 root");
        OriginalMeleeBindings next;
        next.properties_=attributes(*root);
        bool factionsSeen=false, clipsSeen=false;
        for(auto* child=root->FirstChildElement();child;child=child->NextSiblingElement()) {
            const std::string tag=child->Value();
            if(tag=="factions") {
                if(factionsSeen) throw std::runtime_error("Duplicate factions section");
                factionsSeen=true;
                if(child->Attribute("fallbackId")) next.fallback_=integer(*child,"fallbackId");
                for(auto* row=child->FirstChildElement();row;row=row->NextSiblingElement()) {
                    if(std::string(row->Value())!="faction") throw std::runtime_error("Unknown faction entry");
                    OriginalFaction faction;
                    faction.id=integer(*row,"id"); faction.name=field(*row,"name"); faction.properties=attributes(*row);
                    for(auto* relation=row->FirstChildElement();relation;relation=relation->NextSiblingElement()) {
                        if(std::string(relation->Value())!="relation") throw std::runtime_error("Unknown faction relationship entry");
                        if(!faction.relations.emplace(integer(*relation,"target"),integer(*relation,"value")).second)
                            throw std::runtime_error("Duplicate directed faction relationship");
                    }
                    if(!next.factions_.emplace(faction.id,std::move(faction)).second) throw std::runtime_error("Duplicate faction ID");
                }
            } else if(tag=="clips") {
                if(clipsSeen) throw std::runtime_error("Duplicate melee clips section");
                clipsSeen=true;
                for(auto* row=child->FirstChildElement();row;row=row->NextSiblingElement()) {
                    if(std::string(row->Value())!="clip") { next.extraNodes_.push_back(preserve(*row)); continue; }
                    OriginalMeleeClip clip;
                    clip.uri=field(*row,"uri"); clip.startMs=integer(*row,"startMs"); clip.endMs=integer(*row,"endMs");
                    clip.properties=attributes(*row);
                    if(clip.endMs<clip.startMs) throw std::runtime_error("Inverted original clip range");
                    for(auto* item=row->FirstChildElement();item;item=item->NextSiblingElement()) {
                        if(std::string(item->Value())=="marker") {
                            OriginalMeleeMarker marker;
                            marker.name=field(*item,"name"); marker.timeMs=integer(*item,"timeMs");
                            marker.authoredTimeMs=integer(*item,"authoredTimeMs"); marker.properties=attributes(*item);
                            clip.markers.push_back(std::move(marker));
                        } else clip.extraNodes.push_back(preserve(*item));
                    }
                    if(!next.clips_.emplace(clip_key(clip.uri),std::move(clip)).second) throw std::runtime_error("Duplicate melee clip URI");
                }
            } else if(tag=="actor") {
                OriginalMeleeActor actor;
                actor.id=field(*child,"id"); actor.propertyRow=integer(*child,"propertyRow");
                actor.factionId=integer(*child,"factionId"); actor.aiId=integer(*child,"aiId");
                actor.aiName=field(*child,"aiName"); actor.model=field(*child,"model");
                actor.templateClip=field(*child,"template",false); actor.properties=attributes(*child);
                bool aiSeen=false;
                for(auto* section=child->FirstChildElement();section;section=section->NextSiblingElement()) {
                    const std::string kind=section->Value();
                    if(kind=="ai") {
                        if(aiSeen) throw std::runtime_error("Duplicate actor AI metadata");
                        aiSeen=true; actor.aiProperties=attributes(*section);
                        if(section->FirstChildElement()) actor.extraNodes.push_back(preserve(*section));
                    } else if(kind=="state") {
                        const auto name=field(*section,"name");
                        std::vector<OriginalMeleeSequence> variants;
                        for(auto* candidate=section->FirstChildElement();candidate;candidate=candidate->NextSiblingElement()) {
                            if(std::string(candidate->Value())=="sequence") variants.push_back(::dh::foundation::sequence(*candidate));
                            else actor.extraNodes.push_back(preserve(*candidate));
                        }
                        if(!actor.states.emplace(name,std::move(variants)).second) throw std::runtime_error("Duplicate actor sequence state");
                        // Retain any conditions/symbolic attributes on the state.
                        if(attributes(*section).size()>1) actor.extraNodes.push_back(preserve(*section));
                    } else actor.extraNodes.push_back(preserve(*section));
                }
                if(!next.actors_.emplace(actor.id,std::move(actor)).second) throw std::runtime_error("Duplicate melee actor ID");
                if(next.actors_.size()>4096) throw std::runtime_error("Melee actor count limit exceeded");
            } else next.extraNodes_.push_back(preserve(*child));
        }
        if(!factionsSeen||next.factions_.empty()||next.actors_.empty()) throw std::runtime_error("Melee bindings require factions and actors");
        if(next.fallback_&&!next.factions_.count(*next.fallback_)) throw std::runtime_error("Authored fallback faction missing from table");
        *this=std::move(next); error.clear(); return true;
    } catch(const std::exception& exception) { error=exception.what(); return false; }
}
const OriginalMeleeActor* OriginalMeleeBindings::find_actor(const std::string& profileId) const noexcept {
    const auto found=actors_.find(profileId); return found==actors_.end()?nullptr:&found->second;
}
const OriginalMeleeClip* OriginalMeleeBindings::find_clip(const std::string& uri) const {
    if(uri.empty()) return nullptr;
    const auto found=clips_.find(clip_key(uri)); return found==clips_.end()?nullptr:&found->second;
}
std::vector<const OriginalMeleeActor*> OriginalMeleeBindings::actors_for_row(std::int64_t row) const {
    std::vector<const OriginalMeleeActor*> result;
    for(const auto& actor:actors_) if(actor.second.propertyRow==row) result.push_back(&actor.second);
    return result;
}
const OriginalMeleeSequence* OriginalMeleeBindings::sequence(const std::string& profileId, const std::string& state,
                                                            std::size_t explicitVariant) const noexcept {
    const auto* actor=find_actor(profileId); if(!actor) return nullptr;
    const auto found=actor->states.find(state);
    return found==actor->states.end()||explicitVariant>=found->second.size()?nullptr:&found->second[explicitVariant];
}
std::optional<std::int64_t> OriginalMeleeBindings::relationship(std::int64_t sourceFaction, std::int64_t targetFaction) const noexcept {
    const auto source=factions_.find(sourceFaction); if(source==factions_.end()) return {};
    const auto relation=source->second.relations.find(targetFaction);
    return relation==source->second.relations.end()?std::optional<std::int64_t>{}:relation->second;
}

} // namespace dh::foundation
