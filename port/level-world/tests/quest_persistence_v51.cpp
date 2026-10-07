#include "character_menu_quests_v51.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2;
namespace {
unsigned checks{};
void check(bool v,const std::string& message){++checks;if(!v)throw std::runtime_error(message);}
std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Required actual Quest cache file");return {std::istreambuf_iterator<char>(f),{}};}
data::Bytes span(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
}
int main(int argc,char** argv){try{
 if(argc!=3)throw std::runtime_error("actual v2quests_pyarray.bin and names required");
 auto array=read(argv[1]),names=read(argv[2]);std::string e;
 auto tables=std::make_shared<data::QuestTablesPersistenceV51>();
 check(tables->decode(span(array),span(names),e),e);check(tables->rows().size()==64,"Actual cached Quest count");
 check(tables->rows().front().name=="Abbey_Assassin","Actual first Quest name");
 auto save=std::make_shared<data::PlayerSavegameV1>();auto owner=std::make_shared<character::CharacterMenuQuestsV51>(save,tables);
 check(owner->initialize(0,e),e.c_str());check(owner->initialize(1,e),e.c_str());
 std::size_t quantity_leaves{},base_leaves{};
 data::QuestPersistenceOwnerV51 probe;data::QuestSavegameV1 probe_collection;
 check(probe.construct(tables,0x1234,probe_collection,e),e.c_str());
 for(unsigned d=0;d<3;++d)for(auto id:probe_collection.source_quests_v45()[d]){
  auto* q=probe.resolve(id);check(q&&q->character_owner==0x1234,"Same actual Character owner");
  q->state=5;std::vector<std::uint8_t> b;check(probe.save_quest(id,b,e),e.c_str());
  const auto expected=[&](const auto& o){if(o.definition->type==4||o.definition->type==6||o.definition->type==12){++base_leaves;return std::size_t(1);}++quantity_leaves;return std::size_t(5);};
  std::size_t size=4+expected(q->accept)+expected(q->end);for(const auto& o:q->objectives)size+=expected(o);
  check(b.size()==size,"Exact Objective source virtual wire choice");q->state=-1;std::size_t used{};
  check(probe.load_quest(id,span(b),used,e),e.c_str());check(used==b.size()&&q->state==5&&q->volatile64==1,"Exact Quest state/volatile source store");
  b.resize(4);q->state=-1;check(!probe.load_quest(id,span(b),used,e)&&q->state==5&&used==4,"Reached state retained on truncated objective");
 }
 check(quantity_leaves&&base_leaves,"Actual cached both wire families");
 check(!probe.reinitialize({},e)&&e.find("old-state")!=std::string::npos,"No fake gameplay unregister during ReInit");
 auto writer=owner->writer(0);level::SavegameStreamV2 stream;
 check(level::quest_save_collection_v45(writer,stream,e),e.c_str());std::size_t consumed{};
 check(owner->load(span(stream.bytes()),consumed,e),e.c_str());check(consumed==stream.bytes().size(),"QEST actual full rewind load");
 std::array<std::int32_t,3> acts{{7,8,9}},other{{10,11,12}};
 check(data::load_quest_metadata_acts_v51(tables,span(stream.bytes()),acts,other,e),e.c_str());
 check(acts==save->regular_quests_v45().progress().current_act&&other==acts,"Menu metadata actual QEST acts");
 auto short_array=array;short_array.pop_back();data::QuestTablesPersistenceV51 failed_table;
 check(!failed_table.decode(span(short_array),span(names),e),"Truncated table rejects");
 check(!tables->decode(span(array),span(names),e),"Live immutable table cannot replay");
 check(tables->rows().size()==64,"Failed decode preserves immutable published rows");
 std::cout<<"PASS "<<checks<<" actual-cache persistence checks (not gameplay Quest registration)\n";return 0;
 }catch(const std::exception& x){std::cerr<<x.what()<<'\n';return 1;}}
