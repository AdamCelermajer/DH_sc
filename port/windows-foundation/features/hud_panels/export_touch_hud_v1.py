"""Original dqhud_droid touch-HUD layout (menu_HUD_2: fixed three skills + faery + attack on the right, joystick left).

Source: port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf (the 854x480 Android HUD; the
480x320 authored stage). Every number is the product of the authored display-list matrices (twips / 20), resolved through
the named placements root menu_HUD_2 -> HUDelements -> ... (see probe_touch_hud_layout_v1.py for the full table of all four
styles). Output: touch_hud_art_v1.cpp (generated, do not hand-edit):
  * per control the authored sprite-local -> stage transform (scale, origin x, origin y) for btn_skill1/2/3, btn_spell,
    btn_interact, btn_potion, btn_itunes, btn_mainmenu, btn_charactermenu and Joystick (the joystick is NOT drawn: seam),
  * the potion counter text slot (HealthBars.btn_potion.cnt.value) and the joystick stick hit zone,
  * the music button art (itunes_controls.btn_itunes, idle frame) in stage space.
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import export_hud_panels_v1 as hp  # noqa: E402

STYLE_ROOT = "menu_HUD_2"
OUTPUT = HERE / "touch_hud_art_v1.cpp"

CONTROLS = {
    "skill1": ["HUDelements", "controls", "controls", "btn_skill1"],
    "skill2": ["HUDelements", "controls", "controls", "btn_skill2"],
    "skill3": ["HUDelements", "controls", "controls", "btn_skill3"],
    "interact": ["HUDelements", "controls", "controls", "btn_interact"],
    "spell": ["HUDelements", "controls", "controls", "btn_spell"],
    "joystick": ["HUDelements", "controls", "controls", "Joystick"],
    "potion": ["HUDelements", "HealthBars", "btn_potion"],
    "itunes": ["HUDelements", "itunes_controls", "btn_itunes"],
    "mainmenu": ["HUDelements", "btn_mainmenu"],
    "charactermenu": ["HUDelements", "btn_charactermenu"],
}


def cpp_num(x: float) -> str:
    return hp.cpp_num(x)


def resolve(swf, names, root=STYLE_ROOT):
    place = swf.named(swf.root, root)
    matrix = hp.compose(hp.IDENTITY, place["matrix"])
    char = place["character"]
    for name in names:
        place = swf.named(swf.sprites[char][0], name)
        matrix = hp.compose(matrix, place["matrix"])
        char = place["character"]
    return char, matrix


def shape_rect(swf, char, matrix):
    code, tag = swf.shapes[char]
    (x0, x1, y0, y1), _ = swf.dec["rect"](tag, 2)
    pts = [(matrix[0] * x + matrix[2] * y + matrix[4], matrix[1] * x + matrix[3] * y + matrix[5])
           for x, y in ((x0, y0), (x1, y0), (x1, y1), (x0, y1))]
    return [min(p[0] for p in pts) / 20., min(p[1] for p in pts) / 20., max(p[0] for p in pts) / 20., max(p[1] for p in pts) / 20.]


def build(swf):
    out = {"controls": {}}
    for key, names in CONTROLS.items():
        char, m = resolve(swf, names)
        if abs(m[1]) > 1e-9 or abs(m[2]) > 1e-9 or abs(m[0] - m[3]) > 1e-3:
            raise ValueError(f"{key}: authored matrix is not a uniform scale+translate")
        out["controls"][key] = {"character": char, "scale": m[0], "x": m[4] / 20., "y": m[5] / 20.}
    # Potion counter: the only EditText of btn_potion (cnt.value).
    pchar, pm = resolve(swf, CONTROLS["potion"])
    slots: list = []
    hp.edit_slots(swf, pchar, 0, pm, slots)
    if len(slots) != 1:
        raise ValueError(f"expected one potion counter text slot, found {len(slots)}")
    out["potion_count"] = slots[0]
    # Joystick: the stick hit zone (HUDelements.controls.controls.Joystick.stick.hitzone) shape rectangle.
    jchar, jm = resolve(swf, CONTROLS["joystick"])
    stick = swf.named(swf.sprites[jchar][0], "stick")
    stick_m = hp.compose(jm, stick["matrix"])
    zone = swf.named(swf.sprites[stick["character"]][0], "hitzone")
    zchar = zone["character"]
    zone_m = hp.compose(stick_m, zone["matrix"])
    # hitzone is an invisible sprite; use the union of its shape rectangles.
    boxes: list = []

    def walk(char, matrix, depth=0):
        if depth > 8:
            return
        if char in swf.shapes:
            boxes.append(shape_rect(swf, char, matrix))
            return
        for _, p in sorted(swf.sprites.get(char, [{}])[0].items()):
            if p.get("character") is not None:
                walk(p["character"], hp.compose(matrix, p["matrix"]), depth + 1)

    walk(zchar, zone_m)
    out["joystick_zone"] = [min(b[0] for b in boxes), min(b[1] for b in boxes), max(b[2] for b in boxes), max(b[3] for b in boxes)]
    # btframe (the potion bottle holder) carries its own offset inside btn_potion; shape 105 (potion icon) is in btframe space.
    frame_place = swf.named(swf.sprites[pchar][0], "btframe")
    out["potion_icon_offset"] = [frame_place["matrix"][4] / 20., frame_place["matrix"][5] / 20.]
    # Music button (btn_itunes idle) in stage space.
    mchar, mm = resolve(swf, CONTROLS["itunes"])
    out["music"] = hp.batches(swf, mchar, swf.frame_of(mchar, "idle"), mm, "music")
    # The HUD_0 action art already generated (hud_panels_art_v1) sits at menu_HUD_0: translation to menu_HUD_2.
    root0 = swf.named(swf.root, hp.ACTION_ROOT)
    hit0 = hp.chain_to(swf, root0["character"], hp.compose(hp.IDENTITY, root0["matrix"]), hp.ACTION_SPRITE)
    ichar, im = resolve(swf, CONTROLS["interact"])
    out["action_translate"] = [(im[4] - hit0[0][4]) / 20., (im[5] - hit0[0][5]) / 20.]
    # The pause button art (pause_ui, generated at menu_HUD_0 btn_mainmenu) moved to its menu_HUD_2 origin.
    _, pause0 = resolve(swf, CONTROLS["mainmenu"], hp.ACTION_ROOT)
    _, pause2 = resolve(swf, CONTROLS["mainmenu"])
    out["pause_translate"] = [(pause2[4] - pause0[4]) / 20., (pause2[5] - pause0[5]) / 20.]
    return out


def write(data):
    c = data["controls"]
    lines = [
        "// Generated from exact dqhud_droid.swf display lists (root menu_HUD_2) by features/hud_panels/export_touch_hud_v1.py;",
        "// do not hand-edit. Stage = the authored 480x320 pixels (twips / 20).",
        '#include "touch_hud_art_v1.hpp"',
        "namespace dh::foundation::hud_panels {",
        "namespace {",
    ]
    lines.append("const std::vector<PcGameplayHudArtBatchV1> music_idle{" + hp.cpp_batches(data["music"]) + "};")
    pc = data["potion_count"]
    lines.append("const TouchHudAuthoredLayoutV1 layout{")
    for key in ("skill1", "skill2", "skill3", "interact", "spell", "potion", "itunes", "mainmenu", "charactermenu", "joystick"):
        v = c[key]
        lines.append(f"  {{{cpp_num(v['scale'])},{cpp_num(v['x'])},{cpp_num(v['y'])}}}, // {key} (sprite {v['character']})")
    r = pc["rect"]
    lines.append(f"  {{{cpp_num(r[0])},{cpp_num(r[1])},{cpp_num(r[2])},{cpp_num(r[3])}}}, {cpp_num(pc['height'])}, // potion counter rect, height")
    z = data["joystick_zone"]
    lines.append(f"  {{{cpp_num(z[0])},{cpp_num(z[1])},{cpp_num(z[2])},{cpp_num(z[3])}}}, // joystick stick hit zone")
    o = data["potion_icon_offset"]
    lines.append(f"  {{{cpp_num(o[0])},{cpp_num(o[1])}}}, // btn_potion.btframe offset (potion icon shape 105 space)")
    t = data["action_translate"]
    lines.append(f"  {{{cpp_num(t[0])},{cpp_num(t[1])}}}, // btn_interact translation menu_HUD_0 -> menu_HUD_2")
    t = data["pause_translate"]
    lines.append(f"  {{{cpp_num(t[0])},{cpp_num(t[1])}}} // btn_mainmenu (pause) translation menu_HUD_0 -> menu_HUD_2")
    lines.append("};")
    lines += [
        "} // namespace",
        "const TouchHudAuthoredLayoutV1& touch_hud_authored_layout_v1() noexcept { return layout; }",
        "const std::vector<PcGameplayHudArtBatchV1>& touch_music_button_v1() noexcept { return music_idle; }",
        "} // namespace dh::foundation::hud_panels",
    ]
    OUTPUT.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main():
    swf = hp.Swf(hp.SOURCE)
    data = build(swf)
    write(data)
    print(json.dumps({"controls": data["controls"], "potion_count": {k: data["potion_count"][k] for k in ("rect", "height", "rgba", "align")},
                      "joystick_zone": data["joystick_zone"], "potion_icon_offset": data["potion_icon_offset"], "action_translate": data["action_translate"], "pause_translate": data["pause_translate"],
                      "music_batches": len(data["music"]), "output": str(OUTPUT)}, indent=1))


if __name__ == "__main__":
    main()
