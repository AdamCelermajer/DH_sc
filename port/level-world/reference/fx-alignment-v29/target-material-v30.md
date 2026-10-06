# Target-circle actual material plan

The live failure was an incomplete renderer ProfileCOMMON define allowlist, after actual BRES default technique and bundled shader plan already resolved. Actual target_circle.bdae and target_cross.bdae default technique uses vertex `#define TEXTURED\n` and fragment `#define TEXTURED\n#define ALPHABLEND\n`. The old guard accepted only TEXTURED or TEXTURED+ADDITIVEBLEND. The production V5 draw helper now accepts the exact actual ALPHABLEND combination while preserving source chunk equality and actual source bytes.

Bundled ProfileCOMMON_emul_FS.glsl contains no ALPHABLEND-specific shader branch: its textured color equation remains vColor0*actual sampler. Blend/depth/cull still come from actual BRES pass conversion, never from the define name. Reflection and same material/texture/Matrix68 owners are unchanged. No shader substitution, guessed quad or target bypass was added.

Actual asset technique census is target-material-techniques-v30.json, extracted from retained target-art-v28 assets using BRES offsets. Full current model_renderer syntax checks passed ARM64 and x86_64. GPU/live acceptance belongs to root's subsequent build and target-ring test; syntax plus asset census alone does not establish visibility.
