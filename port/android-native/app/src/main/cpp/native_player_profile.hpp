#pragma once
#include "player_save_load_owner_v1.hpp"
#include "fresh_inventory_owned_v4.hpp"
#include "item_power_tables_v5.hpp"
#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dh2::data {struct LevelTables;struct WorldMapTables;}
namespace dh2::data {struct CharacterTable;}
namespace dh2::data {struct PropertyRules;struct PropertyState;}
namespace dh2::native::quests {class Owner;}
namespace dh2::native::player_profile {
struct Receipt {
 std::int32_t slot=-1,character_class=-1,level=0,difficulty=0;
 std::uint32_t source_level_id=0,sections=0,field_reads=0,file_opens=0;
 bool loaded=false;
};
// A synchronous provider for one registered campaign section writer. The
// caller retains `owner` for the duration of save_gameplay; the provider must
// serialize from the supplied canonical Save into caller-owned payload bytes.
struct GameplaySectionPayloadProviderV1 {
 std::array<char,4> tag{};
 std::shared_ptr<void> owner;
 void* context=nullptr;
 bool (*write)(void*,const data::PlayerSavegameV1&,
               std::vector<std::uint8_t>&,std::string&){};
};
struct TransportBindings {
 TransportBindings()=default;
 TransportBindings(std::filesystem::path path,const data::CharacterTable* table,
   std::int32_t* difficulty,data::PlayerSaveLoadServicesV1 services={},
   bool create=false,const data::LevelTables* level_tables=nullptr,
   const data::WorldMapTables* map_tables=nullptr,
   std::shared_ptr<dh2::native::quests::Owner> quest_owner={},
   std::uint8_t* online_status=nullptr,std::uint8_t* hosting_quest=nullptr,
   const data::PropertyRules* property_rules=nullptr,data::PropertyState* properties=nullptr)
  :directory(std::move(path)),characters(table),current_difficulty(difficulty),
   continuation(std::move(services)),create_new(create),levels(level_tables),
   world_map(map_tables),quests(std::move(quest_owner)),online(online_status),
    hosting_quest_flag(hosting_quest),property_rules(property_rules),properties(properties){}
 std::filesystem::path directory;
 const data::CharacterTable* characters=nullptr;
 std::int32_t* current_difficulty=nullptr;
 // Gameplay section readers and init/quest/network operations must be actual
 // selected implementations. This service's lease survives each delivery.
 data::PlayerSaveLoadServicesV1 continuation;
 // Explicit source NativeCreateSaveSlot transport only. Missing primaries
 // construct a real empty profile; existing primaries reject. Metadata's
 // read-only import always leaves this false.
 bool create_new=false;
 // Optional pair borrowed from the world's actual retained data owners.
 // Reached InitLevelStates/LVLS use the same gameplay Save's six arrays.
 const data::LevelTables* levels=nullptr;
 const data::WorldMapTables* world_map=nullptr;
 // Optional owner for the actual gameplay Save's quest logs. InitQuests and
 // QEST callbacks borrow this one owner; QEST readers retain the indexed
 // whole-profile stream and do not allocate a second quest VM/store.
 std::shared_ptr<dh2::native::quests::Owner> quests;
 // Fresh reads of the canonical NativeHost session bytes. Hosting and local
 // hosting remain separate source operations and require their own providers.
 const std::uint8_t* online=nullptr;
 const std::uint8_t* hosting_quest_flag=nullptr;
  const data::SkillTables* skill_tables=nullptr;
  const std::int32_t* skill_tree_selector=nullptr;
  // Borrowed sole PlayerCombat rules and state. PROP creates a temporary view
  // at delivery; the transport retains neither a PropertyView nor property data.
  const data::PropertyRules* property_rules=nullptr;
  data::PropertyState* properties=nullptr;
  // The mask-4 GEAR reader mutates this same Character inventory. Effects and
  // the item-power snapshot are borrowed from the retained gameplay owners;
  // the explicit lease keeps the callback context valid for later loads.
  data::FreshInventoryOwnedV4* inventory=nullptr;
  data::ItemPowerTablesV5::Borrow item_powers;
  data::OwnedInventoryServicesV4 inventory_services{};
  std::shared_ptr<void> inventory_services_owner;
  std::unique_ptr<data::ItemInstanceV1>* inventory_incoming=nullptr;
};
// One retained LoadOwner over a caller's existing Save and canonical +8
// profile slot. The transport owns I/O/callback backing only. It creates no
// Save, inventory, properties, VM, RNG or timer owner. Those borrowed objects
// and bound tables/globals must outlive this transport and synchronous calls.
class Transport {
 struct Impl;std::shared_ptr<Impl> impl_;
 std::unique_ptr<data::PlayerSaveLoadOwnerV1> loader_;
public:
 Transport(data::PlayerSavegameV1&,data::PlayerSaveProfileV1&);
 ~Transport();
 Transport(const Transport&)=delete;Transport& operator=(const Transport&)=delete;
 // Rebind table/global/service providers after a world backing replacement.
 // A callback cannot rebind the active transport. A retained profile keeps
 // its original bytes even when the Save slot or directory later changes.
 bool bind(TransportBindings,std::string&);
 // Exact registered metadata saveAll assembly / same-index publication,
 // followed by explicit atomic durable native new-file persistence. Normal
 // import/gameplay transports reject writes until their writer closure binds.
 bool save_all(std::string&);
 // Synchronous adapter for a loaded mask-1 temporary Save: retain the same
 // Save/index, refresh registered metadata sections and preserve the prior
 // primary as .bak before replacing it. Source Savegame's async job queue is
 // not modeled by this adapter.
 bool save_existing_metadata(std::string&);
 // Offline existing-profile save across the complete registered mask-1 +
 // mask-4 writer set. Every callback must belong to this exact Save, and the
 // provider vector must cover those tags exactly. Online synchronization,
 // checkpoint removal and volatile quest saving are explicitly unsupported;
 // this is not enabled until the caller supplies all actual section writers.
 bool save_gameplay(const std::vector<GameplaySectionPayloadProviderV1>&,
                    std::string&);
 // Production assembly from this transport's retained same-Save bindings.
 // Missing writer owners fail before profile publication.
 bool save_gameplay(std::string&);
 // PlayerSavegame::SG_LoadCheckpoint reaches SG_Load(16), which reads QEST
 // from its temporary checkpoint profile while retaining this exact Save,
 // +8 profile owner, transport, and Quest owner. The campaign borrow is
 // restored before return on success or failure.
 bool load_checkpoint_quests(const std::vector<std::uint8_t>&,
                             std::string&);
 data::PlayerSaveLoadOwnerV1& loader() noexcept;
 const Receipt& receipt()const noexcept;
};
// Read-only campaign import transport for PlayerInfo's metadata Save (+680).
// Character's gameplay Save (+14e8), inventory/properties/VM remain distinct.
// No inferred selected slot, Character association, save writer or new profile.
class Metadata {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // Retained view of the exact metadata Save, +8 profile index, and transport.
 // Passing this bundle into the next owner preserves object identity; it does
 // not create or copy a Save. The caller must keep the bundle alive while the
 // transport or any Save callbacks are in use.
 struct SharedSession {
  std::shared_ptr<data::PlayerSavegameV1> save;
  std::shared_ptr<data::PlayerSaveProfileV1> profile;
  std::shared_ptr<Transport> transport;
  SharedSession()=default;
  SharedSession(std::shared_ptr<data::PlayerSavegameV1> s,
      std::shared_ptr<data::PlayerSaveProfileV1> p,
      std::shared_ptr<Transport> t) noexcept
   :save(std::move(s)),profile(std::move(p)),transport(std::move(t)){}
  SharedSession(const SharedSession& other)
   :save(other.save),profile(other.profile),transport(other.transport){}
  SharedSession& operator=(const SharedSession& other){
   if(this!=&other){reset();save=other.save;profile=other.profile;transport=other.transport;}
   return *this;
  }
  SharedSession(SharedSession&& other) noexcept
   :save(std::move(other.save)),profile(std::move(other.profile)),
    transport(std::move(other.transport)){}
  SharedSession& operator=(SharedSession&& other) noexcept {
   if(this!=&other){reset();save=std::move(other.save);
    profile=std::move(other.profile);transport=std::move(other.transport);}
   return *this;
  }
  ~SharedSession(){reset();}
  void reset() noexcept {transport.reset();profile.reset();save.reset();}
  explicit operator bool()const noexcept{return save&&profile&&transport;}
 };
 // A Character session is a different source lifetime from NativeStartGame's
 // temporary metadata Save. Keeping a distinct type prevents callers from
 // accidentally adopting the metadata bundle as gameplay ownership.
 struct CharacterSession {
  std::shared_ptr<data::PlayerSavegameV1> save;
  std::shared_ptr<data::PlayerSaveProfileV1> profile;
  std::shared_ptr<Transport> transport;
  CharacterSession()=default;
  CharacterSession(std::shared_ptr<data::PlayerSavegameV1> s,
      std::shared_ptr<data::PlayerSaveProfileV1> p,
      std::shared_ptr<Transport> t) noexcept
   :save(std::move(s)),profile(std::move(p)),transport(std::move(t)){}
  CharacterSession(const CharacterSession&)=delete;
  CharacterSession& operator=(const CharacterSession&)=delete;
  CharacterSession(CharacterSession&& other) noexcept
   :save(std::move(other.save)),profile(std::move(other.profile)),
    transport(std::move(other.transport)){}
  CharacterSession& operator=(CharacterSession&& other) noexcept {
   if(this!=&other){reset();save=std::move(other.save);
    profile=std::move(other.profile);transport=std::move(other.transport);}
   return *this;
  }
  ~CharacterSession(){reset();}
  void reset() noexcept {transport.reset();profile.reset();save.reset();}
  explicit operator bool()const noexcept{return save&&profile&&transport;}
 };
 Metadata();~Metadata();
 Metadata(const Metadata&)=delete;Metadata& operator=(const Metadata&)=delete;
 bool load(std::int32_t selected_slot,const std::filesystem::path& directory,
           const data::CharacterTable&,std::int32_t& source_current_difficulty,
           std::string&);
 // NativeStartGame's optional first SG_Save, reached when the authored
 // callback receives a numeric difficulty argument.
 bool save_numeric_request(const data::CharacterTable&,
           std::int32_t& source_current_difficulty,std::string&);
 // Source NativeStartGame's second SG_Save: clear selected LUSP in this same
 // temporary Save, update its canonical +8 profile index and persist it.
 bool clear_spawn_point_and_save(std::size_t difficulty,
           const data::CharacterTable&,std::int32_t& source_current_difficulty,
           std::string&);
 const Receipt& receipt()const noexcept;
 const data::PlayerSavegameV1& save()const noexcept;
 SharedSession share_session()const noexcept;
 // Source Character::InitializePlayerSavegame starts a distinct +14e8 Save,
 // assigns the selected slot, and later SG_Load(4)s it from the persisted
 // profile. This creates that independent Save/Profile/Transport; it copies
 // no mutable metadata state and shares no QEST owner.
 bool create_character_session(std::uintptr_t character,CharacterSession&,
                               std::string&)const;
 std::uintptr_t profile_identity()const noexcept;
 std::uintptr_t save_identity()const noexcept;
};
// Source filename/index/reader kernels execute. Platform I/O is a declared
// adapter; a missing/corrupt primary fails explicitly (backup recovery unbound).
// Named writer thunks are retained, but writes explicitly reject until
// source metadata writer/persistence bodies bind. No personal name is logged.
}
