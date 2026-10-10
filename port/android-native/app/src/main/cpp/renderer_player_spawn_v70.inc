// Actual source deferred Character Spawn stage for PlayerManager AddCharacter.
// This owns no second actor/registry: V60 constructor and original Spawn owner
// mutate the SAME canonical map/defaults/Handle/pending list.
class RendererPlayerSpawnV70 {
 std::weak_ptr<RendererCharacterCandidateV60> transport_;
 std::weak_ptr<dh2::loader::CanonicalReceiverTransportV1> receivers_;
 std::weak_ptr<dh2::world::CanonicalObjectManagerV1> objects_;
 dh2::world::CanonicalPropertyMapV1* properties_{};
 std::unique_ptr<dh2::world::CanonicalSpawnAttemptV1> attempt_;
 std::string name_;
 static bool construct(void* p,const dh2::world::CanonicalFactoryEntryV1& entry,dh2::world::CanonicalClassReceiverV1& out,std::string& e){
  auto& self=*static_cast<RendererPlayerSpawnV70*>(p);auto actual=self.transport_.lock();
  if(!actual){e="Released source player Character factory";return false;}
  if(!actual->construct_source_spawn_v68(entry,self.name_.c_str(),out,e))return false;
  // The player C1 bypasses the XML catalog constructor, just as SpawnGroups
  // does. Retain its SAME dispatch before Add publishes the receiver to the
  // manager; later IsGameObject/localization visits must find that dispatch.
  auto receivers=self.receivers_.lock();
  if(!receivers){e="Released SAME source player catalog receiver transport";return false;}
  return receivers->admit_constructed_source_v91(out,e);
 }
 static bool resolve(void* p,dh2::target_providers::Handle16& handle,bool refresh,const dh2::world::CanonicalObjectBorrowV1*& out,std::string& e){
  auto& self=*static_cast<RendererPlayerSpawnV70*>(p);auto objects=self.objects_.lock();if(!objects){e="Released SAME canonical Spawn manager";return false;}return objects->resolve_handle_v4(handle,refresh,out,[](std::string& error){error="Original Character Spawn resolved NULL (source assertion)";return false;},e);
 }
 static bool updatable(void*,const dh2::world::CanonicalObjectBorrowV1& object,bool& value,std::string& e){
  if(!object.type_f4||*object.type_f4!=0){e="Required actual Character virtual38 receiver";return false;}value=true;return true; //38aac0 literal1.
 }
 static bool pending(void* p,const dh2::world::CanonicalObjectBorrowV1& object,std::string& e){auto objects=static_cast<RendererPlayerSpawnV70*>(p)->objects_.lock();if(!objects){e="Released SAME canonical Spawn pending receiver";return false;}return objects->append_pending(object,e);}
public:
 RendererPlayerSpawnV70(const std::shared_ptr<RendererCharacterCandidateV60>& transport,const model_renderer::SourceCampaignCandidateBorrowV55& actual):transport_(transport),receivers_(actual.receiver_transport_v69),objects_(actual.objects),properties_(actual.properties){}
 bool spawn(const char* name,std::uintptr_t& character,std::string& e){
  auto objects=objects_.lock();if(attempt_||!name||transport_.expired()||!objects||!properties_){e="Required once-only actual deferred player Spawn receiver";return false;}name_=name;
  dh2::world::CanonicalSpawnServicesV1 s;s.context=this;s.construct=construct;s.resolve=resolve;s.virtual38=updatable;s.append_pending=pending;
  attempt_=std::make_unique<dh2::world::CanonicalSpawnAttemptV1>(*objects,*properties_,s);
  if(!attempt_->spawn("Character",name_.c_str(),true,true,e))return false;
  const auto& handle=attempt_->handle();const auto* actual=objects->object(handle.key);
  if(!actual||!actual->identity||actual->identity!=handle.cached){e="Source deferred Spawn did not publish SAME Character Handle";return false;}
  character=actual->identity;return true;
 }
};
