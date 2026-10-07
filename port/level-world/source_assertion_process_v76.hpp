#pragma once
#include <cstdint>
#include <memory>
#include <string>
namespace dh2::world {
//Original gAssertLevel99f914 is a four-byte .bss process global. A cold
//native process therefore starts at0; it is not a per-World policy flag.
class SourceAssertionProcessV76 final {
 std::int32_t level_{};
 SourceAssertionProcessV76()=default;
public:
 static std::shared_ptr<SourceAssertionProcessV76> borrow();
 const std::int32_t* source_level()const noexcept{return &level_;}
 //Only an actual diagnostic/console-variable store may call this endpoint.
 void source_store_level(std::int32_t value)noexcept{level_=value;}
 //Native log transport for the recovered source assertion message. The
//actual write is checked; a missing/failed sink cannot report delivery.
 bool report(const char* file,std::int32_t line,const char* expression,std::string&);
};
}
