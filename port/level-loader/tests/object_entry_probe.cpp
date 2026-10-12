#include "object_entry_v1.hpp"
#include <iomanip>
#include <iostream>
#include <stdexcept>
using namespace dh2::loader;
namespace {
std::string quote(const std::string& text) {
    std::string out="\"";
    const char* hex="0123456789abcdef";
    for (unsigned char c:text) {
        if(c=='"'||c=='\\') {out+='\\';out+=static_cast<char>(c);}
        else if(c<32) {out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}
        else out+=static_cast<char>(c);
    }return out+'"';
}
std::string xml(const std::string& text) {
    std::string out;
    for(char c:text) {
        if(c=='&')out+="&amp;";else if(c=='<')out+="&lt;";else if(c=='>')out+="&gt;";
        else if(c=='"')out+="&quot;";else out+=c;
    }return out;
}
void require(bool ok,const std::string& message) {if(!ok)throw std::runtime_error(message);}
}
int main() {
    try {
        unsigned count{};require(static_cast<bool>(std::cin>>count),"Missing fixture count");
        std::cout<<"{\"registry\":[";
        unsigned index{};
        for(const auto& entry:original_factories_v1()) {
            if(index++)std::cout<<',';
            std::cout<<"{\"gametype\":"<<quote(entry.gametype)<<",\"original_address\":"<<entry.original_address<<'}';
        }
        std::cout<<"],\"cases\":[";
        for(unsigned i=0;i<count;++i) {
            std::string label,route,filter;bool filter_present{};unsigned attr_count{};
            require(static_cast<bool>(std::cin>>std::quoted(label)>>route>>filter_present>>std::quoted(filter)>>attr_count),"Missing entry header");
            std::string bytes="<GameObject";
            for(unsigned j=0;j<attr_count;++j) {
                std::string name,value;require(static_cast<bool>(std::cin>>std::quoted(name)>>std::quoted(value)),"Missing fixture attribute");
                bytes+=' '+name+"=\""+xml(value)+'"';
            }
            bytes+="><ProbeMarker untouched=\"yes\"/></GameObject>";
            ObjectEntryV1 entry;std::string error;
            {
                XmlDocumentV1 doc;
                require(doc.capture(label,{bytes.begin(),bytes.end()},error),error);
                require(prepare_object_entry_v1(doc.borrow(),doc.borrow().roots().at(0),
                    route=="level"?ObjectEntryRouteV1::level:ObjectEntryRouteV1::manager,
                    filter_present?std::optional<std::string>(filter):std::nullopt,entry,error),error);
            }
            // Source lifetime survives facade destruction; nested declarations
            // and unrelated attributes stay reachable through the same borrow.
            require(entry.document.uri()==label && entry.source().children.size()==1
                && entry.document.elements().at(entry.source().children.at(0)).tag=="ProbeMarker","Source owner lost");
            const auto prior=entry.document.uri();
            require(!prepare_object_entry_v1({},0,ObjectEntryRouteV1::manager,{},entry,error)
                && entry.document.uri()==prior,"Rejected candidate replaced previous entry");
            if(i)std::cout<<',';
            std::cout<<"{\"label\":"<<quote(label)<<",\"disposition\":"<<quote(object_entry_disposition_v1(entry.disposition))
                <<",\"factory_address\":"<<(entry.factory?entry.factory->original_address:0)
                <<",\"template_present\":"<<(entry.template_present?"true":"false")
                <<",\"early_init_post\":"<<(entry.early_init_post?"true":"false")
                <<",\"force_id_minus_one\":"<<(entry.force_id_minus_one?"true":"false")<<",\"attributes\":{";
            for(std::size_t j=0;j<entry.source().attributes.size();++j) {
                if(j)std::cout<<',';
                const auto& attribute=entry.source().attributes[j];std::cout<<quote(attribute.first)<<':'<<quote(attribute.second);
            }std::cout<<"},\"ownership_and_failure_checks\":true}";
        }std::cout<<"]}\n";
    }catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 1;}
}
