#include "original_scene.hpp"
#include "../scene-materials/scene.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace dh::foundation {
namespace {
Vec3 point(const std::array<float,16>& m, const float* p) {
    return {m[0]*p[0]+m[4]*p[1]+m[8]*p[2]+m[12],
            m[1]*p[0]+m[5]*p[1]+m[9]*p[2]+m[13],
            m[2]*p[0]+m[6]*p[1]+m[10]*p[2]+m[14]};
}
Vec3 subtract(Vec3 a, Vec3 b) { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
Vec3 cross(Vec3 a, Vec3 b) {return {a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x};}
bool helper_node(const dh2::scene::Node& node) {
    // Exported roles used by fixed_map_v1's geometry classification and the
    // object-resource helper admission. These are format roles, not map names.
    const auto& name=node.name.empty()?node.id:node.name;
    for(const char* role:{"_module_","_floor","_exit","_minimap","_colbox_","_mesh_shadow_"})
        if(name.find(role)!=std::string::npos) return true;
    return false;
}
void include(OriginalScene& s, Vec3 p) {
    if(!std::isfinite(p.x)||!std::isfinite(p.y)||!std::isfinite(p.z))
        throw std::runtime_error("Nonfinite transformed scene position");
    s.minimum={std::min(s.minimum.x,p.x),std::min(s.minimum.y,p.y),std::min(s.minimum.z,p.z)};
    s.maximum={std::max(s.maximum.x,p.x),std::max(s.maximum.y,p.y),std::max(s.maximum.z,p.z)};
}
}

bool decode_original_scene_module(const std::vector<std::uint8_t>& bytes,
                                  const std::string& authoredNode,
                                  const Mat4& placement,
                                  OriginalScene& output, std::string& error) {
    try {
        dh2::resources::BresView view{};
        const auto status=dh2_bres_open(&view,bytes.data(),bytes.size());
        if(status!=dh2::resources::BresError::ok)
            throw std::runtime_error("Original BRES validation failed ("+std::to_string(static_cast<unsigned>(status))+")");
        dh2::scene::Scene authored;
        if(!dh2::scene::load(view,authored,error)) return false;
        std::int32_t selected=-1;
        if(!authoredNode.empty()) {
            for(std::size_t n=0;n<authored.graph.size();++n) {
                const auto& node=authored.graph[n];
                if(node.id==authoredNode||node.name==authoredNode||node.sid==authoredNode) {
                    if(selected>=0) throw std::runtime_error("Original module name is ambiguous: "+authoredNode);
                    selected=static_cast<std::int32_t>(n);
                }
            }
            if(selected<0) throw std::runtime_error("Original module node missing: "+authoredNode);
        }
        std::vector<Mat4> moduleWorld(authored.graph.size());
        std::size_t selectedNodes=selected>=0?1:authored.graph.size();
        if(selected>=0) {
            // Module source roots are packed around a library scene. The level
            // declaration replaces that root's TRS, retaining child locals.
            moduleWorld[selected]=placement;
            for(std::size_t n=selected+1;n<authored.graph.size();++n) {
                const auto& node=authored.graph[n];auto parent=node.parent;
                while(parent>=0&&parent!=selected) parent=authored.graph.at(parent).parent;
                if(parent!=selected) continue;
                ++selectedNodes;
                Mat4 local{};dh2_node_matrix(local.data(),node.translation,node.quaternion,node.scale);
                moduleWorld[n]=dh2::scene::multiply(moduleWorld.at(node.parent),local);
            }
        }
        OriginalScene result;
        const auto infinity=std::numeric_limits<float>::infinity();
        result.minimum={infinity,infinity,infinity}; result.maximum={-infinity,-infinity,-infinity};
        result.nodeCount=selectedNodes;
        std::size_t helpers=0;
        for(const auto& instance:authored.instances) {
            if(selected>=0) {
                auto parent=static_cast<std::int32_t>(instance.node_index);
                while(parent>=0&&parent!=selected) parent=authored.graph.at(parent).parent;
                if(parent!=selected) continue;
            }
            if(helper_node(authored.graph.at(instance.node_index))) {++helpers;continue;}
            ++result.instanceCount;
            const auto world=selected>=0?moduleWorld.at(instance.node_index):dh2::scene::multiply(placement,instance.world);
            if(instance.controller>=0) throw std::runtime_error("Static environment contains a skinned instance: "+instance.node);
            dh2::assets::Mesh mesh{};
            if(dh2_mesh_open(&mesh,&view,instance.geometry)!=dh2::assets::Error::ok)
                throw std::runtime_error("Original geometry rejected: "+instance.node);
            if(mesh.primitives!=instance.materials.size()) throw std::runtime_error("Original material binding count differs");
            for(std::uint32_t j=0;j<mesh.primitives;++j) {
                dh2::assets::Primitive primitive{};
                if(dh2_mesh_primitive(&mesh,j,&primitive)!=dh2::assets::Error::ok||primitive.collada_type!=0||primitive.index_count%3)
                    throw std::runtime_error("Environment primitive is not a checked triangle list");
                dh2::assets::Attribute positions{}, uv{}, colors{};
                if(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)!=dh2::assets::Error::ok||positions.components<3)
                    throw std::runtime_error("Original geometry has no valid position stream");
                const bool hasUv=dh2_mesh_attribute(&mesh,primitive.attributes[4],&uv)==dh2::assets::Error::ok&&uv.components>=2;
                const bool hasColors=dh2_mesh_attribute(&mesh,primitive.attributes[2],&colors)==dh2::assets::Error::ok&&colors.components<=4;
                if(result.mesh.vertices.size()+mesh.vertices>std::numeric_limits<std::uint32_t>::max())
                    throw std::runtime_error("Scene exceeds 32-bit vertex domain");
                const auto base=static_cast<std::uint32_t>(result.mesh.vertices.size());
                const auto& original=authored.materials.at(instance.materials[j]);
                DrawRange range; range.firstIndex=result.mesh.indices.size(); range.indexCount=primitive.index_count;
                std::copy(original.color,original.color+4,range.material.color.begin());
                range.material.transparent=!original.alpha_map.empty()||original.additive||original.color[3]<1;
                range.material.doubleSided=!original.backface;
                range.material.alphaReference=original.alpha_ref;range.material.additive=original.additive;
                // Authored COLOR carries the baked tint. Original unlit vertex
                // color shaders forward Color0 directly; avoid adding the
                // foundation preview's directional lighting over that stream.
                range.material.lightingEnabled=!hasColors;
                OriginalMaterial metadata; metadata.id=original.id;metadata.diffuse=original.diffuse;
                metadata.alphaMap=original.alpha_map;metadata.effectFile=original.effect_file;
                metadata.technique=original.gles2_technique;
                std::copy(original.texture_matrix,original.texture_matrix+16,metadata.textureMatrix.begin());
                metadata.alphaReference=original.alpha_ref;metadata.additive=original.additive;
                for(std::uint32_t k=0;k<mesh.vertices;++k) {
                    float values[4]{}; Vertex vertex;
                    if(!dh2_attribute_read(&positions,k,values)) throw std::runtime_error("Position decode failed");
                    vertex.position=point(world,values); vertex.normal={}; include(result,vertex.position);
                    if(hasColors) {
                        if(!dh2_attribute_read(&colors,k,values)) throw std::runtime_error("Vertex color decode failed");
                        for(std::uint32_t c=0;c<colors.components;++c) vertex.color[c]=values[c]/(colors.type==1?255.f:1.f);
                    }
                    if(hasUv) {if(!dh2_attribute_read(&uv,k,values)) throw std::runtime_error("UV decode failed");
                        const auto& t=original.texture_matrix;
                        // ProfileCOMMON_emul_VS.glsl's 2D convention is
                        // TextureMatrix0 * vec4(TexCoord0,1,0). The generic
                        // preview follows that pipeline, including t8/t9.
                        vertex.u=t[0]*values[0]+t[4]*values[1]+t[8];
                        vertex.v=t[1]*values[0]+t[5]*values[1]+t[9];}
                    result.mesh.vertices.push_back(vertex);
                }
                for(std::uint32_t k=0;k<primitive.index_count;++k) {
                    std::uint32_t index=0;
                    if(!dh2_index_read(&primitive,k,&index)||index>=mesh.vertices) throw std::runtime_error("Original index outside vertex domain");
                    result.mesh.indices.push_back(base+index);
                }
                // Face accumulation also handles nonuniform authored scale.
                for(std::size_t k=range.firstIndex;k<range.firstIndex+range.indexCount;k+=3) {
                    auto& a=result.mesh.vertices[result.mesh.indices[k]];
                    auto& b=result.mesh.vertices[result.mesh.indices[k+1]];
                    auto& c=result.mesh.vertices[result.mesh.indices[k+2]];
                    const auto n=cross(subtract(b.position,a.position),subtract(c.position,a.position));
                    for(auto* v:{&a,&b,&c}) {v->normal.x+=n.x;v->normal.y+=n.y;v->normal.z+=n.z;}
                }
                result.triangleCount+=primitive.index_count/3;
                result.mesh.ranges.push_back(range);result.materials.push_back(std::move(metadata));
            }
        }
        if(result.mesh.indices.empty()) throw std::runtime_error("Original scene has no visible triangle geometry");
        for(auto& vertex:result.mesh.vertices) {
            const float length=std::sqrt(vertex.normal.x*vertex.normal.x+vertex.normal.y*vertex.normal.y+vertex.normal.z*vertex.normal.z);
            if(length>0) {vertex.normal.x/=length;vertex.normal.y/=length;vertex.normal.z/=length;}
            else vertex.normal={0,1,0};
        }
        if(helpers) result.notices.push_back("Excluded exported module/navigation/collision helper instances: "+std::to_string(helpers));
        output=std::move(result);error.clear();return true;
    } catch(const std::exception& ex) {error=ex.what();return false;}
}

bool decode_original_scene(const std::vector<std::uint8_t>& bytes,
                           OriginalScene& output, std::string& error) {
    const Mat4 unit{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    return decode_original_scene_module(bytes,"",unit,output,error);
}

bool load_original_scene(const std::filesystem::path& path,OriginalScene& output,std::string& error) {
    std::ifstream file(path,std::ios::binary|std::ios::ate);
    if(!file) {error="Original environment asset is missing: "+path.string();return false;}
    const auto length=file.tellg();
    if(length<=0||length>std::streamoff(512*1024*1024)) {error="Invalid original environment file size";return false;}
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(length));file.seekg(0);
    if(!file.read(reinterpret_cast<char*>(bytes.data()),length)) {error="Cannot read original environment";return false;}
    if(!decode_original_scene(bytes,output,error)) return false;
    output.source=path.string();return true;
}
} // namespace dh::foundation
