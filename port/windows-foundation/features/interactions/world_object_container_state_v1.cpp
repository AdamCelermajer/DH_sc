#include "world_object_container_state_v1.hpp"
#include <cstring>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error,const char* message) { error=message; return false; }
}

bool encode_source_container_objs_v1(const SourceContainerObjsFieldsV1& fields,
                                     std::vector<std::uint8_t>& output,
                                     std::string& error) {
    std::vector<std::uint8_t> bytes(source_container_objs_payload_bytes_v1);
    bytes[0]=fields.visible80;
    bytes[1]=fields.enabled8a;
    std::uint32_t archetype{};
    std::memcpy(&archetype,&fields.archetype270,sizeof archetype);
    bytes[2]=static_cast<std::uint8_t>(archetype);
    bytes[3]=static_cast<std::uint8_t>(archetype>>8);
    bytes[4]=static_cast<std::uint8_t>(archetype>>16);
    bytes[5]=static_cast<std::uint8_t>(archetype>>24);
    bytes[6]=fields.state394;
    output.swap(bytes);
    error.clear();
    return true;
}

bool decode_source_container_objs_v1(const std::vector<std::uint8_t>& bytes,
                                     SourceContainerObjsFieldsV1& output,
                                     std::string& error) {
    if(bytes.size()!=source_container_objs_payload_bytes_v1)
        return fail(error,"Source container OBJS v1 component must be exactly seven bytes");
    SourceContainerObjsFieldsV1 next;
    next.visible80=bytes[0];
    next.enabled8a=bytes[1];
    const std::uint32_t archetype=std::uint32_t(bytes[2])|
        (std::uint32_t(bytes[3])<<8)|(std::uint32_t(bytes[4])<<16)|
        (std::uint32_t(bytes[5])<<24);
    std::memcpy(&next.archetype270,&archetype,sizeof archetype);
    next.state394=bytes[6];
    output=next;
    error.clear();
    return true;
}

bool bind_source_container_objs_v1(WorldObject& object,
                                   const SourceContainerObjsFieldsV1& fields,
                                   std::string& error) {
    std::vector<std::uint8_t> encoded;
    if(!encode_source_container_objs_v1(fields,encoded,error))return false;
    auto candidate=object;
    candidate.state_components.insert_or_assign(
        source_container_objs_component_v1,std::move(encoded));
    if(!validate_world_object(candidate,error))return false;
    object.state_components.swap(candidate.state_components);
    error.clear();
    return true;
}

bool read_source_container_objs_v1(const WorldObject& object,
                                   SourceContainerObjsFieldsV1& output,
                                   std::string& error) {
    const auto found=object.state_components.find(source_container_objs_component_v1);
    if(found==object.state_components.end())
        return fail(error,"Neutral container lacks its source OBJS v1 component");
    return decode_source_container_objs_v1(found->second,output,error);
}

bool write_source_container_state394_v1(WorldObject& object,std::uint8_t state,
                                        std::string& error) {
    SourceContainerObjsFieldsV1 fields;
    if(!read_source_container_objs_v1(object,fields,error))return false;
    fields.state394=state;
    return bind_source_container_objs_v1(object,fields,error);
}

bool source_object_id_for_save_key_v1(
    const std::vector<ActorDefinition>& definitions,
    const SourceObjectSaveKeyV1& key,void* context,
    SourceObjectRoomResolverV1 resolve_room,ActorId& output,
    std::string& error) {
    output=invalid_actor_id;
    if(key.gametype.empty()||key.map_name.empty()||!resolve_room)
        return fail(error,"Source OBJS key requires gametype, map name, and current room resolver");
    unsigned matches=0;
    for(const auto& definition:definitions){
        if(definition.gametype!=key.gametype||definition.name!=key.map_name)continue;
        std::int32_t room{};
        if(!resolve_room(context,definition,room,error))return false;
        if(room!=key.room)continue;
        if(definition.stableId==invalid_actor_id)
            return fail(error,"Source OBJS definition has invalid stable ObjectId");
        output=definition.stableId;
        ++matches;
    }
    if(matches!=1){output=invalid_actor_id;return fail(error,matches?
        "Source OBJS key is ambiguous across authored occurrences":
        "Source OBJS key has no authored definition in current room");}
    error.clear();
    return true;
}

bool source_object_save_key_v1(const ActorDefinition& definition,
                               std::int32_t room,SourceObjectSaveKeyV1& output,
                               std::string& error) {
    if(definition.stableId==invalid_actor_id||definition.gametype.empty()||
       definition.name.empty())
        return fail(error,"Source OBJS key requires a stable authored definition");
    output={definition.gametype,definition.name,room};
    error.clear();
    return true;
}

SourceContainerRestoreVisualV1 source_container_restore_visual_v1(
    std::uint8_t state) noexcept {
    if(state==2)return SourceContainerRestoreVisualV1::idle;
    if(state==4)return SourceContainerRestoreVisualV1::idleactive;
    return SourceContainerRestoreVisualV1::none;
}

} // namespace dh::foundation::interactions
