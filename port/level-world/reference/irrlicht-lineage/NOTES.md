# Irrlicht lineage: verified correspondences and limits

The supplied Android ELF contains an Irrlicht-derived engine fork under namespace
`glitch`. Historical stock source gives useful GUI and core correspondences, but
the inspected scene graph, rendering API and animation system have substantial
custom behavior. **The exact Irrlicht base version is not identified.** This
discovery does not replace native modules, import an upstream engine, build APKs
or claim full engine equivalence.

Original ELF SHA256:
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
`original-functions.json/.asm` capture13 relevant original routines with byte
hashes. `local-symbol-evidence.json` records exact local symbol evidence.

## Primary historical source retrieved

The project's [official source instructions](https://irrlicht.sourceforge.io/?page_id=140)
identify the SourceForge SVN repository. Direct HTTPS reads from its historical
release tags succeeded, even though the web page reader could not retrieve those
URLs. No source leak or reconstructed third-party game source was used.
`public-source-manifest.json` records15 source files from releases1.4.2,1.5.1 and
1.6.1, their exact URL/response URL, SHA256, byte count, SVN ETag revision,
Last-Modified and retrieval time. Complete fetched files remain in owned scratch
`.local-inputs/irrlicht-lineage/<version>/`.

Important primary URLs and SHA256:

| File | Official release1.6.1 SHA256 |
|---|---|
| [ISceneNode.h](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/include/ISceneNode.h) | `6390d2261dcd8d4fa71fef74dc82d4ac9022d9250a8277c0b7c69b8218276d1f` |
| [CGUIEnvironment.cpp](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/source/Irrlicht/CGUIEnvironment.cpp) | `4636b07dfd5b000ce31de5c799e3aef7fce5d9ca4665f6f79189a8b11b0977ac` |
| [CGUIButton.cpp](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/source/Irrlicht/CGUIButton.cpp) | `7c909663edde9cb74a78a5bc23c373db7c7b9038dcdcca90bf7130c8e7e84b82` |
| [CGUICheckBox.cpp](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/source/Irrlicht/CGUICheckBox.cpp) | `e06c89820cb064e3e87634d7afa37a98868db6c881630e385ba43bb527c6f3fa` |
| [CSceneManager.cpp](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/source/Irrlicht/CSceneManager.cpp) | `81dc3c59ce2d5712e7f6dfbd7f08bd4ba8feee46c621f0c720234017665319ac` |
| [COpenGLDriver.cpp](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/source/Irrlicht/COpenGLDriver.cpp) | `de0abcd87c02572f5fd93dbcd9c035968150e6cc72fe97e50784b2344ded341f` |
| [quaternion.h](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/include/quaternion.h) | `dff258b1ab8bb743f1d4ec5b248670b51d5163e0ebf3d6d1813d28d8a688484c` |
| [irrlicht.h](https://svn.code.sf.net/p/irrlicht/code/tags/release-1.6.1/include/irrlicht.h) | `72ef454154ac2743961021f9d0ff5abddd8440a3290b53a737569278e06997b6` |

For example the scene-node header's SVN ETag identifies revision2927 and
Last-Modified25Nov2009. These metadata identify the inspected public file; they
do not date Gameloft's fork point. The [official release listing](https://sourceforge.net/projects/irrlicht/files/Irrlicht%20SDK/1.6/)
dates the1.6.1 directory13Jan2010. Earlier headers and GUI files were inspected
from the analogous official `release-1.4.2` and `release-1.5.1` tag paths, with
separate hashes in the manifest. A preliminary ZIP request returned a download
HTML page; that scratch file is excluded from source evidence.

## Concrete local/source correspondences

**GUI receiver filter: exact recovered behavior correspondence.**
`glitch::gui::CGUIEnvironment::onEvent` at`0x535710` forwards to its installed
user receiver only when the event is neither mouse(type1) nor key(type2), and a
GUI event(type0) is not attributed to the environment itself. Otherwise it
returnsfalse; forwarded events return the receiver's result. Historical
CGUIEnvironment::OnEvent in all three inspected releases has that same predicate
and result behavior. The local GUI interface's self pointer is base+8 due to
the original class layout. `probe_original.py` executes the actual original
routine for56 combinations of type, receiver presence, caller identity and
callback result, matching the historical predicate with zero mismatches.
This is a bounded correspondence, not a complete GUI reconstruction.

**GUI button: recognizable branch structure, source comparison only.**
`CGUIButton::onEvent` at`0x6a6eac` contains the upstream Enter/Space press paths,
Escape cancellation, push-button toggling, focus-lost handling, clipping-aware
mouse release and parent button-click notification. Original event structs,
class offsets and virtual dispatch are `glitch` ABI. The historical public source
is a practical recovery guide; this discovery has not proven every button branch
or substituted its source into the port. `CGUICheckBox::onEvent` at`0x6a7ed8`
is additionally captured for its related key/focus/mouse notification structure.

**Scene node animation: common skeleton with material differences.**
`ISceneNode::onAnimate` at`0x596d6c` follows animators, absolute transform update,
then child recursion, matching the broad stock operation order. It adds source
flag gates0x400/1/0x200, clears flag0x20 afterward, and advances the animator list
after each callback. Stock1.4.2 likewise advances afterward; stock1.5.1 and1.6.1
advance the iterator before the callback to permit self-removal. This similarity
alone cannot pin the fork to1.4.2: Gameloft modified traversal and list ownership.
Base registration `0x596d08` simply returns1, whereas the inspected stock base
OnRegisterSceneNode walks visible children. The original return1 executes in the
probe and is preserved as explicit contrary evidence.

**Scene transformations: incompatible rotation and caching contract.**
Original getRelativeTransformation `0x598908` uses cached local matrix, dirty bits,
quaternion rotation and a returned matrix pointer. Historical stock
ISceneNode::getRelativeTransformation returns a matrix value generated from Euler
rotation degrees, translation and scale. Local addChild `0x598864` also uses
embedded sibling links and additional manager/visibility updates; stock uses its
own container and reference-count lifecycle. Names do not establish matching ABI.

**Quaternion: close algorithm, specific source alteration.**
Original slerp `0x612d00` has the stock1.6.1 hemisphere choice,0.05 thresholds,
acos/sin spherical branch and legacy orthogonal fallback. Its linear branch
normalizes the result; the inspected stock header returns the raw weighted sum.
The existing reconstructed `dh2_quat_slerp` correctly retains the original
normalization. Reusing an unmodified historical header would change behavior
despite the strong algorithmic lineage.

**Scene manager/driver: custom signatures and ownership.**
Local CSceneManager::drawAll accepts an explicit root(`0x58b7f4`) or vector of
roots(`0x58b728`). registerNodeForRendering `0x58f348` accepts an intrusive
CMaterial reference, opaque data and extra parameters. Inspected stock1.6.1 uses
parameterless drawAll and simpler node/pass registration. The local Android
createDriver `0x6a0454` belongs CAndroidOSDevice and routes through its custom
device/driver services. Stock COpenGLDriver is not the local backend class.

All-symbol `.symtab` substring counts (not unique methods) include1,027CGUI,
167CSceneManager,343IVideoDriver,41CAndroidOSDevice,175CProgrammableGLDriver and
181CGLSLShaderHandler entries. COpenGLDriver,COGLESDriver andCIrrDeviceStub counts
arezero. The recorded Irrlicht global initializer paths identify the source
tree's custom Collada, animation-set/blender, GUI, factory and file-system units.
These counts support lineage and backend distinctions, not code identity.

## Animation and practical reuse boundary

The inspected stock scene interface explains the base animator/child traversal,
but the local two-slot AnimatorSet/AnimationBlender, compiled Collada union targets,
timeline callbacks, root applicator histories and PlayClip replay are custom
recovery work. See `../animation-blend-composition/NOTES.md` and the104 captured
original routines there. They cannot be supplied by stock scene traversal or
by replacing it with generic upstream animated-mesh playback.

The useful next reuse targets are narrow, identifiable GUI/core/file-system/math
algorithms with original instruction comparisons. Preserve recovered original
interfaces, defaults, type conversion and ownership around any shared algorithm.
Stock Irrlicht may reduce future reconstruction effort, but transplanting its
complete scene manager, OpenGL driver or GUI object layout is not justified by
these matches. The exact base revision and extent of unchanged source remain
unresolved; the report deliberately does not infer them from class names alone.

## License and evidence scope

The retrieved official1.6.1 `irrlicht.h` includes the copyright and permissive
zlib-style license. `LICENSE-Irrlicht-1.6.1.txt` is an exact copy of its opening
license block; its byte hash and the containing header hash are recorded in
`license-manifest.json`. It permits use/modification/redistribution subject to
accurate origin, marking alterations and preserving the notice in source
distributions. The block separately acknowledges JPEG/zlib/libPNG dependencies.
This is attribution for these public Irrlicht source files; it is not evidence
of licensing rights over Gameloft's custom engine or supplied game binary.

No downloaded public source is compiled or linked by this discovery. No native
comparison, full GUI behavior, original app/FSM parity or rendered engine parity
is claimed. Source hashes and original captures make the bounded correspondences
reviewable and reproducible.
