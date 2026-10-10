"""Check that the focused cancellation adapter is reached by production startup."""
from pathlib import Path
import unittest

CPP = Path(__file__).resolve().parents[1] / "app/src/main/cpp"


def body(source, signature):
    opening = source.index("{", source.index(signature))
    cursor, depth = opening + 1, 1
    while depth:
        depth += (source[cursor] == "{") - (source[cursor] == "}")
        cursor += 1
    return source[opening + 1:cursor - 1]


class CancellationComposition(unittest.TestCase):
    def test_actual_candidate_installs_callback_before_returning_inputs(self):
        source = (CPP / "source_campaign_runtime_v61.cpp").read_text(encoding="utf-8")
        candidate = body(source, "bool connect_source_campaign_candidate_v55(")
        self.assertIn("out.source.external.cancel_and_unload=source_campaign_cancel_unload_v135(weak,", candidate)
        self.assertIn("std::weak_ptr<CanonicalLevelContextV1>(actual)", candidate)
        self.assertIn("source_campaign_cancel_unload_journal_v135(world->release_v88,level,e)", candidate)
        self.assertLess(candidate.index("cancel_and_unload="), candidate.rindex("error.clear();return true"))
        bootstrap = Path(__file__).resolve().parents[2] / "level-loader/native_root_gs_bootstrap_v55.hpp"
        sequence = body(bootstrap.read_text(encoding="utf-8"), "bool begin(")
        self.assertLess(sequence.index("in.connect_candidate("), sequence.index("NativeRootLoadingConnectionV50::create("))

    def test_release_operation_consumes_only_the_existing_checked_journal(self):
        source = (CPP / "source_campaign_release_v88.cpp").read_text(encoding="utf-8")
        operation = body(source, "\nbool source_campaign_cancel_unload_journal_v135(")
        self.assertIn("owner->transport_->current(current,world,e)", operation)
        self.assertIn("same(current.level,level)", operation)
        self.assertIn("world->release_v88!=owner", operation)
        self.assertIn("owner->unload_->execute(level,e)", operation)
        for forbidden in ("make_shared", "destroy_", "s_level", "field130=", "level34.reset"):
            self.assertNotIn(forbidden, operation)

    def test_unload_failure_policy_is_unchanged(self):
        source = (Path(__file__).resolve().parents[2] / "level-loader/level_unload_source_v1.hpp").read_text(encoding="utf-8")
        self.assertIn("if(complete_){e.clear();return true;}if(attempted_)", source)
        self.assertIn('failure_=e.empty()?"Required genuine Level.Unload leaf', source)

    def test_standalone_cancellation_quiesces_before_parser_cleanup_drain(self):
        source = (CPP / "source_campaign_runtime_v61.cpp").read_text(encoding="utf-8")
        tick = body(source, "bool tick_source_campaign_runtime_v61(")
        self.assertLess(tick.index("source_campaign_cancel_admission_v135(state.world,pending,error)"),
                        tick.index("drain_cancel(error)"))
        self.assertIn("if(pending){error.clear();return true;}", tick)
        request = body(source, "bool request_source_campaign_cancel_runtime_v61(")
        self.assertLess(request.index("source_campaign_cancel_admission_v135"), request.index("request_cancel()"))


if __name__ == "__main__":
    unittest.main()
