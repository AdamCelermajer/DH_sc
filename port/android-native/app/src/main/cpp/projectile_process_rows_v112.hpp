#pragma once
#include "source_process_arrays_v101.hpp"
#include <projectile_methods_v112.hpp>
#include <array>
namespace model_renderer {
//Structs.Projectile.read4edbb8's 72-byte source layout projected from the
//already-decoded immutable process owner. Float fields retain exact bits.
inline bool bind_projectile_process_rows_v112(
 const std::shared_ptr<dh2::android_ui::SourceProcessArraysV101>& arrays,
 dh2::world::ProjectileMethodServicesV112& out,std::string& e){
 const auto* group=arrays?arrays->group("ProjectileTable"):nullptr;
 if(!arrays||!arrays->ready()||!group||!group->records_loaded||
    group->declared_rows!=group->rows.size()||out.table_count||out.table_row){
  e="Required once-bound SAME process ProjectileTable rows";return false;
 }
 out.table_count=[arrays](std::uint32_t& count,std::string& e){
  auto group=arrays->group("ProjectileTable");
  if(!group||!group->records_loaded||group->declared_rows!=group->rows.size()){
   e="Changed actual ProjectileTable count authority";return false;
  }
  count=group->declared_rows;e.clear();return true;
 };
 out.table_row=[arrays](std::int32_t row,dh2::world::ProjectileTableRowV112& out,std::string& e){
  const auto* group=arrays->group("ProjectileTable");
  if(!group||!group->records_loaded||row<0||static_cast<std::size_t>(row)>=group->rows.size()||
     group->rows[row].fields.size()!=20){e="Required actual ProjectileTable selected scalar row";return false;}
  auto field=[arrays,row](unsigned offset,bool byte,std::uint32_t& bits,std::string& e){
   static constexpr std::array<unsigned,20> offsets{4,5,8,12,16,20,24,28,29,32,36,40,44,48,52,53,56,60,64,68};
   const auto* group=arrays->group("ProjectileTable");
   if(!group||row<0||static_cast<std::size_t>(row)>=group->rows.size()||group->rows[row].fields.size()!=offsets.size()){
    e="Retired actual ProjectileTable row";return false;
   }
   for(std::size_t i=0;i<offsets.size();++i)if(offsets[i]==offset){
    const auto& actual=group->rows[row].fields[i];
    const auto expected=byte?dh2::android_ui::ProcessArrayValueV101::Kind::byte:
     dh2::android_ui::ProcessArrayValueV101::Kind::word;
    if(actual.kind!=expected||(byte&&actual.bits>255)){
     e="ProjectileTable selected source field kind differs";return false;
    }
    bits=actual.bits;e.clear();return true;
   }
   e="Unknown original ProjectileTable scalar offset";return false;
  };
  dh2::world::ProjectileTableRowV112 loan;loan.owner=arrays;
  loan.word=[field](unsigned offset,std::uint32_t& bits,std::string& e){return field(offset,false,bits,e);};
  loan.byte=[field](unsigned offset,std::uint8_t& value,std::string& e){std::uint32_t bits{};
   if(!field(offset,true,bits,e))return false;value=static_cast<std::uint8_t>(bits);return true;};
  out=std::move(loan);e.clear();return true;
 };
 e.clear();return true;
}
}
