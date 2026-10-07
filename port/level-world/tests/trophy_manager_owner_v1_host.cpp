#include "../trophy_manager_owner_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace dh2;using namespace dh2::trophies;
namespace {unsigned checks=0;void check(bool v){++checks;if(!v){std::cerr<<"failed check "<<checks<<std::endl;std::abort();}}
std::vector<std::uint8_t> read(const char*name){std::ifstream f(std::string(".local-inputs/trophy-owner-assets-v1/")+name,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
int missing(void*,const char*,std::uintptr_t*out){*out=0;return 0;}int close(void*,std::uintptr_t){std::abort();}
struct Fixture {unsigned texts=0,queries=0,messages=0,saves=0;std::array<std::uint32_t,4> last{};
 static int invoke(void*p,const TrophyRequestV1*q,TrophyResponseV1*r){auto&f=*static_cast<Fixture*>(p);switch(q->service){
 case resolve_text_v1:++f.texts;r->text="fixture-text-"+std::to_string(q->text_id);return 0;
 case application_in_game_v1:++f.queries;r->boolean=true;return 0;
 case queue_message_v1:++f.messages;check(q->message&&!q->message->name.empty());return 0;
 case save_bitmap_v1:++f.saves;check(q->bitmap);f.last=*q->bitmap;return 0;default:return -1;}}
};}
int main(){auto rows=read("trophies_pyarray.bin"),names=read("trophies_pyarraynames.bin"),schema=read("trophies_pystructnames.bin");std::string error;
auto catalog=TrophyCatalogV1::load({rows.data(),rows.size()},{names.data(),names.size()},{schema.data(),schema.size()},error);check(bool(catalog));check(catalog->rows().size()==69);check(catalog->index("epic_withskills")==36);check(catalog->index("use_100_potions")==51);check(catalog->index("absent")==-1);
auto*debug=dh2_character_debug_create();character::DebugFileServices24 files{nullptr,missing,close};check(debug);
auto absent=TrophyManagerOwnerV1::create(catalog,debug,&files,{},error);check(bool(absent));check(absent->initialized());
std::ifstream oracle_file("port/level-world/reference/trophy-manager-owner-v1/constructor-source-fixture.bin",std::ios::binary);std::vector<std::uint8_t> oracle{std::istreambuf_iterator<char>(oracle_file),{}};check(oracle.size()==4+69*36);
for(unsigned i=0;i<69;++i){const auto&r=catalog->rows()[i];auto*t=absent->get(i);check(t&&t->id==int(i));check(t->name==r.name);check(t->desc==r.desc);check(t->type==r.type);check(t->grade==r.grade);check(t->label==r.label);check(t->gl_live==r.gl_live);check(t->gl_index==r.gl_index);check(!t->unlocked);
 std::array<std::int32_t,9> actual{t->id,t->name,t->desc,t->unlocked,t->type,t->grade,t->label,t->gl_live,t->gl_index};for(unsigned j=0;j<9;++j){std::int32_t expected;std::memcpy(&expected,oracle.data()+4+i*36+j*4,4);check(actual[j]==expected);}}
check(absent->unlock(-1)==0);check(absent->unlock(36)==-2);check(absent->is_unlocked(36));check(absent->is_unlocking(36));check(absent->unlock(36)==0);check(absent->error().empty());
std::array<std::uint32_t,4> bits{};check(absent->bitmap(bits)==0);check(bits[1]==16);
Fixture fixture;auto owner=TrophyManagerOwnerV1::create(catalog,debug,&files,{&fixture,Fixture::invoke},error);check(bool(owner));TrophyNativeBindingsV1 native(*owner);
character::skills::SkillAIRequest32V3 q{};character::skills::SkillAIResponse32V3 out{};q.operation=character::skills::skill_ai_trophy_manager_v3;check(native.skill_ai(&q,&out)==0);check(out.identity==reinterpret_cast<std::uintptr_t>(owner.get()));
q.operation=character::skills::skill_ai_trophy_catalog_v3;check(native.skill_ai(&q,&out)==0);check(out.count==69);check(std::string(out.names[36])=="epic_withskills");
check(native.unlock_named("epic_withskills")==0);check(fixture.texts==2&&fixture.queries==1&&fixture.messages==1&&fixture.saves==1);check(fixture.last[1]==16);check(owner->is_unlocking(36));
check(native.unlock_named("use_100_potions")==0);check(fixture.saves==2);check(fixture.last[1]==((1u<<4)|(1u<<19)));
auto replay=TrophyManagerOwnerV1::create(catalog,debug,&files,{&fixture,Fixture::invoke},error);check(bool(replay));check(replay->load_bitmap(fixture.last)==0);check(replay->is_unlocked(36)&&replay->is_unlocked(51));
auto trunc=rows;trunc.pop_back();check(!TrophyCatalogV1::load({trunc.data(),trunc.size()},{names.data(),names.size()},{schema.data(),schema.size()},error));check(catalog->rows().size()==69);
dh2_character_debug_destroy(debug);std::cout<<"{\"checks\":"<<checks<<",\"authored_rows\":69,\"complete_fixture_unlock\":true,\"production_unlock_services\":false}"<<std::endl;
}
