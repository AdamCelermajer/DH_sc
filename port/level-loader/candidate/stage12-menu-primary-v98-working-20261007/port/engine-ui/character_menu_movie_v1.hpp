#pragma once
#include "swf_movie.hpp"
#include <memory>
namespace dh2::ui {
struct CharacterMenuScreenDefinitionV1 {
 const char* name;std::int32_t id,depth,frames;
};
struct CharacterMenuScreenV1 {
 CharacterMenuScreenDefinitionV1 definition{};
 SwfAsValue receiver;
};
// Owns the actual shared/character movie and its authored MovieClip receivers.
// Gameplay/native, GPU, font, input and concrete MenuBase lifecycle are supplied
// by the same game graph. This is not a replacement inventory or menu stack.
class CharacterMenuMovieV1 final {
 struct Provider;
 std::shared_ptr<SwfMovie> movie_;
 bool deleted_{};
 std::shared_ptr<void> source_update_owner_;
 std::vector<CharacterMenuScreenV1> screens_;
 bool loaded_{};
 bool load_impl_v98(const SwfServices&,const char* source_uri,std::string&);
public:
 CharacterMenuMovieV1();~CharacterMenuMovieV1();
 CharacterMenuMovieV1(const CharacterMenuMovieV1&)=delete;
 CharacterMenuMovieV1& operator=(const CharacterMenuMovieV1&)=delete;
 // Restore source player globals before ANY shared/movie script, then run the
 // caller's real startup observers. Caller owner must not own this movie.
 bool load(const SwfServices&,std::string&);
 bool load_source_resource_v98(const char* actual_uri,const SwfServices&,std::string&);
 bool advance(float,std::string&);
 bool source_virtual10_v93(std::int32_t,bool,std::string&);
 bool deleting_movie_v93(std::string&);
 bool clear_movie_slot_v93(std::string&);
 bool claim_source_update_v93(std::shared_ptr<void>,std::string&);
 bool release_source_update_v93(const std::shared_ptr<void>&,std::string&);
 std::shared_ptr<void> source_movie_lease_v94()const noexcept{return movie_;}

 bool display(std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::string&);
 bool action_script(void*,bool(*)(void*,SwfAsGraph&,std::string&),std::string&);
 // Same genuine sprite's environment/receiver; callable=false is preserved.
 bool invoke(const char* screen,const char* method,const std::vector<SwfAsValue>&,
             SwfAsValue&,bool& callable,std::string&);
 const std::vector<CharacterMenuScreenV1>& screens()const noexcept{return screens_;}
 SwfMovie* movie()const noexcept{return loaded_?movie_.get():nullptr;}
 SwfMovie* source_movie_v91()const noexcept{return movie_.get();} // Retained C1/failed Load prefix.
};
}
