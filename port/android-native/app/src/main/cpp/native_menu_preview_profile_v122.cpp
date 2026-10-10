#include "native_menu_preview_profile_v122.hpp"
#include "application_save_files_owner_v61.hpp"
#include "world_map_profile_table_v59.hpp"
#include "player_save_inventory_writer_v45.hpp"
#include "owned_hud_settings_v1.hpp"
#include <cstdio>
#include <cstring>
namespace model_renderer {namespace {
struct PreviewProfileV122:std::enable_shared_from_this<PreviewProfileV122> {
 std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> actor;
 MenuPreviewProcessDomainV121 domain;
 std::shared_ptr<dh2::level::CampaignSaveProfileV45> profile;
 std::shared_ptr<dh2::data::WorldMapProfileTableV59> maps;
 std::shared_ptr<dh2::data::QuestTablesPersistenceV51> quests;
 std::shared_ptr<dh2::character::CharacterMenuQuestsV51> quest_owner;
 std::shared_ptr<dh2::character::CharacterMenuProfileLoadV51> reader;
 std::shared_ptr<dh2::character::CharacterMenuCampaignSaveV50> writer;
 dh2::data::ItemPowerTablesV5::Borrow powers;
 std::vector<std::int32_t> level_defaults;
 bool read(const char* name,std::vector<std::uint8_t>& bytes,std::string& e){bool found{};if(!domain.read||!domain.read(name,found,bytes,e))return false;if(!found){e=std::string("Actual preview profile table missing: ")+name;return false;}return true;}
};
}
bool prepare_native_menu_preview_profile_v122(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 const MenuPreviewProcessDomainV121& domain,dh2::world::CanonicalCharacterCandidateRecordV60& r,std::shared_ptr<void>& owner,std::string& e){
 if(owner||!app||!r.save||!r.load||!r.save_fields||r.save->character()!=r.actor->object->identity||&r.load->save()!=r.save.get()){
  e="Required fresh SAME preview Save14e8/SaveLoad before profile read";return false;
 }
 auto p=std::make_shared<PreviewProfileV122>();owner=p;p->actor=r.shared_from_this();p->domain=domain;
 const auto* levels=r.design.levels();if(!levels){e="Required actual process Level tables for Save initializers";return false;}
 p->level_defaults.reserve(levels->levels.size());for(const auto& row:levels->levels){std::int32_t v;const auto raw=row.scalar.words[0x28/4];std::memcpy(&v,&raw,4);p->level_defaults.push_back(v);}
 std::array<std::vector<std::uint8_t>,3> bytes;
 const char* uris[]{"data/pydata/worldmap_pyarray.bin","data/pydata/worldmap_pyarraynames.bin","data/pydata/worldmap_pystructnames.bin"};
 for(unsigned i=0;i<3;++i)if(!p->read(uris[i],bytes[i],e))return false;
 p->maps=std::make_shared<dh2::data::WorldMapProfileTableV59>();
 if(!p->maps->decode({bytes[0].data(),bytes[0].size()},{bytes[1].data(),bytes[1].size()},{bytes[2].data(),bytes[2].size()},e))return false;
 std::vector<std::uint8_t> array,names;
 if(!p->read("data/pydata/v2quests_pyarray.bin",array,e)||!p->read("data/pydata/v2quests_pyarraynames.bin",names,e))return false;
 p->quests=std::make_shared<dh2::data::QuestTablesPersistenceV51>();if(!p->quests->decode({array.data(),array.size()},{names.data(),names.size()},e))return false;
 p->quest_owner=std::make_shared<dh2::character::CharacterMenuQuestsV51>(r.save,p->quests);
 if(r.save->slot()!=-1){
  auto files=app->source_save_files_v61();if(!files||files->jobs()->failed()||files->jobs()->busy_v61()){e="Required actual idle Application FileManager/jobs before preview profile C1";return false;}
  char filename[32];const auto n=std::snprintf(filename,sizeof(filename),"dh2_%03u.savegame",static_cast<unsigned>(r.save->slot()));
  if(n<=0||std::size_t(n)>=sizeof(filename)){e="Preview profile filename overflow";return false;}
  p->profile=std::make_shared<dh2::level::CampaignSaveProfileV45>(filename,files->files()->services(),files->jobs());
  if(!p->profile->construct(e)||!r.load->publish_profile(p->profile->receiver(),e))return false;
 }
 dh2::character::CharacterMenuProfileLoadServicesV51 reads;reads.tables=domain.owner;
 reads.characters=r.design.characters();reads.skills=r.services.skills;reads.quests=p->quest_owner;
 reads.level_names=&levels->level_names;reads.level_defaults28=&p->level_defaults;reads.map_names=&p->maps->names();reads.map_defaults8=&p->maps->defaults8();
 auto settings=app->source_settings4c_v67();if(!settings){e="Required SAME App4c difficulty for preview PDFL";return false;}
 reads.store_current_difficulty=[settings](auto value,auto& e){settings->source_set_current_difficulty_v67(value);e.clear();return true;};
 auto weak=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(r.shared_from_this());
 reads.character_skill_list=[weak](auto id,const std::vector<std::int32_t>*& out,auto& e){auto r=weak.lock();if(!r||r->actor->object->identity!=id||!r->services.skills){e="Required SAME preview Character skill list";return false;}auto index=r->properties->resolved[28];const auto& lists=r->services.skills.lists();if(index<0||std::size_t(index)>=lists.size())index=3;if(index<0||std::size_t(index)>=lists.size()){e="Actual preview SkillList fallback3 missing";return false;}out=&lists[std::size_t(index)];return true;};
 const auto online=app->get_online_loading_v55();
 reads.remaining={domain.owner,[weak,online,profile=std::weak_ptr<PreviewProfileV122>(p)](const auto& q,auto& out,auto& e){
  if(q.operation==dh2::data::PlayerSaveLoadOpV1::online&&online){out.flag=online->byte5()!=0;e.clear();return true;}
  auto r=weak.lock();auto p=profile.lock();if(!r||!p||q.save!=r->save.get()){e="Retired SAME preview Save reader";return false;}
  if(q.operation==dh2::data::PlayerSaveLoadOpV1::load_section&&q.section&&p->profile&&q.profile.identity==reinterpret_cast<std::uintptr_t>(p->profile.get())){
   auto cache=p->profile->cache();if(!cache.section(q.section)){e.clear();return true;}const auto payload=cache.payload(q.section);
   if(!std::strcmp(q.section,"PROP")){std::size_t used{};return dh2::level::player_save_properties_reader_v45(*r->save,r->view,payload,used,e);}
   if(!std::strcmp(q.section,"GEAR")){auto* gear=r->prepared_equipment_v60;if(!gear||gear->inventory()!=r->inventory37c){e="Required SAME preview Gear before GEAR delivery";return false;}dh2::data::InventoryLoadReceiptV1 receipt;return gear->ready()?gear->load_source_inventory_v122(payload,receipt,e):gear->load_saved_section_v59(payload,receipt,e);}
  }
  e="Required actual preview SaveLoad network operation "+std::to_string(unsigned(q.operation));return false;
 }};
 p->reader=std::make_shared<dh2::character::CharacterMenuProfileLoadV51>(r.save,p->profile,std::move(reads));
 if(!r.load->bind_services_v50(p->reader->load_services(),e))return false;
 return r.load->load(1,e); //actual PCLS before SafeGetCharPropsId, no mask2 replay
}
bool finish_native_menu_preview_profile_v122(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app,
 dh2::world::CanonicalCharacterCandidateRecordV60& r,std::string& e){
 auto p=std::static_pointer_cast<PreviewProfileV122>(r.preview_profile_v122);
 if(!p||p->actor.lock().get()!=&r||!r.equipment||!r.equipment->ready()||r.equipment->inventory()!=r.inventory37c){e="Required completed SAME preview Gear/profile";return false;}
 if(!p->profile){if(r.save->slot()!=-1||r.load->profile().identity){e="Changed actual NULL preview profile";return false;}e.clear();return true;} //Save's genuine NULL-profile return
 if(p->writer){e="Preview section writers already registered";return false;}
 dh2::data::LootTablesV2::Borrow loot;dh2::data::ItemTextServicesV5 text;dh2::data::LootRandom8V2* random{};
 if(!r.equipment->loot_sources_v8(loot,p->powers,text,random,e)||!p->powers||random!=r.services.random)return false;
 const auto settings=app?app->source_settings4c_v67():nullptr;if(!settings){e="Required actual App4c for preview writer";return false;}
 dh2::character::CharacterMenuCampaignSaveServicesV50 services;services.tables=p->domain.owner;
 services.characters=r.design.characters();services.skills=r.services.skills;services.power_names=&p->powers.names();services.level_names=&r.design.levels()->level_names;services.map_names=&p->maps->names();
 services.current_difficulty=[settings](auto& out,auto& e){out=settings->current_difficulty_v67();e.clear();return true;};
 auto weak=std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60>(r.shared_from_this());
 services.actor=[weak](auto& out,auto& e){auto r=weak.lock();if(!r||!r->equipment||r->equipment->inventory()!=r->inventory37c){e="Retired preview writer Character/Gear";return false;}out.receiver=r;out.properties=r->equipment->property_view();out.inventory=r->inventory37c;return true;};
 auto weak_quests=std::weak_ptr<dh2::character::CharacterMenuQuestsV51>(p->quest_owner);
 services.quest_save_data=[weak,weak_quests](auto id,auto& stream,auto& e){
  auto r=weak.lock();auto quests=weak_quests.lock();
  if(!r||!quests||quests->save()!=r->save){e="Required SAME preview Save/Quest persistence owner";return false;}
  return quests->save_quest(id,stream,e);
 };
 const auto online=app->get_online_loading_v55();services.remaining={p->domain.owner,[online](const auto& q,auto& out,auto& e){if(q.operation==dh2::data::PlayerSaveWriteOpV1::online&&online){out.flag=online->byte5()!=0;e.clear();return true;}e="Required positive preview SG_Save network/checkpoint producer";return false;}};
 p->writer=std::make_shared<dh2::character::CharacterMenuCampaignSaveV50>(r.load,p->profile,std::move(services));return p->writer->bind(e);
}
bool save_native_menu_preview_profile_v122(dh2::world::CanonicalCharacterCandidateRecordV60& r,std::string& e){
 auto p=std::static_pointer_cast<PreviewProfileV122>(r.preview_profile_v122);if(!p||!r.load){e="Required actual preview Save authority";return false;}
 if(p->profile&&(!p->writer||!p->writer->ready())){e="Required full actual preview section writers";return false;}
 dh2::data::PlayerSaveWriteOwnerV1 writer(r.load,p->writer?p->writer->write_services():dh2::data::PlayerSaveWriteServicesV1{});return writer.save(e);
}
bool destroy_native_menu_preview_profile_v122(dh2::world::CanonicalCharacterCandidateRecordV60& r,std::uintptr_t captured,std::string& e){
 auto p=std::static_pointer_cast<PreviewProfileV122>(r.preview_profile_v122);
 if(!p||p->actor.lock().get()!=&r||!r.save||!r.load||!r.save_fields||captured!=*r.save_fields->save_slot14e8()||captured!=reinterpret_cast<std::uintptr_t>(r.save.get())){e="Preview Save D0 requires SAME14e8/profile owner";return false;}
 if(!r.load->destroy_source_v108([p](const auto& actual,auto& e){if(!p->profile||actual.identity!=reinterpret_cast<std::uintptr_t>(p->profile.get())||actual.owner.get()!=p->profile.get()){e="Preview Profile D0 receiver mismatch";return false;}return p->profile->destroy_source_v108(e);},[p](auto selector,auto& e){return p->quest_owner->destroy_collection_v108(selector,e);},e))return false;
 if(!r.save_fields->source_save_destroyed_null_store_v108(captured,e))return false;
 r.load.reset();p->writer.reset();p->reader.reset();r.save.reset();e.clear();return true;
}
}
