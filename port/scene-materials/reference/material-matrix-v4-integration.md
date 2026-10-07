# Retained source texture matrix

Use `CharacterParticleDrawSourceV3::source_texture_matrix68` for
`EffectMaterialProfileBorrowV4::source_texture_matrix`. It borrows the sole
matrix owned by the same V3 particle resource. The geometry UV bake reads its
sixteen floats; `scene::Material.texture_matrix` is the identity GPU transport
matrix after that bake and is not the source parameter comparison value.

`material_matrix_v4.hpp` is header-only and needs no CMake source addition.
Whole original constructor631c14, cached identity test5ba19c, material creation
631ce8 and effect-default application6324c4 are captured in the adjacent source
ASM receipts. The BRES branch sets hint0, copies sixteen floats, then executes
the cached identity test. An identity candidate skips parameter installation,
so the retained native value becomes the exact identity default; a nonidentity
candidate retains the copied floats and hint0. This is the source producer,
including its single-precision epsilon arithmetic, rather than a float-derived
replacement flag. Unsupported texture-matrix animation remains outside the
V3 supported material-animation domain (which accepts only diffuse-color).

Original constructor and matrix copies define only bytes0..64. Seeded original
storage preserves A5 in padding65..67; the material hash nevertheless reads68
bytes. The native reconstruction explicitly zeroes all68 before installing the
65 defined bytes, producing deterministic native padding. This is a documented
ABI normalization, not a claim of exact original uninitialized padding. Actual
owner identities remain full-width native pointers.

Validation:400 whole original ARM cached identity cases;782 kernel/default
normalization/padding checks at both O1 and O2 with ASan+UBSan. Post-change
strict ARM64 and x86_64 resource compilation passes. The source parameter
pointer is borrowed synchronously; keep its particle resource retained while
refreshing the material directory.
