#include "character_sneaking_tables.hpp"
#include <cstring>
#include <stdexcept>

namespace dh2::character::sneaking {
SneakingTables::SneakingTables(dh2::data::SkillTables::Borrow source)
 :source_(std::move(source)) {
 if(!source_)throw std::invalid_argument("Missing owned Skill tables");
 const auto& lists=source_.lists();const auto& skills=source_.skills();
 lists_.reserve(lists.size());skills_.resize(skills.size());
 for(const auto& ids:lists)
  lists_.push_back({ids.data(),static_cast<std::uint32_t>(ids.size()),0});
 for(std::size_t i=0;i<skills.size();++i)
  std::memcpy(skills_[i].words,skills[i].scalar.words,sizeof(Skill76));
 view_={lists_.data(),static_cast<std::uint32_t>(lists_.size()),0,
        skills_.data(),static_cast<std::uint32_t>(skills_.size()),0};
}
}
