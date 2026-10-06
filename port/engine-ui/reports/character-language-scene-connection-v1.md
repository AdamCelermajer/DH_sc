# Genuine language scene connection

CharacterLanguageSceneConnectionV1 composes the original intrusive character
list and ObjectManager tree traversal with actual World actor predicates and
the existing Gear item identities. It requires the complete real scene graph,
not an empty placeholder or the characters-only ScriptCharacterObjects map.
Node type pointers and type14 localization_valid pointers must alias original
canonical fields; type14 invalidates byte819 exactly. Source virtual
IsGameObject and ItemObject type3 first-inventory-item localization remain
actual caller services. Source ItemObject first-item-null is a genuine no-op.

IsPlayer uses the existing source V6 predicate (actual AI row, type1 or source
type0 PlayerCharacter prefix). IsMerchant uses actual GetCharType==7, including
the same property1 / fallback AI row8 lookup through the verified query kernel.
The same Gear lookup is required; an actual absent Gear for a merchant delegates
to the required real other-inventory provider, never skips it silently.

Gear inventory localization iterates the actual count and item identities,
obtaining its genuine existing text/cache providers through loot_sources_v8.
Item UpdateLocalization calls original name/stat/requirement kernels, snapshots
native power IDs, clears same PowerInfo owner, then AddPower(id,-1) in original
order. The native zero-power branch needs no powered provider. Nonempty powers
require actual same ItemPresentationOwner clear/add callbacks; failure preserves
reached text/power prefixes. No reconstructed generic item text or invented art.

Source language ordering is option write → character inventories → ObjectManager
items/type14 invalidation → StringManager switch_pack(language,true). Preserve
that order. Gear currently owns its own HudText resource instance; do not
pre-switch it before traversal to approximate localization. A faithful shared
StringManager resource binding or exact same-index transport must be supplied
where required. No shared renderer/storage owner was edited by this handoff.

Strict Android compilation passes. Focused item coordinator O1 ASan/UBSan test
passes using actual text kernels and explicitly declared text/power fixtures,
covering zero powers, captured duplicate IDs/source mode-1, missing providers
and failed rebuilt prefix. It does not prove a complete live World traversal.
Actual original components are captured in language-scene-components-v1.asm;
whole source traversal already has settings-language-scene-v1 differential.
