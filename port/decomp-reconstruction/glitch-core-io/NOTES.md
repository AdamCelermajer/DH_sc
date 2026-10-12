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

## Remaining
- About 1,660 queued functions are still open. Next candidates: CAttributes
  getters for group and stack handling (pop, setGroup), CNumbersAttribute
  vector/rect/matrix getters and setters, CReadFile/CWriteFile simple accessors,
  and the heap/binary-search templates once the comparators are checked.
