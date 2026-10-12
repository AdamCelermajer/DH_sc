{
 // Actual chest/Visual/V38 scope from V41; cache/admission/consumer endpoints
 // below are explicit source-test services, not main's production budget/GPU.
 struct V45AdmissionState {unsigned active{},admitted{},reads{},borrows{};};
 struct V45AdmissionPin {std::shared_ptr<V45AdmissionState> state;std::uint32_t bytes{};
  V45AdmissionPin(std::shared_ptr<V45AdmissionState> s,std::uint32_t n):state(std::move(s)),bytes(n){++state->active;++state->admitted;}
  ~V45AdmissionPin(){--state->active;}
 };
 auto v45_state=std::make_shared<V45AdmissionState>();auto v45_archive=std::make_shared<assets::ZipAssetPackV1>(archive);
 MaterialImageServicesV43 v45_images{v45_archive,supplied_cache_image_uri_v43,
  [v45_archive,v45_state](const std::string& uri,bool& found,std::vector<std::uint8_t>& out,std::string& e){require(v45_state->active>0,"image read ran before required admission fixture");++v45_state->reads;return v45_archive->read(uri,found,out,e);}};
 GenericActorImageServicesV45 v45_services;v45_services.registry_owner=bytes;v45_services.manager=&fixture.manager;v45_services.roots=roots;
 v45_services.borrow_resources=[&](const ActorVisualReadV37& actor,ActorImageOwnerBorrowV45& out,std::string& e){++v45_state->borrows;
  require(actor.receiver.identity==containers.front()->receiver().base().identity(),"V45 fixture borrowed a foreign actor");return actor_image_owner_v45(containers.front(),v45_images,out,e);};
 v45_services.admit_before_read=[v45_state,v45_archive](const MaterialImageIdentityV43&,const std::string& canonical,std::shared_ptr<void>& pin,std::string& e){
  const auto entries=v45_archive->entries();auto item=std::find_if(entries.begin(),entries.end(),[&](const auto& row){return row.uri==canonical;});
  if(item==entries.end()){e="Explicit admission fixture actual ZIP entry missing";return false;}
  pin=std::make_shared<V45AdmissionPin>(v45_state,item->bytes);e.clear();return true;
 };
 ActorMaterialImageBindingV45 v45_saved;unsigned v45_draws=0;std::vector<std::string> v45_materials;
 auto v45_consumer=[&](const ActorPrimitiveImagesReadV45& frame,std::string&){
  ++v45_draws;v45_materials.push_back(frame.draw.bound_material->id);
  require(frame.draw.skin&&frame.draw.position_space==ActorPositionSpaceV38::retained_skin_world&&frame.draw.instance_index!=helper_instance,"image binding changed actual skin/world/helper eligibility");
  require(frame.draw.bound_material==&helper_visual->scene().materials[frame.draw.instance.materials[frame.draw.primitive_index]],"image binding detached the actual V38 material");
  require(frame.images.size()==4,"actual chest source sampler count differs");unsigned none=0,actual=0;unsigned sampler_index=0;
  const bool v45_opaque=frame.draw.bound_material->id=="Material__37";
  require(v45_opaque||frame.draw.bound_material->id=="Material__38","unexpected actual chest material");
  const std::array<const char*,4> v45_names{"Diffuse","AlphaMap","LightMap","Specular"};
  const std::array<std::uint32_t,4> v45_indices=v45_opaque?std::array<std::uint32_t,4>{0,UINT32_MAX,UINT32_MAX,1}:std::array<std::uint32_t,4>{0,2,UINT32_MAX,1};
  for(const auto& binding:frame.images){const auto& image=binding.image;
   require(image.identity.material_id==frame.draw.bound_material->id&&image.identity.source_resource_uri==*containers.front()->receiver().base().string(0x290),"actual source material/model provenance lost");
   require(image.identity.parameter_name==v45_names[sampler_index]&&image.identity.image_catalog==v45_indices[sampler_index]&&image.identity.parameter_order==sampler_index+2,"actual original chest sampler/catalog/order differs");++sampler_index;
   if(image.identity.state==MaterialImageStateV43::source_no_image){++none;require(image.identity.image_catalog==UINT32_MAX&&image.identity.authored_uri.empty()&&!image.bytes&&!binding.cpu_admission_pin&&image.canonical_uri.empty(),"source no-image became white/cache/admission fallback");}
   else {++actual;require(image.identity.authored_uri.rfind("q:/data/iphone/3d/textures/",0)==0&&image.canonical_uri.rfind("data/3d/textures/",0)==0&&image.bytes&&!image.bytes->empty()&&image.resource_owner==v45_archive&&binding.cpu_admission_pin,"actual full image/cache/CPU admission ownership lost");if(!v45_saved.image.bytes)v45_saved=binding;}
  }
  require(none==(v45_opaque?2u:1u)&&actual==(v45_opaque?2u:3u),"actual chest no-image/image distinction differs");return true;
 };
 ActorImageSubmissionResultV45 v45_result;require(submit_generic_actor_images_v45(helper_input,helper_key,v45_services,v45_consumer,v45_result,e)&&e.empty(),e);
 require(v45_result.draws.primitives==2&&v45_result.draws.skinned_primitives==2&&v45_result.draws.static_primitives==0&&v45_result.sampler_bindings==8&&v45_result.source_no_image==3&&v45_result.actual_image_reads==5&&!v45_result.gpu_texture_residency_claimed,"actual chest draw/image source counts or GPU scope differs");
 require(v45_materials==std::vector<std::string>{"Material__37","Material__38"}&&v45_draws==2&&v45_state->reads==5&&v45_state->admitted==5&&v45_state->active==1,"source materials/read-before-admission/pin release differs");
 auto v45_actual_bytes=v45_saved.image.bytes;std::weak_ptr<void> v45_pin=v45_saved.cpu_admission_pin;
 auto v45_prior=v45_result;v45_prior.sampler_bindings=777;
 auto v45_reject=[&](const GenericActorImageServicesV45& service,const char* message){auto preserved=v45_prior;unsigned called=0;
  require(!submit_generic_actor_images_v45(helper_input,helper_key,service,[&](const ActorPrimitiveImagesReadV45&,std::string&){++called;return true;},preserved,e)&&preserved.sampler_bindings==777&&called==0,message);
  require(v45_state->active==1,"failed image preparation leaked/dropped actual prior admission pin");
 };
 auto v45_missing=v45_services;v45_missing.admit_before_read={};v45_reject(v45_missing,"missing actual admission service accepted");
 auto v45_no_pin=v45_services;v45_no_pin.admit_before_read=[](const MaterialImageIdentityV43&,const std::string&,std::shared_ptr<void>&,std::string& e){e.clear();return true;};v45_reject(v45_no_pin,"successful empty admission pin accepted");
 auto v45_foreign=v45_services;v45_foreign.borrow_resources=[&](const ActorVisualReadV37&,ActorImageOwnerBorrowV45& out,std::string& e){return actor_image_owner_v45(containers.at(1),v45_images,out,e);};v45_reject(v45_foreign,"foreign actual receiver resource owner accepted");
 auto v45_unresolved=v45_services;v45_unresolved.borrow_resources=[&](const ActorVisualReadV37&,ActorImageOwnerBorrowV45& out,std::string& e){auto images=v45_images;images.canonical_uri=[](const MaterialImageIdentityV43& id,std::string& uri,std::string& e){uri=id.authored_uri;e.clear();return true;};return actor_image_owner_v45(containers.front(),images,out,e);};v45_reject(v45_unresolved,"authored drive URI treated as canonical cache/admission key");
 auto v45_missing_resource=v45_services;v45_missing_resource.borrow_resources=[&](const ActorVisualReadV37&,ActorImageOwnerBorrowV45& out,std::string& e){auto images=v45_images;images.read=[](const std::string&,bool& found,std::vector<std::uint8_t>&,std::string& e){found=false;e.clear();return true;};return actor_image_owner_v45(containers.front(),images,out,e);};v45_reject(v45_missing_resource,"missing original image became fallback texture");
 auto& v45_handle=containers.front()->receiver().base().shared_handle();const auto v45_key=v45_handle.key;v45_handle.key=v45_key+123;
 v45_reject(v45_services,"stale actual shared canonical handle accepted");v45_handle.key=v45_key;
 // Visual release inside the final consumer is deliberate adversarial fixture.
 // The source guard must abort before V38 touches another freed source span.
 auto v45_preserved=v45_prior;unsigned v45_release_consumers=0;
 require(!submit_generic_actor_images_v45(helper_input,helper_key,v45_services,[&](const ActorPrimitiveImagesReadV45&,std::string& e){++v45_release_consumers;return helper_visual->release(e);},v45_preserved,e)&&v45_release_consumers==1&&v45_preserved.sampler_bindings==777&&!helper_visual->ready(),"callback release produced a successful/stale source image frame");
 const auto v45_borrows=v45_state->borrows;v45_reject(v45_services,"already released actor Visual accepted");require(v45_state->borrows==v45_borrows,"stale actor reached image resource provider");
 require(v45_saved.image.bytes==v45_actual_bytes&&!v45_pin.expired()&&v45_state->active==1,"actor release invalidated independently owned image bytes/admission");
 v45_saved={};require(!v45_pin.expired()&&v45_state->active==1,"byte-only actual resource copy dropped required admission pin");
 v45_actual_bytes.reset();require(v45_pin.expired()&&v45_state->active==0,"actual copied image admission pin leaked");
 std::cerr<<"GENERIC_ACTOR_IMAGE_V45 PASS actual_chest_draws=2 native_skinned=2 source_materials=37,38 samplers=8 no_image=3 actual_original_cache_reads=5 exact_source_sampler_names_indices_order=1 before_read_admission_fixture=1 foreign_stale_missing_rejected=1 callback_visual_release_rejected=1 independent_image_pin_release=1 GPU_texture_residency=0 whole_level_init=0\n";
}
