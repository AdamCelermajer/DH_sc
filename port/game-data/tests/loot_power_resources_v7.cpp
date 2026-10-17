#include "../loot_power_resources_v7.hpp"
#include "../loot_tables_v2.hpp"
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
using Raw=std::vector<std::uint8_t>;
static unsigned checks;
static void ck(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);++checks;}
static Raw read(const std::string& path){std::ifstream f(path,std::ios::binary);ck(bool(f),"Missing cache: "+path);return {std::istreambuf_iterator<char>(f),{}};}
static Bytes span(const Raw& b){return {b.data(),b.size()};}
int main(int argc,char** argv){try{
 ck(argc==2,"Expected canonical pydata directory");
 const char* names[]={"item_powers_pyarray.bin","item_powers_pyarraynames.bin","item_powers_pystructnames.bin","item_powers_monopoly_pyarray.bin","item_powers_monopoly_pyarraynames.bin","item_powers_monopoly_pystructnames.bin","loot_table_pyarray.bin","loot_table_pyarraynames.bin","loot_table_pystructnames.bin"};
 std::array<Raw,9> raw;for(unsigned i=0;i<9;++i)raw[i]=read(std::string(argv[1])+"/"+names[i]);
 // Original 0x4b9bf4 reader identified this exact NumProb section; the runner
 // pins canonical cache hashes before this fixture projection is accepted.
 ck(raw[6].size()==284104,"Unexpected loot cache size");
 std::string error;
 Bytes quantities{};
 // The selected LootTable reader ends at 282872; the final NumProbArray is
 // preceded by another array. Select the last source read by its exact layout.
 LootTablesV2 loot_owner;ck(loot_owner.load(span(raw[6]),span(raw[7]),span(raw[8]),error),error);
 ck(select_source_quantity_array_v7(span(raw[6]),span(raw[7]),loot_owner.borrow().consumed(),quantities,error),error);
 ck(quantities.data==raw[6].data()+283108&&quantities.size==996,"Final NumProbArray boundary differs from canonical source cache");
 LootPowerInputsV7 input{span(raw[0]),span(raw[1]),span(raw[2]),span(raw[3]),span(raw[4]),span(raw[5]),quantities,span(raw[7]),span(raw[8])};
 ItemPowerTablesV5 powers;
 ck(powers.load(input.powers,input.power_names,input.power_schema,error),error);
 auto authority=powers.borrow();LootPowerResourcesV7 resources;
 ck(resources.load(input,authority,error),error);auto pinned=resources.borrow();
 ck(pinned.lists().size()==121&&pinned.quantities().size()==39&&pinned.powers().rows().size()==937,"Canonical counts differ");
 ck(&pinned.powers().rows()==&authority.rows(),"Duplicate power authority");
 ck(!resources.load(input,authority,error),"Reload with live borrowers accepted");
 unsigned guards=1;
 for(unsigned i=0;i<9;++i){auto bad=input;Bytes* fields[]={&bad.powers,&bad.power_names,&bad.power_schema,&bad.monopoly,&bad.monopoly_names,&bad.monopoly_schema,&bad.quantities,&bad.loot_names,&bad.loot_schema};fields[i]->size=1;LootPowerResourcesV7 candidate;ck(!candidate.load(bad,authority,error),"Truncated resource accepted");ck(!candidate.borrow(),"Failed load published snapshot");++guards;}
 {LootPowerResourcesV7 candidate;ck(!candidate.load(input,{},error),"Missing authority accepted");++guards;}
 {auto changed=raw[0];changed.back()^=1;auto bad=input;bad.powers=span(changed);LootPowerResourcesV7 candidate;ck(!candidate.load(bad,authority,error),"Different power properties accepted");++guards;}
 {LootPowerResourcesV7 candidate;ck(candidate.load(input,authority,error),error);auto bad=input;bad.power_names.size=1;ck(!candidate.load(bad,authority,error),"Malformed replacement accepted");ck(candidate.borrow().lists().size()==121,"Failed replacement destroyed previous snapshot");++guards;}
 resources=LootPowerResourcesV7{};powers=ItemPowerTablesV5{};for(auto& b:raw){Raw empty;b.swap(empty);}
 ck(pinned.lists().size()==121&&pinned.quantities().size()==39&&pinned.powers().rows().size()==937,"Borrow did not retain resources");
 ck(&pinned.powers().rows()==&authority.rows(),"Lifetime changed authority identity");
 std::cout<<"{\"validation\":\"PASS\",\"power_lists\":121,\"quantity_lists\":39,\"power_definitions\":937,\"same_power_authority\":true,\"guards\":"<<guards<<",\"checks\":"<<checks<<",\"scope\":\"selected-library host resources and lifetime; no loot creation or gameplay\"}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
