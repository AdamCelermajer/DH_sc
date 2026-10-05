#include "../particle_random_v1.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
int main(int argc,char**argv){try{if(argc!=2)throw std::runtime_error("gold argument");std::ifstream file(argv[1],std::ios::binary);std::uint32_t header[2];file.read(reinterpret_cast<char*>(header),8);if(header[0]!=0x31525050)throw std::runtime_error("gold header");
 for(unsigned i=0;i<header[1];++i){std::uint32_t row[4];file.read(reinterpret_cast<char*>(row),16);std::int32_t seed,expected;std::memcpy(&seed,row,4);std::memcpy(&expected,row+1,4);double value=dh2::animation::dh2_particle_random_v1(&seed);std::uint32_t bits[2];std::memcpy(bits,&value,8);if(seed!=expected||bits[0]!=row[2]||bits[1]!=row[3])throw std::runtime_error("PSRandom mismatch "+std::to_string(i));}
 std::cout<<"{\"validation\":\"PASS\",\"original_cases\":"<<header[1]<<",\"mismatches\":0}"<<std::endl;return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}}
