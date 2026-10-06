#include "../material_compare_v4.hpp"
#include "../shader_program_collection_v4.hpp"
#include "../effect_material_directory_v4.hpp"
#include "material_compare_v4_gold.hpp"
#include <iostream>
#include <cstring>
using namespace dh2::scene;
MaterialCompareViewV4 view(const MaterialGoldView& g){MaterialCompareViewV4 v;v.hash_ready=true;v.source_hash=g.hash;MaterialPassCompareV4 p;std::memcpy(p.state.data(),g.state,32);p.shader_identity=g.shader+1;p.shader_sort_id=g.id;p.active_indices={0,1,2,3};v.passes.push_back(p);
 MaterialParameterCompareV4 x;x.semantic=6;x.type=8;x.count=1;x.element_width=16;x.bytes=g.color;x.byte_size=16;v.parameters.push_back(x);
 x={};x.semantic=3;x.type=11;x.count=1;x.matrices={g.matrix};v.parameters.push_back(x);
 x={};x.semantic=2;x.type=12;x.count=1;x.textures={g.texture};v.parameters.push_back(x);
 x={};x.semantic=33;x.type=5;x.count=1;x.element_width=4;x.bytes=g.bias;x.byte_size=4;v.parameters.push_back(x);return v;
}
bool create(void* c,std::uint16_t id,const std::string&,std::shared_ptr<void>& owner,std::vector<ShaderUniformReflectionV4>& uniforms,std::string&){++*static_cast<unsigned*>(c);owner=std::make_shared<unsigned>(id);ShaderUniformReflectionV4 p;p.name="DiffuseColor";p.gl_type=0x8b52;p.count=1;p.location=4;uniforms={p};return true;}
struct Profile {float color[4]{1,.5,.25,1},bias{};std::uintptr_t texture=0x100002000ull;unsigned char matrix[68]{};const unsigned char* matrix_pointer=matrix;};
bool read_profile(void* context,const ShaderUniformReflectionV4& u,EffectMaterialValueBorrowV4& v,std::string& e){auto& p=*static_cast<Profile*>(context);
 if(u.semantic==6){v.bytes=reinterpret_cast<unsigned char*>(p.color);v.byte_size=16;return true;}
 if(u.semantic==3){v.matrices68=&p.matrix_pointer;v.matrix_count=1;return true;}
 if(u.semantic==2){v.texture_identities=&p.texture;v.texture_count=1;return true;}
 if(u.semantic==33){v.bytes=reinterpret_cast<unsigned char*>(&p.bias);v.byte_size=4;return true;}
 e="unknown reflected value";return false;}
int main(){unsigned checks=0;std::string error;for(const auto& g:material_gold){auto a=view(g.a),b=view(g.b);bool eq=false,less=false;
 if(material_equal_v4(eq,a,b,error)||material_less_v4(less,a,b,error)||eq!=g.equal||less!=g.less){std::cerr<<"gold "<<checks<<" got "<<eq<<','<<less<<" expected "<<g.equal<<','<<g.less<<" "<<error<<'\n';return 1;}++checks;
 auto native=a;if(material_refresh_hash_v4(native,error))return 2;unsigned ordinary=0,texture=0;for(auto i:{0u,1u,3u}){const auto& x=a.parameters[i];if(i==1){for(unsigned k=0;k<68;++k)ordinary=ordinary*13+g.a.matrix[k];}else for(unsigned k=0;k<x.byte_size;++k)ordinary=ordinary*13+x.bytes[k];}
 std::uintptr_t identity=g.a.texture;auto bytes=reinterpret_cast<unsigned char*>(&identity);for(unsigned i=0;i<sizeof(identity);++i)texture=texture*13+bytes[i];unsigned hash=(((g.a.id&255)^(g.a.id>>8))<<24)|((g.a.state[0]&15)<<8)|(ordinary&255)|((texture&4095)<<12);
 if(native.source_hash!=hash)return 3;++checks;
 unsigned texture32=0;for(unsigned j=0;j<4;++j)texture32=texture32*13+((g.a.texture>>(j*8))&255);
 unsigned original=(g.a.hash&~0x00fff0ffu)|(ordinary&255)|((texture32&4095)<<12);
 if(original!=g.a.original_hash)return 12;++checks;
 }
 ShaderProgramCollectionV4 collection;unsigned calls=0;std::shared_ptr<ShaderProgramRecordV4> a,b,c;
 if(!collection.get_or_create("ProfileCOMMON_TEXTURED",&calls,create,a,error)||!collection.get_or_create("ProfileCOMMON_TEXTURED",&calls,create,b,error)||!collection.get_or_create("ProfileCOMMON_ADDITIVE",&calls,create,c,error))return 4;
 if(a!=b||a==c||a->collection_id!=0||c->collection_id!=1||calls!=2||collection.count()!=2||collection.next_id()!=2)return 5;++checks;
 auto fewer=view(material_gold[0].a),more=fewer;more.passes.push_back(more.passes[0]);bool less=false;
 if(material_less_v4(less,fewer,more,error)||!less)return 6;++checks;
 auto invalid=fewer;invalid.parameters[0].bytes=nullptr;bool unchanged=true;if(material_equal_v4(unchanged,invalid,fewer,error)!=-2||!unchanged)return 7;++checks;
 auto program=std::make_shared<ShaderProgramRecordV4>();program->same_program=std::make_shared<unsigned>(99);program->collection_id=17;
 for(auto pair:{std::pair<const char*,unsigned>{"WorldViewProjectionMatrix",0x8b5c},{"TextureMatrix0",0x8b5c},{"DiffuseColor",0x8b52},{"Sampler0",0x8b5e},{"sampler0_bias",0x1406}}){ShaderUniformReflectionV4 u;u.name=pair.first;u.gl_type=pair.second;u.count=1;shader_uniform_reflect_v4(u);program->parameters.push_back(u);}
 program->global_count=shader_uniform_partition_v4(program->parameters);Profile profile;EffectRenderPassV4 pass;EffectMaterialDirectoryV4 directory,second;
 if(!directory.refresh(program,pass,&profile,read_profile,error)||!second.refresh(program,pass,&profile,read_profile,error)||directory.view()->parameters.size()!=4)return 8;
 bool equal=false;if(material_equal_v4(equal,*directory.view(),*second.view(),error)||!equal)return 9;++checks;
 profile.color[0]=.8;if(!second.refresh(program,pass,&profile,read_profile,error)||material_equal_v4(equal,*directory.view(),*second.view(),error)||equal)return 10;++checks;
 const auto before=second.view();program->parameters[1].count=0;if(second.refresh(program,pass,&profile,read_profile,error)||second.view()!=before)return 11;++checks;
 std::cout<<"PASS "<<checks<<" original material compare gold and native64 hash/cache invariants\n";
}
