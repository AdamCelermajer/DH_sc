#include "visual_fx_tables.hpp"
#include <stdexcept>
namespace dh2::fx {
PreloadBacking::PreloadBacking(data::EffectsTables::Borrow source):source_(std::move(source)){
 const auto& rows=source_.sets();const auto effects=source_.dictionary().values.size();
 if(rows.size()>4096||effects>4096)throw std::runtime_error("FX preload table dimensions outside native contract");
 steps_.resize(rows.size());sets_.resize(rows.size());
 for(std::size_t i=0;i<rows.size();++i){
  if(rows[i].steps.size()>4096)throw std::runtime_error("FX preload step dimensions outside native contract");
  for(const auto& step:rows[i].steps)steps_[i].push_back({step.file,step.redir});
  sets_[i]={steps_[i].empty()?nullptr:steps_[i].data(),static_cast<std::int32_t>(steps_[i].size()),0};
 }
 table_={sets_.empty()?nullptr:sets_.data(),static_cast<std::uint32_t>(sets_.size()),static_cast<std::uint32_t>(effects)};
}
std::unique_ptr<PreloadBacking> PreloadBacking::create(data::EffectsTables::Borrow source,std::string& error){
 error.clear();try{
  if(!source)throw std::runtime_error("Missing owned effects snapshot");
  return std::unique_ptr<PreloadBacking>(new PreloadBacking(std::move(source)));
 }catch(const std::exception& e){error=e.what();return nullptr;}
}
}
