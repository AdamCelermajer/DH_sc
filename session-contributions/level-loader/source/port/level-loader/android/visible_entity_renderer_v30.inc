// Authored inspection bridge. Draw original resource hierarchy and native CPU
// skinning at the captured pose; do not imply gameplay InitPost/activation.
void append_entity_geometry(Candidate& next,const dh2::loader::XmlElementV1& entry,std::map<std::string,GLuint>& images){
 using namespace dh2;
 auto get=[&](const char* key)->const std::string&{auto value=entry.attribute(key);require(value,"Entity manifest missing "+std::string(key));return *value;};
 loader::VisualTransformV1 pose;
 auto point=[&](const char* key,Point& result){const char* at=get(key).c_str();for(auto& value:result){char* end{};value=std::strtof(at,&end);require(end!=at&&std::isfinite(value),"Invalid entity pose");at=*end==','?end+1:end;}};
 point("position",pose.position);point("rotation",pose.rotation_degrees);point("scale",pose.scale);std::string error;require(loader::project_visual_transform_v1(pose,error),error);
 Matrix placement{};dh2_node_matrix(placement.data(),pose.position.data(),pose.quaternion.data(),pose.scale.data());
 bool found{};std::vector<std::uint8_t> bytes;require(pack.read(get("asset"),found,bytes,error)&&found,"Entity resource missing: "+get("asset")+" "+error);
 resources::BresView bres{};require(dh2_bres_open(&bres,bytes.data(),bytes.size())==resources::BresError::ok,"Entity BRES rejected");
 scene::Scene scene;const bool scene_ready=scene::load(bres,scene,error);
 if(!scene_ready&&error=="Scene has no visible geometry"){
  ++next.nonvisual_entities;__android_log_print(ANDROID_LOG_INFO,tag,"AUTHORED_ENTITY_NONVISUAL name=%s asset=%s original_geometries=%u original_controllers=%u original_visible_instances=0",get("name").c_str(),get("asset").c_str(),dh2_bres_library_count(&bres,resources::Library::geometry),dh2_bres_library_count(&bres,resources::Library::controller));return;
 }
 require(scene_ready,get("asset")+": "+error);
 Bounds bounds;unsigned draws=0;
 for(const auto& instance:scene.instances){
  // The existing source-backed decor marker classifier identifies _colbox_
  // nodes as physical bounds. Retain them in the loaded scene, but omit their
  // debug geometry from this inspection renderer and report the omission.
  const auto& node=scene.graph.at(instance.node_index);
  if(node.name.compare(0,8,"_colbox_")==0){__android_log_print(ANDROID_LOG_INFO,tag,"INSPECTION_COLLISION_MARKER name=%s node=%s rendered=0",get("name").c_str(),node.name.c_str());continue;}
  assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&bres,instance.geometry)==assets::Error::ok,"Entity geometry rejected");
  skinning::Skin skin;std::vector<skinning::Matrix> palette;
  if(instance.controller>=0){require(skinning::load(bres,unsigned(instance.controller),scene,skin,error),error);require(skinning::palette(skin,scene,palette,error),error);}
  for(unsigned p=0;p<mesh.primitives;++p){
   assets::Primitive primitive{};require(dh2_mesh_primitive(&mesh,p,&primitive)==assets::Error::ok,"Entity primitive rejected");
   require(primitive.collada_type==0&&primitive.index_count%3==0&&mesh.vertices<=65536,"Entity topology unsupported");
   assets::Attribute positions{},uv{},color{};require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)==assets::Error::ok&&positions.components>=3,"Entity positions absent");
   bool have_uv=dh2_mesh_attribute(&mesh,primitive.attributes[4],&uv)==assets::Error::ok&&uv.components>=2;
   bool have_color=dh2_mesh_attribute(&mesh,primitive.attributes[2],&color)==assets::Error::ok;
   std::vector<Point> input(mesh.vertices),posed;
   for(unsigned i=0;i<mesh.vertices;++i){float raw[4]{};require(dh2_attribute_read(&positions,i,raw),"Entity vertex unreadable");std::copy_n(raw,3,input[i].begin());}
   Matrix world=scene::multiply(placement,instance.world);
   if(instance.controller>=0){require(skinning::positions(skin,palette,input,posed,error),error);world=placement;}else posed=std::move(input);
   std::vector<Vertex> vertices(mesh.vertices);
   for(unsigned i=0;i<mesh.vertices;++i){auto& v=vertices[i];std::copy_n(posed[i].begin(),3,v.p);Point placed{};for(unsigned r=0;r<3;++r){placed[r]=world[12+r];for(unsigned c=0;c<3;++c)placed[r]+=world[c*4+r]*v.p[c];require(std::isfinite(placed[r]),"Entity produced nonfinite vertex");}bounds.add(placed);next.bounds.add(placed);
    float raw[4]{};if(have_uv){require(dh2_attribute_read(&uv,i,raw),"Entity UV unreadable");std::copy_n(raw,2,v.uv);}if(have_color){require(color.components<=4&&dh2_attribute_read(&color,i,raw),"Entity color unreadable");for(unsigned j=0;j<color.components;++j)v.color[j]=raw[j]/(color.type==1?255.f:1.f);}
   }
   std::vector<std::uint16_t> indices(primitive.index_count);for(unsigned i=0;i<primitive.index_count;++i){std::uint32_t index{};require(dh2_index_read(&primitive,i,&index)&&index<mesh.vertices,"Entity index out of bounds");indices[i]=std::uint16_t(index);}
   require(p<instance.materials.size(),"Entity material binding absent");next.batches.emplace_back();auto& batch=next.batches.back();batch.world=world;batch.material=scene.materials.at(instance.materials[p]);batch.diffuse=upload(batch.material.diffuse,next,images);batch.alpha=upload(batch.material.alpha_map,next,images);batch.count=primitive.index_count;next.triangles+=primitive.index_count/3;
   glGenBuffers(1,&batch.vertices);glBindBuffer(GL_ARRAY_BUFFER,batch.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_STATIC_DRAW);
   glGenBuffers(1,&batch.indices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,batch.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(std::uint16_t),indices.data(),GL_STATIC_DRAW);check("entity buffers");++draws;
  }
 }
 require(draws>0,"Entity produced no draw batches: "+get("name"));next.entity_bounds.push_back(bounds);next.entity_names.push_back(get("name"));
 if(get("kind")=="Character")++next.characters;else ++next.chests;
 __android_log_print(ANDROID_LOG_INFO,tag,"AUTHORED_ENTITY_READY name=%s kind=%s asset=%s draws=%u gameplay=0",get("name").c_str(),get("kind").c_str(),get("asset").c_str(),draws);
}
void append_authored_entities(AAssetManager* manager,const std::string& identity,Candidate& next,std::map<std::string,GLuint>& images){
 const auto name="loader-objects/"+identity+".xml";auto* asset=AAssetManager_open(manager,name.c_str(),AASSET_MODE_BUFFER);if(!asset)return;
 const auto length=AAsset_getLength64(asset);std::vector<std::uint8_t> bytes(static_cast<std::size_t>(length));const auto read=AAsset_read(asset,bytes.data(),bytes.size());AAsset_close(asset);require(read==length,"Entity manifest short read");
 dh2::loader::XmlDocumentV1 document;std::string error;require(document.capture(name,std::move(bytes),error)&&document.borrow().parsed(),error);
 for(const auto& entry:document.borrow().elements()){
  if(entry.tag=="Entity")append_entity_geometry(next,entry,images);
  else if(entry.tag=="Blocked"){++next.blocked_entities;__android_log_print(ANDROID_LOG_WARN,tag,"AUTHORED_ENTITY_BLOCKED name=%s reason=%s",entry.attribute("name")->c_str(),entry.attribute("reason")->c_str());}
  else if(entry.tag=="Coverage"){if(auto type=entry.attribute("next_constructor"))next.next_constructor=*type;}
 }
}
