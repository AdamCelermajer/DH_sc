# Native timer-era state prefix

The approved addition to `character_state_event` clears attack_gate mask1/2/4
for state events0x2a/2b/2c after native input validation and before current
OnEvent/transition handling, matching original RaiseStateEvent0x3c5684.
Character→AI routing and optional AI expiration service occur before this
entry point and remain caller responsibilities. A controller lock suppresses
the AI expiration callback, not the state-event clear.

The previous checkpoint's exact cpp/hpp bytes were preserved at
`.local-inputs/character-timer-discovery/prior-state-source` before editing.
The previous cpp SHA was
`3dd58f40124ceb7123d5e123ebace01ad25bd799eddcdf95b6c0cbef343501d9`;
hpp SHA was
`ab9313e4e675b8ad5944a98544a1f177291e3fc072a94a8e5a205ce9d61c179b`.
Parent separately preserved the audited checkpoint ZIP. Prior state gold and
prior report files remain unchanged.

`tests/character_state.cpp` replays the existing original3910 corpus against
the changed host module: PASS8773 ordered requests,21 malformed atomic cases
and synchronous state reentry under ASan/UBSan. New
`tests/character_state_timers.cpp` replays30 original gate projections and16
original timer-expiry chains under ASan/UBSan. Its AI expiry/routing adapter is
an explicit service fixture; it executes the native timer and state kernels,
not a reconstructed general player AIS. The16 original discovery chains
execute original Character/AI/FSM/AttackOnEvent plus null/default AIS.

New `reports/character-state-timers-host-sanitizers.json` binds both corpora,
source hashes and host executable hashes. The unchanged original state gold
SHA is `9e11a899a29f9682959c00e988ad4fc04ee4a3f610de9044e933d47997d27a57`.
New ARM64/packaged execution must be audited separately before checkpoint
claims include these new source files.
