#include "item_power_tables_v5.hpp"
#include "faery_tables.hpp"
#include <cstdio>
#include <vector>
#include <string>
std::vector<std::uint8_t> read(const std::string& path){auto* file=std::fopen(path.c_str(),"rb");if(!file)return {};std::fseek(file,0,SEEK_END);auto n=std::ftell(file);std::rewind(file);std::vector<std::uint8_t> out(static_cast<std::size_t>(n));if(std::fread(out.data(),1,out.size(),file)!=out.size())out.clear();std::fclose(file);return out;}
int main(int argc,char**argv){
 if(argc!=2)return 2;using namespace dh2::data;std::string directory=argv[1],error;
 auto load=[&](const char* name,auto&& target){auto b=read(directory+"/"+name+"_pyarray.bin"),n=read(directory+"/"+name+"_pyarraynames.bin"),s=read(directory+"/"+name+"_pystructnames.bin");return target.load({b.data(),b.size()},{n.data(),n.size()},{s.data(),s.size()},error);};
 ItemPowerTablesV5 powers;FaeryTables faeries;if(!load("item_powers",powers)||!load("faeries",faeries)){std::printf("FAIL %s\n",error.c_str());return 3;}
 auto p=powers.borrow();auto f=faeries.borrow();unsigned properties=0;
 for(unsigned i=0;i<p.rows().size();++i){if(p.names()[i].empty())return 4;properties+=unsigned(p.rows()[i].properties.size());}
 for(const auto& list:f.lists())for(auto id:list)if(id<0||std::size_t(id)>=f.faeries().size())return 5;
 std::printf("PASS actual cache powers %zu | ordered properties %u | faery lists %zu | faery rows %zu\n",p.rows().size(),properties,f.lists().size(),f.faeries().size());
 for(unsigned i=0;i<f.lists()[0].size();++i){auto id=f.lists()[0][i];auto& row=f.faeries()[std::size_t(id)];std::printf("FAERY slot%u id%d %s | name0x%08x description0x%08x element%u model%u spellType%u type%u | script%s\n",i,id,f.faery_names()[std::size_t(id)].c_str(),row.scalar.words[4],row.scalar.words[1],row.scalar.words[2],row.scalar.words[3],row.scalar.words[7],row.scalar.words[8],row.script.c_str());}
 for(unsigned i=0;i<3;++i){auto& row=p.rows()[i];std::printf("POWER id%u %s | description0x%08x palette%d effect%d entries%zu\n",i,p.names()[i].c_str(),unsigned(row.scalars.description),row.scalars.palette,row.scalars.special_effect,row.properties.size());
  for(const auto& property:row.properties)std::printf(" ATTR %d BONUS raw%d number%.8g FLAGS%d\n",property.type,property.value,float(property.value)/256.f,property.extra);
 }
 return 0;
}
