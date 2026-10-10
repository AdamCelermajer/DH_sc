"""Guard the production Start Game -> source campaign loading handoff.

This is a source-wiring regression only. It does not execute the Android GL
thread or claim that all source loading stages and gameplay providers work.
"""
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[3]
CPP = ROOT / "port/android-native/app/src/main/cpp"
LOADER = ROOT / "port/level-loader"


def source(path):
    return path.read_text(encoding="utf-8")


class StartGameSourceCampaignHandoff(unittest.TestCase):
    def test_pending_profile_reaches_the_selected_source_campaign_loader(self):
        front = source(CPP / "front_ui_session_v87.cpp")
        callback = front[front.index('if(!std::strcmp(name,"NativeStartGame"))'):
                         front.index('if(!std::strcmp(name,"NativeAssignSaveSlotToPlayer"))')]
        consume = front[front.index("bool FrontUiSessionV87::consume_launch_request("):
                        front.index("bool FrontUiSessionV87::touch(",
                                    front.index("bool FrontUiSessionV87::consume_launch_request("))]

        self.assertIn("swf_front_pending_start_v1", callback)
        self.assertIn("reread_profile_v114", callback)
        self.assertIn("selected_launch_profile_v50=std::move(retained)", callback)
        self.assertIn("self.launch_pending=true", callback)
        self.assertIn("out={impl_->selected_launch_profile_v50->metadata.slot", consume)
        self.assertIn("std::move(impl_->selected_launch_profile_v50)", consume)
        self.assertIn("impl_->launch_pending=false", consume)

    def test_gl_thread_consumes_after_swf_dispatch_and_enters_real_gs_bootstrap(self):
        app = source(CPP / "native_app.cpp")
        dispatch = app[app.index("if(front_ui.active())"):
                       app.index("if(original_ui.active()&&!original_ui.overlays_player())")]
        self.assertLess(dispatch.index("front_ui.render("), dispatch.index("consume_launch_request("))
        self.assertLess(dispatch.index("consume_launch_request("),
                        dispatch.index("native_menu_preview_main_hide_v121("))
        self.assertLess(dispatch.index("native_menu_preview_main_hide_v121("),
                        dispatch.index("start_source_campaign_v55("))
        self.assertIn("request.profile", dispatch)
        self.assertIn("actual GS/C1 loading", dispatch)
        self.assertNotIn("Crypt demo", dispatch)

        runtime = source(CPP / "source_campaign_runtime_v61.cpp")
        start = runtime[runtime.index("bool start_source_campaign_runtime_v61("):
                        runtime.index("bool borrow_source_campaign_candidate_runtime_v61(",
                                      runtime.index("bool start_source_campaign_runtime_v61("))]
        self.assertIn("profile->metadata.location.levels[difficulty]", start)
        self.assertIn("input.request.definition=levels->levels[std::size_t(row)].file", start)
        self.assertIn("NativeRootGSBootstrapInputsV55", start)
        self.assertIn("bootstrap.begin(std::move(input),error)", start)

        bootstrap = source(LOADER / "native_root_gs_bootstrap_v55.hpp")
        self.assertIn("gs_->construct(", bootstrap)
        self.assertIn("NativeRootLoadingConnectionV50::create", bootstrap)

    def test_runtime_start_path_is_not_reported_as_pending_only(self):
        front = source(CPP / "front_ui_session_v87.cpp")
        # The old pending-feedback helper is retained for an older menu event
        # path, but the production NativeStartGame handler never arms it.
        self.assertNotIn("start_feedback_pending=true", front)
        self.assertIn("self.launch_pending=true", front)


if __name__ == "__main__":
    unittest.main()
