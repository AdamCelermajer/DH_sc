#include "character_animation_instance.hpp"
#include "../asset-payloads/sha256.hpp"
#include <stdexcept>
namespace dh2::character {
bool CharacterAnimationResources::load(data::Bytes bytes,const scene::Scene& factory,
 const Reader& reader,std::shared_ptr<const CharacterAnimationResources>& output,std::string& error){
 error.clear();try{
  if(!reader||factory.graph.empty())throw std::runtime_error("Character animation resource producer missing");
  std::shared_ptr<CharacterAnimationResources> candidate(new CharacterAnimationResources);
  if(!data::load_animation_bank(bytes,candidate->metadata_,error))return false;
  if(candidate->metadata_.registration_requests.size()>1024)
   throw std::runtime_error("Character animation occurrence capacity exceeded");
  candidate->factory_=factory;
  for(const auto& resource:candidate->metadata_.resources){
   std::vector<std::uint8_t> payload;
   if(!reader(resource,payload,error)){
    if(error.empty())error="Character animation resource unavailable";
    return false;
   }
   if(payload.size()!=resource.bytes)throw std::runtime_error("Character animation resource size differs");
   assets::Sha256Digest digest{};
   if(!assets::sha256(payload.data(),payload.size(),digest)||digest!=resource.sha256)
    throw std::runtime_error("Character animation resource digest differs");
   animation::Player player;
   if(!player.load(payload.data(),payload.size(),candidate->factory_,error,animation::MissingTargets::ignore))return false;
   candidate->clips_.emplace(resource.clip_id,std::move(player));
  }
  output=std::move(candidate);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
CharacterAnimationInstance::CharacterAnimationInstance(std::shared_ptr<const CharacterAnimationResources> resources):
 resources_(std::move(resources)),scene_(resources_->factory()){}
std::unique_ptr<CharacterAnimationInstance> CharacterAnimationInstance::create(
 std::shared_ptr<const CharacterAnimationResources> resources,std::string& error){
 error.clear();try{
  if(!resources)throw std::runtime_error("Character animation resources missing");
  std::unique_ptr<CharacterAnimationInstance> candidate(new CharacterAnimationInstance(std::move(resources)));
  if(!candidate->visual_.bind(candidate->scene_,error))return nullptr;
  const auto& metadata=candidate->resources_->metadata();
  const auto& clips=candidate->resources_->clips();
  animation::RegistrationSet registration;
  for(const int id:metadata.registration_requests){
   const auto found=clips.find(id);
   if(found==clips.end()||!registration.append(id,data::animation_resource_identity(metadata,id),&found->second,error))return nullptr;
  }
  const auto& default_clip=clips.at(metadata.template_clip_id);
  if(!registration.set_default(data::animation_resource_identity(metadata,metadata.template_clip_id),&default_clip,error))return nullptr;
  registration.refresh_indices();
  if(!candidate->playback_.compile_dynamic(clips,registration,candidate->scene_,candidate->visual_,error))return nullptr;
  return candidate;
 }catch(const std::exception& e){error=e.what();return nullptr;}
}
bool CharacterAnimationInstance::start(const data::AnimationTables& tables,int sequence,
 data::AnimationRandom& random,float speed,std::string& error){
 return playback_.start(tables,sequence,random,resources_->clips(),visual_,scene_,speed,error);
}
bool CharacterAnimationInstance::scene_phase(std::uint32_t timestamp,std::string& error){
 return playback_.scene_phase(timestamp,resources_->clips(),visual_,scene_,error);
}
bool CharacterAnimationInstance::animator_phase(const data::AnimationTables& tables,
 data::AnimationRandom& random,float speed,std::string& error){
 return playback_.animator_phase(tables,random,resources_->clips(),visual_,scene_,speed,playback_.completion.extra_ms,error);
}
}
