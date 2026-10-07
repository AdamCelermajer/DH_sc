// Include after renderer_blood_draw_v3.inc. Uses actual retained BRES material
// and packaged shader bytes. The driver/config/texture/queue owners are borrows.
class BloodProgramConnectionV4 {
 BloodDrawProgramV3 program_;
 dh2::scene::BloodRenderPassV3 pass_;
 std::uint32_t flags_{};std::string additional_;bool has_additional_{},ready_{};
public:
 void initialize(const dh2::resources::BresView& actual_resource,
  const std::vector<std::uint8_t>& actual_shader_pack,std::uint32_t actual_driver_flags,
  const std::string* actual_additional_config){
  if(ready_)throw std::runtime_error("Retained blood source program already initialized");
  std::string error;dh2::scene::BloodRenderPassV3 pass;
  if(!dh2::scene::blood_render_pass_v3(actual_resource,"fx_particles_alpha","default",pass,error))
   throw std::runtime_error("Blood actual material technique rejected: "+error);
  dh2::scene::ShaderSourcePack pack;
  if(!pack.load(actual_shader_pack.data(),actual_shader_pack.size(),error))
   throw std::runtime_error("Blood actual shader pack rejected: "+error);
  const auto* vertex=pack.find(pass.vertex_file);const auto* fragment=pack.find(pass.fragment_file);
  if(!vertex||!fragment)throw std::runtime_error("Blood shader pack lacks actual ProfileCOMMON source");
  dh2::scene::ShaderSourcePlan vs,fs;
  auto text=[](const dh2::scene::ShaderPackMember& entry){return std::string_view(reinterpret_cast<const char*>(entry.bytes.data()),entry.bytes.size());};
  if(!dh2::scene::shader_source_plan(actual_driver_flags,4,pass.vertex_file,pass.vertex_defines,actual_additional_config,text(*vertex),vs,error)||
     !dh2::scene::shader_source_plan(actual_driver_flags,0,pass.fragment_file,pass.fragment_defines,actual_additional_config,text(*fragment),fs,error))
   throw std::runtime_error("Blood actual shader source plan rejected: "+error);
  program_.initialize(pass,vs,fs);
  pass_=std::move(pass);flags_=actual_driver_flags;has_additional_=actual_additional_config!=nullptr;
  additional_=actual_additional_config?*actual_additional_config:std::string();ready_=true;
 }
 bool ready()const noexcept{return ready_;}
 void draw(const std::vector<BloodDrawBorrowV3>& ordered,bool actual_target_flip,
  bool actual_queue_receipt,std::uint32_t actual_driver_flags,const std::string* actual_config){
  if(ordered.empty())return;
  if(!ready_||actual_driver_flags!=flags_||bool(actual_config)!=has_additional_||
     (actual_config&&*actual_config!=additional_))
   throw std::runtime_error("Blood actual driver/config owner changed: rebind source program");
  program_.draw(pass_,ordered,actual_target_flip,actual_queue_receipt);
 }
 // Call release only on its owning live GL context. Lost contexts discard this
 // owner instead; never delete old program IDs into a replacement context.
 void release(){program_.release();ready_=false;additional_.clear();}
};
