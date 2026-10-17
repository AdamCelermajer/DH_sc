# Zone contact projection

This host module contains cache-backed `Zone::InitPost` local-box arithmetic
and a bounded projection of both recovered `Zone::IsInside(GameObject*)`
branches. It does not implement actor/wall collision, trigger edge handling,
or movement integration.

## Local zone box

`make_zone_local_aabb()` mirrors `Zone::InitPost()` in
`recovered/native/assembly/libDungeonHunter2.so/Zone-a8a06e4a5695-001.asm` at
ELF address `0x0039771c`:

1. The registered default `dimensions` is `(200, 200, 200)`.
2. Each dimension is multiplied by the corresponding GameObject scale at
   `+0x120`, `+0x124`, `+0x128`.
3. Half extents and their negatives are stored in local AABB fields
   `+0x144..+0x158`.

For the Infected Village Ambush fixture, cache scale `(1.92682, 1.92682, 1)`
therefore yields local half extents `(192.682, 192.682, 100)`. The original
`Zone::InitPost()` sends the bounds through a virtual call at object vptr
offset `+0x9c`, resolved to `GameObject::SetRelativeAABB`; the resulting world
bounds are stored at `Zone+0x12c..+0x140`. `project_is_inside()` accepts those
six already-updated world bounds. Transforming the local box is outside this
contact projection.

## `Zone::IsInside` projection

The ARM function begins at `0x003970c8`. A null GameObject or null
`GameObject+0x2dc` PhysicalObject returns false before either path.

### `Zone+0x384 == null`: inclusive position-in-AABB

The final comparisons at `0x00397454..0x00397554` use
`__aeabi_fcmple` (first argument `<=` second). They return true exactly when
all six conditions hold:

```text
Zone[+0x12c] <= GameObject[+0x160] <= Zone[+0x138]  // X
Zone[+0x130] <= GameObject[+0x164] <= Zone[+0x13c]  // Y
Zone[+0x134] <= GameObject[+0x168] <= Zone[+0x140]  // Z
```

Both faces are included. The actor's AABB is not consulted; the predicate
checks its position only.

This branch also calls `PhysicalObject::getRadius()` (`0x0046e750`), which
returns `(PhysicalObject+0x0c) * 100.0f`. It normalizes the XY position delta,
scales a temporary vector by that value, and stores it at `sp+0x64..0x6c`.
Those three stack slots are never read before return, so radius does not expand
the tested bounds or affect this result. The host API retains a radius field
only so the tests can assert that independence.

### `Zone+0x384 != null`: selector line query

At `0x00397274`, the scene-node vtable call at `vptr+0xb0` obtains its
`ITriangleSelector`. The collision manager's `getCollisionPoint()` at
`0x006c62e8` receives the constructed line and selector; its boolean result
is preserved in `r8` and returned by `Zone::IsInside`. A missing triangle
selector causes that manager query to return false.

The line's six float fields are assembled at `0x00397110..0x00397270`:

```text
delta = GameObject.position - Zone.position
start = delta + 100 * g
end   = delta - 100 * g
```

`g` is the native global at BSS address `0x0099f878`, referenced through GOT
slot `0x00998dd8` (GOT-relative offset `0x4340`). Its initializer is recovered:
`_GLOBAL__I_.._.._sources_Utils_Point3D.cpp` at `0x00312e00` is registered in
`.init_array` by `.rel.dyn` entry 10 (`r_offset=9789784`, initializer value
`3223040`, or `0x00312e00`). The constructor resolves the `0x4340` GOT entry
at `0x00312e10`, then stores `0.0f` at `g+0` (`0x00312e78`), `0.0f` at
`g+4` (`0x00312e54`), and `1.0f` at `g+8` (`0x00312e3c`). The resulting
native direction is `(0, 0, 1)`. A scan of recovered assembly xrefs for this
GOT entry finds that static constructor as the only direct store to the
vector; other xrefs load its components. The host API therefore uses the
constant `kNativeGlobalDirection` rather than accepting a caller-supplied
vector.

The host caller still supplies the selector line-query callback; the module
does not replace Irrlicht's triangle-selector collision manager. The actual
`Zone+0x384` state for the Infected Village Ambush remains unresolved. The
reason is the runtime `PropertyMap` template step, which runs before the MGP
overrides:

1. `ObjectManager::LoadFromXML()` calls `PropertyMap::SetTemplate()` at
   `0x0034b9ec` with the record's `_templateName` (`"TriggerZone"`).
2. It calls `PropertyMap::LoadDefaultProperties()` at `0x0034ba28`, then
   `LoadOverridesFromXML()` at `0x0034ba40`.
3. `PropertyMap::SetTemplate()` (`0x00513fec`) stores the template name,
   checks the class-map entry returned by `GetPropertyMap()` for a template
   store at `+0x10`, and calls `LoadTemplate()` (`0x0051419c`) when present.
   `LoadTemplate()` consults runtime template state and reaches the
   un-symbolized lookup at `0x0030e004`.
4. `LoadOverridesFromXML()` only calls `SetProperty()` for XML attributes
   that exist. The Ambush record in `infected01.mgp` has no `dae`,
   `xrefobject`, or `xrefmax` attribute, so it does not establish those
   fields' post-template values.

The `GameObject` constructor initializes its `dae`, `xrefobject`, and
`xrefmax` strings empty, but that is only the pre-template state. The cache
does not contain a standalone object-template definition that resolves the
`TriggerZone` entry, so static cache evidence cannot establish the values
after step 3. The precise missing evidence is the result of the
`PropertyMap` template-store lookup at `0x0030e004` for class `TriggerZone`
and template name `TriggerZone` (including whether that class-map `+0x10`
store is populated at runtime).

The downstream path is known: `GameObject::LoadVisualObject()` reads the
`dae` and `xrefobject` strings and calls the visual loader; a successfully
created `VisualObject` is stored at GameObject `+0x2d8`. `Zone::InitPost()`
looks up `_colzone` only on a non-null `+0x2d8` visual and stores the result
at `Zone+0x384`. The recovered Infected Village scene hierarchy has no
`_colzone` node, but this cannot prove that the Ambush never receives a
visual from its template. Therefore this fixture's selector pointer and
`Zone::IsInside` branch are not asserted. Host tests retain both selector-
null and selector-present paths without claiming which one the game instance
uses.

## Host verification

From the repository root:

```powershell
python port/zone-contact-runtime/tests/run_host.py
```

Tests cover cache-derived local dimensions, exact inclusive AABB faces,
one-float-step misses outside each axis, radius independence, the recovered
`(0,0,1)` direction with unchanged segment XY and Z endpoints exactly `delta.z
+/- 100`, and passthrough of both hit and miss results from a stub
collision-manager query. These are host projection tests only; they do not
establish full game contact behavior or integrate with movement.
