// Add PlayerSkillsRuntime members:
// std::unique_ptr<dh2::world::LevelConfigMusicOwnerV1> aggro_config;
// std::uintptr_t aggro_config_identity{};
// dh2::character::PlayerAggroFieldsV1 player_aggro_fields;
// dh2::sound::VoxMusicFieldsV1 vox_music_fields;
// std::unique_ptr<dh2::sound::VoxMusicStateOwnerV1> vox_music;
// std::unique_ptr<dh2::character::CharacterWorldPlayerAggroV2> aggro_events;
// Source projection argument must be extracted from the selected actual XML;
// Crypt's checked source receipt is reference/character-player-aggro-v1/
// crypt01-level-config-music.bin, not a fabricated combat_music_enabled value.
static void bind_player_aggro_v2(PlayerSkillsRuntime& t,dh2::data::Bytes source_config){
 using namespace dh2::character;
 t.aggro_config=dh2::world::LevelConfigMusicOwnerV1::load(source_config,t.error);
 if(!t.aggro_config)throw std::runtime_error(t.error);
 t.aggro_config_identity=t.aggro_config->identity();
 t.vox_music=std::make_unique<dh2::sound::VoxMusicStateOwnerV1>(t.vox_music_fields,
  dh2::sound::VoxMusicServicesV1{});
 // Missing audio is explicit; source current_music24=-1 returns before any
 // audio callback. Never overwrite current_music24 to evade a reached backend.
 WorldPlayerAggroServicesV2 s{};s.context=&t;
 s.events=[](void* p,std::uintptr_t id,AIEventState64** out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);auto a=t.world_actors.find(id);
  if(a==t.world_actors.end()||!out)return -1;
  *out=a->second.npc?&a->second.npc->ai_events:&t.events;return 0;
 };
 s.online=[](void* p,bool* out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);std::uintptr_t identity{};std::int32_t value{};
  if(!out||!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,0,identity,value,t.error))return -1;
  *out=value!=0;return 0;
 };
 s.local_player=[](void* p,std::uintptr_t id,bool* out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);std::uintptr_t value{};
  dh2::character::HitRequest32 q{dh2::character::skills::hit_local_player_v6,0,id,0,nullptr};
  if(!out||!t.player_manager||t.player_manager->hit(q,&value)!=1)return -1;
  *out=value!=0;return 0;
 };
 s.level=[](void* p,PlayerAggroLevelBorrowV1* out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!out||!t.aggro_config)return -1;
  *out={reinterpret_cast<std::uintptr_t>(&level),&t.aggro_config_identity,
   t.aggro_config->combat_music_enabled(),nullptr};return 0;
 };
 s.weight=[](void* p,std::uintptr_t id,std::int32_t* out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);auto a=t.world_actors.find(id);
  if(!out||a==t.world_actors.end()||!a->second.object||!a->second.object->properties)return -1;
  // Character::GetCharAI3a3024 -> GetCharAIId3a2fec reads cached+ffc
  // (resolved[2]), fallback8, stride44. Its row+14 is Flags, NOT embedded
  // runtime CharAI+14. OnAggro adds those signed word bits to AIS weight_d4.
  const auto* ai=t.world->tables.ai();if(!ai)return -1;
  const auto* row=dh2::data::ai_props(*ai,a->second.object->properties->resolved[2]);
  if(!row)return -1;std::memcpy(out,&row->flags,sizeof(*out));return 0;
 };
 s.threshold=[](void* p,std::int32_t* out){
  auto& t=*static_cast<PlayerSkillsRuntime*>(p);const auto* word=t.world->settings.word(0,4);
  if(!out||!word)return -1;std::memcpy(out,word,sizeof(*out));return 0;
 };
 t.aggro_events=std::make_unique<CharacterWorldPlayerAggroV2>(
  WorldPlayerAggroBorrowV2{t.world->player_object->identity,&t.events,&t.player_aggro_fields,t.vox_music.get()},
  s,*t.world->debug,t.world->debug_files);
}
static int player_aggro_callback_v2(void* p,std::uint32_t bit,std::uintptr_t owner,std::uintptr_t target){
 auto& t=*static_cast<PlayerSkillsRuntime*>(p);if(!t.aggro_events)return -1;
 const int status=t.aggro_events->notify(bit,owner,target);
 if(status)t.error=t.aggro_events->error();return status;
}
