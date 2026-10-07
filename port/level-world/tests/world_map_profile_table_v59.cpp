#include "../../game-data/world_map_profile_table_v59.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace dh2::data;
static std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);if(!f)throw std::runtime_error("Required actual WorldMap cache");return {std::istreambuf_iterator<char>(f),{}};}
static Bytes span(const std::vector<std::uint8_t>& b){return {b.data(),b.size()};}
int main(int argc,char** argv){try{
 if(argc!=4)throw std::runtime_error("actual WorldMap records/names/schema required");
 auto a=read(argv[1]),b=read(argv[2]),c=read(argv[3]);std::string e;WorldMapProfileTableV59 t;
 if(!t.decode(span(a),span(b),span(c),e))throw std::runtime_error(e);
 if(t.rows().size()!=13||t.lockers().size()!=3||t.names().front()!="Thamos_Catacombs"||t.names().back()!="Royal_Castle")throw std::runtime_error("Actual cached WorldMap dimensions/names");
 unsigned checks=4;for(std::size_t i=0;i<t.rows().size();++i){
  if(t.rows()[i].state8!=t.defaults8()[i])throw std::runtime_error("Source MapLoc+8 default mismatch");++checks;
 }
 auto truncated=a;truncated.pop_back();WorldMapProfileTableV59 failed;
 if(failed.decode(span(truncated),span(b),span(c),e)||failed.ready()||!failed.rows().empty())throw std::runtime_error("Atomic truncated table rejection");++checks;
 if(t.decode(span(a),span(b),span(c),e)||!t.ready()||t.rows().size()!=13)throw std::runtime_error("No live immutable-table replacement");++checks;
 std::cout<<"PASS "<<checks<<" actual WorldMap source table/default guards; no gameplay map unlocking claim\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
