v52 keyboard compatibility correction

The supplied Android keyboard samples loading-spinner fragments from its splash atlas. Generic corresponding key shapes provide matching original letter/digit, pressed-glow and Delete artwork. Shift and Space use identified original atlas graphics. Their mappings are compatibility choices; the original Android atlas mapping remains unresolved. No replacement artwork was generated. Original cache bytes stay intact and are validated before the separate compatibility SWF is loaded.

repair_keyboard_atlas.py generates front-compat/dqmenus_droid.swf and its provenance manifest. Seven keyboard shapes change, including bitmap matrices introduced by internal NewStyles records. verify_keyboard_atlas.py confirms every other tag, action, placement, shape edge and bound is byte preserved. This fixes visible keyboard fragments without claiming full original visual fidelity. Space currently stretches the original blank capsule; preservation of original border proportions remains a fidelity limitation.

keyboard-tests-v52-settled covers the exact APK on three surfaces: 1080x1920, 1080x2400, 1968x2184. Real taps verify lower/upper case, Space, Delete, digits, eight-character limit, Back and retained-process resize. Held-pointer screenshots show the actual pressed-letter glow. The final visible keyboard uses physical 2400x1080, no wm override. Earlier keyboard-tests-v52-final failed after resizing 0.4 seconds after Back; its blank main screen and logs remain preserved. The successful run waits 1.5 seconds after Back and verifies the actual pop. Resize during that transition remains an unresolved runtime case. No campaign save writes. The main checkout/session/emulator were untouched. Full menu goal remains active: loading/other icons, full actor equipment/lighting and Confirm/game creation remain open.

APK SHA256: e96412b1da97ce4495a8c6dd4e40c53285ba36abca50a8e171d5979e1dc629e0


v57 keyboard texture and Space capsule correction

The separate front-compat movie uses original cached artwork for letter/digit keys, their pressed states, Shift and Delete. The Space normal/glow states now preserve curved end proportions and stretch only their middle. Three bitmap rectangles retain the original Space bounds and bitmap pixels. NewStyles separates their tessellation layers. Explicitly clearing FillStyle0 is required after each layer for the existing GamesWF point test; leaving its sentinel would make the middle/right slices unclickable. No engine/vendor changes were needed.

The structural verifier confirms all 618 tags remain, with every action, placement and non-keyboard tag byte preserved. Five shapes change only matrices; Space shapes80/82 intentionally change their geometry to three adjacent rectangles within original bounds. Original cache assets remain intact. These mappings are compatibility recovery from original artwork; exact original Android atlas coordinates remain unresolved.

The exact installed APK passed real typing, Shift, Delete, digits, eight-character limit and Back on three surfaces:1080x1920,1080x2400,1968x2184. Space was tapped at left, middle and right on each. Screenshots cover normal/pressed Space and held letter artwork. Text survives retained-process resize. The final visible emulator is DH2_Launch:5580 at physical2400x1080 with no wm override, left on Enter Name. Failed development screenshots/receipts remain in keyboard-tests-v57 and keyboard-tests-v57-final; the authoritative final run is keyboard-tests-v57-verified.

The full menu/game-launch/loading/media/resolution objective remains active. Confirm/game creation, occupied save slots, full equipment/lighting and other menu artwork remain unfinished. Keyboard verification on three sizes does not establish all-resolution correctness for the whole application. Main checkout, chat and emulator5554 were untouched.

APK SHA256: b414f21ca5ea266b7fb83bfe3785a72cf80a8b2cf80cd64dc8f0f8868d0a16e8


v63 Exit power icon

Main menu btn_exit sprite509 contains icon sprite508/shape507. The supplied Android shape samples bitmap1 at x48.72..77.22,y697.17..725.67; its embedded bitmap tag has only a seven-byte format0 placeholder. The supplied generic menu owns the complete bitmap atlas and corresponding btn_exit sprite496/icon shape494 uses the actual power glyph at x656.76..694.16,y774.62..812.17. The separate compatibility movie now maps shape507 to that original glyph while preserving Android shape bounds, edges, placements and all actions. No artwork was generated and original cache bytes remain intact. Exact Android atlas mapping remains unresolved; this is recovery from the corresponding original menu asset.

The structural verifier passes all618 tags: five keyboard shapes plus Exit change matrices only; two Space states keep their existing verified capsule slices. Exit normal/pressed artwork, confirmation opening and No cancellation pass on three retained-process surfaces1080x1920,1080x2400,1968x2184 with screenshots visually inspected. Yes/application shutdown is not exercised. Keyboard typing, Shift, Space at left/middle/right, Delete, digits, eight-character limit, Back and retained resize pass again on the exact installed APK. Physical dimensions are restored with no override. Visible emulator5580 remains separate; main session/emulator5554 are untouched.

The build also contains the verified v62 native QuestSavegame dispatch/progress API. Authored quest construction and actual condition/objective data are still required services. Campaign persistence, occupied AS display, Confirm/assignment/StartGame, full equipment/lighting and complete menu/media/resolution verification remain unfinished. Full goal remains active.

APK SHA256: 93040894d9c1356ca91a611a3d2929af75c49fb5c1230b4b6c515c2b9a94f9cd
