#include "xml_document_v1.hpp"
#include "tinyxml.h"
#include <algorithm>
#include <functional>
#include <stdexcept>

namespace dh2::loader {
struct XmlDocumentV1::Snapshot {
    std::string uri;
    std::vector<std::uint8_t> source;
    std::vector<XmlElementV1> elements;
    std::vector<std::uint32_t> roots;
    std::vector<XmlTopLevelNodeV1> top_level_nodes;
    bool level_buffer{};
    XmlDiagnosticV1 diagnostic;
};
const std::string* XmlElementV1::attribute(const std::string& key)const noexcept {
    for(const auto& a:attributes)if(a.first==key)return &a.second;
    return nullptr;
}
const std::string& XmlDocumentV1::Borrow::uri()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->uri;
}
const std::vector<std::uint8_t>& XmlDocumentV1::Borrow::source()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->source;
}
const std::vector<XmlElementV1>& XmlDocumentV1::Borrow::elements()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->elements;
}
const XmlDiagnosticV1& XmlDocumentV1::Borrow::diagnostic()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->diagnostic;
}
const std::vector<std::uint32_t>& XmlDocumentV1::Borrow::roots()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->roots;
}
const std::vector<XmlTopLevelNodeV1>& XmlDocumentV1::Borrow::top_level_nodes()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->top_level_nodes;
}
bool XmlDocumentV1::Borrow::used_level_buffer_route()const {
    if(!snapshot_)throw std::logic_error("XML snapshot unavailable");
    return snapshot_->level_buffer;
}
bool XmlDocumentV1::Borrow::parsed()const {
    return snapshot_&&!snapshot_->diagnostic.code&&!snapshot_->elements.empty();
}
bool XmlDocumentV1::capture(std::string uri,std::vector<std::uint8_t> bytes,std::string& error) {
    return capture_impl(std::move(uri),std::move(bytes),false,error);
}
bool XmlDocumentV1::capture_level_buffer(std::string uri,std::vector<std::uint8_t> bytes,std::string& error) {
    return capture_impl(std::move(uri),std::move(bytes),true,error);
}
bool XmlDocumentV1::capture_impl(std::string uri,std::vector<std::uint8_t> bytes,bool level_buffer,std::string& error) {
    error.clear();
    try {
        if(uri.empty()||uri.find('\0')!=std::string::npos||bytes.size()>16u*1024u*1024u||
           std::find(bytes.begin(),bytes.end(),0)!=bytes.end())
            throw std::runtime_error("XML input outside native capture domain");
        auto next=std::make_shared<Snapshot>();next->uri=std::move(uri);next->source=std::move(bytes);
        next->level_buffer=level_buffer;
        std::string raw;
        raw.reserve(next->source.size());
        for(std::size_t i=0;i<next->source.size();++i) {
            const auto byte=next->source[i];
            if(level_buffer&&byte=='\r') {
                raw+='\n';
                if(i+1<next->source.size()&&next->source[i+1]=='\n')++i;
            }else raw+=static_cast<char>(byte);
        }
        TiXmlDocument doc;doc.Parse(raw.c_str(),nullptr,TIXML_ENCODING_UNKNOWN);
        next->diagnostic={doc.ErrorId(),doc.ErrorRow(),doc.ErrorCol(),doc.ErrorDesc()};
        std::function<std::uint32_t(const TiXmlElement*,std::int32_t,unsigned)> append;
        append=[&](const TiXmlElement* e,std::int32_t parent,unsigned depth) {
            if(depth>256||next->elements.size()>=65536)throw std::runtime_error("XML element domain exceeded");
            const auto index=static_cast<std::uint32_t>(next->elements.size());
            XmlElementV1 node;node.tag=e->Value();node.parent=parent;node.row=e->Row();node.column=e->Column();
            node.next_sibling_is_non_element=e->NextSibling()&&!e->NextSibling()->ToElement();
            node.first_child_is_non_element=e->FirstChild()&&!e->FirstChild()->ToElement();
            if(const auto* text=e->GetText())node.text=text;
            for(auto* a=e->FirstAttribute();a;a=a->Next())node.attributes.emplace_back(a->Name(),a->Value());
            next->elements.push_back(std::move(node));
            for(auto* c=e->FirstChildElement();c;c=c->NextSiblingElement()) {
                auto child=append(c,static_cast<std::int32_t>(index),depth+1);
                next->elements[index].children.push_back(child);
            }
            return index;
        };
        for(auto* root=doc.FirstChild();root;root=root->NextSibling()) {
            XmlTopLevelNodeV1 node;node.kind=static_cast<std::uint32_t>(root->Type());node.value=root->Value();
            if(const auto* element=root->ToElement()) {
                node.element=append(element,-1,0);next->roots.push_back(node.element);
            }
            next->top_level_nodes.push_back(std::move(node));
        }
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
