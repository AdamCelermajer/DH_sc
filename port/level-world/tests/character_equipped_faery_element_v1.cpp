#define main retained_current_spell_fixture_main
#include "character_current_spell_v1.cpp"
#undef main
#include "../character_equipped_faery_element_v1.hpp"
namespace e=dh2::character_equipped_faery_element_v1;
struct ElementFixture {
    std::uint32_t id=0,element=0;int failure=-1;bool throws=false,mutate=false,nested=false;
    unsigned effects=0;int bad_row=0;e::FaeryRow row{};e::Result* output=nullptr;
    e::Bindings bindings{C,{this,invoke}};std::vector<std::array<std::int64_t,4>> trace;
    static int invoke(void* raw,const e::Request* q,e::Response* out){
        auto& f=*static_cast<ElementFixture*>(raw);require(q->character==C&&q->difficulty==-1,"captured owner/difficulty lost");
        f.trace.push_back({std::int64_t(q->operation),q->id,q->difficulty,1});const auto ordinal=f.effects++;
        if(q->operation==e::Operation::selected_faery){out->selected=signed_word(f.id);
            if(f.mutate){f.bindings.character=C+1;f.bindings.services.invoke=nullptr;}
            if(f.nested){f.nested=false;ElementFixture other;other.element=123;e::Result r{};
                require(e::query(C,&other.bindings.services,&r)==e::Status::complete&&r.element==123,"independent nested call failed");}}
        else {require(q->id==f.id,"selected ID was replaced before row query");f.row.words[2]=f.element;out->row=&f.row;
            if(f.bad_row==1)out->row=nullptr;
            if(f.bad_row==2)out->row=reinterpret_cast<const e::FaeryRow*>(reinterpret_cast<std::uintptr_t>(&f.row)+1);
            if(f.bad_row==3)out->row=reinterpret_cast<const e::FaeryRow*>(f.output);}
        if(f.failure==int(ordinal)){if(f.throws)throw std::runtime_error("element provider failed after effect");return -1;}return 0;
    }
    void print(const e::Result& r,e::Status status){
        std::cout<<"{\"status\":"<<int(status)<<",\"selected\":"<<r.selected<<",\"element\":"<<r.element<<",\"calls\":"<<r.calls<<",\"complete\":"<<r.complete<<",\"trace\":[";
        for(std::size_t i=0;i<trace.size();++i){if(i)std::cout<<',';std::cout<<'[';for(unsigned n=0;n<4;++n){if(n)std::cout<<',';std::cout<<trace[i][n];}std::cout<<']';}std::cout<<"]}\n";
    }
};
int main(int argc,char** argv){try{
    if(argc==5&&std::string(argv[1])=="--oracle"){
        ElementFixture f;f.id=word(argv[2]);f.element=word(argv[3]);f.mutate=word(argv[4])!=0;e::Result r{};
        auto status=e::query(C,&f.bindings.services,&r);f.print(r,status);return 0;
    }
    require(argc>=2,"cache argument missing");Catalogue catalogue(argv[1]);
    if(argc==3&&std::string(argv[2])=="--view"){catalogue.describe();return 0;}
    if(argc==7&&std::string(argv[2])=="--borrowed"){
        Borrowed b(catalogue,word(argv[3]));b.difficulty=signed_word(word(argv[4]));b.selection(word(argv[5]));b.mutation=word(argv[6]);
        auto services=e::saved_services(&b.borrowed);e::Result r{};auto status=e::query(C,&services,&r);
        std::cout<<"{\"status\":"<<int(status)<<",\"selected\":"<<r.selected<<",\"element\":"<<r.element<<",\"calls\":"<<r.calls<<",\"queries\":"<<b.queries<<",\"complete\":"<<r.complete<<"}\n";return 0;
    }
    unsigned valid=0,failures=0,guards=0,lua=0;
    for(unsigned class_id=0;class_id<3;++class_id)for(unsigned diff=0;diff<3;++diff)for(unsigned id=0;id<5;++id){
        Borrowed b(catalogue,class_id);b.difficulty=int(diff);b.selection(id);auto services=e::saved_services(&b.borrowed);e::Result r{};
        const auto& tables=catalogue.tables->source_faeries();const auto row=tables.list_rows[b.list].members[id];
        require(e::query(C,&services,&r)==e::Status::complete&&r.element==signed_word(tables.faery_rows[row].words[2])&&r.calls==2&&b.queries==2,"actual selected faery element differs");++valid;
    }
    for(int phase=0;phase<2;++phase)for(bool throws:{false,true}){ElementFixture f;f.failure=phase;f.throws=throws;e::Result r{};
        require(e::query(C,&f.bindings.services,&r)==e::Status::provider_failed&&r.calls==unsigned(phase+1)&&f.effects==r.calls&&!r.complete,"provider failure lost prefix or continued");++failures;}
    for(auto value:{0u,1u,0xffffffffu,0x80000000u,0x7fffffffu,16777217u}){ElementFixture f;f.element=value;f.mutate=true;e::Result r{};
        require(e::query(C,&f.bindings.services,&r)==e::Status::complete&&r.element==signed_word(value)&&r.character==C,"signed row read/owner capture differs");++valid;}
    {ElementFixture f;f.nested=true;e::Result r{};require(e::query(C,&f.bindings.services,&r)==e::Status::complete,"nested distinct result rejected");++valid;}
    {ElementFixture f;e::Result r{};r.element=77;const auto old=r;
        auto invalid=[&](e::Status status){require(status==e::Status::invalid_argument&&!std::memcmp(&r,&old,sizeof(r))&&!f.effects,"invalid query changed outputs/effects");++guards;};
        invalid(e::query(0,&f.bindings.services,&r));invalid(e::query(C,nullptr,&r));
        invalid(e::query(C,&f.bindings.services,reinterpret_cast<e::Result*>(UINTPTR_MAX-7)));
        invalid(e::query(C,&f.bindings.services,reinterpret_cast<e::Result*>(reinterpret_cast<std::uintptr_t>(&r)+1)));
        require(e::query(C,&f.bindings.services,reinterpret_cast<e::Result*>(&f.bindings.services))==e::Status::invalid_argument&&!f.effects,"service/result alias accepted");++guards;
        f.bindings.services.invoke=nullptr;invalid(e::query(C,&f.bindings.services,&r));}
    for(int bad=1;bad<=3;++bad){ElementFixture f;f.bad_row=bad;e::Result r{};f.output=&r;
        require(e::query(C,&f.bindings.services,&r)==e::Status::provider_failed&&r.calls==2&&!r.complete,"invalid returned row read");++guards;}
    Borrowed b(catalogue,0,false);auto services=e::saved_services(&b.borrowed);e::Bindings bindings{C,services};e::Result r{};
    // Unlike saved level delivery, this callback never reads faery level rows.
    require(e::query(C,&services,&r)==e::Status::complete&&r.selected==0,"constructor selection invented a level-row dependency");++valid;
    b.selected=nullptr;b.borrowed.difficulty=nullptr;require(e::query(C,&services,&r)==e::Status::complete&&r.selected==0,"null save read difficulty");++valid;
    b.borrowed.difficulty=&b.difficulty;b.selected=&b.save;b.alternate.set_character(C+1);b.selected=&b.alternate;
    require(e::query(C,&services,&r)==e::Status::provider_failed&&r.calls==1,"foreign saved owner accepted");++guards;b.selected=&b.save;
    b.difficulty=3;require(e::query(C,&services,&r)==e::Status::provider_failed&&r.calls==1,"unsafe difficulty accepted");++guards;b.difficulty=0;
    b.list=-1;require(e::query(C,&services,&r)==e::Status::complete,"original list0 fallback rejected");++valid;b.list=catalogue.selector(0);
    b.borrowed.faery_list_106c=reinterpret_cast<const std::int32_t*>(reinterpret_cast<std::uintptr_t>(&b.list)+1);
    require(e::query(C,&services,&r)==e::Status::provider_failed&&r.calls==2,"misaligned list read");++guards;b.borrowed.faery_list_106c=&b.list;
    b.fail_constant=true;auto before=b.queries;require(e::query(C,&services,&r)==e::Status::provider_failed&&r.calls==2&&b.queries==before+1,"constant error skipped/extended prefix");++failures;b.fail_constant=false;
    {ElementFixture f;dh2_script_value out{};std::uint32_t count=99;char error[128]{};
        require(!e::equipped_faery_element_v1(&f.bindings,reinterpret_cast<const dh2_script_value*>(1),UINT32_MAX,&out,1,&count,error,sizeof(error))&&count==1,"ignored Arguments read");++valid;
        f.effects=0;count=99;require(e::equipped_faery_element_v1(&f.bindings,nullptr,0,reinterpret_cast<dh2_script_value*>(&f.bindings),1,&count,error,sizeof(error))==-1001&&count==99&&!f.effects,"callback output/control alias accepted");++guards;
        require(e::equipped_faery_element_v1(&f.bindings,nullptr,0,&out,1,reinterpret_cast<std::uint32_t*>(&f.bindings),error,sizeof(error))==-1001&&!f.effects,"returned/control alias accepted");++guards;}
    require(e::query(C,&services,&r)==e::Status::complete,"real element reset failed");
    using Vm=std::unique_ptr<dh2_script_vm,decltype(&dh2_script_vm_destroy)>;
    Vm vm(dh2_script_vm_create(1024*1024),dh2_script_vm_destroy);require(bool(vm),"sole real Lua VM unavailable");auto* identity=vm.get();
    require(!dh2_script_vm_bind_source_values(vm.get(),"GetEquippedFaeryElement",e::equipped_faery_element_v1,&bindings),"source callback bind failed");
    const char* script="calls=0; function element() calls=calls+1; return GetEquippedFaeryElement({},false,'ignored') end; function caught_element() return pcall(GetEquippedFaeryElement),calls end";
    require(!dh2_script_vm_load_source_file(vm.get(),script,std::strlen(script)),"real Lua fixture failed");
    for(unsigned i=0;i<3;++i){Observation o;require(!dh2_script_vm_call_all_source_v1(vm.get(),"element",nullptr,0,Observation::observe,&o)&&o.count==1&&o.number==float(r.element)&&vm.get()==identity,"same-VM element query/recreation changed backing");++lua;}
    b.fail_constant=true;auto epoch=dh2_script_vm_required_failure_epoch(vm.get());Observation o;
    require(dh2_script_vm_call_all_source_v1(vm.get(),"caught_element",nullptr,0,Observation::observe,&o)==-5&&o.count==2&&!o.first&&o.calls==1&&dh2_script_vm_required_failure_epoch(vm.get())==epoch+1,"pcall hid required element failure");++lua;
    b.fail_constant=false;require(!dh2_script_vm_call_all_source_v1(vm.get(),"element",nullptr,0,Observation::observe,&o)&&vm.get()==identity,"retained VM failed recovery");++lua;
    vm.reset();
    std::cout<<"{\"validation\":\"PASS\",\"actual_data_cases\":"<<valid<<",\"failure_cases\":"<<failures<<",\"guards\":"<<guards<<",\"real_lua_cases\":"<<lua<<",\"same_vm\":true,\"native_wired\":false}\n";return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
