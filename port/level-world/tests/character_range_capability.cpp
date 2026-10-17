#include "../character_range_capability.hpp"
#include "../character_ai_ranged_range.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
namespace k=dh2::character_range_capability;
unsigned word(int x){unsigned w;std::memcpy(&w,&x,4);return w;}
int signed_word(unsigned x){int w;std::memcpy(&w,&x,4);return w;}
struct Query {unsigned instance,id;};
struct Fixture {
    k::Properties props{};k::Instance instances[2]{{0},{1}};
    const k::Instance* refs[2]{&instances[0],&instances[1]};
    k::EquipSet sets[2]{{&refs[0]},{&refs[1]}},swapped[2]{{&refs[1]},{&refs[0]}};
    k::InventoryView view{sets,2,0};k::Inventory inventory{0x1001437c,&view,k::InventoryBinding::base_inventory,nullptr};
    k::Character character{0x10014000,&props,&inventory};k::Item items[2]{},next[2]{};
    k::ItemTable table{items,2};k::Result result{};k::Services services{this,capture};
    unsigned output[3]{123,234,345},alias=0,mutation=0,fail=0,throws=0;
    std::vector<Query> queries;
    Fixture() {
        props.words[30]=512;props.words[31]=1280;props.words[32]=-1;
        inventory.character_owner=&character;
        items[0].words[22]=4;items[0].words[38]=2;items[0].words[39]=5;items[0].words[40]=9;
        items[1].words[22]=5;items[1].words[38]=4;items[1].words[39]=8;items[1].words[40]=10;
        next[0]=items[0];next[1]=items[1];next[0].words[22]=2;next[1].words[22]=5;
    }
    static int capture(void* p,const k::Instance* instance,unsigned id,const k::ItemTable** out) {
        auto& f=*static_cast<Fixture*>(p);f.queries.push_back({unsigned(instance-&f.instances[0]),id});
        const auto count=f.queries.size();
        if(f.fail==count)return 9;
        if(f.throws==count)throw std::runtime_error("global table capture");
        if(count==1) {
            if(f.mutation==1)f.view.current_set=1;
            if(f.mutation==2)f.view.sets=f.swapped;
            if(f.mutation==3)f.refs[0]=&f.instances[1];
            if(f.mutation==4)f.instances[0].item_id=1;
            if(f.mutation==5)f.table.rows=f.next;
            if(f.mutation==6)f.items[0].words[22]=5;
            if(f.mutation==7)f.props.words[32]=7;
            if(f.mutation==8)f.sets[0].main_hand=nullptr;
            if(f.mutation==9)f.table.rows=reinterpret_cast<k::Item*>(&f.result);
            if(f.mutation==10)f.table.rows=&f.items[0],f.table.count=0;
            if(f.mutation==11)f.table.rows=reinterpret_cast<k::Item*>(&f.character);
        }
        if(count==2 && f.mutation==6)f.items[0].words[22]=2;
        *out=&f.table;return 0;
    }
    k::Status run(unsigned op) {
        unsigned* a=&output[0];unsigned* b=&output[1];unsigned* c=&output[2];
        if(alias==1)c=b;
        if(alias==2)b=c=a;
        if(alias==3)b=a;
        if(alias==4){a=reinterpret_cast<unsigned*>(&props.words[31]);b=reinterpret_cast<unsigned*>(&props.words[32]);c=reinterpret_cast<unsigned*>(&props.words[30]);}
        if(alias==5){a=reinterpret_cast<unsigned*>(&items[0].words[39]);b=reinterpret_cast<unsigned*>(&items[0].words[40]);c=reinterpret_cast<unsigned*>(&items[0].words[38]);}
        if(alias==6){a=&output[2];b=&output[0];c=&output[1];}
        if(alias==7)c=a;
        if(op==0)return k::character_parameters(&character,a,b,c,&services,&result);
        if(op==1)return k::inventory_parameters(&inventory,a,b,c,&services,&result);
        if(op==2)return k::has_ranged_weapon(&inventory,&services,&result);
        return k::character_can_range(&character,&services,&result);
    }
};
void values(const int* values,unsigned count){std::cout<<'[';for(unsigned i=0;i<count;++i){if(i)std::cout<<',';std::cout<<word(values[i]);}std::cout<<']';}
int main(int argc,char** argv) {
    if(argc>1) {
        assert(argc==12);unsigned x[11];for(unsigned i=0;i<11;++i)x[i]=std::strtoul(argv[i+1],nullptr,0);Fixture f;
        f.inventory.binding=x[1]?k::InventoryBinding::character_inventory:k::InventoryBinding::base_inventory;
        f.props.words[30]=signed_word(x[2]);f.props.words[31]=signed_word(x[3]);f.props.words[32]=signed_word(x[4]);
        f.items[0].words[22]=signed_word(x[5]);f.items[1].words[22]=signed_word(x[6]);f.alias=x[7];f.mutation=x[8];
        if(!x[9])f.sets[0].main_hand=nullptr;
        f.view.current_set=signed_word(x[10]);
        const auto status=f.run(x[0]);
        std::cout<<"{\"status\":"<<int(status)<<",\"value\":"<<f.result.value<<",\"stores\":"<<f.result.stores<<",\"outputs\":";
        values(reinterpret_cast<const int*>(f.output),3);std::cout<<",\"properties\":";values(f.props.words+30,3);
        std::cout<<",\"row0\":";values(f.items[0].words+38,3);std::cout<<",\"row1\":";values(f.items[1].words+38,3);
        std::cout<<",\"current\":"<<word(f.view.current_set)<<",\"instance0\":"<<word(f.instances[0].item_id)<<",\"queries\":[";
        bool first=true;for(auto query:f.queries){if(!first)std::cout<<',';first=false;std::cout<<'['<<query.instance<<','<<query.id<<']';}
        std::cout<<"]}\n";return 0;
    }
    unsigned cases=0;
    for(unsigned op=0;op<4;++op) {
        for(unsigned binding=0;binding<2;++binding){Fixture f;f.inventory.binding=binding?k::InventoryBinding::character_inventory:k::InventoryBinding::base_inventory;
            assert(f.run(op)==k::Status::complete && f.result.value==1);++cases;}
        {Fixture f;f.sets[0].main_hand=nullptr;assert(f.run(op)==k::Status::complete && !f.result.value && f.queries.empty());++cases;}
        {Fixture f;f.items[0].words[22]=1;assert(f.run(op)==k::Status::complete && !f.result.value && f.queries.size()==2);++cases;}
        {Fixture f;f.items[0].words[22]=5;assert(f.run(op)==k::Status::complete && f.result.value && f.queries.size()==unsigned(op<2?3:2));++cases;}
        for(unsigned mutation=1;mutation<=7;++mutation){Fixture f;f.mutation=mutation;assert(f.run(op)==k::Status::complete);++cases;}
        {Fixture f;f.items[0].words[22]=5;f.mutation=8;assert(f.run(op)==k::Status::invalid_source_fact);++cases;}
        {Fixture f;f.mutation=9;assert(f.run(op)==k::Status::invalid_source_fact);++cases;}
        {Fixture f;f.mutation=10;assert(f.run(op)==k::Status::invalid_source_fact);++cases;}
        {Fixture f;f.instances[0].item_id=-1;assert(f.run(op)==k::Status::invalid_source_fact);++cases;}
        {Fixture f;f.view.current_set=-1;assert(f.run(op)==k::Status::invalid_source_fact && f.queries.empty());++cases;}
        {Fixture f;f.view.current_set=128;assert(f.run(op)==k::Status::invalid_source_fact && f.queries.empty());++cases;}
        {Fixture f;f.services.capture_items=nullptr;assert(f.run(op)==k::Status::service_unavailable);++cases;}
        {Fixture f;f.fail=1;assert(f.run(op)==k::Status::service_failed && f.result.stores==0);++cases;}
        {Fixture f;f.items[0].words[22]=5;f.fail=2;assert(f.run(op)==k::Status::service_failed && f.result.stores==0);++cases;}
        {Fixture f;f.throws=1;assert(f.run(op)==k::Status::service_failed);++cases;}
        {Fixture f;assert(k::has_ranged_weapon(&f.inventory,&f.services,reinterpret_cast<k::Result*>(&f.services))==k::Status::invalid_argument);++cases;}
    }
    for(unsigned alias=0;alias<8;++alias){Fixture f;f.alias=alias;f.props.words[32]=9;assert(f.run(0)==k::Status::complete && f.result.stores==3 && f.queries.empty());++cases;}
    for(unsigned alias=0;alias<8;++alias){Fixture f;f.alias=alias;assert(f.run(1)==k::Status::complete && f.result.stores==3);++cases;}
    {Fixture f;f.props.words[32]=-2;f.character.inventory=nullptr;f.services.capture_items=nullptr;assert(f.run(0)==k::Status::complete && f.run(3)==k::Status::complete);++cases;}
    {Fixture f;f.inventory.binding=static_cast<k::InventoryBinding>(9);assert(f.run(1)==k::Status::unsupported_virtual && f.queries.empty());++cases;}
    {Fixture f;f.inventory.binding=k::InventoryBinding::character_inventory;f.props.words[32]=1;f.items[0].words[22]=1;
        assert(f.run(1)==k::Status::complete && f.result.value && f.queries.size()==1);++cases;}
    {Fixture f;f.props.words[32]=1;assert(k::character_parameters(&f.character,reinterpret_cast<unsigned*>(&f.result),f.output+1,f.output+2,&f.services,&f.result)==k::Status::invalid_argument);++cases;}
    {Fixture f;f.props.words[32]=1;f.character.properties=reinterpret_cast<k::Properties*>(&f.character);assert(f.run(0)==k::Status::invalid_source_fact);++cases;}
    // Source range caller invokes THIS maintained Character provider with actual
    // aliased pointers, not a separately supplied capability or parameter copy.
    for(bool close:{false,true}) {
        namespace g=dh2::character_ai_ranged_range;Fixture f;f.props.words[32]=9;
        g::State state{0x100143c8,0x10014000,0x10020000};g::Point owner{{0,0,0}},target{{0x40400000,0,0}};g::ResolvedObject resolved{0x10020000,0};
        struct Context{Fixture* f;g::Point* owner;g::Point* target;g::ResolvedObject* resolved;};Context context{&f,&owner,&target,&resolved};
        g::Services services{&context,[](void* p,g::State*,const g::Request* q,g::Response* r)->int {
            auto& c=*static_cast<Context*>(p);
            if(q->operation==g::Operation::resolve_object)r->view=c.resolved;
            else if(q->operation==g::Operation::interaction_type)r->word=8;
            else if(q->operation==g::Operation::can_range_attack) {
                k::Result result{};
                if(k::character_parameters(&c.f->character,q->minimum,q->maximum,q->projectile,&c.f->services,&result)!=k::Status::complete)return 1;
                r->word=result.value;
            } else if(q->operation==g::Operation::target_position)r->view=q->subject==0x10014000?c.owner:c.target;
            else if(q->operation==g::Operation::diagnostic_switch)r->identity=0x10038000;
            else return 1;
            return 0;
        }};g::Result result{};
        const auto status=close?g::evaluate_close(&state,0x10020000,&services,&result):g::evaluate_ranged(&state,0x10020000,&services,&result);
        assert(status==g::Status::complete && result.value==unsigned(!close) && result.minimum_word==2 && result.projectile_word==9);++cases;
    }
    std::cout<<"{\"validation\":\"PASS\",\"host_cases\":"<<cases<<",\"live_inventory_rows\":true,\"output_source_scalar_aliases\":true,\"genuine_virtual_routes\":true,\"range_caller_composition\":true,\"failure_guards\":true}\n";
}
