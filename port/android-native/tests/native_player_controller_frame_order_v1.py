"""Keep native HUD controller commands after the source Character frame turn."""
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
SOURCE = ROOT / "port/android-native/app/src/main/cpp/model_renderer.cpp"


def main() -> None:
    text = SOURCE.read_text(encoding="utf-8")
    start = text.index("void advance_native_actor(unsigned dt_ms){")
    end = text.index("\nint native_frame_actors_v1(", start)
    frame = text[start:end]
    ordered = (
        "prince_character.update_state(dt_ms)",
        "prince_locomotion.animator_phase(",
        "dh2::actor::update_actor(result,request,error)",
        "advance_native_char_ai_queue(dt_ms)",
        "dh2::native::player_input_controller_v1::dispatch(",
        "prince_event(0x1c,0)",
    )
    positions = [frame.index(token) for token in ordered]
    if positions != sorted(positions):
        raise AssertionError(f"source frame order mismatch: {dict(zip(ordered, positions))}")
    print("PASS native_player_controller_frame_order_v1: Character/actor turn precedes HUD command")


if __name__ == "__main__":
    main()
