# Owned FX77 authored BirthRate and source particle generation

Original ELF SHA256 `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`. This additive stage connects retained external animator resource sampling to genuine generation storage and source SParticle vector production. It leaves full model initialization, simulation, material/baker/driver and draw services required.

## Source emission order and storage

PGenerationModel.generateParticles65317c reads current/previous float seconds at context+48/+4c, subtracts and stores delta at model+c. It computes separate float32 `(delta * BirthRate) + carry`, converts by signed f2iz, then stores `amount - float(born)` in model+10 **before** checking born>0. Nonpositive births return current vector end without resize. Negative rates/time deltas therefore still update carry; there is no positive-rate filter.

Positive births add the existing count derived from vector begin/end, stride100. Signed MaxParticles caps that total, except MaxParticles0 permits uncapped growth. Lowering the cap beneath an existing count can shrink the vector while returning begin+old_count*100; the resulting first>end pair is faithfully exposed as indices, not advertised as a valid model-initialization span. Actual STL resize65311c executes in the original oracle. Native initGenerationModel resets carry0, clears owned particle storage and reserves the current maximum; source64fc50 executes in the corpus.

The original temporary particle template is100 bytes. Source writes positive zero at offsets0,4,8,12,16,20,32,36,40,48,52,56,84,88,92; writes ff color bytes24..27; writes1.0f at28 and44. Bytes60..83 and96..99 retain caller stack contents. Native `ParticleSeed100` supplies those untouched bytes explicitly and applies exactly the source writes. No invented zero-fill, lifetime subtraction, particle position randomizer or usable full-cloud claim is attached to this storage.

`ParticleEmissionOwner` retains the real `ParticleGenerationOwner`, uses its owned BirthRate/MaxParticles/delta/carry cells, and owns the vector. It is noncopyable. One emission owner belongs to one generation receiver. Mutable `particle(index)` is borrowed storage for required model providers, invalidated by clearing/reallocation. Providers must initialize and simulate the fields they consume. Allocation exceptions propagate; original allocation-error prefix parity is outside this bounded proof.

## Real external scalar resource ownership

`ParticleAnimationResource::create` deep-copies complete immutable BRES backing and decodes actual single-segment/single-sampler float6 type28 tracks through the genuine resources/payloads APIs. Other channel domains are delegated to their own implementations; this is not a generic scene animator. The two FX77 targets are debris and dust emitters. Authored sampler interpolation is0; an additional explicitly synthetic flag1 corpus exercises the same source cached-key interpolation paths.

Source66a1a8→66a004→actual CFloatEx generic getValue6e2c40→cached key search66b814→6e32cc/6e3374 executes. Prior cursors retain inclusive neighbor history; interpolation uses original float time conversion and truncated signed wrapped millisecond differences. No Player.sample reset, generic binary-search substitution, timeline advance or GPU binding is introduced. `ParticleBirthRateBinding` retains both resource and real parameter lease, validates canonical BirthRate storage, and commits its independent cursor only after raw CFloatEx apply. Caller resolves the original particle URI; no fake scene-node registry is supplied.

Native guards reject nonfinite/unrepresentable emission conversion, negative/oversized maximum, count/uncapped growth beyond65536, malformed/overlapping output and invalid sampling cursor atomically. Those are explicit native contracts, not original guards. Float keys remain raw IEEE data; emission's finite contract is separate.

## Proof and runtime handoff

Original↔NDK29 O2 passes1,496 cases:500 arithmetic/template comparisons,120 owned vector sequences/1,440 steps,656 actual FX cached samples and220 synthetic interpolation samples. All output words, copied particle bytes and returned source pointer-index pairs compare exactly; no tolerance. Source init, vector growth/copy/shrink, accessor and scalar arithmetic bodies execute; allocator/libc/imported IEEE services are explicit.

Actual isolated ASan/UBSan DSO replays this gold, then both actual authored emitter resources drive retained sampling→generation for201 frames each with source MaxParticles20. Owners/backing remain valid after caller references disappear, independent emitter particle storage remains distinct, malformed frame/sample rejections preserve state, and resource backing is released when bindings are released. No shared library build, Android artifact or frozen prior module was modified.

Integration: add `particle_emission.cpp` to engine-animation, linked to genuine asset-payloads/resources plus existing particle_factory/particle_parameter. Host `particle_emission_audit` compiles tests/particle_emission.cpp and links actual animation DSO. Args: reference/fx-particle-emission/particle-emission-fixtures.bin and actual zombie_spawn_fx.bdae. No math wrappers. Renderer-facing sequence is source animator time sample/apply, source cloud current/previous seconds generate, required model initialization on valid emitted indices, then required simulation/baker/material/render. This module does not accept missing later operations as successful.

Internal AnimationDatabase streaming ownership is a separate recovered boundary in reference/fx-particle-database. It must never be mapped by casting the frozen source32 diagnostic cell to a native pointer.
