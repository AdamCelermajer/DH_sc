# Menu receiver and resource lifecycle integration

This source change connects actual registered receiver fields, MenuBase Update,
owned-receiver destruction and live directory erase to the V58 menu kernels.
The original character is weakly held. A replacement at the same UI path or a
new movie binding generation cannot become the old receiver. Temporary AS pins
exist only during the validated owning movie Scope; no stored strong context
keeps the old character alive. MenuBase animation completion increments the
original uint32 counter, including wrap, without advancing the movie.

The reviewed V91 resource packet was adopted after all fourteen baseline and
payload hashes matched. Facades and existing V1/V2 input connections/sessions
share one constructor-created four-controller store. Source reset drops five
references in order without UI events or cursor/enabled changes. Unload performs
the original two reset passes, disables the detached AS binding and clears the
same menu catalog/active-state counts and render context/flags. Adapter backing
is prepared before destructive work, without constructing a second controller
store. Retired backing may survive through external leases; exact final disposal
timing still needs a runtime lifetime test.

The renderer query was resolved directly from the supplied ELF: RenderFX.Unload
7a90bc uses GOT99844c, whose R_ARM_RELATIVE word9fc994 resolves to
`gameswf::s_render_handler`, size4. The UI library exports the actual
`gameswf::get_render_handler()` query without exposing vendor types to Android.
Quiescent NULL is a real result. A nonnull extended-renderer virtualA4 remains a
required provider rather than an inferred successful operation.

Validation uses serial Windows NDK syntax checks in readback-verified 512MiB
jobs, maximum4 descendants, 30-second per-process timeout, at least8GiB free RAM
and at most900 host processes. The changed source and Android bridge compile
for ARM64 and x86-64. Highest observed bridge job usage was229MiB. This is syntax
validation only: no link, fixture execution, APK build, emulator or phone run.
Receipts: `menu-lifecycle-v59-syntax/receipt.json` (each source hash and exit),
`resource-unload-v91-adoption/receipt.json` (reviewed adoption).

Behavioral fixtures cover live registry mutation/unowned receiver survival,
HUD gating, visible-callback receiver rereads, weak expiry/path replacement,
movie generation rejection and counter wrap. They are written and syntax
checked but not executed. Gameplay acceptance remains open.

Remaining activation: actual source PM/Debug update prefix, FlashAnim backend,
map icon owner, resource slot/destructor/auxiliary mappings, and subsequent
loading-stage providers. Source Play now calls selected-profile GS/Swamp
bootstrap; that route is unverified and intentionally retains missing-provider
errors. The installed c6691313 APK remains the previously tested Crypt demo.
