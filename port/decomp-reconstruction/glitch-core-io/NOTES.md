# glitch-core-io notes

## Layouts inferred (from the pseudocode offsets)
- `glitch::io::IAttribute` interface: vtable slots from `decomp_vtable.py`
  (getInt +0x0c, getFloat +0x10, getColorf +0x14, getArray +0x24, getBool +0x28,
  getBinary +0x2c, getVector3d +0x30 ... setVector2d +0xac, setColor(SColorf) +0x9c,
  setColor(SColor) +0xa0, setArray +0x98, setEnum +0xf8). Member `name` at +0x1c.
  The Irrlicht-style `getType` virtual sits at +0x104.
- `glitch::io::CAttributes`: +0x48 is a vector-like list of `IAttribute*`. Setters
  check `index < count`; getters return a default when `count <= index`. Negative
  indices count as out of range (the original compares unsigned).
- `glitch::io::CNumbersAttribute`: +0x24 int storage, +0x30 float storage,
  +0x3c element count, +0x40 is-float flag. getType 0x18 (intlist) / 0x17 (floatlist).
- Scalar attributes: CBool value +0x21 (char), CInt value +0x24, CFloat value +0x24,
  CEnum enum +0x38 (derives from CInt), CUserPointer pointer +0x24.
- Typed attributes: Color, Colorf, Vector2D/3D/4D derive from CNumbersAttribute
  (their D1 destructors call the CNumbersAttribute base destructor). The
  Vector*I, Position, Rect, Matrix, Quaternion, BBox, Plane, Triangle, Line and
  Texture/Light/StringW-array bases are assumed to be IAttribute (not confirmed).
- `glitch::io::CMemoryWriteFile`: +0x04 refcount, +0x08 position, +0x0c..+0x14 the
  byte vector. The ctor reserves the given capacity.
- `glitch::core::vector2d/3d/4d`, `position2d`, `rect`, `SColor`, `SColorf`:
  plain members (deps header only, not the real glitch headers).

## Decisions
- Indexed getters and setters use `at()` with a signed bounds check, matching the
  original unsigned compare.
- Type-name literals were used only where the string resolved in the binary
  (for example "colorf", "vector2d", "string", "triangle"). `getTypeString` for
  CBool, CInt, CEnum, CBBox, CLine3d, CPosition and others was left out because
  the literal addresses (DAT_008ce9e8, DAT_008cea00, DAT_008eeea0, DAT_008eee08,
  DAT_008eee80) do not resolve in strings.csv.
- Compiler-generated destructors are ledgered as `skipped` (README rule), not
  reconstructed.
- Boost intrusive_ptr returns (getLight, getTexture, setTexture, setLight) are
  not reconstructed; they need a boost stand-in first.

## Partial rows (2)
- `CMemoryWriteFile::seek(long, bool)`: pseudocode only has the relative branch.
  The absolute case is not confirmed.
- `CMemoryWriteFile::write`: pseudocode never advances the position after the
  copy. Kept as shown; the position in the original may be updated elsewhere.

## Open questions
- `glitch::os::Timer::VirtualTimerSpeed` initial value (0.f placeholder).
- `Printer::Logger` vtable order is assumed from the slot offsets only.
- `glitch::core::vector2d<float>::getAngle`, `plane3d::setPlane`,
  `plane3d::getIntersectionWithLimitedLine`, `triangle3d::getIntersectionWithLineSegment`:
  noreturn tail-call noise in the pseudocode; not in the ledger.
- `CMatrix4::isIdentity` / `getScale` / `mult34` / `setbyproduct`: the pseudocode
  tail-calls unnamed helpers (for example an epsilon compare). Not reconstructed.
- `heapsort` / `heapsink` / `binary_search` instantiations (SPakFileEntry,
  SZipFileEntry, CFileList::FileEntry, CMeshCache::MeshEntry, ...): the comparator
  and the 1-based indexing need a careful read before a template is written.
- `SIDedCollection` template (about 60 instantiations): container layout not
  inferred yet.
- `CFileSystem`, `CZipReader`, `CPakReader`, `CXMLReaderImpl`, `CXMLWriter`,
  `CReadFile`/`CWriteFile`/`CLimitReadFile` bodies beyond the simple getters.
- CStringAttribute and CFloatAttribute setString need `glitch::core::fast_atof_move`
  and `int2stringw`, which are outside this category's queue so far.

## Session 2 additions
- `glitch::IReferenceCounted` (deps header): `referenceCounter` is `mutable` because
  `drop()` is const in the original (`_ZNK...4dropEv`). `drop()` lives in
  `src/reference_counted.cpp`; `onDelete()` is a virtual hook called before delete.
- `IAttribute` base defaults now ledgered (34 rows). Corrections from the pseudocode:
  base `getColorf()` returns opaque white (all channels 1.0, not black with alpha 1).
  Added `getColor()` (0xffffffff), `getUserPointer()` (null), `setRect()`.
- Base defaults still not written (need deps types that do not exist yet):
  getQuaternion (+0x80 area), getTriangle, getLine2d/2di/3d/3di, getDimension2d,
  getBBox, getPlane, getTexture, getLight, getMatrix, getString/getStringW, and the
  matching setters for quaternion, matrix, triangle, line, dimension2d, bbox, plane,
  texture and light. Base getPlane default is normal (0,1,0) with d = -0.0f.
- `getBBox` default is the box (-1,-1,-1)..(1,1,1); `getLine3d` default is (0,0,0)-(1,1,1).

## Session 3 additions
- deps types (modelled on Irrlicht 1.8.5): `quaternion`, `triangle3d<T>`, `line2d<T>`,
  `line3d<T>`, `dimension2d<T>`, `aabbox3d<T>`, `plane3d<T>`, `CMatrix4<T>` (16 values).
- IAttribute base getters/setters for quaternion, triangle, line2d/2di/3d/3di,
  dimension2d, bbox, plane are ledgered. Setters for these and for setString(wchar_t)
  are no-ops in the base, as in the original.
- CAttributes indexed getters/setters for color, quaternion, box3d, plane3d,
  triangle3d, line2d, line3d, user pointer, rect, and the wchar string setter.
  Their vtable slots map to the IAttribute virtuals above. The indexed quaternion
  default is (0,1,0,0), which differs from the IAttribute default (0,0,0,1).
- CReadFile / CWriteFile: the original keeps a shared `CFile` (ISharedObject) opened by
  `CFileSystem::open`. These classes own a `std::FILE*` directly instead, because
  CFileSystem is not reconstructed. Consequences: full path == name as given; the
  obfuscation flag from the shared CFile is not read (ctor argument kept).
  `CReadFile::read` applies the original's first-4-bytes (~pos - i) byte shift.
- Partial: `getPos` for both (pseudocode drops ftell's result), `CReadFile::openFile`,
  `CWriteFile::openFile` (mode inferred from the bool), `CWriteFile` ctor (bool unused).
- Not started: CFileList FileEntry internals (SGI-style string members at +0x10..+0x2c
  need the glitch string layout first), CLimitReadFile, CZipReader, CPakReader,
  CFileSystem bodies, CXMLWriter write* functions (need IWriteFile).
- Group handling in CAttributes (`setGroup`, `getGroupsCount`, `getGroupName`) uses
  a group object at +0x44 with its own attribute vector at +0x20. Not modelled yet
  (the header keeps one flat attribute list).

## Remaining
- About 1,630 queued functions are still open. Next candidates: CAttributes
  getters for group and stack handling (pop, setGroup), CNumbersAttribute
  vector/rect/matrix getters and setters, CReadFile/CWriteFile simple accessors,
  and the heap/binary-search templates once the comparators are checked.
