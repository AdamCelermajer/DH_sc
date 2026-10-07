#include "../player_save_write_owner_v1.hpp"
#include "../player_save_named_writer_v1.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>
using namespace dh2::data;
void check(bool condition){if(!condition)throw std::runtime_error("Save writer coordinator assertion failed");}
int main(int argc,char** argv){try{
 check(argc==2);
 auto backing=std::make_shared<PlayerSavegameV1>();
 auto authority=std::make_shared<PlayerSaveLoadOwnerV1>(backing);
 std::string error;auto lease=std::make_shared<int>(42);
 std::vector<PlayerSaveWriteOpV1> calls;PlayerSaveWriteOwnerV1* current=nullptr;unsigned reentries=0;
 PlayerSaveWriteServicesV1 services;services.owner=lease;
 services.invoke=[&](const auto& request,auto& response,auto& e){
  check(request.authority==authority.get()&&request.save==backing.get()&&request.profile.identity==42&&request.profile.owner==lease);
  std::string nested;check(!current->save(nested)&&nested.find("recursive")!=std::string::npos);++reentries;
  calls.push_back(request.operation);response.flag=false;
  // Explicit offline/profile-saveAll coordinator boundary, not file writer proof.
  if(request.operation==PlayerSaveWriteOpV1::online||request.operation==PlayerSaveWriteOpV1::save_all)return true;
  e="Unexpected offline coordinator operation";return false;
 };
 PlayerSaveWriteOwnerV1 writer(authority,services);current=&writer;
 check(writer.save(error)&&calls.empty()&&authority->save_mode()==0);
 check(authority->publish_profile({42,lease},error));
 const std::vector<PlayerSaveWriteOpV1> expected={PlayerSaveWriteOpV1::online,PlayerSaveWriteOpV1::online,PlayerSaveWriteOpV1::save_all,PlayerSaveWriteOpV1::online};
 check(writer.save(error)&&calls==expected&&authority->save_mode()==1&&reentries==4);
 PlayerSaveWriteOwnerV1 missing(authority);check(!missing.save(error)&&error.find("required")!=std::string::npos);
 check(authority->publish_profile({},error));check(missing.save(error));
 PlayerSaveNamedWriterV1 named(authority);std::vector<std::uint8_t> bytes;unsigned named_reentry=0;
 PlayerSaveByteWriterV1 sink{lease,[&](Bytes input,std::string&){bytes.insert(bytes.end(),input.data,input.data+input.size);std::string nested;check(!named.write("PLVL",{},nested)&&nested.find("recursive")!=std::string::npos);++named_reentry;return true;}};
 std::size_t consumed=0;const std::uint8_t name_bytes[]={7,0,0,0,'K','n','i','g','h','t',0};
 check(backing->load_name({name_bytes,sizeof name_bytes},consumed,error));
 check(named.write("PNAM",sink,error)&&bytes==std::vector<std::uint8_t>(name_bytes,name_bytes+sizeof name_bytes)&&named_reentry==2);
 bytes.clear();const std::uint8_t level_bytes[]={50,0,0,0};check(backing->load_level({level_bytes,4},consumed,error));check(named.write("PLVL",sink,error)&&bytes==std::vector<std::uint8_t>(level_bytes,level_bytes+4));
 backing->initialize_faeries();const std::uint8_t current_bytes[]={0,0,0,0,1,0,0,0,4,0,0,0};check(backing->load_current_faery({current_bytes,12},consumed,error));bytes.clear();check(named.write("CFEE",sink,error)&&bytes==std::vector<std::uint8_t>(current_bytes,current_bytes+12));
 check(!named.write("QEST",sink,error)&&error.find("not reconstructed")!=std::string::npos);
 std::ifstream gold(argv[1],std::ios::binary);check(bool(gold));auto word=[&](){std::array<unsigned char,4> data{};gold.read(reinterpret_cast<char*>(data.data()),4);check(bool(gold));return std::uint32_t(data[0])|(std::uint32_t(data[1])<<8)|(std::uint32_t(data[2])<<16)|(std::uint32_t(data[3])<<24);};const auto cases=word();check(cases==20);
 for(unsigned index=0;index<cases;++index){std::array<char,5> section{};gold.read(section.data(),4);const auto size=word();check(size<4096);std::vector<std::uint8_t> expected_bytes(size);gold.read(reinterpret_cast<char*>(expected_bytes.data()),size);check(bool(gold));Bytes input{expected_bytes.data(),expected_bytes.size()};consumed=0;
  const std::string name=section.data();if(name=="SKIL")continue;if(name=="PNAM")check(backing->load_name(input,consumed,error));else if(name=="PLVL")check(backing->load_level(input,consumed,error));else if(name=="CFEE")check(backing->load_current_faery(input,consumed,error));else if(name=="FAES"){bool mismatch=false;check(backing->load_faeries(input,consumed,mismatch,error)&&!mismatch);}else check(false);
  bytes.clear();check(named.write(section.data(),sink,error)&&bytes==expected_bytes);
 }
 std::cout<<"{\"validation\":\"PASS\",\"same_Save_profile_authority\":true,\"original_offline_order\":true,\"reentry_guards\":4,\"real_named_sections\":4,\"original_named_byte_cases\":17,\"file_writer_boundary_fixture\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
