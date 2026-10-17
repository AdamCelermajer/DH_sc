#include "quest_savegame_v1.hpp"
#include <cstddef>
#include <limits>

namespace dh2::data::quest_savegame_v1 {
namespace {
bool overlap(const void* a,std::size_t an,const void* b,std::size_t bn) {
    auto x=reinterpret_cast<std::uintptr_t>(a),y=reinterpret_cast<std::uintptr_t>(b);
    if(!x||!y||!an||!bn)return false;
    if(an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y)return true;
    return x<y+bn&&y<x+an;
}
template<class T> bool aligned(const T* value){return value&&reinterpret_cast<std::uintptr_t>(value)%alignof(T)==0;}
bool output_valid(const Result* out,const Runtime* runtime,const QuestSavegame& save) {
    if(!aligned(out)||overlap(out,sizeof(*out),runtime,sizeof(*runtime))||overlap(out,sizeof(*out),&save,sizeof(save))||
       overlap(out,sizeof(*out),&save.word_44,sizeof(save.word_44)))return false;
    for(const auto& vector:save.quests){
        if(overlap(out,sizeof(*out),vector.data(),vector.size()*sizeof(QuestRef*)))return false;
        for(const auto* quest:vector)if(quest){
            if(!aligned(quest)||!aligned(quest->fields))return false;
            if(overlap(out,sizeof(*out),quest,sizeof(*quest))||overlap(out,sizeof(*out),quest->fields,sizeof(*quest->fields)))return false;
        }
    }
    return true;
}
template<class F,class... A> bool send(Result& r,Operation op,const Services& s,F fn,A... args) {
    r.last_operation=op;
    if(!fn){r.status=Status::service_unavailable;return false;}
    ++r.service_calls;
    try{if(fn(s.context,args...)==0)return true;}catch(...){}
    r.status=Status::service_failed;return false;
}
bool valid(QuestRef* quest,std::uintptr_t identity,const QuestSavegame& save,const Result* out,Result& r) {
    if(!aligned(quest)||!aligned(quest->fields)||!identity||quest->identity!=identity){r.status=Status::missing_projection;return false;}
    if(overlap(quest,sizeof(*quest),&save,sizeof(save))||overlap(quest->fields,sizeof(*quest->fields),&save,sizeof(save))||
       overlap(quest,sizeof(*quest),&save.word_44,sizeof(save.word_44))||overlap(quest->fields,sizeof(*quest->fields),&save.word_44,sizeof(save.word_44))||
       overlap(out,sizeof(*out),quest,sizeof(*quest))||overlap(out,sizeof(*out),quest->fields,sizeof(*quest->fields))||
       overlap(quest,sizeof(*quest),quest->fields,sizeof(*quest->fields))){r.status=Status::invalid_argument;return false;}
    for(const auto& vector:save.quests){
        if(overlap(quest,sizeof(*quest),vector.data(),vector.size()*sizeof(QuestRef*))||
           overlap(quest->fields,sizeof(*quest->fields),vector.data(),vector.size()*sizeof(QuestRef*))){r.status=Status::invalid_argument;return false;}
        for(const auto* existing:vector)if(existing&&(existing==quest||existing->identity==identity||existing->fields==quest->fields)){
            r.status=Status::missing_projection;return false;
        }
    }
    return true;
}
struct Guard{bool& busy;~Guard(){busy=false;}};
} // namespace
Status Runtime::construct(Result* out) {
    if(!output_valid(out,this,save_))return Status::invalid_argument;
    if(busy_){out->status=Status::reentrant;return out->status;}
    Result r;r.last_operation=Operation::construct;
    for(const auto& vector:save_.quests)if(!vector.empty()){r.status=Status::invalid_argument;*out=r;return r.status;}
    save_.character_5c=0;
    for(unsigned i=0;i<3;++i){save_.word_2c[i]=-1;save_.word_38[i]=-1;save_.word_44[i]=1;save_.word_50[i]=1;save_.byte_28[i]=0;}
    *out=r;return r.status;
}
Status Runtime::init_quests(Result* out) {
    if(!output_valid(out,this,save_))return Status::invalid_argument;
    if(busy_){out->status=Status::reentrant;return out->status;}
    busy_=true;Guard guard{busy_};Result r;
    auto run=[&]{
        for(unsigned difficulty=0;difficulty<3;++difficulty){
            r.difficulty=difficulty;r.row=0;
            auto& vector=save_.quests[difficulty];const auto captured_length=vector.size();
            std::uint32_t count=0;
            if(!send(r,Operation::table_count,services_,services_.table_count,&count))return;
            if(captured_length){
                for(std::uint32_t row=0;row<count;++row){
                    r.row=row;r.last_operation=Operation::reinit;
                    if(row>=vector.size()||!vector[row]){r.status=Status::source_fault;return;}
                    if(!send(r,Operation::reinit,services_,services_.reinit,vector[row]))return;
                    ++r.reinitialized;
                }
                continue;
            }
            r.last_operation=Operation::resize;
            if(count>vector.max_size()||count>std::uint32_t(INT32_MAX)/0x11cu){r.status=Status::unsafe_storage;return;}
            try{vector.resize(count,nullptr);}catch(...){r.status=Status::unsafe_storage;return;}
            std::uint32_t byte_offset=0;
            for(std::uint32_t row=0;row<count;++row,byte_offset+=0x11c){
                r.row=row;std::uintptr_t table=0;
                if(!send(r,Operation::table_rows,services_,services_.table_rows,&table))return;
                if(!table||table>UINTPTR_MAX-byte_offset){r.status=Status::missing_projection;return;}
                const auto actual_row=table+byte_offset;
                r.allocation=0;
                if(!send(r,Operation::allocate,services_,services_.allocate,std::uint32_t{0x6c},std::uint32_t{0},&r.allocation))return;
                if(!r.allocation){r.status=Status::source_fault;return;}
                QuestRef* quest=nullptr;
                if(!send(r,Operation::quest_construct,services_,services_.quest_construct,r.allocation,std::int32_t(difficulty),&quest)||
                   !valid(quest,r.allocation,save_,out,r))return;
                r.last_operation=Operation::bind_character;quest->fields->character_60=save_.character_5c;
                if(!send(r,Operation::owner_children,services_,services_.owner_children,quest))return;
                std::uintptr_t name=0;
                if(!send(r,Operation::definition_name,services_,services_.definition_name,row,&name))return;
                r.last_operation=Operation::bind_row;quest->fields->row_8=std::int32_t(row);quest->fields->definition_name_14=name;
                if(!send(r,Operation::assign_pydata,services_,services_.assign_pydata,quest,actual_row))return;
                if(!send(r,Operation::reinit,services_,services_.reinit,quest))return;
                ++r.reinitialized;r.last_operation=Operation::publish;
                // Actual providers may update Character fields, but they must
                // not replace or resize this caller's authoritative vector.
                if(vector.size()!=count||vector[row]){r.status=Status::unsafe_storage;return;}
                vector[row]=quest;++r.published;
            }
        }
    };
    run();*out=r;return r.status;
}
Status Runtime::destroy(Result* out) {
    if(!output_valid(out,this,save_))return Status::invalid_argument;
    if(busy_){out->status=Status::reentrant;return out->status;}
    busy_=true;Guard guard{busy_};Result r;
    auto run=[&]{
        for(unsigned difficulty=0;difficulty<3;++difficulty){
            r.difficulty=difficulty;auto& vector=save_.quests[difficulty];const auto length=vector.size();
            for(std::size_t row=0;row<length;++row){
                r.row=std::uint32_t(row);
                if(row>=vector.size()){r.status=Status::unsafe_storage;return;}
                auto* quest=vector[row];if(!quest)continue;
                const auto identity=quest->identity;
                if(!send(r,Operation::quest_destruct,services_,services_.quest_destruct,quest))return;
                if(!send(r,Operation::deallocate,services_,services_.deallocate,identity))return;
                if(row>=vector.size()||vector[row]!=quest){r.status=Status::unsafe_storage;return;}
                vector[row]=nullptr;++r.destroyed;
            }
        }
        r.last_operation=Operation::clear_vectors;
        for(unsigned i=3;i;--i)std::vector<QuestRef*>().swap(save_.quests[i-1]);
    };
    run();*out=r;return r.status;
}
} // namespace dh2::data::quest_savegame_v1
