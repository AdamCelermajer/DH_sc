#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace dh2::ui::text_filter_v1 {
// Source records occupy 44 bytes; unspecified stack bytes are not observable
// fields. Defined byte masks prevent inventing deterministic original padding.
struct Filter {
    std::array<std::uint8_t,44> bytes{}, defined{};
    std::uint32_t word(std::size_t) const;
    float real(std::size_t) const;
};
struct Effect { std::uint32_t blend{}; std::vector<Filter> filters; };
struct Reader {
    // These callbacks are the real source stream operations, in call order.
    std::function<bool(std::uint8_t&,std::string&)> byte;
    std::function<bool(std::uint16_t&,std::string&)> half;
    std::function<bool(std::uint32_t,std::uint32_t&,std::string&)> bits;
    std::function<bool(bool&,std::string&)> bit;
    std::function<bool(float&,std::string&)> fixed;
    std::function<bool(std::array<std::uint8_t,4>&,std::string&)> rgba;
};
// Append, not replace. The source reserves incoming count then appends 0/1/2;
// kind3 consumes its full body and retains no record. Unknown kinds consume
// only the type byte in the recovered source. Every primitive must be supplied.
bool read(Effect&,const Reader&,std::string&);
// Placement retains its tag-owned effect identity. Explicit set_effect takes
// an owned copy, matching 755cf8. The shared lease is a native lifetime safety
// correction; it does not change source mutations or filter order.
class Binding {
    std::shared_ptr<Effect> value_;
public:
    void place(std::shared_ptr<Effect> p){value_=std::move(p);}
    void move(const std::shared_ptr<Effect>& p){if(p)value_=p;}
    bool set(const Effect&,std::string&);
    const std::shared_ptr<Effect>& value() const{return value_;}
};
}
