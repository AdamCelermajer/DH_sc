#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include "../../platform_key_codes.hpp"
#endif
#include "../../platform_sleep.hpp"
#include <GL/gl.h>
#include "art/original_art.hpp"
#include "preview/class_preview_scene.hpp"
#include "input/screen_interaction.hpp"
#include "creation/dynamic_text_bindings.hpp"
#include "frontend_runtime_v1.hpp"
#include "frontend_cleanup_v1.hpp"
#include "native_host.hpp"
#include "rich_text.hpp"
#include "../../save_store.hpp"
#include "../equipment/source_equipment_appearance.hpp"
#include "../equipment/source_equipment_material_binding.hpp"
#include "../../source_level_config.hpp"
#include "../../../level-world/character_debug_stdio_v136.hpp"
#include "../../../level-world/character_design_services.hpp"
#include "../../platform_win32.hpp"
#include "../../content_paths.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <array>
#include <map>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <set>
#include <sstream>
#include <utility>
namespace f=dh::foundation;namespace a=f::frontend::art;namespace flow=f::frontend::flow;namespace fs=std::filesystem;
static f::Mat4 multiply(const f::Mat4&left,const f::Mat4&right){f::Mat4 result{};for(int c=0;c<4;++c)for(int r=0;r<4;++r)for(int k=0;k<4;++k)result[c*4+r]+=left[k*4+r]*right[c*4+k];return result;}
static void capture(const fs::path&path,int w,int h){std::vector<unsigned char>p(std::size_t(w)*h*3);glPixelStorei(GL_PACK_ALIGNMENT,1);glReadBuffer(GL_BACK);glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,p.data());std::ofstream out(path,std::ios::binary);if(!out)throw std::runtime_error("Cannot open capture");out<<"P6\n"<<w<<' '<<h<<"\n255\n";for(int y=h-1;y>=0;--y)out.write(reinterpret_cast<char*>(p.data()+std::size_t(y)*w*3),w*3);if(!out)throw std::runtime_error("Cannot write capture");}
static bool read_bytes(const fs::path& path,std::vector<char>& bytes){std::ifstream in(path,std::ios::binary);if(!in)return false;bytes.assign(std::istreambuf_iterator<char>(in),{});return in.good()||in.eof();}
namespace {
struct ProfilePreviewDebugFilesV1 {
 std::shared_ptr<f::AssetCatalog> assets;
 static int open(void* raw,const char* name,std::uintptr_t* handle){
  auto& self=*static_cast<ProfilePreviewDebugFilesV1*>(raw);if(!name||!handle||!self.assets)return 1;*handle=0;
  try{const auto path=self.assets->resolve(name);auto* file=std::fopen(path.string().c_str(),"rb");if(!file)return 1;*handle=reinterpret_cast<std::uintptr_t>(file);return 0;}
  catch(const std::invalid_argument&){return 1;}catch(const std::exception&){std::error_code ec;const auto exists=fs::exists(self.assets->root()/name,ec);return ec||exists?1:0;}
 }
 static int close(void*,std::uintptr_t handle){return handle?std::fclose(reinterpret_cast<std::FILE*>(handle)):1;}
};
struct ProfilePreviewDebugV1 {
 std::shared_ptr<ProfilePreviewDebugFilesV1> files;
 std::shared_ptr<dh2::character::DebugSwitches> debug;
 dh2::character::DebugFileServices24 file_api{};
 dh2::character::DebugExistingFileServicesV136 stream_api{};
 f::SourceAppearanceDebugServices services;
 bool bind(const f::AssetCatalog& assets,std::string& error){
  files=std::make_shared<ProfilePreviewDebugFilesV1>();files->assets=std::make_shared<f::AssetCatalog>(assets.root());
  debug=std::shared_ptr<dh2::character::DebugSwitches>(dh2_character_debug_create(),dh2_character_debug_destroy);
  if(!debug){error="Selected-profile source Debug allocation failed";return false;}
  file_api={files.get(),ProfilePreviewDebugFilesV1::open,ProfilePreviewDebugFilesV1::close};
  stream_api=dh2::character::debug_stdio_services_v136(nullptr);
  const auto retained=debug;const auto retained_files=files;const auto file_api_copy=file_api;const auto stream_copy=stream_api;
  services.load=[retained,retained_files,file_api_copy,stream_copy](std::string& e){
   if(!retained||!retained_files){e="Selected-profile source Debug owner/files expired";return false;}
   if(dh2_character_debug_load_stream_v136(retained.get(),&file_api_copy,&stream_copy)!=1){e="Selected-profile actual Debug load failed";return false;}e.clear();return true;
  };
  const auto query=f::source_level_config_debug_switch(debug,files,file_api,stream_api);
  services.query=[query](const char* key,std::string& e){bool value=false;return query(key,value,e);};
  error.clear();return true;
 }
};
struct SavedProfileActorV1 {
 f::CharacterVisual body;
 std::unique_ptr<dh2::skinning::VisualSkinOwnerV6> skin;
 f::SourceEquipmentImageLeaseV1 image;
 std::unique_ptr<f::SourceEquipmentOriginalBindingsV1> material_bindings;
 f::Mat4 scale{};
 std::string class_id,character_id;
 f::SourceEquipmentAppearancePlan appearance;
 std::shared_ptr<const f::SourceEquipmentRenderFrameV1> frame;
};
static dh2::skinning::VisualAssetResultV6 profile_weapon_read(void* raw,const char* uri,std::vector<std::uint8_t>& bytes,std::string& error){
 try{bytes=f::read_content(*static_cast<const f::AssetCatalog*>(raw),uri);return dh2::skinning::VisualAssetResultV6::found;}
 catch(const std::exception& e){error=e.what();return dh2::skinning::VisualAssetResultV6::failed;}
}
static f::Mat4 original_main_profile_placement(const f::Mat4& scale){
 constexpr float angle=3.1415927410125732f*-.125f;const float c=std::cos(angle),s=std::sin(angle);
 f::Mat4 placement{c,s,0,0,-s,c,0,0,0,0,1,0,0,-200,-20,1};
 for(unsigned column=0;column<3;++column)for(unsigned row=0;row<4;++row)placement[column*4+row]*=scale[column*5];
 return placement;
}
}
static a::Screen screen(std::string_view name){if(name=="menu_EnterName")return a::Screen::enter_name;if(name=="menu_SelectClass")return a::Screen::select_class;if(name=="menu_StartGame")return a::Screen::start_game;return a::Screen::main_menu;}
namespace dh::foundation::frontend {
FrontendRuntimeResultV1 run_frontend_v1(
    f::Window& window, f::Renderer& renderer,
    const FrontendRunConfigV1& config,
    FrontendRuntimeServicesV1 runtimeServices) {
 try {
 fs::path root=config.asset_root,capturePath=config.capture_path,
          captureDir=config.capture_directory,uiAssets=config.ui_assets;
 const int frames=config.frames,width=config.width,height=config.height,
           captureEvery=config.capture_every;
 const double step=config.fixed_step_seconds;
 const bool fixedStep=config.fixed_step_enabled;
 const unsigned selected=config.selected_class;
 const auto selectedSlot=config.selected_slot;
 const std::string start=config.initial_menu,script=config.action_script;
 const bool dump=config.dump_layout,verifyNative=config.verify_native_input,verifyGeneric=config.verify_generic_creation,verifyProfiles=config.verify_profile_slots;
 if(verifyGeneric&&(selectedSlot.in_use||!runtimeServices.generic_creation))throw std::runtime_error("Generic creation input proof requires an empty selected slot and real creation services");
 if(verifyProfiles&&(verifyNative||verifyGeneric))throw std::runtime_error("Profile-slot smoke is a distinct end-to-end sequence");
 if(root.empty()||selected>2||frames<0||width<=0||height<=0||captureEvery<1)throw std::runtime_error("Invalid native frontend options");
 if(verifyProfiles&&frames!=0)throw std::runtime_error("Profile-slot smoke owns its complete sequence; omit a frame limit");
 if(fixedStep&&(!std::isfinite(step)||step<=0||step>1))throw std::runtime_error("Invalid diagnostic fixed step");
 if(!captureDir.empty())fs::create_directories(captureDir);
 const int fixedMilliseconds=fixedStep?int(std::lround(step*1000)):0;if(fixedStep&&fixedMilliseconds<1)throw std::runtime_error("Fixed step requires a positive source millisecond");
 if(uiAssets.empty()){
  fs::path executablePath;
#if defined(_WIN32)
  wchar_t path[32768]{};if(GetModuleFileNameW(nullptr,path,32768))executablePath=path;
#else
  std::error_code pathError;executablePath=fs::read_symlink("/proc/self/exe",pathError);if(pathError)executablePath.clear();
#endif
  if(!executablePath.empty()){auto candidate=executablePath.parent_path()/"ui-assets";if(fs::is_directory(candidate))uiAssets=candidate;}
  if(uiAssets.empty())uiAssets=fs::path(__FILE__).parent_path()/"art/assets";
 }
 std::string error;f::AssetCatalog assets(root);auto cache=root/"original-cache";if(!a::load(cache,error))throw std::runtime_error(error);
 f::frontend::FrontendRuntimeV1 runtime;
 const bool genericCreationRoute=bool(runtimeServices.generic_creation);
 auto genericCreation=runtimeServices.generic_creation;
 auto profileProjector=runtimeServices.navigation.project_selected_profile;
 struct ProfileSlotProof {
  int phase{};
  int double_tap_releases{};
  std::string created_class;
  int created_slot{-1};
  fs::path original_path,created_path,recovered_path;
  std::vector<char> original_bytes;
  std::vector<fs::path> prior_archives;
 } slotProof;
 if(verifyProfiles){
  if(start!="main"||selectedSlot.id!=0||selectedSlot.in_use||selectedSlot.save_path.empty()||!selectedSlot.save_path.is_absolute()||captureDir.empty()||!runtimeServices.generic_creation)
   throw std::runtime_error("Profile-slot smoke requires main, absolute empty slot 0, a capture directory and generic source services");
  auto& nav=runtimeServices.navigation;
  if(!nav.inspect_slot||!nav.select_slot||!nav.remove_selected_slot||!nav.project_selected_profile)
   throw std::runtime_error("Profile-slot smoke requires the real inspect/select/project/remove providers");
  flow::SlotFact zero,one;
  if(!nav.inspect_slot(0,zero,error)||!nav.inspect_slot(1,one,error))throw std::runtime_error("Profile-slot smoke could not inspect its seeded slots: "+error);
  if(zero.id!=0||zero.in_use||zero.save_path!=selectedSlot.save_path||one.id!=1||!one.in_use||one.save_path.empty()||!one.save_path.is_absolute()||one.save_path==zero.save_path)
   throw std::runtime_error("Profile-slot smoke requires exact empty slot 0 and occupied slot 1 facts");
  f::CharacterState seeded{};
  if(!f::load_character(one.save_path,seeded,error)||seeded.class_id!="RoguePlayerBase")
   throw std::runtime_error("Profile-slot smoke slot 1 must be a source-created Rogue profile: "+error);
  if(!read_bytes(one.save_path,slotProof.original_bytes)||slotProof.original_bytes.empty())throw std::runtime_error("Profile-slot smoke could not snapshot exact Rogue bytes");
  slotProof.original_path=one.save_path;
  std::error_code listError;
  const auto archivePrefix=one.save_path.filename().string()+".removed-";
  for(const auto& entry:fs::directory_iterator(one.save_path.parent_path(),listError)){
   if(listError)break;
   const auto name=entry.path().filename().string();
   if(name.rfind(archivePrefix,0)==0)slotProof.prior_archives.push_back(entry.path());
  }
  if(listError)throw std::runtime_error("Profile-slot smoke could not snapshot recovery siblings: "+listError.message());
  auto remove=std::move(nav.remove_selected_slot);nav.remove_selected_slot=[&,remove=std::move(remove)](const flow::SlotFact& fact,std::string& e){
   std::vector<char> before;if(fact.id!=1||fact.save_path!=slotProof.original_path||!read_bytes(fact.save_path,before)||before!=slotProof.original_bytes){e="Removal smoke path/bytes differ from the seeded Rogue profile";return false;}
   if(!remove(fact,e))return false;
   std::vector<fs::path> matching;
   std::error_code scanError;
   for(const auto& entry:fs::directory_iterator(fact.save_path.parent_path(),scanError)){
    if(scanError)break;
    const auto candidate=entry.path();const auto name=candidate.filename().string();
    if(name.rfind(fact.save_path.filename().string()+".removed-",0)!=0||std::find(slotProof.prior_archives.begin(),slotProof.prior_archives.end(),candidate)!=slotProof.prior_archives.end())continue;
    std::vector<char> bytes;if(read_bytes(candidate,bytes)&&bytes==slotProof.original_bytes)matching.push_back(candidate);
   }
   if(scanError||matching.size()!=1){e=scanError?"Recovery sibling scan failed: "+scanError.message():"Confirmed removal did not produce one new byte-identical recovery sibling";return false;}
   slotProof.recovered_path=matching.front();
   if(fs::exists(fact.save_path)){e="Confirmed removal left the occupied profile path in place";return false;}
   return true;
  };
 }
 if(!runtime.attach(window,renderer,std::move(runtimeServices),error))throw std::runtime_error(error);
 auto& host=runtime.input_host();
 f::frontend::FrontendText text;
 // The cleanup stack is declared after text so its unwind runs first while the
 // glyph owner is alive; its last registered action clears glyph textures,
 // followed by the base artwork texture actions in reverse acquisition order.
 FrontendCleanupStackV1 cleanup;
 std::map<std::string,std::uint32_t> textures;auto upload=[&](const std::string&uri,const std::string&mask="",bool blue=false){auto key=uri+"|"+mask+(blue?"blue":"red");if(textures.count(key))return textures.at(key);f::TextureImage image;if(!f::load_texture(f::resolve_content_path(assets,uri),image,error))throw std::runtime_error(uri+": "+error);
  if(!mask.empty()){f::TextureImage alpha;if(!f::load_texture(f::resolve_content_path(assets,mask),alpha,error))throw std::runtime_error(error);for(unsigned y=0;y<image.height;++y)for(unsigned x=0;x<image.width;++x)image.rgba[(std::size_t(y)*image.width+x)*4+3]=alpha.rgba[(std::size_t(std::uint64_t(y)*alpha.height/image.height)*alpha.width+std::uint64_t(x)*alpha.width/image.width)*4+(blue?2:0)];}
  auto texture=renderer.createTexture(image.width,image.height,image.rgba.data());if(texture)cleanup.own([&renderer,texture]{renderer.destroyTexture(texture);});textures[key]=texture;return texture;};
 const unsigned char whitePixel[]={255,255,255,255};auto white=renderer.createTexture(1,1,whitePixel);if(white)cleanup.own([&renderer,white]{renderer.destroyTexture(white);});
 auto uiTexture=[&](unsigned bitmap){if(!bitmap)return white;auto file=a::source_texture_file(bitmap);if(!file)throw std::runtime_error("Original SWF bitmap producer unavailable: "+std::to_string(bitmap));auto path=uiAssets/fs::path(file).filename();auto key="source-ui:"+path.string();if(textures.count(key))return textures.at(key);f::TextureImage image;if(!f::load_texture(path,image,error))throw std::runtime_error(error);auto texture=renderer.createTexture(image.width,image.height,image.rgba.data());if(texture)cleanup.own([&renderer,texture]{renderer.destroyTexture(texture);});textures[key]=texture;return texture;};
 auto bindScene=[&](f::OriginalScene&scene){for(std::size_t i=0;i<scene.mesh.ranges.size();++i){const auto&m=scene.materials[i];scene.mesh.ranges[i].material.texture=upload(m.diffuse,m.alphaMap,m.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae");}};
 auto bindVisual=[&](f::CharacterVisual&visual){for(std::size_t i=0;i<visual.mutable_meshes().size();++i){const auto&m=visual.original_materials()[i];auto texture=upload(m.diffuse,m.alphaMap,m.effectFile=="GL_Diffuse_L1_VC_iPhone.bdae");for(auto&range:visual.mutable_meshes()[i].ranges)range.material.texture=texture;}};
 f::OriginalScene mainScene;if(!f::frontend::load_menu_preview_backdrop(assets,mainScene,error))throw std::runtime_error(error);bindScene(mainScene);
 f::frontend::ClassPreviewScene classScene;f::frontend::CreationPreview characters;bool classLoaded=false;
 auto loadClass=[&](){if(classLoaded)return;if(!classScene.load(assets,error)||!characters.load(assets,error))throw std::runtime_error(error);bindScene(classScene.backdrop());for(auto&actor:characters.actors()){bindVisual(actor.body);for(auto&weapon:actor.equipment.mutable_attachments())bindVisual(weapon.visual);}classLoaded=true;};
 std::array<SavedProfileActorV1,3> savedProfileActors;bool savedProfileActorsLoaded=false;
 dh2::data::ItemTable savedProfileItems;bool savedProfileItemsLoaded=false;
 ProfilePreviewDebugV1 profileDebug;
 auto loadSavedProfileActors=[&](){
  if(savedProfileActorsLoaded)return;
  loadClass();
  auto data=read_content(assets,"data/pydata/loot_table_pyarray.bin");
  auto names=read_content(assets,"data/pydata/loot_table_pyarraynames.bin");
  auto fields=read_content(assets,"data/pydata/loot_table_pystructnames.bin");
  if(!dh2::data::load_items({data.data(),data.size()},{names.data(),names.size()},{fields.data(),fields.size()},savedProfileItems,error))throw std::runtime_error("Selected-profile source ItemTable: "+error);
  savedProfileItemsLoaded=true;
  if(!profileDebug.bind(assets,error))throw std::runtime_error(error);
  for(unsigned i=0;i<savedProfileActors.size();++i){
   const auto& source=characters.actors()[i];const auto* config=source.body.configuration();
   if(!config)throw std::runtime_error("Selected-profile source class visual configuration is absent");
   auto& actor=savedProfileActors[i];actor.class_id=source.definition.character;actor.scale=source.scale;
   if(!actor.body.load(assets,*config,error)||!actor.body.select("MenuIdle",true,error))throw std::runtime_error("Selected-profile class body: "+error);
   if(!bind_source_equipment_render_skin(assets,actor.body,{&assets,profile_weapon_read},actor.skin,actor.image,error))throw std::runtime_error("Selected-profile source skin: "+error);
   effects::EffectTextureServices textureServices;
   textureServices.upload=[&renderer](const TextureImage& image,std::uint32_t& texture,std::string& e){
    texture=renderer.createTexture(static_cast<int>(image.width),static_cast<int>(image.height),image.rgba.data());
    if(!texture)e="Renderer rejected original selected-profile texture";return texture!=0;
   };
   textureServices.release=[&renderer](std::uint32_t texture){renderer.destroyTexture(texture);};
   actor.material_bindings=std::make_unique<SourceEquipmentOriginalBindingsV1>(assets,std::move(textureServices),
       [](std::shared_ptr<const SourceEquipmentRenderFrameV1>,std::string& e){e.clear();return true;});
  }
  savedProfileActorsLoaded=true;
 };
 FrontendSelectedProfileSnapshotV1 selectedProfileSnapshot;int selectedProfileActor=-1;int menuSelectedDifficulty=0; /* P14 schema: menu starts on the profile's own CurrentDifficulty (PDFL); arrows not wired yet */
 std::string selectedProfileRenderKey,profileActorLoggedKey;
 auto projectSavedProfileActor=[&](const flow::SlotFact& fact,const std::string& menu){
  if(!fact.in_use||!runtimeServices.borrow_selected_profile_snapshot){selectedProfileSnapshot={};selectedProfileActor=-1;selectedProfileRenderKey.clear();menuSelectedDifficulty=0;return;}
  std::ostringstream renderKey;renderKey<<menu<<'|'<<fact.id<<'|'<<fact.save_path.generic_string();
  const auto key=renderKey.str();
  if(key==selectedProfileRenderKey)return;
  selectedProfileSnapshot={};selectedProfileActor=-1;
  auto snapshot=runtimeServices.borrow_selected_profile_snapshot(fact,error);
  if(!snapshot||!snapshot->valid_for(fact))throw std::runtime_error(error.empty()?"Selected-profile snapshot does not match the exact occupied slot/path":error);
  loadSavedProfileActors();
  const auto& state=*snapshot->character;menuSelectedDifficulty=state.current_difficulty<0?0:state.current_difficulty>2?2:state.current_difficulty;
  for(unsigned i=0;i<savedProfileActors.size();++i)if(savedProfileActors[i].class_id==state.class_id){selectedProfileActor=static_cast<int>(i);break;}
  if(selectedProfileActor<0)throw std::runtime_error("Selected profile class has no authored source preview actor: "+state.class_id);
  auto& actor=savedProfileActors[static_cast<std::size_t>(selectedProfileActor)];
  // The equipment appearance projector resolves by the saved textual slot
  // name. Mirror the production Equipment page's source_slot mapping for this
  // exact immutable Character snapshot instead of passing generic slot names.
  std::vector<std::string> profileSlots;
  profileSlots.resize(9);
  std::array<bool,9> boundSourceSlots{};
  std::set<std::string> boundNames;
  for(const auto& binding:state.equipment){
   if(binding.equipment_set>0)continue;
   int sourceSlot=binding.source_slot;
   if(sourceSlot<0){
    if(binding.slot=="main")sourceSlot=1;
    else if(binding.slot=="off")sourceSlot=2;
    else throw std::runtime_error("Selected-profile saved Gear has an unknown source binding: "+binding.slot);
   }
   if(sourceSlot>=static_cast<int>(profileSlots.size()))throw std::runtime_error("Selected-profile saved Gear source slot is out of range: "+std::to_string(sourceSlot));
   if(binding.slot.empty())throw std::runtime_error("Selected-profile saved Gear has an empty source slot name");
   if(boundSourceSlots[static_cast<std::size_t>(sourceSlot)])throw std::runtime_error("Selected-profile saved Gear has duplicate source slot "+std::to_string(sourceSlot));
   if(!boundNames.insert(binding.slot).second)throw std::runtime_error("Selected-profile saved Gear repeats source slot name "+binding.slot);
   boundSourceSlots[static_cast<std::size_t>(sourceSlot)]=true;
   profileSlots[static_cast<std::size_t>(sourceSlot)]=binding.slot;
  }
  for(unsigned slot=0;slot<profileSlots.size();++slot)if(!boundSourceSlots[slot]){
   auto unused="__profile_unused_source_slot_"+std::to_string(slot)+"__";
   while(boundNames.count(unused))unused+="_";
   boundNames.insert(unused);profileSlots[slot]=std::move(unused);
  }
  SourceEquipmentAppearancePlan plan;
  if(!prepare_source_equipment_appearance(state,savedProfileItems,profileSlots,*actor.skin,plan,error))throw std::runtime_error("Selected-profile saved Gear projection: "+error);
  if(!apply_source_equipment_appearance(*actor.skin,plan,profileDebug.services,error))throw std::runtime_error("Selected-profile source Gear application: "+error);
  actor.character_id=state.id;actor.appearance=std::move(plan);actor.frame.reset();selectedProfileSnapshot=std::move(*snapshot);
  selectedProfileRenderKey=key;
 };
 auto& navigator=runtime.navigator();f::frontend::input::ScreenInteraction interaction(navigator);interaction.selected_slot(selectedSlot);interaction.selected_difficulty(0);
 if(start=="name"){if(!navigator.single_player(selectedSlot,error))throw std::runtime_error(error);}else if(start=="class"){if(!navigator.push("menu_SelectClass",error)||!navigator.select_class(selected,error))throw std::runtime_error(error);}else if(start=="start"){if(!selectedSlot.in_use||selectedSlot.id<0)throw std::runtime_error("Direct StartGame requires an explicit in-use save-slot selection");if(!navigator.single_player(selectedSlot,error))throw std::runtime_error(error);}else if(start!="main")throw std::runtime_error("Unknown initial menu");
 // Explicit test actions invoke the same authored path controller and never
 // substitute success for unavailable native save/game services.
 if(!script.empty()){std::size_t at=0;while(at<=script.size()){auto end=script.find('|',at);if(end==script.npos)end=script.size();auto action=script.substr(at,end-at);if(action.rfind("text:",0)==0){if(!interaction.text(action.substr(5)))throw std::runtime_error("Source name input unavailable");}else if(!interaction.dispatch(action,error))throw std::runtime_error(error);if(end==script.size())break;at=end+1;}}
 a::ScreenArt runtimeArt;f::HudGeometry geometry;cleanup.own([&text,&renderer]{text.clear(renderer);});if(!text.load(cache,error))throw std::runtime_error(error);
 f::OverlayRenderer overlay;std::string presentedKey,surfaceKey;unsigned drawn=0;std::uint64_t sourceClockMilliseconds=0;int lastSelected=-1,smokePhase=0;bool expectedServiceFailure=false,sourceOperationFailed=false;std::string sourceFailure,previousMenu;std::map<std::int64_t,std::string> pressed;
 FrontendElapsedClockV1 elapsedClock;elapsedClock.reset(f::Window::seconds());
 dh::foundation::FramePacer framePacer; /* B066 */
 while(framePacer.begin(),runtime.poll()){
  const int milliseconds=frontend_frame_elapsed_milliseconds_v1(config,elapsedClock,f::Window::seconds());
  sourceClockMilliseconds+=static_cast<std::uint64_t>(milliseconds);
  if(runtime.input_focused()&&navigator.top()=="menu_MainMenu"&&window.key_down(VK_ESCAPE))break;
  if(!runtime.input_focused()){interaction.lose_focus();pressed.clear();}
  for(auto&event:runtime.take_input_events()){
   if(event.kind!=f::frontend::HostEvent::Kind::focus_lost&&!runtime.input_focused())continue;
   switch(event.kind){case f::frontend::HostEvent::Kind::key:interaction.key(event.key,event.down,event.shift);break;case f::frontend::HostEvent::Kind::pointer:{
    if(event.phase==f::frontend::input::PointerPhase::down){std::array<float,2> point;if(a::source_point(event.point.x,event.point.y,window.width(),window.height(),point))for(std::size_t i=runtimeArt.hit_regions.size();i>0;--i){auto path=a::normalize_source_path(runtimeArt.hit_regions[i-1].button_path);if(interaction.enabled(path)&&a::contains(runtimeArt.hit_regions[i-1],point[0],point[1])){pressed[event.pointer]=path;break;}}}
    else if(event.phase==f::frontend::input::PointerPhase::up||event.phase==f::frontend::input::PointerPhase::cancel)pressed.erase(event.pointer);
    interaction.pointer(event.pointer,event.phase,event.point);break;}
   case f::frontend::HostEvent::Kind::text:interaction.text(event.text);break;case f::frontend::HostEvent::Kind::focus_lost:interaction.lose_focus();pressed.clear();break;}}
  interaction.selected_difficulty(menuSelectedDifficulty);if(!interaction.flush(error)){std::cerr<<"Required source frontend operation failed: "<<error<<'\n';sourceOperationFailed=true;sourceFailure=error;if(verifyNative&&smokePhase==5&&error=="NativeCreateSaveSlot owner unavailable")expectedServiceFailure=true;}
  if(navigator.start_delivered())break;
  auto menu=std::string(navigator.top());auto currentClass=interaction.class_index();
  if(menu=="menu_SelectClass"){if(previousMenu!=menu){classLoaded=false;loadClass();frontend_rebase_after_blocking_load_v1(config,elapsedClock,f::Window::seconds());lastSelected=-1;}if(lastSelected!=int(currentClass)){if(!characters.select(currentClass,error))throw std::runtime_error(error);lastSelected=int(currentClass);}if(!classScene.sample(currentClass,milliseconds,error)||!characters.update(double(milliseconds)/1000,error))throw std::runtime_error(error);}
  if(menu!=previousMenu)pressed.clear();auto pressedPath=pressed.empty()?std::string{}:pressed.begin()->second;
 const auto& liveSlot=navigator.selected_slot_fact();
 const auto nextSurface=menu+"|"+std::to_string(currentClass)+"|slot="+std::to_string(liveSlot.id)+":"+std::to_string(liveSlot.in_use)+":"+liveSlot.save_path.string()+":"+std::to_string(navigator.erase_confirmation())+"|"+std::to_string(interaction.uppercase_visible())+"|"+std::to_string(window.width())+"x"+std::to_string(window.height());
  const auto key=nextSurface+"|"+interaction.name()+"|"+pressedPath;
   if(key!=presentedKey){flow::PresentationFacts facts;facts.selected_slot=liveSlot;facts.erase_confirmation=navigator.erase_confirmation();facts.class_index=currentClass;facts.entered_name=interaction.name();facts.upper_keyboard_visible=interaction.uppercase_visible();facts.pressed_button_path=pressedPath;
   if((menu=="menu_MainMenu"||menu=="menu_StartGame")&&liveSlot.in_use&&profileProjector){
     if(!profileProjector(liveSlot,facts.profile_text,error))throw std::runtime_error(error.empty()?"Selected profile metadata projection failed":error);
     flow::rebase_profile_text_paths(facts.profile_text,menu);
    }
    if(menu=="menu_MainMenu"||menu=="menu_StartGame"){
     const bool hadProfileActors=savedProfileActorsLoaded;
     projectSavedProfileActor(liveSlot,menu);
     if(!hadProfileActors&&savedProfileActorsLoaded)
      frontend_rebase_after_blocking_load_v1(config,elapsedClock,f::Window::seconds());
    }
    auto presentation=flow::presentation(menu,facts);
   if(!a::compose(screen(menu),presentation,runtimeArt,error))throw std::runtime_error(error);
   if(menu=="menu_SelectClass"){auto bindings=f::frontend::creation::class_text_bindings(navigator.player_class());if(!bindings.ok())throw std::runtime_error(bindings.error);for(auto&field:runtimeArt.text_fields)for(const auto&binding:bindings.fields)if(a::normalize_source_path(field.path)==a::normalize_source_path(binding.field_path))field.initial_text=binding.html_text;}
   if(dump)for(const auto&field:runtimeArt.text_fields)std::cout<<field.path<<" height="<<field.source_height<<" leading="<<field.leading<<" wrap="<<field.word_wrap<<" multiline="<<field.multiline<<" bounds="<<field.bounds[0]<<','<<field.bounds[1]<<','<<field.bounds[2]<<','<<field.bounds[3]<<" local="<<field.local_bounds[0]<<','<<field.local_bounds[1]<<','<<field.local_bounds[2]<<','<<field.local_bounds[3]<<" text="<<field.initial_text<<'\n';
   if(!a::project(runtimeArt,window.width(),window.height(),geometry,error)||!text.rebuild(runtimeArt,window.width(),window.height(),renderer,error))throw std::runtime_error(error);
   if(surfaceKey!=nextSurface){std::vector<f::frontend::input::PathItem> paths;for(const auto&hit:runtimeArt.hit_regions)paths.push_back({a::normalize_source_path(hit.button_path),true});
    interaction.set_surface(std::move(paths),[&](f::frontend::input::Point point){std::array<float,2> source;if(!a::source_point(point.x,point.y,window.width(),window.height(),source))return f::frontend::input::ItemId{0};for(std::size_t i=runtimeArt.hit_regions.size();i>0;--i)if(interaction.enabled(a::normalize_source_path(runtimeArt.hit_regions[i-1].button_path))&&a::contains(runtimeArt.hit_regions[i-1],source[0],source[1]))return f::frontend::input::ItemId(i);return f::frontend::input::ItemId{0};});surfaceKey=nextSurface;}presentedKey=key;
  }
  interaction.set_animation_input_enabled(menu!="menu_SelectClass"||classScene.input_enabled());
  const bool drawProfileActor=(menu=="menu_MainMenu"||menu=="menu_StartGame")&&
      selectedProfileActor>=0&&selectedProfileSnapshot.valid_for(liveSlot);
  if(drawProfileActor){
   auto& actor=savedProfileActors[static_cast<std::size_t>(selectedProfileActor)];
   if(!actor.body.update(double(milliseconds)/1000.0,error))throw std::runtime_error("Selected-profile source idle update: "+error);
   auto services=actor.material_bindings->callbacks();
   SourceEquipmentRenderBridgeV1 bridge(*actor.skin,actor.image,std::move(services));
   if(!bridge.prepare(actor.frame,error))throw std::runtime_error("Selected-profile source body/Gear render: "+error);
   if(profileActorLoggedKey!=selectedProfileRenderKey){
    std::cout<<"{\"profile_actor\":\"PASS\",\"menu\":\""<<menu
        <<"\",\"slot\":"<<selectedProfileSnapshot.selected_slot.id
        <<",\"save\":\""<<selectedProfileSnapshot.selected_slot.save_path.generic_string()
        <<"\",\"character\":\""<<actor.character_id<<"\",\"class\":\""<<actor.class_id
        <<"\",\"appearance_steps\":"<<actor.appearance.steps.size()
        <<",\"draw_packets\":"<<actor.frame->packets.size()<<"}\n"<<std::flush;
    profileActorLoggedKey=selectedProfileRenderKey;
   }
  }
  renderer.resize(window.width(),window.height());renderer.beginFrame(menu=="menu_SelectClass"?classScene.camera():f::frontend::original_menu_preview_camera());
  if(menu=="menu_MainMenu"||menu=="menu_StartGame")renderer.draw(mainScene.mesh);
  if(drawProfileActor){
   const auto& actor=savedProfileActors[static_cast<std::size_t>(selectedProfileActor)];
   const auto placement=original_main_profile_placement(actor.scale);
   for(const auto& packet:actor.frame->packets)renderer.draw(packet.mesh,multiply(placement,packet.world));
  }
  if(menu=="menu_SelectClass"){renderer.draw(classScene.backdrop().mesh);for(unsigned i=0;i<3;++i){const auto&actor=characters.actors()[i];const auto transform=classScene.anchors()[i];for(const auto&mesh:actor.body.meshes())renderer.draw(mesh,transform);for(const auto&weapon:actor.equipment.attachments())for(const auto&mesh:weapon.visual.meshes())renderer.draw(mesh,multiply(transform,weapon.socket_world));}}
  overlay.begin(window.width(),window.height());for(std::size_t i=0;i<geometry.batches.size();++i){std::vector<f::OverlayTriangleVertex> vertices;for(const auto&v:geometry.batches[i].triangles)vertices.push_back({v.x,v.y,v.u,v.v});auto texture=uiTexture(runtimeArt.bitmap_ids[i]);auto color=runtimeArt.batch_colors[i];if(!overlay.drawTriangles(vertices,texture,color))throw std::runtime_error("Invalid source runtime contour");}text.draw(overlay);overlay.end();renderer.endFrame();++drawn;
  if(verifyNative||verifyGeneric||verifyProfiles){auto click=[&](const std::string&path,int taps=1){for(const auto&hit:runtimeArt.hit_regions)if(a::normalize_source_path(hit.button_path)==path&&hit.triangles.size()>=3){const auto&p=hit.triangles;f::frontend::input::Point point{(p[0].x+p[1].x+p[2].x)/3*window.width()/480.f,(p[0].y+p[1].y+p[2].y)/3*window.height()/320.f};for(int tap=0;tap<taps;++tap)if(!host.post_pointer(point,true)||!host.post_pointer(point,false))throw std::runtime_error("Native smoke mouse enqueue failed");return;}throw std::runtime_error("Native smoke authored hit region missing: "+path);};
   if(verifyProfiles){
    const auto& fact=navigator.selected_slot_fact();
    const auto expect=[&](bool condition,const char* message){if(!condition)throw std::runtime_error(std::string("Profile-slot smoke: ")+message);};
    const auto mark=[&](const char* label){
     if((menu=="menu_MainMenu"||menu=="menu_StartGame")&&fact.in_use){
      expect(drawProfileActor&&selectedProfileSnapshot.valid_for(fact)&&selectedProfileActor>=0,
             "occupied source profile did not resolve its exact same-save actor");
      const auto& actor=savedProfileActors[static_cast<std::size_t>(selectedProfileActor)];
      expect(actor.character_id==selectedProfileSnapshot.character->id&&actor.class_id==selectedProfileSnapshot.character->class_id&&
             actor.frame&&!actor.frame->packets.empty(),"selected saved Character/Gear did not produce retained render packets");
     }
     std::ostringstream filename;filename<<"profile-slot-"<<std::setw(2)<<std::setfill('0')<<slotProof.phase<<'-'<<label<<".ppm";
     capture(captureDir/filename.str(),window.width(),window.height());
     std::cout<<"{\"profile_slot_smoke\":\"capture\",\"phase\":"<<slotProof.phase<<",\"menu\":\""<<menu<<"\",\"slot\":"<<fact.id<<",\"occupied\":"<<(fact.in_use?"true":"false")<<",\"path\":\""<<fact.save_path.generic_string()<<"\",\"capture\":\""<<filename.str()<<"\"}\n"<<std::flush;
    };
    const auto inspect_loaded_class=[&](const fs::path& path,const char* expected){f::CharacterState saved{};std::string loadError;return f::load_character(path,saved,loadError)&&saved.class_id==expected;};
    const auto validate_start_game_strings=[&](){
     const auto value_for=[&](const std::string& wanted)->std::optional<std::string>{
      for(const auto& field:runtimeArt.text_fields)if(a::normalize_source_path(field.path)==wanted)return field.initial_text;
      return std::nullopt;
     };
     for(const char* leaf:{"Hud_Act","Hud_Location","Last_Save","Last_Save_Infos","DifficultyTitle","Difficulty"}){
      const auto value=value_for(std::string("menu_StartGame.PlayerInfos.")+leaf+".text");
      expect(value&&value->empty(),"unknown saved-profile metadata retained an authored Warrior placeholder");
     }
     const std::pair<const char*,const char*> buttons[]={
      {"menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER.text","MENU_SINGLE_PLAYER"},
      {"menu_StartGame.StartMenuButtons.btn_MENU_MULTIPLAYER.text","MENU_MULTIPLAYER"},
      {"menu_StartGame.StartMenuButtons.btn_Achievements.text","MENU_ACHIEVEMENTS"},
      {"menu_StartGame.StartMenuButtons.btn_MENU_Leader.text","MENU_LEADERBOARDS"}};
     for(const auto& button:buttons){
      const auto value=value_for(button.first);
      const auto sourceLabel=a::source_english_symbol(button.second);
      // The base source frame includes this exact third-row text receiver.
      if(std::string_view(button.first).find("btn_Achievements")!=std::string_view::npos)
       expect(value.has_value(),"original third-row btn_Achievements/text receiver is absent from composed StartGame art");
      if(value)expect(*value==sourceLabel,"StartGame button label did not resolve its original localization symbol");
     }
     for(const auto& field:runtimeArt.text_fields){
      const auto path=a::normalize_source_path(field.path);
      if(path.find("menu_StartGame.StartMenuButtons.")==0&&path.size()>=5&&path.compare(path.size()-5,5,".text")==0)
       expect(field.initial_text!="BtnText","a visible source StartGame button retained the authored BtnText placeholder");
     }
    };
    switch(slotProof.phase){
    case 0:
     expect(menu=="menu_MainMenu"&&fact.id==0&&!fact.in_use&&fact.save_path==selectedSlot.save_path,"did not begin on the exact empty slot 0");
     mark("empty-slot-0");click("menu_MainMenu.btnRightArrow");slotProof.phase=1;break;
    case 1:
     expect(menu=="menu_MainMenu"&&fact.id==1&&fact.in_use&&fact.save_path==slotProof.original_path,"source right arrow did not select seeded slot 1");
     expect(inspect_loaded_class(fact.save_path,"RoguePlayerBase"),"slot 1 is no longer the seeded Rogue profile");
     mark("occupied-rogue-slot-1");click("menu_MainMenu.btnDelete");slotProof.phase=2;break;
    case 2:
     expect(menu=="menu_MainMenu"&&navigator.erase_confirmation()&&fact.id==1&&fact.save_path==slotProof.original_path,"Delete did not open confirmation for the exact occupied slot");
     mark("remove-confirmation");click("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_REFUSE");slotProof.phase=3;break;
    case 3:{
     std::vector<char> current;expect(menu=="menu_MainMenu"&&!navigator.erase_confirmation()&&fact.id==1&&fact.in_use&&fact.save_path==slotProof.original_path,"refusal changed selected slot or left confirmation open");
     expect(read_bytes(slotProof.original_path,current)&&current==slotProof.original_bytes,"refusal changed exact saved bytes");
     mark("refused-bytes-preserved");click("menu_MainMenu.btnDelete");slotProof.phase=4;break;
    }
    case 4:
     expect(menu=="menu_MainMenu"&&navigator.erase_confirmation()&&fact.save_path==slotProof.original_path,"second Delete did not confirm the same selected path");
     mark("remove-confirmation-accepted");click("menu_MainMenu.Confirmation.ConfirmationBox.btn_GAMEPLAYMENUS_ACCEPT");slotProof.phase=5;break;
    case 5:{
     std::vector<char> archived;expect(menu=="menu_MainMenu"&&!navigator.erase_confirmation()&&fact.id==1&&!fact.in_use&&fact.save_path==slotProof.original_path,"accepted removal did not refresh the same slot to empty");
     expect(!fs::exists(slotProof.original_path)&&!slotProof.recovered_path.empty()&&read_bytes(slotProof.recovered_path,archived)&&archived==slotProof.original_bytes,"accepted removal did not preserve exact bytes in one recoverable sibling");
     mark("empty-slot-1-after-recoverable-rename");click("menu_MainMenu.btnLeftArrow");slotProof.phase=6;break;
    }
    case 6:
     expect(menu=="menu_MainMenu"&&fact.id==0&&!fact.in_use,"left arrow did not select the known empty source slot 0");
     mark("empty-slot-0-from-slot-1");click("menu_MainMenu.btnRightArrow");slotProof.phase=7;break;
    case 7:
     expect(menu=="menu_MainMenu"&&fact.id==1&&!fact.in_use,"left arrow did not return to exact emptied slot 1");
     click("menu_MainMenu.btn_MENU_SINGLE_PLAYER");slotProof.phase=8;break;
    case 8:
     expect(menu=="menu_EnterName"&&interaction.name().empty(),"empty selected slot did not enter a fresh name field");
     if(!host.post_ascii_text("QA")||!host.post_key(VK_RETURN,true)||!host.post_key(VK_RETURN,false))throw std::runtime_error("Profile-slot smoke name input enqueue failed");
     slotProof.phase=9;break;
    case 9:
     expect(menu=="menu_SelectClass"&&navigator.player_name()=="QA","new profile did not enter class selection with the requested name");
     if(!classScene.input_enabled())break;
     expect(currentClass==0&&navigator.player_class()=="KnightPlayerBase","new profile did not begin on the source Knight model");
     mark("class-knight");click("menu_SelectClass.btn_right");slotProof.phase=10;break;
    case 10:
     expect(menu=="menu_SelectClass","source arrow left the class-selection menu");
     if(!classScene.input_enabled())break;
     expect(currentClass==1&&navigator.player_class()=="RoguePlayerBase","source arrow did not select the Rogue class model");
     mark("class-rogue");click("menu_SelectClass.btn_left");slotProof.phase=11;break;
    case 11:
     expect(menu=="menu_SelectClass","return arrow left the class-selection menu");
     if(!classScene.input_enabled())break;
     expect(currentClass==0&&navigator.player_class()=="KnightPlayerBase","return arrow did not restore the Warrior/Knight model");
     mark("class-knight-before-double-confirm");
     // Queue two real down/up cycles before one controller flush. The source
     // transition must consume at most one create; neither release is hidden.
     click("menu_SelectClass.btn_Confirm",2);slotProof.double_tap_releases=2;slotProof.phase=12;break;
    case 12:{
     expect(menu=="menu_StartGame"&&fact.id==1&&fact.in_use,"double Confirm did not settle on the created profile page");
     const auto& creationResult=genericCreation->creation_result();
     expect(slotProof.double_tap_releases==2&&creationResult&&creationResult->status==f::frontend::creation::RuntimeCreationStatusV1::prepared_for_start&&creationResult->saved&&creationResult->reloaded&&creationResult->published_to_shared_state&&creationResult->same_state_owner(genericCreation->shared_state()),"double Confirm did not produce exactly one successful source save/reload into the same state");
     expect(!fact.save_path.empty()&&inspect_loaded_class(fact.save_path,"KnightPlayerBase"),"new profile does not contain the selected Warrior/Knight source class");
     slotProof.created_slot=fact.id;slotProof.created_class="KnightPlayerBase";
     validate_start_game_strings();slotProof.created_path=fact.save_path;mark("created-warrior-profile");click("menu_StartGame.btn_back");slotProof.phase=13;break;
    }
    case 13:
     expect(menu=="menu_MainMenu"&&fact.id==1&&fact.in_use&&fact.save_path==slotProof.created_path,"source Back did not restore the new selected profile");
     click("menu_MainMenu.btnLeftArrow");slotProof.phase=14;break;
    case 14:
     expect(menu=="menu_MainMenu"&&fact.id==0&&!fact.in_use,"post-create left arrow did not select empty slot 0");
     mark("post-create-empty-slot-0");click("menu_MainMenu.btnRightArrow");slotProof.phase=15;break;
    case 15:
     expect(menu=="menu_MainMenu"&&fact.id==1&&fact.in_use&&fact.save_path==slotProof.created_path,"post-create left arrow did not restore the exact new profile");
     click("menu_MainMenu.btn_MENU_SINGLE_PLAYER");slotProof.phase=16;break;
    case 16:
     expect(menu=="menu_StartGame"&&fact.id==1&&fact.save_path==slotProof.created_path,"selected profile did not open its exact StartGame page");
     validate_start_game_strings();mark("selected-profile-before-load");click("menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER");slotProof.phase=17;break;
    default:break;
    }
   }else if(smokePhase==0&&menu=="menu_MainMenu"){click("menu_MainMenu.btn_MENU_SINGLE_PLAYER");smokePhase=1;}
   else if(smokePhase==1&&menu=="menu_EnterName"){if(!host.post_ascii_text("QA")||!host.post_key(VK_RETURN,true)||!host.post_key(VK_RETURN,false))throw std::runtime_error("Native smoke keyboard enqueue failed");smokePhase=2;}
   else if(smokePhase==2&&menu=="menu_SelectClass"&&classScene.input_enabled()){
    if(navigator.player_name()!="QA")throw std::runtime_error("Native smoke did not retain source name");
    if(verifyGeneric){
     if(currentClass!=selected){click(currentClass<selected?"menu_SelectClass.btn_right":"menu_SelectClass.btn_left");}
     else {click("menu_SelectClass.btn_Confirm");smokePhase=3;}
    }else{if(currentClass!=0)throw std::runtime_error("Native smoke did not retain default Knight class");click("menu_SelectClass.btn_right");smokePhase=3;}
   }
   else if(verifyGeneric&&smokePhase==3&&menu=="menu_StartGame"){click("menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER");smokePhase=4;}
   else if(smokePhase==3&&currentClass==1&&classScene.input_enabled()){click("menu_SelectClass.btn_right");smokePhase=4;}
   else if(smokePhase==4&&currentClass==2&&classScene.input_enabled()){click("menu_SelectClass.btn_Confirm");smokePhase=5;}
   else if(smokePhase==5&&expectedServiceFailure){std::cout<<"{\"native_event_smoke\":\"PASS\",\"mouse_release\":true,\"wm_char_and_return\":true,\"all_three_classes\":true,\"missing_service_explicit\":true,\"profile_writes\":false}\n";runtime.present();break;}
  }
  if(!captureDir.empty()&&(drawn==1||drawn%unsigned(captureEvery)==0)){std::ostringstream name;name<<"frame-"<<std::setw(6)<<std::setfill('0')<<drawn<<"-"<<sourceClockMilliseconds<<"ms.ppm";capture(captureDir/name.str(),window.width(),window.height());}
  if(frames&&drawn>=unsigned(frames)&&!capturePath.empty())capture(capturePath,window.width(),window.height());runtime.present();previousMenu=menu;if(frames&&drawn>=unsigned(frames))break;framePacer.wait(); /* B066 */
 }
 if(verifyNative&&!expectedServiceFailure)throw std::runtime_error("Native input smoke did not reach its required service boundary: phase="+std::to_string(smokePhase)+" menu="+std::string(navigator.top())+" class="+std::to_string(interaction.class_index()));
 if(verifyGeneric&&!navigator.start_delivered())throw std::runtime_error("Real generic creation input did not deliver StartGame: phase="+std::to_string(smokePhase)+" menu="+std::string(navigator.top()));
 if(verifyProfiles&&(slotProof.phase!=17||!navigator.start_delivered()||sourceOperationFailed))throw std::runtime_error("Profile-slot smoke did not complete its source Load/Start sequence: phase="+std::to_string(slotProof.phase)+" menu="+std::string(navigator.top())+(sourceFailure.empty()?std::string{}:" error="+sourceFailure));
 const auto outcome=navigator.start_delivered()
      ?(genericCreationRoute?FrontendRuntimeOutcomeV1::generic_gameplay_started
                            :FrontendRuntimeOutcomeV1::gameplay_handoff_ready)
      :(sourceOperationFailed?FrontendRuntimeOutcomeV1::source_operation_failed
                             :FrontendRuntimeOutcomeV1::quit);
 auto result=runtime.result(outcome,sourceFailure);
 if(verifyGeneric&&!result.generic_gameplay_ready())throw std::runtime_error("Real input start did not publish the same-state generic receipt");
 if(verifyProfiles){
  std::optional<f::frontend::creation::FrontendGenericStartReceiptV1> receipt;
  if(genericCreation)receipt=genericCreation->start_receipt();
  if(!result.generic_gameplay_ready()||!receipt||!receipt->valid_for(genericCreation->shared_state(),1)||receipt->created_in_this_flow||receipt->save_path!=slotProof.created_path||slotProof.created_slot!=1||slotProof.created_class!="KnightPlayerBase"||slotProof.double_tap_releases!=2)
   throw std::runtime_error("Profile-slot smoke did not preserve the exact selected-file Load and same-state Start receipt");
  std::cout<<"{\"profile_slot_smoke\":\"PASS\",\"class_previews\":[\"KnightPlayerBase\",\"RoguePlayerBase\",\"KnightPlayerBase\"],\"double_confirm_releases\":"<<slotProof.double_tap_releases<<",\"created_slot\":"<<slotProof.created_slot<<",\"loaded_path\":\""<<receipt->save_path.generic_string()<<"\",\"recovered_path\":\""<<slotProof.recovered_path.generic_string()<<"\",\"generic_start_delivered\":true}\n";
 }
 if(verifyGeneric)std::cout<<"{\"generic_native_input\":\"PASS\",\"mouse_release\":true,\"wm_char_and_return\":true,\"source_confirm\":true,\"source_start_game\":true,\"profile_saved_reloaded\":true}\n";
 std::cout<<"{\"frames\":"<<drawn<<",\"menu\":\""<<navigator.top()<<"\",\"source_clock_ms\":"<<sourceClockMilliseconds<<",\"fixed_step\":"<<(fixedStep?"true":"false")<<",\"source_clip\":\""<<(classLoaded?classScene.selected_clip():std::string{})<<"\",\"source_sample_ms\":"<<(classLoaded?classScene.sampled_milliseconds():0)<<",\"body_clip\":\""<<(classLoaded?characters.actors()[interaction.class_index()].body.animation_name():"")<<"\",\"body_clock_ms\":"<<(classLoaded?std::lround(characters.actors()[interaction.class_index()].body.animation_elapsed_seconds()*1000):0)<<",\"idle_owner_required\":"<<(classLoaded&&characters.idle_transition_required()?"true":"false")<<",\"interactive\":true,\"saved_user_profiles\":false,\"generic_start_delivered\":"<<(result.generic_gameplay_ready()?"true":"false")<<",\"native_profile_loan\":"<<(result.gameplay_ready()?"true":"false")<<"}\n";
 return result;
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';FrontendRuntimeResultV1 failed;failed.outcome=FrontendRuntimeOutcomeV1::host_failed;failed.error=e.what();return failed;}
}
} // namespace dh::foundation::frontend
