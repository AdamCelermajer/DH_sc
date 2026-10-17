#include "modular_skin_catalog.hpp"
#include "../engine-resources/resources.hpp"
#include <algorithm>
#include <cstring>
#include <set>
#include <stdexcept>

namespace dh2::skinning {
namespace {
struct Reader {
    const resources::BresView& view;

    const std::uint8_t* at(std::uint64_t offset,std::uint64_t count) const {
        if(!view.bytes||offset>view.size||count>view.size-offset)
            throw std::runtime_error("Modular skin field outside BRES");
        return view.bytes+offset;
    }
    std::uint32_t word(std::uint64_t offset) const {
        const auto* p=at(offset,4);
        return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|
               (std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
    }
    void array(std::uint64_t offset,std::uint32_t count,std::uint32_t stride) const {
        if(count>100000)throw std::runtime_error("Modular skin array exceeds limit");
        at(offset,std::uint64_t(count)*stride);
    }
    std::string text(std::uint32_t offset) const {
        if(!offset)throw std::runtime_error("Missing modular skin string");
        const auto* p=at(offset,1);
        const auto* end=static_cast<const std::uint8_t*>(
            std::memchr(p,0,std::min<std::size_t>(4096,view.size-offset)));
        if(!end)throw std::runtime_error("Unterminated modular skin string");
        return {reinterpret_cast<const char*>(p),std::size_t(end-p)};
    }
    std::string field(std::uint64_t offset) const {return text(word(offset));}
};

struct ModularInstance {
    const Reader* reader{};
    std::uint32_t record{};
    unsigned matches{};
    std::vector<std::uint32_t> path;

    void node(std::uint32_t offset,unsigned depth) {
        if(depth>=64||path.size()>=64||
           std::find(path.begin(),path.end(),offset)!=path.end())
            throw std::runtime_error("Cyclic or excessive Prince scene graph");
        const auto& r=*reader;r.at(offset,80);path.push_back(offset);
        const auto id=r.field(offset);
        const auto count=r.word(offset+64),instances=r.word(offset+68);
        r.array(instances,count,8);
        if(id=="prince_modular-node")for(unsigned i=0;i<count;++i){
            const auto instance=instances+8*i;
            if(r.word(instance)!=13)continue;
            ++matches;record=r.word(instance+4);r.at(record,16);
        }
        const auto child_count=r.word(offset+56),children=r.word(offset+60);
        r.array(children,child_count,80);
        for(unsigned i=0;i<child_count;++i)node(children+80*i,depth+1);
        path.pop_back();
    }
};

std::uint32_t controller_index(const Reader& r,const resources::BresView& view,
                               const std::string& controller) {
    const auto count=dh2_bres_library_count(&view,resources::Library::controller);
    std::uint32_t result=0;unsigned matches=0;
    for(unsigned i=0;i<count;++i){
        const auto* bytes=dh2_bres_library_item(&view,resources::Library::controller,
                                                static_cast<std::int32_t>(i));
        if(!bytes)throw std::runtime_error("Missing modular controller record");
        const auto offset=std::uint32_t(bytes-view.bytes);r.at(offset,12);
        if(r.word(offset)!=0||r.field(offset+4)!=controller)continue;
        result=i;++matches;
    }
    if(matches!=1)throw std::runtime_error(matches?
        "Ambiguous modular controller: "+controller:
        "Modular controller absent from Prince BRES: "+controller);
    return result;
}

} // namespace

bool load_modular_skin_catalog(const std::uint8_t* model,std::size_t size,
                               ModularSkinCatalog& out,std::string& error) {
    out={};error.clear();
    try {
        if(!model)throw std::runtime_error("Missing Prince modular model");
        resources::BresView view{};
        if(dh2_bres_open(&view,model,size)!=resources::BresError::ok)
            throw std::runtime_error("Prince modular BRES rejected");
        Reader r{view};const auto root=view.root_offset;r.at(root,192);
        const auto scene_count=r.word(root+152),scenes=r.word(root+156);
        r.array(scenes,scene_count,16);
        const auto instance_count=r.word(root+184),instances=r.word(root+188);
        r.array(instances,instance_count,8);
        ModularInstance found{&r,0,0,{}};
        for(unsigned i=0;i<instance_count;++i){
            const auto instance=instances+8*i;
            if(r.word(instance)!=6)throw std::runtime_error("Unsupported Prince scene instance");
            const auto visual=r.word(instance+4);r.at(visual,8);
            if(r.word(visual))throw std::runtime_error("External Prince visual scene");
            auto scene_id=r.field(visual+4);
            if(scene_id.empty()||scene_id[0]!='#')
                throw std::runtime_error("Invalid Prince visual scene URI");
            scene_id.erase(0,1);
            for(unsigned j=0;j<scene_count;++j){
                const auto scene=scenes+16*j;
                if(r.field(scene)!=scene_id)continue;
                const auto count=r.word(scene+8),nodes=r.word(scene+12);
                r.array(nodes,count,80);
                for(unsigned k=0;k<count;++k)found.node(nodes+80*k,0);
            }
        }
        if(found.matches!=1)throw std::runtime_error(found.matches?
            "Prince modular node has multiple modular records":
            "Prince modular node has no type-13 modular record");

        const auto record=found.record;const auto category_count=r.word(record);
        const auto categories=r.word(record+4);r.array(categories,category_count,16);
        if(!category_count||category_count>32)
            throw std::runtime_error("Prince modular category count rejected");
        ModularSkinCatalog candidate;std::set<std::string> category_names,keys;
        for(unsigned category_id=0;category_id<category_count;++category_id){
            const auto category=categories+16*category_id;
            const auto module_count=r.word(category+8),modules=r.word(category+12);
            r.array(modules,module_count,8);
            if(!module_count||module_count>512)
                throw std::runtime_error("Prince modular module count rejected");
            std::string category_name;
            for(unsigned module_id=0;module_id<module_count;++module_id){
                const auto entry=modules+8*module_id;
                if(r.word(entry)!=2)throw std::runtime_error("Unsupported Prince modular module kind");
                const auto module_record=r.word(entry+4);r.at(module_record,8);
                auto uri=r.text(r.word(module_record+4));
                if(uri.size()<12||uri[0]!='#'||
                   uri.compare(uri.size()-10,10,"-mesh-skin")!=0)
                    throw std::runtime_error("Invalid Prince modular controller URI");
                auto module=uri.substr(1,uri.size()-11);
                const auto split=module.find('_',3);
                if(module.rfind("MC_",0)!=0||split==std::string::npos||split<=3)
                    throw std::runtime_error("Invalid Prince modular category/module name");
                const auto current_category=module.substr(0,split);
                if(category_name.empty())category_name=current_category;
                if(category_name!=current_category)
                    throw std::runtime_error("Prince modular category mixes module names");
                if(!keys.insert(category_name+"\n"+module).second)
                    throw std::runtime_error("Duplicate Prince modular category/module pair");
                ModularSkinModule selected;
                selected.category=category_name;selected.item_name=std::move(module);
                selected.controller=selected.item_name+"-mesh-skin";
                selected.category_id=category_id;selected.module_id=module_id;
                selected.controller_index=controller_index(r,view,selected.controller);
                candidate.modules.push_back(std::move(selected));
            }
            if(!category_names.insert(category_name).second)
                throw std::runtime_error("Duplicate Prince modular category");
        }
        if(candidate.modules.empty())throw std::runtime_error("Prince modular catalog is empty");
        out=std::move(candidate);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}

const ModularSkinModule* find_modular_skin(const ModularSkinCatalog& catalog,
                                           const std::string& category,
                                           const std::string& item_name) noexcept {
    const ModularSkinModule* result=nullptr;
    for(const auto& module:catalog.modules)
        if(module.category==category&&module.item_name==item_name){
            if(result)return nullptr;
            result=&module;
        }
    return result;
}

bool resolve_modular_skin(const ModularSkinCatalog& catalog,
                          const std::string& category,
                          const std::string& equipped_item,bool equipped,
                          const ModularSkinModule*& result,bool& placeholder,
                          std::string& error) {
    result=nullptr;placeholder=false;error.clear();
    const auto& exact=equipped?equipped_item:category+"__naked";
    result=find_modular_skin(catalog,category,exact);
    if(result)return true;
    if(equipped){
        result=find_modular_skin(catalog,category,category+"__placeholder");
        if(result){placeholder=true;return true;}
    }
    error=equipped?"Unknown modular item and no exact category placeholder":
                   "Exact naked modular controller is absent";
    return false;
}

} // namespace dh2::skinning
