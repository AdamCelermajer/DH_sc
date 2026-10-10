import pathlib
import unittest


FEATURE = pathlib.Path(__file__).resolve().parent


class RuntimeSourceMapActorMarkerTest(unittest.TestCase):
    def test_only_source_backed_local_player_family_is_emitted(self):
        source = (FEATURE / "runtime_source_map_actor_markers_v1.cpp").read_text()
        self.assertIn("session.player_id()", source)
        self.assertIn("session.actor_binding_lease().lock()", source)
        self.assertIn("world->traits(player)", source)
        self.assertIn("!traits->is_player", source)
        self.assertIn("world->find_actor(player) != actor", source)
        self.assertIn("actor->transform.position", source)
        self.assertIn("next.family = 3", source)
        self.assertNotIn("definition.name", source)
        self.assertNotIn("profileId", source)

    def test_marker_position_is_fresh_and_lease_checked(self):
        source = (FEATURE / "runtime_source_map_actor_markers_v1.cpp").read_text()
        self.assertIn("const auto serial = session.update_serial()", source)
        self.assertIn("session.update_serial() != serial", source)
        self.assertIn("same_owner(binding, after_binding)", source)
        self.assertIn("finite(position)", source)

    def test_session_collector_composes_only_actual_player_without_duplicates(self):
        header = (FEATURE / "runtime_source_map_actor_markers_v1.hpp").read_text()
        source = (FEATURE / "runtime_source_map_actor_markers_v1.cpp").read_text()
        self.assertIn("source_map_append_current_player_marker_v1", header)
        self.assertIn("source_map_current_player_marker_v1(session, player, error)", source)
        self.assertIn("marker.family == player.family && marker.source_id == player.source_id", source)
        self.assertIn("markers.push_back(std::move(player))", source)
        self.assertIn("generic Session", header)


if __name__ == "__main__":
    unittest.main()
