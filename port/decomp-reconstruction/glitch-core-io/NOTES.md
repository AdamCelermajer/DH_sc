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
- Group handling in CAttributes is modelled in wave 3 (see below).

## Wave 3 (this session, 200 new ledger rows)
Status of the session's rows: 166 reconstructed, 26 skipped, 8 partial.
Layouts confirmed from the ARM listings of the group, add and accessor code:
- `IAttribute`: refcounted; `name` (std::string, c_str read at +0x1c); flag byte at +0x20
  (constructor argument, not read by the io code); CBool value +0x21; CInt, CFloat and
  CUserPointer value +0x24.
- `CAttributes`: refcounted; root `SContext` embedded at +0x08; +0x44 current group;
  +0x48 pointer to the current group's attribute vector; +0x58 driver (refcounted,
  may be null in tests). `SContext`: name +0x08, attribute vector +0x20, child vector
  +0x2c, parent +0x38. `push(name)` / `push(int)` (named "group<n>") / `push(name, int)`
  create or find a child via `getContext(create)`; `pop` returns to the parent.
- `CNumbersAttribute`: int list +0x24, float list +0x30, count +0x3c, isFloatList +0x40.
  Typed subclasses (CRect, CVector2D/3D/4D, CVector2DI/3DI/4DI, CPosition2D, CMatrix,
  CQuaternion, CBBox, CPlane, CTriangle, CLine2d, CLine3d, CColorf, CColor) share
  the layout; the typed ctors push components in declaration order.
- `CMemoryReadFile`: refcount +0x04, buffer +0x0c, owner +0x10 (shared_ptr<char>),
  size +0x18, position +0x1c, name +0x20.
- `CFileList`: path string +0x08, entry vector +0x20..+0x28; `FileEntry` is 0x38 bytes:
  name +0x00, full name +0x18, id +0x30, isDirectory +0x34.
- Colour words: SColor packs component 0 in the low byte, then 1, 2, and alpha in the
  top byte (getColor and setColor agree; confirmed from the ARM).

Decisions:
- Glitch strings are std::string. Entry names are read through c_str().
- Refcounting follows `glitch::IReferenceCounted` (deps); the default count is 1.
- Attribute containers own their entries: `clear()` deletes them, so tests allocate
  attributes with `new` and never put stack objects in a container.
- By-name setters: an existing attribute gets the virtual setter; otherwise the
  attribute is added with flag false (confirmed for SColor, SColorf and the numbers).
- Float-list colour from bytes multiplies by (1/255) like the original, not by dividing.
- Compiler-generated destructors and GCC `.clone` copies of constructors are `skipped`.
- Identity default matrix comes from `detail::identityMatrix()`.
- IAttribute base `getTypeString()` returns L"unknown" (the base default is not
  recovered). The type names of CBool and CInt are still left out because their
  literal addresses do not resolve.

Partial rows (8):
- `CAttributes::pop` and `setGroup`: out-of-range and root-pop cases differ from the
  original (see the ledger notes).
- `IAttribute::getString`, `IAttribute::setString(wchar_t)`: the wide/narrow helper is
  not resolved; characters are truncated.
- `CNumbersAttribute::setMatrix`: the stray zero byte stored past the argument is not
  reproduced.
- `getAttributeTypeString` (int and name): base type-name default assumed.
- `addStringAsColor`: the listing builds a zero colour and never reads the string.

## Remaining
- String-argument families in CAttributes: addStringAs* (except addStringAsColor),
  addString(char/wchar), setAttribute(char, wchar), getString/getStringW of numbers.
  Their listings end in noreturn strlen tails, so the parse helper is unresolved.
- CStringAttribute, CEnumAttribute, CBinaryAttribute, CTextureAttribute,
  CLightAttribute and the enum choices variants (addEnum/getEnum/setAttribute with
  choices). Texture and light need ITexture/CLight stand-ins and boost::intrusive_ptr.
- CNumbersAttribute::getString/getStringW/setString(char) (352-428 bytes): format
  helpers not resolved.
- CMatrixAttribute::getQuaternion, CFloatAttribute::getStringW (format unknown).
- CXMLReaderImpl<char/wchar_t> (accessors are trivial but the class layout is not
  inferred), CXMLWriter, CXMLAttributesReader/Writer (large read/write bodies).
- CFileList::getFullFileName (lazy cache) and heapsink/heapsort<FileEntry>; the
  comparator orders directories by flag first and then names with strcasecmp (not
  yet verified against the ARM).
- CMemoryReadFile read/seek/getBuffer virtuals, CLimitReadFile (9 functions),
  CZipReader, CZipWriter, CUnZipReader, CPakReader, CFileSystem, SIDedCollection
  (about 60 instantiations).
- IAttributeExchangingObject and the serializeAttributes/deserializeAttributes
  virtuals of other categories' classes.

## Wave 4 (200 new ledger rows: 115 reconstructed, 31 partial, 54 skipped)

Classes whose queued entries are all ledgered after this wave: IAttributeExchangingObject,
IAttributes, IAttributesReader/Writer, IFileSystem, IFileReadCallBack, IWriteFile, IReadFile,
IEventReceiver, IAttribute (64 of 64), CBinaryAttribute, CColorfAttribute, CPlaneAttribute,
CPosition2DAttribute, CStringWArrayAttribute (constructor partial), the CVector2D/2DI/3D/3DI/4DI
getMatrix overrides, SPakFileEntry (default constructor partial), glitch::os::Timer,
boost::detail::sp_counted_base, sp_counted_impl_p<char>, shared_count, shared_ptr<char>,
intrusive_ptr<ITexture>, and the heapsink/heapsort/binary_search instantiations for the
GUI and scene element types (plus heapsink/heapsort for CFileList, partial).
Not complete: CXMLReaderImpl<char/wchar_t> (accessors only), CStringAttribute (float
conversions partial), CAttributes (string families), CPakReader, CTextureAttribute,
CLightAttribute, CColorAttribute, CEnumAttribute, CMatrix4 and aabbox3d.

Layouts confirmed from the ARM listings:
- `CXMLReaderImpl<char>` attribute records are 0x30 bytes (name at +0x14, value at +0x2c,
  both std::string here); `CXMLReaderImpl<wchar_t>` records are 0x90 bytes. The two are
  separate explicit specialisations. The parser step (vtable +0x0c) is in derived readers.
- Colour packing: CColorAttribute and CColorfAttribute share the integer view. getInt returns
  the packed colour rotated left by one byte; setInt writes four floats byte/255.
- Timer: StaticTime, StartRealTime, LastVirtualTime and TickCount are static members, shared
  by all Timer objects. VirtualTimerSpeed's initial value is still open (see Open questions).
- boost shared count: sp_counted_base has the vtable first, use count at +0x04, weak count
  at +0x08; sp_counted_impl_p<T> holds the pointer at +0x0c. The port uses std::atomic where the
  original uses a spinlock pool (same results, not the same instructions).
- Intrusive policy: boost::intrusive_ptr adds a use on copy and drops the old object on
  assignment. Overloads are found by ADL. ITexture, IVideoDriver and CLight derive from
  IReferenceCounted (count at +0x04, drop()). CMaterial and CMaterialRenderer keep their count
  at +0x00 and are released by a direct destructor call; the port deletes them (CustomFree not
  modelled). These are in deps/glitch_core.hpp.
- fast_atof_move (0x322bc8, partial): the 16-entry scale table is Irrlicht's fast_atof_table.
  The glitch table is 16 floats in .rodata and was not read from the binary. Longer fractions
  use pow(10, -n).
- CStringAttribute: narrow std::string at +0x24, wide std::wstring at +0x3c, isWide flag at
  +0x21 (set in the constructor, which then calls setString(wide)). stringc2stringw sign-extends
  bytes (ldrsb). stringw2stringc is partial: its helper (0x3209f8, a wide range copy) was not read.
- CTextureAttribute: texture at +0x24, driver at +0x28 (referenced in the constructor, released
  in the destructor). The constructor does not store the texture argument (as decompiled), so
  the texture starts null (partial).
- CPakReader: vtable +0x00, refcount +0x04 (set to 1), readFile (IReadFile*) at +0x08, entry
  vector +0x18..+0x20, flags at +0x24 (name lower-casing, used by extractFilename) and +0x25.
  SPakFileEntry is 0x50 bytes: path string +0x00, name string +0x18 (passed to
  createLimitReadFile by openFile), third string +0x30 (use not recovered), offset +0x48
  (seek target, relative flag 0) and size +0x4c (limit size). openFile(int) seeks readFile to the
  offset and wraps it in a limit read file.
- Process buffer heap (object of 20 bytes in .bss): start +0x00, bound +0x04, top +0x08, excess
  flag +0x10. The accessors read it; setSize and the allocator that move these values are not
  reconstructed, so the heap stays empty in the port (getProcessBufferHeapSize is 0). Partial
  for the two size accessors because the bound is written by code not yet read.
- CEnumAttribute: its +0x24 value is inherited from CIntAttribute in the original, and the enum
  index is at +0x38. The port's CIntAttribute layout does not match the vtable calls used by the
  enum family, so the enum family stays unreconstructed (layout conflict).

Decisions:
- SPakFileEntry and the attribute classes keep std::string members with the original offsets as
  comments. Structs that only release their members (compiler-generated destructors) are
  skipped.
- Type-name literals (getTypeString) that are not in strings.csv are reconstructed as partial
  with the name inferred from the class: position, plane, binary, texture, light, stringwarray.
  stringwarray is a guess. These need the wide literal read from the binary.
- Test helpers: CPakReader has a default constructor for tests (its real constructor
  0x57002c/0x5700c4 calls scanLocalHeader and the sort, not reconstructed).

Partial rows (31 in this wave); the reasons are in the ledger notes column. The common causes
are: the glitch string and allocator are std::string here, the vtable slot numbers of stand-in
classes, and tail calls hidden behind strlen in the pseudocode.

## Remaining (after wave 4; 1167 queue entries not in the ledger)
- CPakReader: constructors C1/C2 (0x57002c, 0x5700c4, need scanLocalHeader), extractFilename
  (0x56fcec), scanLocalHeader (0x56fe44), findFile (0x5702b4), openFile(int) (0x56f784) and
  openFile(const char*) (0x5703b8). heapsink/heapsort/binary_search<SPakFileEntry> (0x56fa04,
  0x56fc0c, 0x57015c) need the element compare, which is the path string.
- CStringAttribute: getBinary/setBinary (hex loop garbled). CStringAttribute::getFloat and setFloat
  are partial (they go through fast_atof_move and ostringstream). CFloatAttribute::getStringW is
  partial (its format is not confirmed).
- CAttributes: addStringAs* family (constructor and value handling hidden behind tail calls).
- CEnumAttribute family: layout conflict described above.
- CXMLReaderImpl: 12 (char) and 14 (wchar_t) queue entries are still unledgered. The parser
  step (read, vtable +0x0c) is implemented by derived readers.
- CFileSystem, CZipReader, CLimitReadFile, CXMLWriter, CFile: classes not in the port yet.
- Math: CMatrix4Base::mult (304 bytes, needed for operator*), plane3d::setPlane (3-point form;
  the 44-byte queue size does not match the ARM), isIdentity, getScale, getAngle,
  transformPlane, getIntersectionWithLimitedLine, buildShadowMatrix (garbled tail calls).
- Strings: wcsicmp, getUTF8charValue, iterateUTF8String (garbled); getTypeString for bool, int,
  rect, bbox, line2d, line3d and enum (literal addresses not in strings.csv).
- CFileList heapsink/heapsort are partial (element compare not verified).

## Wave 5 (202 new ledger rows: 155 reconstructed, 45 partial, 2 skipped)

Rows by family:
- `CAttributes::addStringAs*` and `addString(name, wide value)` (22 rows, `src/attributes_string_as.cpp`).
  The pseudocode for this family garbles the tail: the decompiler reads the `strlen` call as a
  noreturn tail, so the body looked like it only allocates. The ARM (`bl` to 0x30de54 is strlen,
  then the name is copied and the object is pushed) shows the real order: the typed attribute is
  built with its default value and the flag, appended to the current group (vector at group +0x48),
  then `setString(wide)` is called through IAttribute's vtable slot +0x94 on the appended element.
  Defaults come from the pseudocode stores: line2d (0,0)-(1,1), line3d (0,0,0)-(1,1,1), plane
  normal (0,1,0) with D = -0.0, box (-1,-1,-1)-(1,1,1), quaternion (0,0,0,1), colour (0,0,0,1),
  matrix identity with the byte at +0x40 set to 1. `addStringAsTexture` is partial (texture
  constructor), `addStringAsBool/Float/UserPointer` are partial (the value field is never written
  by the original). `addString(char, char)` (0x56a818) and the narrow CStringAttribute constructor
  (0x567d60) are not done; the Light and Binary variants (0x568124, 0x56840c) need the
  CLightAttribute constructor (0x566c90) and a CBinaryAttribute constructor, not in the port yet.
- `glitch::CTimer` (9 rows, `os/ctimer.hpp`, `src/ctimer.cpp`). Each queued body is a 4- or 8-byte
  branch. The targets come from the ARM listing (the listing address equals the ELF address; the
  pseudocode header adds 0x10000): tick -> os::Timer::tick (0x60b22c), isStopped (0x60aebc),
  getSpeed (0x60ae9c), setSpeed (0x60b194), startTimer (0x60b1dc), stopTimer (0x60af68), setTime
  (0x60b148), getTime (0x60aee4), getRealTime (0x60b0cc). The two destructors are skipped.
- `boost::intrusive_ptr` instances (10 rows). The template gained `operator=(T*)` for CImage
  (0x603414). Stand-ins were added to deps for IShader, IShaderCode, IBuffer, CImage,
  scene::CMeshBuffer (drop() at +0x04) and CVertexStreams (count at +0x00, direct delete, like
  CMaterial). The CLight destructors and operator= (0x5aa3c8, 0x5b9118, 0x5bb650) and
  intrusive_ptr<CVertexStreams>/<const CMaterialRenderer> (0x5a10fc, 0x65afc0) are partial: the
  original releases them with an inline Matrix4 pool return or a direct destructor call.
- `glitch::core::detail::SIDedCollection` (159 rows over five instances, `core/sided_collection.hpp`).
  The address map is at the top of the header. Layout (32-bit offsets, kept as comments):
  name map header at +0x00..+0x10 (count at +0x10), `vector<SEntry>` at +0x18..+0x20, next free
  id (uint16) at +0x24, live count (uint16) at +0x26. SEntry is the value plus its map iterator:
  8 bytes for a pointer value (ITexture, IShader, IShaderCode, CMaterialRenderer) and 20 bytes for
  SShaderParameterDef (16-byte value, iterator at +0x10). The name map node is 0x38 bytes: the
  key SName at +0x10 (name +0x00, managed +0x04) and the mapped SIdValue at +0x18 (id at +0x1c,
  inside the value, so the id is at node +0x34). The port uses std::map and std::vector with the
  same key order (strcmp), so idBegin/idEnd/removeAll visit names in the original order.
  Decisions: `remove(id, force)` only removes a value whose count (+0x04) is 1 unless forced;
  `remove` lowers the next free id and cuts trailing unset entries from the vector. `add` is a
  test helper (not a queued function). SShaderParameterDef values hold a shared-string
  reference at +0x00 that the port copies without counting, so its SEntry set/reset/copy and
  destructor are partial.
  Not done: the four `rename(unsigned short, char const*, bool)` entries (0x5bb3cc, 0x5dc444,
  0x5e7050, 0x6e1d88): their pseudocode was not read this wave.
- Skipped (compiler-generated): the CTimer destructors (0x6714a4, 0x6715fc).

Partial rows are listed in the ledger notes. The main causes: the CMatrix4 and CNumbers
copies that drop the flag byte, the CLight pool release, and the SShaderParameterDef
shared-string count.

Checked and not reconstructed this wave (reasons):
- `IAttributes::getEnum<E>` and `addEnum<E>` templates (0x5a03cc, 0x5e09cc, 0x5ba66c, ...):
  they call vtable slots +0xf4, +0x100 and +0x104 of IAttributes and `getStringsInternal` (the
  glitch::video enum string tables). The port's IAttributes interface has no virtuals yet.
- `CMatrix4<float>::isIdentity`, `getScale`, `transformPlane`, `buildShadowMatrix`: the
  pseudocode tail-calls `__aeabi_fadd`/`__aeabi_fmul` with garbled arguments. `operator*` needs
  `CMatrix4Base::mult` (304 bytes, not reconstructed).
- `CMatrix4<float>::CMatrix4(eConstructor)` (0x631c14, 0x6c0ce4): GCC `.clone` copies, so they are
  skipped; the constructor body is identity with the flag byte at +0x40 set to 1 (the same
  matrix as addStringAsMatrix uses). Its base constructor is not in the queue as a small body.
- `CFileSystem`, `CZipReader`, `CUnZipReader`, `CZipWriter`, `CXMLWriter`, `CXMLReaderImpl`
  text/parse helpers, `CLimitReadFile`, `CFile`, the process buffer heap, `Printer::logf` and
  `Randomizer`: queued, but not started in this wave. `Printer::logf` forwards a variadic call to
  a logger virtual (vtable +0x24) that the port's ILogger does not declare.

## Wave 6 (200 new ledger rows: 83 reconstructed, 93 partial, 24 skipped)

Count: 1047 ledger rows; 847 in the wave-5 snapshot (52bbcf12), so 200 are new this wave.
Build: host `42 of 42` and Windows `42 of 42` (ctest 100%).

Class layouts (offsets from the original; std members stand in for glitch strings and vectors):
- `CLimitReadFile` (`io/limit_read_file.hpp`): +0x0c name, +0x24 full name, +0x3c size, +0x40 start,
  +0x44 end, +0x48 wrapped file (cloned in the constructor, dropped in the destructor), +0x4c position.
- `CReadFile` (`io/read_file.hpp`, the port's stand-in for CFile + CReadFile): +0x0c FILE*, +0x10 size,
  +0x14 name, +0x2c obfuscation flag (the first four bytes are adjusted by ~pos - i).
- `CZipReader` (`io/zip_reader.hpp`): +0x08 archive file, +0x0c flagA, +0x0d flagB (both from the
  constructor; the ARM of findFile tests them), +0x14 entry vector. `SZipFileEntry` is 0x6c bytes:
  path +0x00, name +0x18, extra +0x30; the ARM of extractFilename also reads a short at +0x66.
- `CPakReader` (`io/pak_reader.hpp`): +0x08 archive file, +0x18 entry vector, +0x24/+0x25 flags.
  `SPakFileEntry` is 0x50 bytes: path +0x00, name +0x18, extra +0x30, offset +0x48, size +0x4c.
- `CZipWriter` (`io/zip_writer.hpp`): +0x08 write file, +0x0c flag, +0x24 names vector. The CRC-32
  table is `boost/crc_table.hpp` (polynomial 0xEDB88320, built by init_table at 0x6b50ec).
- `CXMLWriter` (`io/xml_writer.hpp`): +0x08 write file, +0x0c indent depth, +0x10 flag.
  `CXMLAttributesReader` / `CXMLAttributesWriter` (`io/xml_attributes.hpp`): +0x04 reader or writer,
  +0x08 flag, +0x0c element name (wide).
- `CFileSystem` (`io/file_system.hpp`): archive containers from +0x08 are not modelled (see blockers).
- `CUnZipReader` (`io/unzip_reader.hpp`): derives from CZipReader; the file system at +0x20 and the
  folder name (inline buffer at +0x24, pointers at +0x34/+0x38) are not referenced by the port.
- `IDevice` (`os/device.hpp`): the event queue from the ARM of run: begin pointer +0xb4, storage +0xb8,
  storage end +0xbc, end +0xc4; events are 0x18 bytes (six words). Not implemented (see blockers).
- `CProcessBufferHeap` (`core/process_buffer.hpp`): +0x00 start, +0x04 end, +0x08 top, +0x0c default
  size, +0x10 excess flag. Blocks are boundary tags of 32-bit words (positive allocated, negative free).
- `glitch::os::Randomizer` (`os/randomizer.hpp`): one static seed; reset gives 0x0f0f0f0f; the step is
  Schrage's method for multiplier 40692 modulo 0x7fffff07.
- `CMatrix4<float>` stand-in (`deps/glitch_core.hpp`): M[16] row-major at +0x00, flag at +0x40.

Decisions:
- `CStringAttribute` narrow constructor (0x567d60) keeps the name and flag and clears isWide; the
  value argument is never read by the decompile, so it is dropped (partial). `addString(name,
  const char*)` (0x56a818) therefore also drops its value.
- Null inputs where the original faults: `CEnumAttribute::setString` and `addString` take a null
  value as empty; `getEnum` with a null choice list gives -1.
- `glitch::core::wcsicmp` is qualified in the tests (`glitch::core::wcsicmp`): the CRT declares a
  global `wcsicmp` on Windows, so an unqualified call is ambiguous there.
- `randomString` (0x58bad8) draws `lrand48() % 62`. lrand48 is emulated with the drand48 step from
  the C library's default state, so the port does not depend on the platform's lrand48, and a seed the
  game sets with srand48 is not reflected.
- `CVector4DAttribute::getMatrix` (0x3276f8) reads the first four floats of the float storage as the
  original does, without the list-kind or size check (missing entries read as zero).
- `CAttributes::getEnum(name|index, choices)` compares the attribute's getString(): the original calls
  vtable +0x80 for the text, but the port labels +0x80 as getEnum (an int), so the slot is not
  reused.
- Colour packing in `CColorAttribute::setString` (0x32324c) follows the ARM: the first hex field is
  the top byte, then the fourth, third and second; the result goes through setColor (vtable +0xa0).

New partial rows and their causes (the ledger notes say the same per row):
- Literal names (wide strings not in strings.csv, and the literal pool address cannot be read: the
  ELF is not in the container; recovered/native/debug holds DWARF JSON only): getTypeString of
  CBoolAttribute, CIntAttribute, CEnumAttribute, CRectAttribute, CBBoxAttribute, CLine2dAttribute,
  CLine3dAttribute. The names bool/int/enum/rect/bbox/line2d/line3d are inferred from the class.
- Missing layouts: CEnumAttribute choice table (+0x3c, 24-byte entries), CLightAttribute and
  CTextureAttribute held references (the constructors keep the driver only, the light and texture
  arguments are not stored by the decompile), CStringWArrayAttribute constructor (array not kept).
- Dependencies still partial: stringw2stringc (keeps the low byte of each character) and
  fast_atof_move (used by the wide float getters), CZipReader::scanLocalHeader (no entries),
  deletePathFromFilename (inferred from the tail call), operator< on pak and zip entries (names only).
- Tests: `WideReader` has no read() in the port, so the wide XML float getters compile but are not
  exercised by a test.

Remaining (not reconstructed, with the reason):
- `CFileSystem::createXMLReader` / `createXMLReaderUTF8` / `createIXMLReader` (0x56c230..0x576270):
  need the IIrrXMLReader interface, which the port does not have.
- `CFileSystem` archive functions (addPakFileArchive, addFolderFileArchive, addZipFileArchive,
  createAndOpenFileFromArchives) and getObfuscatedFileName (0x56e064): need the archive containers and
  the ARM's several branches (not recovered).
- `CZipReader::openFile(int)` (0x576f18, 632 bytes) and `CUnzipReadFile` (ctor, dtor, getFileName):
  the unzip read-file class is not modelled. openFile(name) tail-calls openFile(int).
- `CZipWriter::close` (0x6b4fec) and `addNewFile` (0x6b581c): writer internals.
- `IDevice::run`, `postEventFromUser`, `postMouseEventFromUser`, `checkVersion` (0x671b24, 0x671a00,
  0x671aec, 0x671e84): the event queue above is not in the port's device.hpp.
- `CXMLAttributesWriter::write` (0x5723f8): needs writeGroup and the writeElement overloads (IXMLWriter
  +0x10 and +0x14 are not modelled). `CXMLReaderImpl` parse/read/convertTextData: the text buffer
  (+0x08 buffer, +0x10 begin, +0x14 count) is not modelled.
- `CMatrix4` mult, operator*, setbyproduct (needs setbyproduct_nocheck), mult34, transformPlane,
  getScale, buildShadowMatrix: the pseudocode tail-calls with garbled arguments; the ARM listing for
  getScale runs past its declared size. `CMatrixAttribute::getQuaternion` and
  `CQuaternionAttribute::getMatrix` need the quaternion conversions, which the port does not have.
- Texture string conversions (`CTextureAttribute::getString`, `getStringW`, `getString(char*)`,
  `setString`): toString and the driver texture lookup need the video driver interface.
- Binary attributes (`CStringAttribute::getBinary/setBinary`, `CAttributes::addBinary`,
  `addStringAsBinary`, `setAttribute(name, void*, int)`, the CStringAttribute(void*) constructor): the
  CBinaryAttribute data layout is not recovered.
- `CEnumAttribute::getInt`, `setInt`, `setEnum`, `getStringW`: they need the choice table, which is not
  in the layout (getStringW reads +0x34 and +0x38, which do not match the narrow text at +0x24).
- `CXMLWriter::writeElement` (0x576758, 0x576988), `CXMLAttributesReader::read` (0x571920): large,
  and they depend on the unmodelled writer and reader internals above.
- Shared-string heap (`SSharedString::operator=`, `SSharedStringHeapEntry::commit`,
  `registerSharedStringHeapInit`) and the glitch::core copyComponent / quantizeScaleOffset templates:
  not in the port, and the pseudocode is garbled.
- `IAttributes::getEnum<E>` / `addEnum<E>` and the `SIsValidSetMaterialParamaterOverload` enable_if
  templates belong to glitch::video material code outside this category's files.
