#include "../root_motion_applicators_v1.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace {
using namespace dh2::animation::root_motion_applicators_v1;
struct Entry { std::uintptr_t animator, applicator; float xyz[3]; };
struct Context { const Entry* entries; std::uint32_t count; std::uint32_t reads; };
int read_delta(void* opaque, std::uintptr_t animator, std::uintptr_t applicator, float out[3]) {
    auto& context = *static_cast<Context*>(opaque);
    ++context.reads;
    for (std::uint32_t i = 0; i < context.count; ++i) {
        const auto& entry = context.entries[i];
        if (entry.animator == animator && entry.applicator == applicator) {
            std::memcpy(out, entry.xyz, sizeof(entry.xyz));
            return 0;
        }
    }
    return 1;
}
void check(bool condition, const char* name) {
    if (!condition) { std::fprintf(stderr, "FAILED: %s\n", name); std::exit(1); }
}
bool same(const float* a, const float* b) {
    for (unsigned i=0;i<3;++i) if (a[i] != b[i]) return false;
    return true;
}
}

int main() {
    const Entry backing[] = {{11,101,{1.25f,-2.f,3.f}}, {22,202,{.75f,4.f,-1.f}}};
    const AnimatorApplicator animators[] = {{11,101},{22,202}};
    Context context{backing,2,0};
    const Services services{&context,read_delta};
    float output[3] = {99.f,99.f,99.f};
    Request request{animators,2,&services,output,0};
    check(collect(&request)==0,"two attached applicators collected");
    const float expected_sum[3]={2.f,2.f,2.f};
    check(same(output,expected_sum),"source-order XYZ sum");
    check(context.reads==2,"one current read per attached animator");

    const AnimatorApplicator missing[] = {{11,101},{33,303}};
    context.reads=0;output[0]=7.f;output[1]=8.f;output[2]=9.f;
    request.animators=missing;
    check(collect(&request)==2,"missing exact applicator is reported");
    const float preserved[3]={7.f,8.f,9.f};
    check(same(output,preserved),"missing provider preserves output");

    const AnimatorApplicator duplicate[] = {{11,101},{11,303}};
    context.reads=0;request.animators=duplicate;
    check(collect(&request)==1,"duplicate animator identity rejected");
    check(context.reads==1,"duplicate is rejected before its second read");

    request.animators=nullptr;request.count=0;request.services=nullptr;
    check(collect(&request)==0,"empty source list is valid");
    const float zero[3]={0.f,0.f,0.f};
    check(same(output,zero),"empty source list emits zero delta");

    request.reserved=1;
    check(collect(&request)==1,"reserved input rejected");
    std::puts("root-motion applicator provider: 10 checks passed (source pipeline fixture; no runtime consumer wired)");
}
