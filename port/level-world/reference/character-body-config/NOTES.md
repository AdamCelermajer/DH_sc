Character creation was recovered from original ELF SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The manifest and assembly capture includes InitPhysicalObject `0x3b4088`,
SetPhysicalObject `0x394bf8`, POCharacter's cloned constructor `0x3b4010`,
PhysicalObject constructor `0x46f2f0`, `_init` `0x46edf0`, `_addShape` `0x46edb8`,
pin `0x46eb20`, source predicates and SetAsBox `0x7e44b8`. Additional b2World
CreateBody/b2Body constructor/SetMass instructions were captured for provenance;
their allocation/mass/proxy backends do not execute in this config audit.

InitPhysicalObject chooses the first applicable branch in this order:

| Condition | Shape | Group | Category | Mask | Bullet |
| --- | --- | ---: | ---: | ---: | ---: |
| type 9, invisible man | circle | 0 | 0x100 | 0 | 0 |
| nonzero owner byte +0x84 | polygon | 0 | 2 | 0xffff | 0 |
| NPC: type 6, 7 or 8 | circle | -4 | 0x400 | 0xd1f | 0 |
| virtual IsPlayer | circle | -1 | 4 | 0xd7f | 1 |
| follower/summoned: type 2 or 5 | circle | -2 | 8 | 0xd3b | 0 |
| faerie: type 3 | circle | 0 | 0x100 | 0 | 0 |
| monster: type 4 | circle | 2 | 0x10 | 0xd3f | 0 |
| remaining types | no object | | | | |

PhysicalObject's debug collision-group override replaces group with -666;
GameObject's separate disable-physical debug option destroys the newly supplied
object and leaves the prior attachment untouched. These are explicit caller
facts in native config; the original debug/string manager is a service fixture.

Absolute AABB x/y extent uses original owner offsets +0x12c/+0x130 and
+0x138/+0x13c. Subtract before multiplying by binary32 0.01. Radius is
`(width < height ? height : width) * 0.5`, using the MAX extent; unordered
comparison falls back to width. Negative/inverted bounds are not sanitized.
Circle center is (0,0), radius as above. Polygon vertices are
(-w/2,-h/2),(w/2,-h/2),(w/2,h/2),(-w/2,h/2), count 4. Polygon radius field is
unused; the PhysicalObject separately retains the max-extent radius. Owner
position +0x160/+0x164 is independently multiplied by 0.01; body angle is zero.

All shapes have friction 1, restitution 0, sensor false and userData pointing to
the PhysicalObject. Circle density is exact bits 0x4133d70a (binary32 11.24);
special polygon density is zero. BodyDef mass/center/inertia, angle and damping
are zero; allowSleep/isSleeping/fixedRotation are true; bullet is player policy.
The original body constructor maps these to flags 0x58, or 0x78 for the player,
before subsequent shape/mass services. This flag mapping is inspected original
code, not a complete execution of the body allocator/constructor in this audit.

Creation order is allocate PhysicalObject (40 ARM32 bytes), CreateBody,
CreateShape, SetMassFromShapes. POCharacter pins before attachment: pin sets
its byte +0x27 and calls SetMass({mass=0,center=current local center,inertia=0}).
This preserves the backend-produced local center; generic nonzero mass must not
be substituted. Special polygon pins during attachment instead. Attach deletes
the old PhysicalObject when different, stores the replacement, optionally pins
it, and updates PF object. An already identical attachment still updates PF,
without re-pinning. Pin is idempotent. Unsupported types attach null through the
same replacement policy. Disable-physical short-circuits attachment/PF update.

`dh2_character_body_config` produces immutable flat creation definitions and
ordered service request IDs, with real 64-bit userdata identities. Callers must
implement allocation, shape creation/mass, pin, destruction and PF attachment
services; the config producer does not perform those services. Requests describe
the successful-allocation path; original allocation failure contracts are not
reconstructed. Live AI definition lookup and debug flags are supplied inputs.

3864 original/ARM64 fixtures compare every body/shape/filter field and service
order, including 864 branch/precedence/attachment combinations and 3000 synthetic
IEEE/geometry cases. Integer/filter fields compare exactly (negative group
indices must not be mistaken for float NaNs). Arithmetic floats compare exact
finite/infinite/signed-zero bits, with arithmetic NaN sign/payload by class. The
host ASan/UBSan fixture audit checks the same 3864 cases and 22218 requests.

This recovers creation configuration and ownership policy. Full shape collision,
mass/local-center production, b2Body allocation, broadphase/contact mechanics,
unpin transitions and world stepping remain explicit integration boundaries.
