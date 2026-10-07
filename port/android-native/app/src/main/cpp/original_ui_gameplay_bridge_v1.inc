// Protected AS calls from the same authored gameplay movie. Mutable state is
// borrowed freshly from the sole native player on the GL thread.
static bool gameplay_native(void* context,const char* name,const gameswf::fn_call& call,std::string& error){
    auto& self=*static_cast<Impl*>(context);
    bool message_handled{};
    if(!self.message_native_v97(name,call,message_handled,error))return false;
    if(message_handled)return true;
    if(name&&(!std::strcmp(name,"NativeGetNextStatusMessage")||!std::strcmp(name,"NativeStopMessage"))){
        bool handled=false;
        if(!ui::menu_status_native_v26(*self.message_owner(),name,call,handled,error))return false;
        if(handled)return true;
        error="Required original non-status message family provider";return false;
    }
    if(!self.gameplay_bridge){error="Authored gameplay callback bridge unavailable";return false;}
    return self.gameplay_bridge->dispatch(name,call,error);
}
bool gameplay_call(const char* name,ui::CharacterMenuCallV1& call,std::string& error){
    const auto live=model_renderer::player_gameplay_binding();
    if(!name){error="Null authored HUD callback";return false;}
    if(!std::strcmp(name,"NativeGetOptionParameters")||!std::strcmp(name,"NativeUseIpodPlayer")){
        if(!live.settings){error="HUD options lost the actual Application settings";return false;}
        struct Backend {Impl& self;std::string& error;std::string text;} backend{*this,error,{}};
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
        return ui::authored_hud_options_bridge_v1(name,call,*live.settings,services,error);
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
    const auto result=model_renderer::player_gameplay_action(operation,index);
    __android_log_print(ANDROID_LOG_INFO,tag,"Authored gameplay control | %s | %s",name,result.c_str());
    if(result.find("failed")!=std::string::npos||result.find("requires source")!=std::string::npos){error=result;return false;}
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
