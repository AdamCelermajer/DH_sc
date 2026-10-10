"""Guard the authored class-confirm callbacks against native wiring drift.

This is source/asset evidence only. It does not execute ActionScript, create a
save, or claim that gameplay loading is complete.
"""
from pathlib import Path
import importlib.util
import unittest


ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"


def source(path):
    return (path).read_text(encoding="utf-8")


class ConfirmSaveDispatchLive(unittest.TestCase):
    def test_authored_confirm_steps_have_native_registration_and_handlers(self):
        # Reuse the repository's SWF test helper and inspect the packaged
        # movie, rather than duplicating the action sequence in this test.
        spec = importlib.util.spec_from_file_location(
            "confirm_action_helper", ROOT / "port/android-native/tests/test_creation_continuation_live_v1.py")
        helper = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(helper)
        movie = ROOT / "port/android-native/app/src/main/assets/front-compat/dqmenus_droid.swf"
        _, actions = helper.movie_actions(movie)
        confirm = [body[1] for key, body in actions.items()
                   if key[:2] == ("root/sprite428", 29)]
        self.assertEqual(len(confirm), 1)
        names = [value["text"] for row in confirm[0]
                 for value in row.get("values", [])
                 if isinstance(value, dict) and value.get("text", "").startswith("Native")]
        self.assertEqual(names, ["NativeCreateSaveSlot", "NativeAssignSaveSlotToPlayer",
                                 "NativePopAllAbove", "NativeStartFromGCInvite",
                                 "NativePushMenu"])

        front = source(CPP / "front_ui_session_v87.cpp")
        menu_registration = source(CPP / "native_process_menu_actions_v119.inc")
        startup = source(CPP / "native_process_startup_v119.inc")
        for name in names:
            self.assertIn('"' + name + '"', menu_registration)
        for name in ("NativeCreateSaveSlot", "NativeAssignSaveSlotToPlayer",
                     "NativeStartFromGCInvite", "NativePushMenu", "NativePopAllAbove"):
            self.assertIn('if(!std::strcmp(name,"' + name + '")', front)
        self.assertIn("source_register_native_actions_v119(process_menu_actions_v119", startup)

    def test_save_assign_and_start_keep_the_same_selected_profile(self):
        front = source(CPP / "front_ui_session_v87.cpp")
        create = front[front.index("bool create_menu_persona("):
                       front.index("static bool menu_slot_exists(", front.index("bool create_menu_persona("))]
        assign = front[front.index('if(!std::strcmp(name,"NativeAssignSaveSlotToPlayer"))'):
                       front.index('if(!std::strcmp(name,"NativeStartFromGCInvite"))')]
        start = front[front.index('if(!std::strcmp(name,"NativeStartGame"))'):
                      front.index('if(!std::strcmp(name,"NativeAssignSaveSlotToPlayer"))')]

        self.assertIn("O_CREAT|O_EXCL|O_NOFOLLOW", create)
        self.assertIn("write(fd", create)
        self.assertIn("change_menu_avatar_preview_v1(menu_avatar", create)
        self.assertIn("change_menu_avatar_preview_v1(menu_avatar,std::int32_t(slot),true,menu_avatar_services,error,true)", create,
                      "newly written profile must pass explicit fresh-slot intent into preview setup")
        self.assertIn("assign_selected_save_slot_v70(local_index,slot", assign)
        self.assertIn("swf_front_assign_save_slot_args_v1(fn,local_index,slot,error)", assign,
                      "production callback must use the separately executable GameSWF ABI decoder")
        abi_source = source(ROOT / "port/engine-ui/swf_menu_save_slots.cpp")
        abi = abi_source[abi_source.index("bool swf_front_assign_save_slot_args_v1("):
                         abi_source.index("bool swf_front_create_save_slot_v1(")]
        self.assertIn("local=fn.arg(1).to_number()", abi)
        self.assertIn("slot=fn.arg(0).to_number()", abi)
        bootstrap = source(ROOT / "port/level-world/application_player_manager_bootstrap_v59.cpp")
        callback_route = bootstrap[bootstrap.index("bool ApplicationPlayerManagerBootstrapV59::assign_selected_save_slot_v70("):
                                  bootstrap.index("bool ApplicationPlayerManagerBootstrapV59::publish_selected_profile_v68(")]
        self.assertLess(callback_route.index("source_first_local_add_prefix"),
                        callback_route.index("publish_selected_save_slot_v67"),
                        "slot assignment must first establish and then use the actual local PlayerInfo")
        self.assertIn("reread_profile_v114", start)
        self.assertIn("selected_launch_profile_v50=std::move(retained)", start)
        self.assertIn("self.launch_pending=true", start)

        # Confirm for a newly named character executes CreateSaveSlot, then
        # AssignSaveSlotToPlayer, then returns through the menu stack. Keep the
        # first-local publication in Assign ahead of that return: a fresh
        # process manager initially contains only its -1 constructor sentinel.
        spec = importlib.util.spec_from_file_location(
            "confirm_action_helper", ROOT / "port/android-native/tests/test_creation_continuation_live_v1.py")
        helper = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(helper)
        movie = ROOT / "port/android-native/app/src/main/assets/front-compat/dqmenus_droid.swf"
        _, actions = helper.movie_actions(movie)
        confirm = [body[1] for key, body in actions.items()
                   if key[:2] == ("root/sprite428", 29)]
        names = [value["text"] for row in confirm[0]
                 for value in row.get("values", [])
                 if isinstance(value, dict) and value.get("text", "").startswith("Native")]
        self.assertLess(names.index("NativeCreateSaveSlot"), names.index("NativeAssignSaveSlotToPlayer"))
        self.assertLess(names.index("NativeAssignSaveSlotToPlayer"), names.index("NativePopAllAbove"))
        self.assertLess(names.index("NativeAssignSaveSlotToPlayer"), names.index("NativePushMenu"))

        # The authored slot-confirm bytecode pushes arguments in source order
        # (local index zero, then SlotID). GameSWF fn_call::arg(0) addresses
        # the top of that stack, so the production binder must reverse the
        # projection: arg(1) is local index, arg(0) is slot. This is the exact
        # distinction that the prior handler-order-only test missed.
        assign_call = next(i for i, row in enumerate(confirm[0])
                           if row.get("op") == "call_func" and i > 0 and
                           any(isinstance(value, dict) and value.get("text") == "NativeAssignSaveSlotToPlayer"
                               for value in confirm[0][i - 1].get("values", [])))
        before_assign = confirm[0][:assign_call - 1]  # omit call_func's argc/name operands
        pushed = [row.get("values", [None])[0] for row in before_assign
                  if row.get("op") == "push_data"]
        authored = []
        for value in pushed:
            if isinstance(value, dict) and value.get("text") == "SlotID":
                authored.append("SlotID")
            elif isinstance(value, (float, int)):
                authored.append(int(value))
        self.assertEqual(authored[-2:], [0, "SlotID"],
                         "authored bytecode must push local index, then returned SlotID")
        # The companion C++ audit builds real vendor gameswf::fn_call objects
        # from this exact source order and exercises production decoding for
        # a newly created slot3 and occupied slot2.
        abi_test = source(ROOT / "port/engine-ui/tests/swf_assign_save_slot_args_v1.cpp")
        self.assertIn("for(const std::int32_t slot:{3,2})", abi_test)
        self.assertIn("env.push(gameswf::as_value(0))", abi_test)
        self.assertIn("env.push(gameswf::as_value(slot))", abi_test)
        self.assertIn("swf_front_assign_save_slot_args_v1(fn,local,selected,error)", abi_test)
        self.assertIn("fn.arg(0).to_number()==slot&&fn.arg(1).to_number()==0", abi_test)

        # AS returns to the native frame owner, which consumes the retained
        # profile and enters the actual selected-row GS/C1 source loader.
        app = source(CPP / "native_app.cpp")
        dispatch = app[app.index("if(front_ui.active())"):
                       app.index("if(original_ui.active()&&!original_ui.overlays_player())")]
        self.assertIn("front_ui.consume_launch_request(request)", dispatch)
        self.assertIn("start_source_campaign_v55(gameplay_assets,request.profile,error)", dispatch)


if __name__ == "__main__":
    unittest.main()
