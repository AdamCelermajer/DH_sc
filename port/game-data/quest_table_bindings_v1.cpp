#include "quest_table_bindings_v1.hpp"
#include <array>
#include <cstring>
#include <vector>

namespace dh2::data::quest_table_bindings_v1 {
struct State {
    Input input;
    std::unique_ptr<std::uint8_t[]> anchors;
    std::vector<dh2_quest_record> records;
    std::vector<PyDataRef> rows;
    std::vector<std::array<ListRef,5>> lists;
    std::vector<std::array<StubRef,2>> stubs;
    std::vector<std::string> names;
};
namespace {
constexpr std::size_t stride=0x11c;
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn){
    auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    if(!x||!y||!an||!bn)return false;
    if(an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y)return true;
    return x<y+bn&&y<x+an;
}
template<class T> bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
bool reject(std::string& error,const char* text){error=text;return false;}
std::size_t index(const State& state,const PyDataRef& row){
    const auto base=reinterpret_cast<std::uintptr_t>(state.anchors.get());
    if(!base||row.identity<base||(row.identity-base)%stride)return state.rows.size();
    const auto i=(row.identity-base)/stride;
    return i<state.rows.size()&&&state.rows[i]==&row?i:state.rows.size();
}
template<class T> bool output(const State& s,const T* out,const View* view){
    if(!aligned(out)||overlap(out,sizeof(*out),view,sizeof(*view))||overlap(out,sizeof(*out),&s,sizeof(s))||
       overlap(out,sizeof(*out),s.input.table.bytes,s.input.table.size)||overlap(out,sizeof(*out),s.input.names,s.input.names_size)||
       overlap(out,sizeof(*out),s.anchors.get(),s.rows.size()*stride)||
       overlap(out,sizeof(*out),s.records.data(),s.records.size()*sizeof(dh2_quest_record))||
       overlap(out,sizeof(*out),s.rows.data(),s.rows.size()*sizeof(PyDataRef))||
       overlap(out,sizeof(*out),s.lists.data(),s.lists.size()*sizeof(s.lists[0]))||
       overlap(out,sizeof(*out),s.stubs.data(),s.stubs.size()*sizeof(s.stubs[0])))return false;
    for(const auto& name:s.names)if(overlap(out,sizeof(*out),name.data(),name.size()+1)||overlap(out,sizeof(*out),&name,sizeof(name)))return false;
    return true;
}
bool names(const Input& input,std::uint32_t count,std::vector<std::string>& out,std::string& error){
    if(!input.names||input.names_size<4||input.names_size>4u*1024u*1024u)return reject(error,"quest names span invalid");
    std::size_t at=0;
    auto word=[&](std::uint32_t& value){if(input.names_size-at<4)return false;const auto* p=input.names+at;value=std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);at+=4;return true;};
    std::uint32_t actual=0;if(!word(actual)||actual!=count)return reject(error,"quest name/table counts differ");
    out.reserve(count);
    for(std::uint32_t i=0;i<count;++i){std::uint32_t size=0;if(!word(size)||size>input.names_size-at)return reject(error,"quest name truncated");out.emplace_back(reinterpret_cast<const char*>(input.names+at),size);at+=size;}
    return at==input.names_size?true:reject(error,"quest names trailing bytes");
}
bool valid_list(const State& s,const ListRef& list,std::size_t& row){
    if(!aligned(list.row)||list.kind>=5)return false;
    row=index(s,*list.row);
    return row<s.rows.size()&&&s.lists[row][list.kind]==&list&&list.definition==&s.records[row].lists[list.kind];
}
} // namespace
bool Owner::load(const Input& input,std::string& error){
    if(!input.packed_owner||!input.names_owner)return reject(error,"quest packed/name ownership lease missing");
    dh2_quest_table table{};
    if(dh2_quests_open(&table,input.table.bytes,input.table.size)||table.count!=input.table.count)return reject(error,"quest packed table invalid or count changed");
    try{
        auto next=std::make_shared<State>();next->input=input;
        if(!names(input,table.count,next->names,error))return false;
        next->records.resize(table.count);next->rows.resize(table.count);next->lists.resize(table.count);next->stubs.resize(table.count);
        if(table.count)next->anchors=std::make_unique<std::uint8_t[]>(std::size_t(table.count)*stride);
        const auto base=reinterpret_cast<std::uintptr_t>(next->anchors.get());
        for(std::uint32_t i=0;i<table.count;++i){
            if(dh2_quests_record(&table,i,&next->records[i]))return reject(error,"quest decoded row invalid");
            next->rows[i].identity=base+std::size_t(i)*stride;
            for(std::uint32_t kind=0;kind<5;++kind)next->lists[i][kind]={&next->rows[i],kind,&next->records[i].lists[kind]};
            next->stubs[i][0]={&next->rows[i],0x3c,&next->records[i].accept};next->stubs[i][1]={&next->rows[i],0x68,&next->records[i].end};
        }
        state_=std::move(next);error.clear();return true;
    }catch(...){return reject(error,"quest table adaptation allocation failed");}
}
std::uint32_t View::count() const noexcept{return state_?std::uint32_t(state_->rows.size()):0;}
std::uintptr_t View::rows_identity() const noexcept{return state_?reinterpret_cast<std::uintptr_t>(state_->anchors.get()):0;}
const PyDataRef* View::row(std::uint32_t i) const noexcept{return state_&&i<state_->rows.size()?&state_->rows[i]:nullptr;}
const PyDataRef* View::resolve(std::uintptr_t identity) const noexcept{
    const auto base=rows_identity();if(!base||identity<base||(identity-base)%stride)return nullptr;
    const auto i=(identity-base)/stride;return i<count()?row(std::uint32_t(i)):nullptr;
}
const dh2_quest_record* View::record(const PyDataRef& row) const noexcept{
    if(!state_)return nullptr;
    const auto i=index(*state_,row);return i<state_->rows.size()?&state_->records[i]:nullptr;
}
const char* View::definition_name(std::uint32_t i) const noexcept{return state_&&i<state_->names.size()?state_->names[i].c_str():nullptr;}
const ListRef* View::list(const PyDataRef& row,std::uint32_t kind) const noexcept{
    if(!state_||kind>=5)return nullptr;
    const auto i=index(*state_,row);return i<state_->rows.size()?&state_->lists[i][kind]:nullptr;
}
const ListRef* View::resolve_list(std::uintptr_t identity) const noexcept{
    if(!state_||!identity)return nullptr;
    for(const auto& lists:state_->lists)for(const auto& list:lists)if(reinterpret_cast<std::uintptr_t>(&list)==identity)return &list;
    return nullptr;
}
const StubRef* View::resolve_stub(std::uintptr_t identity) const noexcept{
    if(!state_)return nullptr;
    const auto base=rows_identity();if(!base||identity<base)return nullptr;
    const auto relative=identity-base,i=relative/stride,offset=relative%stride;
    return i<count()&&(offset==0x3c||offset==0x68)?&state_->stubs[i][offset==0x68]:nullptr;
}
bool View::read_word(const PyDataRef& row,std::uint32_t offset,std::uintptr_t* out,std::string& error) const{
    if(!state_||!output(*state_,out,this))return reject(error,"quest field output aliases immutable backing");
    const auto* actual=record(row);if(!actual)return reject(error,"quest field row control foreign");
    std::uintptr_t value=0;
    if(offset>=0x14&&offset<=0x38&&offset%4==0){
        const auto kind=(offset-0x14)/8;
        // Original read allocates the source array header even for count0.
        // The real cache's two zero-condition rows therefore retain a nonnull
        // definition-list control; count0 must not manufacture a null pointer.
        value=(offset-0x14)%8==0?actual->lists[kind].count:
            reinterpret_cast<std::uintptr_t>(list(row,kind));
    }else if(offset==0x9c)value=std::uint32_t(actual->state);
    else if(offset==0x118)value=std::uint32_t(actual->act);
    else return reject(error,"quest field offset outside bound caller reads");
    *out=value;error.clear();return true;
}
bool View::list_record(const ListRef& list,std::uint32_t i,dh2_quest_span* out,std::string& error) const{
    if(!state_||!output(*state_,out,this))return reject(error,"quest list output aliases immutable backing");
    std::size_t row=0;if(!valid_list(*state_,list,row))return reject(error,"quest list control foreign");
    dh2_quest_span result{};
    if(dh2_quests_list_record(&state_->input.table,std::uint32_t(row),list.kind,i,&result))return reject(error,"quest list index invalid");
    *out=result;error.clear();return true;
}
bool View::objective(const ListRef& list,std::uint32_t i,dh2_quest_objective* out,std::string& error) const{
    if(!state_||!output(*state_,out,this))return reject(error,"quest objective output aliases immutable backing");
    std::size_t row=0;if(!valid_list(*state_,list,row)||list.kind!=1)return reject(error,"quest objective list control foreign");
    dh2_quest_objective result{};
    if(dh2_quests_objective(&state_->input.table,std::uint32_t(row),i,&result))return reject(error,"quest objective index invalid");
    *out=result;error.clear();return true;
}
bool View::bytes(const dh2_quest_span& span,Span* out,std::string& error) const{
    if(!state_||!output(*state_,out,this)||overlap(out,sizeof(*out),&span,sizeof(span)))return reject(error,"quest string output invalid or aliased");
    const auto& table=state_->input.table;
    if(span.offset>table.size||span.size>table.size-span.offset)return reject(error,"quest string span invalid");
    *out={table.bytes+span.offset,span.size};error.clear();return true;
}
} // namespace dh2::data::quest_table_bindings_v1
