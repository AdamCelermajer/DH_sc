#include "source_cinematics.hpp"
#include <cstring>
#include <stdexcept>
#include <utility>
namespace dh::foundation { namespace {
bool missing(const char* service,std::string& e){if(e.empty())e=std::string("Required original cinematic ")+service;return false;}
bool word(const OriginalCampaignCommand& c,unsigned offset,std::int32_t& out,std::string& e){
    const auto i=c.scalars.find(offset);if(i==c.scalars.end()){e="Missing authored cinematic scalar "+std::to_string(offset);return false;}
    std::memcpy(&out,&i->second,4);return true;
}
bool text(const OriginalCampaignCommand& c,unsigned offset,const std::string*& out,std::string& e){
    const auto i=c.strings.find(offset);if(i==c.strings.end()){e="Missing authored cinematic string "+std::to_string(offset);return false;}
    out=&i->second;return true;
}
bool all_case_insensitive(const std::string& s){return s.size()==3&&(s[0]=='A'||s[0]=='a')&&(s[1]=='L'||s[1]=='l')&&(s[2]=='L'||s[2]=='l');}
}
SourceCinematicCommands::SourceCinematicCommands(SourceCinematicProviders p):providers_(std::move(p)){}
bool SourceCinematicCommands::command(CampaignCommandPhase phase,const OriginalCampaignCommand& c,int module,
    bool skip,bool& handled,bool& blocking,std::string& e){
    e.clear();handled=false;blocking=false;
    switch(c.kind){case 1:case 2:case 4:case 5:case 6:case 7:case 8:case 24:case 25:case 40:case 41:case 45:case 46:handled=true;break;default:return true;}
    if(phase==CampaignCommandPhase::update)return true; // original base Update45562c BX LR
    if(c.kind==4)return true; // source SetCamera455678 literal BX LR
    if(phase==CampaignCommandPhase::execute&&(c.kind==24||c.kind==45)&&skip)return true;
    if(phase==CampaignCommandPhase::execute){
        if(c.kind==1||c.kind==2)return providers_.cutsceneMode?providers_.cutsceneMode(c.kind==1,skip,module,e):missing("PM/UI/network Enter/ExitCutSceneMode",e);
        if(!providers_.trace||!providers_.trace(e))return missing("Debug tracing source service",e);
    }
    if(c.kind==1||c.kind==2||c.kind==24||c.kind==25){
        if(phase!=CampaignCommandPhase::execute)return true; // literal false IsBlocking
        const std::string* name{};if(!text(c,12,name,e))return false;
        std::shared_ptr<void> pin;std::uint8_t* cell=nullptr;
        if(c.kind==24?all_case_insensitive(*name):*name=="All"){
            if(!providers_.globalControl||!providers_.globalControl(pin,cell,e))return missing("global controller block cell",e);
        }else{
            const auto manager=providers_.objects.lock();if(!manager)return missing("SAME ObjectManager",e);
            dh2::target_providers::Handle16 handle{};const dh2::world::CanonicalObjectBorrowV1* object=nullptr;
            if(!manager->by_name(name->c_str(),module,false,nullptr,handle,e)||!manager->resolve_handle_v4(handle,false,object,{},e))return false;
            if(!object)return true;std::uintptr_t character=0;
            if(!object->as_character||!object->as_character(object->context,character,e))return false;
            if(!character)return true;
            if(!providers_.actorControl||!providers_.actorControl(character,pin,cell,e))return missing("SAME Character controller locked8 cell",e);
        }
        if(!pin||!cell)return missing("retained actual control cell",e);
        *cell=c.kind==24?1:0;return true;
    }
    if(c.kind==40||c.kind==41||c.kind==46)return actor_command(phase,c,module,skip,blocking,e);
    if(c.kind==8){
        if(!providers_.targetCamera)return missing("SAME camera target transition owner",e);
        return providers_.targetCamera->command(phase,c,skip,handled,blocking,e);
    }
    if(c.kind==45){
        std::int32_t wait=0;if(phase==CampaignCommandPhase::is_blocking&&!word(c,28,wait,e))return false;
        if(phase==CampaignCommandPhase::is_blocking&&!wait)return true;
        const auto manager=providers_.objects.lock();if(!manager)return missing("SAME actor animation ObjectManager",e);
        const dh2::world::CanonicalObjectBorrowV1* object=nullptr;
        if(phase==CampaignCommandPhase::execute){
            const std::string* name{};if(!text(c,24,name,e))return false;
            auto& handle=actors_[&c];
            if(!manager->by_name(name->c_str(),module,false,nullptr,handle,e)||!manager->resolve_handle_v4(handle,false,object,{},e))return false;
            if(!object)return true;std::uintptr_t character=0;
            if(!object->as_character||!object->as_character(object->context,character,e))return false;
            if(!character)return true;
            std::int32_t first=0,second=0,base=0;
            if(!word(c,8,first,e)||!word(c,12,second,e)||!word(c,16,base,e))return false;
            return providers_.playActor?providers_.playActor(character,first,second,base,e):missing("authored actor animation/FSM service",e);
        }
        if(!wait)return true;
        const auto found=actors_.find(&c);if(found==actors_.end())return missing("retained executed animation handle",e);
        auto& handle=found->second;
        // Source IsBlocking resolves SAME stored handle twice, preserving frame/cache.
        if(!manager->resolve_handle_v4(handle,false,object,{},e))return false;if(!object)return true;
        if(!manager->resolve_handle_v4(handle,false,object,{},e))return false;if(!object)return true;
        std::int32_t base=0;if(!word(c,16,base,e))return false;
        return providers_.actorBlocking?providers_.actorBlocking(object->identity,base,blocking,e):missing("actual actor animation wait state",e);
    }
    std::int32_t wait=1;if(c.kind==5&&phase==CampaignCommandPhase::is_blocking&&!word(c,12,wait,e))return false;
    if(phase==CampaignCommandPhase::is_blocking&&(c.kind==6||!wait))return true;
    std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
    if(!providers_.camera||!providers_.camera(camera,e))return missing("borrowed current-Level camera",e);
    if(!camera)return true;
    if(!camera->level()){if(c.kind==6)return true;return missing("SAME CameraLevel",e);}
    if(phase==CampaignCommandPhase::is_blocking){blocking=camera->level()->fields().shake84;return true;}
    if(c.kind==7)return true; // source WaitCamera executes Debug only
    if(c.kind==5){
        if(skip)return camera->play_idle(e);
        std::int32_t animation=0;if(!word(c,8,animation,e))return false;
        return camera->level()->play_animation(animation,0,false,e);
    }
    // SetCameraClip uses authored near+12 before far+8, -2 loaded defaults.
    std::int32_t near=0,far=0;if(!word(c,12,near,e))return false;
    if(near==-2){std::int32_t ignored=0;if(!camera->source_clip_defaults_v120(near,ignored,e))return false;}
    if(near>0&&!camera->set_clip_start(static_cast<float>(near),e))return false;
    if(!word(c,8,far,e))return false;
    if(far==-2){std::int32_t ignored=0;if(!camera->source_clip_defaults_v120(ignored,far,e))return false;}
    return far<=0||camera->set_clip_end(static_cast<float>(far),e);
}
bool SourceCinematicCommands::actor_command(CampaignCommandPhase phase,const OriginalCampaignCommand& c,int module,
    bool skip,bool& blocking,std::string& e){
    // Whole source command orchestration from source_campaign_script_execution_v96.
    // The callbacks below own the original methods, including failure prefixes.
    if(phase==CampaignCommandPhase::is_blocking){
        if(c.kind!=40)return true; // Look/SetActorPosition literal false.
        const auto state=moves_.find(&c);if(state==moves_.end())return missing("executed MoveActor receiver state",e);
        if(!state->second.wait||!state->second.actor)return true;
        return providers_.moveBlocking?providers_.moveBlocking(state->second.actor,state->second.originalStatic,blocking,e):missing("actual MoveActor path/Stop/Revive wait",e);
    }
    const auto manager=providers_.objects.lock();if(!manager)return missing("SAME actor command ObjectManager",e);
    using Object=dh2::world::CanonicalObjectBorrowV1;
    auto lookup=[&](unsigned offset,const Object*& out,dh2::target_providers::Handle16& handle,const char* filter=nullptr){
        const std::string* name{};return text(c,offset,name,e)&&manager->by_name(name->c_str(),module,false,filter,handle,e)&&manager->resolve_handle_v4(handle,false,out,{},e);
    };
    auto as_character=[&](const Object* object,std::uintptr_t& out){out=0;if(!object)return true;return object->as_character&&object->as_character(object->context,out,e);};
    auto player=[&](std::uintptr_t actor,bool& out){return providers_.isPlayer?providers_.isPlayer(actor,out,e):missing("SAME Character.IsPlayer",e);};
    auto look=[&](std::uintptr_t actor,std::uintptr_t target){
        std::shared_ptr<void> pin;std::uint8_t* forced=nullptr;
        if(!providers_.controllerForced||!providers_.controllerForced(actor,pin,forced,e))return missing("SAME controller forced9 cell",e);
        if(!pin||!forced)return missing("retained actual forced9 cell",e);
        *forced=1;
        if(!providers_.controllerLook||!providers_.controllerLook(actor,target,e))return missing("actual Cmd_LookAt",e);
        *forced=0;return true; // Failed command retains reached source forced store.
    };
    auto point=[&](std::uintptr_t actor,std::shared_ptr<void>& pin,const float*& out){
        if(!providers_.position||!providers_.position(actor,pin,out,e))return missing("SAME object Position160",e);
        return pin&&out?true:missing("retained actual Position160",e);
    };
    auto place=[&](std::uintptr_t actor,const float* xyz){
        if(!providers_.setPosition||!providers_.setPosition(actor,xyz,true,e))return missing("actual SetPosition(true)",e);
        return providers_.forceUpdatePosition?providers_.forceUpdatePosition(actor,e):missing("actual ForceUpdatePosition/dirty208",e);
    };
    const Object* recipient=nullptr;const Object* target=nullptr;
    dh2::target_providers::Handle16 recipientHandle{},targetHandle{};
    if(!lookup(c.kind==40?24:20,recipient,recipientHandle))return false;
    std::uintptr_t character=0;
    // SetActorPosition resolves both handles before AsChar; Look/Move AsChar first.
    if(c.kind!=46&&!as_character(recipient,character))return missing("actual ObjectHandle.AsChar",e);
    const std::string* targetName{};if(!text(c,12,targetName,e))return false;
    const std::string* recipientName{};if(!text(c,c.kind==40?24:20,recipientName,e))return false;
    const char* filter=c.kind==41&&*targetName=="HighestThreatPlayer"?recipientName->c_str():nullptr;
    if(!lookup(12,target,targetHandle,filter))return false;
    if(c.kind==40){
        std::int32_t wait=0,collision=0;if(!word(c,28,wait,e)||!word(c,16,collision,e))return false;
        auto& state=moves_[&c];state={};state.wait=wait!=0;state.actor=character;
        if(character){
            state.lease=recipient->lease;std::shared_ptr<void> pin;const std::uint8_t* cell=nullptr;
            if(!providers_.actorStatic||!providers_.actorStatic(character,pin,cell,e))return missing("SAME captured actor byte84",e);
            if(!pin||!cell||!state.lease)return missing("retained MoveActor receiver/byte84",e);state.originalStatic=*cell;
        }
        if(!character||!target){state.wait=false;return true;}
        return providers_.moveActor?providers_.moveActor(character,target->identity,skip,collision!=0,wait!=0,state.originalStatic,state.wait,e):missing("actual GoTo/controller/physical/path body",e);
    }
    if(c.kind==46){
        if(!recipient||!target)return true;
        if(!as_character(recipient,character))return missing("actual ObjectHandle.AsChar",e);
        if(character){
            if(!providers_.setIdle||!providers_.setIdle(character,false,e))return missing("actual SM_SetIdleState(false)",e);
            bool isPlayer=false;if(!player(character,isPlayer))return false;
            if(isPlayer)for(int i=1;i<4;++i){
                std::uintptr_t peer=0;if(!providers_.peerCharacter||!providers_.peerCharacter(i,peer,e))return missing("actual PM.GetPlayer(i,false)",e);
                if(!peer)continue;std::shared_ptr<void> pin;const float* xyz=nullptr;if(!point(target->identity,pin,xyz))return false;
                const float placed[]={xyz[0]+(i==1?-200.f:i==2?200.f:0.f),xyz[1]+(i==3?-200.f:0.f),xyz[2]+0.f};
                if(!place(peer,placed))return false;
            }
        }
        std::shared_ptr<void> pin;const float* xyz=nullptr;return point(target->identity,pin,xyz)&&place(recipient->identity,xyz);
    }
    if(character&&target){
        bool isPlayer=false;if(!player(character,isPlayer))return false;
        if(isPlayer){
            bool level=false;if(!providers_.currentLevel||!providers_.currentLevel(level,e))return missing("actual current Level presence",e);
            if(level)for(int i=1;i<4;++i){std::uintptr_t peer=0;if(!providers_.peerCharacter||!providers_.peerCharacter(i,peer,e))return missing("actual PM.GetPlayer(i,false)",e);if(peer&&!look(peer,target->identity))return false;}
        }
        if(!look(character,target->identity))return false;
    }
    const Object* finalTarget=nullptr;return manager->resolve_handle_v4(targetHandle,false,finalTarget,{},e);
}
bool SourceCinematicCommands::scene_phase(std::uint32_t timestamp,std::string& e){
    e.clear();
    std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11> camera;
    if(!providers_.camera||!providers_.camera(camera,e))return missing("borrowed current-Level camera frame",e);
    return !camera||camera->scene_phase(timestamp,e);
}
struct SourceCinematicSession::Impl {
    std::shared_ptr<OriginalCampaignRuntime> runtime;SourceCinematicSessionProviders providers;
    CinematicState state=CinematicState::ready;int script=-1,module=-1;bool skip=false,busy=false;
    void publish(){if(providers.event)providers.event({state,script,module});}
};
SourceCinematicSession::SourceCinematicSession(std::shared_ptr<OriginalCampaignRuntime> runtime,SourceCinematicSessionProviders providers):impl_(std::make_shared<Impl>()){
    if(!runtime||!providers.commands)throw std::invalid_argument("Loaded original campaign and cinematic source commands required");
    for(const auto& script:runtime->scripts())if(runtime->running(script.id))throw std::invalid_argument("Cinematic session cannot replace active campaign services");
    impl_->runtime=std::move(runtime);impl_->providers=std::move(providers);
    const std::weak_ptr<Impl> weak=impl_;auto services=impl_->providers.campaign;
    const auto remaining=services.command;
    services.command=[weak,remaining](auto phase,const auto& command,int module,bool& block,std::string& e){
        const auto self=weak.lock();if(!self)return missing("live exclusive session",e);
        bool handled=false;if(!self->providers.commands->command(phase,command,module,self->skip,handled,block,e))return false;
        if(handled)return true;
        return remaining?remaining(phase,command,module,block,e):missing("remaining reached authored effect",e);
    };
    impl_->runtime->bind(std::move(services));
}
bool SourceCinematicSession::start(int script,int module,bool received,std::string& e){
    e.clear();
    auto& self=*impl_;if(self.state!=CinematicState::ready||self.busy){e="Cinematic session is single-use and nonreentrant";return false;}
    self.busy=true;struct Guard{bool& b;~Guard(){b=false;}}guard{self.busy};
    if(script<0||static_cast<std::size_t>(script)>=self.runtime->scripts().size()){e="Unknown authored cinematic script";return false;}
    for(const auto& entry:self.runtime->scripts())if(self.runtime->running(entry.id)){e="Cinematic session requires exclusive idle campaign bank";return false;}
    if(!self.runtime->start(script,module,received,e)){self.state=CinematicState::failed;self.publish();return false;}
    self.script=script;self.module=module;
    // Source admission denial is a normal no-op, with no start/finish event.
    if(!self.runtime->running(script))return true;
    self.state=CinematicState::running;self.publish();return true;
}
bool SourceCinematicSession::tick_scripts(std::int32_t dt,std::string& e){
    auto& self=*impl_;if(self.state!=CinematicState::running||self.busy||dt<0){e="Cinematic scripts require running nonreentrant session and nonnegative dt";return false;}
    self.busy=true;struct Guard{bool& b;~Guard(){b=false;}}guard{self.busy};
    if(!self.runtime->tick(dt,e)){self.state=CinematicState::failed;self.publish();return false;}
    bool running=false;for(const auto& entry:self.runtime->scripts())running|=self.runtime->running(entry.id);
    if(!running){self.state=CinematicState::finished;self.publish();}return true;
}
bool SourceCinematicSession::input(CinematicInput input,std::string& e){
    e.clear();
    auto& self=*impl_;if(self.state!=CinematicState::running||self.busy){e="Cinematic input requires idle running session";return false;}
    self.busy=true;struct Guard{bool& b;~Guard(){b=false;}}guard{self.busy};
    if(input==CinematicInput::cancel){
        if(!self.providers.abortWorld||!self.providers.abortWorld(self.script,self.module,e))return missing("actual abort/world restoration service",e);
        self.state=CinematicState::cancelled;self.publish();return true;
    }
    bool actualSkip=self.skip;
    if(!self.providers.input||!self.providers.input(input,actualSkip,e))return missing("actual skip input admission/state",e);
    self.skip=actualSkip;return true;
}
CinematicState SourceCinematicSession::state()const noexcept{return impl_->state;}
bool SourceCinematicSession::skip_active()const noexcept{return impl_->skip;}
}
