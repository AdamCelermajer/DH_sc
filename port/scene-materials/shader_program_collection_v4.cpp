#include "shader_program_collection_v4.hpp"
namespace dh2::scene {
std::string effect_program_cache_name_v4(const EffectRenderPassV4& p){
 // Source634b80..bec appends SPass +4,+c,+10,+18 without separators.
 return p.vertex_file+p.vertex_defines+p.fragment_file+p.fragment_defines;
}
bool ShaderProgramCollectionV4::get_or_create(const std::string& name,void* context,
 ShaderProgramCreateV4 create,std::shared_ptr<ShaderProgramRecordV4>& out,std::string& error){
 auto found=records_.find(name);if(found!=records_.end()){out=found->second;return true;}
 if(!create||name.empty()||next_==0xffff){error="Required valid source shader collection creation";return false;}
 auto pending=std::make_shared<ShaderProgramRecordV4>();pending->collection_id=next_;
 if(!create(context,next_,name,pending->same_program,pending->parameters,error))return false;
 if(!pending->same_program){error="Required actual linked GL shader owner";return false;}
 for(auto& p:pending->parameters)shader_uniform_reflect_v4(p);
 pending->global_count=shader_uniform_partition_v4(pending->parameters);
 records_.emplace(name,pending);++next_;++count_;out=std::move(pending);return true;
}
}
