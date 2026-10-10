// Protected AS calls from the same authored gameplay movie. Mutable state is
// borrowed freshly from the sole native player on the GL thread.
struct GameplaySettingsNativeContextV1 {
    Impl& self;
    std::shared_ptr<application::ApplicationServicesOwnerV5> application;
    std::shared_ptr<ui::OwnedHudSettingsV1> settings;
};
static bool gameplay_settings_current_v1(Impl& self,GameplaySettingsNativeContextV1& out,std::string& error){
    auto expected=self.process_application_v109.lock();
    if(!expected||!model_renderer::borrow_actual_application_services_v5(out.application,error)||out.application!=expected){
        if(error.empty())error="Gameplay settings lost SAME process Application";return false;
    }
    out.settings=out.application->source_settings4c_v67();
    if(!out.settings){error="Gameplay settings lost actual process SavegameManager";return false;}
    const auto player=model_renderer::player_gameplay_binding();
    if(player.settings&&player.settings!=out.settings.get()){
        error="Gameplay settings no longer match the live player's process SavegameManager";return false;
    }
    error.clear();return true;
}
static bool gameplay_option_string_v1(void* raw,std::int32_t id,std::string& text,std::string& error){
    auto& context=*static_cast<GameplaySettingsNativeContextV1*>(raw);bool is_null{};
    if(!Impl::integer_string(&context.self,id,text,is_null,error))return false;
    if(is_null)text.clear();return true;
}
static bool gameplay_apply_option_v1(void* raw,const char* name,std::int32_t value,std::string& error){
    auto& context=*static_cast<GameplaySettingsNativeContextV1*>(raw);auto& settings=*context.settings;
    if(name&&!std::strcmp(name,"Language")&&context.self.source_korean_build){
        const auto previous=settings.language();
        if(value==6&&previous==3)value=4;else if(value==6&&previous==4)value=5;
        else if(value==3&&previous==6)value=5;else if(value==3&&previous==5)value=4;
    }
    if(name)settings.set_option(name,value); // Source SavegameManager ignores unknown option names.
    (void)settings.saved_option("AutoOrientation"); // UpdateSavedValues -> Android ResetOrientation is bx lr.
    if(name&&(!std::strcmp(name,"VolumeMusic")||!std::strcmp(name,"VolumeFX"))){
        dh2::audio::AudioApplicationBorrowV42 audio;
        if(!model_renderer::borrow_actual_application_audio_v42(audio,error))return false;
        if(audio.manager){const int selector=!std::strcmp(name,"VolumeFX")?1:2;
            if(!audio.manager->set_source_volume_v68(selector,float(settings.saved_option(name)),error))return false;}
    }
    error.clear();return true;
}
static bool gameplay_load_settings_v1(void* raw,std::string& error){
    auto& context=*static_cast<GameplaySettingsNativeContextV1*>(raw);
    if(!context.self.gameplay_settings_actions_v1.load){error="Required process settings load owner unavailable";return false;}
    return context.self.gameplay_settings_actions_v1.load(error);
}
static bool gameplay_save_settings_v1(void* raw,std::string& error){
    auto& context=*static_cast<GameplaySettingsNativeContextV1*>(raw);
    if(!context.self.gameplay_settings_actions_v1.save){error="Required process settings save/language owner unavailable";return false;}
    return context.self.gameplay_settings_actions_v1.save(error);
}
static bool gameplay_enter_options_v1(void* raw,std::string& error){
    auto& context=*static_cast<GameplaySettingsNativeContextV1*>(raw);
    if(!context.self.gameplay_settings_actions_v1.enter){error="Required process Vox music resume owner unavailable";return false;}
    return context.self.gameplay_settings_actions_v1.enter(error);
}
static bool gameplay_refresh_hud_v1(void* raw,std::string& error){
    auto& context=*static_cast<GameplaySettingsNativeContextV1*>(raw);
    if(!context.self.gameplay_settings_actions_v1.refresh_hud){error="Required process HUD refresh owner unavailable";return false;}
    return context.self.gameplay_settings_actions_v1.refresh_hud(error);
}
static bool gameplay_settings_native_v1(Impl& self,const char* name,const gameswf::fn_call& call,std::string& error){
    GameplaySettingsNativeContextV1 context{self,{},{}};
    if(!gameplay_settings_current_v1(self,context,error))return false;
    ui::SwfMenuOptionServicesV1 services;services.settings=context.settings.get();services.context=&context;
    services.string_by_id=gameplay_option_string_v1;services.apply_option=gameplay_apply_option_v1;
    services.reset_fonts=[](void*,std::string& e){return model_renderer::native_reset_process_menu_fonts_v119(e);};
    services.load=gameplay_load_settings_v1;services.save=gameplay_save_settings_v1;
    services.enter=gameplay_enter_options_v1;services.refresh_hud=gameplay_refresh_hud_v1;
    services.japanese_build=self.gameplay_settings_actions_v1.japanese_build;
    return ui::swf_menu_settings_action(name,call,services,error);
}
static bool gameplay_legacy_volume_v1(Impl& self,const char* name,const gameswf::fn_call& call,std::string& error){
    GameplaySettingsNativeContextV1 context{self,{},{}};
    if(!gameplay_settings_current_v1(self,context,error))return false;
    if(call.nargs<1||!call.env){error="Malformed authored legacy volume callback";return false;}
    const float volume=static_cast<float>(call.arg(0).to_number());
    if(!std::isfinite(volume)){error="Legacy volume option is not a finite source float";return false;}
    dh2::audio::AudioApplicationBorrowV42 audio;
    if(!model_renderer::borrow_actual_application_audio_v42(audio,error))return false;
    if(!audio.manager){error.clear();return true;} // Original global VoxSoundManager null branch.
    const int selector=!std::strcmp(name,"NativeOptionFX")?1:2;
    if(!audio.manager->set_source_volume_v68(selector,volume,error))return false;
    const auto value=volume>=2147483647.f?INT32_MAX:volume<=-2147483648.f?INT32_MIN:static_cast<std::int32_t>(volume);
    context.settings->set_option(selector==1?"VolumeFX":"VolumeMusic",value);
    error.clear();return true;
}
static bool gameplay_native_settings_name_v1(const char* name){
    return name&&(!std::strcmp(name,"NativeSetOptions")||!std::strcmp(name,"NativeSaveSettings")||
        !std::strcmp(name,"NativeEnterOptionMenu")||!std::strcmp(name,"NativeRefreshHudManager")||
        !std::strcmp(name,"NativeIsJapaneseVersion")||!std::strcmp(name,"NativeIsKorean"));
}
static bool gameplay_native(void* context,const char* name,const gameswf::fn_call& call,std::string& error){
    auto& self=*static_cast<Impl*>(context);
    if(ui::AuthoredCharacterApplicationV1::owns(name)){
        if(!self.process_startup_bridge_v109){error=std::string("Required SAME process HUD startup callback: ")+name;return false;}
        const bool delivered=self.process_startup_bridge_v109->dispatch(name,call,error);
        if(delivered)__android_log_print(ANDROID_LOG_INFO,tag,"Original HUD startup callback | %s | same process Application",name);
        return delivered;
    }
    bool message_handled{};
    if(!self.message_native_v97(name,call,message_handled,error))return false;
    if(message_handled)return true;
    if(name&&(!std::strcmp(name,"NativeGetNextStatusMessage")||!std::strcmp(name,"NativeStopMessage"))){
        bool handled=false;
        if(!ui::menu_status_native_v26(*self.message_owner(),name,call,handled,error))return false;
        if(handled)return true;
        error="Required original non-status message family provider";return false;
    }
    if(name&&!std::strcmp(name,"NativeChangeRolloverInputBehavior"))
        return dispatch_process_menu_rollover_v1(name,call,error);
    if(gameplay_native_settings_name_v1(name))return gameplay_settings_native_v1(self,name,call,error);
    if(name&&(!std::strcmp(name,"NativeOptionFX")||!std::strcmp(name,"NativeOptionMusic")))
        return gameplay_legacy_volume_v1(self,name,call,error);
    if(!self.gameplay_bridge){error="Authored gameplay callback bridge unavailable";return false;}
    return self.gameplay_bridge->dispatch(name,call,error);
}
bool gameplay_call(const char* name,ui::CharacterMenuCallV1& call,std::string& error){
    const auto live=model_renderer::player_gameplay_binding();
    if(!name){error="Null authored HUD callback";return false;}
    if(!std::strcmp(name,"NativeGetOptionParameters")||!std::strcmp(name,"NativeUseIpodPlayer")){
        GameplaySettingsNativeContextV1 owner{*this,{},{}};
        if(!gameplay_settings_current_v1(*this,owner,error))return false;
        struct Backend {Impl& self;ui::OwnedHudSettingsV1& settings;std::string& error;std::string text;} backend{*this,*owner.settings,error,{}};
        const ui::HudInitServices16 services{&backend,[](void* raw,const ui::HudInitRequest64* q,ui::HudInitResponse32* out)->int{
            auto& b=*static_cast<Backend*>(raw);using Op=ui::HudInitOperation;
            if(q->operation==Op::language_override){out->value=b.self.source_korean_build;return 1;}
            if(q->operation==Op::string_symbol){bool is_null=false;
                if(!b.self.localization.integer_string(q->value,b.self.text_services(),b.text,is_null,b.error))return 0;
                out->text=is_null?nullptr:b.text.c_str();return 1;}
            if(q->operation==Op::platform_music_support){
                if(!b.self.platform_music){b.error="Required Android isSupportMM JNI receiver";return 0;}
                return b.self.platform_music(out->value,b.error)?1:0;
            }
            b.error="Required original HUD option application operation "+std::to_string(static_cast<unsigned>(q->operation));return 0;
        }};
        return ui::authored_hud_options_bridge_v1(name,call,backend.settings,services,error);
    }
    if(!std::strcmp(name,"NativeSkillGetEquipedSkillsIDs")||!std::strcmp(name,"NativeGetSkillDetails")||
       !std::strcmp(name,"NativeHUDGetActiveFaery")||!std::strcmp(name,"NativeSwapEquipment")){
        if(!gameplay_queries){error="Same-player HUD query owner unavailable";return false;}
        return gameplay_queries->dispatch(live,name,call,error);
    }
    int operation=-1,index=0;
    if(!std::strcmp(name,"NativeHUDSkill")){
        if(call.arguments.size()!=1||call.arguments[0].kind!=2)return true;
        const auto value=call.arguments[0].number;if(!std::isfinite(value)||value<0||value>2)return true;
        operation=0;index=static_cast<int>(value);
    }else if(!std::strcmp(name,"NativeHUDSpell"))operation=1;
    else if(!std::strcmp(name,"NativeUsePotion"))operation=2;
    if(operation<0){error=std::string("Required authored HUD application callback: ")+name;return false;}
    // The source HUD calls these actions on its current local player. A
    // missing binding is a broken same-player bridge, not an ordinary
    // cooldown/locked rejection; reject it before the string-valued action
    // adapter can turn "Player action unavailable" into a successful AS call.
    if(!live.active||!live.life||!live.save||!live.skills||!live.properties||!live.state){
        error="Authored gameplay action lost the same live player/save/skill/property/state owners";
        return false;
    }
    const auto result=model_renderer::player_gameplay_action(operation,index);
    __android_log_print(ANDROID_LOG_INFO,tag,"Authored gameplay control | %s | %s",name,result.c_str());
    // A known missing provider must fail the protected AS call. In particular,
    // the faery branch reports an unavailable cast owner before it can reach
    // the cast command; treating that diagnostic as an ordinary cooldown/no-op
    // made NativeHUDSpell look delivered when no gameplay owner received it.
    if(result.find("failed")!=std::string::npos||result.find("requires source")!=std::string::npos||
       result.find("owner unavailable")!=std::string::npos){error=result;return false;}
    return true;
}
ui::AuthoredJoystickServicesV1 joystick_services(){
    return {this,
        [](void* context,std::int32_t x,std::int32_t y,std::string& e){return static_cast<Impl*>(context)->gameplay->joystick_stick_position(x,y,e);},
        [](void*,bool& allowed,std::string& e){const auto live=model_renderer::player_gameplay_binding();if(!live.active||!live.controller.controller){e="Actual joystick player/controller unavailable";return false;}allowed=live.controller.forced||(!live.controller.global_blocked&&!live.controller.locked);return true;},
        [](void*,const float base[3],float degrees,float out[3],std::string&){dh2::math::Vector3f value{base[0],base[1],base[2]},origin{};dh2_vec3_normalize(&value);dh2_vec3_rotate_xy(&value,degrees,&origin);out[0]=value.x;out[1]=value.y;out[2]=value.z;return true;},
        [](void*,const float direction[3],std::string& error){return model_renderer::authored_hud_command(direction,false,error);},
        [](void*,std::string& error){return model_renderer::authored_hud_command(nullptr,true,error);}};
}
