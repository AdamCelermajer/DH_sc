#include "native_player_profile.hpp"
#include "native_exclusive_publish_v1.hpp"
#include "player_profile_filename_v1.hpp"
#include "player_profile_create_v1.hpp"
#include "player_profile_atomic_replace_v1.hpp"
#include "player_level_states_save_writer_v1.hpp"
#include "player_skill_save_writer_v1.hpp"
#include "player_save_section_writers_v1.hpp"
#include "player_property_save_writer_v1.hpp"
#include "player_gear_save_writer_v1.hpp"
#include "data.hpp"
#include "level_tables.hpp"
#include "world_map_tables.hpp"
#include "player_saved_level_states_v1.hpp"
#include "player_saved_fast_travel_v1.hpp"
#include "player_saved_inventory_v1.hpp"
#include "native_quest_owner.hpp"
#include "native_quest_cursor.hpp"
#include "properties.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <cstdio>
#include <limits>
#ifdef _WIN32
#include <io.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace dh2::native::player_profile {
namespace {
bool metadata_tag(const char* tag){
 if(!tag)return false;
 for(const char* name:{"PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP"})
  if(!std::strcmp(tag,name))return true;
 return false;
}
constexpr std::array<const char*,15> kMask1And4WriterTags{{
 "PNAM","PLVL","PCLS","PDFL","LNAM","LEPT","LUSP",
 "LVLS","SKIL","FAES","CFEE","QEST","PROP","GEAR","FTVL"}};
struct GameplayPayloadSink {std::vector<std::uint8_t>* bytes=nullptr;};
bool append_gameplay_payload(void* raw,data::Bytes bytes,std::string& error){
 auto* sink=static_cast<GameplayPayloadSink*>(raw);
 if(!sink||!sink->bytes||(!bytes.data&&bytes.size)||
    sink->bytes->size()>UINT32_MAX||
    bytes.size>std::size_t(UINT32_MAX)-sink->bytes->size()){
  error="gameplay section output exceeds source 32-bit span";return false;
 }
 if(bytes.size)sink->bytes->insert(sink->bytes->end(),bytes.data,bytes.data+bytes.size);
 return true;
}
template<class QuestOwner>
auto save_quest_payload(QuestOwner& owner,
 const data::player_save_section_writers_v1::WriteServicesV1& stream,
 std::string& error,int)
 ->decltype(owner.save_quests(stream,error),bool()){
 return owner.save_quests(stream,error);
}
template<class QuestOwner>
bool save_quest_payload(QuestOwner&,
 const data::player_save_section_writers_v1::WriteServicesV1&,
 std::string& error,long){
 error="same-Save Native Quest save provider is not available";return false;
}
bool read_profile_file(const std::filesystem::path& path,
                       std::vector<std::uint8_t>& bytes,std::string& error){
 std::ifstream file(path,std::ios::binary|std::ios::ate);
 if(!file){error="existing campaign primary unavailable";return false;}
 const auto size=file.tellg();
 if(size<4||size>32*1024*1024){error="existing campaign size outside native span";return false;}
 bytes.resize(static_cast<std::size_t>(size));file.seekg(0);
 if(!file.read(reinterpret_cast<char*>(bytes.data()),std::streamsize(bytes.size()))){error="existing campaign read failed";return false;}
 return true;
}
enum class CampaignReadV1 { loaded, missing, failed };
CampaignReadV1 read_campaign_candidate_v1(const std::filesystem::path& path,
 std::vector<std::uint8_t>& bytes,std::string& error){
 std::error_code status_error;
 const auto status=std::filesystem::status(path,status_error);
 if(status_error==std::errc::no_such_file_or_directory)return CampaignReadV1::missing;
 if(status_error){error="campaign file inspection failed: "+path.string();return CampaignReadV1::failed;}
 if(!std::filesystem::exists(status))return CampaignReadV1::missing;
 if(!std::filesystem::is_regular_file(status)){
  error="campaign path is not a regular file: "+path.string();return CampaignReadV1::failed;
 }
 std::error_code size_error;const auto size=std::filesystem::file_size(path,size_error);
 if(size_error||size>32u*1024u*1024u){
  error="campaign file size outside native span: "+path.string();return CampaignReadV1::failed;
 }
 std::ifstream file(path,std::ios::binary);
 if(!file){error="campaign file open failed: "+path.string();return CampaignReadV1::failed;}
 bytes.resize(static_cast<std::size_t>(size));
 if(!bytes.empty()&&!file.read(reinterpret_cast<char*>(bytes.data()),
                                static_cast<std::streamsize>(bytes.size()))){
  error="campaign file read failed: "+path.string();return CampaignReadV1::failed;
 }
 if(file.peek()!=std::char_traits<char>::eof()){
  error="campaign changed during read: "+path.string();return CampaignReadV1::failed;
 }
 return CampaignReadV1::loaded;
}
bool usable_campaign_header_v1(const std::vector<std::uint8_t>& bytes){
 return bytes.size()>3&&!(bytes[0]==0xff&&bytes[1]==0xff&&
                          bytes[2]==0xff&&bytes[3]==0xff);
}
bool read_existing_campaign_v1(const std::filesystem::path& primary,
 std::vector<std::uint8_t>& bytes,std::uint32_t& opens,std::string& error){
 bytes.clear();
 auto result=read_campaign_candidate_v1(primary,bytes,error);
 if(result==CampaignReadV1::failed)return false;
 if(result==CampaignReadV1::loaded)++opens;
 // Original cache recovery is limited to a missing primary, a header shorter
 // than one word, or its FFFFFFFF corruption marker. A malformed section
 // index after a usable header is not a reason to retry an arbitrary backup.
 if(result==CampaignReadV1::loaded&&usable_campaign_header_v1(bytes))return true;
 bytes.clear();
 result=read_campaign_candidate_v1(std::filesystem::path(primary.string()+".bak"),bytes,error);
 if(result==CampaignReadV1::failed)return false;
 if(result==CampaignReadV1::loaded)++opens;
 if(result!=CampaignReadV1::loaded||!usable_campaign_header_v1(bytes)){
  error="no usable campaign primary/backup header: "+primary.string();return false;
 }
 error.clear();return true;
}
bool write_synced_file(const std::filesystem::path& path,
                       const std::vector<std::uint8_t>& bytes,std::string& error){
 auto* file=std::fopen(path.string().c_str(),"wb");
 if(!file){error="campaign output open failed";return false;}
 bool okay=std::fwrite(bytes.data(),1,bytes.size(),file)==bytes.size()&&std::fflush(file)==0;
#ifdef _WIN32
 if(okay)okay=::_commit(::_fileno(file))==0;
#else
 if(okay)okay=::fsync(::fileno(file))==0;
#endif
 if(std::fclose(file))okay=false;
 if(!okay)error="campaign output write/flush failed";
 return okay;
}
bool replace_file(const std::filesystem::path& from,
                  const std::filesystem::path& to,std::string& error){
#ifdef _WIN32
 if(::MoveFileExW(from.c_str(),to.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))return true;
 error="campaign output replace failed";return false;
#else
 std::error_code ec;std::filesystem::rename(from,to,ec);
 if(!ec)return true;
 error="campaign output replace failed: "+ec.message();return false;
#endif
}
bool sync_campaign_directory(const std::filesystem::path& target,
                             std::string& error){
#ifdef _WIN32
 (void)target;(void)error;
 return true; // MOVEFILE_WRITE_THROUGH is the Windows rename durability boundary.
#else
 auto parent=target.parent_path();if(parent.empty())parent=".";
 const int fd=::open(parent.c_str(),O_RDONLY);
 if(fd<0){error="campaign directory open for sync failed";return false;}
 const bool okay=::fsync(fd)==0;const int close_result=::close(fd);
 if(!okay||close_result!=0){error="campaign directory sync failed";return false;}
 return true;
#endif
}
}
struct Transport::Impl {
 struct Profile {
  data::PlayerProfileIndexV1 index;
  std::filesystem::path directory;
  std::string filename;
  std::int32_t slot=-1;
  struct Callback {data::PlayerSavegameV1* save=nullptr;bool reader=false,writer=false;};
  std::map<std::string,Callback> callbacks;
 };
 data::PlayerSavegameV1& save;
 data::PlayerSaveProfileV1& profile;
 TransportBindings bindings;
 Receipt receipt;
 std::shared_ptr<Profile> input;
 bool active=false;
 struct GameplayProviderLease {
  std::shared_ptr<Impl> transport;
  std::array<char,4> tag{};
 };
 static bool write_gameplay_payload(void*,const data::PlayerSavegameV1&,
   std::vector<std::uint8_t>&,std::string&);
 Impl(data::PlayerSavegameV1& saved,data::PlayerSaveProfileV1& canonical):save(saved),profile(canonical){}
 void refresh(){
  receipt.slot=save.slot();receipt.character_class=save.class_id();receipt.level=save.level();
  receipt.source_level_id=save.level_name_fields().level_id;
  receipt.sections=profile.campaign?std::uint32_t(profile.campaign.source_sections().size()):0;
 }
 bool continue_load(const data::PlayerSaveLoadRequestV1& q,data::PlayerSaveLoadResponseV1& response,std::string& error){
  using Op=data::PlayerSaveLoadOpV1;
  if(q.operation==Op::init_quests&&bindings.quests)
   return bindings.quests->initialize(q.argument,error);
  if(q.operation==Op::online){
   if(!bindings.online){error="canonical PlayerManager online byte unavailable";return false;}
   response.flag=*bindings.online!=0;error.clear();return true;
  }
  if(q.operation==Op::hosting_quest_flag){
   if(!bindings.hosting_quest_flag){error="PlayerManager quest-host flag unavailable";return false;}
   response.flag=*bindings.hosting_quest_flag!=0;error.clear();return true;
  }
  if(q.operation==Op::init_skills){
   if(!bindings.skill_tables||!bindings.skill_tree_selector){error="live SkillTables/SkillTree providers unavailable";return false;}
   return save.initialize_skills(*bindings.skill_tables,*bindings.skill_tree_selector,error);
  }
  if(q.operation==Op::init_faeries){save.initialize_faeries();error.clear();return true;}
  if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"SKIL")&&bindings.skill_tables){
   std::size_t consumed=0;
   return save.load_skills(q.profile.campaign.payload(q.section),*bindings.skill_tables,consumed,error)==0;
  }
  if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"FAES")){
   std::size_t consumed=0;bool source_count_mismatch=false;
   const bool loaded=save.load_faeries(q.profile.campaign.payload(q.section),consumed,
                                       source_count_mismatch,error);
   // The source reader returns normally after its count-mismatch boundary;
   // retain that result without inventing a different row count or payload.
   (void)source_count_mismatch;
   return loaded;
  }
  if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"PROP")){
   if(!bindings.property_rules||!bindings.properties){error="live gameplay PropertyRules/PropertyState unavailable";return false;}
   auto view=data::property_view(*bindings.property_rules,*bindings.properties);
   std::size_t consumed=0;
   return save.load_properties(q.profile.campaign.payload(q.section),view,consumed,error);
  }
  if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"FTVL")){
   namespace travel=data::player_saved_fast_travel_v1;
   travel::Runtime reader(&save);travel::Result result;
   return reader.load(q.profile.campaign.payload(q.section),&result,error)==travel::Status::complete;
  }
  if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"GEAR")){
   if(!bindings.inventory||!bindings.item_powers||!bindings.inventory_services_owner||
      !bindings.inventory_services.context||!bindings.inventory_services.invoke||
      !bindings.inventory_services.observe_storage||!bindings.inventory_incoming||
      !bindings.property_rules||!bindings.properties||
      bindings.inventory->character()!=save.character()){
    error="GEAR requires the same Character V4 inventory, item-power, property, effect and incoming-item owners";return false;
   }
   auto property_view=data::property_view(*bindings.property_rules,*bindings.properties);
   player_saved_inventory_v1::Bindings input{
    &save,bindings.inventory,&property_view,&bindings.inventory_services,
    bindings.item_powers,bindings.inventory_incoming};
   player_saved_inventory_v1::Runtime reader(std::move(input));
   player_saved_inventory_v1::Result result{};
   const auto status=reader.load(q.profile.campaign.payload(q.section),&result,error);
   if(status!=player_saved_inventory_v1::Status::complete){
    if(*bindings.inventory_incoming){
     std::string retirement_error;
     if(!bindings.inventory->retire_item(
          data::RetainedItemSlotV4{bindings.inventory_incoming},
          bindings.inventory_services,retirement_error)){
      if(!error.empty())error+="; ";
      error+="GEAR partial Item remains owned because source retirement failed: "+retirement_error;
     }
    }
    if(error.empty())error="Source GEAR reader failed";
    return false;
   }
   if(*bindings.inventory_incoming){
    std::string retirement_error;
    if(!bindings.inventory->retire_item(
         data::RetainedItemSlotV4{bindings.inventory_incoming},
         bindings.inventory_services,retirement_error)){
     error="GEAR reader left an untransferred Item and source retirement failed: "+retirement_error;return false;
    }
    error="GEAR reader returned with an untransferred Item";return false;
   }
   error.clear();return true;
  }
  if(bindings.levels&&bindings.world_map){
   if(q.operation==Op::init_levels){
    data::SavedLevelStateServicesV1 tables{this,
     [](void* raw,data::SavedStateTableV1 kind,std::uint32_t* count,std::string& e){
      const auto& b=static_cast<Impl*>(raw)->bindings;
      const auto size=kind==data::SavedStateTableV1::levels?b.levels->levels.size():b.world_map->locations.size();
      if(size>65536){e="actual level-state table exceeds native bound";return false;}
      *count=std::uint32_t(size);return true;
     },
     [](void* raw,data::SavedStateTableV1 kind,std::uint32_t row,std::int32_t* word,std::string& e){
      const auto& b=static_cast<Impl*>(raw)->bindings;
      if(kind==data::SavedStateTableV1::levels){
       if(row<b.levels->levels.size()){*word=b.levels->levels[row].level_state;return true;}
      }else if(data::read_world_map_default_word(*b.world_map,row,*word))return true;
      e="actual level-state default row unavailable";return false;
     },
     {nullptr,[](void*,std::size_t size,int tag)->void*{return tag==0?std::malloc(size):nullptr;},
       [](void*,void* memory){std::free(memory);}}};
    return save.initialize_level_states(tables,error);
   }
   if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"LVLS")){
    namespace states=data::player_saved_level_states_v1;
    // Valid stores need no assertion global. Reached assertion branches reject
    // explicitly until the genuine global/logger binds; no mode is invented.
    states::TableBindings tables{bindings.levels,bindings.world_map};
    states::Runtime reader({&save,states::table_services(tables)});
    states::Result result;
    return reader.load(q.profile.campaign.payload(q.section),&result,error)==states::Status::complete;
   }
  }
  if(q.operation==Op::load_section&&q.section&&!std::strcmp(q.section,"QEST")&&bindings.quests){
   if(!q.profile.campaign){error="QEST campaign snapshot unavailable";return false;}
   quests::Cursor cursor(q.profile.campaign,"QEST");
   return bindings.quests->load_quests(cursor,error);
  }
  const auto services=bindings.continuation;
  if(!services.owner||!services.invoke){error="campaign reached an unbound gameplay/quest provider";return false;}
  return services.invoke(q,response,error);
 }
 bool invoke(const data::PlayerSaveLoadRequestV1& q,data::PlayerSaveLoadResponseV1& response,std::string& error){
  using Op=data::PlayerSaveLoadOpV1;
  if(q.save!=&save||active){error="campaign Save identity/reentry differs";return false;}
  active=true;struct Guard{Impl& state;~Guard(){state.refresh();state.active=false;}}guard{*this};
  if(q.operation==Op::filename){response.text=data::player_profile_filename_v1(q.argument,false,false);return true;}
  if(q.operation==Op::create_profile){
   if(bindings.directory.empty()||!q.filename||std::filesystem::path(q.filename).filename()!=q.filename){error="invalid campaign directory/filename";return false;}
   std::ifstream file(bindings.directory/q.filename,std::ios::binary|std::ios::ate);
   std::vector<std::uint8_t> bytes;
   if(bindings.create_new){
    if(file||std::filesystem::exists(bindings.directory/q.filename)||std::filesystem::exists((bindings.directory/q.filename).string()+".bak")){error="new campaign slot is already occupied";return false;}
    bytes.assign(4,0);
   }else{
    if(!read_existing_campaign_v1(bindings.directory/q.filename,bytes,
                                  receipt.file_opens,error))return false;
   }
   auto next=std::make_shared<Profile>();
   if(!next->index.load({bytes.data(),bytes.size()},error))return false;
   next->directory=bindings.directory;next->filename=q.filename;next->slot=save.slot();
   input=std::move(next);response.profile={reinterpret_cast<std::uintptr_t>(input.get()),input,input->index.borrow()};
   return true;
  }
  if(q.operation==Op::load_section){
   if(!input||q.profile.identity!=reinterpret_cast<std::uintptr_t>(input.get())||q.profile.owner.get()!=input.get()||!q.section||!q.profile.campaign){error="campaign callback owner differs";return false;}
   // Both source registrations precede the read/no-read branch. Writes remain
   // rejecting thunks until source writer/persistence bodies are connected.
   input->callbacks[q.section]={q.save,q.reader_enabled,q.writer_enabled};
   const auto section=q.profile.campaign.section(q.section);
   if(!q.reader_enabled||!section||!section->size){error.clear();return true;}
   if(!metadata_tag(q.section))return continue_load(q,response,error);
   data::PlayerMetadataServicesV1 services{bindings.characters,this,[](void* raw,std::int32_t value,std::string& e){auto& s=*static_cast<Impl*>(raw);if(!s.bindings.current_difficulty){e="source CurrentDifficulty global unavailable";return false;}*s.bindings.current_difficulty=value;s.receipt.difficulty=value;return true;}};
   std::size_t consumed=0;
   if(!data::load_player_metadata_section_v1(q,services,consumed,error))return false;
   if(consumed)++receipt.field_reads;
   return true;
  }
 return continue_load(q,response,error);
 }
};
bool Transport::Impl::write_gameplay_payload(void* raw,
 const data::PlayerSavegameV1& save,std::vector<std::uint8_t>& output,
 std::string& error){
 auto* lease=static_cast<GameplayProviderLease*>(raw);
 if(!lease||!lease->transport||&lease->transport->save!=&save){
  error="gameplay writer lease does not belong to the canonical Save";return false;
 }
 auto& state=*lease->transport;const auto& bindings=state.bindings;
 const std::string tag(lease->tag.data(),lease->tag.size());
 output.clear();GameplayPayloadSink sink{&output};
 const data::PlayerMetadataWriteServicesV1 metadata_stream{&sink,append_gameplay_payload};
 const data::player_save_section_writers_v1::WriteServicesV1 section_stream{&sink,append_gameplay_payload};
 if(metadata_tag(tag.c_str())){
  if(!bindings.characters||!bindings.current_difficulty){error="metadata writer owner unavailable";return false;}
  return data::write_player_metadata_section_v1(tag.c_str(),save,
    *bindings.characters,bindings.current_difficulty,metadata_stream,error);
 }
 if(tag=="LVLS"){
  if(!bindings.levels||!bindings.world_map){error="LVLS table owners unavailable";return false;}
  const data::player_level_states_save_writer_v1::WriteServicesV1 stream{
    &sink,append_gameplay_payload};
  const auto status=data::player_level_states_save_writer_v1::write_lvls_v1(
    *bindings.levels,*bindings.world_map,save,stream,error);
  if(status==data::player_level_states_save_writer_v1::Status::complete)return true;
  if(error.empty())error="LVLS source writer did not complete";
  return false;
 }
 if(tag=="SKIL"){
  if(!bindings.skill_tables||!save.skills_initialized()){
   error="same-Save initialized Skill list and retained SkillTables required";return false;
  }
  const auto status=data::player_skill_save_writer_v1::write_section_v1(
    save,*bindings.skill_tables,
    {section_stream.context,section_stream.write},error);
  if(status==data::player_skill_save_writer_v1::Status::complete)return true;
  if(error.empty())error="SKIL source writer did not complete";
  return false;
 }
 if(tag=="FAES"||tag=="CFEE"||tag=="FTVL"){
  const auto status=data::player_save_section_writers_v1::write_section_v1(
    tag.c_str(),save,section_stream,error);
  if(status==data::player_save_section_writers_v1::Status::complete)return true;
  if(error.empty())error=tag+" source writer did not complete";
  return false;
 }
 if(tag=="QEST"){
  if(!bindings.quests||!bindings.quests->owns_save(&save)){
   error="same-Save Native Quest serializer owner unavailable";return false;
  }
  return save_quest_payload(*bindings.quests,section_stream,error,0);
 }
 if(tag=="PROP"){
  if(!bindings.property_rules||!bindings.properties||!save.character()){
   error="same-Character live property sheets and rules required for PROP";return false;
  }
  auto view=data::property_view(*bindings.property_rules,*bindings.properties);
  const auto status=data::player_property_save_writer_v1::write_properties_v1(
    save,view,section_stream,error);
  if(status==data::player_save_section_writers_v1::Status::complete)return true;
  if(error.empty())error="PROP source writer did not complete";
  return false;
 }
 if(tag=="GEAR"){
  if(!bindings.inventory||!bindings.item_powers||
     !bindings.inventory_services_owner||!bindings.inventory_services.context||
     !bindings.inventory_services.invoke||!bindings.inventory_services.observe_storage||
     !bindings.inventory_incoming||bindings.inventory->character()!=save.character()){
   error="same-Character V4 Inventory, ItemPower and callback leases required for GEAR";return false;
  }
  const auto& slots=bindings.inventory->items();
  const auto& names=bindings.inventory->table().identifiers;
  const auto& powers=bindings.item_powers.names();
  std::uint64_t required=12;
  const auto add_size=[&](std::uint64_t value){
   if(value>UINT32_MAX-required)return false;
   required+=value;return true;
  };
  for(const auto& slot:slots){
   if(!slot||!slot->item){error="GEAR reached an unavailable same-Character Item slot";return false;}
   const auto& item=*slot->item;
   if(item.id<0||std::size_t(item.id)>=names.size()){
    error="GEAR Item ID has no retained source identifier row";return false;
   }
   if(!add_size(21)||!add_size(std::uint64_t(names[std::size_t(item.id)].size())+5)){
    error="GEAR output exceeds source section span";return false;
   }
   for(const auto id:item.powers){
    if(id<0||std::size_t(id)>=powers.size()){
     error="GEAR ItemPower ID has no retained source identifier row";return false;
    }
    if(!add_size(std::uint64_t(powers[std::size_t(id)].size())+5)){
     error="GEAR output exceeds source section span";return false;
    }
   }
  }
  output.resize(static_cast<std::size_t>(required));
  player_gear_save_writer_v1::Result result{};
  const player_gear_save_writer_v1::Bindings input{
    &save,bindings.inventory,bindings.item_powers};
  const auto status=player_gear_save_writer_v1::save(input,
    {output.data(),output.size()},&result,error);
  if(status!=player_gear_save_writer_v1::Status::complete){
   if(error.empty())error="GEAR source writer did not complete";
   return false;
  }
  if(result.written!=required){error="GEAR source writer length differs from its bounded output";return false;}
  error.clear();return true;
 }
 error="no concrete provider for registered gameplay tag: "+tag;return false;
}
Transport::Transport(data::PlayerSavegameV1& save,data::PlayerSaveProfileV1& profile):impl_(std::make_shared<Impl>(save,profile)){
 const auto state=impl_;
 loader_=std::make_unique<data::PlayerSaveLoadOwnerV1>(save,profile,data::PlayerSaveLoadServicesV1{state,[state](const auto& q,auto& response,auto& error){return state->invoke(q,response,error);}});
}
Transport::~Transport()=default;
bool Transport::bind(TransportBindings bindings,std::string& error){
 if(impl_->active){error="cannot rebind campaign transport during delivery";return false;}
 if(bool(bindings.continuation.owner)!=bool(bindings.continuation.invoke)){error="campaign continuation lease and provider disagree";return false;}
 if(bool(bindings.property_rules)!=bool(bindings.properties)){error="gameplay PropertyRules and PropertyState must bind together";return false;}
 if(bool(bindings.levels)!=bool(bindings.world_map)){error="campaign level and WorldMap owners must bind together";return false;}
 if(bindings.quests&&!bindings.quests->owns_save(&impl_->save)){error="Quest factory owner belongs to a different gameplay Save";return false;}
 const bool any_gear_owner=bindings.inventory||bool(bindings.item_powers)||
  bindings.inventory_services.context||bindings.inventory_services.invoke||
  bindings.inventory_services.observe_storage||bindings.inventory_services_owner||
  bindings.inventory_incoming;
 if(any_gear_owner&&(!bindings.inventory||!bindings.item_powers||
     !bindings.inventory_services.context||!bindings.inventory_services.invoke||
     !bindings.inventory_services.observe_storage||!bindings.inventory_services_owner||
     !bindings.inventory_incoming||bindings.inventory->character()!=impl_->save.character())){
  error="GEAR provider bundle must bind the canonical Save Character, V4 inventory, power table, effect lease and incoming Item slot";return false;
 }
 impl_->bindings=std::move(bindings);error.clear();return true;
}
bool Transport::save_all(std::string& error){
 auto& s=*impl_;
 if(s.active||!s.bindings.create_new||!s.bindings.characters||!s.bindings.current_difficulty||!s.input||
    s.profile.identity!=reinterpret_cast<std::uintptr_t>(s.input.get())||s.profile.owner.get()!=s.input.get()||!s.profile.campaign){error="explicit new-profile writer binding required";return false;}
 s.active=true;struct Guard{Impl& state;~Guard(){state.refresh();state.active=false;}}guard{s};
 try{
  std::vector<std::string> writers;
  for(const auto& pair:s.input->callbacks){if(pair.second.save!=&s.save){error="registered writer Save differs";return false;}if(pair.second.writer)writers.push_back(pair.first);}
  if(writers.size()!=7){error="indexed ctor must register all seven metadata writers";return false;}
  std::vector<std::uint8_t> bytes;
  if(!data::serialize_player_metadata_profile_v1(s.profile.campaign,writers,s.save,*s.bindings.characters,s.bindings.current_difficulty,bytes,error))return false;
  // Source saveAll refreshes its cache before opening the output file.
  s.profile.campaign={};
  if(!s.input->index.load({bytes.data(),bytes.size()},error)){s.profile.campaign=s.input->index.borrow();return false;}
  s.profile.campaign=s.input->index.borrow();
  const auto target=s.bindings.directory/data::player_profile_filename_v1(std::uint32_t(s.save.slot()),false,false);
  if(std::filesystem::exists(target)||std::filesystem::exists(target.string()+".bak")){error="new campaign destination became occupied";return false;}
  const auto temporary=std::filesystem::path(target.string()+".creating");
  auto* file=std::fopen(temporary.string().c_str(),"wb");if(!file){error="new campaign temporary open failed";return false;}
  bool okay=std::fwrite(bytes.data(),1,bytes.size(),file)==bytes.size()&&std::fflush(file)==0;
#ifdef _WIN32
  if(okay)okay=::_commit(::_fileno(file))==0;
#else
  if(okay)okay=::fsync(::fileno(file))==0;
#endif
  if(std::fclose(file))okay=false;
  if(!okay){std::filesystem::remove(temporary);error="new campaign durable write failed";return false;}
  if(std::filesystem::exists(target)){std::filesystem::remove(temporary);error="new campaign destination became occupied";return false;}
  if(!publish_new_campaign_file(temporary,target,error)){
   std::filesystem::remove(temporary);return false;
  }
  if(!sync_campaign_directory(target,error))return false;
  error.clear();return true;
 }catch(...){if(error.empty())error="new campaign persistence adapter threw";return false;}
}
bool Transport::save_existing_metadata(std::string& error){
 auto& s=*impl_;
 if(s.active||s.bindings.create_new||s.bindings.directory.empty()||!s.bindings.characters||
    !s.bindings.current_difficulty||!s.input||s.save.slot()<0||
    s.profile.identity!=reinterpret_cast<std::uintptr_t>(s.input.get())||
    s.profile.owner.get()!=s.input.get()||!s.profile.campaign){
  error="loaded existing metadata Save/index and persistence binding required";return false;
 }
 s.active=true;struct Guard{Impl& state;~Guard(){state.refresh();state.active=false;}}guard{s};
 try{
  std::vector<std::string> writers;
  for(const auto& pair:s.input->callbacks){
   if(pair.second.save!=&s.save){error="registered writer Save differs";return false;}
   if(pair.second.writer){if(!metadata_tag(pair.first.c_str())){error="mask-1 metadata writer set required";return false;}writers.push_back(pair.first);}
  }
  if(writers.size()!=7||s.input->callbacks.size()!=7){error="mask-1 indexed Save must register exactly seven metadata writers";return false;}
  const auto target=s.bindings.directory/data::player_profile_filename_v1(std::uint32_t(s.save.slot()),false,false);
  std::vector<std::uint8_t> previous;
  if(!read_profile_file(target,previous,error))return false;
  {
   const auto prior=s.input->index.borrow();
   if(!prior||previous!=prior.bytes()){error="campaign primary changed since its canonical index was loaded";return false;}
  }
  std::vector<std::uint8_t> next;
  if(!data::serialize_player_metadata_profile_v1(s.profile.campaign,writers,s.save,
       *s.bindings.characters,s.bindings.current_difficulty,next,error))return false;

  // Savegame::saveAll refreshes the same +8 index before it submits its
  // storage job. Preserve that ordering and identity when updating this
  // synchronous native adapter.
  s.profile.campaign={};
  if(!s.input->index.load({next.data(),next.size()},error)){
   s.profile.campaign=s.input->index.borrow();return false;
  }
  s.profile.campaign=s.input->index.borrow();

  const auto backup=std::filesystem::path(target.string()+".bak");
  const auto backup_temp=std::filesystem::path(backup.string()+".saving");
  const auto primary_temp=std::filesystem::path(target.string()+".saving");
  if(!write_synced_file(backup_temp,previous,error)){
   std::error_code ignored;std::filesystem::remove(backup_temp,ignored);return false;
  }
  if(!replace_file(backup_temp,backup,error)){
   std::error_code ignored;std::filesystem::remove(backup_temp,ignored);return false;
  }
  if(!sync_campaign_directory(target,error))return false;
  if(!write_synced_file(primary_temp,next,error)){
   std::error_code ignored;std::filesystem::remove(primary_temp,ignored);return false;
  }
  if(!replace_file(primary_temp,target,error)){
   std::error_code ignored;std::filesystem::remove(primary_temp,ignored);return false;
  }
  if(!sync_campaign_directory(target,error))return false;
  error.clear();return true;
 }catch(...){if(error.empty())error="existing campaign save adapter threw";return false;}
}
bool Transport::save_gameplay(
 const std::vector<GameplaySectionPayloadProviderV1>& providers,
 std::string& error){
 auto& s=*impl_;
 using namespace data::player_profile_atomic_replace_v1;
 if(s.active||s.bindings.create_new||s.bindings.directory.empty()||
    !s.bindings.characters||!s.bindings.current_difficulty||
    !s.bindings.online||*s.bindings.online!=0||s.save.source_save_blocked()||
    s.save.slot()<0||!s.save.character()||!s.input||!s.profile.identity||
    s.profile.identity!=reinterpret_cast<std::uintptr_t>(s.input.get())||
    s.profile.owner.get()!=s.input.get()||!s.profile.campaign||
    s.input->slot!=s.save.slot()||s.input->directory!=s.bindings.directory||
    s.input->filename!=data::player_profile_filename_v1(
       std::uint32_t(s.save.slot()),false,false)||
    s.input->callbacks.size()!=kMask1And4WriterTags.size()){
  error="offline gameplay Save, unblocked same-slot profile/index and all mask-1/mask-4 registrations required";return false;
 }
 // Require the exact source registration set, all enabled for writes and all
 // attached to this one Save. No absent/extra callback can be silently skipped.
 for(const auto* tag:kMask1And4WriterTags){
  const auto found=s.input->callbacks.find(tag);
  if(found==s.input->callbacks.end()||found->second.save!=&s.save||
     !found->second.writer){
   error=std::string("registered source writer missing or belongs to another Save: ")+tag;return false;
  }
 }
 std::map<std::string,const GameplaySectionPayloadProviderV1*,std::less<>> by_tag;
 for(const auto& provider:providers){
  if(std::memchr(provider.tag.data(),0,provider.tag.size())||!provider.owner||
     !provider.write){error="gameplay section provider has an invalid tag, lease or writer";return false;}
  const std::string tag(provider.tag.data(),provider.tag.size());
  if(!by_tag.emplace(tag,&provider).second){error="duplicate gameplay section payload provider: "+tag;return false;}
 }
 if(by_tag.size()!=kMask1And4WriterTags.size()){
  error="gameplay payload providers must exactly cover registered mask-1 and mask-4 writers";return false;
 }
 for(const auto* tag:kMask1And4WriterTags)if(by_tag.find(tag)==by_tag.end()){
  error=std::string("gameplay payload provider missing registered writer: ")+tag;return false;
 }
 for(const auto& row:by_tag){
  bool registered=false;for(const auto* tag:kMask1And4WriterTags)
   if(row.first==tag){registered=true;break;}
  if(!registered){error="extra gameplay payload provider is not registered: "+row.first;return false;}
 }

 s.active=true;
 struct Guard{Impl& state;~Guard(){state.refresh();state.active=false;}}guard{s};
 try{
  // PlayerSavegame::SG_Save stores its offline source mode before invoking
  // registered callbacks; a later provider failure retains this reached field.
  s.save.set_source_save_mode(1);
  struct Payload {std::array<char,4> tag{};std::vector<std::uint8_t> bytes;};
  std::vector<Payload> payloads;payloads.reserve(kMask1And4WriterTags.size());
  for(const auto* tag:kMask1And4WriterTags){
   const auto* provider=by_tag.at(tag);Payload payload;
   std::memcpy(payload.tag.data(),tag,payload.tag.size());
   if(!provider->write(provider->context,s.save,payload.bytes,error)){
    if(error.empty())error=std::string("registered gameplay section writer failed: ")+tag;
    return false;
   }
   if(payload.bytes.size()>UINT32_MAX){error=std::string("gameplay section exceeds source span: ")+tag;return false;}
   payloads.push_back(std::move(payload));
  }
  if(s.save.source_save_blocked()||!s.bindings.online||*s.bindings.online!=0||
     s.save.slot()!=s.input->slot||!s.profile.campaign||
     s.profile.identity!=reinterpret_cast<std::uintptr_t>(s.input.get())||
     s.profile.owner.get()!=s.input.get()){
   error="gameplay Save/profile/offline identity changed during section serialization";return false;
  }
  std::vector<data::PlayerProfileRawSectionV1> replacements;
  replacements.reserve(payloads.size());
  for(const auto& payload:payloads)
   replacements.push_back({payload.tag,{payload.bytes.data(),payload.bytes.size()}});
  Result receipt{};
  const auto primary=s.input->directory/s.input->filename;
  if(!replace_existing_profile_sections_v1(primary,s.input->index,
       s.profile.campaign,replacements,&receipt,error))return false;
  if(!s.input->index.owns(s.profile.campaign)||
     s.profile.campaign.bytes().empty()){
   error="gameplay profile index publication did not retain the canonical Save view";return false;
  }
  error.clear();return true;
 }catch(...){if(error.empty())error="offline gameplay profile save provider threw";return false;}
}
bool Transport::save_gameplay(std::string& error){
 auto& state=*impl_;const auto& bindings=state.bindings;
 if(state.active||!bindings.characters||!bindings.current_difficulty||
    !bindings.levels||!bindings.world_map||!bindings.skill_tables||
    !state.save.skills_initialized()||!bindings.quests||
    !bindings.quests->owns_save(&state.save)||
    !bindings.property_rules||!bindings.properties||
    !bindings.inventory||!bindings.item_powers||
    !bindings.inventory_services_owner||!bindings.inventory_services.context||
    !bindings.inventory_services.invoke||!bindings.inventory_services.observe_storage||
    !bindings.inventory_incoming||!state.save.character()||
    bindings.inventory->character()!=state.save.character()||
    bindings.inventory->properties()!=bindings.properties||
    !bindings.online||*bindings.online!=0||state.save.source_save_blocked()){
  error="offline gameplay persistence requires every same-Save metadata, level, skill, quest, property and inventory writer owner";return false;
 }
 for(const auto initialized:state.save.faeries_initialized())if(!initialized){
  error="offline gameplay persistence requires all same-Save faery lists initialized";return false;
 }
 for(std::uint32_t difficulty=0;difficulty<3;++difficulty){
  const auto* levels=state.save.source_level_states(difficulty);
  const auto* world=state.save.source_world_map_states(difficulty);
  if(!levels||!world||!levels->words||!world->words||
     levels->count!=bindings.levels->levels.size()||
     world->count!=bindings.world_map->locations.size()){
   error="offline gameplay persistence requires all same-Save LVLS arrays";return false;
  }
 }
 std::vector<GameplaySectionPayloadProviderV1> providers;
 providers.reserve(kMask1And4WriterTags.size());
 for(const auto* tag:kMask1And4WriterTags){
  auto lease=std::make_shared<Impl::GameplayProviderLease>();lease->transport=impl_;
  std::memcpy(lease->tag.data(),tag,lease->tag.size());
  providers.push_back({lease->tag,lease,lease.get(),Impl::write_gameplay_payload});
 }
 return save_gameplay(providers,error);
}
bool Transport::load_checkpoint_quests(
 const std::vector<std::uint8_t>& image,std::string& error){
 auto& state=*impl_;
 if(state.active||!state.input||state.save.slot()<0||!state.profile.identity||
    state.profile.identity!=reinterpret_cast<std::uintptr_t>(state.input.get())||
    state.profile.owner.get()!=state.input.get()||!state.profile.campaign||
    state.input->slot!=state.save.slot()||!state.bindings.quests||
    !state.bindings.quests->owns_save(&state.save)){
  error="checkpoint QEST load requires this active canonical Save/profile/Quest owner";
  return false;
 }
 data::PlayerProfileIndexV1 checkpoint_index;
 if(!checkpoint_index.load({image.data(),image.size()},error))return false;
 const auto campaign=state.profile.campaign;
 struct RestoreCampaign {
  data::PlayerSaveProfileV1& profile;
  data::PlayerProfileIndexV1::Borrow campaign;
  ~RestoreCampaign(){profile.campaign=std::move(campaign);}
 } restore{state.profile,campaign};
 state.profile.campaign=checkpoint_index.borrow();
 // This is the authored PlayerSavegame::SG_LoadCheckpoint(16) route: the
 // same LoadOwner initializes both quest logs and dispatches only QEST from
 // the temporary checkpoint image. No parallel Save/Quest owner is created.
 if(!loader_->load(16,error))return false;
 state.refresh();
 error.clear();
 return true;
}
data::PlayerSaveLoadOwnerV1& Transport::loader()noexcept{return *loader_;}
const Receipt& Transport::receipt()const noexcept{impl_->refresh();return impl_->receipt;}

struct Metadata::Impl {
 std::shared_ptr<data::PlayerSavegameV1> metadata=std::make_shared<data::PlayerSavegameV1>();
 std::shared_ptr<data::PlayerSaveProfileV1> profile=std::make_shared<data::PlayerSaveProfileV1>();
 std::shared_ptr<Transport> transport=std::make_shared<Transport>(*metadata,*profile);
 Receipt receipt;
 std::filesystem::path directory;
 bool active=false;
};
Metadata::Metadata():impl_(new Impl){}
Metadata::~Metadata()=default;
bool Metadata::load(std::int32_t slot,const std::filesystem::path& directory,const data::CharacterTable& characters,std::int32_t& current_difficulty,std::string& error){
 auto& s=*impl_;
 if(s.active||slot<0||directory.empty()){error="invalid/reentrant metadata input";return false;}
 if(s.metadata->slot()!=-1&&(s.metadata->slot()!=slot||s.directory!=directory)){error="metadata owner belongs to another selected slot/directory";return false;}
 // Manage creates +680 through the indexed C1(slot,1,false), whose level
 // default is 1. A blank Save followed by SetSlot would incorrectly retain
 // level0 when a valid profile has no PLVL section. Repeat reads reuse C1.
 if(s.metadata->slot()==-1&&!s.metadata->initialize_new_profile_metadata(slot,error))return false;
 s.directory=directory;s.active=true;s.receipt.loaded=false;
 struct Guard{Impl& state;~Guard(){std::string unused;state.transport->bind({},unused);state.active=false;}}guard{s};
 if(!s.transport->bind({directory,&characters,&current_difficulty,{}},error))return false;
 if(!s.transport->loader().load(1,error)){s.receipt=s.transport->receipt();return false;}
 s.receipt=s.transport->receipt();
 if(!s.metadata->level_name_loaded()||!s.metadata->use_spawn_points_loaded()){error="campaign metadata omitted required initialized fields";return false;}
 s.receipt.loaded=true;
 return true;
}
bool Metadata::save_numeric_request(const data::CharacterTable& characters,
 std::int32_t& current_difficulty,std::string& error){
 auto& s=*impl_;
 if(s.active||!s.receipt.loaded||s.metadata->slot()<0||s.directory.empty()){
  error="loaded NativeStartGame temporary Save required";return false;
 }
 s.active=true;
 struct Guard{Impl& state;~Guard(){std::string ignored;state.transport->bind({},ignored);state.active=false;}}guard{s};
 if(!s.transport->bind({s.directory,&characters,&current_difficulty,{},false},error))return false;
 return s.transport->save_existing_metadata(error);
}
bool Metadata::clear_spawn_point_and_save(std::size_t difficulty,
 const data::CharacterTable& characters,std::int32_t& current_difficulty,std::string& error){
 auto& s=*impl_;
 if(s.active||!s.receipt.loaded||s.metadata->slot()<0||s.directory.empty()){
  error="loaded NativeStartGame temporary Save required";return false;
 }
 s.active=true;
 struct Guard{Impl& state;~Guard(){std::string ignored;state.transport->bind({},ignored);state.active=false;}}guard{s};
 if(!s.transport->bind({s.directory,&characters,&current_difficulty,{},false},error))return false;
 if(!s.metadata->clear_use_spawn_point(difficulty,error))return false;
 return s.transport->save_existing_metadata(error);
}
const Receipt& Metadata::receipt()const noexcept{return impl_->receipt;}
const data::PlayerSavegameV1& Metadata::save()const noexcept{return *impl_->metadata;}
Metadata::SharedSession Metadata::share_session()const noexcept{
 return {impl_->metadata,impl_->profile,impl_->transport};
}
bool Metadata::create_character_session(std::uintptr_t character,
 CharacterSession& out,std::string& error)const{
 const auto& s=*impl_;
 if(!s.receipt.loaded||s.metadata->slot()<0||!character){
  error="loaded selected metadata and actual Character identity required";return false;
 }
 if(out.save||out.profile||out.transport){
  error="Character session output must be empty";return false;
 }
 // Application::LoadLevel receives the slot, not the temporary NativeStartGame
 // Save pointer. Character::InitializePlayerSavegame then allocates its own
 // Save and its matching profile/LoadOwner before InitPost calls SG_Load(4).
 auto save=std::make_shared<data::PlayerSavegameV1>();
 save->set_character(character);save->set_slot(s.metadata->slot());
 auto profile=std::make_shared<data::PlayerSaveProfileV1>();
 auto transport=std::make_shared<Transport>(*save,*profile);
 out=CharacterSession(std::move(save),std::move(profile),std::move(transport));
 error.clear();return true;
}
std::uintptr_t Metadata::profile_identity()const noexcept{return impl_->profile->identity;}
std::uintptr_t Metadata::save_identity()const noexcept{return reinterpret_cast<std::uintptr_t>(impl_->metadata.get());}
}
