#include "../character_oid_cache_v1.hpp"

#include <cstdlib>
#include <iostream>

using namespace dh2::character_oid_cache_v1;

namespace {
void check(bool condition,const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}
}

int main() {
    Owner cache;
    Result out{};
    check(cache.count(1,&out)==Status::no_level,"cache lookup requires a live Level");
    check(cache.begin_level(0x1000,64)==Status::complete,"begin first Level");
    const auto first_generation=cache.generation();
    check(cache.begin_level(0x1000,64)==Status::complete&&
          cache.generation()==first_generation,"same Level bind is idempotent");
    check(cache.begin_level(0x1000,63)==Status::invalid_argument&&
          cache.generation()==first_generation,"same identity cannot change CharacterTable size");

    const Argument one[]={{ValueKind::unsigned_integer,7}};
    check(register_summon(cache,{one,1},&out)==Status::complete&&
          out.value==1&&out.changed,"RegisterSummon default count is one");
    const Argument lower[]={{ValueKind::unsigned_integer,7},
                            {ValueKind::unsigned_integer,0}};
    check(register_summon(cache,{lower,2},&out)==Status::complete&&
          out.value==1&&!out.changed,"source cache preserves max(existing,requested)");
    const Argument higher[]={{ValueKind::unsigned_integer,7},
                             {ValueKind::unsigned_integer,4}};
    check(register_summon(cache,{higher,2},&out)==Status::complete&&
          out.value==4&&out.changed,"larger requested count replaces cached count");
    const Argument wrong_second[]={{ValueKind::unsigned_integer,7},
                                   {ValueKind::other,99}};
    check(register_summon(cache,{wrong_second,2},&out)==Status::complete&&
          out.value==4&&!out.changed,"non-UInteger optional count defaults to one");

    const Argument invalid_first[]={{ValueKind::other,7}};
    check(register_summon(cache,{invalid_first,1},&out)==Status::complete&&
          cache.count(7,&out)==Status::complete&&out.value==4,
          "wrong arg0 type is source no-op");
    const Argument out_of_range[]={{ValueKind::unsigned_integer,64}};
    check(register_summon(cache,{out_of_range,1},&out)==Status::complete&&
          cache.entry_count()==1,"out-of-range CharacterTable ID is source no-op");
    check(cache.add_char_oid(64,9,&out)==Status::out_of_range,
          "direct cache provider rejects out-of-range ID");
    check(cache.clear_level(0x9999,first_generation)==Status::stale_level&&
          cache.count(7,&out)==Status::complete&&out.value==4,
          "stale Level destructor cannot clear current Level");

    check(cache.begin_level(0x2000,64)==Status::complete&&
          cache.generation()>first_generation&&cache.entry_count()==0,
          "replacement Level starts with empty source cache");
    check(cache.clear_level(0x1000,first_generation)==Status::stale_level&&cache.entry_count()==0,
          "old Level cannot clear replacement cache");
    const auto replacement_generation=cache.generation();
    check(cache.clear_level(0x2000,replacement_generation)==Status::complete&&cache.entry_count()==0&&
          cache.level_identity()==0,"Level destructor clears source cache");
    check(cache.clear_level(0x2000,replacement_generation)==Status::complete,
          "clear after destruction is idempotent");
    check(cache.begin_level(0x2000,64)==Status::complete&&
          cache.clear_level(0x2000,replacement_generation)==Status::stale_level&&
          cache.entry_count()==0,
          "reused Level identity cannot let a stale destructor clear new generation");
    std::cout << "PASS character OID cache max-count, RegisterSummon args, and Level lifetime\n";
}
