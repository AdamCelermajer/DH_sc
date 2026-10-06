# Authored FX geometry packet V7

Live emulator5554, installed APK a4b5ae41ff45c73485d10a98adde3a0c1a0d32f923c1790f260e2dc0efc576f9, PID9645: sourceBash activation accepted with one native target, clip1234 range0..1666 and event700/FX164. The first FX rendering frame failed beforeUse: `Required authored effect triangles/attributes`. Evidence is under android-native/reports/generic-skill-live-v6. There was no verified do_skill or enemy damage in that cast.

The production mesh guard required COLLADA type0 with engine type3. Original ColladaPrimitiveMap at8eb338 is [6,4,3,1,2], so that pair cannot come from the actual BRES decoder. The earlier GLES test used a synthetic quad and exercised source material/default-attribute behavior, not this production geometry guard.

New authored_fx_geometry_packet_v7.hpp/.cpp builds the actual retained upload packet and requires source triangle-list type0→6. All original backing, material-symbol, position count, U8Color4, UV2, source indices and ushort range checks remain. Failure leaves output unchanged and reports actual enum/index details. MissingColor0 uses the existing source constant-attribute draw path. V6 generated billboard primitives now carry the same triangle-list engine6 metadata; old V5 resources/receipts remain unchanged.

renderer_authored_effect_scene_v5.inc calls this whole packet builder. model_renderer.cpp includes its header; level-world/CMakeLists.txt adds the TU. Actual-cache linked regression passes334179 checks over38 supported resources,1888 mesh packets and279 particle packets; the impossible old enum pair is explicitly rejected. Strict packet/changedV6 compile passes bothABIs (4), and the coherent renderer/panel closure passes18 compiles. These are packet and compilation receipts, not live GPU/Use/damage acceptance. Root must rebuild and repeat the actual enemy cast.

The V7 manifest records current packet/V6/renderer-source hashes. The preceding V6 manifest is a historical pre-packet-fix snapshot and is not a current-source hash assertion.
