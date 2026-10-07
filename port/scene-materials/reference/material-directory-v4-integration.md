Source material directory V4
============================

New CMake units in scene-materials: shader_reflection_v4.cpp,
shader_program_collection_v4.cpp, material_compare_v4.cpp,
effect_material_directory_v4.cpp. New Android include:
renderer_effect_material_directory_v4.inc. Shared renderer/CMake were not edited.

Retain ONE ShaderProgramCollectionV4 on the owning GL context. Call
get_or_create(effect_program_cache_name_v4(actualPass), context, create, record,
error). Source634b30 concatenates vertex file, vertex defines, fragment file,
fragment defines without separators. ShaderSourcePlan.cache_name is a CODE key,
not this program key. The create callback initializes the actual
EffectDrawProgramV4 and returns shared_ptr<void> to that SAME object plus the
actual GL reflection list. Add a borrowed GLuint getter to EffectDrawProgramV4
and use reflect_effect_program_v4. The callback gets constructor collection-ID
0,1,...; no GLuint or resource ordinal substitutes for source shader+40 ID.
Source collection constructor5e5404, insert5e7110 and lookup5d85d0 were executed
against original ELF (receipt shader-program-collection-v4-original.json).
Supported collection domain is append-only source successful creation/query;
remove/reuse and full manager/code construction are not claimed.

Resource GPU records retain shared_ptr<ShaderProgramRecordV4>. Call draw on
static_pointer_cast<EffectDrawProgramV4>(record->same_program). A GPU resource
must not call program.release() while another resource/cache retains it. Use
an actual GL-context deleter to release the program once. On GL-context teardown,
drop resources before the collection, on that same context.

Retain one EffectMaterialDirectoryV4 per actual submitted material resource.
Before collecting/sorting its scene queue records, call refresh(record,
actualEffectRenderPassV4, &profileContext, EffectMaterialProfileContextV4::read,
error). Values must be current source producers:

* current_diffuse_color: actual retained material float4, including FIRE type86
  animation; immutable packed black BRES default is not a substitute.
* source_texture_matrix: actual source Matrix4f68 including its identity hint
  and padding. This is the original BRES/runtime UV matrix, even when the GPU
  transport uses identity because CPU particle UVs are already transformed.
* same_texture_owner: uintptr_t of SAME retained ITexture/native texture owner,
  never GLuint, asset ordinal, a new marker, or truncated ARM32 address.
* source_sampler_bias: actual source texture descriptor/LOD bias producer.

Missing active values explicitly fail. WVP is global semantic39 and never asks
the material callback. Source5e7b50 stable partitions semantics34..62 first;
all local parameters preserve actual glGetActiveUniform ordering. Whole original
5e2284 semantic-map construction and5e7f80 sub-ID parsing generated258 gold
records; native sanitized reflection test passed264 checks.

Use directory.view() in source queue material_equal/material_less callbacks.
Successful refresh atomically publishes the new snapshot; failure retains the
previous view but MUST propagate failure rather than submit stale material.
Do not refresh/destroy directories while the queue is comparing their borrows.

Original3537b0/353684/5ca72c equals,3537e4/5ca3fc less and5c5d88 parameter hash
ran for300 cases (public wrappers and real GetTechnique no-override branch).
Gold contains600 original hash results, one per side. Native host replay passed
906 ASan/UBSan checks:300 comparison pairs,300 original32-bit hash replays,
300 native64 hash invariants, plus cache/publication/directory regressions.
Shader/hash high byte is source collectionID low/high XOR; state nibble comes
from actual pass[0]; parameter polynomial is13 with separate texture12bit and
ordinary8bit components. Native texture hashing uses sizeof(uintptr_t)=8 bytes
from the actual retained identity; original golden arithmetic uses its4byte ABI.
No ARM32 surrogate or address truncation is introduced. Numeric native hashes
therefore are intentionally ABI-dependent, like the source pointer hash.

Strict ARM64 and x86_64 production+Android include syntax compile passed.
GPU linked reflection/draw with this directory and overlapping effect queue are
root integration/live verification tasks, not claimed by the host tests.
