`dh::foundation::frontend::creation::class_choices()` returns the original menu
order: Knight, Rogue, Mage, with exact profile, title and description tokens.
`initial_class_token` is `KnightPlayerBase`. Numeric table rows are cache-bound
metadata, not runtime character objects.

`stage_creation(CreationRequest, CompleteCreationService)` returns a staged owned
`optional<dh::foundation::CharacterState>`. The adapter checks request identity,
source player level 1 and the existing shared state's validation constraints.
It never calls a save store, SG_Save, SG_Delete or a filesystem API. The caller
may subsequently apply an explicitly accepted staged value to its normal state
owner. The initial campaign level row 41 is metadata; it is not an invented
`CharacterState::unlocks` entry.

Without a complete initialization service, creation explicitly returns
`unavailable_service` with no staged character. The service must supply resolved
properties and actual grants/effects, skill state and campaign unlock mapping.
The existing `make_default_character` function labels its numbers as foundation
fixtures and cannot fulfill this contract. Neither the selected skill dictionary
row nor the class FaeryList constitutes a proof of a shared-model unlock ID.

Add `creation_adapter.cpp` to the Windows frontend target and link the existing
`character_state.cpp` owner. The standalone test links these two translation
units with `creation_adapter_tests.cpp`. Test-provider values are explicitly
arbitrary interface fixtures; successful tests do not certify full game creation.

Evidence: exported IDA `MenuCharacterSelect::Init` 0x428cc0, `Update` 0x428498,
`MenuBase::FS_StartGame` 0x4220a0, plus executed original class/cache and fresh
profile captures. Live MCP access on 127.0.0.1:8746 was attempted and refused;
the complete static IDA export was read. See `source_evidence.json` for hashes.

Dynamic text: link `dynamic_text_bindings.cpp` and use
`class_text_bindings(token)`. The returned exact source values include `Warrior`,
`Rogue`, `Mage`, descriptions, `Confirm` and `Choose a class`. Raw caret markup
is retained alongside HTML; specialization spans use actual FontTextColors.two
`#9ADEFF`. Match each `field_path` against the original `art::TextField`, retain
that receiver's font/color/layout, and render `html_text` with its color spans.
The native `class_title/text` receiver must be recovered from the movie's active
frame: it is absent from the initial-frame static art export.

`saved_profile_text_bindings(state, service)` receives the SAME shared saved
state by const reference. Name and level are read from that object. The mandatory
service projects class StrID, displayed act, location, difficulty and local save
date from the matching campaign profile. It must match the state identity and
class token. Specialized `RoguePlayerBase_Archer` is valid saved state; it is not
offered as a fresh base class. The reference screenshot's David/level22/Act7 are
illustrative and are never introduced as defaults. Main-menu fields publish
atomically only after the profile service succeeds.

The existing `fresh_player_profile_v1` produces original profile metadata only,
not initialized gameplay stats/inventory. `fresh_inventory_v2` still requires
actual item stats/name/requirements/native effects, and the initial grant owner
requires skill/property/update services. Frozen snapshots and grant-caller proofs
do not supply the missing complete shared-model stats/effects/unlock projection.
Fresh creation therefore keeps its explicit missing-service failure.

`SelectedProfileBindingV1` prepares the real selected-file inputs for the
existing `CharacterProfileBootstrapV59`. Call it only after the actual
Character Save C1 and same-Save `SetSlot` route have produced
`CharacterProfileSlotStoreReceiptV59`. Supply the existing
`ApplicationSaveFilesOwnerV61`, selected slot, same Save/Load/source-cell
aliases, direct Save slot field (no PlayerInfo proxy), actual setter callback,
and genuine reader/tail providers. The binding reads source
`dh2_NNN.savegame` through `read_save` (matching-job flush included), constructs
`CampaignSaveProfileV45` from that exact owner's `files()->services()` and
`jobs()`, then rejects unless the immutable profile cache equals the retained
read receipt. Its `bootstrap_inputs()` is passed into the existing
`CharacterProfileBootstrapV59`, which continues to own Load1/2/4 ordering.
The helper does not create the indexed file, allocate Save/PlayerInfo, or claim
profile/bootstrap readiness until the caller prepares that existing owner. The
Windows test seeds a profile with the exact metadata helper and exercises the
same FileManager/jobs/cache path; it does not stand in for a live
`NativeCreateSaveSlot` or completed InitPost.

`source_font_colors()` exposes actual FontTextColors.zero..nine RGB values,
including one `#9CFF9A`, two `#9ADEFF` and three `#D49AFF`. These are decoded
directly from `fonts_pycst.bin` by the evidence collector. `source_localized_html`
preserves trusted authored HTML and expands source caret color controls to FONT
tags. Reset `^r` closes a FONT and restores the enclosing authored style; it is
not a fixed white reset. The helper also follows source `^n`, pipe separator,
unknown-control and preserved format-directive handling. This bounded helper
does not run varargs substitution or the language-pack spacing pass. The host
must parse paragraph/BR/FONT tags case-insensitively and maintain nested colors
before wrapping text; source field font, color and line metrics remain active.
