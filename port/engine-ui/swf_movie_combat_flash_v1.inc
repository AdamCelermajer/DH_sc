// Integration: include inside dh2::ui after SwfMovie::Impl is defined.
// Declare in SwfMovie's public section:
// bool source_display_rectangle(float rectangle[4], std::int32_t viewport[4], std::string&);
// Called before CombatFlashSwfV1 enters action_script, never from its callback.
bool SwfMovie::source_display_rectangle(float rectangle[4],std::int32_t viewport[4],std::string& error){
 auto owner=impl_;auto connection=viewport_;
 if(!owner||!owner->root||!connection||!rectangle||!viewport){error="Required retained source combat viewport unavailable";return false;}
 Impl::Scope scope(owner.get());if(!scope.entered){error="SWF core busy";return false;}
 if(!connection->display_rectangle(rectangle,error))return false;
 const auto& source=connection->state().viewport;
 for(unsigned i=0;i<4;++i)viewport[i]=source[i];
 return owner->finish(error);
}
