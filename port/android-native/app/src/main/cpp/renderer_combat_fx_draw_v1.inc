// Retained GPU presentation only; gameplay FX remain owned by the source manager.
std::vector<Draw> combat_fx_draws;
std::vector<GLuint> combat_fx_images;
std::vector<dh2::skinning::VisualDrawPartV6> combat_fx_parts;
std::vector<std::size_t> combat_fx_draw_part;
void sync_combat_fx_draws(std::vector<dh2::skinning::VisualDrawPartV6> parts,
 AAssetManager* assets,dh2::android_ui::OriginalCacheAssetsV1& original){
  bool rebuild=parts.size()!=combat_fx_parts.size();
  for(std::size_t i=0;i<parts.size();++i){const auto& part=parts[i];
    if(!part.retention||!part.geometry||!part.material_table||!part.materials||
       part.positions.size()!=part.geometry->positions.size()||part.positions.empty())
      throw std::runtime_error("Combat FX draw snapshot backing is invalid");
    if(!rebuild){const auto& old=combat_fx_parts[i];
      rebuild=part.geometry!=old.geometry||part.material_table!=old.material_table||
        part.materials!=old.materials||part.category!=old.category||part.module!=old.module||
        part.weapon_slot!=old.weapon_slot;
    }
  }
  if(rebuild){
    std::vector<Draw> next;std::vector<GLuint> textures;
    std::vector<std::size_t> mapping;std::map<std::string,GLuint> cache;
    try{
      std::size_t total=0;
      for(std::size_t i=0;i<parts.size();++i){const auto& part=parts[i];const auto& geometry=*part.geometry;
        if(geometry.primitives.size()!=part.materials->size()||geometry.positions.size()>65536)
          throw std::runtime_error("Combat FX material/topology count is invalid");
        for(std::size_t j=0;j<geometry.primitives.size();++j){const auto& primitive=geometry.primitives[j];
          if(primitive.collada_type||primitive.indices.size()%3||primitive.indices.size()>3000000||
             total>1000000-geometry.positions.size())
            throw std::runtime_error("Combat FX triangle/vertex budget exceeded");
          total+=geometry.positions.size();
          const auto material=part.materials->at(j);
          if(material>=part.material_table->size()||part.material_table->at(material).id!=primitive.material_symbol)
            throw std::runtime_error("Combat FX primitive material binding differs");
          auto attribute=[&](unsigned slot)->const dh2::skinning::VisualAttributeV6*{
            const auto index=primitive.attributes.at(slot);
            if(index<0)return nullptr;
            if(std::size_t(index)>=geometry.attributes.size())throw std::runtime_error("Combat FX attribute index exceeds geometry");
            const auto& value=geometry.attributes[index];
            if(!value.components||value.components>4||value.values.size()!=geometry.positions.size()*value.components)
              throw std::runtime_error("Combat FX attribute stream is incomplete");
            return &value;
          };
          const auto* uv=attribute(4);const auto* color_stream=attribute(2);
          if(uv&&uv->components<2)throw std::runtime_error("Combat FX UV stream has fewer than two components");
          std::vector<Vertex> vertices(geometry.positions.size());
          for(std::size_t k=0;k<vertices.size();++k){auto& vertex=vertices[k];
            std::copy(part.positions[k].begin(),part.positions[k].end(),vertex.p);
            if(uv)std::copy_n(uv->values.data()+k*uv->components,2,vertex.uv);
            std::fill(vertex.color,vertex.color+4,1.f);
            if(color_stream)for(unsigned c=0;c<color_stream->components;++c)
              vertex.color[c]=color_stream->values[k*color_stream->components+c]/(color_stream->type==1?255.f:1.f);
          }
          std::vector<std::uint16_t> indices;indices.reserve(primitive.indices.size());
          for(auto index:primitive.indices){if(index>=vertices.size()||index>65535)
              throw std::runtime_error("Combat FX index exceeds vertex/GLES2 bounds");
            indices.push_back(static_cast<std::uint16_t>(index));}
          next.emplace_back();auto& draw=next.back();draw.material=part.material_table->at(material);
          draw.placement=part.world;draw.environment=true;draw.count=static_cast<GLsizei>(indices.size());
          draw.cpu_vertices=std::move(vertices);mapping.push_back(i);
          draw.diffuse=upload(assets,draw.material.diffuse,cache,textures,&original);
          draw.alpha=upload(assets,draw.material.alpha_map,cache,textures,&original);
          glGenBuffers(1,&draw.vertices);glBindBuffer(GL_ARRAY_BUFFER,draw.vertices);
          glBufferData(GL_ARRAY_BUFFER,draw.cpu_vertices.size()*sizeof(Vertex),draw.cpu_vertices.data(),GL_DYNAMIC_DRAW);
          glGenBuffers(1,&draw.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,draw.indices);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(std::uint16_t),indices.data(),GL_STATIC_DRAW);
          check("Combat FX source buffer upload");
        }
      }
    }catch(...){release(next,textures);throw;}
    release(combat_fx_draws,combat_fx_images);combat_fx_draws=std::move(next);
    combat_fx_images=std::move(textures);combat_fx_draw_part=std::move(mapping);
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Combat FX GPU graph rebuilt | parts %zu | draws %zu | textures %zu | live source snapshot",
      parts.size(),combat_fx_draws.size(),combat_fx_images.size());
  }else{
    for(std::size_t i=0;i<combat_fx_draws.size();++i){auto& draw=combat_fx_draws[i];const auto& part=parts.at(combat_fx_draw_part.at(i));
      if(draw.cpu_vertices.size()!=part.positions.size())throw std::runtime_error("Combat FX pose size differs from retained mesh");
      for(std::size_t k=0;k<part.positions.size();++k)std::copy(part.positions[k].begin(),part.positions[k].end(),draw.cpu_vertices[k].p);
      draw.placement=part.world;glBindBuffer(GL_ARRAY_BUFFER,draw.vertices);
      glBufferSubData(GL_ARRAY_BUFFER,0,draw.cpu_vertices.size()*sizeof(Vertex),draw.cpu_vertices.data());
    }
  }
  combat_fx_parts=std::move(parts);
}
