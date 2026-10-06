v80 authored Exit and same-process relaunch

Original ARM NativeHUDInteract43afb8 literals verified at8cbbd8("quit") and8cbbe0("no"). quit sets confirmation flag, no clears it, other strings call appDestroy530a1c -> Application::Quit. Authored dqmenus sprite510 exit/Yes/No handlers remain unchanged. Main front registers the callback; exit request is consumed after ActionScript/frame dispatch on the GL owner. It releases menu preview/movies/fonts/textures, resets avatar state, deactivates model renderer, and posts Android audio release/finishAndRemoveTask. No process kill, save deletion or gameplay-owner teardown is used.

Same-process relaunch exposed two upstream engine lifecycle defects: last player clears loader table but ensure_loaders_registered used a permanent flag; builtin method table cleanup deleted maps without nulling pointers. Loader registration now probes the live table, and deleted builtin maps are reset to NULL. Crash evidence is preserved in exit-v80-final2/observation-expired-all.log (get_builtin use-after-free). Scoped vendor diffs are included in cumulative review patch, with verified unchanged main-source baselines. Main checkout and emulator5554 are untouched.

Final installed APK on visible isolated5580/privateADB5038 tested at1080x1920,1968x2184,1080x2400. For each: authored confirmation fits; No resumes main; Yes releases front task/audio; same live process relaunch loads main and restores Warrior/sword preview. Actual resumed Activity after Exit is outside com.example.dh2. Screenshots reviewed. Campaign/settings SHA hashes are unchanged. Evidence exit-v80-final3/validation.json and per-size PNG/log files.

Loading/cinematic v79 receipts remain tied to their exact earlier APK. This v80 change specifically tests Exit/relaunch; no new full cinematic claim. Automatic loading transitions, actual game start/canonical player/save assignment, online services and preview lighting remain unfinished. No routine message was sent to the main session. Goal remains active.

Installed APK SHA256 a24dd9947daf20af2f31d5090baefccc730082efa30b6326e16244ba6ce8585d
