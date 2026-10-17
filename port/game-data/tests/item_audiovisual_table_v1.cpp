#ifdef NDEBUG
#undef NDEBUG
#endif
#include "../item_audiovisual_table_v1.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace dh2::data;
namespace {
unsigned checks=0;
void ck(bool value) {
    ++checks;
    if(!value)throw std::runtime_error("ItemAudioVisual check "+std::to_string(checks));
}
std::vector<std::uint8_t> file(const std::string& path) {
    std::ifstream input(path,std::ios::binary);ck(bool(input));
    return {std::istreambuf_iterator<char>(input),{}};
}
}

int main(int argc,char** argv) {
    try {
        ck(argc==2);
        const std::string root=std::string(argv[1])+"/";
        auto records=file(root+"loot_audiovisual_pyarray.bin");
        auto names=file(root+"loot_audiovisual_pyarraynames.bin");
        auto schema=file(root+"loot_audiovisual_pystructnames.bin");
        ItemAudioVisualTableV1 owner;std::string error;
        ck(owner.load({records.data(),records.size()},{names.data(),names.size()},
                      {schema.data(),schema.size()},error));
        auto view=owner.borrow();ck(bool(view));ck(view.rows().size()==29);
        const auto* axe=view.get(0);ck(axe&&axe->identifier=="Axe");
        ck(axe->audio_drop==64&&axe->audio_pickup==157);
        ck(axe->visual=="dummy_itemdrop_BattleAxe");
        const auto* bag=view.get(1);ck(bag&&bag->identifier=="Bag");
        ck(bag->audio_drop==62&&bag->audio_pickup==155);
        ck(bag->visual=="dummy_itemdrop_bag");
        ck(view.id("Axe")==0&&view.id("Bag")==1&&view.id("Missing")==-1);
        ck(view.get(-1)==nullptr&&view.get(29)==nullptr);
        for(const auto& row:view.rows())ck(!row.identifier.empty()&&!row.visual.empty());

        auto bad_schema=schema;ck(bad_schema.size()>8);bad_schema[8]^=1;
        ck(!owner.load({records.data(),records.size()},{names.data(),names.size()},
                       {bad_schema.data(),bad_schema.size()},error));
        ck(!error.empty());
        const auto retained=owner.borrow();ck(retained.rows().size()==29);
        ck(retained.get(0)->visual=="dummy_itemdrop_BattleAxe");

        records.pop_back();
        ck(!owner.load({records.data(),records.size()},{names.data(),names.size()},
                       {schema.data(),schema.size()},error));
        ck(owner.borrow().rows().size()==29);
        std::cout<<"{\"validation\":\"PASS\",\"rows\":29,\"source_audio_visual_id\":0,"
                    "\"first_visual\":\"dummy_itemdrop_BattleAxe\",\"source_fields\":[\"AudioDrop\",\"AudioPickup\",\"Visual\"],"
                    "\"atomic_failure_checks\":2,\"checks\":"<<checks<<"}\n";
        return 0;
    } catch(const std::exception& failure) {
        std::cerr<<failure.what()<<'\n';return 1;
    }
}
