#include "inventory_feature.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
using namespace dh::foundation;
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){
    assert(argc==2);const std::string root=argv[1];
    auto records=read(root+"/loot_table_pyarray.bin"),names=read(root+"/loot_table_pyarraynames.bin"),fields=read(root+"/loot_table_pystructnames.bin");
    dh2::data::ItemTable table;std::string error;
    assert(dh2::data::load_items({records.data(),records.size()},{names.data(),names.size()},{fields.data(),fields.size()},table,error));
    std::string stack,bare,gold;
    for(std::size_t i=0;i<table.rows.size();++i){const auto& row=table.rows[i];auto type=dh2::data::item_type(row);
        if(type==13&&gold.empty())gold=table.identifiers[i];
        if(type!=13&&row.record.words[7]&&stack.empty())stack=table.identifiers[i];
        if(type!=13&&!row.record.words[7]&&bare.empty())bare=table.identifiers[i];
    }
    assert(!stack.empty()&&!bare.empty()&&!gold.empty());
    auto owner=make_default_character();inventory::Presenter p(owner,table);inventory::View view;std::string retained;
    assert(p.pickup({"a",stack,2},retained,error)&&retained=="a");
    assert(p.pickup({"b",stack,3},retained,error)&&retained=="a"&&owner.inventory.size()==1&&owner.inventory[0].quantity==5);
    assert(!p.pickup({"a",stack,1},retained,error)&&owner.inventory[0].quantity==5);
    assert(!p.pickup({"bad\n",stack,1},retained,error)&&owner.inventory[0].quantity==5);
    assert(!p.pickup({"g",gold,1},retained,error));
    assert(!p.pickup({"missing","UNKNOWN-SOURCE-ITEM",1},retained,error));
    assert(!p.pickup({"z",stack,0},retained,error));
    assert(p.pickup({"c",bare,1},retained,error)&&p.pickup({"d",bare,1},retained,error)&&owner.inventory.size()==3);
    assert(p.select("c",error));p.sort(inventory::Sort::name);assert(!p.present(view,error));p.sort(inventory::Sort::definition);assert(p.present(view,error));
    assert(view.rows.size()==3&&owner.inventory[0].instance_id=="a");
    bool selected=false;for(const auto& row:view.rows){if(row.instance_id=="c")selected=row.selected;assert(row.icon_name==dh2::data::item(table,dh2::data::item_id(table,row.definition_id))->icon_name);}assert(selected);
    assert(!p.select("unknown",error));
    owner.equipment.push_back({"slot1","c"});InventoryItem removed;
    assert(!p.drop("c",1,removed,error));owner.equipment.clear();
    assert(!p.drop("a",6,removed,error));assert(!p.drop("a",0,removed,error));
    assert(p.drop("a",2,removed,error)&&removed.quantity==2&&owner.inventory[0].quantity==3);
    assert(p.drop("c",1,removed,error)&&p.present(view,error));for(const auto& row:view.rows)assert(!row.selected);
    owner.inventory[0].quantity=std::numeric_limits<std::uint32_t>::max();
    assert(!p.pickup({"e",stack,1},retained,error)&&owner.inventory[0].quantity==std::numeric_limits<std::uint32_t>::max());
    assert(p.credit_gold(10,error)&&p.debit_gold(3,error)&&owner.gold==7);assert(!p.debit_gold(8,error)&&owner.gold==7);
    owner.gold=std::numeric_limits<std::uint64_t>::max();assert(!p.credit_gold(1,error)&&owner.gold==std::numeric_limits<std::uint64_t>::max());
    // Failed presentation commits no partial frame when an upstream definition disappears.
    owner.inventory[0].definition_id="UNKNOWN-SOURCE-ITEM";view.gold=123;
    assert(!p.present(view,error)&&view.gold==123);
    std::cout<<"inventory feature passed against "<<table.rows.size()<<" original ItemTable rows\n";
}
