#pragma once
#include "data.hpp"
namespace dh2::data {
using PropertySheet=std::array<std::int32_t,224>;
struct ClassFormula {std::int32_t destination=-1,type=0,p1=0,p2=0,p3=0;};
struct ClassTables {std::vector<std::string> names;std::vector<std::vector<ClassFormula>> rows;std::size_t data_consumed=0;};
struct ClassRow {const ClassFormula* data=nullptr;std::uint32_t count=0;};
struct PropertyView;
bool load_classes(Bytes records,Bytes names,Bytes schema,ClassTables&,std::string&);
// Cached-sheet class application. With a buff sheet, original buff reads use
// that snapshot; without it, linear reads use the supplied sheet directly.
// Original uncached RecalcProperty/gear/buff resolution is outside this API.
bool apply_class(const ClassTables&,std::int32_t,PropertySheet&,std::string&,const PropertySheet* buff=nullptr);
}
// One recovered formula: 0 applied, 1 dispatch group, 2 stop class (type 8),
// 3 original ignored formula, 4 invalid caller/reference. ARM32 wrap and ASR
// arithmetic are explicit. Group traversal belongs to apply_class.
extern "C" unsigned dh2_class_formula(std::int32_t destination,std::int32_t type,std::int32_t p1,std::int32_t p2,std::int32_t p3,std::int32_t* sheet,const std::int32_t* buff);
extern "C" unsigned dh2_class_apply(const dh2::data::ClassRow* table,std::uint32_t count,std::int32_t id,std::int32_t* sheet,const std::int32_t* buff);
// Original normal base-sheet evaluation: linear source reads resolve the
// current owner property. The target must be the view's base sheet.
extern "C" unsigned dh2_class_apply_to_base(const dh2::data::ClassRow*,std::uint32_t,std::int32_t,std::int32_t* base,dh2::data::PropertyView*);
extern "C" unsigned dh2_class_recalc_base(const dh2::data::ClassRow*,std::uint32_t,std::int32_t* base,dh2::data::PropertyView*);
