// Public provider enrollment in existing source runtime namespace. Existing
// borrow_source_campaign_condition_world_v70 returns SAME actual source owner.
bool bind_source_campaign_stage14_inactive_v97(const SourceCampaignCandidateBorrowV55& candidate,
 dh2::loader::Stage14InactiveSourcesV97 leaves,std::string& e){
 std::shared_ptr<SourceWorldBorrowV61> world;
 if(!borrow_source_campaign_condition_world_v70(candidate,world,e)||!world||!world->application||!candidate.level||!leaves.owner){if(e.empty())e="Required SAME actual World/App/Level inactive-stage14 source";return false;}
 auto aliases=[](const auto& a,const auto& b){return a&&b&&!a.owner_before(b)&&!b.owner_before(a);};
 if(aliases(leaves.owner,candidate.actual_world)||aliases(leaves.owner,world->application)||aliases(leaves.owner,candidate.level)||world->stage14_inactive_v97){e="Required independent native tail authority and unbound SAME provider slot";return false;}
 if(candidate.level->constructor_fields_v3().field130>=14){e="Inactive Stage14 providers must enroll before actual source delivery";return false;}
 world->stage14_inactive_v97=std::make_shared<dh2::loader::Stage14InactiveSourcesV97>(std::move(leaves));e.clear();return true;
}
