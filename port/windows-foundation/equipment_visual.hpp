#pragma once
#include "original_character.hpp"
#include <string>
#include <vector>

namespace dh::foundation {
class AssetCatalog;
struct EquipmentVisualDefinition {
    std::string id;
    std::string model_uri;
    std::string anchor_name;
    Mat4 local_offset{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    CharacterVisualConfig model_config;
};
struct EquipmentAttachmentVisual {
    EquipmentVisualDefinition definition;
    CharacterVisual visual;
    // In the body's model space. Host submits actorWorld * socket_world.
    // The weapon's authored instance transform is already baked in its vertices.
    Mat4 socket_world{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
};

// Generic source-data attachments. No class, inventory item or bone presets.
class EquipmentAttachmentSet {
public:
    bool load(const AssetCatalog&, const std::vector<EquipmentVisualDefinition>&,
              std::string& error);
    // Update after body pose sampling; failure preserves every previous socket.
    bool update(const CharacterVisual& body, std::string& error);
    const std::vector<EquipmentAttachmentVisual>& attachments() const {return attachments_;}
    std::vector<EquipmentAttachmentVisual>& mutable_attachments() {return attachments_;}
    void clear() {attachments_.clear();}
private:
    std::vector<EquipmentAttachmentVisual> attachments_;
};
}
