#include "world.hpp"

#include <cmath>
#include <limits>
#include <utility>

namespace dh::foundation {
namespace {

bool valid_transform(const Transform& transform) noexcept {
    for (const auto* values : {&transform.position, &transform.rotation, &transform.scale}) {
        for (float value : *values) {
            if (!std::isfinite(value)) return false;
        }
    }
    return true;
}

ObjectId successor(ObjectId id) noexcept {
    return id == std::numeric_limits<ObjectId>::max() ? invalid_object_id : id + 1;
}

} // namespace

bool validate_world_object(const WorldObject& object,std::string& error) {
    if(!object.id){error="World object ID must be nonzero.";return false;}
    if(!valid_transform(object.transform)){error="World object transform contains a non-finite value.";return false;}
    if(object.state_components.size()>world_object_component_limit){error="World object component count exceeds limit.";return false;}
    std::size_t bytes=0;
    for(const auto& component:object.state_components){
        if(component.first.empty()||component.first.size()>256){error="World object component key is invalid.";return false;}
        for(unsigned char c:component.first)if(c<32||c==127){error="World object component key is invalid.";return false;}
        if(component.second.size()>world_object_state_limit-bytes){error="World object persistent state exceeds limit.";return false;}
        bytes+=component.second.size();
    }
    error.clear();return true;
}

bool World::bind(WorldObject object,std::string* error) {
    std::string validation;
    if(!validate_world_object(object,validation)){if(error)*error=std::move(validation);return false;}
    const auto id=object.id;
    if(!objects_.emplace(id,std::move(object)).second){if(error)*error="World object ID already bound.";return false;}
    if(next_id_&&id>=next_id_)next_id_=successor(id);
    if(error)error->clear();return true;
}

ObjectId World::spawn(std::string name, VisualReference visual, Transform transform) {
    if (next_id_ == invalid_object_id || !valid_transform(transform)) {
        return invalid_object_id;
    }
    const ObjectId id = next_id_;
    objects_.emplace(id, WorldObject{id, std::move(name), transform, std::move(visual),{}});
    next_id_ = successor(id);
    return id;
}

bool World::remove(ObjectId id) {
    return objects_.erase(id) != 0;
}

const WorldObject* World::find(ObjectId id) const noexcept {
    const auto found = objects_.find(id);
    return found == objects_.end() ? nullptr : &found->second;
}

bool World::set_transform(ObjectId id, const Transform& transform) {
    if (!valid_transform(transform)) return false;
    const auto found = objects_.find(id);
    if (found == objects_.end()) return false;
    found->second.transform = transform;
    return true;
}

bool World::set_visual(ObjectId id, VisualReference visual) {
    const auto found = objects_.find(id);
    if (found == objects_.end()) return false;
    found->second.visual = std::move(visual);
    return true;
}

bool World::replace(std::vector<WorldObject> objects, std::string* error) {
    std::map<ObjectId, WorldObject> staged;
    ObjectId maximum_id = 0;
    for (auto& object : objects) {
        std::string validation;
        if(!validate_world_object(object,validation)){if(error)*error=std::move(validation);return false;}
        const ObjectId id = object.id;
        if (!staged.emplace(id, std::move(object)).second) {
            if (error) *error = "World object IDs must be unique.";
            return false;
        }
        if (id > maximum_id) maximum_id = id;
    }
    objects_.swap(staged);
    next_id_ = successor(maximum_id);
    if (error) error->clear();
    return true;
}

void World::clear() noexcept {
    objects_.clear();
    next_id_ = 1;
}

} // namespace dh::foundation
