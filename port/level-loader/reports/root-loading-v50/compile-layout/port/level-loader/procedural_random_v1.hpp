#pragma once
#include <cstdint>
#include <string_view>
namespace dh2::loader {
// Original rnd::RandomGenerator primitive state only. This is not the global
// gameplay Random service and does not establish rule/layout execution.
class ProceduralRandomV1 {
    std::uint32_t state_;
public:
    explicit ProceduralRandomV1(std::uint32_t seed):state_(seed){}
    std::uint32_t state()const noexcept{return state_;}
    std::uint32_t next()noexcept;
    // Original half-open interval; min>=max returns min without advancing.
    std::int32_t between(std::int32_t minimum,std::int32_t maximum)noexcept;
    // std::random_shuffle callback. Zero is outside its original caller domain;
    // reject it rather than inventing the imported divide-by-zero policy.
    std::uint32_t bounded(std::uint32_t exclusive);
    // Original byte-string hash stops at NUL, using unsigned bytes and wrap.
    static std::uint32_t hash(std::string_view)noexcept;
};
}
