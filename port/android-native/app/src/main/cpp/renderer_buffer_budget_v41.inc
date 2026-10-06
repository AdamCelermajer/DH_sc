struct BufferChargeV41 {
 std::shared_ptr<dh2::resources::ContextResourceBudgetV37> budget;
 dh2::resources::ResourceTokenV37 token;
 std::uint64_t generation{},bytes{};
 GLenum target{};dh2::resources::ResourceScopeV37 scope{};
};
std::map<GLuint,BufferChargeV41> generic_buffer_charges_v41;
void release_buffer_v41(GLuint& name,bool lost){
 if(!name)return;
 auto found=generic_buffer_charges_v41.find(name);
 if(found==generic_buffer_charges_v41.end())throw std::runtime_error("V41 generic buffer lacks admitted owner");
 auto& owner=found->second;const auto current=owner.budget->snapshot();
 const bool same=current.context_ready&&current.context_generation==owner.generation;
 if(lost&&same)throw std::runtime_error("V41 live buffer context cannot be abandoned");
 if(!lost&&same)glDeleteBuffers(1,&name);
 std::string error;if(!owner.budget->release(owner.token,error))throw std::runtime_error(error);
 generic_buffer_charges_v41.erase(found);name=0;
}
void discard_generic_buffers_v41(){while(!generic_buffer_charges_v41.empty()){auto name=generic_buffer_charges_v41.begin()->first;release_buffer_v41(name,true);}}
std::uint64_t buffer_bytes_v41(std::size_t count,std::size_t stride){std::string error;std::uint64_t bytes;if(!dh2::resources::checked_resource_bytes_v37(count,stride,bytes,error))throw std::runtime_error(error);return bytes;}
void buffer_storage_v41(GLuint& name,GLenum target,std::uint64_t bytes,const void* data,GLenum usage,
 dh2::resources::ResourceScopeV37 scope){
 using Kind=dh2::resources::ResourceKindV37;
 if((target!=GL_ARRAY_BUFFER&&target!=GL_ELEMENT_ARRAY_BUFFER)||!bytes||bytes>std::uint64_t(std::numeric_limits<GLsizeiptr>::max()))throw std::runtime_error("V41 generic buffer target/byte range");
 auto ledger=dh2::android_resources::budget_lease_v39();const auto generation=ledger->snapshot().context_generation;
 auto old=generic_buffer_charges_v41.end();
 if(name){old=generic_buffer_charges_v41.find(name);if(old==generic_buffer_charges_v41.end()||old->second.target!=target||old->second.generation!=generation||old->second.scope!=scope)throw std::runtime_error("V41 same buffer owner/target/context/scope required");}
 std::string error;dh2::resources::ResourceReservationV37 admission;
 const dh2::resources::ResourceChargeV37 charge{target==GL_ARRAY_BUFFER?Kind::vertex_buffer:Kind::index_buffer,scope,bytes,0};
 const bool accepted=name?ledger->reserve_replace(old->second.token,charge,dh2::resources::ReplacementModeV37::same_object_coexisting_storage,admission,error):ledger->reserve_create(charge,admission,error);
 if(!accepted)throw std::runtime_error(error);
 std::map<GLuint,BufferChargeV41> pending;decltype(pending)::node_type node;
 GLuint candidate=name;
 try{
  if(!name){
   pending.emplace(0,BufferChargeV41{ledger,{},generation,bytes,target,scope});node=pending.extract(0);
   glGenBuffers(1,&candidate);if(!candidate)throw std::runtime_error("V41 generic buffer GL name unavailable");
   if(generic_buffer_charges_v41.count(candidate)){candidate=0;throw std::runtime_error("V41 generated buffer name conflicts with live owner");}
  }
  glBindBuffer(target,candidate);glBufferData(target,GLsizeiptr(bytes),data,usage);check("Admitted generic buffer storage");
  if(name){if(!admission.commit(old->second.token,error))throw std::runtime_error(error);old->second.bytes=bytes;}
  else{
   if(!admission.commit(node.mapped().token,error))throw std::runtime_error(error);
   node.key()=candidate;auto inserted=generic_buffer_charges_v41.insert(std::move(node));
   if(!inserted.inserted){if(!ledger->release(inserted.node.mapped().token,error))throw std::runtime_error(error);throw std::runtime_error("V41 generic buffer publication changed");}
   name=candidate;
  }
 }catch(...){
  admission.abort();
  if(name)release_buffer_v41(name,false);
  else{const auto current=ledger->snapshot();if(candidate&&current.context_ready&&current.context_generation==generation)glDeleteBuffers(1,&candidate);}
  throw;
 }
}
void buffer_subdata_v41(GLuint& name,GLenum target,std::uint64_t bytes,const void* data,dh2::resources::ResourceScopeV37 scope){
 if(!name){buffer_storage_v41(name,target,bytes,data,GL_DYNAMIC_DRAW,scope);return;}
 auto found=generic_buffer_charges_v41.find(name);
 if(found==generic_buffer_charges_v41.end()||found->second.target!=target||found->second.scope!=scope||
    !found->second.budget->snapshot().context_ready||found->second.generation!=found->second.budget->snapshot().context_generation||bytes>found->second.bytes||!data)
  throw std::runtime_error("V41 subdata requires same live admitted buffer and bounded source bytes");
 try{glBindBuffer(target,name);glBufferSubData(target,0,GLsizeiptr(bytes),data);check("Admitted generic skin/geometry update");}
 catch(...){release_buffer_v41(name,false);throw;}
}
