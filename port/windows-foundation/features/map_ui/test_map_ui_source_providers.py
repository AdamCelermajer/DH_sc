#!/usr/bin/env python3
"""Source-backed map provider checks; no generated map or marker fixtures."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[4]
IDA = ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0045"
FEATURE = ROOT / "port/windows-foundation/features/map_ui"


class MapSourceProviderTest(unittest.TestCase):
    def test_room_zone_adapter_uses_canonical_source_fields(self):
        header = (FEATURE / "source_map_room_zone_v1.hpp").read_text()
        source = (FEATURE / "source_map_room_zone_v1.cpp").read_text()
        self.assertIn("CanonicalRoomZoneRecordV3", header)
        self.assertIn("constructor_completed_v91", source)
        self.assertIn("source_has_been_visited_v104", source)
        self.assertIn("source_has_inside_v104", source)
        self.assertNotIn("ModulePFRoomsV3", source)

    def test_marker_family_producers_match_original_native_source(self):
        producers = {
            "004548ac.c": [("DuplicateIcon", ", 0,")],
            "00454a08.c": [("DuplicateIcon", ", 3,"), ("DuplicateIcon", ", 7,")],
            "00454fb8.c": [("DuplicateIcon", ", 4,"), ("DuplicateIcon", ", 10,"), ("DuplicateIcon", ", 11,")],
            "00454d5c.c": [("DuplicateIcon", ", 1,"), ("DuplicateIcon", ", 2,"), ("DuplicateIcon", ", 12,")],
            "00454b8c.c": [("DuplicateIcon", "v6,"), ("IsInsideRooms", "i, v12, 1")],
        }
        for path, needles in producers.items():
            with self.subTest(source=path):
                body = (IDA / path).read_text()
                for function, fragment in needles:
                    self.assertIn(function, body)
                    self.assertIn(fragment, body)

    def test_renderer_has_eighteen_source_icon_families(self):
        body = (IDA / "00454030.c").read_text()
        self.assertIn("i != 18", body)
        self.assertIn("RenderIconType", body)
        self.assertIn("i != 7 && i != 8 && i != 9", body)

    def test_screen_projection_matches_camera_base_matrix_calls(self):
        source = (FEATURE / "source_map_kernel_v1.cpp").read_text()
        body = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040f714.c").read_text()
        self.assertIn("source_camera_base_screen_coord_v1", source)
        self.assertIn("matrices(0, view", source)
        self.assertIn("matrices(2, projection", source)
        self.assertIn("setbyproduct_nocheck", body)
        self.assertIn("multiplyWith1x4Matrix", body)
        self.assertIn("1.0 / v14", body)

    def test_map_camera_configuration_replays_actual_set_data_order(self):
        source = (FEATURE / "source_map_camera_v1.cpp").read_text()
        runtime = (ROOT / "port/level-world/gameplay_camera_runtime_v11.cpp").read_text()
        set_data = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040e9a8.c").read_text()
        create = (IDA / "0045351c.c").read_text()
        calls = ["source_map_fields_v1", "enable_damping", "source_set_data_v1",
                 "source_set_camera_vector_v1"]
        start = source.index("bool source_map_camera_configure_v1")
        at = [source.index(call, start) for call in calls]
        self.assertEqual(at, sorted(at))
        setters = runtime[runtime.index("GameplayCameraRuntimeV11::source_map_fields_v1"):]
        setter_calls = ["level_->fields().mode85=byte133!=0;",
                        "level_->fields().automatic_zoom8c=float140;",
                        "level_->fields().requested_zoom88=source_float(word136);",
                        "view_.fov=horizontal_fov_or_mag;view_.aspect=aspect;", "set_clip_start(near_plane",
                        "set_clip_end(far_plane", "source_set_camera_vector_v1({0.0f,0.0f,1.0f}"]
        setter_at = [setters.index(call) for call in setter_calls]
        self.assertEqual(setter_at, sorted(setter_at))
        self.assertIn("+ 316", set_data)
        self.assertIn("+ 312", set_data)
        self.assertIn("+ 304", set_data)
        self.assertIn("+ 308", set_data)
        level_update = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0041/00410390.c").read_text()
        handle_zoom = (ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0041/00410218.c").read_text()
        self.assertIn("*((_BYTE *)this + 133)", level_update)
        self.assertIn("*((float *)this + 34)", handle_zoom)
        self.assertIn("*((float *)this + 35)", handle_zoom)
        set_data_call = create.index("CameraBase::SetData")
        vector_override = create.index("v12[0] = -1082130432", set_data_call)
        self.assertLess(set_data_call, vector_override)
        self.assertIn("v12[1] = 1065353216", create[vector_override:])
        self.assertIn("v12[2] = 0", create[vector_override:])

    def test_runtime_projection_uses_same_camera_view_and_active_frame(self):
        source = (FEATURE / "source_map_camera_v1.cpp").read_text()
        self.assertIn("source_map_camera_frame_current_v1(frame, error)", source)
        self.assertIn("camera->view(view, error)", source)
        self.assertIn("view.view.values", source)
        self.assertIn("view.projection.values", source)
        self.assertIn("camera.get()", source)
        self.assertIn("source_map_camera_frame_current_v1(frame, error)) return false", source)


if __name__ == "__main__":
    unittest.main()
