#include "../camera_math.hpp"
#include "../camera_node_transform.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <limits>
#include <vector>

namespace cm = dh2::engine_camera::camera_math;
namespace cn = dh2::engine_camera::camera_node_transform;
namespace {
float as_float(std::uint32_t word) { float x; std::memcpy(&x,&word,4); return x; }
std::uint32_t as_word(float x) { std::uint32_t word; std::memcpy(&word,&x,4); return word; }
std::uint32_t add(void*,std::uint32_t a,std::uint32_t b) { return as_word(as_float(a)+as_float(b)); }
std::uint32_t sub(void*,std::uint32_t a,std::uint32_t b) { return as_word(as_float(a)-as_float(b)); }
std::uint32_t mul(void*,std::uint32_t a,std::uint32_t b) { return as_word(as_float(a)*as_float(b)); }
std::uint32_t div(void*,std::uint32_t a,std::uint32_t b) { return as_word(as_float(a)/as_float(b)); }
std::uint32_t eq(void*,std::uint32_t a,std::uint32_t b) { return std::uint32_t(as_float(a)==as_float(b)); }
std::uint32_t sqrt_word(void*,std::uint32_t a) { return as_word(std::sqrt(as_float(a))); }
const cm::MathServices math{nullptr,0,&add,&sub,&mul,&div,&eq,&sqrt_word};

struct CallContext {
    cn::CameraNode* node = nullptr;
    const cm::MathServices* math = nullptr;
    const cn::SceneServices* scene = nullptr;
    std::vector<unsigned> events;
    cn::Status nested = cn::Status::complete;
    bool probe_reentry = false;
};
void transform(void* opaque,void* frustum,std::uint32_t state) noexcept {
    auto& c=*static_cast<CallContext*>(opaque);
    if (frustum != reinterpret_cast<unsigned char*>(c.node)+0x168 || state != 0 ||
        c.node->view_matrix.definitely_identity != 0) std::terminate();
    c.events.push_back(1);
    if(c.probe_reentry)c.nested=cn::recalculate_matrices(c.node,c.math,c.scene);
}
void view_area(void* opaque,cn::CameraNode* node) noexcept {
    auto& c=*static_cast<CallContext*>(opaque);
    if(node!=c.node || c.events.size()!=1 || c.events[0]!=1)std::terminate();
    c.events.push_back(2);
}
int guards() {
    unsigned checks=0;
    cm::Vector3 p{{0x3f800000,0x40000000,0x40400000}},t{{0,0,0}},u{{0,0x3f800000,0}};
    cm::Matrix4 matrix;std::memset(&matrix,0x5a,sizeof(matrix));const auto original=matrix;
    if(cm::build_look_at(nullptr,&t,&u,&matrix,&math)!=cm::Status::invalid_argument ||
       std::memcmp(&matrix,&original,sizeof(matrix))) return 1;
    ++checks;
    if(cm::build_look_at(&p,&t,&u,nullptr,&math)!=cm::Status::invalid_argument) return 2;
    ++checks;
    const cm::Vector3 p_original=p;
    if(cm::build_look_at(&p,&t,&u,reinterpret_cast<cm::Matrix4*>(&p),&math)!=cm::Status::invalid_argument ||
       std::memcmp(&p,&p_original,sizeof(p))) return 3;
    ++checks;
    const auto max=std::numeric_limits<std::uintptr_t>::max();
    if(cm::build_look_at(&p,&t,&u,reinterpret_cast<cm::Matrix4*>(max-3),&math)!=cm::Status::invalid_argument) return 4;
    ++checks;

    cn::CameraNode node{};node.absolute_position=p;node.target=t;node.up=u;
    CallContext context;context.node=&node;context.math=&math;context.probe_reentry=true;
    cn::SceneServices scene{&context,sizeof(context),&transform,&view_area};context.scene=&scene;
    if(cn::recalculate_matrices(&node,&math,&scene)!=cn::Status::complete ||
       context.events!=std::vector<unsigned>({1,2}) ||
       context.nested!=cn::Status::reentrant_call) return 5;
    ++checks;
    const auto saved=node.view_matrix;
    if(cn::recalculate_matrices(nullptr,&math,&scene)!=cn::Status::invalid_argument ||
       std::memcmp(&node.view_matrix,&saved,sizeof(saved))) return 6;
    ++checks;
    if(cn::recalculate_matrices(&node,nullptr,&scene)!=cn::Status::invalid_argument) return 7;
    ++checks;
    if(cn::recalculate_matrices(&node,&math,nullptr)!=cn::Status::invalid_argument) return 8;
    ++checks;
    if(cn::recalculate_matrices(&node,&math,reinterpret_cast<cn::SceneServices*>(&node))!=cn::Status::invalid_argument) return 9;
    ++checks;
    std::printf("{\"guards\":%u}\n",checks);
    return 0;
}
bool parse(const char* text,std::uint32_t& value) {
    char* end=nullptr;const auto n=std::strtoul(text,&end,16);
    if(!end || *end || n>0xfffffffful) return false;
    value=std::uint32_t(n);return true;
}
void print_words(const std::uint32_t* words,std::size_t n) {
    std::printf("[");
    for(std::size_t i=0;i<n;++i)std::printf("%s%u",i?",":"",words[i]);
    std::printf("]");
}
}
int main(int argc,char** argv) {
    if(argc==2 && std::strcmp(argv[1],"guards")==0)return guards();
    if(argc!=11 || (std::strcmp(argv[1],"helper") && std::strcmp(argv[1],"node")))return 64;
    std::uint32_t words[9];for(unsigned i=0;i<9;++i)if(!parse(argv[i+2],words[i]))return 65;
    cm::Vector3 p{{words[0],words[1],words[2]}},t{{words[3],words[4],words[5]}},u{{words[6],words[7],words[8]}};
    cm::Matrix4 matrix{};
    if(std::strcmp(argv[1],"helper")==0) {
        const auto status=cm::build_look_at(&p,&t,&u,&matrix,&math);
        std::printf("{\"status\":%d,\"words\":",int(status));print_words(matrix.elements,16);
        std::printf(",\"identity\":%u}\n",matrix.definitely_identity);return 0;
    }
    cn::CameraNode node{};std::memset(&node,0xa5,sizeof(node));
    node.absolute_position=p;node.target=t;node.up=u;
    CallContext context;context.node=&node;context.math=&math;
    cn::SceneServices scene{&context,sizeof(context),&transform,&view_area};context.scene=&scene;
    const auto status=cn::recalculate_matrices(&node,&math,&scene);
    std::printf("{\"status\":%d,\"words\":",int(status));print_words(node.view_matrix.elements,16);
    std::printf(",\"identity\":%u,\"events\":[",node.view_matrix.definitely_identity);
    for(std::size_t i=0;i<context.events.size();++i)std::printf("%s%u",i?",":"",context.events[i]);
    std::printf("]}\n");return 0;
}
