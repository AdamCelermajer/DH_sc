# Native GameSWF input and frame connection v1

This batch connects the recovered four-cursor RenderFX coordinator to a retained native GameSWF graph, source root/sprite scheduling, deferred init/action batches, goto/play and source 2D drag. It does not call stock root advance or its mouse-processing prefix. Vendor source bytes, the existing facade, Android sources and packaged checkpoints were not changed.

## Original identities and order

Original ELF SHA256 is `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. Main input captures are the already frozen `../swf-input-connection`; additional complete bodies and manifests are under `../swf-cursor-input`.

- `UpdateCursor 7ac924`, `UpdateInput 7ac4bc`, `SetFocus 7ac228`, `ResetFocus 7ac410` and `RenderFX::Update(int,bool) 7ad68c`: source masks, four slots, old/new strong assignments, event consumption and live rereads. Update obtains a strong root, converts signed milliseconds to float seconds by division by 1000, calls root advance with the incoming bool, then handles four pending completion events unless flag64 is set.
- `root::advance(float,bool) 775304`: engine mutex getter, listeners, remainder/GC delta, random, first-load flash variables, deferred init actions, construct, sprite advance, first-load root load event, remainder subtraction, optional catch-up, periodic GC and fmodf. The source engine mutex getter is executed, not relabeled as an invented native lock/unlock pair.
- `sprite::advance 78240c`: first construct/load before the visibility gate; need flag from goto count; drag; copied and cleared goto batches; source warning after the twelfth batch; play/frame count/current frame/wrap; optional EnterFrame; current actions/frame script before child advance; loaded flag at exit. Callback mutations and newly queued goto actions are reread. Source frame indices are signed16 and play state is signed8.
- `execute_frame_tags 78215c`, `do_actions 781eac`, `do_init_actions 78069c`, `construct 77efd8`, `display_list::construct 755a10`, `display_list::advance 755908`: actual native tag/AS execution, independent pending init history, copied action batches and reverse strong pushes/forward child processing. `movie_def_impl::create_instance 76020c` executes frame0 tags after creating the root. The connected test uses this constructor entry.
- `character::advance 752d88` clears need9d. `edit_text::advance 78a384` only invokes virtual to_string; text layout/HTML semantics remain the separate text overlay. `button::advance 7c6f40` runs drag, reads world matrix, visits forward records with fresh counts, advances active records and calls this_alive on inactive records. Stock alive is not substituted.
- `sprite::this_alive 780594` marks base members first, then the captured display count, reading live children and their garbage epochs. `as_object::this_alive 76c518` examines stored OBJECT members without property getter calls, then prototype. The native GC representation uses the real upstream player heap/weak identities; it is not a byte-layout reconstruction of the original heap or original boxed-object payload representation.

Source 2D do_mouse_drag `75e3a8`, start `775154`, stop `77415c`, and sprite drag get/set are captured. Initial offset is derived from raw mouse twips minus **local** matrix translation on the first advance, not eagerly at startDrag. Lock-center uses parent inverse. Bounds are source pixel bounds multiplied by20. Source finite stores and comparison selection, including IEEE cases, are retained. Stop clears only target, preserving offsets and flags.

`MenuFX::Update 7ad88c` is the sole direct branch caller found for RenderFX::Update and passes the incoming bool unchanged. This does **not** establish the original Android Application/UI caller's chosen bool. The integration must supply that producer explicitly. The one-argument native root overlay calls the explicit false variant; this is a declared adapter policy, not proof of the Application's policy.

## Concrete integration

1. Compose `gameswf_input_overlay_v1.cmake` and `gameswf_frame_overlay_v1.cmake` after `gameswf_sources.cmake`. They replace exact stock character/object and sprite/root/button translation units. Font and text overlays replace separate units and must be composed independently. Add input/history/geometry/policy/connection, frame schedule/connection and drag sources plus the frozen event and viewport modules to the same core DSO.
2. In facade graph_start, before any shared/root load_file, bind `SwfInputHistory` and then `SwfFrameConnection` to the exact player. These owners hold weak graph identities; do not store a strong SwfAsLease owner in this provider.
3. Bind `SwfInputConnection` to the exact facade/viewport lease in its Scope. Supply actual orientation/dimensions, native CanHandleEvent and mutable native event services, source context/flags/selection. Advance forwards to `SwfFrameConnection::advance(root, seconds, sourceFlag, error)`.
4. InputConnection and its frozen viewport connection hold a **strong** graph lease. Keep a bound InputConnection in an outer session/controller, not inside native_owner retained by Impl: otherwise Impl→native_owner→InputConnection→Impl forms a cycle. Reset input before releasing/reloading its movie; callbacks borrow the outer owner only while its Scope is valid. Frame/history may live in native_owner.
5. Invoke input/frame functions inside the exact facade Scope. Borrowed player/root/AS pointers are not cross-graph handles. In-flight operations pin receiver state; releasing the handle during a callback does not destroy the receiver mid-call. Strong input slots and child snapshots survive source synchronous reentry. Native invalid/expired/unobserved bindings fail explicitly.

History observes source setters after watcher invocation and before map/read-only/store checks. Incoming values do not determine handler flags; the exact case-sensitive eight names and onEnterFrame producer are used. Capital `onRollOver/onRollOut` are distinct from flag-setting `onRollover/onRollout`. No scan of current members substitutes for setter history.

The FrameConnection domain is source 2D GameSWF characters. Input's optional 3D scene-mouse provider is explicit, but frame drag's corresponding 3D mapping is not implemented. Do not connect source nonzero scene bindings to this frame owner. AVM2 and missing source frame/history receivers fail; they are not accepted as no-ops. Sound services remain required when an actual stream sound is reached. Native can-handle/event services remain real caller-owned endpoints; this batch does not fabricate HUD/game action acceptance or route named buttons directly.

## Proof and limits

Optimized ARM64 original-instruction comparisons: 6000 full input cases (24774 ordered services,14 recursive ResetFocus calls); 5200 inverse/point IEEE cases and1700 exact setter-policy cases; 1200 root and1200 sprite cases (17776 ordered services); 2400 source 2D drag cases (600 delayed set-drag calls). Total17700, zero mismatches. Original graph hit/shape, AS/tag, sound, GC and application services are explicit oracle fixtures. Actual native topmost traversal/shape fixtures and actual native AS are separately exercised by the connected host, not relabeled as a whole original movie/VM oracle.

Host ASan/UBSan/LSan: original6000 input gold plus2400 frame and2400 drag gold;17 connected input cases,5 setter-history cases,51 native events,43 actual AS methods,6 input guards;28 connected frame checks, actual init/load/action/child order,12-batch requeue, source base/button endpoints, 2D drag, GC subtree retention and release inside actual AS callbacks.26 atomic/provider-prefix frame guards. Zero findings. f32 libm graphic outputs in input gold are original-derived fixtures; arithmetic NaNs compare classification, finite words and signed zero compare exactly. This is a connected native source coordinator audit with genuine linked AS/tag services, not complete original AS interpreter, heap allocator, text, GPU or live Android parity.

The bounded caller contract rejects unsafe catch-up whose floating remainder cannot decrease (overflow/infinity/spacing), and invalid initial frame time before effects; dangerous callback-produced frame mutations report a required-provider failure with the executed prefix preserved. Source goto storage is bounded to4096 per native caller span. No guessed dt clamp, extra root mouse processing or once-only frame guard was added.

Reports: `../../reports/swf-cursor-input-arm64-differential.json`, `swf-input-geometry-arm64-differential.json`, `swf-frame-schedule-arm64-differential.json`, `swf-drag-values-arm64-differential.json`, and `swf-native-input-frame-host-audit.json`.

Reproduction (WSL, isolated workspace build only):

```text
python3 port/engine-ui/tools/swf_cursor_input_host.py --build-directory .local-inputs/swf-cursor-input-host --gold port/engine-ui/reference/swf-native-input-frame-v1/input-gold.bin --frame-gold port/engine-ui/reference/swf-native-input-frame-v1/frame-drag-gold.bin --report NEW_REPORT.json
```

Targets are `input_core` (actual shared core), `audit`, `frame_audit`, `gold_audit` and `frame_gold_audit`. The input gold executable uses direct source kernels and test-only sinf/cosf/sincosf linker wrappers; frame/drag gold and connected tests execute the actual core DSO. Host reports bind its exact library, executables, selected new sources, complete immutable vendor source inventory and replay bytes. No APK, emulator or shared build was changed.
