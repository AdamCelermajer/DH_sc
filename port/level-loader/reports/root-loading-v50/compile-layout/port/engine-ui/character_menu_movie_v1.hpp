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
 std::unique_ptr<SwfMovie> movie_;
 std::vector<CharacterMenuScreenV1> screens_;
 bool loaded_{};
public:
 CharacterMenuMovieV1();~CharacterMenuMovieV1();
 CharacterMenuMovieV1(const CharacterMenuMovieV1&)=delete;
 CharacterMenuMovieV1& operator=(const CharacterMenuMovieV1&)=delete;
 // Restore source player globals before ANY shared/movie script, then run the
 // caller's real startup observers. Caller owner must not own this movie.
 bool load(const SwfServices&,std::string&);
 bool advance(float,std::string&);
 bool display(std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::string&);
 bool action_script(void*,bool(*)(void*,SwfAsGraph&,std::string&),std::string&);
 // Same genuine sprite's environment/receiver; callable=false is preserved.
 bool invoke(const char* screen,const char* method,const std::vector<SwfAsValue>&,
             SwfAsValue&,bool& callable,std::string&);
 const std::vector<CharacterMenuScreenV1>& screens()const noexcept{return screens_;}
 SwfMovie* movie()const noexcept{return loaded_?movie_.get():nullptr;}
};
}
