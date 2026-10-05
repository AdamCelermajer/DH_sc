#include "procedural_connections_v1.hpp"
#include <map>
#include <stdexcept>
namespace dh2::loader {
bool connect_procedural_blocks_v1(const std::vector<std::string>& names,
                                 const std::vector<ProceduralBlockV1>& blocks,
                                 ProceduralConnectionGraphV1& output,std::string& error) {
    error.clear();
    try {
        if(names.size()!=blocks.size()||names.size()>4096)
            throw std::runtime_error("Procedural block-map input outside checked domain");
        ProceduralConnectionGraphV1 next;std::map<std::string,std::uint32_t> map;
        for(std::size_t i=0;i<names.size();++i) {
            if(names[i].find('\0')!=std::string::npos||names[i].size()>4096||blocks[i].exits.size()>8)
                throw std::runtime_error("Procedural block name/exits outside checked domain");
            next.inserted.push_back(map.emplace(names[i],static_cast<std::uint32_t>(i)).second);
        }
        for(const auto& item:map)next.selected_sources.push_back(item.second);
        next.connections.resize(map.size());
        for(std::size_t i=0;i<next.selected_sources.size();++i) {
            const auto& b=blocks[next.selected_sources[i]];next.connections[i].resize(b.exits.size());
            for(const auto& e:b.exits)if(e.direction>4||e.link_types.size()>4097)
                throw std::runtime_error("Procedural exit outside checked connection domain");
        }
        static constexpr unsigned opposites[]={2,3,0,1,4};
        for(std::size_t own=0;own<next.selected_sources.size();++own) {
            const auto& own_block=blocks[next.selected_sources[own]];
            // Original outer loop is target map order, then own exit, target
            // exit, own type and target type. Do not reorder or deduplicate.
            for(std::size_t target=0;target<next.selected_sources.size();++target) {
                const auto& target_block=blocks[next.selected_sources[target]];
                for(std::size_t a=0;a<own_block.exits.size();++a) {
                    const auto& source_exit=own_block.exits[a];
                    for(std::size_t b=0;b<target_block.exits.size();++b) {
                        const auto& target_exit=target_block.exits[b];
                        for(const auto& source_type:source_exit.link_types) {
                            if(source_type.empty())continue;
                            for(const auto& target_type:target_exit.link_types) {
                                if(source_type!=target_type||source_exit.direction!=opposites[target_exit.direction])continue;
                                auto& connections=next.connections[own][a];
                                if(connections.size()>=64)
                                    throw std::runtime_error("Original 64-connection capacity exceeded: "+names[next.selected_sources[own]]+" exit "+std::to_string(a));
                                connections.push_back({static_cast<std::uint32_t>(target),static_cast<std::uint32_t>(b)});
                            }
                        }
                    }
                }
            }
        }
        output=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
struct ProceduralConnectionsV1::Snapshot {
    ProceduralBlocksV1::Borrow blocks;
    ProceduralConnectionGraphV1 graph;
};
const ProceduralBlocksV1::Borrow& ProceduralConnectionsV1::Borrow::blocks()const {
    if(!snapshot_)throw std::logic_error("Procedural connection snapshot unavailable");
    return snapshot_->blocks;
}
const ProceduralConnectionGraphV1& ProceduralConnectionsV1::Borrow::graph()const {
    if(!snapshot_)throw std::logic_error("Procedural connection snapshot unavailable");
    return snapshot_->graph;
}
bool ProceduralConnectionsV1::prepare(ProceduralBlocksV1::Borrow blocks,std::string& error) {
    error.clear();
    try {
        if(!blocks)throw std::runtime_error("Procedural block snapshot unavailable");
        auto next=std::make_shared<Snapshot>();next->blocks=std::move(blocks);
        std::vector<std::string> names;
        for(const auto& b:next->blocks.sources().blocks())names.push_back(b.name);
        if(!connect_procedural_blocks_v1(names,next->blocks.blocks(),next->graph,error))return false;
        snapshot_=std::move(next);return true;
    }catch(const std::exception& e){error=e.what();return false;}
}
}
