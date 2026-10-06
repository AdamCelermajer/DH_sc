v82 Twitter browser handoff

Source: original MenuBase::OnEvent423214 owns release-kind6 Twitter navigation to http://ingameads.gameloft.com/redir/?from=D2HP&op=TBFV&game=D2HP&t=twitter. NativeLaunchTwitter43a6a4 only queries SavegameManager language; registering it does not enqueue another browser request. Original nativeOpenBrowser532c48 and DungeonHunter2.openBrowser use Android ACTION_VIEW with Uri.parse, confirmed from original ELF/DEX.

The isolated front now supplies MenuNativeEventServicesV1.browser, retains a GL-owned URL queue, and drains it through JNI after dispatch. MainActivity opens the Intent on its UI thread. If Android has no handler, it reports that condition instead of claiming the link opened. The queue clears with session teardown. No preview save fields, new website URL, login, purchase, catalog or online-game state was invented.

Offline x86_64 APK build passed. Installed SHA2562b3a4914139180ee59378390cc26d2576c5e12253a25fcd7dbc5d2c2611fc63c matches build output. Authored Info/Twitter taps at1080x1920,1968x2184,1080x2400 each produced exactly one native-event request and one Android dispatch. Actual ACTION_VIEW launch was observed. Launcher return retained the same process and Info stack; authored Back returned to main. Campaign/settings hashes unchanged. Evidence browser-v82/validation.json, logs and screenshots. Info screenshots and settled-main screenshot inspected directly. Immediate return screenshots include the original slide animation and must not be mistaken for final layouts.

Chrome's startup screen intercepted the link; the original remote redirect/page remains unverified. The test proves platform dispatch and menu return, not current availability of Gameloft services. More Games/GLive and canonical gameplay loading still remain unfinished. No routine message was sent to the main session. Emulator5554 and main checkout were untouched.

Snapshot checkpoint: checkpoints/front-v82.apk and front-v82-source. Cumulative contribution-review.patch parses;181file receipts. Current visible emulator5580 is back at the main menu with Warrior/sword intact.
