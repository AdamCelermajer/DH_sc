    static bool source_controls_event_v62(void* raw,const events::EventBorrowV12& event,events::EventManagerOwnerV12&,std::int32_t& answer,std::string& e){
        auto& self=*static_cast<Impl*>(raw);answer=0;
        ui::HudManagerRequest request{};request.operation=ui::HudManagerOperation::saved_option;request.text="DPad";ui::HudManagerResponse result{};
        if(!self.source_hud_world_v62||self.source_hud_world_v62(request,result,e)!=1)return false;
        if(result.value||self.attack_controls_v46.held9||self.attack_controls_v46.consumed84){e.clear();return true;}
        std::int32_t type{};if(!event.get_type||!event.get_type(event.context,type,e))return false;
        std::int16_t x{},y{};std::uint8_t active{};
        auto pointer_fields=[&](){
            if(event.input_v120){events::InputEventProjectionV120 p;if(!event.input_v120(event.context,p,e))return false;x=p.x8;y=p.y_a;active=p.pressed10;return true;}
            return self.source_controls_v62.pointer_event_fields&&self.source_controls_v62.pointer_event_fields(event,x,y,active,e);
        };
        if(type==4){
            if(!pointer_fields()){if(e.empty())e="Required actual HUD event4 fields8/A/10";return false;}
            self.attack_controls_v46.pending_x7c=active?x:-1;self.attack_controls_v46.pending_y80=active?y:-1;e.clear();return true;
        }
        //Original reloads GetType on the non4 branch.
        if(!event.get_type(event.context,type,e))return false;
        if(type==5){
            if(!pointer_fields()){if(e.empty())e="Required actual HUD event5 fields8/A";return false;}
            self.attack_controls_v46.pending_x7c=x;self.attack_controls_v46.pending_y80=y;
        }
        e.clear();return true;
    }
    bool construct_source_controls_v108(std::shared_ptr<events::EventManagerOwnerV12> actual,std::string& e){
        if(!actual){e="HUDControls C1 requires actual Application.events14";return false;}
        if(source_controls_constructed_v62){if(source_controls_events_v62.lock()!=actual){e="HUDControls belongs to another EventManager";return false;}e.clear();return true;}
        if(source_controls_constructor_attempted_v62){e="HUDControls retained partial source registration; cannot replay";return false;}
        source_controls_constructor_attempted_v62=true;source_controls_events_v62=actual;
        events::EventReceiverV12 receiver;receiver.identity=reinterpret_cast<std::uintptr_t>(&attack_controls_v46);receiver.context=this;receiver.lifetime=shared_from_this();receiver.on_event=source_controls_event_v62;
        bool inserted{};
        if(!actual->attach(4,receiver,0,inserted,e)||!inserted){if(e.empty())e="HUDControls event4 already registered";return false;}
        if(!actual->attach(5,receiver,0,inserted,e)||!inserted){if(e.empty())e="HUDControls event5 already registered";return false;}
        source_controls_constructed_v62=true;e.clear();return true;
    }
    static bool source_hud_notify_v62(void* raw,gameswf::sprite_instance* sprite,std::string& e){return static_cast<Impl*>(raw)->source_hud_advance_v62.notify(sprite,e);}
    static bool source_hud_sound_v62(void*,std::uintptr_t& out,std::string&){out=reinterpret_cast<std::uintptr_t>(gameswf::get_sound_handler());return true;}
    static bool source_hud_pause_v62(void*,std::uintptr_t id,std::int32_t sound,bool paused,std::string& e){
        auto* handler=gameswf::get_sound_handler();if(!handler||id!=reinterpret_cast<std::uintptr_t>(handler)){e="InfoHUD source sound receiver changed";return false;}
        handler->pause(sound,paused);return true;
    }
    static int source_hud_text_v62(void* raw,const ui::HudManagerRequest& q,ui::HudManagerResponse&,std::string& e){
        auto& self=*static_cast<Impl*>(raw);if(!self.source_hud_graph_v62){e="InfoHUD text requires its SAME live movie scope";return 0;}
        ui::SwfAsValue receiver;
        return self.source_hud_graph_v62->retain_object(reinterpret_cast<gameswf::as_object*>(q.subject),receiver,e)&&
            ui::renderfx_set_plain_text(*self.source_hud_graph_v62,receiver,q.text,q.value!=0,e)?1:0;
    }
    static int source_hud_dispatch_v62(void* raw,ui::HudManagerState*,const ui::HudManagerRequest* q,ui::HudManagerResponse* out){
        auto& self=*static_cast<Impl*>(raw);
        if(!q||!out||!self.source_hud_error_v62)return 0;auto& e=*self.source_hud_error_v62;
        try{
            using Op=ui::HudManagerOperation;
            if(q->operation==Op::current_level_flush){
                loader::CanonicalCurrentLevelBorrowV1 actual;
                if(!model_renderer::borrow_current_native_level_v27(actual,e))return 0;
                out->identity=actual.identity();out->value=actual?actual.level()->constructor_fields_v3().byte144:0;
                e.clear();return 1;
            }
            if(q->operation==Op::cached_target_store){
                if(!q->subject){self.source_cached_target_lifetime_v107.reset();self.source_cached_target_projection_v107=0;self.source_cached_target_produced_v107=true;e.clear();return 1;}
                auto world=self.source_campaign_world_v104.lock();
                if(!world){ //Legacy Crypt projection has no canonical actor lifetime authority.
                    self.source_cached_target_lifetime_v107.reset();self.source_cached_target_projection_v107=q->subject;self.source_cached_target_produced_v107=false;e.clear();return 1;
                }
                const auto* actor=reinterpret_cast<const ui::HudManagerActor*>(q->subject);
                std::weak_ptr<void> lifetime;
                if(!actor->identity||!model_renderer::borrow_source_campaign_character_lifetime_v107(world,actor->identity,lifetime,e))return 0;
                self.source_cached_target_lifetime_v107=std::move(lifetime);self.source_cached_target_projection_v107=q->subject;self.source_cached_target_produced_v107=true;e.clear();return 1;
            }
            if(q->operation==Op::cached_target_alive){
                if(!self.source_cached_target_produced_v107||self.source_cached_target_projection_v107!=q->subject){e="InfoHUD Flush requires actual cached-target weak lifetime receipt";return 0;}
                out->value=!self.source_cached_target_lifetime_v107.expired();e.clear();return 1;
            }
            if(self.source_hud_graph_v62&&self.source_hud_core_v62){
                const auto core=self.source_hud_core_v62->dispatch(*q,*out,e);if(core>=0)return core;
            }
            if(!self.source_hud_transport_v62||!self.source_hud_world_v62){e="Required actual InfoHUD world operation "+std::to_string(static_cast<unsigned>(q->operation));return 0;}
            return self.source_hud_world_v62(*q,*out,e);
        }catch(const std::exception& ex){e=ex.what();return 0;}catch(...){e="Actual InfoHUD provider threw";return 0;}
    }
    bool source_info_run_v62(ui::HudManagerEntry entry,std::string& e){
        auto invoke=[&](ui::SwfAsGraph* graph){
            const auto old_graph=source_hud_graph_v62;auto* old_error=source_hud_error_v62;
            struct Scope{Impl& self;ui::SwfAsGraph* graph;std::string* error;~Scope(){self.source_hud_graph_v62=graph;self.source_hud_error_v62=error;}} scope{*this,old_graph,old_error};
            source_hud_graph_v62=graph;source_hud_error_v62=&e;
            const ui::HudManagerServices services{this,source_hud_dispatch_v62};
            return source_info_hud_v62.run(entry,services,e);
        };
        if(!source_info_hud_v62.fields().render_fx)return invoke(nullptr);
        if(!movie||reinterpret_cast<std::uintptr_t>(movie.get())!=source_info_hud_v62.fields().render_fx){e="InfoHUD57c lost its original LoadMenu3 movie generation";return false;}
        struct Run{Impl& self;decltype(invoke)& fn;
            static bool apply(void* raw,ui::SwfAsGraph& graph,std::string& e){auto& q=*static_cast<Run*>(raw);ui::SwfAsValue root;gameswf::as_object* object{};
                if(!graph.root_value(root,e)||!graph.borrow_object(root,object,e)||!object||!object->is(gameswf::character::m_class_id)){if(e.empty())e="InfoHUD actual root character unavailable";return false;}
                if(!q.self.source_hud_core_v62)q.self.source_hud_core_v62=std::make_unique<ui::HudManagerCore>();
                const ui::HudSpriteCoreServices services{&q.self,source_hud_notify_v62,source_hud_sound_v62,source_hud_pause_v62};
                if(!q.self.source_hud_core_v62->bind(static_cast<gameswf::character*>(object),hud_sha,services,e))return false;
                q.self.source_hud_core_v62->required_operations(&q.self,source_hud_text_v62);
                return q.fn(&graph);
            }} run{*this,invoke};
        return movie->menu_action_script(&run,Run::apply,e);
    }
