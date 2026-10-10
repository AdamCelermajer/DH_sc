#include "original_character.hpp"
#include "source_material_pass.hpp"
#include "actor_lighting.hpp"
#include "modular_defaults.hpp"
#include <set>
#include "asset_catalog.hpp"
#include "animation_markers.hpp"
#include "../engine-animation/animation.hpp"
#include "../engine-skinning/skinning.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <limits>
#include <cctype>

namespace dh::foundation {
namespace {

void require(bool success, const std::string& message) { if (!success) throw std::runtime_error(message); }
Vec3 vec(const std::array<float,3>& p) { return {p[0],p[1],p[2]}; }
// Original GetAnimRoot semantic scene names, shared across actor/camera assets.
constexpr const char* sourceRootNames[]{"root_camera","Bip01","Root","root_character"};
}
struct CharacterVisual::Impl {
    struct Part {
        dh2::skinning::Skin skin;
        std::vector<std::array<float,3>> positions, normals;
        std::vector<std::array<float,3>> deformed_positions, deformed_normals;
        std::vector<dh2::skinning::Matrix> palette;
        std::vector<float> palette_bytes;
        std::uint32_t rigid_node=0;
        bool skinned=true;
    };
    dh2::scene::Scene scene;
    std::vector<dh2::animation::Player> players;
    std::unique_ptr<dh2::animation::Player> default_library;
    std::vector<std::pair<std::string,std::string>> clips;
    std::vector<Part> parts;
    std::vector<Mesh> meshes;
    std::vector<std::string> textures;
    std::vector<OriginalMaterial> materials;
    CharacterVisualConfig config;
    std::size_t active_clip = 0;
    bool loop = true;
    double clock = 0;
    bool ready = false;
    bool sampled = false;
    std::int32_t sampled_time = 0;
    dh2::animation::PoseSampleWorkspaceV32 pose_scratch;
    unsigned unbound_targets=0;
    dh2::animation::TransformSet dynamic_set;
    bool dynamic_channels=false;
    std::vector<AnimationMarkers> marker_tracks;
    std::int32_t motion_node=-1;
    std::vector<std::array<float,3>> motion_starts,motion_ends;
    std::array<float,3> motion_previous{};
    Vec3 motion_delta{};
    Vec3 motion_rest_origin{};
    bool deform(std::string& error) {
    for(std::size_t i=0;i<parts.size();++i) {
        auto& part=parts[i];
        part.deformed_positions.resize(part.positions.size());part.deformed_normals.resize(part.normals.size());
        if(part.skinned) {
            const auto& skin=part.skin;
            if(skin.nodes.empty() || skin.nodes.size()!=skin.inverse_bind.size() || skin.influences.size()!=part.positions.size() || !skin.influence_count || skin.influence_count>4) {error="Actor skin dimensions differ";return false;}
            part.palette.resize(skin.nodes.size());part.palette_bytes.resize(skin.nodes.size()*16);
            for(std::size_t j=0;j<skin.nodes.size();++j) {
                if(skin.nodes[j]>=scene.graph.size()) {error="Actor skin joint outside scene";return false;}
                dh2_skin_palette_matrix(part.palette[j].data(),scene.graph[skin.nodes[j]].world.data(),skin.inverse_bind[j].data(),skin.bind_shape.data());
                for(float value:part.palette[j])if(!std::isfinite(value)) {error="Actor skin matrix overflow";return false;}
                std::copy(part.palette[j].begin(),part.palette[j].end(),part.palette_bytes.begin()+j*16);
            }
            for(std::size_t j=0;j<part.positions.size();++j) {
                const auto& influence=skin.influences[j];
                for(unsigned k=0;k<skin.influence_count;++k)if(influence.joints[k]>=part.palette.size()) {error="Actor skin influence outside palette";return false;}
                dh2_skin_point(part.deformed_positions[j].data(),part.palette_bytes.data(),influence.joints.data(),influence.weights.data(),skin.influence_count,part.positions[j].data());
            }
            if(!dh2::skinning::directions_v113(skin,part.palette,part.normals,part.deformed_normals,error))return false;
        } else {
            if(part.rigid_node>=scene.graph.size()) {error="Rigid actor node outside scene";return false;}
            const auto& world=scene.graph[part.rigid_node].world;
            for(std::size_t j=0;j<part.positions.size();++j)for(unsigned axis=0;axis<3;++axis) {
                part.deformed_positions[j][axis]=world[12+axis];part.deformed_normals[j][axis]=0;
                for(unsigned k=0;k<3;++k) {part.deformed_positions[j][axis]+=world[k*4+axis]*part.positions[j][k];part.deformed_normals[j][axis]+=world[k*4+axis]*part.normals[j][k];}
            }
        }
        for(std::size_t j=0;j<part.deformed_positions.size();++j) {
            for(float v:part.deformed_positions[j]) if(!std::isfinite(v)) {error="Nonfinite Character pose";return false;}
            meshes[i].vertices[j].position=vec(part.deformed_positions[j]);meshes[i].vertices[j].normal=vec(part.deformed_normals[j]);
        }
    }
        error.clear();return true;
    }
    bool sample_pose_at(std::size_t clip,std::int32_t time,dh2::scene::Scene& targetScene,
                        dh2::animation::PoseSampleWorkspaceV32& workspace,std::string& error) const {
        if(!players[clip].sample_reuse(targetScene,time,workspace,error))return false;
        if(dynamic_channels) {
            const auto& targets=dynamic_set.targets();
            for(std::size_t i=0;i<targets.size();++i) {
                const auto& target=targets[i];if(target.node==UINT32_MAX)continue;
                if(target.node>=targetScene.graph.size()) {error="Dynamic actor target outside scene";return false;}
                auto& node=targetScene.graph[target.node];
                float* destination=target.type>=1 && target.type<=4 ? node.translation : target.type==5 || target.type==9 ? node.quaternion : target.type==10 ? node.scale : nullptr;
                if(!destination) {error="Unsupported dynamic actor target domain";return false;}
                if(!dynamic_set.sample(clip,i,time,destination,target.components,nullptr,error))return false;
            }
            if(!dh2::scene::update_world(targetScene,error))return false;
        }
        return true;
    }
    bool sample_pose(std::size_t clip,std::int32_t time,std::string& error) {
        return sample_pose_at(clip,time,scene,pose_scratch,error);
    }
};
CharacterVisual::CharacterVisual():impl_(std::make_unique<Impl>()) {}
CharacterVisual::~CharacterVisual() = default;
CharacterVisual::CharacterVisual(CharacterVisual&&) noexcept = default;
CharacterVisual& CharacterVisual::operator=(CharacterVisual&&) noexcept = default;
bool CharacterVisual::load(const AssetCatalog& assets, const CharacterVisualConfig& config, std::string& error) {
    return load_with_ranges(assets,config,{},error);
}
bool CharacterVisual::load_embedded_scene(const AssetCatalog& assets,const std::string& path,std::string& error){
    try{
        const auto bytes=assets.read(path);std::vector<EmbeddedSceneClip> ranges;
        if(!decode_embedded_scene_clips(bytes.data(),bytes.size(),ranges,error))return false;
        CharacterVisualConfig config;config.model_path=path;config.include_static_instances=true;
        for(const auto& range:ranges)config.clips.emplace_back(range.name,path);
        return load_with_ranges(assets,config,ranges,error,true);
    }catch(const std::exception& e){error=e.what();return false;}
}
bool CharacterVisual::load_with_ranges(const AssetCatalog& assets,const CharacterVisualConfig& config,
                                      const std::vector<EmbeddedSceneClip>& ranges,std::string& error,bool object_scene) {
    try {
        auto next = std::make_unique<Impl>();
        next->config=config;
        const auto bytes = assets.read(config.model_path);
        dh2::resources::BresView view{};
        require(dh2_bres_open(&view,bytes.data(),bytes.size()) == dh2::resources::BresError::ok,"Character BRES rejected");
        require(dh2::scene::load(view,next->scene,error),error);
        const auto targetPolicy=config.allow_missing_animation_targets ? dh2::animation::MissingTargets::ignore : dh2::animation::MissingTargets::reject;
        if(!config.template_clip_path.empty()) {
            const auto data=assets.read(config.template_clip_path);
            next->default_library=std::make_unique<dh2::animation::Player>();
            auto& baseline=*next->default_library;
            require(baseline.load(data.data(),data.size(),next->scene,error,targetPolicy),error);
            require(baseline.track_count()>0 && baseline.skipped==0,"Character template contains unsupported tracks");
            next->unbound_targets+=baseline.unbound;
            require(baseline.sample(next->scene,baseline.start,error),error);
        }
        next->clips=config.clips;
        if(next->clips.empty()) {
            constexpr const char* names[]{"idle","walk","attack"};
            for(unsigned i=0;i<3;++i)if(!config.animation_paths[i].empty())next->clips.emplace_back(names[i],config.animation_paths[i]);
        }
        require(next->clips.size()<=4096,"Character exceeds 4096 named animation clips");
        next->players.resize(next->clips.size());
        for (std::size_t i=0;i<next->clips.size();++i) {
            require(!next->clips[i].first.empty(),"Character animation name is empty");
            require(std::none_of(next->clips.begin(),next->clips.begin()+i,[&](const auto& clip){return clip.first==next->clips[i].first;}),"Duplicate character animation name: "+next->clips[i].first);
            const auto data=assets.read(next->clips[i].second);
            require(next->players[i].load(data.data(),data.size(),next->scene,error,targetPolicy), next->clips[i].second+": "+error);
            require(next->players[i].track_count()>0 || next->players[i].skipped>0,"Character animation has no bound tracks");
            next->unbound_targets+=next->players[i].unbound;
        }
        // Original actor sets use the dynamic default-data producer even for
        // ordinary full-vector channels, not only axis/angle interpreters.
        next->dynamic_channels=!next->players.empty();
        if(next->dynamic_channels) {
            std::vector<dh2::animation::TransformClipInput> inputs;
            for(std::size_t i=0;i<next->players.size();++i)inputs.push_back({static_cast<std::int32_t>(i),&next->players[i]});
            require(next->dynamic_set.compile_dynamic(inputs,next->scene,error,next->default_library.get()),"Dynamic actor animation: "+error);
            for(const auto& target:next->dynamic_set.targets())if(target.node==UINT32_MAX) {
                require(config.allow_missing_animation_targets,"Dynamic actor animation has an unbound target: "+target.uri);
                ++next->unbound_targets;
            }
        }
        next->marker_tracks.resize(next->players.size());
        for(std::size_t i=0;i<next->players.size();++i) {
            const auto* dynamicClip=next->dynamic_channels ? next->dynamic_set.clip(i) : nullptr;
            const auto start=!ranges.empty()?ranges.at(i).start_ms:dynamicClip ? dynamicClip->start : next->players[i].start;
            const auto end=!ranges.empty()?ranges.at(i).end_ms:dynamicClip ? dynamicClip->end : next->players[i].end;
            require(next->marker_tracks[i].load(next->players[i].events.view(),start,end,error),"Actor marker track: "+error);
        }
        if(config.consume_root_motion || !config.motion_node_id.empty() || !config.motion_node_candidates.empty()) {
            if(config.motion_node_id.empty() || config.motion_node_id=="auto") {
                std::vector<std::string> candidates=config.motion_node_candidates;
                if(candidates.empty())for(const auto* name:sourceRootNames)candidates.emplace_back(name);
                // Scene graph is authored depth-first order. Source root-name
                // policy takes the first scene-name match for each candidate.
                for(const auto& name:candidates) {
                    const auto found=std::find_if(next->scene.graph.begin(),next->scene.graph.end(),[&](const auto& node){return node.name==name;});
                    if(found!=next->scene.graph.end()) {next->motion_node=static_cast<std::int32_t>(found-next->scene.graph.begin());break;}
                }
            }
            else {
            for(std::size_t i=0;i<next->scene.graph.size();++i) {
                const auto& node=next->scene.graph[i];
                if(node.id==config.motion_node_id || node.sid==config.motion_node_id || node.name==config.motion_node_id) {
                    require(next->motion_node<0,"Ambiguous authored motion node");next->motion_node=static_cast<std::int32_t>(i);
                }
            }
            }
            require(next->motion_node>=0,"Unknown authored motion node: "+config.motion_node_id);
            const auto* origin=next->scene.graph[next->motion_node].translation;
            next->motion_rest_origin={origin[0],origin[1],origin[2]};
            next->motion_starts.resize(next->players.size());next->motion_ends.resize(next->players.size());
            for(std::size_t i=0;i<next->players.size();++i) {
                require(next->sample_pose(i,next->marker_tracks[i].start_ms(),error),error);
                std::copy(next->scene.graph[next->motion_node].translation,next->scene.graph[next->motion_node].translation+3,next->motion_starts[i].begin());
                require(next->sample_pose(i,next->marker_tracks[i].end_ms(),error),error);
                std::copy(next->scene.graph[next->motion_node].translation,next->scene.graph[next->motion_node].translation+3,next->motion_ends[i].begin());
            }
            if(!next->players.empty())next->motion_previous=next->motion_starts[0];
            else std::copy(next->scene.graph[next->motion_node].translation,next->scene.graph[next->motion_node].translation+3,next->motion_previous.begin());
        }
        const auto controllers=dh2_bres_library_count(&view,dh2::resources::Library::controller);
        require(config.skin_id_contains.empty()||(!config.use_authored_modular_defaults&&config.controller_ids.empty()),"Conflicting character controller selection policies");
        std::set<std::string> explicitControllers,matchedControllers,modularControllers,defaultControllers;
        std::set<std::uint32_t> visibleControllers;
        for(const auto& instance:next->scene.instances)if(instance.controller>=0)visibleControllers.insert(static_cast<std::uint32_t>(instance.controller));
        for(const auto& id:config.controller_ids)require(!id.empty()&&explicitControllers.insert(id).second,"Duplicate or empty explicit controller ID");
        if(config.use_authored_modular_defaults&&explicitControllers.empty()) {
            std::vector<ModularDefaultCategory> categories;
            require(decode_modular_defaults(bytes,categories,error),error);
            for(const auto& category:categories) {
                modularControllers.insert(category.available_controller_ids.begin(),category.available_controller_ids.end());
                if(!category.controller_id.empty())defaultControllers.insert(category.controller_id);
            }
        }
        unsigned selected=0;
        struct Binding {dh2::skinning::Skin skin;unsigned geometry=0,node=0;bool skinned=true;std::vector<std::uint32_t> materials;};
        std::vector<Binding> bindings;
        for (unsigned c=0;c<controllers;++c) {
            dh2::skinning::Skin skin;
            require(dh2::skinning::load(view,c,next->scene,skin,error),error);
            // Select authored equipment through caller-owned configuration.
            if (!config.skin_id_contains.empty() && skin.id.find(config.skin_id_contains)==std::string::npos) continue;
            if(!explicitControllers.empty()&&!explicitControllers.count(skin.id))continue;
            if(explicitControllers.empty()&&modularControllers.count(skin.id)&&!defaultControllers.count(skin.id))continue;
            if(config.use_authored_modular_defaults&&explicitControllers.empty()&&!modularControllers.count(skin.id)&&!visibleControllers.count(c))continue;
            matchedControllers.insert(skin.id);
            ++selected;
            bool instantiated=false;
            for(const auto& instance:next->scene.instances)if(instance.controller==static_cast<std::int32_t>(c)) {
                require(instance.geometry==skin.geometry,"Character instance/controller geometry differs");
                Binding binding;binding.geometry=skin.geometry;binding.node=instance.node_index;
                binding.skin=skin;binding.materials=instance.materials;bindings.push_back(std::move(binding));
                instantiated=true;
            }
            // Explicit modular selections can name library controllers without a
            // visible serialized instance. Preserve their existing symbol lookup.
            if(!instantiated) {
                Binding binding;binding.geometry=skin.geometry;binding.skin=std::move(skin);bindings.push_back(std::move(binding));
            }
        }
        if(controllers==0 || config.include_static_instances) {
            for(const auto& instance:next->scene.instances)if(instance.controller<0) {
                // Original object _colbox_ groups supply physical bounds, not
                // visible model geometry. Retain their graph for source sampling.
                // Existing actor configurations keep their established policy.
                if(object_scene){
                    auto node=static_cast<std::int32_t>(instance.node_index);bool collider=false;
                    while(node>=0){const auto& source=next->scene.graph.at(node);
                        const auto& name=source.name.empty()?source.id:source.name;
                        if(name.compare(0,8,"_colbox_")==0){collider=true;break;}
                        node=source.parent;
                    }
                    if(collider)continue;
                }
                Binding binding;binding.geometry=instance.geometry;binding.node=instance.node_index;binding.skinned=false;binding.materials=instance.materials;bindings.push_back(std::move(binding));
            }
        }
        for(const auto& binding:bindings) {
            dh2::assets::Mesh source{};
            require(dh2_mesh_open(&source,&view,binding.geometry)==dh2::assets::Error::ok,"Character geometry rejected");
            for(unsigned p=0;p<source.primitives;++p) {
                dh2::assets::Primitive primitive{};
                require(dh2_mesh_primitive(&source,p,&primitive)==dh2::assets::Error::ok,"Character primitive rejected");
                require(primitive.collada_type==0 && primitive.index_count%3==0,"Character requires triangle primitives");
                dh2::assets::Attribute position{},normal{},uv{},colors{};
                require(dh2_mesh_attribute(&source,primitive.attributes[0],&position)==dh2::assets::Error::ok && position.components>=3,"Character missing position stream");
                const bool haveNormal=dh2_mesh_attribute(&source,primitive.attributes[1],&normal)==dh2::assets::Error::ok && normal.components>=3;
                const bool haveUv=dh2_mesh_attribute(&source,primitive.attributes[4],&uv)==dh2::assets::Error::ok && uv.components>=2;
                const bool haveColors=dh2_mesh_attribute(&source,primitive.attributes[2],&colors)==dh2::assets::Error::ok && colors.components<=4;
                Impl::Part part;part.skin=binding.skin;part.skinned=binding.skinned;part.rigid_node=binding.node;part.positions.resize(source.vertices);part.normals.resize(source.vertices);
                Mesh mesh;mesh.vertices.resize(source.vertices);mesh.indices.resize(primitive.index_count);
                for(unsigned v=0;v<source.vertices;++v) {
                    float value[16]{};
                    require(dh2_attribute_read(&position,v,value),"Character position read failed");
                    std::copy(value,value+3,part.positions[v].begin());
                    if(haveNormal) { require(dh2_attribute_read(&normal,v,value),"Character normal read failed");std::copy(value,value+3,part.normals[v].begin()); }
                    else part.normals[v]={0,0,1};
                    if(haveUv) { require(dh2_attribute_read(&uv,v,value),"Character UV read failed"); mesh.vertices[v].u=value[0];mesh.vertices[v].v=value[1]; }
                    if(haveColors) {require(dh2_attribute_read(&colors,v,value),"Actor color read failed");for(unsigned k=0;k<colors.components;++k)mesh.vertices[v].color[k]=value[k]/(colors.type==1 ? 255.f : 1.f);}
                }
                for(unsigned j=0;j<primitive.index_count;++j) {
                    require(dh2_index_read(&primitive,j,&mesh.indices[j]) && mesh.indices[j]<source.vertices,"Character index out of range");
                }
                const dh2::scene::Material* material=nullptr;
                if(!binding.materials.empty()) {
                    require(p<binding.materials.size() && binding.materials[p]<next->scene.materials.size(),"Character instance material binding outside domain");
                    material=&next->scene.materials[binding.materials[p]];
                }
                else {const auto found=std::find_if(next->scene.materials.begin(),next->scene.materials.end(),[&](const dh2::scene::Material& m){return primitive.material && m.id==primitive.material;});if(found!=next->scene.materials.end())material=&*found;}
                require(material!=nullptr,"Character material unresolved");
                // Preserve the authored shader UV transform, as the static scene
                // adapter does. GPU texture assignment must not apply it again.
                if(haveUv)for(auto& vertex:mesh.vertices) {
                    const float u=vertex.u,v=vertex.v;const auto& t=material->texture_matrix;
                    // Original shader multiplies TextureMatrix0 * vec4(uv,1,0).
                    vertex.u=t[0]*u+t[4]*v+t[8];vertex.v=t[1]*u+t[5]*v+t[9];
                    require(std::isfinite(vertex.u) && std::isfinite(vertex.v),"Nonfinite actor material UV transform");
                }
                DrawRange range;range.indexCount=mesh.indices.size();std::copy(material->color,material->color+4,range.material.color.begin());
                // Original backface flag enables culling; renderer doubleSided disables it.
                range.material.doubleSided=!material->backface;range.material.transparent=material->color[3]<1 || !material->alpha_map.empty();mesh.ranges.push_back(range);
                mesh.ranges.back().material.additive=material->additive;mesh.ranges.back().material.alphaReference=material->alpha_ref;
                mesh.ranges.back().material.lightingEnabled=!haveColors;
                CommonMaterialPass commonPass;
                const auto passResult=resolve_common_material_pass(view,material->id,commonPass,error);
                require(passResult!=CommonMaterialPassResult::invalid,error);
                if(passResult==CommonMaterialPassResult::applied) {
                    mesh.ranges.back().material.sourcePass=commonPass.state;
                    if(classifySourceVertexLighting(commonPass.vertexShader,commonPass.vertexDefines)==SourceVertexLighting::CommonUnlit)
                        mesh.ranges.back().material.lightingEnabled=false;
                }
                OriginalMaterial original;original.id=material->id;original.diffuse=material->diffuse;original.alphaMap=material->alpha_map;original.effectFile=material->effect_file;original.technique=material->gles2_technique;
                if(passResult==CommonMaterialPassResult::applied)original.technique=commonPass.technique;
                std::copy(material->texture_matrix,material->texture_matrix+16,original.textureMatrix.begin());original.alphaReference=material->alpha_ref;original.additive=material->additive;
                next->materials.push_back(std::move(original));
                next->meshes.push_back(std::move(mesh));next->parts.push_back(std::move(part));next->textures.push_back(material->diffuse);
            }
        }
        require(!next->meshes.empty(),"No actor geometry matched configuration");
        require(explicitControllers.empty()||matchedControllers==explicitControllers,"Explicit character controller ID not found");
        require(!config.expected_controller_count || selected==config.expected_controller_count,"Character controller count differs from configured preset");
        next->ready=true;
        // Sample before publication so a failed reload preserves the current character.
        CharacterVisual candidate;candidate.impl_=std::move(next);
        require(candidate.update(0,error),error);
        impl_=std::move(candidate.impl_);error.clear();return true;
    } catch(const std::exception& e) { error=e.what();return false; }
}
void CharacterVisual::select(CharacterPose pose) {
    if(static_cast<unsigned>(pose)>=3)return;
    constexpr const char* names[]{"idle","walk","attack"};
    std::string error;
    select(names[static_cast<unsigned>(pose)],true,error);
}
bool CharacterVisual::select(const std::string& name,bool loop,std::string& error) {
    if(!loaded()) {error="Character visual not loaded";return false;}
    const auto it=std::find_if(impl_->clips.begin(),impl_->clips.end(),[&](const auto& clip){return clip.first==name;});
    if(it==impl_->clips.end()) {error="Unknown character animation: "+name;return false;}
    const auto index=static_cast<std::size_t>(it-impl_->clips.begin());
    if(impl_->active_clip!=index || impl_->loop!=loop) {
        impl_->clock=0;impl_->sampled=false;impl_->motion_delta={};
        if(impl_->motion_node>=0)impl_->motion_previous=impl_->motion_starts[index];
    }
    impl_->active_clip=index;impl_->loop=loop;error.clear();return true;
}
bool CharacterVisual::restart(const std::string& name,bool loop,std::string& error) {
    if(!select(name,loop,error))return false;
    impl_->clock=0;impl_->sampled=false;impl_->motion_delta={};
    if(impl_->motion_node>=0)impl_->motion_previous=impl_->motion_starts[impl_->active_clip];
    return update(0,error);
}
bool CharacterVisual::update(double seconds,std::string& error) {
    if(!loaded()) {error="Character visual not loaded";return false;}
    if(!std::isfinite(seconds) || seconds<0) {error="Invalid animation elapsed time";return false;}
    auto& state=*impl_;
    const auto* player=state.players.empty() ? nullptr : &state.players[state.active_clip];
    const auto* authoredRange=player ? &state.marker_tracks[state.active_clip] : nullptr;
    const auto start=authoredRange ? authoredRange->start_ms() : 0;
    const auto duration=authoredRange ? std::max<std::int64_t>(0,std::int64_t(authoredRange->end_ms())-start) : 0;
    // Reduce large elapsed intervals before converting to milliseconds.
    const double durationSeconds=duration/1000.0;
    double nextClock=duration ? (state.loop ? std::fmod(state.clock+std::fmod(seconds,durationSeconds),durationSeconds) : state.clock+std::min(seconds,durationSeconds-state.clock)) : 0;
    double loopCount=player && state.loop && duration ? std::floor(seconds/durationSeconds)+std::floor((state.clock+std::fmod(seconds,durationSeconds))/durationSeconds) : 0;
    // Decimal frame intervals can land a few ulps below an authored integer ms.
    // Snap only numerical noise, including an almost-complete loop boundary.
    const double nearestMillisecond=std::round(nextClock*1000);
    if(std::abs(nextClock*1000-nearestMillisecond)<1e-9) {
        nextClock=nearestMillisecond/1000;
        if(state.loop && duration && nearestMillisecond>=duration) {nextClock=0;++loopCount;}
    }
    const auto offset=state.loop ? static_cast<std::int64_t>(nextClock*1000) : (nextClock>=durationSeconds ? duration : static_cast<std::int64_t>(nextClock*1000));
    const auto time=static_cast<std::int32_t>(std::int64_t(start)+offset);
    const bool samePose=state.sampled && time==state.sampled_time;
    if(player && !samePose && !state.sample_pose(state.active_clip,time,error))return false;
    state.motion_delta={};
    if(state.motion_node>=0) {
        const double loops=loopCount;
        if(!std::isfinite(loops)) {error="Root-motion loop count overflow";return false;}
        const auto& node=state.scene.graph[state.motion_node];
        float delta[3]{};
        for(unsigned axis=0;axis<3;++axis) {
            const double cycle=player ? double(state.motion_ends[state.active_clip][axis])-state.motion_starts[state.active_clip][axis] : 0;
            const double displacement=double(node.translation[axis])-state.motion_previous[axis]+loops*cycle;
            if(!std::isfinite(displacement) || std::abs(displacement)>std::numeric_limits<float>::max()) {error="Root-motion displacement overflow";return false;}
            delta[axis]=static_cast<float>(displacement);
        }
        state.motion_delta={delta[0],delta[1],delta[2]};
        std::copy(node.translation,node.translation+3,state.motion_previous.begin());
        if(state.config.consume_root_motion && !samePose) {
            // Original VisualObject helper parent cancels the animated root's XYZ.
            Mat4 compensation{1,0,0,0,0,1,0,0,0,0,1,0,-node.translation[0],-node.translation[1],-node.translation[2],1};
            for(auto& sceneNode:state.scene.graph)sceneNode.world=dh2::scene::multiply(compensation,sceneNode.world);
            for(auto& instance:state.scene.instances)instance.world=dh2::scene::multiply(compensation,instance.world);
        }
    }
    if(samePose) {state.clock=nextClock;error.clear();return true;}
    if(!state.deform(error))return false;
    state.clock=nextClock;state.sampled=true;state.sampled_time=time;error.clear();return true;
}
bool CharacterVisual::loaded() const {return impl_ && impl_->ready;}
const dh2::scene::Scene* CharacterVisual::retained_scene_borrow() const noexcept {return loaded()?&impl_->scene:nullptr;}
const CharacterVisualConfig* CharacterVisual::configuration() const noexcept {return loaded()?&impl_->config:nullptr;}
const char* CharacterVisual::animation_name() const {return loaded() && !impl_->clips.empty() ? impl_->clips[impl_->active_clip].second.c_str() : "";}
const std::vector<Mesh>& CharacterVisual::meshes() const {static const std::vector<Mesh> empty;return impl_ ? impl_->meshes : empty;}
std::vector<Mesh>& CharacterVisual::mutable_meshes() {if(!impl_)impl_=std::make_unique<Impl>();return impl_->meshes;}
const std::vector<std::string>& CharacterVisual::texture_uris() const {static const std::vector<std::string> empty;return impl_ ? impl_->textures : empty;}
const std::vector<OriginalMaterial>& CharacterVisual::original_materials() const {static const std::vector<OriginalMaterial> empty;return impl_ ? impl_->materials : empty;}
unsigned CharacterVisual::unbound_animation_targets() const {return impl_ ? impl_->unbound_targets : 0;}
const AnimationMarkers* CharacterVisual::markers(const std::string& name,std::string& error) const {
    if(!loaded()) {error="Actor visual not loaded";return nullptr;}
    const auto clip=std::find_if(impl_->clips.begin(),impl_->clips.end(),[&](const auto& entry){return entry.first==name;});
    if(clip==impl_->clips.end()) {error="Unknown actor animation: "+name;return nullptr;}
    error.clear();return &impl_->marker_tracks[static_cast<std::size_t>(clip-impl_->clips.begin())];
}
bool CharacterVisual::animation_range(const std::string& name,std::int32_t& start,std::int32_t& end,std::string& error) const {
    const auto* track=markers(name,error);if(!track)return false;
    start=track->start_ms();end=track->end_ms();return true;
}
double CharacterVisual::animation_elapsed_seconds() const {return impl_ ? impl_->clock : 0;}
bool CharacterVisual::bone_world(const std::string& name,Mat4& output,std::string& error) const {
    if(!loaded()) {error="Actor visual not loaded";return false;}
    if(name.empty()) {error="Actor bone name is empty";return false;}
    const dh2::scene::Node* match=nullptr;
    for(const auto& node:impl_->scene.graph)if(node.id==name || node.sid==name || node.name==name) {
        if(match) {error="Ambiguous actor bone name: "+name;return false;}
        match=&node;
    }
    if(!match) {error="Unknown authored actor bone: "+name;return false;}
    for(float value:match->world)if(!std::isfinite(value)) {error="Nonfinite actor bone world";return false;}
    output=match->world;error.clear();return true;
}
bool CharacterVisual::source_target_node(std::uintptr_t& token,std::string& error) const {
    if(!loaded()){error="Actor visual not loaded";return false;}
    const auto& graph=impl_->scene.graph;
    std::vector<std::size_t> pending;
    for(std::size_t i=graph.size();i>0;--i)if(graph[i-1].parent<0)pending.push_back(i-1);
    constexpr const char wanted[]="target_node";
    while(!pending.empty()) {
        const auto index=pending.back();pending.pop_back();const auto& node=graph[index];
        bool same=node.name.size()==sizeof(wanted)-1;
        for(std::size_t i=0;same&&i<node.name.size();++i)
            same=std::tolower(static_cast<unsigned char>(node.name[i]))==wanted[i];
        if(same){token=reinterpret_cast<std::uintptr_t>(&node);error.clear();return true;}
        for(std::size_t i=graph.size();i>0;--i)
            if(graph[i-1].parent==static_cast<std::int32_t>(index))pending.push_back(i-1);
    }
    token=0;error.clear();return true;
}
bool CharacterVisual::source_target_node_world(std::uintptr_t token,Mat4& output,std::string& error) const {
    if(!loaded()){error="Actor visual not loaded";return false;}
    std::uintptr_t actual=0;if(!source_target_node(actual,error))return false;
    if(!token||token!=actual){error="Token is not this current visual's source target_node";return false;}
    for(const auto& node:impl_->scene.graph)if(reinterpret_cast<std::uintptr_t>(&node)==token) {
        output=node.world;error.clear();return true;
    }
    error="Current source target_node token is unavailable";return false;
}
bool CharacterVisual::socket_world(const std::string& name,const Mat4& offset,Mat4& output,std::string& error) const {
    for(float value:offset)if(!std::isfinite(value)) {error="Nonfinite actor socket offset";return false;}
    Mat4 bone;if(!bone_world(name,bone,error))return false;
    const auto combined=dh2::scene::multiply(bone,offset);
    for(float value:combined)if(!std::isfinite(value)) {error="Actor socket transform overflow";return false;}
    output=combined;error.clear();return true;
}
Vec3 CharacterVisual::root_motion_delta() const {return impl_ ? impl_->motion_delta : Vec3{};}
Vec3 CharacterVisual::take_root_motion() {const auto delta=root_motion_delta();if(impl_)impl_->motion_delta={};return delta;}
const char* CharacterVisual::root_motion_node_id() const {return impl_ && impl_->motion_node>=0 ? impl_->scene.graph[impl_->motion_node].id.c_str() : "";}
Vec3 CharacterVisual::root_motion_rest_origin() const {return impl_ ? impl_->motion_rest_origin : Vec3{};}
const char* CharacterVisual::source_motion_root_name(bool characterOnly) const {
    if(!loaded())return "";
    for(unsigned i=characterOnly ? 1 : 0;i<4;++i)for(const auto& node:impl_->scene.graph)if(node.name==sourceRootNames[i])return node.name.c_str();
    return "";
}
bool CharacterVisual::indexed_bounds(Vec3& minimum,Vec3& maximum,std::string& error) const {
    if(!loaded()) {error="Actor visual not loaded";return false;}
    Vec3 lo{INFINITY,INFINITY,INFINITY},hi{-INFINITY,-INFINITY,-INFINITY};bool found=false;
    auto include=[&](const Vertex& vertex) {
        const auto& point=vertex.position;
        if(!std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z))return false;
        lo.x=std::min(lo.x,point.x);lo.y=std::min(lo.y,point.y);lo.z=std::min(lo.z,point.z);
        hi.x=std::max(hi.x,point.x);hi.y=std::max(hi.y,point.y);hi.z=std::max(hi.z,point.z);found=true;return true;
    };
    for(const auto& mesh:impl_->meshes) {
        const auto domain=mesh.indices.empty() ? mesh.vertices.size() : mesh.indices.size();
        auto span=[&](std::size_t first,std::size_t count) {
            if(first>domain || count>domain-first) {error="Actor extent draw range outside geometry";return false;}
            for(std::size_t i=first;i<first+count;++i) {
                const auto index=mesh.indices.empty() ? i : mesh.indices[i];
                if(index>=mesh.vertices.size()) {error="Actor extent index outside geometry";return false;}
                if(!include(mesh.vertices[index])) {error="Nonfinite indexed actor extent";return false;}
            }
            return true;
        };
        if(mesh.ranges.empty()) {if(!span(0,domain))return false;}
        else for(const auto& range:mesh.ranges) {
            if(!span(range.firstIndex,range.indexCount))return false;
        }
    }
    if(!found) {error="Actor has no indexed visible geometry";return false;}
    minimum=lo;maximum=hi;error.clear();return true;
}
bool CharacterVisual::sample_local_pose(const std::string& name,std::int32_t time,SkeletalPose& output,std::string& error) const {
    if(!loaded()) {error="Actor visual not loaded";return false;}
    const auto found=std::find_if(impl_->clips.begin(),impl_->clips.end(),[&](const auto& clip){return clip.first==name;});
    if(found==impl_->clips.end()) {error="Unknown actor animation: "+name;return false;}
    const auto index=static_cast<std::size_t>(found-impl_->clips.begin());const auto& range=impl_->marker_tracks[index];
    if(time<range.start_ms() || time>range.end_ms()) {error="Independent actor pose time outside authored clip range";return false;}
    auto candidate=impl_->scene;dh2::animation::PoseSampleWorkspaceV32 workspace;
    if(!impl_->sample_pose_at(index,time,candidate,workspace,error))return false;
    return capture_scene_pose(candidate,output,error);
}
bool CharacterVisual::apply_local_pose(const SkeletalPose& pose,std::string& error) {
    if(!loaded()) {error="Actor visual not loaded";return false;}
    auto candidate=impl_->scene;if(!apply_scene_pose(pose,candidate,error))return false;
    if(impl_->config.consume_root_motion && impl_->motion_node>=0) {
        const auto* root=candidate.graph[impl_->motion_node].translation;
        Mat4 helper{1,0,0,0,0,1,0,0,0,0,1,0,-root[0],-root[1],-root[2],1};
        for(auto& node:candidate.graph)node.world=dh2::scene::multiply(helper,node.world);
        for(auto& instance:candidate.instances)instance.world=dh2::scene::multiply(helper,instance.world);
    }
    auto previousMeshes=impl_->meshes;auto previous=std::move(impl_->scene);impl_->scene=std::move(candidate);
    if(!impl_->deform(error)) {impl_->scene=std::move(previous);impl_->meshes=std::move(previousMeshes);return false;}
    impl_->sampled=false;error.clear();return true;
}
bool CharacterVisual::current_local_pose(SkeletalPose& output,std::string& error) const {
    if(!loaded()) {error="Actor visual not loaded";return false;}
    return capture_scene_pose(impl_->scene,output,error);
}
bool CharacterVisual::root_translation_from_pose(const SkeletalPose& pose,Vec3& output,std::string& error) const {
    if(!loaded() || impl_->motion_node<0) {error="Required configured authored animation root";return false;}
    if(pose.size()!=impl_->scene.graph.size()) {error="Root pose node domain differs";return false;}
    for(std::size_t i=0;i<pose.size();++i)if(pose[i].id!=impl_->scene.graph[i].id) {error="Root pose node identities differ";return false;}
    const auto& value=pose[impl_->motion_node].translation;
    for(float lane:value)if(!std::isfinite(lane)) {error="Nonfinite authored root coordinate";return false;}
    output={value[0],value[1],value[2]};error.clear();return true;
}
bool CharacterVisual::sample_root_translation(const std::string& name,std::int32_t milliseconds,Vec3& output,std::string& error) const {
    SkeletalPose pose;if(!sample_local_pose(name,milliseconds,pose,error))return false;
    return root_translation_from_pose(pose,output,error);
}
bool CharacterVisual::sample_source_root_translation(const std::string& name,std::int32_t time,Vec3& scratch,std::string& error) const {
    if(!loaded() || impl_->motion_node<0) {error="Required configured authored animation root";return false;}
    const auto clip=std::find_if(impl_->clips.begin(),impl_->clips.end(),[&](const auto& entry){return entry.first==name;});
    if(clip==impl_->clips.end()) {error="Unknown authored root clip: "+name;return false;}
    const auto index=static_cast<std::size_t>(clip-impl_->clips.begin());const auto& range=impl_->marker_tracks[index];
    if(time<range.start_ms() || time>range.end_ms()) {error="Source root time outside authored clip range";return false;}
    std::int32_t target=-1;
    for(std::size_t i=0;i<impl_->dynamic_set.targets().size();++i) {
        const auto& binding=impl_->dynamic_set.targets()[i];
        if(binding.type==1 && binding.node==std::uint32_t(impl_->motion_node))target=static_cast<std::int32_t>(i);
    }
    if(target<0) {error="Original full-vector motion-root target is absent";return false;}
    float point[]{scratch.x,scratch.y,scratch.z};
    if(!impl_->dynamic_set.sample(index,target,time,point,3,nullptr,error))return false;
    scratch={point[0],point[1],point[2]};error.clear();return true;
}
bool CharacterVisual::step_root_motion(const std::string& name,double seconds,bool loop,std::uint32_t stamp,
                                       RootMotionHistory& history,Vec3& output,std::string& error) const {
    if(!loaded() || impl_->motion_node<0) {error="Required configured authored animation root";return false;}
    if(!std::isfinite(seconds) || seconds<0) {error="Invalid cumulative authored root time";return false;}
    const auto clip=std::find_if(impl_->clips.begin(),impl_->clips.end(),[&](const auto& entry){return entry.first==name;});
    if(clip==impl_->clips.end()) {error="Unknown authored root clip: "+name;return false;}
    const auto index=static_cast<std::size_t>(clip-impl_->clips.begin());const auto& range=impl_->marker_tracks[index];
    const auto length=std::int64_t(range.end_ms())-range.start_ms();const double duration=length/1000.0;
    if(length<=0) {error="Authored root clip has no finite range";return false;}
    const double elapsed=loop ? seconds : std::min(seconds,duration);
    auto next=history;
    if(!next.initialized || next.clip_name!=name) {
        next={};next.clip_name=name;next.previous=vec(impl_->motion_starts[index]);next.loop=loop;
    } else if(next.loop!=loop || elapsed+1e-12<next.source_seconds) {error="Root history replay/loop change requires explicit reset";return false;}
    double cycles=loop ? std::floor(elapsed/duration) : 0;
    double offset=loop ? std::fmod(elapsed,duration)*1000 : elapsed*1000;
    const double nearest=std::round(offset);if(std::abs(offset-nearest)<1e-9)offset=nearest;
    if(loop && offset>=length) {offset=0;++cycles;}
    if(!std::isfinite(cycles)) {error="Authored root loop count overflow";return false;}
    const auto sourceTime=static_cast<std::int32_t>(std::int64_t(range.start_ms())+static_cast<std::int64_t>(offset));
    Vec3 point;if(!sample_root_translation(name,sourceTime,point,error))return false;
    const float coordinate[]{point.x,point.y,point.z},previous[]{next.previous.x,next.previous.y,next.previous.z};float delta[3]{};
    if(!next.initialized || next.timestamp!=stamp)for(unsigned axis=0;axis<3;++axis) {
        const double stride=double(impl_->motion_ends[index][axis])-impl_->motion_starts[index][axis];
        const double value=double(coordinate[axis])-previous[axis]+(cycles-next.cycles)*stride;
        if(!std::isfinite(value) || std::abs(value)>std::numeric_limits<float>::max()) {error="Independent authored root delta overflow";return false;}
        delta[axis]=static_cast<float>(value);
    }
    next.previous=point;next.source_seconds=elapsed;next.cycles=cycles;next.timestamp=stamp;next.initialized=true;
    history=std::move(next);output={delta[0],delta[1],delta[2]};error.clear();return true;
}
}


