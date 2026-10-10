"""Static production-composition regressions; no build, APK, or emulator.

The previous implementation had a complete PostInit API with no startup
caller. Check the reachable default-startup chain, not only API presence.
Runtime execution remains part of the integrated campaign milestone.
"""
from pathlib import Path
import unittest


CPP = Path(__file__).resolve().parents[1] / "app/src/main/cpp"
RUNTIME = (CPP / "source_campaign_runtime_v61.cpp").read_text(encoding="utf-8")
ACTOR = (CPP / "source_campaign_script_actor_v96.cpp").read_text(encoding="utf-8")


def body(source, signature):
    start = source.index(signature)
    opening = source.index("{", start)
    depth = 1
    cursor = opening + 1
    while depth:
        depth += (source[cursor] == "{") - (source[cursor] == "}")
        cursor += 1
    return source[opening + 1:cursor - 1]


class Stage26PostInitComposition(unittest.TestCase):
    def test_default_startup_enrolls_before_success(self):
        startup = body(RUNTIME, "bool start_source_campaign_runtime_v61(")
        character = startup.index("!bind_campaign_character_providers_v62(assets,candidate,error)")
        post_init = startup.index("!bind_source_campaign_post_init_native_v134(candidate,error)")
        later = startup.index("!prepare_source_campaign_character_preload_v81(candidate,error)")
        self.assertLess(character, post_init)
        self.assertLess(post_init, later)
        # Enrollment is mandatory in the same failure-propagating startup
        # condition, before any loading tick or startup success can escape.
        self.assertIn("||", startup[character:post_init])
        self.assertLess(post_init, startup.rindex("error.clear();return true"))
        self.assertIn("source_campaign_v55->failed=true", startup[later:])
        self.assertNotIn(".tick(", startup[:post_init])

    def test_enrollment_pins_pm_and_borrows_world(self):
        helper = body(RUNTIME, "bool bind_source_campaign_post_init_native_v134(const SourceCampaignCandidateBorrowV55& candidate")
        self.assertIn("borrow_source_campaign_condition_world_v70(candidate,world,e)", helper)
        self.assertIn("services.provider=world->player_manager", helper)
        self.assertIn("std::weak_ptr<SourceWorldBorrowV61>(world)", helper)
        self.assertIn("source_campaign_character_verify_specialization_v134(current->owner,id,e)", helper)
        self.assertIn("bind_source_campaign_post_init_services_v66(candidate,std::move(services),e)", helper)

    def test_existing_binder_retains_same_owner_and_stage_gate(self):
        binder = body(RUNTIME, "bool bind_source_campaign_post_init_runtime_v66(")
        self.assertIn("pm!=state.world->player_manager", binder)
        self.assertIn("pm.owner_before(state.world->player_manager)", binder)
        self.assertIn("actual.level->constructor_fields_v3().field130>=26", binder)
        self.assertIn("state.world->post_init_characters=std::move(body)", binder)
        services = body(RUNTIME, "bool bind_source_campaign_post_init_services_v66(")
        self.assertIn("source_post_init_characters_v66(*pm,*application,services,e)", services)
        self.assertIn("is_active_selected_v67", services)

    def test_direct_specialization_does_not_replay_reload(self):
        leaf = body(ACTOR, "bool verify_specialization(std::uintptr_t id,")
        self.assertIn("source_native_character_spec_time_v96(actor.actual_world,specialization,e)", leaf)
        self.assertIn("if(level>11)", leaf)
        for class_id in (263, 325, 290):
            self.assertRegex(leaf, rf"class_id\(\).*?=={class_id}")
        for prefix in ("remove_all", "native_reload_skills", "check_item_requirements"):
            self.assertNotIn(prefix, leaf)
        reload = body(ACTOR, "bool reload(std::uintptr_t id,")
        self.assertIn("return verify_specialization(id,e)", reload)

    def test_stage26_runs_pm_between_caches_and_complete_refresh(self):
        stage = body(RUNTIME, "out.source.external.stage_body[26]=")
        self.assertLess(stage.index("refresh_actual_message_caches_stage26_v66"),
                        stage.index("world->post_init_characters(e)"))
        self.assertLess(stage.index("world->post_init_characters(e)"),
                        stage.index("complete_actual_hud_refresh_stage26_v66"))


if __name__ == "__main__":
    unittest.main()
