# V57 helper declaration typo supplement

SOURCE ONLY, NOT COMPILED. One declaration token changes same_light_runtime_owner_v56 to same_light_runtime_owner_v57, matching the existing three callers. No ownership or source teardown semantic change.

Baseline header SHA256: 7e2a440458c9ee4517746634c0ff5bfa47b5643faeccdcf59a7337c09d304a55
Corrected header SHA256: 58e22fe2ca7d11c12da22d99a115b7a10c5d9329eae28e5665b83e2e9a5fd4ff
Frozen V57 archive SHA256: 107480bf15bf4075444a82654daf6f87180b72b5384b309a130d90ad9b492750

Apply this supplement to Main's V57 adoption before compilation: use the corrected proposed header or the one-line patch. Do not ship the original undefined helper spelling. Prior V57 source/archive remain unchanged. Corrected bytes reverse to the exact baseline bytes when this single token is reverted, including line endings/encoding.

Bounded helper review: corrected helper declaration matches every runtime-owner call; prepare_source_teardown and execute_source_teardown declarations match their source bodies; current-thread/exact-actor/exact-native-reference validation, actual native drop and actual base continuation declarations match all their source uses. No superseded V56 destructor helper appears in V57 source. This is text/source compatibility review, not C++ compile/link/runtime proof.

Actual Main native/base/retained-journal/quiescence providers remain mandatory. No build, tests/oracle/native execution/GPU/device or shared-source edits occurred.
