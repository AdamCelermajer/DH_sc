#include "../level_tables.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <dlfcn.h>
namespace {
unsigned checks=0;
void check(bool value){++checks;if(!value)throw std::runtime_error("Level table check "+std::to_string(checks));}
std::uint32_t word(std::istream& input){std::uint32_t value;input.read(reinterpret_cast<char*>(&value),4);check(bool(input));return value;}
std::vector<std::uint8_t> field(std::istream& input){auto n=word(input);check(n<=1024*1024);std::vector<std::uint8_t> raw(n);input.read(reinterpret_cast<char*>(raw.data()),n);check(bool(input));return raw;}
std::vector<std::uint8_t> file(const std::string& path){std::ifstream input(path,std::ios::binary);check(bool(input));return {std::istreambuf_iterator<char>(input),{}};}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value){return {value.data(),value.size()};}
}
int main(int argc,char** argv){try{
 check(argc==3);std::ifstream in(argv[1],std::ios::binary);check(bool(in)&&word(in)==0x3144544c);const auto count=word(in);check(count==212);
 std::vector<dh2::data::FastTravelRecord> travel;std::vector<dh2::data::LevelRecord> levels;
 unsigned compared_words=0,compared_texts=0,truncated=0;
 for(unsigned i=0;i<count;++i){const auto kind=word(in);check(kind<=1);auto raw=field(in);const auto n=kind?18u:7u;std::uint32_t expected[18]{};
  in.read(reinterpret_cast<char*>(expected),n*4);check(bool(in));std::vector<std::vector<std::uint8_t>> text;for(unsigned j=0;j<(kind?2u:1u);++j)text.push_back(field(in));
  dh2::data::LevelProjection72 output{};dh2::data::LevelTextSpan16 spans[2]{};std::uint32_t used=0;
  auto decode=[&](unsigned size){if(kind)return dh2_level_decode_record(&output,spans,&used,raw.data(),size);
   dh2::data::FastTravelProjection28 fast{};auto status=dh2_fast_travel_decode_record(&fast,spans,&used,raw.data(),size);if(!status)std::memcpy(&output,&fast,sizeof fast);return status;};
  check(decode(raw.size())==0&&used==raw.size()&&!std::memcmp(output.words,expected,n*4));compared_words+=n;
  for(unsigned j=0;j<text.size();++j){check(!spans[j].reserved&&spans[j].size==text[j].size()&&spans[j].data>=raw.data()&&spans[j].data+spans[j].size<=raw.data()+raw.size());check(text[j].empty()||!std::memcmp(spans[j].data,text[j].data(),text[j].size()));++compared_texts;}
  if(i<84){if(kind){dh2::data::LevelRecord row;row.scalar=output;row.description.assign(reinterpret_cast<const char*>(spans[0].data),spans[0].size);row.file.assign(reinterpret_cast<const char*>(spans[1].data),spans[1].size);levels.push_back(std::move(row));}
   else{dh2::data::FastTravelRecord row;std::memcpy(&row.scalar,&output,sizeof row.scalar);row.level_name.assign(reinterpret_cast<const char*>(spans[0].data),spans[0].size);travel.push_back(std::move(row));}}
  for(unsigned prefix=0;prefix<raw.size();++prefix){std::memset(&output,0xa5,sizeof output);std::memset(spans,0xb6,sizeof spans);used=0xcccccccc;auto old_output=output;dh2::data::LevelTextSpan16 old_spans[2];std::memcpy(old_spans,spans,sizeof spans);
   check(decode(prefix)==1&&used==0xcccccccc&&!std::memcmp(&output,&old_output,sizeof output)&&!std::memcmp(spans,old_spans,sizeof spans));++truncated;}
 }
 char extra;check(!in.read(&extra,1));check(travel.size()==33&&levels.size()==51);
 const std::string folder=argv[2];auto records=file(folder+"/levels_pyarray.bin"),names=file(folder+"/levels_pyarraynames.bin"),schema=file(folder+"/levels_pystructnames.bin");
 dh2::data::LevelTables table;std::string error;check(dh2::data::load_levels(bytes(records),bytes(names),bytes(schema),table,error)&&error.empty());check(table.travel_data_consumed==1133&&table.data_consumed==5574&&table.travel.size()==33&&table.levels.size()==51);
 for(unsigned i=0;i<travel.size();++i)check(!std::memcmp(&travel[i].scalar,&table.travel[i].scalar,sizeof travel[i].scalar)&&travel[i].level_name==table.travel[i].level_name);
 for(unsigned i=0;i<levels.size();++i)check(!std::memcmp(&levels[i].scalar,&table.levels[i].scalar,sizeof levels[i].scalar)&&levels[i].description==table.levels[i].description&&levels[i].file==table.levels[i].file);
 auto crypt=std::find(table.level_names.begin(),table.level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=table.level_names.end());const auto& row=table.levels.at(crypt-table.level_names.begin());
 const std::uint32_t ranges[6]{10,47,76,8,45,74};check(!std::memcmp(row.scalar.words+12,ranges,24)&&row.file=="007_crypt_01.rule.xml");
 unsigned guards=0;for(unsigned mode=0;mode<7;++mode){auto bad_records=records,bad_names=names,bad_schema=schema;
  if(mode==0)bad_records.pop_back();if(mode==1)bad_names.pop_back();if(mode==2)bad_schema.pop_back();if(mode==3)bad_records.push_back(0);if(mode==4)bad_records[0]=32;if(mode==5)bad_schema[8]='X';if(mode==6)bad_records[1133]=50;
  check(!dh2::data::load_levels(bytes(bad_records),bytes(bad_names),bytes(bad_schema),table,error)&&table.levels.empty()&&table.travel.empty()&&!error.empty());++guards;}
 dh2::data::LevelProjection72 output{};dh2::data::LevelTextSpan16 spans[2]{};std::uint32_t used=0;
 check(dh2_level_decode_record(nullptr,spans,&used,records.data(),records.size())==1);++guards;
 check(dh2_level_decode_record(&output,nullptr,&used,records.data(),records.size())==1);++guards;
 check(dh2_level_decode_record(&output,spans,nullptr,records.data(),records.size())==1);++guards;
 check(dh2_level_decode_record(&output,spans,&used,nullptr,1)==1);++guards;
 check(dh2_level_decode_record(&output,spans,&used,records.data(),8*1024*1024+1)==1);++guards;
 check(dh2_level_decode_record(&output,spans,reinterpret_cast<std::uint32_t*>(&output),records.data(),records.size())==1);++guards;
 Dl_info info{};check(dladdr(reinterpret_cast<void*>(&dh2_level_decode_record),&info));
 std::cout<<"{\"validation\":\"PASS\",\"record_cases\":"<<count<<",\"compared_words\":"<<compared_words<<",\"compared_strings\":"<<compared_texts<<",\"truncated_prefix_guards\":"<<truncated<<",\"native_guards\":"<<guards<<",\"cache_travel_rows\":33,\"cache_level_rows\":51,\"checks\":"<<checks<<",\"module_library\":\""<<info.dli_fname<<"\",\"source_Application_selection_verified\":false}\n";
 }catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
