"""Extract actual loading owners into a separate TU; no compiler/process launch."""
from pathlib import Path

ROOT=Path(__file__).resolve().parents[3]
CPP=ROOT/'port/android-native/app/src/main/cpp'
BASE=ROOT/'port/level-loader/reports/campaign-runtime-v61/baseline'

candidate=(BASE/'renderer_source_candidate_v55.inc').read_text()
candidate=candidate[candidate.index('struct RendererSourceFactoryCoreV55'):]
candidate=candidate.rsplit('\n}',1)[0]
candidate=candidate.replace('RendererCanonicalWorldV4','SourceCanonicalBorrowV61')
candidate=candidate.replace('WorldScriptContext','SourceWorldBorrowV61')
candidate=candidate.replace('RendererLevelApplicationV25','SourceWorldBorrowV61')
candidate=candidate.replace('std::shared_ptr<CanonicalObjectManagerV1>(candidate->canonical,&candidate->canonical->manager)',
                            'candidate->canonical->manager_lease')
candidate=candidate.replace('manager=std::shared_ptr<CanonicalObjectManagerV1>(candidate->canonical,&candidate->canonical->manager)',
                            'manager=candidate->canonical->manager_lease')
candidate=candidate.replace('in.world->host_level.row_index','in.world->host_level->row_index')
candidate=candidate.replace('in.world->host_level.difficulty','in.world->host_level->difficulty')
candidate=candidate.replace('in.application->source->module_globals','in.application->level_application->module_globals')
candidate=candidate.replace('construction.candidate=candidate->canonical','construction.candidate=candidate->canonical->owner')
candidate=candidate.replace('graph_services.candidate=candidate->canonical','graph_services.candidate=candidate->canonical->owner')
candidate=candidate.replace('in.application->files->cache.borrow_archive_v55(archive,error)','in.application->archive(archive,error)')
candidate=candidate.replace('archive,in.level,candidate->canonical,&candidate->canonical->manager',
                            'archive,in.level,candidate->canonical->owner,&candidate->canonical->manager')
candidate=candidate.replace('out.manager={canonical,&canonical->manager};','out.manager=canonical->manager_lease;')

script=(BASE/'renderer_source_script_manager_v55.inc').read_text()
script=script[script.index('using SourceScriptFactoryV55'):].rsplit('\n}',1)[0]
script=script.replace('RendererLevelApplicationV25','SourceWorldBorrowV61')
script=script.replace('!application->files','!application->files_owner')
script=script.replace('services.owner=application->files;','services.owner=application->files_owner;')
script=script.replace('[files=application->files]','[read=application->read]')
script=script.replace('files->cache.read(name,found,*bytes,e)','read(name,found,*bytes,e)')

campaign=(CPP/'renderer_source_campaign_v55.inc').read_text()
campaign=campaign[campaign.index('struct RendererSourceCampaignV55'):]
# Replace the App camera/file lifetime transport, now supplied by the actual
# renderer boundary, rather than carrying a private renderer type into this TU.
a=campaign.index('struct SourceApplicationDebugTransportV55')
b=campaign.index('bool source_debug_load_v55',a)
campaign=campaign[:a]+campaign[b:]
campaign=campaign.replace('WorldScriptContext','SourceWorldBorrowV61')
campaign=campaign.replace('RendererLevelApplicationV25','SourceWorldBorrowV61')
campaign=campaign.replace('&world->debug_files','world->debug_files')
campaign=campaign.replace('state.application->application','state.world->application')
campaign=campaign.replace('[files=state.application->files]','[read=state.world->read]')
campaign=campaign.replace('files->cache.read(name,found,*result,e)','read(name,found,*result,e)')
campaign=campaign.replace('std::shared_ptr<physical::NativeWorld>(application,&actor_world)','state.world->physical_world')
campaign=campaign.replace('borrow_native_gs_frame_menu_v58(current_world,','borrow_native_gs_frame_menu_v58(current_world->owner,')
campaign=campaign.replace('unload_native_menu_v58(world,id,e)','unload_native_menu_v58(world->owner,id,e)')
# Anonymous fragment closure preceded root exported start definitions.
campaign=campaign.replace('\n}\n\nbool start_source_campaign_impl_v55','\n\nbool start_source_campaign_runtime_v61',1)
campaign=campaign.replace('profile,std::string& error)try{','profile,SourceCampaignRendererServicesV61 renderer,std::string& error)try{',1)
a=campaign.index(' if(renderer_gs_globals_v27->s_level)')
b=campaign.index(' auto held_design=',a)
campaign=campaign[:a]+''' if(!renderer.globals||!renderer.create_world){error="Required actual renderer source boundary";return false;}
 if(renderer.globals->s_level){error="Original GS current Level exists; require actual unload";return false;}
 auto state=std::make_unique<RendererSourceCampaignV55>();state->profile=std::move(profile);
 source_campaign_v55=std::move(state);auto* retained=source_campaign_v55.get();
 if(!renderer.create_world(assets,retained->profile,retained->world,error)||!retained->world){
  retained->failed=true;retained->failure=error;return false;
 }
 auto* state=retained;
 const auto application=state->world->application;
 state->application=state->world;state->camera_application=state->world->camera_application;
 state->player_manager=state->world->player_manager;state->globals=renderer.globals;
 const auto difficulty=std::size_t(state->profile->metadata.selected_difficulty);
 auto tables=state->world->design->borrow();const auto* levels=tables.levels();
 const auto row=state->profile->metadata.location.levels[difficulty];
 if(!levels||row<0||std::size_t(row)>=levels->levels.size()){
  error="Required authentic selected LNAM/default row";state->failed=true;state->failure=error;return false;
 }
''' +campaign[b:]
campaign=campaign.replace('auto state=std::make_unique<RendererSourceCampaignV55>();state->profile=std::move(profile);',
                          'auto owned=std::make_unique<RendererSourceCampaignV55>();owned->profile=std::move(profile);',1)
campaign=campaign.replace('source_campaign_v55=std::move(state);auto* retained','source_campaign_v55=std::move(owned);auto* retained',1)
campaign=campaign.replace('app.owner=state->application;','app.owner=state->world->files_owner;')
campaign=campaign.replace('state->application->source->bind','state->world->level_application->bind')
campaign=campaign.replace('input.globals=renderer_gs_globals_v27','input.globals=state->globals')
campaign=campaign.replace('auto* stable=state.get();','auto* stable=state;')
campaign=campaign.replace('borrow_native_gs_menu_v27(world,','borrow_native_gs_menu_v27(world->owner,')
campaign=campaign.replace('bind_native_menu_application_ec_v58(world,','bind_native_menu_application_ec_v58(world->owner,')
campaign=campaign.replace(' source_campaign_v55=std::move(state); // Retain failed source prefixes BEFORE begin.',' // State was retained before the actual renderer startup prefix.')
campaign=campaign.replace('world.native_gslevel_v27,world.native_loading_v50','*world.native_gslevel_v27,*world.native_loading_v50')
campaign=campaign.replace('world.native_level_c1_v25=world.native_gslevel_v27->connection()->level_connection();',
                          '*world.native_level_c1_v25=(*world.native_gslevel_v27)->connection()->level_connection();')
campaign=campaign.replace(' last_frame=std::chrono::steady_clock::now();',' *world.last_frame=std::chrono::steady_clock::now();')
campaign=campaign.replace('renderer_gs_globals_v27->s_level','state.globals->s_level')
campaign=campaign.replace('out={state.world,state.application->application,','out={state.world->owner,state.world->application,')
campaign=campaign.replace('out={state.world,state.world->application,','out={state.world->owner,state.world->application,')
campaign=campaign.replace('std::shared_ptr<dh2::world::CanonicalObjectManagerV1>(candidate->canonical,&candidate->canonical->manager)',
                          'candidate->canonical->manager_lease')
campaign=campaign.replace('source_campaign_active_impl_v55','source_campaign_runtime_active_v61')
campaign=campaign.replace('borrow_source_campaign_candidate_impl_v55','borrow_source_campaign_candidate_runtime_v61')
campaign=campaign.replace('bind_source_campaign_class_factory_impl_v60','bind_source_campaign_class_factory_runtime_v61')
campaign=campaign.replace('tick_source_campaign_impl_v55','tick_source_campaign_runtime_v61')
campaign=campaign.replace('state.world->native_loading_v50->','(*state.world->native_loading_v50)->')
campaign=campaign.replace('!state.world->native_loading_v50','(!state.world->native_loading_v50||!*state.world->native_loading_v50)')
campaign=campaign.replace('last_frame.time_since_epoch()','state.world->last_frame->time_since_epoch()')
campaign=campaign.replace('dt=now_ms-old_ms;last_frame=now','dt=now_ms-old_ms;*state.world->last_frame=now')
campaign=campaign.replace('current_application_dt=dt;current_application_tick=dt<=2000','*state.world->application_dt=dt;*state.world->application_tick=dt<=2000')
campaign=campaign.replace('if(!current_application_tick)','if(!*state.world->application_tick)')
campaign=campaign.replace('dh2::loader::NativeRootGSBootstrapV55 bootstrap;',
 'dh2::loader::NativeRootGSBootstrapV55 bootstrap;\n std::shared_ptr<dh2::loader::NativeGSLevelGlobalsV27> globals;')
headers='''#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include "../level-loader/native_root_gs_bootstrap_v55.hpp"
#include "../level-loader/stage_loader_v52_script_manager.hpp"
#include "canonical_module_graph_v3.hpp"
#include "scene_manager_map_owner_v2.hpp"
#include "character_game_design.hpp"
#include "character_design_services.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "application_player_manager_bootstrap_v59.hpp"
#include <android/log.h>
#include <cstring>
#include <exception>
namespace model_renderer {
namespace {
'''
# Helper definitions are local; the runtime exports below live outside the
# anonymous namespace and cannot shadow the header's model_renderer APIs.
split=campaign.index('bool start_source_campaign_runtime_v61')
output=headers+candidate+'\n'+script+'\n'+campaign[:split]+'\n}\n'+campaign[split:]+'\n}\n'
(CPP/'source_campaign_runtime_v61.cpp').write_text(output)
print('Wrote standalone actual runtime; shared renderer edits are separate.')
