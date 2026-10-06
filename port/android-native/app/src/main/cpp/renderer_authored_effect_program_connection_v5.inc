// Include after renderer_effect_draw_v3.inc. Uses actual retained BRES material
// and packaged shader bytes. The driver/config/texture/queue owners are borrows.
class EffectProgramConnectionV5 {
 EffectDrawProgramV5 program_;
 dh2::scene::EffectRenderPassV4 pass_;
 std::uint32_t flags_{};std::string additional_;bool has_additional_{},ready_{};
public:
 void initialize(const dh2::resources::BresView& actual_resource,const char* actual_material,
  const std::vector<std::uint8_t>& actual_shader_pack,std::uint32_t actual_driver_flags,
  const std::string* actual_additional_config){
  if(ready_)throw std::runtime_error("Retained effect source program already initialized");
  std::string error;dh2::scene::EffectRenderPassV4 pass;
  if(!dh2::scene::effect_render_pass_v4(actual_resource,actual_material,"default",pass,error))
   throw std::runtime_error("Effect actual material technique rejected: "+error);
  dh2::scene::ShaderSourcePack pack;
  if(!pack.load(actual_shader_pack.data(),actual_shader_pack.size(),error))
   throw std::runtime_error("Effect actual shader pack rejected: "+error);
  const auto* vertex=pack.find(pass.vertex_file);const auto* fragment=pack.find(pass.fragment_file);
  if(!vertex||!fragment)throw std::runtime_error("Effect shader pack lacks actual ProfileCOMMON source");
  dh2::scene::ShaderSourcePlan vs,fs;
  auto text=[](const dh2::scene::ShaderPackMember& entry){return std::string_view(reinterpret_cast<const char*>(entry.bytes.data()),entry.bytes.size());};
  if(!dh2::scene::shader_source_plan(actual_driver_flags,4,pass.vertex_file,pass.vertex_defines,actual_additional_config,text(*vertex),vs,error)||
     !dh2::scene::shader_source_plan(actual_driver_flags,0,pass.fragment_file,pass.fragment_defines,actual_additional_config,text(*fragment),fs,error))
   throw std::runtime_error("Effect actual shader source plan rejected: "+error);
  program_.initialize(pass,vs,fs);
  pass_=std::move(pass);flags_=actual_driver_flags;has_additional_=actual_additional_config!=nullptr;
  additional_=actual_additional_config?*actual_additional_config:std::string();ready_=true;
 }
 bool ready()const noexcept{return ready_;}
 GLuint native_program()const noexcept{return program_.native_program();}
 void draw_one_in_batch(const EffectDrawProgramV5::BatchScope& scope,
  const dh2::scene::EffectRenderPassV4& source_pass,const EffectDrawBorrowV5& draw,
  bool flip,bool receipt,std::uint32_t flags,const std::string* config){
  if(!ready_||flags!=flags_||bool(config)!=has_additional_||(config&&*config!=additional_)||
     source_pass.vertex_file!=pass_.vertex_file||source_pass.vertex_defines!=pass_.vertex_defines||
     source_pass.fragment_file!=pass_.fragment_file||source_pass.fragment_defines!=pass_.fragment_defines)
   throw std::runtime_error("Effect cached source shader/config owner disagrees with actual material pass");
  program_.draw_in_batch(scope,source_pass,draw,flip,receipt);
 }
 void draw_pass(const dh2::scene::EffectRenderPassV4& source_pass,
  const std::vector<EffectDrawBorrowV5>& ordered,bool actual_target_flip,
  bool actual_queue_receipt,std::uint32_t actual_driver_flags,const std::string* actual_config){
  if(ordered.empty())return;
  if(!ready_||actual_driver_flags!=flags_||bool(actual_config)!=has_additional_||
     (actual_config&&*actual_config!=additional_)||
     source_pass.vertex_file!=pass_.vertex_file||source_pass.vertex_defines!=pass_.vertex_defines||
     source_pass.fragment_file!=pass_.fragment_file||source_pass.fragment_defines!=pass_.fragment_defines)
   throw std::runtime_error("Effect cached source shader/config owner disagrees with actual material pass");
  program_.draw(source_pass,ordered,actual_target_flip,actual_queue_receipt);
 }
 void draw(const std::vector<EffectDrawBorrowV5>& ordered,bool actual_target_flip,
  bool actual_queue_receipt,std::uint32_t actual_driver_flags,const std::string* actual_config){
  if(ordered.empty())return;
  if(!ready_||actual_driver_flags!=flags_||bool(actual_config)!=has_additional_||
     (actual_config&&*actual_config!=additional_))
   throw std::runtime_error("Effect actual driver/config owner changed: rebind source program");
  program_.draw(pass_,ordered,actual_target_flip,actual_queue_receipt);
 }
 // Call release only on its owning live GL context. Lost contexts discard this
 // owner instead; never delete old program IDs into a replacement context.
 void release(){program_.release();ready_=false;additional_.clear();}
};
