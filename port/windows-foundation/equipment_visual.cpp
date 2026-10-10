#include "equipment_visual.hpp"
#include "content_paths.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>

namespace dh::foundation {
bool EquipmentAttachmentSet::load(const AssetCatalog& assets,
    const std::vector<EquipmentVisualDefinition>& definitions,std::string& error) {
    try {
        if(definitions.size()>256)throw std::runtime_error("Equipment attachment count exceeds limit");
        std::set<std::string> identities;
        std::vector<EquipmentAttachmentVisual> candidate;
        candidate.reserve(definitions.size());
        for(const auto& definition:definitions) {
            if(definition.id.empty() || definition.id.size()>4096 ||
               definition.model_uri.empty() || definition.anchor_name.empty())
                throw std::runtime_error("Equipment attachment identity/model/anchor is empty or invalid");
            if(!identities.insert(definition.id).second)throw std::runtime_error("Duplicate equipment attachment identity: "+definition.id);
            for(float value:definition.local_offset)if(!std::isfinite(value))throw std::runtime_error("Nonfinite equipment local offset");
            auto config=definition.model_config;
            if(!config.clips.empty() || std::any_of(config.animation_paths.begin(),config.animation_paths.end(),[](const auto& path){return !path.empty();}))
                throw std::runtime_error("Equipment attachment requires a static/rest-pose model configuration");
            config.model_path=resolve_content_path(assets,definition.model_uri).lexically_relative(assets.root()).generic_string();
            EquipmentAttachmentVisual attachment;attachment.definition=definition;
            if(!attachment.visual.load(assets,config,error))throw std::runtime_error("Equipment "+definition.id+": "+error);
            candidate.push_back(std::move(attachment));
        }
        attachments_=std::move(candidate);error.clear();return true;
    } catch(const std::exception& e) {error=e.what();return false;}
}
bool EquipmentAttachmentSet::update(const CharacterVisual& body,std::string& error) {
    std::vector<Mat4> candidate;
    candidate.reserve(attachments_.size());
    for(const auto& attachment:attachments_) {
        Mat4 socket;
        if(!body.socket_world(attachment.definition.anchor_name,attachment.definition.local_offset,socket,error))return false;
        candidate.push_back(socket);
    }
    for(std::size_t i=0;i<candidate.size();++i)attachments_[i].socket_world=candidate[i];
    error.clear();return true;
}
}
