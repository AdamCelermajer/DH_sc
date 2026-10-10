#include "source_campaign_frame_environment_v76.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "model_renderer.hpp"
#include <campaign_frame_scratch_v76.hpp>
#include <campaign_navigation_registry_v64.hpp>
#include <floors.hpp>
#include <exception>
namespace model_renderer {namespace {
struct FrameEnvironmentLeaseV76 {
 std::shared_ptr<SourceWorldBorrowV61> world;
 std::shared_ptr<dh2::floors::World> floors;
 std::shared_ptr<dh2::navigation::CampaignNavigationRegistryV64> navigation;
 std::shared_ptr<dh2::navigation::CampaignFrameScratchV76> scratch;
 std::shared_ptr<void> workspace;
};
}
bool borrow_source_campaign_frame_environment_v76(const SourceCampaignCandidateBorrowV55& expected,
 std::uint32_t paths,std::uint32_t actors,SourceCampaignFrameEnvironmentV76& out,std::string& e){
 out={};
 SourceCampaignCandidateBorrowV55 actual;
 if(!borrow_source_campaign_candidate_v55(actual,e))return false;
 const auto same=[](const auto& a,const auto& b){return a&&b&&a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);};
 if(!same(actual.actual_world,expected.actual_world)||!same(actual.level,expected.level)||
    !same(actual.application,expected.application)||!same(actual.objects,expected.objects)||
    actual.properties!=expected.properties||!actual.floors||!actual.navigation_registry){
  e="Generic frame environment requires SAME current campaign floor/navigation owners";return false;
 }
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(actual,world,e))return false;
 try{
  if(!world->frame_scratch_v76)world->frame_scratch_v76=std::make_shared<dh2::navigation::CampaignFrameScratchV76>();
  auto lease=std::make_shared<FrameEnvironmentLeaseV76>();
  lease->world=world;lease->floors=actual.floors;lease->navigation=actual.navigation_registry;lease->scratch=world->frame_scratch_v76;
  SourceCampaignFrameEnvironmentV76 prepared;
  if(!lease->scratch->borrow(paths,actors,lease->navigation->registry().floor_capacity,lease->workspace,prepared.workspace,e)||
     !borrow_application_dt_v93(prepared.dt_ms,e))return false;
  prepared.geometry=&lease->floors->collision_world;prepared.graph=&lease->floors->graph;
  prepared.registry=&lease->navigation->registry();prepared.motion_policy=&lease->floors->source_motion_policy_v95;
  prepared.actual_scope=std::move(lease);out=std::move(prepared);e.clear();return true;
 }catch(const std::exception& failure){e=failure.what();return false;}
}
}
