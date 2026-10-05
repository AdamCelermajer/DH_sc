#include "../character_menu_stats_owner_v1.hpp"
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
using namespace dh2;
static void check(bool x,const std::string& e){if(!x)throw std::runtime_error(e);}
static std::vector<std::uint8_t> file(const std::string& p){std::ifstream f(p,std::ios::binary);check(bool(f),p);return {std::istreambuf_iterator<char>(f),{}};}
static data::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
int main(int argc,char** argv){try{
 check(argc==2||argc==3,"usage: actual-native-data-root [original-stat-gold]");std::string root=argv[1],error;
 auto a=file(root+"/character_properties_pyarray.bin"),b=file(root+"/character_properties_pyarraynames.bin"),c=file(root+"/character_properties_pystructnames.bin");
 data::CharacterTable actors;data::PropertyRules rules;check(data::load_characters(bytes(a),bytes(b),bytes(c),actors,error)&&data::load_property_rules(actors,rules,error),error);
 a=file(root+"/character_classes_pyarray.bin");b=file(root+"/character_classes_pyarraynames.bin");c=file(root+"/character_classes_pystructnames.bin");
 data::ClassTables classes;check(data::load_classes(bytes(a),bytes(b),bytes(c),classes,error),error);std::vector<data::ClassRow> rows;for(auto& row:classes.rows)rows.push_back({row.data(),std::uint32_t(row.size())});
 unsigned cases{},debug_calls{},guards{},original_cases{};
 for(auto actor:{263,290,325})for(unsigned stat=0;stat<4;++stat)for(int points:{0,1,2}){
  data::PropertyState state;data::reset_properties(rules,state,&actors.rows[actor]);auto view=data::property_view(rules,state);
  check(!dh2_class_recalc_base(rows.data(),rows.size(),state.base.data(),&view),"actual class recalc");
  check(!dh2_property_set_int(&view,148,points),"points write");auto before=state.saved;
  ui::CharacterMenuStatGraphV1 graph{&state,&view,&actors,rows.data(),std::uint32_t(rows.size()),actor,{}};
  graph.debug_load=[](std::string&){return true;};
  graph.debug_query=[&](const char* name,bool& tracing,std::string&){check(std::string(name)=="isTracingChar_Stats","original Debug query name");tracing=false;++debug_calls;return true;};
  check(ui::character_menu_assign_stat_v1(graph,stat,error),error);
  check(state.saved[148]==before[148]-(points>0?256:0),"source point debit");
  check(state.saved[149+stat]==before[149+stat]+(points>0?256:0),"source stat credit");
  for(unsigned p=0;p<224;++p)if(p!=148&&p!=149+stat)check(state.saved[p]==before[p],"unrelated saved property changed");
  graph.debug_query={};check(!ui::character_menu_assign_stat_v1(graph,stat,error)&&!error.empty(),"missing Debug accepted");++guards;
  auto detached=view;detached.saved=state.base.data();graph.view=&detached;
  check(!ui::character_menu_assign_stat_v1(graph,stat,error),"detached menu properties accepted");++guards;++cases;
 }
 if(argc==3){
  std::ifstream gold(argv[2],std::ios::binary);auto word=[&](){std::uint32_t out;check(bool(gold.read(reinterpret_cast<char*>(&out),4)),"truncated source gold");return out;};
  check(word()==0x31534d43,"source stat gold magic");auto count=word();
  for(unsigned i=0;i<count;++i){auto actor=word(),stat=word();word();data::PropertyState state,expected;
   check(bool(gold.read(reinterpret_cast<char*>(&state),sizeof(state)))&&bool(gold.read(reinterpret_cast<char*>(&expected),sizeof(expected))),"truncated original sheets");
   auto view=data::property_view(rules,state);ui::CharacterMenuStatGraphV1 graph{&state,&view,&actors,rows.data(),std::uint32_t(rows.size()),std::int32_t(actor),{},{}};
   graph.debug_load=[](std::string&){return true;};graph.debug_query=[](const char* name,bool& tracing,std::string&){check(std::string(name)=="isTracingChar_Stats","original Debug name");tracing=false;return true;};
   check(ui::character_menu_assign_stat_v1(graph,stat,error),error);
   if(state.base!=expected.base||state.saved!=expected.saved||state.gear!=expected.gear||state.resolved!=expected.resolved){
    for(unsigned p=0;p<224;++p)if(state.resolved[p]!=expected.resolved[p])std::cerr<<"case "<<i<<" property "<<p<<" actual "<<state.resolved[p]<<" expected "<<expected.resolved[p]<<'\n';
    throw std::runtime_error("whole original stat owner sheet mismatch");}
   ++original_cases;
  }
  check(gold.peek()==std::char_traits<char>::eof(),"trailing original gold");
 }
 std::cout<<"{\"validation\":\"PASS\",\"actual_classes\":3,\"cases\":"<<cases<<",\"whole_original_cases\":"<<original_cases<<",\"debug_branches\":"<<debug_calls<<",\"required_graph_guards\":"<<guards<<"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
