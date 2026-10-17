#include "../player_attack_melee_radius_v1.hpp"
#include <cmath>
#include <cstdio>
#include <stdexcept>

int main() {
    using namespace dh2;
    using namespace dh2::character::player_attack_melee_radius_v1;
    data::AiTables tables;
    tables.rows.resize(9);
    tables.rows[2].melee_radius=1.25f;
    tables.rows[8].melee_radius=3.5f;
    data::ItemRecord164 item{};
    float radius=-1.f;
    if(query(nullptr,&tables,2,&radius)!=Status::complete||radius!=1.25f)
        throw std::runtime_error("unarmed Player radius changed");
    item.words[22]=4;item.words[39]=99;
    if(query(&item,&tables,2,&radius)!=Status::complete||radius!=1.25f)
        throw std::runtime_error("ranged item contributed melee radius");
    item.words[22]=5;
    if(query(&item,&tables,2,&radius)!=Status::complete||radius!=1.25f)
        throw std::runtime_error("staff/bow item contributed melee radius");
    item.words[22]=1;item.words[39]=2;
    if(query(&item,&tables,2,&radius)!=Status::complete||radius!=3.25f)
        throw std::runtime_error("melee Item+0x9c contribution or AI radius changed");
    if(query(nullptr,&tables,-1,&radius)!=Status::complete||radius!=3.5f)
        throw std::runtime_error("invalid Character AI ID did not use source fallback row 8");
    if(query(nullptr,nullptr,2,&radius)!=Status::invalid_argument)
        throw std::runtime_error("missing AI rows accepted");
    std::puts("PASS: live Player melee radius uses current equipped-item source rule and AI fallback");
    return 0;
}
