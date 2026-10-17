#include "../character_ai_classification.hpp"

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace classification=dh2::character_ai_classification;
namespace {
struct Fixture {
    classification::State state{0x10014000,0,0,nullptr,0};
    std::array<classification::AiRow,16> rows{},alternate{};
    classification::AiTable table{rows.data(),16},other{alternate.data(),16};
    const classification::AiTable* current=&table;
    std::int32_t count=16,faction_count=20;
    std::string name="PlayerCharacter",other_name="changed";
    unsigned mutation=0,fail_call=0,type_calls=0;
    std::vector<unsigned> trace;
    classification::Services services{this,invoke};
    static std::int32_t invoke(void* raw,classification::State* state,
            const classification::Request* request,classification::Response* response) {
        auto& f=*static_cast<Fixture*>(raw);
        f.trace.push_back(static_cast<unsigned>(request->operation));
        if(f.fail_call==f.trace.size())return 1;
        using Op=classification::Operation;
        switch(request->operation) {
        case Op::ai_table:
            response->table=f.current;
            ++f.type_calls;
            if(f.mutation==1){state->ai_id=1;f.current=&f.other;}
            if(f.mutation==4)f.rows[0].type=f.type_calls==1?5:f.type_calls==2?7:8;
            return 0;
        case Op::ai_count:
            response->count=f.count;
            if(f.mutation==2)state->ai_id=1;
            if(f.mutation==3)f.rows[0].type=9;
            return 0;
        case Op::faction_count:response->count=f.faction_count;return 0;
        case Op::find_player_name:
            response->match=std::strstr(request->name,"PlayerCharacter");
            if(f.mutation==5)state->name=f.other_name.c_str();
            return 0;
        }
        return 1;
    }
};
void require(bool value){if(!value)throw std::runtime_error("classification assertion");}
void guards() {
    Fixture f;for(auto& row:f.rows)row.type=4;f.state.name=f.name.c_str();
    classification::Result output{},saved{};
    f.state.character=0;output.word=77;saved=output;
    require(classification::query(classification::Query::monster,&f.state,&f.services,&output)==classification::Status::invalid_argument&&output.word==saved.word&&f.trace.empty());
    f.state.character=0x100000014ull;
    require(classification::query(classification::Query::monster,&f.state,&f.services,&output)==classification::Status::complete&&output.word==1);
    auto missing=f.services;missing.invoke=nullptr;
    require(classification::query(classification::Query::monster,&f.state,&missing,&output)==classification::Status::service_unavailable);
    f.state.ai_id=-1;
    require(classification::query(classification::Query::ai_id,&f.state,&missing,&output)==classification::Status::complete&&output.word==8&&output.calls==0);
    f.state.dead=255;
    require(classification::query(classification::Query::dead,&f.state,&missing,&output)==classification::Status::complete&&output.word==255&&output.calls==0);
    f.table.capacity=8;
    require(classification::query(classification::Query::type,&f.state,&f.services,&output)==classification::Status::invalid_source_fact);
    f.table.capacity=16;f.state.ai_id=0;f.rows[0].type=0;f.state.name=nullptr;
    require(classification::query(classification::Query::player,&f.state,&f.services,&output)==classification::Status::invalid_source_fact);
    output.word=91;
    require(classification::query(static_cast<classification::Query>(99),&f.state,&f.services,&output)==classification::Status::invalid_argument&&output.word==91);
    require(classification::query(classification::Query::type,&f.state,&f.services,reinterpret_cast<classification::Result*>(&f.state))==classification::Status::invalid_argument);
    std::cout<<"{\"guard_checks\":9}";
}
}
int main(int argc,char** argv) {
    try {
        if(argc==2&&std::strcmp(argv[1],"--guards")==0){guards();return 0;}
        if(argc!=12)return 2;
        Fixture f;
        const auto query=static_cast<classification::Query>(std::strtoul(argv[1],nullptr,0));
        f.state.ai_id=static_cast<std::int32_t>(std::strtoll(argv[2],nullptr,0));
        f.state.faction_id=static_cast<std::int32_t>(std::strtoll(argv[3],nullptr,0));
        f.count=static_cast<std::int32_t>(std::strtoll(argv[4],nullptr,0));
        f.faction_count=static_cast<std::int32_t>(std::strtoll(argv[5],nullptr,0));
        for(auto& row:f.rows){row.type=static_cast<std::int32_t>(std::strtoll(argv[6],nullptr,0));row.flags=static_cast<std::uint32_t>(std::strtoull(argv[7],nullptr,0));}
        f.rows[1].type=2;
        for(auto& row:f.alternate){row.type=9;row.flags=~0u;}
        f.name=argv[8];f.state.name=f.name.c_str();
        f.state.dead=static_cast<std::uint8_t>(std::strtoul(argv[9],nullptr,0));
        f.mutation=std::strtoul(argv[10],nullptr,0);f.fail_call=std::strtoul(argv[11],nullptr,0);
        classification::Result result{};
        const auto status=classification::query(query,&f.state,&f.services,&result);
        const auto address=reinterpret_cast<std::uintptr_t>(result.row);
        const auto first=reinterpret_cast<std::uintptr_t>(f.rows.data());
        const auto second=reinterpret_cast<std::uintptr_t>(f.alternate.data());
        const auto index=!address?-1:address>=first&&address<first+sizeof(f.rows)?
            std::int64_t((address-first)/sizeof(classification::AiRow)):
            address>=second&&address<second+sizeof(f.alternate)?
            100+std::int64_t((address-second)/sizeof(classification::AiRow)):-2;
        std::cout<<"{\"status\":"<<static_cast<int>(status)<<",\"word\":"<<result.word
            <<",\"row\":"<<index<<",\"ai_id\":"<<f.state.ai_id<<",\"trace\":[";
        for(std::size_t i=0;i<f.trace.size();++i){if(i)std::cout<<',';std::cout<<f.trace[i];}
        std::cout<<"]}";return 0;
    }catch(const std::exception& error){std::cerr<<error.what();return 1;}
}
