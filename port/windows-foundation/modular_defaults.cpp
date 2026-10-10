#include "modular_defaults.hpp"
#include "../engine-resources/resources.hpp"
#include <algorithm>
#include <cstring>
#include <functional>
#include <set>
#include <stdexcept>

namespace dh::foundation {
namespace {
struct Reader {
    const dh2::resources::BresView& view;
    const std::uint8_t* range(std::uint64_t at,std::uint64_t count) const {
        if(at>view.size||count>view.size-at)throw std::runtime_error("Modular default descriptor outside resource");
        return view.bytes+at;
    }
    std::uint32_t word(std::uint64_t at) const {
        auto p=range(at,4);return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24;
    }
    void array(std::uint32_t at,std::uint32_t count,std::uint32_t stride) const {
        if(count>100000)throw std::runtime_error("Modular default array outside resource budget");
        range(at,std::uint64_t(count)*stride);
    }
    std::string text(std::uint32_t at) const {
        if(!at)throw std::runtime_error("Missing modular default string");
        auto p=range(at,1);
        auto end=static_cast<const std::uint8_t*>(std::memchr(p,0,std::min<std::size_t>(view.size-at,4096)));
        if(!end)throw std::runtime_error("Unterminated modular default string");
        return {reinterpret_cast<const char*>(p),std::size_t(end-p)};
    }
    std::string field(std::uint64_t at) const {return text(word(at));}
    std::string controller(const std::string& uri) const {
        if(uri.empty()||uri.front()!='#')throw std::runtime_error("External modular controller requires content resolver");
        const auto name=uri.substr(1);
        const auto library=dh2::resources::Library::controller;
        bool found=false;
        for(unsigned i=0;i<dh2_bres_library_count(&view,library);++i) {
            auto p=dh2_bres_library_item(&view,library,i);
            if(field(std::uint64_t(p-view.bytes)+4)==name) {
                if(found)throw std::runtime_error("Ambiguous modular controller ID");
                found=true;
            }
        }
        if(!found)throw std::runtime_error("Modular controller not in resource: "+name);
        return name;
    }
};
} // namespace

bool decode_modular_defaults(const std::vector<std::uint8_t>& bytes,
                             std::vector<ModularDefaultCategory>& output,std::string& error) {
    try {
        dh2::resources::BresView view{};
        if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)
            throw std::runtime_error("Invalid modular-default BRES resource");
        Reader r{view};std::vector<ModularDefaultCategory> result;
        std::set<std::uint32_t> active;unsigned visited=0;
        std::function<void(std::uint32_t)> visit=[&](std::uint32_t node) {
            r.range(node,80);
            if(active.size()>=64||!active.insert(node).second||++visited>100000)
                throw std::runtime_error("Modular default node cycle or budget exceeded");
            const auto count=r.word(node+64),base=r.word(node+68);r.array(base,count,8);
            for(unsigned i=0;i<count;++i) {
                const auto instance=base+8*i;
                if(r.word(instance)!=13)continue;
                const auto descriptor=r.word(instance+4);r.range(descriptor,16);
                if(r.word(descriptor+8)||r.word(descriptor+12))
                    throw std::runtime_error("Extra modular descriptor array requires original factory support");
                const auto categoryCount=r.word(descriptor),categories=r.word(descriptor+4);
                const auto firstCategory=result.size();
                if(categoryCount>4096)throw std::runtime_error("Modular category count exceeds budget");
                r.array(categories,categoryCount,16);
                for(unsigned c=0;c<categoryCount;++c) {
                    const auto category=categories+16*c;
                    ModularDefaultCategory record;
                    record.node_id=r.field(node);record.category=r.field(category);record.default_uri=r.field(category+4);
                    const auto moduleCount=r.word(category+8),modules=r.word(category+12);r.array(modules,moduleCount,8);
                    for(unsigned m=0;m<moduleCount;++m) {
                        const auto module=modules+8*m;
                        if(r.word(module)!=2)throw std::runtime_error("Modular default module is not a controller instance");
                        const auto controller=r.word(module+4);r.range(controller,24);
                        if(r.word(controller))throw std::runtime_error("External modular descriptor requires content resolver");
                        const auto uri=r.field(controller+4);
                        const auto id=r.controller(uri);record.available_controller_ids.push_back(id);
                    }
                    result.push_back(std::move(record));
                }
                // getModuleId0x6474b8 scans categories in serialized order and
                // returns a category-local module index. Constructor0x649120
                // then applies that index to the category being initialized.
                for(std::size_t c=firstCategory;c<result.size();++c) {
                    std::int32_t selected=-1;
                    for(std::size_t search=firstCategory;search<result.size()&&selected<0;++search) {
                        const auto& modules=result[search].available_controller_ids;
                        for(std::size_t m=0;m<modules.size();++m) {
                            if("#"+modules[m]==result[c].default_uri) {selected=static_cast<std::int32_t>(m);break;}
                        }
                    }
                    if(selected>=0) {
                        if(static_cast<std::size_t>(selected)>=result[c].available_controller_ids.size())
                            throw std::runtime_error("Original modular default index outside its category");
                        result[c].controller_id=result[c].available_controller_ids[selected];
                    }
                }
            }
            const auto children=r.word(node+56),childBase=r.word(node+60);r.array(childBase,children,80);
            for(unsigned i=0;i<children;++i)visit(childBase+80*i);
            active.erase(node);
        };
        const auto sceneCount=r.word(view.root_offset+152),scenes=r.word(view.root_offset+156);r.array(scenes,sceneCount,16);
        for(unsigned i=0;i<sceneCount;++i) {
            const auto scene=scenes+16*i;const auto nodes=r.word(scene+8),base=r.word(scene+12);r.array(base,nodes,80);
            for(unsigned j=0;j<nodes;++j)visit(base+80*j);
        }
        output=std::move(result);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
} // namespace dh::foundation
