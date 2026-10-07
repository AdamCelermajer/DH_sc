// One owner registry for generic world/actor/equipment/loot texture names on
// the actual context. No shader, wrapping, filtering or decoded bytes change.
struct ModelTextureChargeV40 {
 std::shared_ptr<dh2::resources::ContextResourceBudgetV37> budget;
 dh2::resources::ResourceTokenV37 token;
 std::uint64_t generation{};
};
std::map<GLuint,ModelTextureChargeV40> model_texture_charges_v40;
void release_model_texture_v40(GLuint name,bool lost){
 if(!name)return;
 auto found=model_texture_charges_v40.find(name);
 if(found==model_texture_charges_v40.end())throw std::runtime_error("V40 model texture lacks admitted owner");
 auto& owner=found->second;const auto current=owner.budget->snapshot();
 const bool same=current.context_ready&&current.context_generation==owner.generation;
 if(lost&&same)throw std::runtime_error("V40 live model texture context cannot be abandoned");
 if(!lost&&same)glDeleteTextures(1,&name);
 std::string error;if(!owner.budget->release(owner.token,error))throw std::runtime_error(error);
 model_texture_charges_v40.erase(found);
}
void discard_model_textures_v40(){
 while(!model_texture_charges_v40.empty())release_model_texture_v40(model_texture_charges_v40.begin()->first,true);
}
void release(std::vector<Draw>& batches,std::vector<GLuint>& textures){
 for(auto& b:batches){release_buffer_v41(b.vertices,false);release_buffer_v41(b.indices,false);}
 for(auto name:textures)release_model_texture_v40(name,false);
 batches.clear();textures.clear();
}
std::shared_ptr<dh2::resources::AdmittedVectorV40> read_model_texture_v40(AAssetManager* assets,const std::string& name,
 dh2::android_ui::OriginalCacheAssetsV1* original,dh2::resources::ResourceScopeV37 scope,
 const std::shared_ptr<dh2::resources::ContextResourceBudgetV37>& ledger){
 auto result=std::make_shared<dh2::resources::AdmittedVectorV40>();std::string error;
 if(original){
  bool found=false;
  const auto admit=[&](std::uint32_t bytes,std::string& why){return result->admission.reserve(ledger,scope,bytes,why);};
  if(!original->read_admitted("data/3d/textures/"+name,found,result->bytes,admit,error)||!found)
   throw std::runtime_error("Original equipped texture unavailable: "+name+" | "+error);
 }else{
  auto path=name;std::transform(path.begin(),path.end(),path.begin(),[](unsigned char c){return char(std::tolower(c));});
  struct Close {void operator()(AAsset* a)const{if(a)AAsset_close(a);}};
  std::unique_ptr<AAsset,Close> asset(AAssetManager_open(assets,("textures/"+path).c_str(),AASSET_MODE_STREAMING));
  if(!asset)throw std::runtime_error("Bundled asset missing: textures/"+name);
  const auto length=AAsset_getLength64(asset.get());
  if(length<=0||length>32*1024*1024)throw std::runtime_error("Texture exceeds size limit");
  if(!result->admission.reserve(ledger,scope,std::uint64_t(length),error))throw std::runtime_error(error);
  result->bytes.resize(std::size_t(length));std::size_t done=0;
  while(done<result->bytes.size()){
   const auto got=AAsset_read(asset.get(),result->bytes.data()+done,result->bytes.size()-done);
   if(got<=0)throw std::runtime_error("Short asset read");
   done+=std::size_t(got);
  }
 }
 if(!result->admission.commit(error))throw std::runtime_error(error);
 return result;
}
GLuint upload(AAssetManager* assets,const std::string& name,std::map<std::string,GLuint>& cache,std::vector<GLuint>& owned,
 dh2::android_ui::OriginalCacheAssetsV1* original=nullptr,
 dh2::resources::ResourceScopeV37 scope=dh2::resources::ResourceScopeV37::world){
 const auto found=cache.find(name);if(found!=cache.end()){if(!found->second)throw std::runtime_error("V40 pending model texture reentered");return found->second;}
 auto ledger=dh2::android_resources::budget_lease_v39();std::string error;
 unsigned width=1,height=1;dh2::textures::View view{};
 std::shared_ptr<dh2::resources::AdmittedVectorV40> encoded;
 if(!name.empty()){
  encoded=read_model_texture_v40(assets,name,original,scope,ledger);
  if(dh2_texture_open(encoded->bytes.data(),encoded->bytes.size(),&view)!=dh2::textures::Error::ok)throw std::runtime_error("Texture header rejected: "+name);
  width=view.width;height=view.height;
 }
 GLint maximum=0;glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maximum);
 if(maximum<=0||width>unsigned(maximum)||height>unsigned(maximum))throw std::runtime_error("Texture exceeds GPU limit");
 std::uint64_t gpu_bytes;
 if(!dh2::resources::rgba_texture_bytes_v37(width,height,false,gpu_bytes,error))throw std::runtime_error(error);
 dh2::resources::ResourceReservationV37 gpu;
 if(!ledger->reserve_create({dh2::resources::ResourceKindV37::texture,scope,gpu_bytes,0},gpu,error))throw std::runtime_error(error);
 dh2::resources::RetainedBytesV39 rgba;
 if(!rgba.allocate(ledger,scope,static_cast<std::size_t>(gpu_bytes),error))throw std::runtime_error(error);
 if(name.empty())std::fill_n(rgba.data(),4,255);
 else if(dh2_texture_decode(&view,rgba.data(),rgba.size())!=dh2::textures::Error::ok)throw std::runtime_error("Texture decode rejected: "+name);
 // Publication bookkeeping must already exist before the first GL allocation.
 auto cached=cache.try_emplace(name,0);if(!cached.second)throw std::runtime_error("V40 model cache publication changed");
 GLuint candidate=0;const auto generation=ledger->snapshot().context_generation;
 std::map<GLuint,ModelTextureChargeV40> pending_registry;
 try{
  pending_registry.emplace(0,ModelTextureChargeV40{ledger,{},generation});
  auto node=pending_registry.extract(0);
  if(owned.size()==owned.capacity()){
   const auto capacity=owned.capacity();
   if(capacity>std::numeric_limits<std::size_t>::max()/2)throw std::runtime_error("V40 model ownership capacity overflow");
   owned.reserve(std::max(owned.size()+1,capacity?capacity*2:std::size_t(8)));
  }
  glGenTextures(1,&candidate);if(!candidate)throw std::runtime_error("V40 model texture GL name unavailable");
  if(model_texture_charges_v40.count(candidate)){candidate=0;throw std::runtime_error("V40 generated name conflicts with existing admitted owner");}
  glBindTexture(GL_TEXTURE_2D,candidate);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());check("Model texture upload");
  if(!gpu.commit(node.mapped().token,error))throw std::runtime_error(error);
  node.key()=candidate;
  auto published=model_texture_charges_v40.insert(std::move(node));
  if(!published.inserted){
   auto& ticket=published.node.mapped().token;
   if(!ledger->release(ticket,error))throw std::runtime_error(error);
   throw std::runtime_error("V40 GL texture name already has an admitted owner");
  }
  // push_back cannot allocate after the pre-reserve. No allocation follows GL.
  owned.push_back(candidate);cached.first->second=candidate;return candidate;
 }catch(...){
  const auto current=ledger->snapshot();
  if(candidate&&current.context_ready&&current.context_generation==generation)glDeleteTextures(1,&candidate);
  cache.erase(cached.first);throw;
 }
}
