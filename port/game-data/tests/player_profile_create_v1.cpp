#include "player_profile_create_v1.hpp"
#include "player_profile_filename_v1.hpp"
#include "native_player_profile.hpp"
#include "data.hpp"
#include "../../engine-ui/menu_class_selection_v1.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2;
namespace ui = dh2::ui;
namespace {
using B=std::vector<std::uint8_t>;using Status=data::PlayerProfileCreateStatusV1;using Stage=data::PlayerProfileCreateStageV1;
void check(bool condition,const char* text){if(!condition)throw std::runtime_error(text);}
B file(const std::filesystem::path& path){std::ifstream f(path,std::ios::binary);check(bool(f),"fixture file");return B(std::istreambuf_iterator<char>(f),{});}
struct Reader {B bytes;std::size_t at=0;std::uint32_t u(){check(bytes.size()-at>=4,"fixture word");std::uint32_t r=0;for(unsigned i=0;i<4;++i)r|=std::uint32_t(bytes[at++])<<(8*i);return r;}B block(){const auto n=u();check(n<=bytes.size()-at,"fixture block");B r(bytes.begin()+at,bytes.begin()+at+n);at+=n;return r;}std::string str(){auto b=block();return std::string(b.begin(),b.end());}};
struct Providers {
 native::player_profile::Transport* transport=nullptr;std::int32_t slot=0;std::uint32_t seed=0,date=0,count=3;int fail=0,online_calls=0,save_calls=0;
 data::PlayerProfileCreateServicesV1 services(){data::PlayerProfileCreateServicesV1 s;s.context=this;
  s.next_free_slot=[](void* p,std::int32_t* out,std::string&){auto& x=*static_cast<Providers*>(p);if(x.fail==1)return false;*out=x.slot;return true;};
  s.seed_time=[](void* p,std::uint32_t* out,std::string&){auto& x=*static_cast<Providers*>(p);if(x.fail==2)return false;*out=x.seed;return true;};
  s.save_date_time=[](void* p,std::uint32_t* out,std::string&){auto& x=*static_cast<Providers*>(p);if(x.fail==3)return false;*out=x.date;return true;};
  s.difficulty_count=[](void* p,std::uint32_t* out,std::string&){auto& x=*static_cast<Providers*>(p);if(x.fail==4)return false;*out=x.count;return true;};
  s.online=[](void* p,bool* out,std::string&){auto& x=*static_cast<Providers*>(p);++x.online_calls;if(x.fail==4+x.online_calls)return false;*out=false;return true;};
  s.save_all=[](void* p,data::PlayerSavegameV1&,const data::PlayerSaveProfileV1&,std::string& e){auto& x=*static_cast<Providers*>(p);++x.save_calls;if(x.fail==8)return false;return x.transport->save_all(e);};return s;
 }
};
std::uint32_t replay(const std::filesystem::path& fixtures,const std::filesystem::path& scratch,data::CharacterTable& characters){
 Reader r{file(fixtures)};check(r.bytes.size()>=4&&!std::memcmp(r.bytes.data(),"PCT1",4),"fixture magic");r.at=4;
 const auto names=r.u();for(std::uint32_t i=0;i<names;++i)characters.names.push_back(r.str());const auto cases=r.u();std::string error;
 // Bind authored MainMenu class order to the real CharacterTable rows used by
 // NativeCreateSaveSlot. The adjacent click regression proves that each
 // on_clicked advances this same index; the create receipts below prove the
 // selected string is then persisted and read back by a distinct gameplay Save.
 constexpr std::int32_t expected_rows[]{263,325,290};
 for(int index=0;index<3;++index){
  const auto* choice=ui::menu_class_choice_v1(index);
  check(choice,"authored class choice exists");
  std::int32_t found=-1;
  for(std::size_t row=0;row<characters.names.size();++row)
   if(characters.names[row]==choice->profile_character){found=std::int32_t(row);break;}
  check(found==expected_rows[index],"authored class selection resolves to its source CharacterTable row");
 }
 for(std::uint32_t i=0;i<cases;++i){const auto argc=r.u();const auto name=r.str(),class_name=r.str();Providers p;p.slot=std::int32_t(r.u());p.seed=r.u();p.date=r.u();const bool published=r.u()!=0;const auto expected_bytes=r.block();std::array<std::uint32_t,20> expected{};std::array<std::uint8_t,3> spawn{};
  if(published){for(auto& v:expected)v=r.u();for(auto& v:spawn)v=r.bytes.at(r.at++);}
  const auto directory=scratch/("case"+std::to_string(i));std::filesystem::create_directories(directory);
  const auto path=directory/data::player_profile_filename_v1(std::uint32_t(p.slot),false,false);std::filesystem::remove(path);std::filesystem::remove(path.string()+".bak");std::filesystem::remove(path.string()+".creating");
  data::PlayerSavegameV1 save;data::PlayerSaveProfileV1 profile;std::int32_t difficulty=99;native::player_profile::Transport transport(save,profile);p.transport=&transport;
  check(transport.bind({directory,&characters,&difficulty,{},true},error),"new transport bind");data::PlayerProfileCreateRuntimeV1 runtime({&save,&profile,&transport.loader(),&characters,&difficulty,p.services()});data::PlayerProfileCreateResultV1 result;
  const auto status=argc==2?runtime.create(name,class_name,&result,error):Status::rejected;
  check(result.source_return_published==published,"source numeric return publication");
  if(!published){check(status==Status::rejected&&save.slot()==-1&&save.class_id()==-1&&save.name().empty()&&p.online_calls==0&&!std::filesystem::exists(path),"rejected source prefix");continue;}
  check(status==Status::complete&&file(path)==expected_bytes&&profile.campaign.bytes()==expected_bytes,"whole source saveAll bytes and same-index publication");
  check(std::uint32_t(save.slot())==expected[0]&&std::uint32_t(save.level())==expected[1]&&std::uint32_t(save.class_id())==expected[2]&&std::uint32_t(difficulty)==expected[3]&&std::uint32_t(save.unlocked_difficulty())==expected[4]&&save.save_date()==expected[5]&&save.saved_properties_byte_194()==expected[6]&&std::uint32_t(save.source_save_mode())==expected[7],"source constructor/create scalar fields");
  const auto& fields=save.level_name_fields();for(std::size_t j=0;j<3;++j)check(std::uint32_t(fields.word50[j])==expected[8+j]&&std::uint32_t(fields.word5c[j])==expected[11+j]&&std::uint32_t(fields.quest_wordfc[j])==expected[14+j]&&std::uint32_t(save.level_entry_points()[j])==expected[17+j]&&save.use_spawn_points()[j]==spawn[j],"source initial location/seed/quest fields");
  check(save.name()==name&&p.online_calls==3&&p.save_calls==1&&profile.campaign.source_sections().size()==7,"source services/registration");
  // A DISTINCT gameplay Save reads real persisted PCLS; no preview copy.
  data::PlayerSavegameV1 gameplay;gameplay.set_character(0x123400);gameplay.set_slot(p.slot);data::PlayerSaveProfileV1 gameplay_profile;std::int32_t live_difficulty=17;native::player_profile::Transport reader(gameplay,gameplay_profile);
  check(reader.bind({directory,&characters,&live_difficulty,{}},error)&&reader.loader().load(1,error),"persisted gameplay SG_Load1");check(gameplay.class_id()==save.class_id()&&gameplay.name()==save.name()&&gameplay.character()==0x123400&&&gameplay!=&save&&gameplay_profile.identity!=profile.identity,"separate saved gameplay authority");
  check(!reader.save_all(error)&&file(path)==expected_bytes,"import remains read-only");
 }
 check(r.at==r.bytes.size(),"fixture consumed");return cases;
}
std::uint32_t failures(const std::filesystem::path& scratch,const data::CharacterTable& characters){std::uint32_t cases=0;std::string error;
 for(int fail=1;fail<=8;++fail){const auto directory=scratch/("failure"+std::to_string(fail));std::filesystem::create_directories(directory);const auto path=directory/data::player_profile_filename_v1(0,false,false);std::filesystem::remove(path);
  data::PlayerSavegameV1 save;data::PlayerSaveProfileV1 profile;std::int32_t difficulty=99;native::player_profile::Transport transport(save,profile);check(transport.bind({directory,&characters,&difficulty,{},true},error),"failure bind");Providers p;p.transport=&transport;p.fail=fail;p.seed=0xffffffff;p.date=0x12345678;
  data::PlayerProfileCreateRuntimeV1 runtime({&save,&profile,&transport.loader(),&characters,&difficulty,p.services()});data::PlayerProfileCreateResultV1 result;check(runtime.create("FailureHero","MagePlayerBase",&result,error)==Status::failed&&!result.source_return_published,"required provider failure");
  if(fail==1)check(save.slot()==-1&&difficulty==99&&!profile.identity,"next slot failure before construction");
  else{check(save.slot()==0&&save.class_id()==290&&save.level()==1&&save.name()=="FailureHero"&&difficulty==0,"retained field prefix");check(runtime.create("Other","KnightPlayerBase",&result,error)==Status::failed,"no retry on same temporary Save");}
  if(fail==2)check(save.level_name_fields().word5c[0]==0&&save.saved_properties_byte_194()==0,"seed failure before seeds/flag");
  if(fail==3)check(std::uint32_t(save.level_name_fields().word5c[0])==0xffffffff&&save.saved_properties_byte_194()==0,"date failure retains seeds");
  if(fail==4)check(save.save_date()==0x12345678&&save.saved_properties_byte_194()==1&&!save.use_spawn_points_loaded(),"count failure retains date and flag");
  if(fail==6||fail==7||fail==8)check(save.source_save_mode()==1,"save mode prefix");
  if(fail==7)check(p.save_calls==1&&std::filesystem::exists(path)&&profile.campaign.source_sections().size()==7,"third online failure after persistence");else check(!std::filesystem::exists(path),"failure file prefix");++cases;
 }
 // Source writer reads unlocked difficulty again after the first write.
 {data::PlayerSavegameV1 save;save.set_unlocked_difficulty(4);std::int32_t current=17;struct Sink{data::PlayerSavegameV1& save;B bytes;int calls=0;}sink{save,{}};
  const data::PlayerMetadataWriteServicesV1 stream{&sink,[](void* p,data::Bytes b,std::string&){auto& s=*static_cast<Sink*>(p);s.bytes.insert(s.bytes.end(),b.data,b.data+b.size);if(++s.calls==1)s.save.set_unlocked_difficulty(7);return true;}};
  check(data::write_player_metadata_section_v1("PDFL",save,characters,&current,stream,error)&&sink.bytes==B({17,0,0,0,7,0,0,0}),"PDFL fresh second field");++cases;
 }
 {data::PlayerSavegameV1 save;save.set_player_name("WriterHero");B prefix;int calls=0;struct Sink{B& bytes;int& calls;}sink{prefix,calls};const data::PlayerMetadataWriteServicesV1 stream{&sink,[](void* p,data::Bytes b,std::string&){auto& s=*static_cast<Sink*>(p);if(++s.calls==2)return false;s.bytes.insert(s.bytes.end(),b.data,b.data+b.size);return true;}};
  check(!data::write_player_metadata_section_v1("PNAM",save,characters,nullptr,stream,error)&&prefix==B({11,0,0,0})&&calls==2,"PNAM failed payload keeps length prefix");++cases;
 }
 {data::PlayerSavegameV1 save;save.set_character(123);check(!save.initialize_new_profile_metadata(0,error)&&save.character()==123&&save.slot()==-1,"gameplay Save cannot be creation temporary");++cases;}
 return cases;
}
}
int main(int argc,char** argv){try{check(argc==3,"usage fixtures scratch");data::CharacterTable characters;const auto replay_cases=replay(argv[1],argv[2],characters);const auto failure_cases=failures(argv[2],characters);std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<replay_cases<<",\"required_failure_and_mutation_cases\":"<<failure_cases<<",\"persisted_profiles\":9,\"metadata_writers\":7,\"mismatches\":0}";}catch(const std::exception& e){std::cerr<<e.what();return 1;}}
