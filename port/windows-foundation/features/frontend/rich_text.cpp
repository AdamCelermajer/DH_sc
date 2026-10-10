#include "rich_text.hpp"
#include "creation/dynamic_text_bindings.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>
namespace dh::foundation::frontend {
namespace {
struct Span{std::string text;std::array<float,4> color;};
std::vector<Span> parse(const std::string&html,std::array<float,4> color){
 std::vector<Span> out;std::vector<std::array<float,4>> stack;std::string text;
 auto flush=[&](){if(!text.empty()){out.push_back({std::move(text),color});text.clear();}};
 for(std::size_t i=0;i<html.size();){if(html[i]=='<'){auto end=html.find('>',i);if(end==html.npos)break;flush();auto tag=html.substr(i+1,end-i-1);auto upper=tag;for(auto&c:upper)c=char(std::toupper(static_cast<unsigned char>(c)));
   if(upper.rfind("FONT",0)==0){stack.push_back(color);auto hex=upper.find('#');if(hex!=upper.npos&&hex+7<=upper.size()){auto rgb=std::stoul(upper.substr(hex+1,6),nullptr,16);color={float((rgb>>16)&255)/255,float((rgb>>8)&255)/255,float(rgb&255)/255,color[3]};}}
   else if(upper=="/FONT"&&!stack.empty()){color=stack.back();stack.pop_back();}
   else if(upper.rfind("BR",0)==0||upper=="/P")out.push_back({"\n",color});i=end+1;
  }else if(html[i]=='&'){auto end=html.find(';',i);if(end!=html.npos&&end-i<12){auto entity=html.substr(i,end-i+1);text+=entity=="&amp;"?"&":entity=="&lt;"?"<":entity=="&gt;"?">":entity=="&quot;"?"\"":entity=="&nbsp;"?" ":entity;i=end+1;}else text+=html[i++];}
  else text+=html[i++];
 }flush();return out;
}
}
bool FrontendText::load(const std::filesystem::path&cache,std::string&error){return font_.load(cache/"data/Fontin SmallCaps.ttf",error);}
void FrontendText::clear(Renderer&r){for(auto texture:textures_)r.destroyTexture(texture);textures_.clear();sprites_.clear();}
bool FrontendText::rebuild(const art::ScreenArt&art,int width,int height,Renderer&r,std::string&error){
 clear(r);if(width<=0||height<=0){error="Invalid frontend text viewport";return false;}
 for(const auto&field:art.text_fields){if(field.initial_text.empty()||!art::source_font_file(field.font_id))continue;
  auto html=creation::source_localized_html(field.initial_text);if(!html.ok()){error=html.error;return false;}
  std::array<float,4> color{};for(unsigned j=0;j<4;++j)color[j]=field.rgba[j]/255.f;
  auto spans=parse(html.html,color);struct Word{HudGlyphRun run;std::array<float,4> color;bool newline=false;};std::vector<Word> words;
  for(const auto&span:spans){std::size_t start=0;for(std::size_t i=0;i<=span.text.size();++i)if(i==span.text.size()||span.text[i]==' '||span.text[i]=='\n'){
    if(i>start||(i<span.text.size()&&span.text[i]==' ')){Word word;word.color=span.color;auto value=span.text.substr(start,i-start);if(i<span.text.size()&&span.text[i]==' ')value+=' ';if(!font_.raster(value,int(std::lround(field.source_height)),2,word.run,error))return false;words.push_back(std::move(word));}
    if(i<span.text.size()&&span.text[i]=='\n')words.push_back({{},span.color,true});start=i+1;}}
  const float available=field.local_bounds[1]-field.local_bounds[0]-field.margins[0]-field.margins[1]-4;std::vector<std::vector<const Word*>> lines(1);std::vector<float> advances(1);
  for(const auto&word:words){if(word.newline){if(!lines.back().empty()){lines.emplace_back();advances.push_back(0);}continue;}if(advances.back()+word.run.advance>available&&!lines.back().empty()){lines.emplace_back();advances.push_back(0);}lines.back().push_back(&word);advances.back()+=word.run.advance;}
  for(std::size_t line=0;line<lines.size();++line){HudTargetTextField layout;layout.source_height=field.source_height;layout.align=field.align;layout.local_bounds=field.local_bounds;layout.matrix=field.matrix;layout.margins=field.margins;layout.font_metrics=field.font_metrics;
   std::array<float,2> baseline;if(!layout_original_target_text(layout,advances[line],baseline,error))return false;
   const float lineY=float(line)*(field.source_height+field.leading);baseline[0]+=field.matrix[2]*lineY;baseline[1]+=field.matrix[3]*lineY;
   float pen=0;for(const auto*word:lines[line]){for(const auto&glyph:word->run.glyphs){if(!glyph.image.width||!glyph.image.height)continue;
     const float x=baseline[0]+field.matrix[0]*(pen+glyph.x)+field.matrix[2]*glyph.y,y=baseline[1]+field.matrix[1]*(pen+glyph.x)+field.matrix[3]*glyph.y;
     const float w=glyph.width*field.matrix[0],h=glyph.height*field.matrix[3];if(w<=0||h<=0)continue;
     // Original edit text extends beyond its authored one-line rectangle when
     // formatting multiline content. That rectangle is a layout width, not a
     // scissor mask. Movie masks remain the source composition's responsibility.
     float left=x,right=x+w,top=y,bottom=y+h;
     auto texture=r.createTexture(glyph.image.width,glyph.image.height,glyph.image.rgba.data());textures_.push_back(texture);OverlaySprite sprite;
     sprite.x=left*width/480.f;sprite.y=top*height/320.f;sprite.width=(right-left)*width/480.f;sprite.height=(bottom-top)*height/320.f;sprite.u0=(left-x)/w*glyph.u1;sprite.u1=(right-x)/w*glyph.u1;sprite.v0=(top-y)/h*glyph.v1;sprite.v1=(bottom-y)/h*glyph.v1;sprite.texture=texture;sprite.color=word->color;sprites_.push_back(sprite);
    }pen+=word->run.advance;}
  }
 }error.clear();return true;
}
void FrontendText::draw(OverlayRenderer&r)const{for(const auto&sprite:sprites_)r.drawSprite(sprite);}
}
