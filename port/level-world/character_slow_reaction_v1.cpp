#include "character_slow_reaction_v1.hpp"
#include <algorithm>
namespace dh2::character {
int character_slow_reaction_v1(const SlowReactionBorrowV1& b,std::uint32_t duration,
 BuffResult24& output,std::string& error){
 output={};if(!b.properties||dh2_property_validate(b.properties)||!b.ai){error="Required SAME slow Character properties/AI table";return -1;}
 const auto* ai=data::ai_props(*b.ai,b.properties->resolved[1]);if(!ai){error="Required original slow IsBoss AI row/fallback8";return -2;}
 if((ai->flags&4)||!duration)return 1;
 if(!b.classes){error="Required original ClassDict for Debuff_Slow";return -2;}
 const auto found=std::find(b.classes->names.begin(),b.classes->names.end(),"Debuff_Slow");
 if(found==b.classes->names.end())return 1;
 const auto id=static_cast<std::int32_t>(found-b.classes->names.begin());
 if(!b.effects){error="Required original EffectsDict for AUTO_DEBUFF_SLOW";return -2;}
 const auto& fx_names=b.effects.set_names();const auto fx=std::find(fx_names.begin(),fx_names.end(),"AUTO_DEBUFF_SLOW");
 const auto fx_id=fx==fx_names.end()?-1:static_cast<std::int32_t>(fx-fx_names.begin());
 if(!b.buffs){error="Required SAME slow BuffOwner/TimerStore/FX services";return -2;}
 const auto status=dh2_character_buff_add(&output,b.buffs,id,duration,1,0,fx_id,"debuff_slow");
 if(status!=1){error="Original slow PROPS_AddBuff failed after reached prefix";return -2;}
 if(!output.instance)return 1;
 std::int32_t* sheet=nullptr;
 if(skills::skill_buff_sheet_v3(b.buffs,output.instance,&sheet)||!sheet){error="Required same retained slow BuffInst sheet";return -2;}
 std::vector<data::ClassRow> rows;rows.reserve(b.classes->rows.size());
 for(const auto& row:b.classes->rows)rows.push_back({row.data(),static_cast<std::uint32_t>(row.size())});
 if(dh2_class_apply(rows.data(),static_cast<std::uint32_t>(rows.size()),id,sheet,b.properties->resolved)){error="Original slow ApplyClassToSheet failed";return -2;}
 for(int property:{48,47,46}){std::int32_t ignored{};if(dh2_property_resolve(b.properties,property,&ignored)){error="Original slow RecalcProperty failed at "+std::to_string(property);return -2;}}
 return 1;
}
}
