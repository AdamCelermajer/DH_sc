#define main effects_metadata_audit_main
#include "effects_executor_tests.cpp"
#undef main
#include "effects_render_bridge.hpp"
#include "effects_material_binding.hpp"
#include <cstring>

// Portable test-only packet fixture, consumed by the native root Renderer
// smoke test. Every vertex/index/texture originates in the actual source frame.
template<class T> static void write(std::ofstream& out,const T& value) {
    static_assert(std::is_trivially_copyable_v<T>);out.write(reinterpret_cast<const char*>(&value),sizeof(value));
}
static void fixture(const std::string& path,const EffectRenderFrame& frame,
                    const std::map<std::uint32_t,dh::foundation::TextureImage>& textures) {
    std::ofstream out(path,std::ios::binary);check(bool(out),"native render fixture output");
    write(out,std::uint32_t(0x46584631));write(out,std::uint32_t(textures.size()));
    for(const auto& [id,image]:textures) {
        write(out,id);write(out,image.width);write(out,image.height);write(out,std::uint32_t(image.rgba.size()));
        out.write(reinterpret_cast<const char*>(image.rgba.data()),image.rgba.size());
    }
    write(out,std::uint32_t(frame.packets.size()));
    for(const auto& packet:frame.packets) {
        static_assert(sizeof(dh::foundation::Vertex)==48);
        write(out,std::uint32_t(packet.mesh.vertices.size()));write(out,std::uint32_t(packet.mesh.indices.size()));
        out.write(reinterpret_cast<const char*>(packet.mesh.vertices.data()),packet.mesh.vertices.size()*48);
        out.write(reinterpret_cast<const char*>(packet.mesh.indices.data()),packet.mesh.indices.size()*4);
        write(out,packet.world);const auto& material=packet.mesh.ranges.at(0).material;
        write(out,material.color);write(out,material.texture);write(out,material.alphaReference);
        write(out,std::uint32_t(material.transparent)|(std::uint32_t(material.doubleSided)<<1)|
                  (std::uint32_t(material.additive)<<2)|(std::uint32_t(material.lightingEnabled)<<3));
        const auto& pass=*material.sourcePass;
        for(auto value:{pass.blendSource,pass.blendDestination,pass.depthFunction,pass.cullFace,pass.frontFace})write(out,value);
        write(out,std::uint32_t(pass.blend)|(std::uint32_t(pass.depthTest)<<1)|
                  (std::uint32_t(pass.depthWrite)<<2)|(std::uint32_t(pass.cull)<<3)|(std::uint32_t(pass.alphaTest)<<4));
    }
    check(bool(out),"actual packet fixture complete");
}
int main(int argc,char** argv) { try {
    check(argc==5,"table-dir exact-uri-map actual-texture-root native-fixture-output");
    Services services(argv[2]);data::EffectsTables tables;std::string error;std::string dir=argv[1];
    auto a=read(dir+"/effects_pyarray.bin"),b=read(dir+"/effects_pyarraynames.bin"),
         c=read(dir+"/effects_pystructnames.bin"),d=read(dir+"/effects_dictionary_pyarraynames.bin"),
         e=read(dir+"/effects_dictionary_pyarray.bin");
    auto bytes=[](const Raw& value) { return data::Bytes{value.data(),value.size()}; };
    check(tables.load(bytes(a),bytes(b),bytes(c),bytes(d),bytes(e),error),error);
    dh::foundation::AssetCatalog assets(argv[3]);std::map<std::uint32_t,dh::foundation::TextureImage> uploaded;
    unsigned uploads=0,releases=0;
    OriginalEffectMaterialBinding materials(assets,{
        [&](const dh::foundation::TextureImage& image,std::uint32_t& id,std::string&) {
            check(image.width&&image.height&&image.rgba.size()==std::size_t(image.width)*image.height*4,"decoded original RGBA upload");
            id=++uploads;uploaded.emplace(id,image);return true;
        },[&](std::uint32_t id) { check(uploaded.erase(id)==1,"same original texture handle released");++releases; }});
    auto table=tables.borrow();fx::CharacterAuthoredFxForceFactoryOwnerV4 forces;
    fx::CharacterAuthoredResourceFactoryV32 resources({nullptr,camera,driver},forces.factory());
    std::uint64_t packets=0,vertices=0,indices=0;unsigned rendered_sets=0,required_sets=0;
    bool wrote_fixture=false;unsigned retained_events=0;
    for(auto set:{164,150,160,253,254,255}) {
        fx::CharacterMeshFxOwnerV4 manager(table,services.live,{&services,Services::asset},
                                         {&services,Services::invoke},resources.factory());
        check(manager.precache_libraries(error),error);EffectsExecutor executor(manager);
        std::uintptr_t identity=0;check(executor.play_set(set,services.actor,true,false,&identity,error)&&identity,error);
        std::shared_ptr<const EffectRenderFrame> retained;
        EffectRenderServices renderer;
        renderer.material=[&](const EffectDrawSource& source,const scene::Material& material,dh::foundation::Material& target,std::string& e) {
            return materials.bind(source,material,target,e);
        };
        renderer.submit=[&](std::shared_ptr<const EffectRenderFrame> frame,std::string&) { retained=std::move(frame);return true; };
        EffectsRenderBridge bridge(manager,renderer);bool saw=false;
        for(int ms=9000;ms<9500;ms+=32) {
            check(executor.scene_phase(ms,32,error)&&executor.manager_phase(32,error),error);
            const auto before=manager.views();
            bool submitted=bridge.submit(error);
            if(!submitted) {std::cout<<"REQUIRED RENDER | "<<set<<" | "<<error<<std::endl;++required_sets;break;}
            check(manager.views()[0].current_ms==before[0].current_ms,"root render submission never ticks source FX");
            std::vector<fx::CharacterFxMeshDrawSourceV4> meshes;std::vector<fx::CharacterParticleDrawSourceV3> particles;
            check(executor.draw_sources(meshes,particles,error),error);
            for(const auto& packet:retained->packets) {
                const skinning::VisualDrawPartV6* original=nullptr;
                if(packet.source.kind==EffectDrawKind::authored_mesh) {
                    for(const auto& mesh:meshes)if(mesh.node_identity==packet.source.node&&mesh.primitive==packet.source.primitive)original=&mesh.part;
                } else for(const auto& cloud:particles)if(cloud.particle_identity==packet.source.particle)original=&cloud.part;
                check(original,"same original retained render receiver");
                const auto& primitive=original->geometry->primitives.at(packet.source.primitive);
                const auto& uv=original->geometry->attributes.at(primitive.attributes[4]);
                check(packet.mesh.indices==primitive.indices&&packet.world==original->world,"exact source topology/attachment transform");
                check(packet.source_retention&&packet.source.resource_bytes&&packet.mesh.ranges[0].material.sourcePass,
                      "source snapshots, bytes, original pass and decoded texture bound");
                for(std::size_t v=0;v<packet.mesh.vertices.size();++v) {
                    const auto& vertex=packet.mesh.vertices[v];const auto& position=original->positions[v];
                    check(vertex.position.x==position[0]&&vertex.position.y==position[1]&&vertex.position.z==position[2],"exact source mesh-skin/billboard vertices");
                    check(vertex.u==uv.values[v*2]&&vertex.v==uv.values[v*2+1],"original source UV is not transformed twice");
                }
                ++packets;vertices+=packet.mesh.vertices.size();indices+=packet.mesh.indices.size();saw=true;
            }
            if(!wrote_fixture&&!particles.empty()&&!retained->packets.empty()) {
                fixture(argv[4],*retained,uploaded);wrote_fixture=true;
            }
        }
        if(saw)++rendered_sets;
        if(retained) {
            auto preserved=retained->packets;executor.release_actor(services.actor.actor);
            for(const auto& packet:retained->packets)check(!packet.mesh.vertices.empty(),"retained submitted frame survives source detach");
        }
        RetainedEffectsAdapter events(manager,{[&](std::uintptr_t actor,ActorAttachment& output,std::string&) {
            output=services.actor;return actor==output.actor; }});
        dh::foundation::RetainedAnimationEvent marker;marker.name="fx_"+table.set_names()[set];
        marker.clip_id="same-retained-clip";marker.generation=2;marker.wall_timestamp_ms=10000;
        check(events.event(services.actor.actor,marker,0,error)==DispatchResult::delivered,error);
        check(events.event(services.actor.actor,marker,0,error)==DispatchResult::duplicate,"same shared audiovisual batch once");
        marker.wall_timestamp_ms=10032;
        check(events.event(services.actor.actor,marker,0,error)==DispatchResult::delivered,"new batch in same generation is preserved");
        retained_events+=2;events.release_actor(services.actor.actor);
    }
    check(rendered_sets>=6&&required_sets==0&&packets>0&&wrote_fixture,"actual skill/swoosh renderer closure");
    auto count=materials.texture_count();materials.clear();check(releases==count&&uploaded.empty(),"source texture/context unload exact cleanup");
    std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"source_sets_rendered\":"<<rendered_sets
             <<",\"actual_render_packets\":"<<packets<<",\"exact_vertices\":"<<vertices<<",\"exact_indices\":"<<indices
             <<",\"original_texture_uploads\":"<<uploads<<",\"retained_events\":"<<retained_events
             <<",\"native_renderer_fixture\":true,\"live_gl_upload\":false}\n";
} catch(const std::exception& e) { std::cerr<<e.what()<<'\n';return 1; } }
