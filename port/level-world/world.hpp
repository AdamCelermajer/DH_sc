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
