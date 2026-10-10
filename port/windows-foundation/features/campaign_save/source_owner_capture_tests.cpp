#include "source_owner_capture.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation::campaign_save;
void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
std::vector<std::uint8_t> read(const std::string& path){std::ifstream in(path,std::ios::binary);check(bool(in),"Missing actual quest cache");return {std::istreambuf_iterator<char>(in),{}};}
int main(int argc,char** argv){try{
    check(argc==2,"Expected actual quest cache directory");std::string error;
    auto rows=read(std::string(argv[1])+"/v2quests_pyarray.bin");
    auto names=read(std::string(argv[1])+"/v2quests_pyarraynames.bin");
    auto tables=std::make_shared<dh2::data::QuestTablesPersistenceV51>();
    check(tables->decode({rows.data(),rows.size()},{names.data(),names.size()},error),error);
    // Identity/lease is a fixture; actual native tables and collection owners
    // are exercised. This is NOT a ready gameplay Character factory.
    auto character=std::make_shared<int>(1);
    auto save=std::make_shared<dh2::data::PlayerSavegameV1>();
    save->set_character(reinterpret_cast<std::uintptr_t>(character.get()));
    auto owner=std::make_shared<dh2::character::CharacterMenuQuestsV51>(save,tables);
    check(owner->initialize(0,error)&&owner->initialize(1,error),error);
    auto registry=std::make_shared<SourceOwnerCapture>();
    check(registry->add_quest_collections(owner,character,{"profile/regular","profile/volatile"},"actual-cache-revision",error),error);
    check(!registry->add_quest_collections(owner,character,{"duplicate/a","duplicate/b"},"actual-cache-revision",error),"Duplicate owner accepted");
    auto services=registry->services();std::vector<SourceRequirement> required;
    check(services.enumerate(required,error)&&required.size()==2,error);
    for(const auto& r:required){SourceSection section;check(services.capture(r,section,error)&&services.validate(section,error),error);
        auto bad=section;bad.bytes.push_back(0);check(!services.validate(bad,error),"Trailing source bytes accepted");
        bad=section;bad.requirement.definition_revision="wrong";check(!services.validate(bad,error),"Changed revision accepted");}
    check(registry->require_unsupported({"profile/lifecycle","actual-cache-revision",SourceCodec::actor_lifecycle_v3},error),error);
    check(services.enumerate(required,error)&&required.size()==3,error);
    SourceSection preserved;preserved.bytes={99};
    check(!services.capture(required.back(),preserved,error)&&preserved.bytes==std::vector<std::uint8_t>{99},"Unsupported fragment mutated output");
    check(registry->require_unsupported({"profile/native-save","actual-cache-revision",SourceCodec::player_profile_source_v1},error),error);
    check(services.enumerate(required,error)&&required.size()==4,error);
    check(!services.capture(required.back(),preserved,error)&&preserved.bytes==std::vector<std::uint8_t>{99},"Metadata mistaken for complete profile codec");
    check(!registry->require_unsupported({"profile/fake","revision",SourceCodec::quest_collection_v45},error),"Supported codec admitted without owner");
    registry.reset();required.clear();check(!services.enumerate(required,error)&&required.empty(),"Expired inventory succeeded");
    std::cout<<"PASS same-source quest owner capture/validation rows="<<tables->rows().size()<<" collections=2 unsupported-rejection=2 expired-lease=1\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
