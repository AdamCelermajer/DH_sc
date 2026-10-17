# Infected Village actor asset closure

The four Ambush Character records in `infected01.mgp` (records 6, 7, 8, and
19) share `Charater_Templates[68]`, named
`InfectedVillage_CommonType1`. That template offers six CharacterTable model
choices. The cache-only manifest records their BDAE model paths, the positive
`Infected` CharAnim state-to-AnimTpl links, all referenced AnimDict paths, and
the four model texture paths. It lists 30 unique files totaling 2,371,288
bytes; it contains metadata and hashes only, with no copied game payloads.

## Verify against the extracted cache

From the repository root:

```powershell
python port/level-runtime/tests/verify_infected_village_actor_assets.py --cache-root ../cache/files
```

The verifier recomputes every listed file's size and SHA-256, confirms the four
MGP records still match the cached XML, and checks that template choices,
AnimTpl references, animation paths, model textures, roles, and closure counts
link consistently. PyData table IDs and decoded animation mappings in the
manifest are traced source metadata; this verifier does not independently
decode PyData tables. It fails on missing files, size/hash drift, unsafe paths,
or a broken manifest link.

## Unresolved source questions

- **Initial Limbus animation and transitions:** all four MGP records author
  `ai_state=Limbus`, while the `Infected` CharAnim slot for Limbus is `-1`.
  The initial pose, fallback, and state transitions are unknown; selecting Idle
  or another clip would be an unsupported guess.
- **Cutscene clip skeleton compatibility:** the Attack AnimTpl references
  faery and prince Madruk cutscene BDAEs; Stunned references an NPC Madruk
  talk BDAE between infected clips. Those source paths are preserved, but
  compatibility with each infected model's skeleton has not been established.
- **GL effect resolver:** the models reference
  `GL_Diffuse_L1_VC_iPhone.bdae`, and the cache has two same-basename
  candidates: `data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae` (18,792 bytes,
  SHA-256 `10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc`)
  and the cache-root file `gl_diffuse_l1_vc_iphone.bdae` (18,792 bytes,
  SHA-256 `af9518292b54a0bcd78db7682ecd7cfbd48cb50347dd495718446f42166a9d2d`).
  The resolver's search order is unknown, so neither candidate is selected or
  counted in the 30-file closure.

This is an authored-data and cache-integrity audit, not proof that the six
models or clips have been instantiated or played by the Android runtime.
