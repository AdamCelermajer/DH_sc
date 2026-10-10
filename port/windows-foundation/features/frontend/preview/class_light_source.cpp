#include "class_light_source.hpp"
#include <cstring>
#include <stdexcept>
namespace dh::foundation::frontend {
namespace {float bits(std::uint32_t word){float value;std::memcpy(&value,&word,4);return value;}}
bool read_class_select_light_inputs(const dh2::scene::Scene& scene,ClassSelectLightInputs& output,std::string& error){try{
    ClassSelectLightInputs next;
    if(scene.lights_v113.empty()){output=std::move(next);error.clear();return true;}
    // Original getSceneNodeFromType('lght') selects the first authored node.
    const auto& light=scene.lights_v113.front();
    if(light.node_index>=scene.graph.size())throw std::runtime_error("Original class light node index unavailable");
    next.authored_light_present=true;next.node_index=light.node_index;next.source_light_type=light.type;
    const auto& node=scene.graph[light.node_index];next.authored_node=node.name;
    next.position={node.world[12],node.world[13],node.world[14],1};
    // Exact original ARM Show429270..42928c constants. Setter40b3c8
    // converts linear/quadratic units; color setters preserve float1 directly.
    next.attenuation_setter={bits(0x3ec00000),bits(0x3f030004),bits(0x3f86002a)};
    next.normalized_attenuation={next.attenuation_setter[0],next.attenuation_setter[1]/1000.f,next.attenuation_setter[2]/1000000.f};
    next.ambient={1,1,1,1};next.diffuse={1,1,1,1};next.specular={1,1,1,1};
    output=std::move(next);error.clear();return true;
}catch(const std::exception&e){error=e.what();return false;}}
const char* required_class_select_light_owner(){return "Required SAME authored scene light -> spawned native LightPoint -> PlayerLight slot0 -> actual material CLight uniform owner; source setter metadata is not a bound light";}
}
