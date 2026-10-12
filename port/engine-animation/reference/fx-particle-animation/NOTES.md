# FX77 type28 particle parameter recovery

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. The manifest captures 31 complete source routines. The original instruction probe and the isolated native contribution proof have distinct scopes.

## Typed sampler and application

FX283 `zombie_spawn_fx.bdae` is 17,288 bytes, SHA256 `b54e3b05ba684eca654c8a58b8ebd5e84f7cad25929e0b5a60405f66dfdc772e`. Its type28 targets are `gl_pcloud_debris-emitter` and `gl_pcloud_dust-emitter`. The original factory `611ae0` selects `CFloatEx::s_Instance` (`9f8350`, initialized address point `98b2d8`) for both. The probe supplies the initialized singleton projection; it does not execute global C++ initialization.

`6e32cc` copies one float word. `6e3374` computes `first + fraction * (next - first)`, with separate float operations in that source order. `6e3288` computes `next - first`. `6e32f4` interpolates first, then subtracts the reference key. All paths use the actual accessor getter `669e24`; no default-metadata multiplier is present.

`6e3124` and `6e3184` both start at positive zero and accumulate every `value[i] * weight[i]` in slot order, including a single contribution and zero weights. Signed count<=0 writes positive zero. This differs from the full node/material count1-copy producer. `6e3258` obtains the source size through virtual+8 and copies four raw bytes to resolved parameter storage. It has no clamp, dirty notification, renderer update or particle simulation call.

New `ParticleAccessor16` is a native borrowed keys/count projection. Caller guards reject malformed indices/pointers/reserved fields and output overlap atomically. The original does not check aliasing; in particular its sum writes output each iteration. Alias inputs are outside the native bounded contract. IEEE inputs remain valid: direct copies preserve bits; arithmetic NaNs are compared by classification, other outputs (including finite words, infinities and signed zeros) match exactly. Compiler contraction and fast math are disabled.

## Real parameter identity and ownership

Source `forceBind 667f18`, type28 branch `668354`, calls root lookup `65b4e4`. That routine walks the root particle list at +158, invokes each particle's virtual+54 name getter and calls `strcasecmp`; the probe models ASCII comparison for these authored ASCII URIs. On a match it invokes the particle's virtual+FC with literal `BirthRate` at `8e53c0`. It then calls animator virtual+68 with channel index, returned float pointer and null applicator info. A missing particle still receives the null target binding.

`CParticleSystemSceneNode::getParticleSystemParameter 651924` projects the context virtual base from node+178 and calls actual `hashString 64d0bc`. `BirthRate` hashes to `7d66d025`. The original STL map stores a parameter pointer in node+14. A missing key inserts **a null pointer**, not an owned float initialized to zero. Both actual hash/STL lookup/insertion and source forceBind execute in the probe. The channel accessors, virtual name and setTarget receiver are explicit services.

The real storage producer is `PGenerationModel<SParticle>::C2 6545d0`: owned BirthRate at model+4 is initialized to `1.0f`; model+8 integer is initialized to1. The constructor registers model+4 under BirthRate using actual hash and unique STL insertion. The probe supplies VTT/virtual-base layout, executes the original constructor, verifies the actual getter returns model+4, executes original `setParameter<float> 650400`, and executes `CFloatEx::applyValue` into that storage. Only BirthRate's four bytes change in the latter application.

Source `initParticleSystem 656450` calls singleton `PSManager::getInstance 63b3d0`, then `createPCloudSystem 655314(bool)` and stores the returned owned object at node+178. Factory655314 allocates `0x1dc` bytes, zeroes its first `0x180`, and selects mixin constructor654ec4 for false or654a74 for true. At `6564e4..6564fc` the source reads borrowed SEmitter+20 (hex) and writes BirthRate through the real parameter setter. The model constructor is recovered; the complete mixin factory/emitter decoder, other model updates, buffer allocation, renderer/material factory and GPU draw are not supplied by this module.

## Attachment and destruction

Source `attach 64f6f8` reads node+150's attachment-name vector, resizes/zeroes node+164's pointer vector through actual STL routines, and processes the authored names in order. It strips the first byte before root name lookup5985e4. For each found root node it traverses the child list at +f4, invokes child virtual+BC, accepts exact tag `0x66656164`, then calls child virtual+F4 with the particle node. The next link is read after the callback. The probe mutates it during attachment and proves the subsequent child is skipped. Root search and child virtual methods remain required providers; the probe does not manufacture completed attachments.

Source destructor64ecac resets the node's vtables, invokes owned node+178 deleting virtual+8 when nonnull, then calls base destructor6678b8. The probe records that exact order with required destructor services. It does not claim the base destructor or model cleanup is reconstructed.

## Next genuine factory interface

The smallest live ownership shape is an owned, address-stable `ParticleNodeOwner` retaining the relocated resource/database and SEmitter lifetime, real model allocation, material instances/renderer handles and attachment receivers. `resolve_parameter("BirthRate")` must expose the registered model's real storage and preserve null insertion semantics. A `ParticleParameterLease { shared_ptr<ParticleNodeOwner> owner; float* storage; }` pins that receiver while compiled slot bindings exist; raw float pointers alone do not suffice. Release compiled bindings before destroying the particle node/model. Child attachment callbacks may mutate lists synchronously, so traversal must reload links as the source does.

Concrete next work: reconstruct native parameter/context registration and the PGeneration owned constructor, then the actual createPCloudSystem/mixin initialization wrapper with required typed model/emitter/material/renderer providers. A missing provider fails explicitly. No zero-entry substitution, no fake attach success and no renderer-neutral particle startup are acceptable. Particle simulation/baker/draw are separate mandatory deeper bodies, rather than accepted no-ops.

## Proof and reproduction

`probe.py` executes original selection, parameter map, generation constructor/storage, forceBind, attachment and destruction ordering. `particle_parameter_differential.py` executes actual original interpreters, both contribution producers and raw applicator versus NDK29 O2 ARM64. `particle_parameter_host.py` builds an isolated actual DSO and replays the original-derived corpus under ASan/UBSan. It never rebuilds shared DSOs or Android artifacts.

Current results: 4,925 comparisons, including610 real FX77 key/interpolation cases; 16 atomic caller rejections; zero mismatches and zero sanitizer findings. Reports and `native-source-freeze.json` bind exact source, gold, original capture and executed binaries. Frozen material-color sources/proofs remain unchanged.
