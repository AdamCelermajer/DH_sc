// Native dispatch transport only. The source owner remains authoritative;
// closing admission neither clears source state nor simulates Level D1.
namespace {
struct NativeCampaignDeliveryV104 {
 model_renderer::SourceCampaignDeliveryV104 scope;
 std::string error;
 bool allowed{true};
 explicit NativeCampaignDeliveryV104(model_renderer::SourceCampaignDeliveryKindV104 kind){
  if(!model_renderer::source_campaign_active_v55())return;
  std::shared_ptr<void> world;std::shared_ptr<model_renderer::SourceCampaignAdmissionV104> gate;
  bool blocked{};
  allowed=model_renderer::borrow_source_campaign_world_owner_v104(world,error)&&
   model_renderer::borrow_source_campaign_admission_v104(world,gate,error)&&
   scope.acquire(std::move(gate),kind,blocked,error)&&!blocked;
  if(!allowed&&!error.empty())__android_log_print(ANDROID_LOG_ERROR,tag,"Native campaign delivery: %s",error.c_str());
 }
 explicit operator bool()const noexcept{return allowed;}
};
}
