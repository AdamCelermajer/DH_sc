#include "effect_material_directory_v4.hpp"
#include <cstring>
namespace dh2::scene {
struct EffectMaterialDirectoryV4::Storage {
 std::shared_ptr<ShaderProgramRecordV4> program;
 MaterialCompareViewV4 view;
 std::vector<std::vector<std::uint8_t>> bytes;
 std::vector<std::vector<std::array<std::uint8_t,68>>> matrices;
};
EffectMaterialDirectoryV4::EffectMaterialDirectoryV4()=default;
EffectMaterialDirectoryV4::~EffectMaterialDirectoryV4()=default;
const MaterialCompareViewV4* EffectMaterialDirectoryV4::view()const noexcept{return storage_?&storage_->view:nullptr;}
bool EffectMaterialDirectoryV4::refresh(const std::shared_ptr<ShaderProgramRecordV4>& program,
 const EffectRenderPassV4& source,void* context,EffectMaterialReadV4 read,std::string& error){
 if(!program||!program->same_program||!read||program->global_count>program->parameters.size()){
  error="Required actual linked program/reflected material directory";return false;}
 auto pending=std::make_unique<Storage>();pending->program=program;
 const auto count=program->parameters.size()-program->global_count;
 if(count>0x8000){error="Required source material parameter index range";return false;}
 pending->bytes.resize(count);pending->matrices.resize(count);pending->view.parameters.reserve(count);
 MaterialPassCompareV4 pass;pass.state=source.pass;pass.shader_identity=program->identity();pass.shader_sort_id=program->collection_id;
 constexpr std::uint8_t widths[]={0,4,8,12,16,4,8,12,16,16,36,0,0,0,0,0};
 for(std::size_t i=0;i<count;++i){const auto& reflected=program->parameters[program->global_count+i];EffectMaterialValueBorrowV4 value;
  if(!read(context,reflected,value,error))return false;
  MaterialParameterCompareV4 p;p.semantic=reflected.semantic;p.type=reflected.type;p.count=reflected.count;
  if(!p.count){error="Required positive actual GL uniform element count";return false;}
  if(p.semantic==2){if(value.texture_count<p.count||(p.count&&!value.texture_identities)){error="Required same retained texture material value";return false;}
   p.textures.assign(value.texture_identities,value.texture_identities+p.count);
  }else if(p.type==11){if(value.matrix_count<p.count||(p.count&&!value.matrices68)){error="Required source Matrix4f68 material value";return false;}
   auto& matrices=pending->matrices[i];matrices.resize(p.count);
   for(unsigned j=0;j<p.count;++j){if(!value.matrices68[j]){error="Required actual nonnull material matrix owner";return false;}
    std::memcpy(matrices[j].data(),value.matrices68[j],68);p.matrices.push_back(matrices[j].data());}
  }else{if(p.type>=sizeof(widths)||!widths[p.type]){error="Required supported reflected material type";return false;}
   p.element_width=widths[p.type];const auto n=std::size_t(p.count)*p.element_width;
   if(value.byte_size<n||(n&&!value.bytes)){error="Required full current material typed bytes";return false;}
   auto& bytes=pending->bytes[i];bytes.assign(value.bytes,value.bytes+n);p.bytes=bytes.data();p.byte_size=bytes.size();
  }
  pass.active_indices.push_back(std::uint16_t(i));pending->view.parameters.push_back(std::move(p));
 }
 pending->view.passes.push_back(std::move(pass));if(material_refresh_hash_v4(pending->view,error))return false;
 storage_=std::move(pending);return true;
}
}
