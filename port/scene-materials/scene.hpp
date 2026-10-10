#pragma once
#include "../asset-payloads/payloads.hpp"
#include <array>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace dh2::scene {
struct Material {
    std::string id, diffuse, alpha_map;
    std::string effect_file,effect_uri,gles2_technique;
    float color[4]{1,1,1,1};
    float texture_matrix[16]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    float alpha_ref=0;
    bool additive=false, backface=false;
};
// A COLLADA instance_material is keyed by the geometry's primitive symbol,
// then points at a material record whose ID can be different from that symbol.
struct InstanceMaterialBindingV1 {
    std::string symbol;
    Material target;
};
struct InstanceMaterialSymbolV1 {std::string symbol;};
// Actual per-node/per-mesh source storage. A retained child can own these
// cells independently; scene vectors are views over the SAME live cells.
struct VisibilityV91 {std::uint8_t local120{1},parent121{1};};
struct MeshFieldsV91 {
 std::array<float,3> position{0,0,0};
 std::array<float,4> quaternion{0,0,0,1};
 std::array<float,3> scale{1,1,1};
 std::uint32_t flags{0x721u};VisibilityV91 visibility;
};
struct InstanceStorageV91 {
 std::string node;std::uint32_t node_index{},geometry{};
 std::array<float,16> world{};std::vector<std::uint32_t> materials;std::vector<InstanceMaterialSymbolV1> material_symbols_v1;std::int32_t controller{-1};
 MeshFieldsV91 mesh_fields;std::uint8_t detached{};
 std::string native_name24;std::uintptr_t parentec{}; // Actual IMeshSceneNode C1-empty name and parent.
};
struct Instance {
private:std::shared_ptr<InstanceStorageV91> storage_;
 explicit Instance(std::shared_ptr<InstanceStorageV91> p):storage_(std::move(p)),node(storage_->node),node_index(storage_->node_index),
  geometry(storage_->geometry),world(storage_->world),materials(storage_->materials),material_symbols_v1(storage_->material_symbols_v1),controller(storage_->controller){}
public:
 std::string& node;std::uint32_t& node_index;std::uint32_t& geometry;
 std::array<float,16>& world;std::vector<std::uint32_t>& materials;std::vector<InstanceMaterialSymbolV1>& material_symbols_v1;std::int32_t& controller;
 Instance():Instance(std::make_shared<InstanceStorageV91>()){}
 Instance(std::string name,std::uint32_t index,std::uint32_t mesh,std::array<float,16> matrix,
  std::vector<std::uint32_t> slots,std::int32_t skin=-1):Instance(){node=std::move(name);node_index=index;geometry=mesh;world=matrix;materials=std::move(slots);controller=skin;}
 Instance(const Instance& v):Instance(std::make_shared<InstanceStorageV91>(*v.storage_)){}
 Instance(Instance&& v)noexcept:Instance(v.storage_){}
 Instance& operator=(const Instance& v){if(this!=&v&&storage_.get()!=v.storage_.get())*storage_=*v.storage_;return *this;}
 Instance& operator=(Instance&& v)noexcept{if(this!=&v&&storage_.get()!=v.storage_.get())*storage_=std::move(*v.storage_);return *this;}
 InstanceStorageV91& source_storage_v91()noexcept{return *storage_;}
 const InstanceStorageV91& source_storage_v91()const noexcept{return *storage_;}
 std::shared_ptr<InstanceStorageV91> source_owner_v91()const noexcept{return storage_;}
};
struct NodeStorageV91 {
 std::string id,sid,name,user_properties;std::int32_t parent{-1};
 float translation[3]{},quaternion[4]{0,0,0,1},scale[3]{1,1,1};
 std::array<float,16> world{};std::uint32_t flags{0x60fu};VisibilityV91 visibility;std::uint8_t detached{};
};
struct Node {
private:std::shared_ptr<NodeStorageV91> storage_;
 explicit Node(std::shared_ptr<NodeStorageV91> p):storage_(std::move(p)),id(storage_->id),sid(storage_->sid),name(storage_->name),
  user_properties(storage_->user_properties),parent(storage_->parent),translation(storage_->translation),
  quaternion(storage_->quaternion),scale(storage_->scale),world(storage_->world){}
public:
 std::string& id;std::string& sid;std::string& name;std::string& user_properties;std::int32_t& parent;
 float (&translation)[3];float (&quaternion)[4];float (&scale)[3];std::array<float,16>& world;
 Node():Node(std::make_shared<NodeStorageV91>()){}
 Node(const Node& v):Node(std::make_shared<NodeStorageV91>(*v.storage_)){}
 Node(Node&& v)noexcept:Node(v.storage_){}
 Node& operator=(const Node& v){if(this!=&v&&storage_.get()!=v.storage_.get())*storage_=*v.storage_;return *this;}
 Node& operator=(Node&& v)noexcept{if(this!=&v&&storage_.get()!=v.storage_.get())*storage_=std::move(*v.storage_);return *this;}
 NodeStorageV91& source_storage_v91()noexcept{return *storage_;}
 const NodeStorageV91& source_storage_v91()const noexcept{return *storage_;}
 std::shared_ptr<NodeStorageV91> source_owner_v91()const noexcept{return storage_;}
};
// Nonowning vector interface used by existing source control. It never owns
// another flag/visibility/detach array; operator[] lends each node's actual cell.
template<class Element,class T,auto Member>class SourceFieldViewV91 {
 std::vector<Element>* elements_{};
public:
 void bind(std::vector<Element>& v)noexcept{elements_=&v;}
 std::size_t size()const noexcept{return elements_?elements_->size():0;}
 void clear()noexcept{elements_=nullptr;}
 void assign(std::size_t n,const T& value){if(n!=size())throw std::logic_error("Source field view domain");for(std::size_t i=0;i<n;++i)(*this)[i]=value;}
 T& operator[](std::size_t i){return (*elements_)[i].source_storage_v91().*Member;}
 const T& operator[](std::size_t i)const{return (*elements_)[i].source_storage_v91().*Member;}
 template<bool Const>struct Iterator {
  using View=std::conditional_t<Const,const SourceFieldViewV91,SourceFieldViewV91>;
  View* view{};std::size_t index{};
  decltype(auto) operator*()const{return (*view)[index];}
  Iterator& operator++(){++index;return *this;}
  bool operator!=(const Iterator& other)const noexcept{return index!=other.index||view!=other.view;}
 };
 Iterator<false> begin(){return {this,0};}Iterator<false> end(){return {this,size()};}
 Iterator<true> begin()const{return {this,0};}Iterator<true> end()const{return {this,size()};}
};
using NodeFlagsViewV91=SourceFieldViewV91<Node,std::uint32_t,&NodeStorageV91::flags>;
using NodeVisibilityViewV91=SourceFieldViewV91<Node,VisibilityV91,&NodeStorageV91::visibility>;
using NodeDetachViewV91=SourceFieldViewV91<Node,std::uint8_t,&NodeStorageV91::detached>;
using MeshFieldsViewV91=SourceFieldViewV91<Instance,MeshFieldsV91,&InstanceStorageV91::mesh_fields>;
using MeshDetachViewV91=SourceFieldViewV91<Instance,std::uint8_t,&InstanceStorageV91::detached>;

struct Scene {
    // Parsed COLLADA camera instances. These preserve the authored camera
    // record attached to a node; they do not register a native SceneManager camera.
    struct CameraInstanceV1 {
        std::uint32_t node_index{},camera{},kind{};
        std::string id,target_uri;
        float horizontal_fov_or_mag{},aspect{},znear{},zfar{};
    };
    // Parsed COLLADA SLight data attached to a node. `color` follows the
    // source importer's authored-byte * (float intensity / 255) calculation.
    // `parameters` holds three attenuation values for point lights and those
    // values plus cutoff/exponent for spot lights.
    struct LightInstanceV113 {
        std::uint32_t node_index{},light{},type{};
        std::string id;
        std::array<std::uint8_t,4> authored_color{};
        float intensity{},color[4]{};
        std::array<float,5> parameters{};
        std::uint8_t parameter_count{};
    };
    std::vector<Material> materials;
    std::vector<Instance> instances;
    std::vector<CameraInstanceV1> cameras_v1;
    std::vector<LightInstanceV113> lights_v113;
    std::vector<Node> graph;
    unsigned nodes=0, ignored_instances=0;
};
// Authored visibility is separate from mesh membership. The generic native
// visual constructs hidden geometry too; existing visible-only consumers keep
// their original load domain and do not change Character/camera semantics.
struct AuthoredVisibilityV76 {
 std::vector<std::uint8_t> node_local,node_effective,mesh_local,mesh_effective;
};
bool load_authored_v76(const resources::BresView&,Scene&,AuthoredVisibilityV76&,std::string& error);
// Checked reconstruction of serialized node, image and material links.
// Owns strings and matrices, borrows no file bytes. Does not create the
// original engine's shaders, lights, animation or runtime object ABI.
bool load(const resources::BresView&, Scene&, std::string& error);
bool update_world(Scene&,std::string& error);
std::array<float,16> multiply(const std::array<float,16>&, const std::array<float,16>&);
}
// Pure matrix entry point for original-instruction differential validation.
extern "C" void dh2_node_matrix(float* out16,const float* translation3,const float* quaternion4,const float* scale3);
