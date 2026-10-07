#include "particle_random_v1.hpp"
#include <cstring>
#include <limits>
namespace dh2::animation {
extern "C" double dh2_particle_random_v1(std::int32_t* seed){
 if(!seed)return std::numeric_limits<double>::quiet_NaN();
 const auto value=*seed;const auto quotient=value/44488;
 const auto remainder=std::int64_t(value)-std::int64_t(quotient)*44488;
 auto bits=std::uint32_t(remainder*48271-std::int64_t(quotient)*3399);
 std::int32_t next;std::memcpy(&next,&bits,4);
 if(next<0){bits-=0x80000001u;std::memcpy(&next,&bits,4);}
 *seed=next;return double(next)/2147483647.0;
}
}
