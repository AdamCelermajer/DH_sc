#include "level_config_publication_v2.hpp"
namespace dh2::world {
bool level_config_publication_v2(const LevelConfigPublicationBorrowV2& b,std::uintptr_t identity,std::string& e){
 if(!b.level_owner||!b.config38){e="Required SAME Level config38 owner";return false;}
 *b.config38=identity; // source prefix before NULL assertion/lookup
 if(!identity){e="Original SetLevelConfig NULL assertion/unsafe dereference branch required";return false;}
 if(!b.arrays_owner||!b.sound_names||!b.config){e="Required actual Arrays::Sounds/current canonical LevelConfig";return false;}
 auto apply=[&](std::uint32_t offset,std::int32_t* dest,bool optional){
  const auto* receiver=b.config(*b.config38);const auto* name=receiver?receiver->string(offset):nullptr;
  if(!name){e="Required SAME currently published LevelConfig CString";return false;}
  if(optional&&name->empty())return true;
  if(!dest){e="Required SAME Level sound ID field";return false;}
  std::int32_t id=-1;for(std::size_t i=0;i<b.sound_names->size();++i)if((*b.sound_names)[i]==*name){id=static_cast<std::int32_t>(i);break;}
  *dest=id;return true;
 };
 return apply(0x168,b.music11c,false)&&apply(0x1b0,b.ambient124,true)&&apply(0x180,b.safezone120,true);
}
}
