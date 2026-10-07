#include "game_object_arrays_owner_v81.hpp"
#include <cstring>
namespace dh2::loader {
struct GameObjectArraysOwnerV81::Snapshot {
 std::vector<std::uint8_t> records,names;
 std::array<GroupSpan,11> groups;
 std::vector<std::string> door_names,trigger_names;
 std::vector<world::DoorDeclarationRowV77> doors;
 std::vector<world::TriggerObjectDeclarationRowV78> triggers;
 world::OpenableContainerTableV1 openable;
 world::DestructibleContainerTableV16 destructible;
 data::GameObjectDictionaryV11 dictionary;
};
namespace {
struct Cursor {
 const std::vector<std::uint8_t>& bytes;std::size_t at{};std::string& error;
 bool word(std::uint32_t& value){if(bytes.size()-at<4){error="Short original game_objects word";return false;}value=std::uint32_t(bytes[at])|(std::uint32_t(bytes[at+1])<<8)|(std::uint32_t(bytes[at+2])<<16)|(std::uint32_t(bytes[at+3])<<24);at+=4;return true;}
 bool integer(std::int32_t& value){std::uint32_t raw{};if(!word(raw))return false;std::memcpy(&value,&raw,4);return true;}
 bool string(std::string* out=nullptr){std::uint32_t size{};if(!word(size))return false;if(size>bytes.size()-at){error="Short original game_objects CString";return false;}if(out)out->assign(reinterpret_cast<const char*>(bytes.data()+at),size);at+=size;return true;}
 bool row(const char* shape){for(;*shape;++shape){if(*shape=='S'){if(!string())return false;}else if(*shape=='b'){if(at==bytes.size()){error="Short original game_objects bool";return false;}++at;}else{std::uint32_t ignored{};if(!word(ignored))return false;}}return true;}
};
// Exact PyDataArrays.C1 4be550 addFuncsForFile order on game_objects streams.
// Shapes follow each original Structs::*read; S is source len32+bytes, b one
// byte, i signed word. Retain full raw groups even where no class uses them.
constexpr const char* group_names[]{"DestructibleContainers","Doors","ExplosiveTraps","GameObjectDamager","LiftableObjects","OpenableContainers","ProjectileTraps","TimerTraps","TriggerObjects","TriggerPlates","TriggerTraps"};
constexpr const char* shapes[]{"iiiiiiibiiSiiii","Siii","iiSi","iiiii","i","iibiSiii","iiibiSii","iiiSii","iSii","ibiiiii","iiSii"};
std::size_t minimum(const char* shape){std::size_t size{};for(;*shape;++shape)size+=*shape=='b'?1:4;return size;}
}
bool GameObjectArraysOwnerV81::initialize(const std::uint8_t* records,std::size_t n,const std::uint8_t* names,std::size_t nn,const std::uint8_t* dictionary,std::size_t dn,const std::uint8_t* dictionary_names,std::size_t dnn,std::string& e){
 if(snapshot_){e="Actual game_objects Arrays snapshot already initialized";return false;}
 if(!records||!names||n<4||nn<4){e="Required complete actual game_objects streams";return false;}
 auto same=std::make_shared<Snapshot>();same->records.assign(records,records+n);same->names.assign(names,names+nn);
 Cursor r{same->records,0,e},keys{same->names,0,e};
 for(std::size_t group=0;group<same->groups.size();++group){
  auto& span=same->groups[group];span.name=group_names[group];span.records_offset=r.at;
  if(!r.word(span.count)||span.count>(r.bytes.size()-r.at)/minimum(shapes[group])){if(e.empty())e="Invalid original game_objects group count";return false;}
  span.names_read=true; // skipNames4b662c tail-branches to actual readNames4b6494.
  std::vector<std::string>* labels=group==1?&same->door_names:group==8?&same->trigger_names:nullptr;
  if(span.names_read){span.names_offset=keys.at;std::uint32_t count{};
   if(!keys.word(count)||count!=span.count||count>(keys.bytes.size()-keys.at)/4){if(e.empty())e="Original game_objects names/count mismatch";return false;}
   if(labels)labels->reserve(count);
   for(std::uint32_t i=0;i<count;++i){std::string value;if(!keys.string(labels?&value:nullptr))return false;if(labels)labels->push_back(std::move(value));}
   span.names_size=keys.at-span.names_offset;
  }
  if(group==1)same->doors.reserve(span.count);if(group==8)same->triggers.reserve(span.count);
  for(std::uint32_t i=0;i<span.count;++i){
   if(group==1){world::DoorDeclarationRowV77 row;if(!r.string(&row.script4)||!r.integer(row.sound_c)||!r.integer(row.sound10)||!r.integer(row.visual14))return false;same->doors.push_back(std::move(row));}
   else if(group==8){world::TriggerObjectDeclarationRowV78 row;if(!r.integer(row.raw4)||!r.string(&row.external_script_c)||!r.integer(row.sound10)||!r.integer(row.visual14))return false;same->triggers.push_back(std::move(row));}
   else if(!r.row(shapes[group]))return false;
  }
  span.records_size=r.at-span.records_offset;
 }
 if(r.at!=r.bytes.size()||keys.at!=keys.bytes.size()){e="Unexpected original grouped game_objects stream tail";return false;}
 const auto load=[&](std::size_t group,auto& table){const auto& span=same->groups[group];return table.load(same->records.data()+span.records_offset,span.records_size,same->names.data()+span.names_offset,span.names_size,e);};
 // Existing actual family decoders own each table exactly once in this same
 // grouped snapshot; adapters receive aliases, never another decoded table.
 if(!load(0,same->destructible)||!load(5,same->openable)||!same->dictionary.load(dictionary,dn,dictionary_names,dnn,e))return false;
 snapshot_=std::move(same);e.clear();return true;
}
bool GameObjectArraysOwnerV81::borrow(Borrow& out,std::string& e)const{
 if(!snapshot_){e="Required initialized original grouped game_objects Arrays owner";return false;}
 Borrow same;same.receiver=snapshot_;same.dictionary=std::shared_ptr<const data::GameObjectDictionaryV11>(snapshot_,&snapshot_->dictionary);
 same.doors={same.receiver,&snapshot_->door_names,&snapshot_->doors,same.dictionary};same.triggers={same.receiver,&snapshot_->trigger_names,&snapshot_->triggers,same.dictionary};
 same.openable=std::shared_ptr<const world::OpenableContainerTableV1>(snapshot_,&snapshot_->openable);same.destructible=std::shared_ptr<const world::DestructibleContainerTableV16>(snapshot_,&snapshot_->destructible);
 out=std::move(same);e.clear();return true;
}
}
