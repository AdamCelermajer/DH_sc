//Actual selected BRES effect/technique state; no alpha-map/additive heuristic.
struct BatchMaterialReaderV112 {
 const dh2::resources::BresView& image;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{if(!image.bytes||p>image.size||n>image.size-p)throw std::runtime_error("Batch material BRES range");return image.bytes+p;}
 std::uint32_t word(std::uint64_t p)const{std::uint32_t value;std::memcpy(&value,at(p,4),4);return value;}
 std::string text(std::uint32_t p)const{if(!p)throw std::runtime_error("Actual batch material string absent");auto begin=at(p,1);auto end=static_cast<const std::uint8_t*>(std::memchr(begin,0,image.size-p));if(!end)throw std::runtime_error("Actual batch material string unterminated");return {reinterpret_cast<const char*>(begin),reinterpret_cast<const char*>(end)};}
};
bool selected_batch_material_pass_v112(const dh2::resources::BresView& image,const std::string& uri,const std::string& technique,dh2::scene::EffectRenderPassV4& out,std::string& e)try{
 if(uri.empty()||uri[0]!='#'){e="Required actual authored batch effect URI";return false;}
 BatchMaterialReaderV112 r{image};unsigned matches{};dh2::scene::EffectRenderPassV4 next;
 for(unsigned i=0;i<dh2_bres_library_count(&image,dh2::resources::Library::effect);++i){auto item=dh2_bres_library_item(&image,dh2::resources::Library::effect,i);if(!item)throw std::runtime_error("Actual batch effect row absent");const auto p=item-image.bytes;if(r.text(r.word(p))!=uri.substr(1))continue;
  const auto count=r.word(p+32),base=r.word(p+36);r.at(base,std::uint64_t(count)*12);
  //createMaterial631d28 calls allocate5cc0a0: source SState byte0=0,
  //CMaterial C1 stores that as technique8. An authored CurrentTechnique
  //parameter subsequently selects by name; absence retains actual row0.
  for(unsigned j=0;j<count;++j){const auto t=base+j*12;if(technique.empty()?j!=0:r.text(r.word(t))!=technique)continue;if(r.word(t+4)!=1)throw std::runtime_error("Actual batch multipass material requires selected pass sequence");++matches;const auto pass=r.word(t+8);r.at(pass,116);
   next.vertex_file=r.text(r.word(pass+4));next.vertex_defines=r.text(r.word(pass+12));next.fragment_file=r.text(r.word(pass+16));next.fragment_defines=r.text(r.word(pass+24));std::memcpy(next.source.data(),r.at(pass+28,76),76);
  }
 }
 if(matches!=1)throw std::runtime_error("Actual batch effect/technique selection did not resolve uniquely");
 if(dh2_render_pass_convert_v3(&next.pass,&next.source))throw std::runtime_error("Actual batch material render-state conversion failed");
 std::uint32_t a,b;std::memcpy(&a,next.pass.data(),4);std::memcpy(&b,next.pass.data()+4,4);
 static const std::uint32_t blend[]{0,1,0x300,0x301,0x302,0x303,0x306,0x307,0x304,0x305,0x8001,0x8002,0x8003,0x8004,0x308},equation[]{0x8006,0x800a,0x800b,0x8007,0x8008},cull[]{0x405,0x404,0x408};
 const auto src=a&15,dst=(a>>4)&15,eq=(a>>24)&7,cf=a>>30;if(src>=15||dst>=15||eq>=5||cf>=3)throw std::runtime_error("Actual batch material state enum unsupported");
 next.blend_src=blend[src];next.blend_dst=blend[dst];next.blend_equation=equation[eq];next.depth_function=0x200+((a>>27)&7);next.cull_face=cull[cf];next.front_face=(b&(1u<<18))?0x900:0x901;
 next.blend=b&(1u<<16);next.cull=b&(1u<<17);next.depth=b&(1u<<19);next.depth_write=b&(1u<<20);next.stencil=b&(1u<<27);next.sample_coverage=b&(1u<<25);next.polygon_offset=b&((1u<<21)|(1u<<22)|(1u<<23));
 if(next.stencil||next.sample_coverage||next.polygon_offset)throw std::runtime_error("Actual batch auxiliary pass-state owner required");
 out=std::move(next);e.clear();return true;
}catch(const std::exception& error){e=error.what();return false;}
