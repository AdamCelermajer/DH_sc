Source-only completion of V95 LightPoint changed-enable delivery. No tests/build/compiler/hash/runtime jobs.

Apply light-enable-v96-integration.patch after the V95 loading-field addition, surgically preserving current V92 Light resource/driver/teardown and Main visitor hunks. Two canonical_light_point_v53 HPP/CPP source deltas only, no new TU, registry, provider, D0 or barrier. Successor copies are context; do not overwrite newer shared Root files.

Recovered native vtable: LightBase9679c0 and LightPoint967a58 have virtual3c=ObjectBase.setUpdating33dcf0,40=ObjectBase.SetVisible33dcf8,44=ObjectBase.Enabled33de04,48=ObjectBase.Disabled33de34. LightBase does NOT override SetVisible. V95's previous claimed visibility-resource gap was incorrect.

Actual33dcf0 stores requested byte85 only. Actual33dcf8 stores byte80=requested?actual8a:0 only. Enabled invokes SetVisible(true) then setUpdating(true); Disabled reverses their order with false. V95 already writes8a before this inherited virtual delivery through object_test_enable_condition_v2. This patch lends the SAME LightPoint fields and weak typed Enabled/Disabled callback, pins the actual alias through the synchronous call, and respects existing teardown writability. No GameObject visual/PF methods are invoked.

LightPoint.InitPost40bcdc clears1b0/1b1, qualified LightBase.InitPost40b0c0, writes SAME CLight type58=0, then RefreshAttachment virtual58. Complete40b0c0 has no ObjectBase.InitPost33ec0c or ConditionData.Init call; therefore no condition compiler call/table/arena is added. Existing embedded original C1NULL a8/cc and tested0 ac/d0 (V95) remain authentic. Source inherited TestEnable can read these actual cells without generating conditions from a detached table.

LightBase.SyncData40a8f8 publishes radius40 (strict>0.01), attenuation34/38/3c, and RGB/alpha1 at4/14/24; it writes neither node visibility nor CLight.enabled5a. Existing V92 retained CLight/node/LightSet owners and Debug DAE gate are preserved. This patch deliberately leaves Scene/LightSet eligibility and renderer clock in Main's real provider lifecycle; SetVisible does not invent a CLight setter or Scene registration event.

No catalog_auxiliary_v67 or catalog_generic_v70 edits: Main combat visitor additions are reserved.
