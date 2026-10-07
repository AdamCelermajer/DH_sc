#include "visual_skin_owner_v6.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <stdexcept>
namespace dh2::skinning {
namespace {
using resources::Library;
struct Reader {
 const resources::BresView& v;
 const std::uint8_t* at(std::uint64_t p,std::uint64_t n)const{if(!v.bytes||p>v.size||n>v.size-p)throw std::runtime_error("V6 BRES range");return v.bytes+p;}
 std::uint32_t w(std::uint64_t p)const{auto b=at(p,4);return b[0]|std::uint32_t(b[1])<<8|std::uint32_t(b[2])<<16|std::uint32_t(b[3])<<24;}
 std::string text(unsigned p)const{if(!p)throw std::runtime_error("V6 missing string");auto b=at(p,1);auto e=static_cast<const std::uint8_t*>(std::memchr(b,0,std::min<std::size_t>(v.size-p,4096)));if(!e)throw std::runtime_error("V6 string range");return {reinterpret_cast<const char*>(b),std::size_t(e-b)};}
 std::string field(std::uint64_t p)const{return text(w(p));}
 void array(unsigned p,unsigned n,unsigned stride)const{if(n>100000)throw std::runtime_error("V6 excessive array");at(p,std::uint64_t(n)*stride);}
 unsigned find(Library l,const std::string& uri)const{if(uri.empty()||uri[0]!='#')throw std::runtime_error("V6 external URI requires source resolver");for(unsigned i=0;i<dh2_bres_library_count(&v,l);++i){auto b=dh2_bres_library_item(&v,l,i);if(field(b-v.bytes+(l==Library::controller?4:0))==uri.substr(1))return i;}throw std::runtime_error("V6 unresolved URI: "+uri);}
};
VisualGeometryV6 geometry(const resources::BresView& v,unsigned id){
 assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&v,id)!=assets::Error::ok)throw std::runtime_error("V6 mesh decode failed");VisualGeometryV6 g;g.id=mesh.id;std::copy(mesh.minimum,mesh.minimum+3,g.minimum);std::copy(mesh.maximum,mesh.maximum+3,g.maximum);
 if(mesh.vertices>1000000||mesh.attributes>256||mesh.primitives>4096)throw std::runtime_error("V6 mesh capacity");
 for(unsigned i=0;i<mesh.attributes;++i){assets::Attribute a{};if(dh2_mesh_attribute(&mesh,i,&a)!=assets::Error::ok||!a.components||a.components>4)throw std::runtime_error("V6 unsupported attribute");VisualAttributeV6 out;out.type=a.type;out.components=a.components;out.values.resize(std::size_t(mesh.vertices)*a.components);
  for(unsigned j=0;j<mesh.vertices;++j){float x[4]{};if(!dh2_attribute_read(&a,j,x))throw std::runtime_error("V6 attribute read");for(unsigned k=0;k<a.components;++k){if(!std::isfinite(x[k]))throw std::runtime_error("V6 nonfinite attribute");out.values[std::size_t(j)*a.components+k]=x[k];}}g.attributes.push_back(std::move(out));
 }
 std::int32_t positions=-1;
 for(unsigned i=0;i<mesh.primitives;++i){assets::Primitive p{};if(dh2_mesh_primitive(&mesh,i,&p)!=assets::Error::ok)throw std::runtime_error("V6 primitive decode");VisualPrimitiveV6 out;out.material_symbol=p.material?p.material:"";out.collada_type=p.collada_type;out.engine_type=p.engine_type;std::copy(p.attributes,p.attributes+18,out.attributes.begin());if(positions==-1)positions=p.attributes[0];else if(positions!=p.attributes[0])throw std::runtime_error("V6 different primitive position streams");out.indices.resize(p.index_count);for(unsigned j=0;j<p.index_count;++j)if(!dh2_index_read(&p,j,&out.indices[j])||out.indices[j]>=mesh.vertices)throw std::runtime_error("V6 primitive index range");g.primitives.push_back(std::move(out));}
 if(positions<0||std::size_t(positions)>=g.attributes.size()||g.attributes[positions].components<3)throw std::runtime_error("V6 missing position stream");const auto& a=g.attributes[positions];g.positions.resize(mesh.vertices);for(unsigned j=0;j<mesh.vertices;++j)std::copy_n(a.values.data()+std::size_t(j)*a.components,3,g.positions[j].begin());return g;
}
std::array<float,16> identity_matrix(){return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};}
}
struct VisualSkinResourcesV6::Storage {std::vector<std::uint8_t> bytes;scene::Scene factory;std::vector<VisualCategoryResourceV6> categories;std::uint32_t node{};};
const scene::Scene& VisualSkinResourcesV6::Borrow::factory_scene()const{if(!storage_)throw std::logic_error("V6 empty resources");return storage_->factory;}
const std::vector<VisualCategoryResourceV6>& VisualSkinResourcesV6::Borrow::categories()const{if(!storage_)throw std::logic_error("V6 empty resources");return storage_->categories;}
const std::vector<std::uint8_t>& VisualSkinResourcesV6::Borrow::bytes()const{if(!storage_)throw std::logic_error("V6 empty resources");return storage_->bytes;}
std::uint32_t VisualSkinResourcesV6::Borrow::modular_node()const{if(!storage_)throw std::logic_error("V6 empty resources");return storage_->node;}
bool VisualSkinResourcesV6::load(const std::vector<std::uint8_t>& bytes,std::string& error){
 error.clear();if(storage_&&storage_.use_count()>1){error="V6 reload while resources borrowed";return false;}
 try{auto s=std::make_shared<Storage>();s->bytes=bytes;resources::BresView v{};if(dh2_bres_open(&v,s->bytes.data(),s->bytes.size())!=resources::BresError::ok)throw std::runtime_error("V6 invalid BRES");if(!scene::load(v,s->factory,error))throw std::runtime_error(error);Reader r{v};std::vector<unsigned> stack,path;unsigned modular=0;std::string modular_id;
  std::function<void(unsigned)> node=[&](unsigned p){r.at(p,80);if(path.size()>64||std::find(path.begin(),path.end(),p)!=path.end())throw std::runtime_error("V6 node cycle");path.push_back(p);const auto count=r.w(p+64),base=r.w(p+68);r.array(base,count,8);for(unsigned i=0;i<count;++i){auto a=base+8*i;if(r.w(a)==13){if(modular)throw std::runtime_error("V6 multiple modular instances unsupported");modular=r.w(a+4);modular_id=r.field(p);}}const auto n=r.w(p+56),b=r.w(p+60);r.array(b,n,80);for(unsigned i=0;i<n;++i)node(b+80*i);path.pop_back();};
  const auto n=r.w(v.root_offset+152),b=r.w(v.root_offset+156);r.array(b,n,16);for(unsigned i=0;i<n;++i){auto vs=b+16*i;auto nn=r.w(vs+8),bb=r.w(vs+12);r.array(bb,nn,80);for(unsigned j=0;j<nn;++j)node(bb+80*j);}if(!modular)throw std::runtime_error("V6 no source modular instance");r.at(modular,16);if(r.w(modular+8)||r.w(modular+12))throw std::runtime_error("V6 extra modular array requires source factory");
  auto ni=std::find_if(s->factory.graph.begin(),s->factory.graph.end(),[&](const scene::Node& a){return a.id==modular_id;});if(ni==s->factory.graph.end())throw std::runtime_error("V6 modular node unresolved");s->node=unsigned(ni-s->factory.graph.begin());auto cn=r.w(modular),cb=r.w(modular+4);if(!cn||cn>4096)throw std::runtime_error("V6 category count");r.array(cb,cn,16);
  for(unsigned i=0;i<cn;++i){auto c=cb+16*i;VisualCategoryResourceV6 category;category.name=r.field(c);category.default_uri=r.field(c+4);const auto mn=r.w(c+8),mb=r.w(c+12);r.array(mb,mn,8);for(unsigned j=0;j<mn;++j){auto m=mb+8*j;if(r.w(m)!=2)throw std::runtime_error("V6 module is not source controller instance");auto a=r.w(m+4);r.at(a,24);if(r.w(a))throw std::runtime_error("V6 external controller requires resolver");VisualModuleResourceV6 module;module.uri=r.field(a+4);module.descriptor_offset=a;auto ci=r.find(Library::controller,module.uri);if(!skinning::load(v,ci,s->factory,module.part.skin,error))throw std::runtime_error(error);module.part.geometry=geometry(v,module.part.skin.geometry);const auto bn=r.w(a+12),bb=r.w(a+16);r.array(bb,bn,60);for(unsigned k=0;k<bn;++k){auto binding=bb+60*k;if(r.w(binding))throw std::runtime_error("V6 external material requires resolver");module.part.materials.push_back(r.find(Library::material,r.field(binding+4)));}if(module.part.materials.size()!=module.part.geometry.primitives.size())throw std::runtime_error("V6 material/primitive binding mismatch");category.modules.push_back(std::move(module));}s->categories.push_back(std::move(category));}
  storage_=std::move(s);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
struct VisualSkinOwnerV6::Impl {
 struct Runtime {std::uint32_t refs{1};const VisualModuleResourceV6* module{};std::string uri;std::vector<std::uint8_t> bytes;scene::Scene weapon;std::vector<VisualGeometryV6> geometry;std::int32_t parent{-1};SkinPoseCacheV32 pose;};
 VisualSkinResourcesV6::Borrow resources;const scene::Scene* live;VisualAssetServicesV6 assets;std::vector<std::vector<VisualModuleV6>> modules;std::vector<VisualCategoryV6> categories;std::vector<VisualCellV6> cells;VisualSelectionV6 selection{};std::map<std::uintptr_t,std::shared_ptr<Runtime>> records;std::vector<std::pair<unsigned,std::uintptr_t>> draw_modules;std::vector<VisualDrawViewV32> views;bool dirty{},initialized{};std::string error;data::GearSkinServicesV5 debug{};VisualSkinOwnerV6* owner;
 Impl(VisualSkinResourcesV6::Borrow b,const scene::Scene& scene,VisualAssetServicesV6 a,VisualSkinOwnerV6* o):resources(std::move(b)),live(&scene),assets(a),owner(o){if(!resources)return;modules.resize(resources.categories().size());for(unsigned i=0;i<modules.size();++i)for(const auto& m:resources.categories()[i].modules)modules[i].push_back({m.uri.c_str(),reinterpret_cast<std::uintptr_t>(&m)});for(unsigned i=0;i<modules.size();++i){const auto& c=resources.categories()[i];categories.push_back({c.name.c_str(),c.default_uri.c_str(),modules[i].data(),std::uint32_t(modules[i].size()),0});cells.push_back({-1,0,0});}selection={categories.data(),cells.data(),std::uint32_t(cells.size()),0,{0,0},reinterpret_cast<std::uintptr_t>(live)};}
 bool graph()const{if(!resources||live->graph.size()!=resources.factory_scene().graph.size())return false;for(unsigned i=0;i<live->graph.size();++i)if(live->graph[i].id!=resources.factory_scene().graph[i].id||live->graph[i].sid!=resources.factory_scene().graph[i].sid||live->graph[i].parent!=resources.factory_scene().graph[i].parent)return false;return true;}
 std::uintptr_t add(std::unique_ptr<Runtime> r){auto id=reinterpret_cast<std::uintptr_t>(r.get());records.emplace(id,std::shared_ptr<Runtime>(std::move(r)));return id;}
 void release(std::uintptr_t id){auto at=records.find(id);if(at==records.end()||!at->second->refs)throw std::runtime_error("V6 invalid retained resource");if(!--at->second->refs)records.erase(at);}
 static bool invoke(void* context,VisualSelectionV6&,const VisualRequestV6& q,std::uintptr_t* result){auto& s=*static_cast<Impl*>(context);try{switch(q.operation){
  case VisualOperationV6::construct_module:{auto r=std::make_unique<Runtime>();r->module=reinterpret_cast<const VisualModuleResourceV6*>(q.descriptor);*result=s.add(std::move(r));break;}
  case VisualOperationV6::retain:{auto at=s.records.find(q.resource);if(at==s.records.end())throw std::runtime_error("V6 retain missing resource");++at->second->refs;break;}
  case VisualOperationV6::release:s.release(q.resource);break;
  case VisualOperationV6::update_buffers:{std::vector<std::pair<unsigned,std::uintptr_t>> parts;for(unsigned i=0;i<s.cells.size();++i)if(s.cells[i].resource){if(!s.records.count(s.cells[i].resource))throw std::runtime_error("V6 selected graph resource missing");parts.push_back({i,s.cells[i].resource});}s.draw_modules=std::move(parts);break;} // native retained part graph; no original GPU packing claim
  case VisualOperationV6::visibility:s.dirty=true;break;
  case VisualOperationV6::construct_weapon:{if(!s.assets.read)throw std::runtime_error("V6 weapon source file provider required");auto r=std::make_unique<Runtime>();r->uri=q.text;const auto status=s.assets.read(s.assets.context,q.text,r->bytes,s.error);if(status==VisualAssetResultV6::missing){*result=0;break;}if(status!=VisualAssetResultV6::found)throw std::runtime_error(s.error.empty()?"V6 weapon read failed":s.error);resources::BresView v{};if(dh2_bres_open(&v,r->bytes.data(),r->bytes.size())!=resources::BresError::ok)throw std::runtime_error("V6 weapon BRES invalid");if(!scene::load(v,r->weapon,s.error))throw std::runtime_error(s.error);if(dh2_bres_library_count(&v,Library::controller))throw std::runtime_error("V6 skinned weapon needs source constructor");for(const auto& i:r->weapon.instances)r->geometry.push_back(geometry(v,i.geometry));*result=s.add(std::move(r));break;}
  case VisualOperationV6::detach:{auto at=s.records.find(q.resource);if(at==s.records.end())throw std::runtime_error("V6 detach missing resource");if(at->second->parent>=0){at->second->parent=-1;s.release(q.resource);}break;}
  case VisualOperationV6::search:*result=0;for(unsigned i=0;i<s.live->graph.size();++i)if(s.live->graph[i].name==q.text){*result=std::uintptr_t(i)+1;break;}break;
  case VisualOperationV6::attach:if(q.resource){auto at=s.records.find(q.resource);if(at==s.records.end()||!q.descriptor||q.descriptor>s.live->graph.size())throw std::runtime_error("V6 attach invalid resource");if(at->second->parent>=0){at->second->parent=-1;s.release(q.resource);}++at->second->refs;at->second->parent=std::int32_t(q.descriptor-1);}break;
 }return true;}catch(const std::exception& e){s.error=e.what();return false;}}
 VisualServicesV6 services(){return {this,invoke};}
};
VisualSkinOwnerV6::VisualSkinOwnerV6(VisualSkinResourcesV6::Borrow b,const scene::Scene& scene,VisualAssetServicesV6 a):impl_(std::make_unique<Impl>(std::move(b),scene,a,this)){}
VisualSkinOwnerV6::~VisualSkinOwnerV6()=default;
bool VisualSkinOwnerV6::initialize(std::string& e){e.clear();auto& s=*impl_;if(s.initialized||!s.graph()){e="V6 initialized or incompatible live graph";return false;}auto v=s.services();for(unsigned i=0;i<s.categories.size();++i){auto module=dh2_visual_module_uri_v6(&s.selection,s.categories[i].default_uri);if(dh2_visual_set_category_v6(&s.selection,i,module,0,&v)){e=s.error;return false;}}VisualRequestV6 q{VisualOperationV6::update_buffers,-1,-1,0,1,0,0,nullptr};std::uintptr_t result=0;if(!Impl::invoke(&s,s.selection,q,&result)){e=s.error;return false;}s.initialized=true;return true;}
std::int32_t VisualSkinOwnerV6::category_id(const char* name)const{return dh2_visual_category_v6(&impl_->selection,name);}
std::int32_t VisualSkinOwnerV6::module_id(std::int32_t category,const char* name)const{return dh2_visual_module_v6(&impl_->selection,category,name);}
bool VisualSkinOwnerV6::set_modular(std::int32_t c,std::int32_t m,std::string& e){e.clear();auto& s=*impl_;if(!s.initialized||!s.graph()){e="V6 uninitialized or changed graph";return false;}auto v=s.services();auto status=dh2_visual_set_modular_v6(&s.selection,c,m,&v);if(status){e=s.error.empty()?"V6 modular request invalid":s.error;return false;}return true;}
bool VisualSkinOwnerV6::set_weapon(const char* name,std::int32_t slot,std::int32_t mode,std::string& e){e.clear();auto& s=*impl_;if(!s.initialized||!s.graph()){e="V6 uninitialized or changed graph";return false;}auto v=s.services();auto status=dh2_visual_set_weapon_v6(&s.selection,name,slot,mode,&v);if(status){e=s.error.empty()?"V6 weapon request invalid":s.error;return false;}return true;}
std::int32_t VisualSkinOwnerV6::current_module(std::uint32_t c)const{return c<impl_->cells.size()?impl_->cells[c].id:-1;}
std::string VisualSkinOwnerV6::weapon_uri(std::int32_t slot)const{auto id=impl_->selection.weapons[slot==1?0:1];return id?impl_->records.at(id)->uri:std::string{};}
bool VisualSkinOwnerV6::visibility_dirty()const noexcept{return impl_->dirty;}
void VisualSkinOwnerV6::clear_visibility_dirty()noexcept{impl_->dirty=false;}
bool VisualSkinOwnerV6::draw_parts(std::vector<VisualDrawPartV6>& out,std::string& e)const{e.clear();auto& s=*impl_;if(!s.initialized||!s.graph()){e="V6 draw graph invalid";return false;}try{std::vector<VisualDrawPartV6> parts;for(auto entry:s.draw_modules){auto c=entry.first;const auto& record=*s.records.at(entry.second);auto& p=record.module->part;VisualDrawPartV6 d;d.retention=s.resources.storage_;d.geometry=&p.geometry;d.material_table=&s.resources.factory_scene().materials;d.materials=&p.materials;d.world=identity_matrix();d.skinned=true;d.category=c;d.module=s.cells[c].id;std::vector<Matrix> palette;if(!skinning::palette(p.skin,*s.live,palette,e)||!skinning::positions(p.skin,palette,p.geometry.positions,d.positions,e))return false;parts.push_back(std::move(d));}
  for(unsigned slot=0;slot<2;++slot){auto id=s.selection.weapons[slot];if(!id)continue;const auto& r=*s.records.at(id);if(r.parent<0)continue;for(unsigned i=0;i<r.weapon.instances.size();++i){auto& instance=r.weapon.instances[i];VisualDrawPartV6 d;d.retention=s.records.at(id);d.geometry=&r.geometry[i];d.material_table=&r.weapon.materials;d.materials=&instance.materials;d.positions=d.geometry->positions;d.world=scene::multiply(s.live->graph[r.parent].world,instance.world);for(float x:d.world)if(!std::isfinite(x))throw std::runtime_error("V6 weapon transform overflow");d.weapon_slot=slot?2:1;parts.push_back(std::move(d));}}
  out=std::move(parts);return true;}catch(const std::exception& x){e=x.what();return false;}}

bool VisualSkinOwnerV6::draw_views(const std::vector<VisualDrawViewV32>*& out,std::string& e)const{
 out=nullptr;e.clear();auto& s=*impl_;
 if(!s.initialized||!s.graph()){e="V6 draw graph invalid";return false;}
 try{
  s.views.clear();
  for(auto entry:s.draw_modules){
   auto& record=*s.records.at(entry.second);const auto& p=record.module->part;
   record.pose.bind(p.skin,p.geometry.positions);
   if(!record.pose.sample(*s.live,e))return false;
   VisualDrawViewV32 d;d.retention=s.resources.storage_;d.geometry=&p.geometry;
   d.material_table=&s.resources.factory_scene().materials;d.materials=&p.materials;
   d.positions=&record.pose.positions();d.world=identity_matrix();d.skinned=true;
   d.positions_changed=record.pose.changed_last_call();d.pose_revision=record.pose.revision();
   d.category=entry.first;d.module=s.cells[entry.first].id;s.views.push_back(std::move(d));
  }
  for(unsigned slot=0;slot<2;++slot){
   auto id=s.selection.weapons[slot];if(!id)continue;
   const auto& r=*s.records.at(id);if(r.parent<0)continue;
   for(unsigned i=0;i<r.weapon.instances.size();++i){
    const auto& instance=r.weapon.instances[i];VisualDrawViewV32 d;
    d.retention=s.records.at(id);d.geometry=&r.geometry[i];d.material_table=&r.weapon.materials;
    d.materials=&instance.materials;d.positions=&d.geometry->positions;
    d.world=scene::multiply(s.live->graph[r.parent].world,instance.world);
    for(float x:d.world)if(!std::isfinite(x))throw std::runtime_error("V6 weapon transform overflow");
    d.weapon_slot=slot?2:1;s.views.push_back(std::move(d));
   }
  }
  out=&s.views;return true;
 }catch(const std::exception& x){e=x.what();return false;}
}
SkinPoseCountersV32 VisualSkinOwnerV6::pose_counters()const noexcept{
 SkinPoseCountersV32 out;
 for(const auto& entry:impl_->records){const auto& c=entry.second->pose.counters();
  out.calls+=c.calls;out.hits+=c.hits;out.deformations+=c.deformations;
  out.joint_checks+=c.joint_checks;out.vertices+=c.vertices;out.storage_growths+=c.storage_growths;
 }
 return out;
}
data::GearSkinServicesV5 VisualSkinOwnerV6::gear_services(data::GearSkinServicesV5 debug){impl_->debug=debug;return {impl_.get(),[](void* p,data::FreshInventoryOwnedV4& inv,const data::GearSkinRequestV5& q,std::int32_t& result,std::string& e){auto& s=*static_cast<Impl*>(p);using O=data::GearSkinOperationV5;if(q.operation==O::debug_load||q.operation==O::debug_query){if(!s.debug.invoke){e="V6 genuine Debug provider required";return false;}return s.debug.invoke(s.debug.context,inv,q,result,e);}if(q.visual!=s.owner->identity()){e="V6 visual identity mismatch";return false;}switch(q.operation){case O::category_id:result=s.owner->category_id(q.name);return true;case O::module_id:result=s.owner->module_id(q.category,q.name);return true;case O::set_modular:return s.owner->set_modular(q.category,q.module,e);case O::set_weapon:return s.owner->set_weapon(q.name,q.slot,q.mode,e);default:e="V6 unknown Skin operation";return false;}}};}
}
