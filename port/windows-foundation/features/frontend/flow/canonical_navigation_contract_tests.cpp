// Admission/failure contracts only. No successful fake CreateSaveSlot, source
// initialization, assignment, level start or checkpoint capture is provided.
#include "canonical_navigation.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
using namespace dh::foundation::frontend::flow;
namespace {
int checks{};void require(bool ok,const char* why){++checks;if(!ok)throw std::runtime_error(why);}
struct RejectingRoot final:CanonicalNavigationOwner{
    CharacterState sentinel;bool enabled=false,loan=true;int creates=0,assigns=0,starts=0,captures=0;
    // Sentinel only verifies address forwarding; it is not an initialized player.
    CharacterState* live_character()noexcept override{return loan?&sentinel:nullptr;}
    bool available(CanonicalOperation)const noexcept override{return enabled;}
    bool create_slot(CharacterState& same,const std::string& name,const std::string& cls,int&,std::string& e)override{
        require(&same==&sentinel&&name=="Hero"&&cls=="KnightPlayerBase","create uses exact canonical loan/request");++creates;e="full source initialization missing";return false;}
    bool assign_slot(CharacterState& same,int slot,int player,std::string& e)override{
        require(&same==&sentinel&&slot==2&&player==0,"assignment exact loan/args");++assigns;e="indexed SG_Load unavailable";return false;}
    bool start_game(CharacterState& same,int difficulty,std::string& e)override{
        require(&same==&sentinel&&difficulty==1,"start exact loan/difficulty");++starts;e="actual level-start owner unavailable";return false;}
    bool capture_checkpoint(const CharacterState& same,GameSave& candidate,std::string& e)override{
        require(&same==&sentinel,"checkpoint exact canonical loan");++captures;candidate.level_uri="failed-capture-sentinel";e="registered source lifecycle incomplete";return false;}
};
}
int main(){try{
    std::string error;int slot=-1;
    auto unavailable=bind_canonical_navigation({});
    require(!unavailable.create_save("Hero","KnightPlayerBase",slot,error),"missing owner never creates");
    require(error=="NativeCreateSaveSlot: canonical root owner unavailable"&&slot==-1,"missing owner output unchanged");
    auto owner=std::make_shared<RejectingRoot>();auto services=bind_canonical_navigation(owner);
    require(!services.create_save("Hero","KnightPlayerBase",slot,error)&&owner->creates==0,"unavailable create does not dispatch");
    owner->enabled=true;owner->loan=false;
    require(!services.assign_save(2,0,error)&&owner->assigns==0,"missing canonical loan does not dispatch");
    owner->loan=true;
    Navigator nav(services);require(nav.single_player({0,false},error)&&nav.accept_name("Hero",error),"pure navigation to real service gate");
    require(!nav.confirm_class(error)&&owner->creates==1&&owner->assigns==0&&owner->starts==0,"create failure blocks later source prefixes");
    require(error=="NativeCreateSaveSlot: full source initialization missing"&&nav.creation_stage()==CreationStage::editing,"source create failure retained");
    require(!services.assign_save(2,0,error)&&owner->assigns==1,"actual assignment rejection forwarded");
    require(!services.start_game(1,error)&&owner->starts==1,"actual start rejection forwarded");
    require(error=="NativeStartGame: actual level-start owner unavailable","source start failure retained");
    GameSave saved;saved.level_uri="original-destination";
    require(!capture_canonical_checkpoint(*owner,saved,error)&&owner->captures==1&&saved.level_uri=="original-destination","failed source capture preserves same canonical snapshot destination");
    std::cout<<"PASS canonical navigation admission/failure contracts: "<<checks<<" assertions; no source success simulated\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
