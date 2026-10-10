#include "source_master.hpp"
#include "source_master_natives.hpp"
#include "../../../script-runtime/script_object_bridge.h"
#include <cassert>
#include <iostream>
using namespace dh::foundation::companions;
int main() {
    dh2::character::CharacterAiPointerFieldsV105 fields;
    dh2::data::AiTables tables;tables.rows.resize(51);tables.rows[50].view_radius=5;
    std::vector<std::uintptr_t> queried;std::string error;
    SourceMasterBorrow b;b.fields=&fields;b.same_character=10;b.tables=&tables;
    b.get_ai_id=[&](std::int32_t& id,std::string&){assert(fields.master50==20);id=50;return true;};
    b.is_dead=[&](std::uintptr_t id,std::uint32_t& dead,std::string&){assert(id==20);dead=0;return true;};
    b.target_position=[&](std::uintptr_t id,std::array<float,3>& position,std::string&){queried.push_back(id);position=id==20?std::array<float,3>{3,4,0}:std::array<float,3>{0,0,0};return true;};
    assert(set_source_master(b,20,error));assert(fields.master50==20&&fields.master_alive54==1&&fields.master_sight55==0); // strict equal radius boundary
    assert((queried==std::vector<std::uintptr_t>{20,10}));
    tables.rows[50].view_radius=6;assert(set_source_master(b,20,error));assert(fields.master_sight55==1);
    b.is_dead=[](std::uintptr_t,std::uint32_t& dead,std::string&){dead=1;return true;};
    assert(set_source_master(b,20,error));assert(fields.master_alive54==0&&fields.master_sight55==1);
    assert(set_source_master(b,0,error));assert(fields.master50==0&&fields.master_alive54==0&&fields.master_sight55==1);
    bool result=true;b.hosting_player_character={};assert(source_is_master_host(b,result,error)&&!result);
    b.get_ai_id={};assert(!set_source_master(b,20,error));assert(fields.master50==20&&!error.empty());
    dh2_script_value rejected{};rejected.type=DH2_SCRIPT_NUMBER;
    assert(source_set_master_values(b,&rejected,1,error)&&fields.master50==20);
    assert(source_set_master_values(b,nullptr,0,error)&&fields.master50==20);
    dh2_script_value clear{};clear.type=DH2_SCRIPT_SOURCE_OBJECT;clear.identity=0;
    assert(source_set_master_values(b,&clear,1,error)&&fields.master50==0);
    fields.master50=20;b.hosting_player_character=[](std::uintptr_t& host,std::string&){host=20;return true;};
    assert(source_is_master_host(b,result,error)&&result);
    b.hosting_player_character=[](std::uintptr_t& host,std::string&){host=30;return true;};
    assert(source_is_master_host(b,result,error)&&!result);
    SourceMasterNativeOwner native;native.lease=std::make_shared<int>(1);
    native.borrow=[&](SourceMasterBorrow& out,std::string&){out=b;return true;};
    dh2_script_function function{};void* context{};
    assert(select_source_master_native(&native,0x3b6f3c,&function,&context)==1);
    dh2_script_value values[1]{};std::uint32_t returned{};char diagnostic[256]{};
    assert(function(context,nullptr,0,values,1,&returned,diagnostic,sizeof diagnostic)==0);
    assert(returned==1&&values[0].type==DH2_SCRIPT_BOOLEAN&&values[0].boolean);
    assert(select_source_master_native(&native,0x3b9084,&function,&context)==1);
    assert(function(context,&clear,1,values,1,&returned,diagnostic,sizeof diagnostic)==0&&returned==0&&fields.master50==0);
    assert(select_source_master_native(&native,0x3b6fc4,&function,&context)==1);
    b.hosting_player_character={};assert(function(context,nullptr,0,values,1,&returned,diagnostic,sizeof diagnostic)==0&&!values[0].boolean);
    fields.master50=20;assert(function(context,nullptr,0,values,1,&returned,diagnostic,sizeof diagnostic)==DH2_SCRIPT_REQUIRED_SERVICE_FAILURE);
    assert(diagnostic[0]);
    assert(select_source_master_native(&native,0x3b6fc8,&function,&context)==0);
    std::cout<<"source master original pointer/alive/sight/strict-radius/prefix/wrapper tests passed\n";
}
