#include "../frustum_runtime.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace dh2::engine_camera::frustum_runtime;
namespace {
int guards() {
    Matrix matrix{};
    Frustum frustum{};
    std::memset(&frustum,0x5a,sizeof(frustum));
    const Frustum original=frustum;
    unsigned checks=0;
    const auto reject=[&](const Matrix* input, Frustum* output) {
        ++checks;
        return set_from(input,output)==Status::invalid_argument &&
               std::memcmp(&frustum,&original,sizeof(frustum))==0;
    };
    if (!reject(nullptr,&frustum) || !reject(&matrix,nullptr)) return 1;
    alignas(Matrix) unsigned char input_bytes[sizeof(Matrix)+alignof(Matrix)]{};
    alignas(Frustum) unsigned char output_bytes[sizeof(Frustum)+alignof(Frustum)]{};
    if (!reject(reinterpret_cast<const Matrix*>(input_bytes+1),&frustum) ||
        !reject(&matrix,reinterpret_cast<Frustum*>(output_bytes+1))) return 2;
    if (!reject(reinterpret_cast<const Matrix*>(&frustum),&frustum)) return 3;
    if (!reject(reinterpret_cast<const Matrix*>(
                    reinterpret_cast<const unsigned char*>(&frustum)+sizeof(Frustum)-4),
                &frustum)) return 4;
    const auto maximum=std::numeric_limits<std::uintptr_t>::max();
    if (!reject(reinterpret_cast<const Matrix*>(maximum-3),&frustum) ||
        !reject(&matrix,reinterpret_cast<Frustum*>(maximum-3))) return 5;
    // These calls execute the entire composed source, rather than substituting
    // a bounds/intersection output. Position seeds survive both regular and
    // degenerate matrices; the result remains an owned raw-word value.
    matrix.elements[0]=matrix.elements[5]=matrix.elements[10]=matrix.elements[15]=0x3f800000u;
    frustum.position[0]=0x40000000u;frustum.position[1]=0xc0400000u;frustum.position[2]=0x40800000u;
    if (set_from(&matrix,&frustum)!=Status::complete ||
        frustum.position[0]!=0x40000000u || frustum.position[1]!=0xc0400000u ||
        frustum.position[2]!=0x40800000u) return 6;
    ++checks;
    matrix={};
    if (set_from(&matrix,&frustum)!=Status::complete ||
        frustum.position[0]!=0x40000000u || frustum.position[1]!=0xc0400000u ||
        frustum.position[2]!=0x40800000u) return 7;
    ++checks;
    std::printf("{\"guards\":%u}\n",checks);
    return 0;
}
}
int main(int argc, char** argv) {
    if (argc==2 && std::strcmp(argv[1],"guards")==0) return guards();
    if (argc!=51 || std::strcmp(argv[1],"full")!=0) return 64;
    std::uint32_t matrix_words[16],frustum_words[33];
    for (unsigned i=0;i!=49;++i) {
        char* end=nullptr;
        const auto word=std::strtoul(argv[i+2],&end,16);
        if (!end || *end || word>0xffffffffu) return 65;
        if (i<16) matrix_words[i]=std::uint32_t(word);
        else frustum_words[i-16]=std::uint32_t(word);
    }
    Matrix matrix;Frustum frustum;
    std::memcpy(&matrix,matrix_words,sizeof(matrix));
    static_assert(sizeof(frustum)==sizeof(frustum_words));
    std::memcpy(&frustum,frustum_words,sizeof(frustum));
    const auto status=set_from(&matrix,&frustum);
    std::memcpy(frustum_words,&frustum,sizeof(frustum));
    std::printf("{\"status\":%d,\"words\":[",int(status));
    for (unsigned i=0;i!=33;++i) std::printf("%s%u",i?",":"",frustum_words[i]);
    std::printf("]}\n");
    return 0;
}
