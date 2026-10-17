#include "../world_map_tables.hpp"
#include "../level_tables.hpp"
#include "../player_savegame_v1.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
using namespace dh2::data;
static void require(bool value,const char* error){if(!value)throw std::runtime_error(error);}
static std::vector<std::uint8_t> file(const char* path){std::ifstream input(path,std::ios::binary);require(bool(input),"input open");return {std::istreambuf_iterator<char>(input),{}};}
static Bytes bytes(const std::vector<std::uint8_t>& data){return {data.data(),data.size()};}
static void quote(std::ostream& out,const std::string& text){out<<'"';for(unsigned char c:text){if(c=='"'||c=='\\')out<<'\\';out<<c;}out<<'"';}
static void print_table(std::ostream& out,const WorldMapTables& table) {
    out<<"{\"locations\":[";
    for(std::size_t i=0;i<table.locations.size();++i){if(i)out<<',';const auto& row=table.locations[i];out<<"{\"name\":";quote(out,row.name);out<<",\"name_id\":"<<row.name_id<<",\"state\":"<<row.state<<",\"location_levels\":[";for(std::size_t j=0;j<row.location_levels.size();++j){if(j)out<<',';out<<row.location_levels[j];}out<<"]}";}
    out<<"],\"lockers\":[";
    for(std::size_t i=0;i<table.lockers.size();++i){if(i)out<<',';const auto& row=table.lockers[i];out<<"{\"name\":";quote(out,row.name);out<<",\"on_state\":"<<row.on_state<<",\"quest_id\":"<<row.quest_id<<'}';}
    out<<"]}";
}
static std::string snapshot(const WorldMapTables& table){std::ostringstream output;print_table(output,table);return output.str();}
struct Composition {
    LevelTables levels;WorldMapTables world;PlayerSavegameV1 save;
    unsigned count_calls=0,default_calls=0,allocations=0;
    static void* allocate(void* raw,std::size_t size,int tag){auto& c=*static_cast<Composition*>(raw);require(tag==0,"source allocation tag");++c.allocations;return std::calloc(1,size?size:4);}
    static void release(void*,void* pointer){std::free(pointer);}
    static bool count(void* raw,SavedStateTableV1 table,std::uint32_t* output,std::string&){auto& c=*static_cast<Composition*>(raw);++c.count_calls;*output=static_cast<unsigned>(table==SavedStateTableV1::levels?c.levels.levels.size():c.world.locations.size());return true;}
    static bool word(void* raw,SavedStateTableV1 table,unsigned row,std::int32_t* output,std::string&){auto& c=*static_cast<Composition*>(raw);++c.default_calls;if(table==SavedStateTableV1::world_map)return read_world_map_default_word(c.world,row,*output);if(row>=c.levels.levels.size())return false;*output=c.levels.levels[row].level_state;return true;}
    SavedLevelStateServicesV1 services(){return {this,count,word,{this,allocate,release}};}
};
int main(int argc,char** argv) {
    try {
        require(argc==7,"six cache paths required");std::array<std::vector<std::uint8_t>,3> raw{file(argv[1]),file(argv[2]),file(argv[3])};
        Composition c;std::string error;require(load_world_map(bytes(raw[0]),bytes(raw[1]),bytes(raw[2]),c.world,error),"actual WorldMap decode");
        auto old=snapshot(c.world);unsigned rejected=0;
        const auto reject=[&](const auto& values){require(!load_world_map(bytes(values[0]),bytes(values[1]),bytes(values[2]),c.world,error),"malformed WorldMap accepted");require(snapshot(c.world)==old,"failed WorldMap load changed prior table");++rejected;};
        for(unsigned i=0;i<3;++i){auto values=raw;values[i].pop_back();reject(values);values=raw;values[i].push_back(0);reject(values);}
        {auto values=raw;values[0][0]=255;values[0][1]=255;reject(values);}
        {auto values=raw;values[0][12]=255;values[0][13]=255;reject(values);}
        {auto values=raw;values[1][0]=12;reject(values);}
        {auto values=raw;values[2][8]='X';reject(values);}
        {auto values=raw;values[1][8]=255;reject(values);}
        {auto values=raw;values[1][8]=0;reject(values);}
        std::int32_t value=919;require(!read_world_map_default_word(c.world,999,value)&&value==919,"out-of-range map word changes output");
        require(find_world_map_location(c.world,"Thamos_Catacombs")==0&&find_world_map_location(c.world,"Royal_Castle")==12&&find_world_map_location(c.world,"absent")==-1,"actual map name domain");
        auto records=file(argv[4]),names=file(argv[5]),schema=file(argv[6]);require(load_levels(bytes(records),bytes(names),bytes(schema),c.levels,error),"actual LevelList decode");
        require(c.save.initialize_level_states(c.services(),error),"actual saved level/map defaults composition");
        require(c.allocations==6&&c.default_calls==3*(c.levels.levels.size()+c.world.locations.size()),"one canonical Save initialized six arrays");
        std::cout<<"{\"validation\":\"PASS\",\"rejection_cases\":"<<rejected<<",\"table\":";print_table(std::cout,c.world);std::cout<<",\"level_defaults\":[";
        for(std::size_t i=0;i<c.levels.levels.size();++i){if(i)std::cout<<',';std::cout<<c.levels.levels[i].level_state;}
        std::cout<<"],\"saved_defaults\":[";
        for(unsigned d=0;d<3;++d)for(unsigned t=0;t<2;++t){if(d||t)std::cout<<',';auto* array=t?c.save.source_world_map_states(d):c.save.source_level_states(d);std::cout<<'[';for(unsigned i=0;i<array->count;++i){if(i)std::cout<<',';std::cout<<array->words[i];}std::cout<<']';}
        const auto calls=c.count_calls;c.save.source_level_states(0)->words[0]=-77;c.save.source_world_map_states(0)->words[0]=-88;
        require(c.save.initialize_level_states({},error)&&c.count_calls==calls&&c.allocations==6&&c.save.source_level_states(0)->words[0]==-77&&c.save.source_world_map_states(0)->words[0]==-88,"nonnull campaign state is retained without table providers");
        std::cout<<"],\"saved_default_stores\":"<<c.default_calls<<",\"allocations\":"<<c.allocations<<",\"retained_existing_states\":true}"<<std::endl;return 0;
    }catch(const std::exception& failure){std::cerr<<failure.what()<<std::endl;return 1;}
}
