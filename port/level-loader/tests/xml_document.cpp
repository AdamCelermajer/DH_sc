#include "xml_document_v1.hpp"
#include <iostream>
#include <stdexcept>

using dh2::loader::XmlDocumentV1;
static void require(bool value,const char* why) {
    if(!value)throw std::runtime_error(why);
}
static std::vector<std::uint8_t> bytes(const std::string& s) {
    return {s.begin(),s.end()};
}
int main() {
    try {
        XmlDocumentV1::Borrow retained;
        std::string error;
        {
            XmlDocumentV1 owner;
            require(!owner.borrow(),"fresh owner has a snapshot");
            const auto source=bytes("<level name=swamp><object id='chest' label='A &amp; B'/></level>");
            require(owner.capture("data/scene/001_swamp.mlx",source,error),"capture failed");
            retained=owner.borrow();
            require(retained.parsed()&&retained.source()==source,"raw bytes not retained");
            require(retained.elements().size()==2,"element count");
            const auto& child=retained.elements().at(1);
            require(child.parent==0&&retained.elements().at(0).children.at(0)==1,"parent/child ownership");
            require(child.attribute("label")&&*child.attribute("label")=="A & B","attribute entity");
            require(!child.attribute("missing"),"missing attribute fabricated");
            require(!owner.capture("",bytes("<new/>"),error),"empty resource accepted");
            require(owner.borrow().uri()==retained.uri(),"failed capture replaced active snapshot");
            auto nul=bytes("<new/>");nul.push_back(0);
            require(!owner.capture("nul",std::move(nul),error),"embedded NUL accepted");
            require(owner.capture("partial",bytes("<a><b/></A>"),error),"diagnostic capture failed");
            require(!owner.borrow().parsed()&&owner.borrow().diagnostic().code,"malformed XML granted parsed status");
            require(owner.capture("replacement",bytes("<other/>"),error),"replacement failed");
            require(owner.capture("two-level-roots",bytes("<Level name='a'/><Level name='b'/>"),error),"multiple root capture failed");
            require(owner.borrow().roots().size()==2,"multiple top-level authored roots dropped");
            require(retained.elements().at(1).tag=="object", "publication invalidated older borrow");
        }
        require(retained.parsed()&&retained.elements().at(0).tag=="level", "owner destruction invalidated borrow");
        require(retained.uri()=="data/scene/001_swamp.mlx","resource identity changed");
        std::cout<<"{\"validation\":\"PASS\",\"scope\":\"XML retained ownership, replacement, rejection and diagnostics\"}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
