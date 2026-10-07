#include "../authored_hud_options_bridge_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::ui;
unsigned checks{};void check(bool b){++checks;if(!b)throw std::runtime_error("bridge check "+std::to_string(checks));}
std::vector<std::uint8_t> read(const char* name){std::ifstream f(name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
struct Probe {bool fail{},override{};int support=-1;std::vector<unsigned> calls;};
int backend(void* p,const HudInitRequest64* q,HudInitResponse32* out){auto& t=*static_cast<Probe*>(p);t.calls.push_back(static_cast<unsigned>(q->operation));if(t.fail)return 0;
 switch(q->operation){case HudInitOperation::language_override:out->value=t.override;return 1;case HudInitOperation::string_symbol:out->text="actual symbol observer";return 1;case HudInitOperation::publish_language:return 1;case HudInitOperation::platform_music_support:out->value=t.support;return 1;default:return 0;}}
int main(){try{std::string e;GameOptionTableV1 table;const auto d=read(".local-inputs/design-settings/design_pyarray.bin"),n=read(".local-inputs/design-settings/design_pyarraynames.bin"),s=read(".local-inputs/design-settings/design_pystructnames.bin");check(table.load_design_cache({d.data(),d.size()},{n.data(),n.size()},{s.data(),s.size()},e));OwnedHudSettingsV1 settings(table.borrow());settings.initialize_defaults();check(settings.set_option("HUDStyle",2));Probe probe;HudInitServices16 services{&probe,backend};CharacterMenuCallV1 call;call.arguments={CharacterMenuValueV1::string("HUDStyle"),CharacterMenuValueV1::reference(123)};std::vector<std::pair<std::string,CharacterMenuValueV1>> members;
 call.member=[&](auto id,const char* key,const auto& v,std::string&){check(id==123);members.emplace_back(key,v);return true;};
 check(authored_hud_options_bridge_v1("NativeGetOptionParameters",call,settings,services,e));check(members.size()==3&&members[0].first=="NumOptions"&&members[1].first=="CurrentOption"&&members[1].second.number==2&&members[2].first=="OptionString");check(call.result.object==123);
 call.arguments.clear();for(int support:{-1,0,1,2}){probe.support=support;check(authored_hud_options_bridge_v1("NativeUseIpodPlayer",call,settings,services,e));check(call.result.boolean==(support==1));}
 probe.fail=true;e.clear();const auto before=call.result;check(!authored_hud_options_bridge_v1("NativeUseIpodPlayer",call,settings,services,e));check(call.result.kind==before.kind&&call.result.boolean==before.boolean&&!e.empty());
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<"}\n";
 }catch(const std::exception& e){std::cerr<<e.what();return 1;}}
