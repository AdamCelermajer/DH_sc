# Irrlicht 1.5.1 second-opinion pass over the reconstructed engine bodies (owner U, 2026-10-16)

Scope: glitch::gui (all), glitch::io, glitch::core math, and the scene animators, particle emitters and affectors that have an upstream
counterpart. Upstream is a read-only reference (`%TEMP%\irr-hunt\x1.5.1`, with 1.4.2, 1.5.0, 1.6.1 and 1.7.3 for version comparison);
nothing was copied into the tree. The binary decides (TRACK-B decision 6): every difference from upstream was settled against Hex-Rays
(`tools/decomp_extract.py --address`) or the assembly.

Per-method verdicts: `reports/decomp-reconstruction/irrlicht-verify-2026-10-16.csv` (one row per ledger row in scope).

## Result in one paragraph

No behavioural BUG was found. In every method read in full (about 115, listed below) our body equals the binary, and where it differs
from Irrlicht 1.5.1 the binary differs the same way (a glitch fork or an older Irrlicht). One function that upstream resolves was
genuinely missing and is now filled (CNumbersAttribute::setString, 0x325070, was `skipped`). One signedness spelling was made explicit
(no behaviour change, see below). Member names were aligned with upstream where the binary offsets agree (SPakFileEntry, the
CZipReader / CPakReader / CUnZipReader flags, the particle mesh emitters' `Particles` vector).

## How the verdicts were produced

1. Every ledger row (ledger.csv overlaid with the newer ledger.d parts) whose class maps to an upstream class in
   `irrlicht-class-map.csv` and falls in the scope above: 1,940 rows. Our body is found by its `// 0x...` marker in the category sources
   (core-io, scene-collada-gui, shared glitch_core.hpp). The upstream body is the same-named method in 1.5.1 (header inline or .cpp).
2. 1,305 rows have both bodies. Each pair is scored by a token-sequence ratio (calls, string and number literals, control keywords; names
   of locals and members ignored) against 1.5.1 and against the other four releases:
   - MATCH: ratio against 1.5.1 at least 0.85, or against any release at least 0.95 (same logic).
   - ADAPTED: 0.60 to 0.85: the same shape in glitch types (accessors, allocators, intrusive_ptr, helpers). Not individually read unless marked `read`.
   - FORK: below 0.60, or a manual read that found a difference (the note column of the csv says which).
3. Binary cross-checks run over all 1,305 pairs against the Hex-Rays text: (a) number literals in our body that appear in neither the
   binary nor upstream; (b) comparison thresholds present in both the binary and upstream but missing in ours, and the reverse; (c) callee
   names in the binary missing in ours. About 140 flags came out; each was opened and is idiom noise (SColor memcpy, intrusive_ptr drop,
   wcslen via assign, std::cos for cosf, hex literals) except the items under "Bugs fixed".
4. About 115 methods were read in full against upstream and Hex-Rays (column `read`). Gui: CGUIButton (all), CGUICheckBox, CGUIComboBox,
   CGUIContextMenu (draw, recalculateSize, sendClick), CGUIListBox, CGUIEnvironment (setFocus, hover, tab order, fonts, built-in font),
   CGUIFont, CGUIImage, CGUIInOutFader, CGUIScrollBar, CGUIStaticText, CGUITabControl, CGUITable drag, the CGUISkin index functions, the
   edit box cursor, scroll and input functions and the key switch, IGUIElement::updateAbsolutePosition, the colour ring. Scene: gravity,
   fade-out, attraction and rotation affectors, box emitter, delete, fly-straight, follow-spline, texture and FPS-camera animators. Core:
   slerp, line2d::intersectWith, plane3d::getIntersectionWithPlane, triangle3d::isTotalInsideBox, getRotationDegrees, getInverse. Io:
   CStringAttribute::getBinary, CColorAttribute::setString, CMemoryReadFile::seek, the XML reader's readFile, parseComment,
   parseOpeningXMLElement and replaceSpecialCharacters, CZipReader::scanLocalHeader, CFileList::getFullFileName.

Limits: MATCH and ADAPTED rows that were not read rest on the ratio and on the cross-checks in step 3, not on a line-by-line comparison
with the binary. Virtual callees are anonymous in Hex-Rays, so check (c) cannot see a wrong virtual. The io FORK count is inflated by
CAttributes, whose accessors go through glitch helpers (`at`, `getp`, `currentAttributes`).

## Counts

| scope | ledger rows | paired | MATCH | ADAPTED | FORK | BUG | fixed | read in full | glitch-only method | upstream name, no marker | dtors |
|---|---|---|---|---|---|---|---|---|---|---|---|
| **core** | 69 | 37 | 5 | 7 | 25 | 0 | 0 | 6 | 11 | 21 | 0 |
| **gui** | 711 | 519 | 244 | 143 | 132 | 0 | 0 | 85 | 40 | 42 | 110 |
| **io** | 740 | 468 | 124 | 103 | 241 | 0 | 0 | 9 | 107 | 67 | 98 |
| **scene** | 420 | 281 | 216 | 14 | 51 | 0 | 0 | 14 | 46 | 14 | 79 |
| **total** | 1940 | 1305 | 589 | 267 | 449 | 0 | 0 | 114 | 204 | 144 | 287 |

Rows without a pair: `glitch-only` have no same-named upstream method (glitch additions and engine helpers); `no marker`
have an upstream method but their body is a constructor or an inline definition without an address comment (not compared); `dtors` are
the compiler-generated destructors and deleting thunks.

### Per class (classes with at least one pair)

| class | rows | paired | MATCH | ADAPTED | FORK | BUG | fixed | read | glitch-only | no marker | dtors |
|---|---|---|---|---|---|---|---|---|---|---|---|
| core::CMatrix4 | 31 | 13 | 1 | 2 | 10 | 0 | 0 | 2 | 2 | 16 | 0 |
| core::aabbox3d | 4 | 2 | 1 | 0 | 1 | 0 | 0 | 0 | 2 | 0 | 0 |
| core::line2d | 1 | 1 | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0 |
| core::plane3d | 5 | 5 | 0 | 1 | 4 | 0 | 0 | 1 | 0 | 0 | 0 |
| core::quaternion | 12 | 8 | 1 | 3 | 4 | 0 | 0 | 1 | 4 | 0 | 0 |
| core::rect | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| core::triangle3d | 7 | 6 | 1 | 1 | 4 | 0 | 0 | 1 | 1 | 0 | 0 |
| core::vector2d | 1 | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
| gui::CDefaultGUIElementFactory | 11 | 8 | 2 | 1 | 5 | 0 | 0 | 0 | 0 | 1 | 2 |
| gui::CGUIButton | 24 | 20 | 12 | 2 | 6 | 0 | 0 | 19 | 1 | 1 | 2 |
| gui::CGUICheckBox | 10 | 6 | 5 | 1 | 0 | 0 | 0 | 5 | 1 | 1 | 2 |
| gui::CGUIColorSelectDialog | 10 | 6 | 1 | 2 | 3 | 0 | 0 | 5 | 1 | 1 | 2 |
| gui::CGUIComboBox | 19 | 15 | 3 | 8 | 4 | 0 | 0 | 5 | 1 | 1 | 2 |
| gui::CGUIContextMenu | 34 | 30 | 6 | 14 | 10 | 0 | 0 | 3 | 1 | 1 | 2 |
| gui::CGUIEditBox | 35 | 31 | 16 | 8 | 7 | 0 | 0 | 7 | 1 | 1 | 2 |
| gui::CGUIEnvironment | 73 | 62 | 26 | 25 | 11 | 0 | 0 | 6 | 9 | 1 | 1 |
| gui::CGUIFileOpenDialog | 11 | 7 | 4 | 1 | 2 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::CGUIFont | 30 | 21 | 8 | 5 | 8 | 0 | 0 | 5 | 5 | 1 | 3 |
| gui::CGUIImage | 14 | 11 | 8 | 1 | 2 | 0 | 0 | 1 | 0 | 1 | 2 |
| gui::CGUIInOutFader | 13 | 10 | 5 | 2 | 3 | 0 | 0 | 3 | 0 | 1 | 2 |
| gui::CGUIListBox | 37 | 33 | 10 | 13 | 10 | 0 | 0 | 7 | 1 | 1 | 2 |
| gui::CGUIMenu | 10 | 6 | 2 | 3 | 1 | 0 | 0 | 1 | 1 | 1 | 2 |
| gui::CGUIMeshViewer | 11 | 7 | 4 | 1 | 2 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::CGUIMessageBox | 9 | 5 | 2 | 1 | 2 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::CGUIModalScreen | 11 | 7 | 4 | 2 | 1 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::CGUIScrollBar | 20 | 16 | 8 | 4 | 4 | 0 | 0 | 4 | 1 | 1 | 2 |
| gui::CGUISkin | 30 | 28 | 8 | 6 | 14 | 0 | 0 | 7 | 0 | 1 | 1 |
| gui::CGUISpinBox | 20 | 16 | 13 | 2 | 1 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::CGUISpriteBank | 12 | 9 | 2 | 1 | 6 | 0 | 0 | 0 | 0 | 1 | 2 |
| gui::CGUIStaticText | 25 | 22 | 15 | 4 | 3 | 0 | 0 | 2 | 0 | 1 | 2 |
| gui::CGUITab | 15 | 12 | 12 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 2 |
| gui::CGUITabControl | 29 | 25 | 11 | 9 | 5 | 0 | 0 | 2 | 1 | 1 | 2 |
| gui::CGUITable | 49 | 40 | 18 | 15 | 7 | 0 | 0 | 2 | 6 | 1 | 2 |
| gui::CGUIToolBar | 8 | 4 | 0 | 3 | 1 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::CGUIWindow | 11 | 7 | 5 | 0 | 2 | 0 | 0 | 0 | 1 | 1 | 2 |
| gui::IGUIButton | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUICheckBox | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIColorSelectDialog | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIComboBox | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIContextMenu | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIEditBox | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIElement | 56 | 33 | 12 | 9 | 12 | 0 | 0 | 1 | 3 | 15 | 5 |
| gui::IGUIFileOpenDialog | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIFont | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIFontBitmap | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIImage | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIInOutFader | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIListBox | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIMeshViewer | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIScrollBar | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUISkin | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUISpinBox | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIStaticText | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUITab | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUITabControl | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUITable | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIToolBar | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| gui::IGUIWindow | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CAttributes | 193 | 133 | 1 | 46 | 86 | 0 | 0 | 0 | 57 | 1 | 2 |
| io::CBBoxAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CBinaryAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CBoolAttribute | 13 | 11 | 7 | 1 | 3 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CColorAttribute | 10 | 8 | 1 | 2 | 5 | 0 | 0 | 1 | 0 | 0 | 2 |
| io::CColorfAttribute | 8 | 6 | 1 | 2 | 3 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CEnumAttribute | 15 | 13 | 4 | 3 | 6 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CFileList | 8 | 4 | 0 | 1 | 3 | 0 | 0 | 1 | 0 | 2 | 2 |
| io::CFileSystem | 32 | 24 | 8 | 4 | 12 | 0 | 0 | 0 | 5 | 1 | 2 |
| io::CFloatAttribute | 12 | 10 | 3 | 1 | 6 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CIntAttribute | 12 | 10 | 5 | 1 | 4 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CLimitReadFile | 17 | 9 | 3 | 4 | 2 | 0 | 0 | 0 | 4 | 2 | 2 |
| io::CLine2dAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CLine3dAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CMatrixAttribute | 6 | 3 | 2 | 0 | 1 | 0 | 0 | 0 | 0 | 1 | 2 |
| io::CMemoryReadFile | 20 | 6 | 3 | 2 | 1 | 0 | 0 | 1 | 8 | 3 | 3 |
| io::CNumbersAttribute | 69 | 57 | 23 | 6 | 28 | 0 | 0 | 0 | 5 | 4 | 3 |
| io::CPakReader | 13 | 10 | 2 | 1 | 7 | 0 | 0 | 0 | 0 | 1 | 2 |
| io::CPlaneAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CPosition2DAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CQuaternionAttribute | 5 | 3 | 2 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CReadFile | 16 | 8 | 4 | 1 | 3 | 0 | 0 | 0 | 4 | 1 | 3 |
| io::CRectAttribute | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CStringAttribute | 19 | 16 | 1 | 2 | 13 | 0 | 0 | 1 | 0 | 0 | 3 |
| io::CStringWArrayAttribute | 7 | 5 | 4 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CTextureAttribute | 12 | 11 | 3 | 1 | 7 | 0 | 0 | 0 | 0 | 0 | 1 |
| io::CTriangleAttribute | 5 | 3 | 2 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CUnZipReader | 7 | 4 | 2 | 1 | 1 | 0 | 0 | 0 | 0 | 1 | 2 |
| io::CUserPointerAttribute | 11 | 9 | 5 | 1 | 3 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CVector3DAttribute | 5 | 3 | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 2 |
| io::CWriteFile | 12 | 6 | 5 | 1 | 0 | 0 | 0 | 0 | 2 | 1 | 3 |
| io::CXMLReaderImpl | 58 | 54 | 14 | 13 | 27 | 0 | 0 | 4 | 2 | 0 | 2 |
| io::CXMLWriter | 13 | 10 | 1 | 3 | 6 | 0 | 0 | 0 | 0 | 1 | 2 |
| io::CZipReader | 17 | 11 | 1 | 3 | 7 | 0 | 0 | 1 | 2 | 2 | 2 |
| io::IAttribute | 64 | 5 | 0 | 2 | 3 | 0 | 0 | 0 | 11 | 46 | 2 |
| io::IAttributeExchangingObject | 4 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::CDefaultSceneNodeAnimatorFactory | 12 | 9 | 1 | 3 | 5 | 0 | 0 | 0 | 0 | 1 | 2 |
| scene::CParticleAnimatedMeshSceneNodeEmitter | 28 | 20 | 17 | 0 | 3 | 0 | 0 | 0 | 5 | 1 | 2 |
| scene::CParticleAttractionAffector | 17 | 15 | 14 | 1 | 0 | 0 | 0 | 1 | 0 | 0 | 2 |
| scene::CParticleBoxEmitter | 22 | 17 | 14 | 0 | 3 | 0 | 0 | 1 | 3 | 0 | 2 |
| scene::CParticleCylinderEmitter | 28 | 23 | 22 | 0 | 1 | 0 | 0 | 0 | 3 | 0 | 2 |
| scene::CParticleFadeOutAffector | 11 | 8 | 5 | 2 | 1 | 0 | 0 | 1 | 0 | 1 | 2 |
| scene::CParticleGravityAffector | 11 | 9 | 7 | 0 | 2 | 0 | 0 | 1 | 0 | 0 | 2 |
| scene::CParticleMeshEmitter | 28 | 20 | 17 | 0 | 3 | 0 | 0 | 0 | 5 | 1 | 2 |
| scene::CParticlePointEmitter | 20 | 15 | 12 | 0 | 3 | 0 | 0 | 0 | 3 | 0 | 2 |
| scene::CParticleRingEmitter | 24 | 19 | 18 | 0 | 1 | 0 | 0 | 0 | 3 | 0 | 2 |
| scene::CParticleRotationAffector | 9 | 7 | 7 | 0 | 0 | 0 | 0 | 3 | 0 | 0 | 2 |
| scene::CParticleSphereEmitter | 22 | 17 | 16 | 0 | 1 | 0 | 0 | 0 | 3 | 0 | 2 |
| scene::CSceneNodeAnimatorCameraFPS | 17 | 13 | 9 | 0 | 4 | 0 | 0 | 1 | 1 | 1 | 2 |
| scene::CSceneNodeAnimatorCameraMaya | 19 | 14 | 11 | 0 | 3 | 0 | 0 | 0 | 2 | 1 | 2 |
| scene::CSceneNodeAnimatorCollisionResponse | 22 | 17 | 13 | 1 | 3 | 0 | 0 | 0 | 2 | 1 | 2 |
| scene::CSceneNodeAnimatorDelete | 8 | 4 | 2 | 0 | 2 | 0 | 0 | 1 | 0 | 1 | 3 |
| scene::CSceneNodeAnimatorFlyCircle | 10 | 7 | 3 | 1 | 3 | 0 | 0 | 0 | 0 | 1 | 2 |
| scene::CSceneNodeAnimatorFlyStraight | 11 | 7 | 4 | 0 | 3 | 0 | 0 | 2 | 0 | 1 | 3 |
| scene::CSceneNodeAnimatorFollowSpline | 9 | 6 | 3 | 1 | 2 | 0 | 0 | 1 | 0 | 1 | 2 |
| scene::CSceneNodeAnimatorRotation | 10 | 6 | 2 | 0 | 4 | 0 | 0 | 1 | 0 | 1 | 3 |
| scene::CSceneNodeAnimatorTexture | 12 | 8 | 2 | 3 | 3 | 0 | 0 | 1 | 1 | 1 | 2 |
| scene::IParticleAffector | 6 | 4 | 3 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleAnimatedMeshSceneNodeEmitter | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleAttractionAffector | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleBoxEmitter | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleCylinderEmitter | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleEmitter | 5 | 3 | 2 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleFadeOutAffector | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleGravityAffector | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleMeshEmitter | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleRingEmitter | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleRotationAffector | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::IParticleSphereEmitter | 3 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2 |
| scene::ISceneNodeAnimator | 18 | 2 | 2 | 0 | 0 | 0 | 0 | 0 | 13 | 0 | 3 |
| scene::ISceneNodeAnimatorFactory | 2 | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 1 |

## Bugs fixed

None. The checks that could have turned one up, and what they found:

- Signedness. Hex-Rays compares the CGUISkin index functions (0x54aa24 getColor, 0x54aa64 setColor, 0x54aaa0 getSize, 0x54aab4 setSize,
  0x54ab74 getIcon, 0x54ab88 setIcon) as unsigned words, and 1.5.1 writes `(u32)index < COUNT`. Our spelling `index <= LAST` relied on
  GCC making the enum unsigned. The six bodies now cast to std::uint32_t explicitly. The new test
  `glitch-scene-collada-gui/tests/gui_skin_range_test.cpp` passes both before and after, because the compiled behaviour did not change:
  this is hardening, not a bug fix.
- CStringAttribute::getBinary (0x31f324). Upstream and the binary both continue while `text[pairIndex]` is non-zero (they index by pair,
  not by character), so for `size > len/2` they read bytes past the terminator. The port stops at the first NUL of `text[2*pair]`. That
  differs only inside undefined behaviour and is kept as documented in the source.
- Everything else compared equal to the binary, including the places where the binary keeps an upstream defect or a glitch change
  (see the forks below).

## Partial rows filled

- CNumbersAttribute::setString (0x325070), previously `skipped` (about 2.9 KB of noisy Hex-Rays output). Irrlicht 1.5.1
  `CNumbersAttribute::setString` is the same algorithm: reset the list, then for each of `Count` elements skip to the next digit or `-`
  and read the number with fast_atof_move; an int list truncates. The Hex-Rays loop was matched against it, then written. Test:
  `glitch-core-io/tests/numbers_set_string_test.cpp` (float list, int truncation, skipping, clearing, null, empty list). It does not link
  against the old library (the symbol is missing) and passes with the new body.
- The other `skipped` and `partial` rows in scope (CTextureAttribute strings, CMatrixAttribute::getQuaternion,
  CQuaternionAttribute::getMatrix, CMatrix4::operator*, plane3d::getIntersectionWithLimitedLine, the GUI constructor variants, the
  CEnumAttribute family) are stale in `ledger.csv`: the sources already hold their bodies (earlier owners wrote them, with ledger parts
  not merged yet, or the C1 and C2 variants share one body). What is still really partial is deliberate: CAttributes::setGroup out of
  range, the 56-byte window of CPakReader::extractFilename, and fast_atof_move reading past its table, all undefined behaviour in the original.

## Names replaced

- `SPakFileEntry` (offsets +0x00, +0x18, +0x30, +0x48, +0x4c): `path, name, extra, offset, size` became `pakFileName, simpleFileName,
  path, pos, length`, the Irrlicht 1.5.1 names and the names SZipFileEntry already uses. `extra` was documented as use not recovered; it is
  the directory part that `extractFilename` stores. Sources and five tests updated and run.
- `CZipReader`, `CPakReader`, `CUnZipReader`: `flagA, flagB` became `ignoreCase, ignorePaths` (the 1.5.1 constructor parameter names; the
  binary offsets +0x0c/+0x0d and +0x24/+0x25 agree with the lower-casing and base-name uses). pak_find_test and the CFileSystem test updated.
- `CParticleMeshEmitter::unk_28` and `CParticleAnimatedMeshSceneNodeEmitter::unk_30` became `std::vector<SParticle> particles` (upstream
  `Particles`: the slot between `NormalDirectionModifier` and `Direction`, freed by the destructor; emitt is a stub in the binary).
- Left alone: `IReadFile::unk_08`, `CAttributes::unk_4c..54` and `CZipReader::unk_10` have no upstream counterpart (glitch additions).
  The Hex-Rays-style locals in quaternion.cpp (`v10`, `v22`), CMatrix4::getInverse and the xml_reader parse templates were not renamed:
  quaternion.cpp is being edited by another owner, and the others do not map onto upstream names.
- `glitch-core-io/tests/r1-glitch-core-io-glitch_core-6498de-0_test.cpp` did not compile before this pass (SZipFileEntry::name had been
  renamed by another owner); its zip lines now use `simpleFileName`.

## Notable forks (the binary differs from 1.5.1, ours follows the binary)

- CParticleGravityAffector::affect (0x6fac14): the blend weights are inverted. vector = start*t + gravity*(1-t), t the clamped age
  fraction; 1.5.1 gives gravity*t + start*(1-t) (a particle starts at the gravity vector and ends on its own).
- CParticleRotationAffector constructor (0x6fcd74): argument order (pivot, speed); 1.5.1 takes (speed, pivot).
- CParticleMeshEmitter and CParticleAnimatedMeshSceneNodeEmitter::emitt: stubs returning 0 (1.5.1 emits from the mesh).
- CSceneNodeAnimatorCameraFPS::animateNode (0x6c7f1c) and CameraMaya::animateNode: the older camera animators (no isInputReceiverEnabled or
  active-camera test, yaw accumulated with +=).
- CGUIEditBox::getCursorPos (0x6afd18): clamps with `y >= bottom` and measures the line it found (1.5.1 clamps with `>` and measures `Text`;
  measuring the line is what 1.7.3 does).
- CGUIEnvironment::getNextElement (0x537b44) and loadBuiltInFont (0x539ef0, which also saves and restores a driver option);
  CGUIFont::load (0x53fbd4: the font XML uses the short names T, f, i, h, c, u, o, r) and getDimension(const char*) (0x53dbfc, a stub
  returning 0x0); the default texts of CGUISkin, CGUIWindow and CGUIColorSelectDialog (single letters instead of OK, Cancel, ...);
  CGUIColorSelectDialog::buildColorRing (alpha in the low byte of the format-12 pixel).
- line2d::intersectWith (0x525310): the older cross-product test with a 1e-6 epsilon, no coincident-line case.
- Revision hints. fast_atof_move matches 1.4.2 (earlier finding); CGUIColorSelectDialog::draw, quaternion::normalize and fromAngleAxis are
  nearer to 1.4.2 than to 1.5.1; CFileList::getFileName and CGUISkin::drawIcon are nearer to 1.6.1 and 1.7.3 only because glitch dropped
  checks. Over the 1,110 methods scored against all five releases, 1.5.0 and 1.5.1 average 0.724, against 0.718 for 1.4.2, 0.711 for 1.6.1
  and 0.703 for 1.7.3. The earlier 1.5-era conclusion stands; no method pins an exact revision.

## Not done

- Files other owners were editing during the pass (mtime inside ten minutes): glitch-core-io quaternion, xml_writer, xml_attributes,
  iattributes and device, and shared/include/glitch-shared/glitch_core.hpp. They were read, not edited. The zip reader was being edited by
  another owner when the flag rename went in (zip_reader.cpp and .hpp compile, check_tu OK); that owner should re-read it.
- Video, collada and ps were skipped as instructed; pydata files were not touched.
- Ledger: `ledger.d/ou-1.csv` in glitch-core-io (setString) and in glitch-scene-collada-gui (the six CGUISkin rows). merge_ledger.py was not run.

## Files touched

Under port/decomp-reconstruction:

- glitch-core-io: include/glitch-core-io/io/{numbers_attribute,pak_reader,zip_reader,unzip_reader}.hpp; src/{numbers_attribute,pak_reader,zip_reader,unzip_reader}.cpp;
  tests/numbers_set_string_test.cpp (new), tests/{pak_reader_test,pak_find_test,r1-glitch-core-io-pack1-a5bb77-0_test,r1-glitch-core-io-glitch_core-6498de-0_test,r1-glitch-core-io-glitch_io_CFileSystem-48a7aa-0_test}.cpp; ledger.d/ou-1.csv
- glitch-scene-collada-gui: include/glitch-scene-collada-gui/particle_emitters.hpp; src/gui/CGUISkin.cpp; tests/gui_skin_range_test.cpp (new); ledger.d/ou-1.csv

Under reports/decomp-reconstruction: irrlicht-verify-2026-10-16.md, irrlicht-verify-2026-10-16.csv, trackb-progress/owners-2026-10-15.md (one line appended).
Tests were run from build/own-u (build/own-u/run-test.sh links the new objects ahead of the tb-int archives): numbers_set_string_test,
gui_skin_range_test, pak_reader_test, pak_find_test, a5bb77, 6498de and 48a7aa all pass. check_tu.sh prints OK for every touched file.
