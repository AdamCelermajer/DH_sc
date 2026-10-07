#include "native_conditions_v69.hpp"
#include <cstring>
#include <exception>
#include <limits>
namespace dh2::world {namespace {
struct Reader {
 data::Bytes bytes;std::size_t at{};std::string& error;
 bool word(std::uint32_t& value){if(at>bytes.size||bytes.size-at<4){error="Truncated original v2conditions word";return false;}const auto* b=bytes.data+at;at+=4;
  value=std::uint32_t(b[0])|std::uint32_t(b[1])<<8|std::uint32_t(b[2])<<16|std::uint32_t(b[3])<<24;return true;}
 bool integer(std::int32_t& value){std::uint32_t raw;if(!word(raw))return false;std::memcpy(&value,&raw,4);return true;}
 bool strings(std::vector<std::string>& out,std::size_t max_rows,std::size_t& string_bytes,std::size_t max_bytes){
  std::uint32_t count;if(!word(count))return false;
  if(count>max_rows||count>(bytes.size-at)/4){error="v2conditions names exceed bounded transport or available words";return false;}
  out.reserve(count);
  for(std::uint32_t i=0;i<count;++i){std::uint32_t length;if(!word(length))return false;
   if(length>bytes.size-at||string_bytes>max_bytes||length>max_bytes-string_bytes){error="Truncated or over-budget v2conditions CString";return false;}
   std::string text(reinterpret_cast<const char*>(bytes.data+at),length);at+=length;string_bytes+=length;
   //Source readNames adds its own terminator; embedded NUL is not a new name.
   if(text.find('\0')!=std::string::npos){error="Embedded NUL in original v2conditions name/schema";return false;}out.push_back(std::move(text));
  }return true;
 }
};
}
bool NativeConditionTableV69::decode(data::Bytes records,data::Bytes names,data::Bytes schema,const NativeConditionDecodeBudgetV69& budget,std::string& error){
 if(ready_){error="Immutable Arrays.v2Conditions table cannot be replayed";return false;}
 if((records.size&&!records.data)||(names.size&&!names.data)||(schema.size&&!schema.data)||
    records.size>budget.input_bytes||names.size>budget.input_bytes-records.size||schema.size>budget.input_bytes-records.size-names.size){error="Required bounded original v2conditions cache spans";return false;}
 try{
  Reader n{names,0,error},s{schema,0,error},r{records,0,error};std::size_t name_bytes{};std::vector<std::string> decoded_names;
  if(!n.strings(decoded_names,budget.rows,name_bytes,budget.name_bytes)||n.at!=names.size){if(error.empty())error="Unconsumed v2conditions name suffix";return false;}
  std::vector<std::vector<std::string>> structures;
  while(s.at<schema.size){std::vector<std::string> fields;if(!s.strings(fields,budget.rows,name_bytes,budget.name_bytes))return false;structures.push_back(std::move(fields));}
  if(structures.size()<3||structures.front()!=std::vector<std::string>{"Op"}||structures[1]!=std::vector<std::string>{"Op","Op1","Op2"}||structures.back()!=std::vector<std::string>{"Conds","Type"}){error="Original v2conditions native struct schema differs";return false;}
  for(std::size_t i=2;i+1<structures.size();++i){const auto& f=structures[i];
   if(f.size()!=3||f[0]!="Op"||!((f[1]=="Quest"&&f[2]=="State")||(f[1]=="Event"&&f[2]=="State")||(f[1]=="Level"&&f[2]=="NotUsed1"))){error="Unsupported original v2condition stub metadata";return false;}}
  std::uint32_t count;if(!r.word(count))return false;
  if(count>budget.rows||count>(records.size-r.at)/8||count!=decoded_names.size()){error="Original v2conditions row/name count differs or exceeds transport";return false;}
  std::vector<NativeConditionRowV69> rows;rows.reserve(count);std::size_t total_stubs{};
  for(std::uint32_t i=0;i<count;++i){NativeConditionRowV69 row;row.name=std::move(decoded_names[i]);std::uint32_t length;if(!r.word(length))return false;
   if(length>static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())||length>(records.size-r.at)/12||total_stubs>budget.stubs||length>budget.stubs-total_stubs){error="Original v2conditions stub array exceeds source signed count/transport";return false;}
   row.count4=static_cast<std::int32_t>(length);total_stubs+=length;
   //The native reader allocates even an empty array header. A genuine native
   //zero-length allocation supplies its real pointer identity to AssignPyData.
   auto stubs=std::make_unique<NativeConditionStubV69[]>(length);
   for(std::uint32_t j=0;j<length;++j){auto& stub=stubs[j];
    if(!r.integer(stub.op4)||!r.integer(stub.argument8)||!r.integer(stub.argument_c))return false;
    if(stub.op4<0||stub.op4>=7){error="Original condition operator outside captured seven-entry native factory";return false;}}
   if(!r.integer(row.type_c))return false;row.stubs8=std::move(stubs);rows.push_back(std::move(row));
  }
  if(r.at!=records.size){error="Unconsumed original v2conditions record suffix";return false;}
  std::vector<ConditionDataRowV3> source;source.reserve(rows.size());
  for(const auto& row:rows)source.push_back({row.name,static_cast<std::uintptr_t>(row.count4),reinterpret_cast<std::uintptr_t>(row.stubs8.get())});
  rows_=std::move(rows);condition_data_rows_=std::move(source);ready_=true;error.clear();return true;
 }catch(const std::exception& exception){error=std::string("Original condition table allocation/read failed: ")+exception.what();return false;}
}
const NativeConditionRowV69* NativeConditionTableV69::argument_receiver(std::uintptr_t stubs,std::uintptr_t count)const noexcept{
 if(!ready_)return nullptr;
 for(const auto& row:rows_)if(reinterpret_cast<std::uintptr_t>(row.stubs8.get())==stubs&&static_cast<std::uintptr_t>(row.count4)==count)return &row;
 return nullptr;
}
bool read_native_condition_table_v69(const NativeConditionCacheInputsV69& input,
 std::shared_ptr<const NativeConditionTableV69>& out,std::string& e){
 if(out||!input.actual_cache||!input.read){e="Required unproduced original Arrays.v2Conditions cache owner/read service";return false;}
 std::array<std::vector<std::uint8_t>,3> bytes;
 constexpr const char* names[]{"data/pydata/v2conditions_pyarray.bin","data/pydata/v2conditions_pyarraynames.bin","data/pydata/v2conditions_pystructnames.bin"};
 std::size_t total{};
 for(unsigned i=0;i<3;++i){bool found{};if(!input.read(names[i],found,bytes[i],e))return false;
  if(!found){e=std::string("Actual original condition table asset absent: ")+names[i];return false;}
  if(total>input.budget.input_bytes||bytes[i].size()>input.budget.input_bytes-total){e="Original condition table cache read exceeded explicit transport budget";return false;}total+=bytes[i].size();
 }
 auto table=std::make_shared<NativeConditionTableV69>();
 if(!table->decode({bytes[0].data(),bytes[0].size()},{bytes[1].data(),bytes[1].size()},
                  {bytes[2].data(),bytes[2].size()},input.budget,e))return false;
 out=std::move(table);return true;
}
}
