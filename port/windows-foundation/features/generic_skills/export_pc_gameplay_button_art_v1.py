"""Original dqhud_droid skill/spell/potion button art and cooldown frames (PC HUD, HUDBTN).

Imported by export_pc_gameplay_hud_source_art_v1.py. Source sprites:
  btn_skill 353 (CoolDown child 350 at depth 12, Grey 249, hitzone 352)
  btn_spell 398 (CoolDown 350 at depth 7, Grey 249)
  btn_potion 121 (no CoolDown child)
Art excludes icon holders (btimg/btframe), dynamic glows/counters and hit zones;
the PC renderer draws those separately. Coordinates are sprite-local pixels
(twips / 20). Every batch keeps the exact source shape id and fill kind.
"""
from __future__ import annotations

from pathlib import Path

BUTTON_SPRITES = {"skill": 353, "spell": 398, "potion": 121}
BUTTON_EXCLUDED_NAMES = {"btimg", "btframe", "Grey", "hitzone", "CoolDown", "cnt",
                         "DistressGlow", "warning_glow", "locked", "anim_glow"}
COOLDOWN_SPRITE = 350
COOLDOWN_FRAMES = 100          # frames 0..99 are reachable (source clamps at 99)
GREY_SPRITE = 249
IDENTITY = [1., 0., 0., 1., 0., 0.]


def compose(a, b):
    a0, b0, c0, d0, tx0, ty0 = a
    a1, b1, c1, d1, tx1, ty1 = b
    return [a0*a1 + c0*b1, b0*a1 + d0*b1, a0*c1 + c0*d1, b0*c1 + d0*d1,
            a0*tx1 + c0*ty1 + tx0, b0*tx1 + d0*ty1 + ty0]


def leaves(sprites, shape_tags, char, frame, matrix, mult, path, out, depth=0):
    """Flatten a display list to shape leaves with composed matrix and colour multiplier."""
    if depth > 16:
        raise ValueError("button art nesting bound")
    if char in shape_tags:
        out.append({"shape": char, "matrix": matrix, "mult": mult, "path": path})
        return
    if char not in sprites or not 0 <= frame < len(sprites[char]):
        raise ValueError(f"button art references missing character/frame {char}/{frame}")
    for depth_key, placement in sorted(sprites[char][frame].items()):
        color = placement.get("color", [[1.] * 4, [0.] * 4])
        if color[0][3] == 0 and color[1][3] == 0:
            continue
        if placement.get("clip_depth"):
            raise ValueError(f"button art clip mask at {char}/{depth_key}")
        if placement.get("placeobject3_extra_flags"):
            raise ValueError(f"button art filter/blend at {char}/{depth_key}")
        if any(v != 0 for v in color[1]):
            raise ValueError(f"button art colour add term at {char}/{depth_key}")
        child = placement.get("character")
        if child is None:
            continue
        name = placement.get("name", "")
        child_mult = [m * c for m, c in zip(mult, color[0])]
        leaves(sprites, shape_tags, child, 0, compose(matrix, placement["matrix"]),
               child_mult, path + "/" + (name or "") + ":" + str(child), out, depth + 1)


def placement_matrix(sprites, char, frame, name):
    found = [p for p in sprites[char][frame].values() if p.get("name") == name]
    if len(found) != 1:
        raise ValueError(f"sprite {char} frame {frame} has no unique placement {name}")
    return found[0]["matrix"]


def shape_batches(swf, shape_tags, cache, leaf):
    shape_id = leaf["shape"]
    if shape_id not in cache:
        cache[shape_id] = swf["parse_shape"](*shape_tags[shape_id])
    shape = cache[shape_id]
    m = leaf["matrix"]
    mult = leaf["mult"]
    if shape["triangles"]:
        if shape["solid_triangles"]:
            raise ValueError(f"shape {shape_id} mixes bitmap and solid fills")
        verts = [[(m[0]*x + m[2]*y + m[4]) / 20., (m[1]*x + m[3]*y + m[5]) / 20., u, v]
                 for x, y, u, v in shape["triangles"]]
        return [{"shape": shape_id, "bitmap": True, "rgba": list(mult), "triangles": verts}]
    if shape["solid_triangles"]:
        colours = {tuple(round(c, 6) for c in t[2:6]) for t in shape["solid_triangles"]}
        if len(colours) != 1:
            raise ValueError(f"shape {shape_id} has several solid colours")
        base = next(iter(colours))
        rgba = [b * k for b, k in zip(base, mult)]
        verts = [[(m[0]*x + m[2]*y + m[4]) / 20., (m[1]*x + m[3]*y + m[5]) / 20., 0., 0.]
                 for x, y, *_ in shape["solid_triangles"]]
        return [{"shape": shape_id, "bitmap": False, "rgba": rgba, "triangles": verts}]
    return []


def base_batches(swf, sprites, shape_tags, cache, sprite_id):
    out = []
    for _, placement in sorted(sprites[sprite_id][0].items()):
        name = placement.get("name", "")
        if name in BUTTON_EXCLUDED_NAMES:
            continue
        child = placement.get("character")
        color = placement.get("color", [[1.] * 4, [0.] * 4])
        if child is None or (color[0][3] == 0 and color[1][3] == 0):
            continue
        found = []
        leaves(sprites, shape_tags, child, 0, placement["matrix"], list(color[0]), name, found)
        for leaf in found:
            segments = [s.split(":")[0] for s in leaf["path"].split("/")]
            if any(seg in BUTTON_EXCLUDED_NAMES for seg in segments):
                continue
            out.extend(shape_batches(swf, shape_tags, cache, leaf))
    return out


def write_button_art(swf, sprites, shape_tags, output: Path):
    cache: dict = {}
    base = {kind: base_batches(swf, sprites, shape_tags, cache, sprite)
            for kind, sprite in BUTTON_SPRITES.items()}
    cool_matrix = placement_matrix(sprites, BUTTON_SPRITES["skill"], 0, "CoolDown")
    spell_cool_matrix = placement_matrix(sprites, BUTTON_SPRITES["spell"], 0, "CoolDown")
    grey_matrix = placement_matrix(sprites, BUTTON_SPRITES["skill"], 0, "Grey")
    grey_leaves: list = []
    leaves(sprites, shape_tags, GREY_SPRITE, 0, grey_matrix, [1., 1., 1., 1.], "Grey", grey_leaves)
    grey = [b for leaf in grey_leaves for b in shape_batches(swf, shape_tags, cache, leaf)]
    cooldown: list = []
    for frame in range(COOLDOWN_FRAMES):
        batches: list = []
        if frame:
            found: list = []
            leaves(sprites, shape_tags, COOLDOWN_SPRITE, frame, cool_matrix, [1., 1., 1., 1.], "CoolDown", found)
            for leaf in found:
                batches.extend(shape_batches(swf, shape_tags, cache, leaf))
        cooldown.append(batches)
    spell_cool: list = []
    for frame in range(1, COOLDOWN_FRAMES):
        found = []
        leaves(sprites, shape_tags, COOLDOWN_SPRITE, frame, spell_cool_matrix, [1., 1., 1., 1.], "CoolDown", found)
        spell_cool.append([[leaf["shape"], leaf["matrix"]] for leaf in found])
    if any(len(entry) != 1 for entry in spell_cool):
        raise ValueError("spell CoolDown frame does not resolve to one shape")
    if spell_cool_matrix != cool_matrix:
        raise ValueError("spell CoolDown placement differs from the skill CoolDown placement")

    def num(c):
        text = f"{float(c):.7g}"
        if "." not in text and "e" not in text:
            text += ".0"
        return text + "f"

    def cpp_batches(batches):
        rows = []
        for b in batches:
            tris = ",".join("{" + ",".join(num(c) for c in v) + "}" for v in b["triangles"])
            rows.append("{" + str(b["shape"]) + "," + ("true" if b["bitmap"] else "false") + ",{" +
                        ",".join(num(c) for c in b["rgba"]) + "},{" + tris + "}}")
        return ",".join(rows)

    lines = [
        "// Generated from exact dqhud_droid.swf button sprites by",
        "// features/generic_skills/export_pc_gameplay_button_art_v1.py; do not hand-edit.",
        '#include "pc_gameplay_hud_button_art_v1.hpp"',
        "namespace dh::foundation::generic_skills {",
        "namespace {",
        "const std::vector<PcGameplayHudArtBatchV1> skill_base{" + cpp_batches(base["skill"]) + "};",
        "const std::vector<PcGameplayHudArtBatchV1> spell_base{" + cpp_batches(base["spell"]) + "};",
        "const std::vector<PcGameplayHudArtBatchV1> potion_base{" + cpp_batches(base["potion"]) + "};",
        "const std::vector<PcGameplayHudArtBatchV1> grey_overlay{" + cpp_batches(grey) + "};",
    ]
    for frame, batches in enumerate(cooldown):
        lines.append(f"const std::vector<PcGameplayHudArtBatchV1> cooldown_{frame}{{" + cpp_batches(batches) + "};")
    lines += [
        "const std::array<const std::vector<PcGameplayHudArtBatchV1>*, " + str(COOLDOWN_FRAMES) + "> cooldown_frames{{" +
        ",".join(f"&cooldown_{i}" for i in range(COOLDOWN_FRAMES)) + "}};",
        "} // namespace",
        "const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_skill_base_v1() noexcept { return skill_base; }",
        "const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_spell_base_v1() noexcept { return spell_base; }",
        "const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_potion_base_v1() noexcept { return potion_base; }",
        "const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_grey_overlay_v1() noexcept { return grey_overlay; }",
        "const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_cooldown_frame_v1(std::size_t frame) noexcept { return *cooldown_frames[frame]; }",
        "} // namespace dh::foundation::generic_skills",
    ]
    output.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return {
        "skill_base_batches": len(base["skill"]),
        "spell_base_batches": len(base["spell"]),
        "potion_base_batches": len(base["potion"]),
        "grey_batches": len(grey),
        "cooldown_frames": COOLDOWN_FRAMES,
        "cooldown_nonempty_frames": sum(1 for b in cooldown if b),
        "cooldown_shape_ids": [b[0]["shape"] if b else None for b in cooldown[:4]],
    }
