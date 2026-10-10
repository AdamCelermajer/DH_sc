"""Guard the existing source-backed touch-to-player controller connection.

These checks verify production source wiring only. They do not execute the
Android input loop, controller virtuals, or Swamp navigation at runtime.
"""
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
LEVEL = ROOT / "port/level-world"


def read(path):
    return path.read_text(encoding="utf-8")


class CampaignCharacterTouchControllerV107(unittest.TestCase):
    def test_press_release_reach_the_current_selected_player_character(self):
        renderer = read(CPP / "model_renderer.cpp")
        bind = renderer[renderer.index("void PlayerSkillsRuntime::bind_world_touch"):
                        renderer.index("void initialize_player_skills", renderer.index(
                            "void PlayerSkillsRuntime::bind_world_touch"))]
        self.assertIn("borrow_source_campaign_candidate_v55(candidate,error)", bind)
        self.assertIn("borrow_source_campaign_player_gameplay_v67(candidate.actual_world,player,error)", bind)
        self.assertIn("!player.active||!player.character", bind)
        self.assertIn("point.data(),released,error", bind)
        self.assertIn("source_campaign_character_click_point_v120", bind)

        touch = read(LEVEL / "renderer_world_touch_live_v2.inc")
        self.assertIn("click.move=[&](const std::array<float,3>& p,bool released", touch)
        self.assertIn("invoke_source_click_move_v20(p,released,e)", touch)
        # The retained world-touch actor cursor walks the Character registry;
        # generic GameObjects are supplied only as the source's empty fallback.
        self.assertIn("click.characters=[&]", touch)
        self.assertIn("click.objects=[]", touch)

    def test_stage24_loaded_player_binding_resolves_the_actual_local_character(self):
        lookup = read(CPP / "renderer_character_campaign_lookup_v62.inc")
        pre_begin = lookup.index("bool borrow_source_campaign_player_pre_gameplay_v68(")
        game_begin = lookup.index("bool borrow_source_campaign_player_gameplay_v67(", pre_begin)
        lifetime_begin = lookup.index("bool borrow_source_campaign_character_lifetime_v107(", game_begin)
        pregame = lookup[pre_begin:game_begin]
        gameplay = lookup[game_begin:lifetime_begin]

        # Resolve PlayerInfo through the current source Application/PlayerManager,
        # then borrow that exact Character660 from the same campaign World.
        self.assertIn("candidate.actual_world!=world", pregame)
        self.assertIn("candidate.application->source_player_manager_v59()", pregame)
        self.assertIn("get_local_player(0,true,local", pregame)
        self.assertIn("local->character660", pregame)
        self.assertIn("borrow_source_campaign_character_v62(world,local->character660,actual,e)", pregame)
        self.assertIn("r.is_player(player,e)||!player", pregame)
        self.assertIn("binding.character=r.actor->object->identity", pregame)
        self.assertIn("binding.world_owner=world", pregame)

        # Gameplay binding stays inactive before the complete stage-38 tail;
        # after load, require the same candidate, a positive AddCharacter count,
        # and only then expose the exact selected PlayerInfo Character660.
        self.assertIn("binding.active=false", pregame)
        self.assertIn("source_campaign_loading_complete_v64()", gameplay)
        self.assertIn("candidate.actual_world!=world", gameplay)
        self.assertIn("*manager->count_field()<=0", gameplay)
        self.assertIn("binding.active=true", gameplay)

        runtime = read(CPP / "source_campaign_runtime_v61.cpp")
        complete_begin = runtime.index("bool source_campaign_loading_complete_runtime_v64(){")
        complete_end = runtime.index("bool capture_source_campaign_modules_runtime_v64(", complete_begin)
        complete = runtime[complete_begin:complete_end]
        self.assertIn("source_campaign_v55->final_tail_complete", complete)
        self.assertIn("constructor_fields_v3().field130==38", complete)
        self.assertIn("source_campaign_v55->globals->s_level==gs->fields().level34", complete)

    def test_press_uses_canonical_destination_and_release_uses_that_point(self):
        fsm = read(CPP / "source_campaign_character_fsm_v101.cpp")
        begin = fsm.index("bool click_point_v120(const float* point,bool released)")
        end = fsm.index(" int timer(", begin)
        body = fsm[begin:end]
        # Original Ctrl_Click3addc8 dispatches press to Character::Ctrl_HeadTo
        # (3adac8, vtable +e4) and release to Ctrl_MoveTo (3ada30, +ec).
        # HeadTo's IDA body is IsRemotelyUpdated -> build point-origin vector
        # -> SetHeadingDirection(vector, true) -> SetDestination(point) ->
        # RaiseEvent(0, nullptr). Guard the same order in the production route.
        press_order = [
            "if(released)return move(point,false)",
            "if(!remote(remotely_updated))",
            "const float direction[3]{point[0]-origin[0],point[1]-origin[1],point[2]-origin[2]}",
            "navigation::set_heading_unchecked(actor.runtime.controller.heading,direction,1)",
            "actor.runtime.rotation.heading_angle=actor.runtime.controller.heading.angle",
            "state().heading_active=actor.runtime.controller.heading.active",
            "std::copy_n(point,3,actor.runtime.controller.destination)",
            "return raise(0,0)",
        ]
        locations = [body.index(item) for item in press_order]
        self.assertEqual(locations, sorted(locations))

        # The Point overload checks the same virtual IsRemotelyUpdated first,
        # then invokes GameObject::PathTo with the release point. Ctrl_Click's
        # outer gates are handled before this continuation, so no extra Cmd_*
        # gate is introduced here.
        move_begin = fsm.index(" bool move(const float* point,bool command_gate=true)")
        move_end = fsm.index(" bool click_point_v120(", move_begin)
        move = fsm[move_begin:move_end]
        self.assertLess(move.index("if(command_gate)"), move.index("bool remotely_updated"))
        self.assertLess(move.index("if(!remote(remotely_updated))"), move.index("PathToState40 value"))
        self.assertIn("if(released)return move(point,false)", body)
        self.assertIn("std::copy_n(point,3,actor.runtime.controller.destination)", body)
        self.assertIn("return raise(0,0)", body)

        # IDA's original Character::Ctrl_Click dispatches its press/release
        # virtuals at Character vtable offsets +0xe4 and +0xec respectively.
        original = read(LEVEL / "reference/world-touch-target-v1/original-source.asm")
        self.assertIn("003ae288: ldr pc, [r3, #0xe4]", original)
        self.assertIn("003ae43c: ldr pc, [r3, #0xec]", original)

    def test_cancellation_clears_pending_touch_without_synthesizing_release(self):
        session = read(CPP / "original_ui_session.cpp")
        cancel = session[session.index("bool cancel_gameplay_pointers_v124"):
                         session.index("bool dispatch_attack_v46", session.index(
                             "bool cancel_gameplay_pointers_v124"))]
        self.assertIn("world_pointers.clear()", cancel)
        self.assertIn("model_renderer::world_touch_cancel()", cancel)
        self.assertNotIn("world_touch(", cancel)

        renderer = read(CPP / "model_renderer.cpp")
        begin = renderer.index("void world_touch_cancel()")
        end = renderer.index("namespace {", begin)
        cancel = renderer[begin:end]
        self.assertIn("pending.pending14c8=0", cancel)
        self.assertIn("pending.skill14ca=-1", cancel)
        self.assertNotIn("released", cancel)


if __name__ == "__main__":
    unittest.main()
