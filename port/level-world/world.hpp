#pragma once
#include "../scene-materials/scene.hpp"
#include "source_handle_ledger_v1.hpp"
#include "floors.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::data { struct CharacterTable; struct Dictionary; }
namespace dh2::world {
using Point=std::array<float,3>;
struct Triangle{Point a,b,c;unsigned room;};
struct Level{scene::Scene scene;std::vector<Triangle> floor;Point spawn{};unsigned rooms=0;std::unique_ptr<floors::World> native_floor;};
struct Transform{Point position{};Point rotation_degrees{};Point scale{1,1,1};};
struct EntryPoint{std::int32_t id=-1;unsigned room=0;std::string name;Transform local;Transform world;};
struct SourceMgpView{const char* source_path=nullptr;const std::uint8_t* data=nullptr;std::size_t size=0;};
struct SourceMvpView{const char* source_path=nullptr;const std::uint8_t* data=nullptr;std::size_t size=0;};
struct SourceMvpDecor{
 std::uint32_t module_index=0;
 std::uint32_t source_record=0;
 std::string name;
 std::string xrefobject;
 std::string dae_path;
 std::string source_path;
 Transform local;
 Transform world;
};
struct StaticDactActorHandleV1 {
 std::uint32_t dact_record=0;
 std::uint32_t module_index=0;
 std::uint32_t source_record=0;
 std::int32_t source_handle=-1;
 std::string name;
 std::string character;
};
// One source MGP Character object from the bounded Level census. Source handle
// is joined through the same ordered ObjectManager ledger; defer bits preserve
// every currently unsupported authored activation route without executing it.
struct SourceCharacterCensusV1 {
 std::uint32_t module_index=0;
 std::uint32_t source_record=0;
 std::int32_t source_handle=-1;
 bool duplicate_name=false;
 // MGP runtime template fields (kept distinct from editor _templateName and
 // CharacterTable charpropsname above).
 std::string char_template_pydata,char_template;
 std::string name,character,template_name,activate_condition,
             deactivate_condition,script,spawn_probability,auto_spawn,ai_state,
             char_group,char_group_role,spawn_view_radius;
 bool is_global=false;
 std::uint32_t deferred_reason_mask=0;
 bool supported_direct_dact_v1=false;
 Transform local,world;
};
// Generic exact MGP GameObject projection joined to the same canonical
// ObjectManager occurrence ledger. It carries authored conditions, spawn
// probability, class-table keys, and transforms; callers own type-specific
// activation and lifecycle.
struct SourceObjectCensusV1 {
 std::uint32_t module_index=0;
 // Runtime ObjectBase+0x64 module owner for this source MGP row. Module
 // constructors receive their per-Level ID in Module::s_moduleId order,
 // which is the same ordered module slot retained by module_index here.
 std::int32_t source_module_id=-1;
 std::uint32_t source_record=0;
 std::int32_t source_handle=-1;
 bool duplicate_name=false;
 std::string name,gametype,template_name,activate_condition,deactivate_condition,
             spawn_probability,min_difficulty,is_demo_only,dimensions,data_desc,data_desc_pydata,target_node,
             exit_level_id,exit_level_name,exit_entrypoint_id,exit_fasttravel,
             visible,skip_checkpoint,door_data,door_opened,door_collision;
 Transform local,world;
};
struct SpawnSelection{EntryPoint source;Point position{};Point rotation_degrees{};bool floor_snapped=false;};
// Generated worlds with an authoritative SPWN sidecar may defer descriptor
// spawn-floor validation until the selected source entrypoint is loaded.
bool load(const resources::BresView&,const std::uint8_t* descriptor,std::size_t,
          Level&,std::string&,bool validate_descriptor_spawn=true);
// Build the bounded static Module layout from the original Level .mlx and
// selected SpawnPoint sidecar. This replaces the generated DWLD module table
// at runtime; conditional/rotated/scaled modules remain unsupported.
bool compile_source_layout(const std::uint8_t* mlx,std::size_t mlx_size,
                           const std::uint8_t* spawnpoints,std::size_t spawn_size,
                           std::int32_t entrypoint_id,
                           std::vector<std::uint8_t>& descriptor,std::string& error);
// Import the bounded SWAMP MGP set and serialize supported source SpawnPoints
// into the existing SPWN v1 entrypoint owner (IDs 0, 3 and 13).
bool compile_source_spawnpoints(const std::uint8_t* mlx,std::size_t mlx_size,
                                const SourceMgpView* mgps,std::size_t mgp_count,
                                std::vector<std::uint8_t>& spawnpoints,std::string& error);
// Compile only the five reviewed, unconditional direct SWAMP Monster records
// into the current DACT v1 owner. Character model and authored scale values
// come from the supplied source CharacterTable and ModelFile dictionary.
bool compile_source_dact(const std::uint8_t* mlx,std::size_t mlx_size,
                         const SourceMgpView* mgps,std::size_t mgp_count,
                         const data::CharacterTable& characters,
                         const data::Dictionary& models,
                         std::vector<std::uint8_t>& dact,std::string& error);
// Same byte-exact static DACT compile, plus source provenance resolved through
// the exact MGP (module_index, source_record) tuple and canonical ObjectManager
// handle ledger. The DACT row ordinal is retained; names are validation facts,
// never the key used to select a source handle.
bool compile_source_dact_with_handles(const std::uint8_t* mlx,std::size_t mlx_size,
                         const SourceMgpView* mgps,std::size_t mgp_count,
                         const data::CharacterTable& characters,
                         const data::Dictionary& models,
                         const source_handle_ledger_v1::Ledger& handle_ledger,
                         std::vector<StaticDactActorHandleV1>& actor_sources,
                         std::vector<std::uint8_t>& dact,std::string& error);
// Enumerate every source MGP Character and join its exact MGP occurrence to
// the caller's already-built canonical ObjectManager handle ledger. Duplicate
// names remain census rows with the ledger's shared key and duplicate flag;
// no partial census is published on malformed or unmapped provenance.
bool census_source_characters_with_handles(const std::uint8_t* mlx,
                         std::size_t mlx_size,const SourceMgpView* mgps,
                         std::size_t mgp_count,
                         const source_handle_ledger_v1::Ledger& handle_ledger,
                         std::vector<SourceCharacterCensusV1>& output,
                         std::string& error);
// Same exact source census, generalized for other retained Level MLX/MGP
// owners (including generated Crypt). The overload above remains the
// byte/source-path contract for authored SWAMP callers.
bool census_source_characters_with_handles_for_level(const std::uint8_t* mlx,
                         std::size_t mlx_size,const char* level_name,
                         const char* source_path,const SourceMgpView* mgps,
                         std::size_t mgp_count,
                         const source_handle_ledger_v1::Ledger& handle_ledger,
                         std::vector<SourceCharacterCensusV1>& output,
                         std::string& error);
// Generic MGP object census joined by exact (module_index, source_record).
// Empty gametype_filter includes all records; nonempty filters exact gametype.
bool census_source_objects_with_handles_for_level(const std::uint8_t* mlx,
                         std::size_t mlx_size,const char* level_name,
                         const char* source_path,const SourceMgpView* mgps,
                         std::size_t mgp_count,const char* gametype_filter,
                         const source_handle_ledger_v1::Ledger& handle_ledger,
                         std::vector<SourceObjectCensusV1>& output,
                         std::string& error);
// Import the nine source MVPs and project only the ten reviewed unconditional
// SWAMP static Decor records into owned render-instance descriptions.
bool compile_source_mvp(const std::uint8_t* mlx,std::size_t mlx_size,
                        const SourceMvpView* mvps,std::size_t mvp_count,
                        std::vector<SourceMvpDecor>& out,std::string& error);
// SPWN v1 carries Crypt's two active entrypoints and SWAMP's three supported
// unconditional MGP entrypoints. Conditional SWAMP transition groups remain
// outside this format.
// Selection mirrors the ID lookup and floor snap in Level::_LoadPlayer, but
// live-object activation and manager iteration/order semantics are not yet
// generalized. Position Z uses the loaded-world floor query used by world::load.
bool load_entrypoints(const std::uint8_t* data,std::size_t size,unsigned room_count,
                      std::vector<EntryPoint>& out,std::string& error);
bool select_entrypoint(const Level&,const std::vector<EntryPoint>&,std::int32_t id,
                       SpawnSelection& out,std::string& error);
// Authored levels use the source-built floor selector/collision kernel. The
// radius/step/substep movement policy remains a new adapter; PF route search,
// obstacles and gameplay movement scheduling need further reconstruction.
bool height(const Level&,const Point&,float& result);
bool supported(const Level&,const Point&,float radius,float& floor_height);
bool move(const Level&,Point&,float dx,float dy,float radius);
}
