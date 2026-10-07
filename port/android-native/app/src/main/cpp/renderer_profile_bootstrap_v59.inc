// Includes required by root: character_profile_bootstrap_v59.hpp,
// private_save_file_transport_v45.hpp, front_selected_profile_v50.hpp.
// Also world_map_profile_table_v59.hpp and level_tables.hpp.
// This connection is source campaign-only. It neither substitutes a Crypt
// actor nor completes GS/PlayerManager/Character construction by declaration.
struct RendererProfileBootstrapInputsV59 {
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> selected;
 // SAME source C1 Save and SaveLoad/profile authority supplied by the native
 // player receiver before Gear. SkillV6 later adopts these exact leases.
 std::shared_ptr<dh2::data::PlayerSavegameV1> save;
 std::shared_ptr<dh2::data::PlayerSaveLoadOwnerV1> load;
 std::uintptr_t character{};
 std::shared_ptr<void> source_cells;
 const std::uintptr_t* save14e8{};
 const std::int32_t* player_info_slot664{};
 const std::int32_t* direct_save_slot_v122{};
 std::function<bool(std::int32_t,std::string&)> set_slot;
 // ONE Application-global source jobs owner. Never allocate per-profile jobs.
 std::shared_ptr<dh2::level::PrivateSaveFileTransportV45> private_files;
 std::shared_ptr<dh2::level::SavegameJobsOwnerV2> application_jobs;
 std::shared_ptr<void> cache_owner;
 std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::string&)> read_cache;
 const dh2::data::LevelTables* actual_levels{}; // Pinned by reads.tables.
 dh2::character::CharacterMenuProfileLoadServicesV51 reads;
 dh2::data::PlayerSaveLoadServicesV1 remaining;
 bool defer_mask2_to_initpost_v62{};
};
class RendererProfileBootstrapV59 {
 struct ReaderTables {
  std::shared_ptr<void> actual_primary;
  std::vector<std::int32_t> level_defaults28;
  std::shared_ptr<const dh2::data::WorldMapProfileTableV59> maps;
  std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> quests;
 };
 std::shared_ptr<const dh2::android_ui::FrontSelectedProfileV50> selected_;
 std::shared_ptr<dh2::level::CampaignSaveProfileV45> profile_;
 std::shared_ptr<const dh2::data::QuestTablesPersistenceV51> quest_tables_;
 std::shared_ptr<const dh2::data::WorldMapProfileTableV59> map_tables_;
 std::shared_ptr<ReaderTables> reader_tables_;
 const dh2::data::LevelTables* levels_v67_{};
 std::shared_ptr<dh2::character::CharacterProfileBootstrapV59> source_;
public:
 static bool create(RendererProfileBootstrapInputsV59 in,
  std::shared_ptr<RendererProfileBootstrapV59>& out,std::string& e){
  if(out){e="Selected-profile bootstrap receiver already retained; no replay";return false;}
  if(!in.selected||in.selected->metadata.slot<0||in.selected->metadata.slot>=4||
     !in.private_files||!in.application_jobs||!in.cache_owner||!in.read_cache||
     !in.save||!in.load||!in.source_cells||!in.save14e8||(!in.player_info_slot664&&!in.direct_save_slot_v122)||!in.set_slot||
     !in.actual_levels||!in.reads.tables){
   e="Required genuine campaign file/jobs/cache/pre-Gear Save14e8/PlayerInfo bootstrap inputs";return false;
  }
  auto self=std::make_shared<RendererProfileBootstrapV59>();
  out=self; // Preserve any reached real file/Save prefix on failure.
  self->selected_=std::move(in.selected);
  if(in.private_files->directory_v59()!=self->selected_->private_directory||
     in.application_jobs->file_storage_v59().get()!=in.private_files.get()){
   e="Required SAME selected Android private directory/Application FileManager jobs";return false;
  }
  const auto* source_slot=in.direct_save_slot_v122?in.direct_save_slot_v122:in.player_info_slot664;
  if(in.direct_save_slot_v122&&(in.player_info_slot664||in.direct_save_slot_v122!=in.save->source_slot_field_v122())){e="Preview profile requires actual Save slot and no PlayerInfo proxy";return false;}
  if(&in.load->save()!=in.save.get()||in.save->character()!=in.character||
     *in.save14e8!=reinterpret_cast<std::uintptr_t>(in.save.get())||
     *source_slot!=self->selected_->metadata.slot){
   e="Required current SAME Save14e8/PlayerInfo before source SetSlot/profile C1";return false;
  }
  if(!in.set_slot(*source_slot,e))return false;
  if(in.save->slot()!=*source_slot||*source_slot!=self->selected_->metadata.slot){
   e="Actual SetSlot changed selected source receiver before profile C1";return false;
  }
  if(in.application_jobs->failed()||in.application_jobs->busy_v61()){
   e="Required nonfailed idle SAME Application jobs before actual CampaignProfile C1";return false;
  }
  const dh2::character::CharacterProfileSlotStoreReceiptV59 slot_store{in.save.get(),in.save->slot()};
  char filename[32];const auto n=std::snprintf(filename,sizeof(filename),"dh2_%03u.savegame",
    unsigned(self->selected_->metadata.slot));
  if(n<=0||std::size_t(n)>=sizeof(filename)){e="Source selected profile filename overflow";return false;}
  self->profile_=std::make_shared<dh2::level::CampaignSaveProfileV45>(filename,
    in.private_files->services(),std::move(in.application_jobs));
  if(!self->profile_->construct(e))return false; // Actual job flush/read/backup/cache.
  self->reader_tables_=std::make_shared<ReaderTables>();
  self->reader_tables_->actual_primary=std::move(in.reads.tables);
  self->levels_v67_=in.actual_levels;
  if(in.actual_levels->level_names.size()!=in.actual_levels->levels.size()){
   e="Actual Level default-state names/rows differ";return false;
  }
  self->reader_tables_->level_defaults28.reserve(in.actual_levels->levels.size());
  for(const auto& row:in.actual_levels->levels){std::int32_t value;
   const auto raw=row.scalar.words[0x28/4];std::memcpy(&value,&raw,4);self->reader_tables_->level_defaults28.push_back(value);
  }
  auto maps=std::make_shared<dh2::data::WorldMapProfileTableV59>();
  std::array<std::vector<std::uint8_t>,3> map_bytes;
  const char* map_uris[]{"data/pydata/worldmap_pyarray.bin","data/pydata/worldmap_pyarraynames.bin","data/pydata/worldmap_pystructnames.bin"};
  for(unsigned i=0;i<3;++i){bool found{};if(!in.read_cache(map_uris[i],found,map_bytes[i],e))return false;
   if(!found){e=std::string("Actual campaign WorldMap cache absent: ")+map_uris[i];return false;}
  }
  if(!maps->decode({map_bytes[0].data(),map_bytes[0].size()},{map_bytes[1].data(),map_bytes[1].size()},
     {map_bytes[2].data(),map_bytes[2].size()},e))return false;
  self->map_tables_=std::move(maps);
  self->reader_tables_->maps=self->map_tables_;
  in.reads.level_names=&in.actual_levels->level_names;in.reads.level_defaults28=&self->reader_tables_->level_defaults28;
  in.reads.map_names=&self->map_tables_->names();in.reads.map_defaults8=&self->map_tables_->defaults8();
  if(in.reads.quests){
   if(in.reads.quests->save()!=in.save||!in.reads.quests->tables()||!in.reads.quests->tables()->ready()){
    e="Supplied Quest persistence receiver belongs to another Save/table";return false;
   }
   self->quest_tables_=in.reads.quests->tables();
  }else{
   std::vector<std::uint8_t> array,names;bool found{};
   if(!in.read_cache("data/pydata/v2quests_pyarray.bin",found,array,e))return false;
   if(!found){e="Actual campaign Quest PyArray absent";return false;}
   if(!in.read_cache("data/pydata/v2quests_pyarraynames.bin",found,names,e))return false;
   if(!found){e="Actual campaign Quest names absent";return false;}
   auto quests=std::make_shared<dh2::data::QuestTablesPersistenceV51>();
   if(!quests->decode({array.data(),array.size()},{names.data(),names.size()},e))return false;
   self->quest_tables_=std::move(quests);
   in.reads.quests=std::make_shared<dh2::character::CharacterMenuQuestsV51>(in.save,self->quest_tables_);
  }
  self->reader_tables_->quests=self->quest_tables_;
  in.reads.tables=self->reader_tables_;
  dh2::character::CharacterProfileBootstrapInputsV59 input;
  input.save=std::move(in.save);input.load=std::move(in.load);input.profile=self->profile_;
  input.selected_file_lease=self->selected_;
  input.selected_file_bytes={self->selected_->file.bytes.data(),self->selected_->file.bytes.size()};
  input.selected_slot=self->selected_->metadata.slot;input.character=in.character;
  input.actual_source_cells_lease=std::move(in.source_cells);
  input.source_save14e8=in.save14e8;input.source_player_info_slot664=in.player_info_slot664;
  input.source_direct_save_slot_v122=in.direct_save_slot_v122;
  input.source_set_slot=std::move(in.set_slot);input.reads=std::move(in.reads);input.remaining=std::move(in.remaining);
  input.prior_slot_store=slot_store;
  input.defer_mask2_to_initpost_v62=in.defer_mask2_to_initpost_v62;
  self->source_=std::make_shared<dh2::character::CharacterProfileBootstrapV59>(std::move(input));
  return self->source_->prepare(e);
 }
 bool configure_equipment(dh2::player::PlayerEquipmentRenderInputsV1& in,std::string& e){
  if(!source_){e="Required actual profile preparation before Gear";return false;}
  return source_->configure_equipment(in,e);
 }
 bool finish(dh2::player::PlayerEquipmentRenderOwnerV1& equipment,
  dh2::character::CharacterMenuCampaignSaveServicesV50 services,std::string& e){
  if(!source_){e="Required retained campaign bootstrap before writer registration";return false;}
  if(!reader_tables_||!levels_v67_||!map_tables_){e="Required SAME retained campaign collection reader tables before writer registration";return false;}
  struct WriterPinsV67 {std::shared_ptr<void> caller;std::shared_ptr<ReaderTables> readers;};
  auto pins=std::make_shared<WriterPinsV67>();pins->caller=std::move(services.tables);pins->readers=reader_tables_;services.tables=pins;
  services.level_names=&levels_v67_->level_names;services.map_names=&map_tables_->names();
  return source_->finish(equipment,std::move(services),e);
 }
 const auto& source()const noexcept{return source_;}
 const auto& world_map_tables_v68()const noexcept{return map_tables_;}
};
template<class Runtime> static bool adopt_profile_skill_authority_v59(Runtime& runtime,
 const std::shared_ptr<RendererProfileBootstrapV59>& bootstrap,
 std::shared_ptr<dh2::character::CharacterMenuCampaignSaveV50>& writer,std::string& e){
 if(!bootstrap||!bootstrap->source()||!bootstrap->source()->finished()||!runtime.world||
    !runtime.world->player_object||!runtime.world->player_canonical||
    !runtime.world->player_canonical->loot_fields_v49){
  e="Required completed SAME campaign bootstrap before SkillV6 Save adoption";return false;
 }
 const auto& source=*bootstrap->source();const auto& save=source.save();
 const auto* slot=runtime.world->player_canonical->loot_fields_v49->save_slot14e8();
 if(!save||!slot||*slot!=reinterpret_cast<std::uintptr_t>(save.get())||
    save->character()!=runtime.world->player_object->identity||
    (runtime.save&&runtime.save!=save)||
    (runtime.player_save_load&&runtime.player_save_load.get()!=source.load_owner().get())||
    (runtime.player&&runtime.player->native_savegame()!=save.get())){
  e="Cannot replace Source14e8/SkillV6/SaveLoad with a different campaign Save";return false;
 }
 // Runtime.player_save_load must be shared_ptr, retaining the actual bootstrap
 // owner. A new unique SaveLoad with copied scalar state is not compatible.
 runtime.save=save;runtime.player_save_load=source.load_owner();writer=source.campaign_writer();
 if(!writer||!writer->ready()){e="Required SAME bound campaign menu writer";return false;}
 e.clear();return true;
}
// Root source player construction boundary:
// 1. Execute real Save C1/SetCharacter and Character+14e8 store exactly once.
//    Supply actual PlayerInfo664/SetSlot and SAME SaveLoad owner to create().
// 2. Bootstrap owns real profile+8 and source Load1/Load2, before Gear allocation.
// 3. configure_equipment(input), then actual Gear.initialize. The hook invokes
//    source Load4 in order; one GEAR delivery occurs before source InitialGrant.
// 4. finish(actual Gear, actual writer services). SkillV6 reuses source()->save()
//    and source()->load_owner(); it MUST NOT allocate/reset/initialize them again.
//    Change only Runtime.player_save_load from unique_ptr to shared_ptr, then
//    invoke adopt_profile_skill_authority_v59 on the campaign branch before the
//    old blank Save allocation/initialize_skills/initialize_faeries block. The
//    separate explicitly labelled development launch keeps its old flow.
// 5. CharacterPanel mutation services obtain source()->campaign_writer()->
//    write_services. On context restore reuse all of these owners; no replay.
// Teardown: release panel/SkillV6/reader callbacks/bootstrap before destroying
// Gear/source cells/PM/table leases. Application jobs remain Application-owned.
