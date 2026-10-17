"""Emit the anim-ui slice findings CSV (csv.writer, UTF-8, newline='')."""
import csv, os

OUT = (r"C:\Users\nacworkstation\Documents\Projects\Dungeon Hunter 2 Recomp\DH_sc\.worktrees"
       r"\track-b\reports\decomp-reconstruction\deepseek-audit\_parts\findings-anim-ui.csv")

HEADER = ["address", "function", "source_file", "line", "severity", "kind",
          "original_snippet", "reconstruction_snippet", "explanation"]

ROWS = [
    dict(
        address="0x3fd5d0",
        function="SortByEquipability::operator()(ItemInventory::Item const&,ItemInventory::Item const&)",
        source_file="port/decomp-reconstruction/game-ui/src/g1-game-ui-pack4-0efacf-0.cpp",
        line=209,
        severity="medium",
        kind="field",
        original_snippet=("v10 = 1;\n"
                          "if ( !v31 )\n"
                          "  goto LABEL_10;\n"
                          "LABEL_25:\n"
                          "  Constant = PyDataConstants::getConstant(..., \"EquipmentSlots\", \"LeftHand\");\n"
                          "  ... if ( v15 ) return !v22 || *a2 == GetEquippedItem(..., Constant);"),
        reconstruction_snippet=("    if (aInSlots) {\n"
                                "        return !bInSlots || a.m_item == equipped(slot);\n"
                                "    }"),
        explanation=(
            "The final tie-break compares a's item with the wrong slot on the hand and ring paths. In the original each "
            "path re-reads its own slot id: the hand path (m_equipSlot == LeftHand or RightHand) compares against "
            "GetEquippedItem(LeftHand) and the ring path against GetEquippedItem(LeftHandRingFinger) "
            "(recovered/native/hexrays-all/chunk_011.c:11898 and :11937, used at :11925 and :11945), while the "
            "reconstruction compares against equipped(slot), i.e. the m_equipSlot that happens to be current. For "
            "m_equipSlot == RightHand the reconstruction returns \"already equipped\" for an item in the right hand "
            "where the original asks about the left hand, so the inventory sort order of equippable items changes. "
            "severity medium: m_equipSlot is the slot being compared, often LeftHand, and only the tie-break differs."),
    ),
    dict(
        address="0x4761cc",
        function="AnimSetManager::GetSynchronizedAnimator(int)",
        source_file="port/decomp-reconstruction/game-anim/src/g1-game-anim-AnimSetManager-d3d27a-0.cpp",
        line=105,
        severity="low",
        kind="side-effect",
        original_snippet=("v6 = (_BYTE *)v5[8];\n"
                          "if ( v6[112] )\n"
                          "  (*(void (__fastcall **)(_DWORD))(*(_DWORD *)v6 + 56))(v5[8]);"),
        reconstruction_snippet=("    if (set.m_animSet->dirty) {\n"
                                "        set.m_animSet->compile();\n"
                                "    }"),
        explanation=(
            "The dirty-set compile is guarded differently. The original tests the engine-set pointer for null before "
            "reading its dirty byte (+112) and calling compile (recovered/native/hexrays-all/chunk_016.c:366), so an "
            "entry whose m_animSet is null is skipped; the reconstruction dereferences set.m_animSet unconditionally. "
            "Every path that registers a set calls CreateAnimSet (0x47653c, 0x476398), so this is a robustness gap "
            "rather than a reachable crash today: severity low. The same pattern appears in GetAnimator "
            "(chunk_016.c:404) and is not reported twice."),
    ),
]


def main():
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(HEADER)
        for r in ROWS:
            w.writerow([r["address"], r["function"], r["source_file"], r["line"], r["severity"],
                        r["kind"], r["original_snippet"], r["reconstruction_snippet"], r["explanation"]])
    print("wrote", OUT, len(ROWS), "rows")


if __name__ == "__main__":
    main()
