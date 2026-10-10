#include "loot_tables_v2.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>
using namespace dh2::data;
std::vector<std::uint8_t> read(const std::string& name) {
    std::ifstream file(name,std::ios::binary);
    return {std::istreambuf_iterator<char>(file),{}};
}
Bytes view(const std::vector<std::uint8_t>& bytes){return {bytes.data(),bytes.size()};}
int main(int argc,char** argv) {
    try {
        if(argc!=2) return 2;
        const std::string root=argv[1];
        auto records=read(root+"/loot_table_pyarray.bin");
        auto names=read(root+"/loot_table_pyarraynames.bin");
        auto schema=read(root+"/loot_table_pystructnames.bin");
        LootTablesV2 owner;std::string error;
        if(!owner.load(view(records),view(names),view(schema),error))throw std::runtime_error(error);
        const auto bank=owner.borrow();std::set<int> visited;
        auto visit=[&](auto&& self,int id)->void {
            if(!visited.insert(id).second)return;
            const auto& row=bank.loots().at(id);
            std::cout<<"LOOT\t"<<id<<'\t'<<bank.loot_names().at(id)<<'\t'<<row.roll_type<<'\t'<<row.num_random_item_probs
                     <<'\t'<<row.fixed_entries.size()<<'\t'<<row.random_entries.size()<<'\t'<<row.sub_loots.size()<<'\n';
            auto entries=[&](const auto& rows,const char* kind) {
                for(const auto& entry:rows) {
                    std::cout<<"ENTRY\t"<<kind;
                    for(auto word:entry.words)std::cout<<'\t'<<word;
                    std::cout<<'\n';
                    const auto list=entry.words[0];
                    std::cout<<"LIST\t"<<list<<'\t'<<bank.item_list_names().at(list)<<'\n';
                    for(const auto& selection:bank.item_lists().at(list)) {
                        const auto& item=bank.items().rows.at(selection.item);
                        std::cout<<"ITEM\t"<<selection.item<<'\t'<<bank.items().identifiers.at(selection.item)
                                 <<'\t'<<selection.probability<<'\t'<<unsigned(selection.quantity)
                                 <<'\t'<<item.name<<'\t'<<item.icon_name;
                        for(auto word:item.record.words)std::cout<<'\t'<<word;
                        std::cout<<'\n';
                    }
                }
            };
            entries(row.fixed_entries,"fixed");entries(row.random_entries,"random");
            for(auto sub:row.sub_loots){std::cout<<"SUB\t"<<sub<<'\n';self(self,sub);}
        };
        visit(visit,192);
        return 0;
    } catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
