#include "source_campaign_camera_release_v88.hpp"
#include "model_renderer.hpp"
#include "gameplay_camera_application_v23.hpp"
#include "canonical_level_context_v1.hpp"
namespace model_renderer {
bool borrow_source_campaign_camera_release_v88(const SourceCampaignCandidateBorrowV55& c,
 std::uintptr_t id,SourceCampaignCameraReleaseBorrowV88& out,std::string& e){
 out={};if(!id||!c.actual_world||!c.level||!c.camera_application){e="Required actual campaign Level camera D0 borrower";return false;}
 auto session=c.camera_application->world();auto* load=session?session->camera.get():nullptr;
 if(!session||!load){e="Required SAME retained World camera-load owner";return false;}
 const auto& cells=c.level->constructor_fields_v3();
 const auto level=load->level();const auto overview=load->overview();
 const bool is_level=level&&cells.field128==id&&reinterpret_cast<std::uintptr_t>(level.get())==id;
 const bool is_overview=overview&&cells.field12c==id&&overview->identity()==id;
 if(is_level==is_overview){e="Camera D0 identity has no unique SAME source128/12c producer";return false;}
 const auto weak_session=std::weak_ptr<dh2::camera::CameraWorldSessionV23>(session);
 const auto weak_app=std::weak_ptr<dh2::camera::GameplayCameraApplicationV23>(c.camera_application);
 const auto weak_level=std::weak_ptr<dh2::loader::CanonicalLevelContextV1>(c.level);
 out.receiver=is_level?std::shared_ptr<void>(level):std::shared_ptr<void>(overview);
 out.identity=id;out.source_slot=is_level?0x128:0x12c;
 const auto slot=out.source_slot;
 out.deleting_destructor=[weak_session,weak_app,weak_level,id,slot](std::string& e){
  //The callback pins the containing native load graph during its synchronous
  //D0. Individual receiver leases alone must not outlive V19's typed services.
  auto session=weak_session.lock();auto application=weak_app.lock();auto actual_level=weak_level.lock();
  if(!session||!application||!actual_level||application->world()!=session||!session->camera){e="Released/replaced SAME campaign camera D0 scope";return false;}
  const auto& fields=actual_level->constructor_fields_v3();
  if((slot==0x128?fields.field128:fields.field12c)!=id){e="Camera D0 source slot changed before captured delivery";return false;}
  //Loader owns each subsequent zero store, precisely after successful D0.
  return slot==0x128?application->source_release_level128_v88(id,e):application->source_release_overview12c_v88(id,e);
 };
 e.clear();return true;
}
}
