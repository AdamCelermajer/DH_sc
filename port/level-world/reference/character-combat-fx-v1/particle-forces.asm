
FUNCTION 0x60e450 _ZNK6glitch7collada16CColladaDatabase8getForceEi
0x60e450 ldr r3, [r0]
0x60e454 ldr r3, [r3, #0x24]
0x60e458 ldr r3, [r3, #0x20]
0x60e45c ldr r0, [r3, #0x8c]
0x60e460 add r0, r0, r1, lsl #4
0x60e464 bx lr

FUNCTION 0x61a910 _ZNK6glitch7collada16CColladaDatabase8getForceEPKc
0x61a910 push {r4, r5, r6, r7, r8, lr}
0x61a914 ldr r3, [r0]
0x61a918 mov r7, r1
0x61a91c ldr r3, [r3, #0x24]
0x61a920 ldr r3, [r3, #0x20]
0x61a924 ldr r6, [r3, #0x88]
0x61a928 cmp r6, #0
0x61a92c ble #0x61a968
0x61a930 ldr r4, [r3, #0x8c]
0x61a934 mov r5, #0
0x61a938 b #0x61a948
0x61a93c cmp r5, r6
0x61a940 add r4, r4, #0x10
0x61a944 beq #0x61a968
0x61a948 ldr r0, [r4]
0x61a94c mov r1, r7
0x61a950 bl #0x30e31c
0x61a954 cmp r0, #0
0x61a958 add r5, r5, #1
0x61a95c bne #0x61a93c
0x61a960 mov r0, r4
0x61a964 pop {r4, r5, r6, r7, r8, pc}
0x61a968 mov r0, #0
0x61a96c pop {r4, r5, r6, r7, r8, pc}

FUNCTION 0x62fee4 _ZN6glitch7collada15particle_system15CForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
0x62fee4 bx lr

FUNCTION 0x62fee8 _ZN6glitch7collada15particle_system15CForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
0x62fee8 bx lr

FUNCTION 0x62feec _ZNK6glitch7collada15particle_system15CForceSceneNode7getTypeEv
0x62feec movw r0, #0x6164
0x62fef0 movt r0, #0x6665
0x62fef4 bx lr

FUNCTION 0x62fef8 _ZN6glitch7collada15particle_system15CForceSceneNode6renderEPv
0x62fef8 bx lr

FUNCTION 0x62ff40 _ZN6glitch2ps6PForceINS0_9SParticleEED1Ev
0x62ff40 bx lr

FUNCTION 0x62ff4c _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_5PWindEED1Ev
0x62ff4c bx lr

FUNCTION 0x62ff50 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_8PGravityEED1Ev
0x62ff50 bx lr

FUNCTION 0x62ff5c _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_10PDeflectorEED1Ev
0x62ff5c bx lr

FUNCTION 0x630170 _ZN6glitch7collada15particle_system22CGravityForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
0x630170 add r3, r0, #0x140
0x630174 ldr r0, [r1, #0x178]
0x630178 mov r1, r3
0x63017c b #0x630108

FUNCTION 0x630180 _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE9bindForceINS0_8PGravityEEEiRNT_15parameters_typeE
0x630180 push {r4, r5, r6, r7, r8, lr}
0x630184 ldr r2, [r0]
0x630188 mov r3, r0
0x63018c mov r5, r1
0x630190 ldr r7, [r2, #-0xc]
0x630194 mov r1, #0
0x630198 mov r0, #0x10
0x63019c ldr r2, [r3, r7]
0x6301a0 add r7, r3, r7
0x6301a4 ldr r4, [pc, #0x34]
0x6301a8 ldr r6, [r2, #0x58]
0x6301ac bl #0x5341ac
0x6301b0 ldr r3, [pc, #0x2c]
0x6301b4 add r4, pc, r4
0x6301b8 mov r2, #0
0x6301bc ldr r3, [r4, r3]
0x6301c0 mov r1, r0
0x6301c4 str r2, [r0, #8]
0x6301c8 add r3, r3, #8
0x6301cc str r5, [r0, #0xc]
0x6301d0 stm r0, {r3, r5}
0x6301d4 mov r0, r7
0x6301d8 blx r6
0x6301dc pop {r4, r5, r6, r7, r8, pc}
0x6301e0 ldrsbteq r4, [r6], -ip
0x6301e4 andeq r3, r0, r0, lsr #28

FUNCTION 0x6301e8 _ZN6glitch7collada15particle_system22CGravityForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
0x6301e8 add r3, r0, #0x140
0x6301ec ldr r0, [r1, #0x178]
0x6301f0 mov r1, r3
0x6301f4 b #0x630180

FUNCTION 0x6303f0 _ZNK6glitch7collada15particle_system22CGravityForceSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
0x6303f0 push {r4, r5, r6, lr}
0x6303f4 mov r4, r1
0x6303f8 mov r5, r0
0x6303fc bl #0x5972a4
0x630400 ldr r1, [pc, #0x5c]
0x630404 mov r0, r4
0x630408 ldr r2, [r5, #0x148]
0x63040c ldr ip, [r4]
0x630410 add r1, pc, r1
0x630414 mov r3, #0
0x630418 mov lr, pc
0x63041c ldr pc, [ip, #0x64]
0x630420 ldr r1, [pc, #0x40]
0x630424 mov r0, r4
0x630428 ldr r2, [r5, #0x144]
0x63042c ldr ip, [r4]
0x630430 add r1, pc, r1
0x630434 mov r3, #0
0x630438 mov lr, pc
0x63043c ldr pc, [ip, #0x64]
0x630440 ldr r1, [pc, #0x24]
0x630444 mov r0, r4
0x630448 ldr r2, [r5, #0x14c]
0x63044c add r1, pc, r1
0x630450 ldr ip, [r4]
0x630454 mov r3, #0
0x630458 mov lr, pc
0x63045c ldr pc, [ip, #0x4c]
0x630460 pop {r4, r5, r6, pc}
0x630464 eoreq r4, fp, r0, lsr fp
0x630468 eoreq r4, fp, r0, lsl #22
0x63046c eoreq r0, fp, ip, ror r5

FUNCTION 0x630648 _ZN6glitch7collada15particle_system22CGravityForceSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS3_26SAttributeReadWriteOptionsE
0x630648 push {r4, r5, r6, lr}
0x63064c mov r4, r1
0x630650 mov r5, r0
0x630654 bl #0x598058
0x630658 ldr r1, [pc, #0x34]
0x63065c ldr r3, [r4]
0x630660 mov r0, r4
0x630664 add r1, pc, r1
0x630668 mov lr, pc
0x63066c ldr pc, [r3, #0x70]
0x630670 ldr r1, [pc, #0x20]
0x630674 str r0, [r5, #0x148]
0x630678 ldr r3, [r4]
0x63067c mov r0, r4
0x630680 add r1, pc, r1
0x630684 mov lr, pc
0x630688 ldr pc, [r3, #0x70]
0x63068c str r0, [r5, #0x144]
0x630690 pop {r4, r5, r6, pc}

FUNCTION 0x6306d4 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_10PDeflectorEED0Ev
0x6306d4 push {r4, lr}
0x6306d8 mov r4, r0
0x6306dc bl #0x30e2b0
0x6306e0 mov r0, r4
0x6306e4 pop {r4, pc}

FUNCTION 0x6306e8 _ZN6glitch2ps6PForceINS0_9SParticleEED0Ev
0x6306e8 push {r4, lr}
0x6306ec mov r4, r0
0x6306f0 bl #0x30e2b0
0x6306f4 mov r0, r4
0x6306f8 pop {r4, pc}

FUNCTION 0x630710 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_5PWindEED0Ev
0x630710 push {r4, lr}
0x630714 mov r4, r0
0x630718 bl #0x30e2b0
0x63071c mov r0, r4
0x630720 pop {r4, pc}

FUNCTION 0x630738 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_8PGravityEED0Ev
0x630738 push {r4, lr}
0x63073c mov r4, r0
0x630740 bl #0x30e2b0
0x630744 mov r0, r4
0x630748 pop {r4, pc}

FUNCTION 0x630768 _ZN6glitch7collada15particle_system15CForceSceneNodeD1Ev
0x630768 push {r4, r5, r6, lr}
0x63076c ldr r5, [pc, #0x40]
0x630770 ldr r3, [pc, #0x40]
0x630774 mov r4, r0
0x630778 add r5, pc, r5
0x63077c ldr r3, [r5, r3]
0x630780 add r0, r0, #0x134
0x630784 add r2, r3, #0x128
0x630788 add r3, r3, #0x1c
0x63078c str r3, [r4]
0x630790 str r2, [r4, #0x140]
0x630794 bl #0x619474
0x630798 ldr r1, [pc, #0x1c]
0x63079c mov r0, r4
0x6307a0 ldr r1, [r5, r1]
0x6307a4 add r1, r1, #4
0x6307a8 bl #0x598cbc
0x6307ac mov r0, r4
0x6307b0 pop {r4, r5, r6, pc}
0x6307b4 eorseq r4, r6, r8, lsl r3
0x6307b8 andeq r0, r0, r0, lsl sl
0x6307bc andeq r2, r0, r0, lsl r7

FUNCTION 0x6307e0 _ZN6glitch7collada15particle_system15CForceSceneNodeD0Ev
0x6307e0 push {r4, lr}
0x6307e4 mov r4, r0
0x6307e8 bl #0x630768
0x6307ec mov r0, r4
0x6307f0 bl #0x30e2b0
0x6307f4 mov r0, r4
0x6307f8 pop {r4, pc}

FUNCTION 0x63081c _ZN6glitch7collada15particle_system15CForceSceneNodeD2Ev
0x63081c push {r4, r5, r6, lr}
0x630820 ldr r3, [r1]
0x630824 mov r4, r0
0x630828 mov r5, r1
0x63082c str r3, [r4]
0x630830 ldr r2, [r1, #0x10]
0x630834 ldr r3, [r3, #-0x1c]
0x630838 add r0, r0, #0x134
0x63083c str r2, [r4, r3]
0x630840 ldr r3, [r4]
0x630844 ldr r2, [r1, #0x14]
0x630848 ldr r3, [r3, #-0xc]
0x63084c str r2, [r4, r3]
0x630850 bl #0x619474
0x630854 mov r0, r4
0x630858 add r1, r5, #4
0x63085c bl #0x598cbc
0x630860 mov r0, r4
0x630864 pop {r4, r5, r6, pc}

FUNCTION 0x630940 _ZN6glitch7collada15particle_system22CGravityForceSceneNodeD1Ev
0x630940 ldr r3, [pc, #0x38]
0x630944 ldr r2, [pc, #0x38]
0x630948 ldr r1, [pc, #0x38]
0x63094c add r3, pc, r3
0x630950 ldr r2, [r3, r2]
0x630954 ldr r1, [r3, r1]
0x630958 push {r4, lr}
0x63095c add ip, r2, #0x128
0x630960 add r2, r2, #0x1c
0x630964 mov r4, r0
0x630968 str r2, [r0]
0x63096c str ip, [r0, #0x150]
0x630970 add r1, r1, #4
0x630974 bl #0x63081c
0x630978 mov r0, r4
0x63097c pop {r4, pc}
0x630980 eorseq r4, r6, r4, asr #2
0x630984 andeq r0, r0, r8, asr #29
0x630988 ldrdeq r3, r4, [r0], -r4

FUNCTION 0x630ee8 _ZNK6glitch7collada15particle_system15CForceSceneNode14getBoundingBoxEv
0x630ee8 push {r4, r5, r6, lr}
0x630eec ldr r4, [pc, #0x6c]
0x630ef0 ldr r3, [pc, #0x6c]
0x630ef4 add r4, pc, r4
0x630ef8 ldr r5, [r4, r3]
0x630efc ldr r3, [r5]
0x630f00 tst r3, #1
0x630f04 beq #0x630f14
0x630f08 ldr r6, [pc, #0x58]
0x630f0c ldr r0, [r4, r6]
0x630f10 pop {r4, r5, r6, pc}
0x630f14 mov r0, r5
0x630f18 bl #0x30e76c
0x630f1c cmp r0, #0
0x630f20 beq #0x630f08
0x630f24 ldr r6, [pc, #0x3c]
0x630f28 mov r1, #0xbf000000
0x630f2c add r1, r1, #0x800000
0x630f30 ldr r3, [r4, r6]
0x630f34 mov r2, #0x3f800000
0x630f38 mov r0, r5
0x630f3c str r2, [r3, #0x14]
0x630f40 str r1, [r3, #8]
0x630f44 str r1, [r3]
0x630f48 str r1, [r3, #4]
0x630f4c str r2, [r3, #0xc]
0x630f50 str r2, [r3, #0x10]
0x630f54 bl #0x30ea3c
0x630f58 ldr r0, [r4, r6]
0x630f5c pop {r4, r5, r6, pc}
0x630f60 mlaseq r6, ip, fp, r3
0x630f64 andeq r3, r0, ip, asr #2
0x630f68 andeq r2, r0, r0, ror ip

FUNCTION 0x630f6c _ZN6glitch7collada15particle_system15CForceSceneNodeC2ERKNS0_16CColladaDatabaseERKNS0_6SForceE
0x630f6c push {r4, r5, r6, r7, lr}
0x630f70 sub sp, sp, #0x34
0x630f74 add r4, sp, #8
0x630f78 mov ip, #0
0x630f7c mov lr, #0x3f800000
0x630f80 mov r6, r2
0x630f84 str r4, [sp]
0x630f88 mvn r2, #0
0x630f8c add r4, sp, #0x18
0x630f90 mov r5, r1
0x630f94 mov r7, r3
0x630f98 add r1, r1, #4
0x630f9c add r3, sp, #0x24
0x630fa0 str r4, [sp, #4]
0x630fa4 str ip, [sp, #0x10]
0x630fa8 mov r4, r0
0x630fac str lr, [sp, #0x20]
0x630fb0 str ip, [sp, #0x24]
0x630fb4 str ip, [sp, #0x28]
0x630fb8 str ip, [sp, #0x2c]
0x630fbc str ip, [sp, #8]
0x630fc0 str ip, [sp, #0xc]
0x630fc4 str lr, [sp, #0x14]
0x630fc8 str lr, [sp, #0x18]
0x630fcc str lr, [sp, #0x1c]
0x630fd0 bl #0x5990c0
0x630fd4 ldr r2, [r6]
0x630fd8 ldr r3, [pc, #0x68]
0x630fdc str r2, [r4, #0x134]
0x630fe0 ldr r1, [r6, #4]
0x630fe4 cmp r2, #0
0x630fe8 add r3, pc, r3
0x630fec str r1, [r4, #0x138]
0x630ff0 beq #0x631004
0x630ff4 ldr r1, [r2, #4]
0x630ff8 cmp r1, #0
0x630ffc addne r1, r1, #1
0x631000 strne r1, [r2, #4]
0x631004 ldr r2, [pc, #0x40]
0x631008 mov r0, r4
0x63100c ldr r2, [r3, r2]
0x631010 add r2, r2, #4
0x631014 str r2, [r4, #0x130]
0x631018 ldr r3, [r5]
0x63101c str r3, [r4]
0x631020 ldr r3, [r3, #-0x1c]
0x631024 ldr r2, [r5, #0x10]
0x631028 str r2, [r4, r3]
0x63102c ldr r3, [r4]
0x631030 ldr r2, [r5, #0x14]
0x631034 ldr r3, [r3, #-0xc]
0x631038 str r2, [r4, r3]
0x63103c str r7, [r4, #0x13c]
0x631040 add sp, sp, #0x34
0x631044 pop {r4, r5, r6, r7, pc}
0x631048 eorseq r3, r6, r8, lsr #21
0x63104c strheq r1, [r0], -r4

FUNCTION 0x631274 _ZN6glitch7collada15CColladaFactory32createParticleSystemGravityForceERKNS0_16CColladaDatabaseEPNS0_6SForceE
0x631274 push {r4, r5, r6, r7, r8, lr}
0x631278 mov r0, #0x158
0x63127c mov r7, r1
0x631280 mov r1, #0
0x631284 mov r6, r2
0x631288 bl #0x5341ac
0x63128c ldr r5, [pc, #0x98]
0x631290 ldr r2, [pc, #0x98]
0x631294 ldr r3, [pc, #0x98]
0x631298 add r5, pc, r5
0x63129c ldr r1, [r5, r2]
0x6312a0 ldr r3, [r5, r3]
0x6312a4 mov ip, #1
0x6312a8 ldr r2, [r1, #0x24]
0x6312ac add r3, r3, #8
0x6312b0 str ip, [r0, #0x154]
0x6312b4 str r2, [r0]
0x6312b8 str r3, [r0, #0x150]
0x6312bc ldr r3, [r2, #-0xc]
0x6312c0 ldr ip, [r1, #0x28]
0x6312c4 mov r2, r7
0x6312c8 add r1, r1, #4
0x6312cc str ip, [r0, r3]
0x6312d0 mov r3, r6
0x6312d4 mov r4, r0
0x6312d8 bl #0x630f6c
0x6312dc ldr r3, [pc, #0x54]
0x6312e0 ldr r2, [r4, #0x13c]
0x6312e4 add r1, r4, #0x24
0x6312e8 ldr r3, [r5, r3]
0x6312ec str r1, [r4, #0x140]
0x6312f0 mov r0, r4
0x6312f4 add r1, r3, #0x128
0x6312f8 add r3, r3, #0x1c
0x6312fc str r3, [r4]
0x631300 str r1, [r4, #0x150]
0x631304 ldr r3, [r2, #0xc]
0x631308 ldr r3, [r3]
0x63130c str r3, [r4, #0x144]
0x631310 ldr r3, [r2, #0xc]
0x631314 ldr r3, [r3, #4]
0x631318 str r3, [r4, #0x148]
0x63131c ldr r3, [r2, #0xc]
0x631320 ldr r3, [r3, #8]
0x631324 str r3, [r4, #0x14c]
0x631328 pop {r4, r5, r6, r7, r8, pc}
0x63132c ldrshteq r3, [r6], -r8
0x631330 ldrdeq r3, r4, [r0], -r4
0x631334 andeq r2, r0, r4, asr #22
0x631338 andeq r0, r0, r8, asr #29

FUNCTION 0x632a88 _ZN6glitch7collada15particle_system22CGravityForceSceneNodeD0Ev
0x632a88 ldr r3, [pc, #0x40]
0x632a8c ldr r2, [pc, #0x40]
0x632a90 ldr r1, [pc, #0x40]
0x632a94 add r3, pc, r3
0x632a98 ldr r2, [r3, r2]
0x632a9c ldr r1, [r3, r1]
0x632aa0 push {r4, lr}
0x632aa4 add ip, r2, #0x128
0x632aa8 add r2, r2, #0x1c
0x632aac mov r4, r0
0x632ab0 str r2, [r0]
0x632ab4 str ip, [r0, #0x150]
0x632ab8 add r1, r1, #4
0x632abc bl #0x63081c
0x632ac0 mov r0, r4
0x632ac4 bl #0x30e2b0
0x632ac8 mov r0, r4
0x632acc pop {r4, pc}
0x632ad0 ldrshteq r1, [r6], -ip
0x632ad4 andeq r0, r0, r8, asr #29
0x632ad8 ldrdeq r3, r4, [r0], -r4

FUNCTION 0x633360 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_5PWindEE5applyEPS2_S5_PNS0_16IParticleContextIS2_EE
0x633360 add r0, r0, #0xc
0x633364 b #0x632f6c

FUNCTION 0x633624 _ZN6glitch2ps8PGravity5applyINS0_9SParticleEEEvNS0_16IParticleContextIT_E11ParticleIttES7_PS6_
0x633624 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0x633628 mov ip, #0
0x63362c sub sp, sp, #0x24
0x633630 str ip, [sp, #0x1c]
0x633634 str ip, [sp, #0x14]
0x633638 str ip, [sp, #0x18]
0x63363c ldr r5, [r0]
0x633640 mov r7, r1
0x633644 mov r1, #0x44000000
0x633648 ldr r0, [r5, #4]
0x63364c add r1, r1, #0x7a0000
0x633650 mov r4, r2
0x633654 mov r6, r3
0x633658 bl #0x30ed6c
0x63365c str r0, [sp, #4]
0x633660 ldr r6, [r6, #0x50]
0x633664 ldr sb, [r5, #0xc]
0x633668 cmp r7, r4
0x63366c str r6, [sp]
0x633670 ldr r6, [r5]
0x633674 ldr fp, [r5, #8]
0x633678 beq #0x633754
0x63367c add r3, fp, #0x80000000
0x633680 str r3, [sp, #0xc]
0x633684 add r3, sp, #0x14
0x633688 mov r5, r7
0x63368c str r3, [sp, #8]
0x633690 cmp sb, #0
0x633694 bne #0x6337d8
0x633698 ldr r3, [r6, #0x20]
0x63369c ldr r0, [sp, #8]
0x6336a0 str r3, [sp, #0x14]
0x6336a4 ldr r3, [r6, #0x24]
0x6336a8 str r3, [sp, #0x18]
0x6336ac ldr r3, [r6, #0x28]
0x6336b0 strb sb, [r6, #0x40]
0x6336b4 str r3, [sp, #0x1c]
0x6336b8 bl #0x35e8e0
0x6336bc mov r0, fp
0x6336c0 mov r1, #0
0x6336c4 bl #0x30e2f8
0x6336c8 cmp r0, #0
0x6336cc bne #0x63375c
0x6336d0 ldr r0, [sp, #4]
0x6336d4 ldr r1, [sp]
0x6336d8 bl #0x30ed6c
0x6336dc ldr r1, [sp, #0x14]
0x6336e0 mov r7, r0
0x6336e4 bl #0x30ed6c
0x6336e8 mov r1, r7
0x6336ec mov sl, r0
0x6336f0 ldr r0, [sp, #0x18]
0x6336f4 str sl, [sp, #0x14]
0x6336f8 bl #0x30ed6c
0x6336fc mov r1, r7
0x633700 mov r8, r0
0x633704 ldr r0, [sp, #0x1c]
0x633708 str r8, [sp, #0x18]
0x63370c bl #0x30ed6c
0x633710 mov r1, sl
0x633714 mov r7, r0
0x633718 ldr r0, [r5, #0xc]
0x63371c str r7, [sp, #0x1c]
0x633720 bl #0x30eba4
0x633724 mov r1, r8
0x633728 str r0, [r5, #0xc]
0x63372c ldr r0, [r5, #0x10]
0x633730 bl #0x30eba4
0x633734 mov r1, r7
0x633738 str r0, [r5, #0x10]
0x63373c ldr r0, [r5, #0x14]
0x633740 bl #0x30eba4
0x633744 str r0, [r5, #0x14]
0x633748 add r5, r5, #0x64
0x63374c cmp r4, r5
0x633750 bne #0x633690
0x633754 add sp, sp, #0x24
0x633758 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0x63375c ldr r1, [r6, #0x30]
0x633760 ldr r0, [r5]
0x633764 bl #0x30e3ac
0x633768 ldr r1, [sp, #0x14]
0x63376c bl #0x30ed6c
0x633770 ldr r1, [r6, #0x34]
0x633774 mov r7, r0
0x633778 ldr r0, [r5, #4]
0x63377c bl #0x30e3ac
0x633780 ldr r1, [sp, #0x18]
0x633784 bl #0x30ed6c
0x633788 mov r1, r0
0x63378c mov r0, r7
0x633790 bl #0x30eba4
0x633794 ldr r1, [r6, #0x38]
0x633798 mov r7, r0
0x63379c ldr r0, [r5, #8]
0x6337a0 bl #0x30e3ac
0x6337a4 ldr r1, [sp, #0x1c]
0x6337a8 bl #0x30ed6c
0x6337ac mov r1, r0
0x6337b0 mov r0, r7
0x6337b4 bl #0x30eba4
0x6337b8 bic r1, r0, #0x80000000
0x6337bc ldr r0, [sp, #0xc]
0x6337c0 bl #0x30ed6c
0x6337c4 bl #0x30ec64
0x6337c8 mov r1, r0
0x6337cc ldr r0, [sp, #4]
0x6337d0 bl #0x30ed6c
0x6337d4 b #0x6336d4
0x6337d8 ldr r1, [r5]
0x6337dc ldr r0, [r6, #0x30]
0x6337e0 bl #0x30e3ac
0x6337e4 ldr r1, [r5, #4]
0x6337e8 mov sl, r0
0x6337ec ldr r0, [r6, #0x34]
0x6337f0 bl #0x30e3ac
0x6337f4 ldr r1, [r5, #8]
0x6337f8 mov r8, r0
0x6337fc ldr r0, [r6, #0x38]
0x633800 bl #0x30e3ac
0x633804 mov r1, sl
0x633808 mov r7, r0
0x63380c mov r0, sl
0x633810 str sl, [sp, #0x14]
0x633814 str r8, [sp, #0x18]
0x633818 str r7, [sp, #0x1c]
0x63381c bl #0x30ed6c
0x633820 mov r1, r8
0x633824 mov sl, r0
0x633828 mov r0, r8
0x63382c bl #0x30ed6c
0x633830 mov r1, r0
0x633834 mov r0, sl
0x633838 bl #0x30eba4
0x63383c mov r1, r7
0x633840 mov r8, r0
0x633844 mov r0, r7
0x633848 bl #0x30ed6c
0x63384c mov r1, r0
0x633850 mov r0, r8
0x633854 bl #0x30eba4
0x633858 bl #0x30e8a4
0x63385c bl #0x30e1c0
0x633860 bl #0x30e6a0
0x633864 mov r1, #0
0x633868 mov r8, r0
0x63386c bl #0x30df8c
0x633870 cmp r0, #0
0x633874 bne #0x6338b8
0x633878 mov r1, r8
0x63387c mov r0, #0x3f800000
0x633880 bl #0x30ec94
0x633884 mov r7, r0
0x633888 mov r1, r0
0x63388c ldr r0, [sp, #0x14]
0x633890 bl #0x30ed6c
0x633894 mov r1, r7
0x633898 str r0, [sp, #0x14]
0x63389c ldr r0, [sp, #0x18]
0x6338a0 bl #0x30ed6c
0x6338a4 mov r1, r7
0x6338a8 str r0, [sp, #0x18]
0x6338ac ldr r0, [sp, #0x1c]
0x6338b0 bl #0x30ed6c
0x6338b4 str r0, [sp, #0x1c]
0x6338b8 mov r0, fp
0x6338bc mov r1, #0
0x6338c0 bl #0x30e2f8
0x6338c4 cmp r0, #0
0x6338c8 beq #0x6336d0
0x6338cc ldr r0, [sp, #0xc]
0x6338d0 mov r1, r8
0x6338d4 b #0x6337c0

FUNCTION 0x6338d8 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_8PGravityEE5applyEPS2_S5_PNS0_16IParticleContextIS2_EE
0x6338d8 add r0, r0, #0xc
0x6338dc b #0x633624

FUNCTION 0x634518 _ZN6glitch2ps11PForceProxyINS0_9SParticleENS0_10PDeflectorEE5applyEPS2_S5_PNS0_16IParticleContextIS2_EE
0x634518 add r0, r0, #0xc
0x63451c b #0x6338e0

FUNCTION 0x63a0ec _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE20_M_compute_next_sizeEj
0x63a0ec push {r4, r5, r6, lr}
0x63a0f0 ldm r0, {r2, r4}
0x63a0f4 movw r3, #0xffff
0x63a0f8 movt r3, #0x3fff
0x63a0fc rsb r4, r2, r4
0x63a100 asr r4, r4, #2
0x63a104 rsb r3, r4, r3
0x63a108 cmp r3, r1
0x63a10c mov r5, r1
0x63a110 blo #0x63a138
0x63a114 cmp r4, r5
0x63a118 addhs r0, r4, r4
0x63a11c addlo r0, r4, r5
0x63a120 cmn r0, #0xc0000001
0x63a124 bhi #0x63a130
0x63a128 cmp r0, r4
0x63a12c bhs #0x63a134
0x63a130 mvn r0, #0xc0000000
0x63a134 pop {r4, r5, r6, pc}
0x63a138 ldr r0, [pc, #8]
0x63a13c add r0, pc, r0
0x63a140 bl #0x708e40
0x63a144 b #0x63a114
0x63a148 eoreq r4, r8, ip, lsr #6

FUNCTION 0x63a6d8 _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE20_M_allocate_and_copyIPS4_EESC_RjT_SE_
0x63a6d8 push {r4, r5, r6, lr}
0x63a6dc ldr r0, [r1]
0x63a6e0 mov r1, #0
0x63a6e4 mov r4, r2
0x63a6e8 lsl r0, r0, #2
0x63a6ec mov r6, r3
0x63a6f0 bl #0x310568
0x63a6f4 cmp r4, r6
0x63a6f8 mov r5, r0
0x63a6fc beq #0x63a70c
0x63a700 mov r1, r4
0x63a704 rsb r2, r4, r6
0x63a708 bl #0x30e868
0x63a70c mov r0, r5
0x63a710 pop {r4, r5, r6, pc}

FUNCTION 0x63a714 _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE7reserveEj
0x63a714 push {r4, r5, r6, lr}
0x63a718 mov r4, r0
0x63a71c ldr r2, [r0]
0x63a720 ldr r0, [r0, #8]
0x63a724 sub sp, sp, #8
0x63a728 str r1, [sp, #4]
0x63a72c rsb r0, r2, r0
0x63a730 cmp r1, r0, asr #2
0x63a734 bls #0x63a784
0x63a738 cmn r1, #0xc0000001
0x63a73c bhi #0x63a78c
0x63a740 ldr r3, [r4, #4]
0x63a744 cmp r2, #0
0x63a748 rsb r5, r2, r3
0x63a74c asr r5, r5, #2
0x63a750 beq #0x63a7a0
0x63a754 mov r0, r4
0x63a758 add r1, sp, #4
0x63a75c bl #0x63a6d8
0x63a760 mov r6, r0
0x63a764 ldr r0, [r4]
0x63a768 bl #0x310450
0x63a76c ldr r3, [sp, #4]
0x63a770 add r5, r6, r5, lsl #2
0x63a774 str r5, [r4, #4]
0x63a778 add r3, r6, r3, lsl #2
0x63a77c str r3, [r4, #8]
0x63a780 str r6, [r4]
0x63a784 add sp, sp, #8
0x63a788 pop {r4, r5, r6, pc}
0x63a78c ldr r0, [pc, #0x24]
0x63a790 add r0, pc, r0
0x63a794 bl #0x708e40
0x63a798 ldr r2, [r4]
0x63a79c b #0x63a740
0x63a7a0 ldr r0, [sp, #4]
0x63a7a4 mov r1, r2
0x63a7a8 lsl r0, r0, #2
0x63a7ac bl #0x310568
0x63a7b0 mov r6, r0
0x63a7b4 b #0x63a76c

FUNCTION 0x63ade8 _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE18_M_fill_insert_auxEPS4_jRKS4_RKSt12__false_type
0x63ade8 push {r4, r5, r6, r7, r8, sl, lr}
0x63adec ldr ip, [r0]
0x63adf0 mov r5, r3
0x63adf4 sub sp, sp, #0x14
0x63adf8 cmp r3, ip
0x63adfc mov r4, r0
0x63ae00 mov r6, r1
0x63ae04 mov r3, r2
0x63ae08 ldrlo r7, [r0, #4]
0x63ae0c blo #0x63ae3c
0x63ae10 ldr r7, [r0, #4]
0x63ae14 cmp r5, r7
0x63ae18 bhs #0x63ae3c
0x63ae1c ldr ip, [r5]
0x63ae20 add r3, sp, #0x10
0x63ae24 str ip, [r3, #-8]!
0x63ae28 add ip, sp, #0xc
0x63ae2c str ip, [sp]
0x63ae30 bl #0x63ade8
0x63ae34 add sp, sp, #0x14
0x63ae38 pop {r4, r5, r6, r7, r8, sl, pc}
0x63ae3c rsb r2, r6, r7
0x63ae40 asr r8, r2, #2
0x63ae44 cmp r3, r8
0x63ae48 bhs #0x63aec0
0x63ae4c lsl r8, r3, #2
0x63ae50 rsb r3, r8, r7
0x63ae54 cmp r3, r7
0x63ae58 moveq sl, r7
0x63ae5c beq #0x63ae78
0x63ae60 mov r1, r3
0x63ae64 rsb r2, r3, r7
0x63ae68 mov r0, r7
0x63ae6c mov sl, r3
0x63ae70 bl #0x30e868
0x63ae74 ldr r3, [r4, #4]
0x63ae78 rsb r2, r6, sl
0x63ae7c add r3, r3, r8
0x63ae80 cmp r2, #0
0x63ae84 str r3, [r4, #4]
0x63ae88 ble #0x63ae98
0x63ae8c rsb r0, r2, r7
0x63ae90 mov r1, r6
0x63ae94 bl #0x30df38
0x63ae98 asr r8, r8, #2
0x63ae9c cmp r8, #0
0x63aea0 ble #0x63ae34
0x63aea4 mov r2, #0
0x63aea8 ldr r1, [r5]
0x63aeac str r1, [r6, r2, lsl #2]
0x63aeb0 add r2, r2, #1
0x63aeb4 cmp r2, r8
0x63aeb8 bne #0x63aea8
0x63aebc b #0x63ae34
0x63aec0 rsb r3, r8, r3
0x63aec4 sbfx sl, r3, #0, #0x1e
0x63aec8 cmp sl, #0
0x63aecc add r0, r7, r3, lsl #2
0x63aed0 ble #0x63aeec
0x63aed4 mov r1, #0
0x63aed8 ldr ip, [r5]
0x63aedc str ip, [r7, r1, lsl #2]
0x63aee0 add r1, r1, #1
0x63aee4 cmp r1, sl
0x63aee8 bne #0x63aed8
0x63aeec cmp r6, r7
0x63aef0 str r0, [r4, #4]
0x63aef4 beq #0x63af04
0x63aef8 mov r1, r6
0x63aefc bl #0x30e868
0x63af00 ldr r0, [r4, #4]
0x63af04 add r0, r0, r8, lsl #2
0x63af08 cmp r8, #0
0x63af0c str r0, [r4, #4]
0x63af10 ble #0x63ae34
0x63af14 mov r3, #0
0x63af18 ldr r2, [r5]
0x63af1c str r2, [r6, r3, lsl #2]
0x63af20 add r3, r3, #1
0x63af24 cmp r8, r3
0x63af28 bne #0x63af18
0x63af2c b #0x63ae34

FUNCTION 0x63d998 _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE14_M_fill_insertEPS4_jRKS4_
0x63d998 push {r4, r5, r6, r7, r8, sb, sl, lr}
0x63d99c subs r6, r2, #0
0x63d9a0 sub sp, sp, #0x10
0x63d9a4 mov r5, r0
0x63d9a8 mov r7, r1
0x63d9ac mov r4, r3
0x63d9b0 beq #0x63da50
0x63d9b4 ldmib r0, {ip, lr}
0x63d9b8 rsb ip, ip, lr
0x63d9bc cmp r6, ip, asr #2
0x63d9c0 bls #0x63da58
0x63d9c4 mov r1, r6
0x63d9c8 bl #0x63a0ec
0x63d9cc lsl sb, r0, #2
0x63d9d0 mov r1, #0
0x63d9d4 mov r0, sb
0x63d9d8 bl #0x310568
0x63d9dc ldr r1, [r5]
0x63d9e0 mov r8, r0
0x63d9e4 subs sl, r7, r1
0x63d9e8 moveq r0, r0
0x63d9ec beq #0x63d9fc
0x63d9f0 mov r2, sl
0x63d9f4 bl #0x30df38
0x63d9f8 add r0, r0, sl
0x63d9fc mov r2, r6
0x63da00 mov r3, #0
0x63da04 ldr r1, [r4]
0x63da08 subs r2, r2, #1
0x63da0c str r1, [r0, r3]
0x63da10 add r3, r3, #4
0x63da14 bne #0x63da04
0x63da18 ldr r3, [r5, #4]
0x63da1c add r0, r0, r6, lsl #2
0x63da20 subs r4, r3, r7
0x63da24 moveq r6, r0
0x63da28 beq #0x63da3c
0x63da2c mov r1, r7
0x63da30 mov r2, r4
0x63da34 bl #0x30df38
0x63da38 add r6, r0, r4
0x63da3c ldr r0, [r5]
0x63da40 add sb, r8, sb
0x63da44 bl #0x310450
0x63da48 stmib r5, {r6, sb}
0x63da4c str r8, [r5]
0x63da50 add sp, sp, #0x10
0x63da54 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0x63da58 add ip, sp, #0xc
0x63da5c str ip, [sp]
0x63da60 bl #0x63ade8
0x63da64 b #0x63da50

FUNCTION 0x63db08 _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE6resizeEjRKS4_
0x63db08 push {r4, r5}
0x63db0c ldr r4, [r0, #4]
0x63db10 ldr r5, [r0]
0x63db14 mov r3, r2
0x63db18 rsb r2, r5, r4
0x63db1c asr r2, r2, #2
0x63db20 cmp r1, r2
0x63db24 bhs #0x63db3c
0x63db28 add r5, r5, r1, lsl #2
0x63db2c cmp r5, r4
0x63db30 strne r5, [r0, #4]
0x63db34 pop {r4, r5}
0x63db38 bx lr
0x63db3c rsb r2, r2, r1
0x63db40 mov r1, r4
0x63db44 pop {r4, r5}
0x63db48 b #0x63d998

FUNCTION 0x64c234 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE11initPForcesEPS2_S4_
0x64c234 bx lr

FUNCTION 0x64c518 _ZNSt4priv9__find_ifIPPN6glitch2ps6PForceINS2_9SParticleEEENS2_12CompareForceIS4_EEEET_SA_SA_T0_RKSt26random_access_iterator_tag
0x64c518 mov r3, r0
0x64c51c rsb r0, r0, r1
0x64c520 asr ip, r0, #4
0x64c524 cmp ip, #0
0x64c528 push {r4, r5}
0x64c52c asr r4, r0, #2
0x64c530 movle r0, r3
0x64c534 ble #0x64c5e0
0x64c538 ldr r0, [r3]
0x64c53c ldr r4, [r2, #4]
0x64c540 ldr r0, [r0, #4]
0x64c544 cmp r4, r0
0x64c548 moveq r0, r3
0x64c54c beq #0x64c5fc
0x64c550 ldr r5, [r3, #4]
0x64c554 add r0, r3, #4
0x64c558 ldr r5, [r5, #4]
0x64c55c cmp r4, r5
0x64c560 beq #0x64c5fc
0x64c564 ldr r5, [r0, #4]!
0x64c568 ldr r5, [r5, #4]
0x64c56c cmp r4, r5
0x64c570 beq #0x64c5fc
0x64c574 ldr r5, [r0, #4]!
0x64c578 ldr r5, [r5, #4]
0x64c57c cmp r4, r5
0x64c580 bne #0x64c5cc
0x64c584 b #0x64c5fc
0x64c588 ldr r0, [r3, #0x10]
0x64c58c ldr r0, [r0, #4]
0x64c590 cmp r0, r4
0x64c594 beq #0x64c638
0x64c598 ldr r0, [r3, #0x14]
0x64c59c ldr r0, [r0, #4]
0x64c5a0 cmp r4, r0
0x64c5a4 beq #0x64c640
0x64c5a8 ldr r0, [r3, #0x18]
0x64c5ac ldr r0, [r0, #4]
0x64c5b0 cmp r4, r0
0x64c5b4 beq #0x64c648
0x64c5b8 ldr r0, [r3, #0x1c]
0x64c5bc add r3, r3, #0x10
0x64c5c0 ldr r0, [r0, #4]
0x64c5c4 cmp r4, r0
0x64c5c8 beq #0x64c650
0x64c5cc subs ip, ip, #1
0x64c5d0 bne #0x64c588
0x64c5d4 add r0, r3, #0x10
0x64c5d8 rsb r4, r0, r1
0x64c5dc asr r4, r4, #2
0x64c5e0 cmp r4, #2
0x64c5e4 beq #0x64c604
0x64c5e8 cmp r4, #3
0x64c5ec beq #0x64c658
0x64c5f0 cmp r4, #1
0x64c5f4 beq #0x64c630
0x64c5f8 mov r0, r1
0x64c5fc pop {r4, r5}
0x64c600 bx lr
0x64c604 ldr r3, [r2, #4]
0x64c608 ldr r2, [r0]
0x64c60c ldr r2, [r2, #4]
0x64c610 cmp r2, r3
0x64c614 beq #0x64c5fc
0x64c618 add r0, r0, #4
0x64c61c ldr r2, [r0]
0x64c620 ldr r2, [r2, #4]
0x64c624 cmp r2, r3
0x64c628 movne r0, r1
0x64c62c b #0x64c5fc
0x64c630 ldr r3, [r2, #4]
0x64c634 b #0x64c61c
0x64c638 add r0, r3, #0x10
0x64c63c b #0x64c5fc
0x64c640 add r0, r3, #0x14
0x64c644 b #0x64c5fc
0x64c648 add r0, r3, #0x18
0x64c64c b #0x64c5fc
0x64c650 add r0, r3, #0xc
0x64c654 b #0x64c5fc
0x64c658 ldr ip, [r0]
0x64c65c ldr r3, [r2, #4]
0x64c660 ldr r2, [ip, #4]
0x64c664 cmp r3, r2
0x64c668 beq #0x64c5fc
0x64c66c add r0, r0, #4
0x64c670 b #0x64c608

FUNCTION 0x64ca74 _ZSt11__push_heapIPPN6glitch2ps6PForceINS1_9SParticleEEEiS5_NS1_17SortPriorityForceIS3_EEEvT_T0_SA_T1_T2_
0x64ca74 cmp r1, r2
0x64ca78 push {r4, r5, r6, r7}
0x64ca7c ble #0x64cacc
0x64ca80 sub ip, r1, #1
0x64ca84 add ip, ip, ip, lsr #31
0x64ca88 ldr r5, [r3, #8]
0x64ca8c asr ip, ip, #1
0x64ca90 ldr r4, [r0, ip, lsl #2]
0x64ca94 ldr r6, [r4, #8]
0x64ca98 cmp r6, r5
0x64ca9c bge #0x64cacc
0x64caa0 sub r5, ip, #1
0x64caa4 add r5, r5, r5, lsr #31
0x64caa8 cmp r2, ip
0x64caac str r4, [r0, r1, lsl #2]
0x64cab0 asr r5, r5, #1
0x64cab4 mov r1, ip
0x64cab8 add r7, r0, ip, lsl #2
0x64cabc blt #0x64cad4
0x64cac0 str r3, [r7]
0x64cac4 pop {r4, r5, r6, r7}
0x64cac8 bx lr
0x64cacc add r7, r0, r1, lsl #2
0x64cad0 b #0x64cac0
0x64cad4 ldr r4, [r0, r5, lsl #2]
0x64cad8 ldr r6, [r3, #8]
0x64cadc mov ip, r5
0x64cae0 ldr r5, [r4, #8]
0x64cae4 cmp r5, r6
0x64cae8 bge #0x64cac0
0x64caec b #0x64caa0

FUNCTION 0x64caf0 _ZSt13__adjust_heapIPPN6glitch2ps6PForceINS1_9SParticleEEEiS5_NS1_17SortPriorityForceIS3_EEEvT_T0_SA_T1_T2_
0x64caf0 push {r4, r5, r6, r7, r8, lr}
0x64caf4 mov ip, r1
0x64caf8 add r1, r1, #1
0x64cafc lsl lr, r1, #1
0x64cb00 cmp lr, r2
0x64cb04 sub sp, sp, #0x10
0x64cb08 movge r1, ip
0x64cb0c bge #0x64cb4c
0x64cb10 mov r6, ip
0x64cb14 sub r1, lr, #1
0x64cb18 ldr r4, [r0, lr, lsl #2]
0x64cb1c ldr r5, [r0, r1, lsl #2]
0x64cb20 ldr r8, [r4, #8]
0x64cb24 ldr r7, [r5, #8]
0x64cb28 cmp r8, r7
0x64cb2c movge r1, lr
0x64cb30 add lr, r1, #1
0x64cb34 lsl lr, lr, #1
0x64cb38 movlt r4, r5
0x64cb3c cmp r2, lr
0x64cb40 str r4, [r0, r6, lsl #2]
0x64cb44 mov r6, r1
0x64cb48 bgt #0x64cb14
0x64cb4c cmp lr, r2
0x64cb50 subeq lr, lr, #1
0x64cb54 ldreq r2, [r0, lr, lsl #2]
0x64cb58 streq r2, [r0, r1, lsl #2]
0x64cb5c mov r2, ip
0x64cb60 moveq r1, lr
0x64cb64 add ip, sp, #0xc
0x64cb68 str ip, [sp]
0x64cb6c bl #0x64ca74
0x64cb70 add sp, sp, #0x10
0x64cb74 pop {r4, r5, r6, r7, r8, pc}

FUNCTION 0x64cb78 _ZSt11__make_heapIPPN6glitch2ps6PForceINS1_9SParticleEEENS1_17SortPriorityForceIS3_EES5_iEvT_S9_T0_PT1_PT2_
0x64cb78 push {r4, r5, r6, r7, r8, sl, lr}
0x64cb7c rsb r1, r0, r1
0x64cb80 cmp r1, #7
0x64cb84 sub sp, sp, #0x14
0x64cb88 mov r6, r0
0x64cb8c ble #0x64cbd4
0x64cb90 asr r8, r1, #2
0x64cb94 sub r4, r8, #2
0x64cb98 asr r4, r4, #1
0x64cb9c mov r5, #0
0x64cba0 add sl, sp, #0xc
0x64cba4 add r7, r0, r4, lsl #2
0x64cba8 b #0x64cbb0
0x64cbac sub r4, r4, #1
0x64cbb0 ldr r3, [r7, r5]
0x64cbb4 mov r1, r4
0x64cbb8 mov r0, r6
0x64cbbc mov r2, r8
0x64cbc0 str sl, [sp]
0x64cbc4 bl #0x64caf0
0x64cbc8 cmp r4, #0
0x64cbcc sub r5, r5, #4
0x64cbd0 bne #0x64cbac
0x64cbd4 add sp, sp, #0x14
0x64cbd8 pop {r4, r5, r6, r7, r8, sl, pc}

FUNCTION 0x64cbdc _ZSt9sort_heapIPPN6glitch2ps6PForceINS1_9SParticleEEENS1_17SortPriorityForceIS3_EEEvT_S9_T0_
0x64cbdc push {r4, r5, r6, r7, lr}
0x64cbe0 rsb r5, r0, r1
0x64cbe4 cmp r5, #7
0x64cbe8 sub sp, sp, #0x14
0x64cbec mov r6, r0
0x64cbf0 ble #0x64cc28
0x64cbf4 mov r4, r1
0x64cbf8 add r7, sp, #0xc
0x64cbfc ldr r2, [r6]
0x64cc00 sub r5, r5, #4
0x64cc04 ldr r3, [r4, #-4]
0x64cc08 mov r0, r6
0x64cc0c str r2, [r4, #-4]!
0x64cc10 mov r1, #0
0x64cc14 asr r2, r5, #2
0x64cc18 str r7, [sp]
0x64cc1c bl #0x64caf0
0x64cc20 cmp r5, #7
0x64cc24 bgt #0x64cbfc
0x64cc28 add sp, sp, #0x14
0x64cc2c pop {r4, r5, r6, r7, pc}

FUNCTION 0x64cc30 _ZNSt4priv14__partial_sortIPPN6glitch2ps6PForceINS2_9SParticleEEES6_NS2_17SortPriorityForceIS4_EEEEvT_SA_SA_PT0_T1_
0x64cc30 push {r4, r5, r6, r7, r8, sl, lr}
0x64cc34 mov ip, #0
0x64cc38 sub sp, sp, #0x1c
0x64cc3c mov r6, r2
0x64cc40 mov r7, r1
0x64cc44 mov r3, ip
0x64cc48 add r2, sp, #0x10
0x64cc4c str ip, [sp]
0x64cc50 mov r5, r0
0x64cc54 bl #0x64cb78
0x64cc58 cmp r7, r6
0x64cc5c bhs #0x64ccbc
0x64cc60 rsb r8, r5, r7
0x64cc64 asr r8, r8, #2
0x64cc68 mov r4, r7
0x64cc6c add sl, sp, #0xc
0x64cc70 b #0x64cc80
0x64cc74 add r4, r4, #4
0x64cc78 cmp r6, r4
0x64cc7c bls #0x64ccbc
0x64cc80 ldr r3, [r4]
0x64cc84 ldr r2, [r5]
0x64cc88 ldr r0, [r3, #8]
0x64cc8c ldr r1, [r2, #8]
0x64cc90 cmp r0, r1
0x64cc94 bge #0x64cc74
0x64cc98 str r2, [r4]
0x64cc9c mov r0, r5
0x64cca0 mov r1, #0
0x64cca4 mov r2, r8
0x64cca8 add r4, r4, #4
0x64ccac str sl, [sp]
0x64ccb0 bl #0x64caf0
0x64ccb4 cmp r6, r4
0x64ccb8 bhi #0x64cc80
0x64ccbc mov r0, r5
0x64ccc0 mov r1, r7
0x64ccc4 add r2, sp, #0x14
0x64ccc8 bl #0x64cbdc
0x64cccc add sp, sp, #0x1c
0x64ccd0 pop {r4, r5, r6, r7, r8, sl, pc}

FUNCTION 0x64ccd4 _ZNSt4priv16__introsort_loopIPPN6glitch2ps6PForceINS2_9SParticleEEES6_iNS2_17SortPriorityForceIS4_EEEEvT_SA_PT0_T1_T2_
0x64ccd4 push {r4, r5, r6, r7, r8, sb, sl, lr}
0x64ccd8 rsb r2, r0, r1
0x64ccdc cmp r2, #0x43
0x64cce0 sub sp, sp, #0x10
0x64cce4 mov r5, r0
0x64cce8 mov r6, r3
0x64ccec ble #0x64cdfc
0x64ccf0 cmp r3, #0
0x64ccf4 addne r7, sp, #0xc
0x64ccf8 beq #0x64cde4
0x64ccfc asr r2, r2, #3
0x64cd00 ldr r4, [r5, r2, lsl #2]
0x64cd04 ldr r0, [r5]
0x64cd08 sub r6, r6, #1
0x64cd0c ldr r3, [r4, #8]
0x64cd10 ldr lr, [r0, #8]
0x64cd14 cmp lr, r3
0x64cd18 bge #0x64ce04
0x64cd1c ldr sl, [r1, #-4]
0x64cd20 ldr r2, [sl, #8]
0x64cd24 cmp r3, r2
0x64cd28 blt #0x64ce1c
0x64cd2c cmp lr, r2
0x64cd30 bge #0x64ce30
0x64cd34 mov sb, r2
0x64cd38 mov ip, sl
0x64cd3c mov r3, r1
0x64cd40 mov r8, r5
0x64cd44 cmp lr, r2
0x64cd48 movge r4, r8
0x64cd4c bge #0x64cd64
0x64cd50 mov r4, r8
0x64cd54 ldr r0, [r4, #4]!
0x64cd58 ldr lr, [r0, #8]
0x64cd5c cmp lr, r2
0x64cd60 blt #0x64cd54
0x64cd64 cmp sb, r2
0x64cd68 sub lr, r3, #4
0x64cd6c ble #0x64cd88
0x64cd70 ldr ip, [r3, #-8]
0x64cd74 sub r3, r3, #4
0x64cd78 ldr lr, [ip, #8]
0x64cd7c cmp lr, r2
0x64cd80 bgt #0x64cd70
0x64cd84 sub lr, r3, #4
0x64cd88 cmp lr, r4
0x64cd8c bls #0x64cdb8
0x64cd90 mov r8, r4
0x64cd94 str ip, [r8], #4
0x64cd98 str r0, [lr]
0x64cd9c ldr ip, [lr, #-4]
0x64cda0 ldr r0, [r4, #4]
0x64cda4 mov r3, lr
0x64cda8 ldr sb, [ip, #8]
0x64cdac ldr lr, [r0, #8]
0x64cdb0 ldr r2, [sl, #8]
0x64cdb4 b #0x64cd44
0x64cdb8 mov r2, #0
0x64cdbc mov r0, r4
0x64cdc0 mov r3, r6
0x64cdc4 str r7, [sp]
0x64cdc8 bl #0x64ccd4
0x64cdcc rsb r2, r5, r4
0x64cdd0 cmp r2, #0x43
0x64cdd4 ble #0x64cdfc
0x64cdd8 cmp r6, #0
0x64cddc mov r1, r4
0x64cde0 bne #0x64ccfc
0x64cde4 add ip, sp, #8
0x64cde8 mov r0, r5
0x64cdec mov r2, r1
0x64cdf0 mov r3, #0
0x64cdf4 str ip, [sp]
0x64cdf8 bl #0x64cc30
0x64cdfc add sp, sp, #0x10
0x64ce00 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0x64ce04 ldr sl, [r1, #-4]
0x64ce08 ldr r2, [sl, #8]
0x64ce0c cmp lr, r2
0x64ce10 blt #0x64ce30
0x64ce14 cmp r3, r2
0x64ce18 blt #0x64cd34
0x64ce1c mov sb, r2
0x64ce20 mov ip, sl
0x64ce24 mov r2, r3
0x64ce28 mov sl, r4
0x64ce2c b #0x64cd3c
0x64ce30 mov sb, r2
0x64ce34 mov ip, sl
0x64ce38 mov r2, lr
0x64ce3c mov sl, r0
0x64ce40 b #0x64cd3c

FUNCTION 0x64d64c _ZN6glitch2ps12PForcesModelINS0_9SParticleEED1Ev
0x64d64c ldr r2, [pc, #0x78]
0x64d650 ldr r3, [pc, #0x78]
0x64d654 push {r4, r5, r6, r7, r8, lr}
0x64d658 add r2, pc, r2
0x64d65c ldr r3, [r2, r3]
0x64d660 mov r7, r0
0x64d664 mov r6, r0
0x64d668 add r2, r3, #0xc
0x64d66c str r2, [r7], #0x14
0x64d670 ldmib r0, {r4, r5}
0x64d674 add r3, r3, #0xc4
0x64d678 str r3, [r0, #0x14]
0x64d67c cmp r4, r5
0x64d680 beq #0x64d6ac
0x64d684 ldr r3, [r4]
0x64d688 add r4, r4, #4
0x64d68c cmp r3, #0
0x64d690 mov r0, r3
0x64d694 beq #0x64d6a4
0x64d698 ldr r3, [r3]
0x64d69c mov lr, pc
0x64d6a0 ldr pc, [r3, #4]
0x64d6a4 cmp r5, r4
0x64d6a8 bne #0x64d684
0x64d6ac ldr r0, [r6, #4]
0x64d6b0 cmp r0, #0
0x64d6b4 beq #0x64d6bc
0x64d6b8 bl #0x310450
0x64d6bc mov r0, r7
0x64d6c0 bl #0x64d298
0x64d6c4 mov r0, r6
0x64d6c8 pop {r4, r5, r6, r7, r8, pc}
0x64d6cc eorseq r7, r4, r8, lsr r4
0x64d6d0 andeq r1, r0, ip, lsl r6

FUNCTION 0x64d6e4 _ZN6glitch2ps12PForcesModelINS0_9SParticleEED0Ev
0x64d6e4 push {r4, lr}
0x64d6e8 mov r4, r0
0x64d6ec bl #0x64d64c
0x64d6f0 mov r0, r4
0x64d6f4 bl #0x30e2b0
0x64d6f8 mov r0, r4
0x64d6fc pop {r4, pc}

FUNCTION 0x64d710 _ZN6glitch2ps12PForcesModelINS0_9SParticleEED2Ev
0x64d710 push {r4, r5, r6, lr}
0x64d714 ldr r3, [r1]
0x64d718 mov r6, r0
0x64d71c str r3, [r0]
0x64d720 ldr r3, [r3, #-0xc]
0x64d724 ldr r2, [r1, #4]
0x64d728 str r2, [r0, r3]
0x64d72c ldmib r0, {r4, r5}
0x64d730 cmp r4, r5
0x64d734 beq #0x64d760
0x64d738 ldr r3, [r4]
0x64d73c add r4, r4, #4
0x64d740 cmp r3, #0
0x64d744 mov r0, r3
0x64d748 beq #0x64d758
0x64d74c ldr r3, [r3]
0x64d750 mov lr, pc
0x64d754 ldr pc, [r3, #4]
0x64d758 cmp r5, r4
0x64d75c bne #0x64d738
0x64d760 ldr r0, [r6, #4]
0x64d764 cmp r0, #0
0x64d768 beq #0x64d770
0x64d76c bl #0x310450
0x64d770 mov r0, r6
0x64d774 pop {r4, r5, r6, pc}

FUNCTION 0x64d77c _ZNSt4priv16__insertion_sortIPPN6glitch2ps6PForceINS2_9SParticleEEES6_NS2_17SortPriorityForceIS4_EEEEvT_SA_PT0_T1_
0x64d77c cmp r0, r1
0x64d780 push {r4, r5, r6, r7, r8, sb, sl, lr}
0x64d784 mov r7, r0
0x64d788 mov sl, r1
0x64d78c beq #0x64d7f0
0x64d790 add r3, r0, #4
0x64d794 cmp r1, r3
0x64d798 beq #0x64d7f0
0x64d79c add r5, r0, #8
0x64d7a0 mov r6, #4
0x64d7a4 ldr r4, [r5, #-4]
0x64d7a8 ldr r3, [r7]
0x64d7ac sub r1, r5, #4
0x64d7b0 ldr r2, [r4, #8]
0x64d7b4 ldr r3, [r3, #8]
0x64d7b8 cmp r2, r3
0x64d7bc bge #0x64d7f4
0x64d7c0 cmp r6, #0
0x64d7c4 mov r8, r5
0x64d7c8 ble #0x64d7dc
0x64d7cc rsb r0, r6, r5
0x64d7d0 mov r1, r7
0x64d7d4 mov r2, r6
0x64d7d8 bl #0x30df38
0x64d7dc str r4, [r7]
0x64d7e0 cmp sl, r8
0x64d7e4 add r5, r5, #4
0x64d7e8 add r6, r6, #4
0x64d7ec bne #0x64d7a4
0x64d7f0 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0x64d7f4 ldr r3, [r5, #-8]
0x64d7f8 ldr r0, [r3, #8]
0x64d7fc cmp r2, r0
0x64d800 bge #0x64d828
0x64d804 sub r2, r5, #8
0x64d808 str r3, [r1]
0x64d80c mov r0, r2
0x64d810 ldr r3, [r2, #-4]!
0x64d814 ldr r8, [r4, #8]
0x64d818 mov r1, r0
0x64d81c ldr ip, [r3, #8]
0x64d820 cmp r8, ip
0x64d824 blt #0x64d808
0x64d828 str r4, [r1]
0x64d82c mov r8, r5
0x64d830 b #0x64d7e0

FUNCTION 0x64d834 _ZNSt4priv22__final_insertion_sortIPPN6glitch2ps6PForceINS2_9SParticleEEENS2_17SortPriorityForceIS4_EEEEvT_SA_T0_
0x64d834 push {r4, r5, r6, r7, lr}
0x64d838 rsb r3, r0, r1
0x64d83c cmp r3, #0x43
0x64d840 sub sp, sp, #0xc
0x64d844 mov r4, r0
0x64d848 mov r5, r1
0x64d84c ble #0x64d8c4
0x64d850 add r4, r0, #0x40
0x64d854 mov r1, r4
0x64d858 mov r2, #0
0x64d85c add r3, sp, #4
0x64d860 bl #0x64d77c
0x64d864 cmp r5, r4
0x64d868 beq #0x64d8bc
0x64d86c ldmda r4, {r3, r7}
0x64d870 ldr r1, [r7, #8]
0x64d874 ldr r2, [r3, #8]
0x64d878 cmp r1, r2
0x64d87c movge r1, r4
0x64d880 bge #0x64d8ac
0x64d884 sub r2, r4, #4
0x64d888 mov r0, r4
0x64d88c str r3, [r0]
0x64d890 mov r1, r2
0x64d894 ldr r3, [r2, #-4]!
0x64d898 ldr r6, [r7, #8]
0x64d89c mov r0, r1
0x64d8a0 ldr ip, [r3, #8]
0x64d8a4 cmp r6, ip
0x64d8a8 blt #0x64d88c
0x64d8ac add r4, r4, #4
0x64d8b0 cmp r5, r4
0x64d8b4 str r7, [r1]
0x64d8b8 bne #0x64d86c
0x64d8bc add sp, sp, #0xc
0x64d8c0 pop {r4, r5, r6, r7, pc}
0x64d8c4 mov r2, #0
0x64d8c8 mov r3, sp
0x64d8cc bl #0x64d77c
0x64d8d0 b #0x64d8bc

FUNCTION 0x64d8d4 _ZSt4sortIPPN6glitch2ps6PForceINS1_9SParticleEEENS1_17SortPriorityForceIS3_EEEvT_S9_T0_
0x64d8d4 push {r4, r5, lr}
0x64d8d8 cmp r0, r1
0x64d8dc sub sp, sp, #0x14
0x64d8e0 mov r5, r0
0x64d8e4 mov r4, r1
0x64d8e8 beq #0x64d940
0x64d8ec rsb r2, r0, r1
0x64d8f0 asr r2, r2, #2
0x64d8f4 cmp r2, #1
0x64d8f8 moveq r3, #0
0x64d8fc beq #0x64d918
0x64d900 mov r3, #0
0x64d904 asr r2, r2, #1
0x64d908 cmp r2, #1
0x64d90c add r3, r3, #1
0x64d910 bne #0x64d904
0x64d914 lsl r3, r3, #1
0x64d918 mov r2, #0
0x64d91c mov r0, r5
0x64d920 mov r1, r4
0x64d924 add ip, sp, #0xc
0x64d928 str ip, [sp]
0x64d92c bl #0x64ccd4
0x64d930 mov r0, r5
0x64d934 mov r1, r4
0x64d938 add r2, sp, #8
0x64d93c bl #0x64d834
0x64d940 add sp, sp, #0x14
0x64d944 pop {r4, r5, pc}

FUNCTION 0x64d948 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE12applyPForcesEPS2_S4_
0x64d948 push {r4, r5, r6, r7, r8, lr}
0x64d94c ldrb r3, [r0, #0x10]
0x64d950 sub sp, sp, #8
0x64d954 mov r5, r0
0x64d958 cmp r3, #0
0x64d95c mov r8, r1
0x64d960 mov r7, r2
0x64d964 beq #0x64d980
0x64d968 ldr r0, [r0, #4]
0x64d96c ldr r1, [r5, #8]
0x64d970 add r2, sp, #4
0x64d974 bl #0x64d8d4
0x64d978 mov r3, #0
0x64d97c strb r3, [r5, #0x10]
0x64d980 ldmib r5, {r4, r6}
0x64d984 cmp r4, r6
0x64d988 beq #0x64d9bc
0x64d98c ldr r3, [r5]
0x64d990 ldr r2, [r4], #4
0x64d994 mov r1, r8
0x64d998 ldr r3, [r3, #-0xc]
0x64d99c mov r0, r2
0x64d9a0 ldr ip, [r2]
0x64d9a4 add r3, r5, r3
0x64d9a8 mov r2, r7
0x64d9ac mov lr, pc
0x64d9b0 ldr pc, [ip, #8]
0x64d9b4 cmp r4, r6
0x64d9b8 bne #0x64d98c
0x64d9bc add sp, sp, #8
0x64d9c0 pop {r4, r5, r6, r7, r8, pc}

FUNCTION 0x64d9d4 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE16initPForcesModelEv
0x64d9d4 push {r4, lr}
0x64d9d8 ldrb r3, [r0, #0x10]
0x64d9dc sub sp, sp, #8
0x64d9e0 mov r4, r0
0x64d9e4 cmp r3, #0
0x64d9e8 beq #0x64da04
0x64d9ec ldr r0, [r0, #4]
0x64d9f0 ldr r1, [r4, #8]
0x64d9f4 add r2, sp, #4
0x64d9f8 bl #0x64d8d4
0x64d9fc mov r3, #0
0x64da00 strb r3, [r4, #0x10]
0x64da04 add sp, sp, #8
0x64da08 pop {r4, pc}

FUNCTION 0x64f190 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE9addPForceEPNS0_6PForceIS2_EE
0x64f190 push {r4, r5, r6, r7, r8, lr}
0x64f194 ldr r8, [r0, #8]
0x64f198 ldr r3, [r0, #0xc]
0x64f19c mov r4, r0
0x64f1a0 mov r5, r1
0x64f1a4 cmp r8, r3
0x64f1a8 beq #0x64f1d4
0x64f1ac str r1, [r8]
0x64f1b0 ldr r8, [r0, #8]
0x64f1b4 mov r2, #1
0x64f1b8 add r8, r8, #4
0x64f1bc str r8, [r0, #8]
0x64f1c0 ldr r3, [r4, #4]
0x64f1c4 strb r2, [r4, #0x10]
0x64f1c8 rsb r8, r3, r8
0x64f1cc asr r0, r8, #2
0x64f1d0 pop {r4, r5, r6, r7, r8, pc}
0x64f1d4 ldr r3, [r0, #4]
0x64f1d8 rsb r3, r3, r8
0x64f1dc asr r3, r3, #2
0x64f1e0 cmp r3, #1
0x64f1e4 addhs r7, r3, r3
0x64f1e8 addlo r7, r3, #1
0x64f1ec cmn r7, #0xc0000001
0x64f1f0 bhi #0x64f260
0x64f1f4 cmp r3, r7
0x64f1f8 lslls r7, r7, #2
0x64f1fc bhi #0x64f260
0x64f200 mov r1, #0
0x64f204 mov r0, r7
0x64f208 bl #0x310568
0x64f20c ldr r1, [r4, #4]
0x64f210 mov r6, r0
0x64f214 subs r8, r8, r1
0x64f218 moveq r8, r0
0x64f21c beq #0x64f22c
0x64f220 mov r2, r8
0x64f224 bl #0x30df38
0x64f228 add r8, r0, r8
0x64f22c str r5, [r8], #4
0x64f230 ldr r0, [r4, #4]
0x64f234 bl #0x310450
0x64f238 str r6, [r4, #4]
0x64f23c ldr r3, [r4, #4]
0x64f240 add r7, r6, r7
0x64f244 str r8, [r4, #8]
0x64f248 mov r2, #1
0x64f24c rsb r8, r3, r8
0x64f250 str r7, [r4, #0xc]
0x64f254 strb r2, [r4, #0x10]
0x64f258 asr r0, r8, #2
0x64f25c pop {r4, r5, r6, r7, r8, pc}
0x64f260 mvn r7, #3
0x64f264 b #0x64f200

FUNCTION 0x64f278 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE12removePForceEPNS0_6PForceIS2_EE
0x64f278 push {r4, r5, lr}
0x64f27c mov r4, r0
0x64f280 sub sp, sp, #0xc
0x64f284 mov r2, r1
0x64f288 ldr r0, [r0, #4]
0x64f28c ldr r1, [r4, #8]
0x64f290 add r3, sp, #4
0x64f294 bl #0x64c518
0x64f298 mov r5, r0
0x64f29c ldr r0, [r4, #8]
0x64f2a0 cmp r5, r0
0x64f2a4 beq #0x64f2f0
0x64f2a8 ldr r3, [r5]
0x64f2ac cmp r3, #0
0x64f2b0 beq #0x64f2c8
0x64f2b4 mov r0, r3
0x64f2b8 ldr r3, [r3]
0x64f2bc mov lr, pc
0x64f2c0 ldr pc, [r3, #4]
0x64f2c4 ldr r0, [r4, #8]
0x64f2c8 add r1, r5, #4
0x64f2cc cmp r1, r0
0x64f2d0 beq #0x64f2e0
0x64f2d4 subs r2, r0, r1
0x64f2d8 moveq r1, r0
0x64f2dc bne #0x64f2f8
0x64f2e0 sub r1, r1, #4
0x64f2e4 mov r3, #1
0x64f2e8 strb r3, [r4, #0x10]
0x64f2ec str r1, [r4, #8]
0x64f2f0 add sp, sp, #0xc
0x64f2f4 pop {r4, r5, pc}
0x64f2f8 mov r0, r5
0x64f2fc bl #0x30df38
0x64f300 ldr r1, [r4, #8]
0x64f304 b #0x64f2e0

FUNCTION 0x64f318 _ZN6glitch2ps12PForcesModelINS0_9SParticleEE12removePForceEi
0x64f318 cmp r1, #0
0x64f31c push {r4, r5, r6, lr}
0x64f320 mov r4, r0
0x64f324 ble #0x64f388
0x64f328 ldr r0, [r0, #8]
0x64f32c ldr r5, [r4, #4]
0x64f330 rsb r3, r5, r0
0x64f334 cmp r1, r3, asr #2
0x64f338 bge #0x64f388
0x64f33c ldr r3, [r5, r1, lsl #2]
0x64f340 add r5, r5, r1, lsl #2
0x64f344 cmp r3, #0
0x64f348 beq #0x64f360
0x64f34c mov r0, r3
0x64f350 ldr r3, [r3]
0x64f354 mov lr, pc
0x64f358 ldr pc, [r3, #4]
0x64f35c ldr r0, [r4, #8]
0x64f360 add r1, r5, #4
0x64f364 cmp r0, r1
0x64f368 beq #0x64f380
0x64f36c subs r2, r0, r1
0x64f370 beq #0x64f380
0x64f374 mov r0, r5
0x64f378 bl #0x30df38
0x64f37c ldr r0, [r4, #8]
0x64f380 sub r0, r0, #4
0x64f384 str r0, [r4, #8]
0x64f388 pop {r4, r5, r6, pc}

FUNCTION 0x6c1f8c _ZN6glitch5scene24CParticleSystemSceneNode21createGravityAffectorERKNS_4core8vector3dIfEEj
0x6c1f8c push {r4, r5, r6, lr}
0x6c1f90 mov r0, #0x20
0x6c1f94 mov r5, r1
0x6c1f98 mov r1, #0
0x6c1f9c mov r6, r2
0x6c1fa0 bl #0x5341ac
0x6c1fa4 mov r1, r5
0x6c1fa8 mov r4, r0
0x6c1fac mov r2, r6
0x6c1fb0 bl #0x6fab1c
0x6c1fb4 mov r0, r4
0x6c1fb8 pop {r4, r5, r6, pc}

FUNCTION 0x6ca6dc _ZN6glitch5scene35CSceneNodeAnimatorCollisionResponse10setGravityERKNS_4core8vector3dIfEE
0x6ca6dc ldr r3, [r1]
0x6ca6e0 str r3, [r0, #0x24]
0x6ca6e4 ldr r3, [r1, #4]
0x6ca6e8 str r3, [r0, #0x28]
0x6ca6ec ldr r3, [r1, #8]
0x6ca6f0 str r3, [r0, #0x2c]
0x6ca6f4 bx lr

FUNCTION 0x6ca6f8 _ZNK6glitch5scene35CSceneNodeAnimatorCollisionResponse10getGravityEv
0x6ca6f8 ldr r2, [r1, #0x24]
0x6ca6fc str r2, [r0]
0x6ca700 ldr r2, [r1, #0x28]
0x6ca704 str r2, [r0, #4]
0x6ca708 ldr r2, [r1, #0x2c]
0x6ca70c str r2, [r0, #8]
0x6ca710 bx lr

FUNCTION 0x6fa9f4 _ZNK6glitch5scene24IParticleGravityAffector7getTypeEv
0x6fa9f4 mov r0, #3
0x6fa9f8 bx lr

FUNCTION 0x6fa9fc _ZN6glitch5scene24CParticleGravityAffector16setTimeForceLostEf
0x6fa9fc str r1, [r0, #8]
0x6faa00 bx lr

FUNCTION 0x6faa04 _ZN6glitch5scene24CParticleGravityAffector10setGravityERKNS_4core8vector3dIfEE
0x6faa04 ldr r3, [r1]
0x6faa08 str r3, [r0, #0xc]
0x6faa0c ldr r3, [r1, #4]
0x6faa10 str r3, [r0, #0x10]
0x6faa14 ldr r3, [r1, #8]
0x6faa18 str r3, [r0, #0x14]
0x6faa1c bx lr

FUNCTION 0x6faa20 _ZNK6glitch5scene24CParticleGravityAffector16getTimeForceLostEv
0x6faa20 ldr r0, [r0, #8]
0x6faa24 bx lr

FUNCTION 0x6faa28 _ZNK6glitch5scene24CParticleGravityAffector10getGravityEv
0x6faa28 add r0, r0, #0xc
0x6faa2c bx lr

FUNCTION 0x6faa30 _ZN6glitch5scene24IParticleGravityAffectorD1Ev
0x6faa30 bx lr

FUNCTION 0x6faa54 _ZN6glitch5scene24CParticleGravityAffectorC2ERKNS_4core8vector3dIfEEj
0x6faa54 push {r4, r5, r6, r7, r8, lr}
0x6faa58 mov r4, r0
0x6faa5c mov r0, #0
0x6faa60 str r0, [r4]
0x6faa64 str r0, [r4, #4]
0x6faa68 str r0, [r4, #0xc]
0x6faa6c str r0, [r4, #8]
0x6faa70 add ip, r1, #4
0x6faa74 ldr r0, [ip, #4]
0x6faa78 add lr, ip, #4
0x6faa7c mov r7, r2
0x6faa80 str r0, [r4]
0x6faa84 ldr r5, [r0, #-0x1c]
0x6faa88 ldr r6, [lr, #4]
0x6faa8c mov r0, r3
0x6faa90 str r6, [r4, r5]
0x6faa94 ldr r3, [r4]
0x6faa98 ldr r2, [lr, #8]
0x6faa9c ldr r3, [r3, #-0xc]
0x6faaa0 str r2, [r4, r3]
0x6faaa4 mov r3, #1
0x6faaa8 strb r3, [r4, #4]
0x6faaac ldr r3, [r1, #4]
0x6faab0 str r3, [r4]
0x6faab4 ldr r2, [ip, #0x10]
0x6faab8 ldr r3, [r3, #-0x1c]
0x6faabc str r2, [r4, r3]
0x6faac0 ldr r3, [r4]
0x6faac4 ldr r2, [ip, #0x14]
0x6faac8 ldr r3, [r3, #-0xc]
0x6faacc str r2, [r4, r3]
0x6faad0 ldr r3, [r1]
0x6faad4 str r3, [r4]
0x6faad8 ldr r2, [r1, #0x1c]
0x6faadc ldr r3, [r3, #-0x1c]
0x6faae0 str r2, [r4, r3]
0x6faae4 ldr r3, [r4]
0x6faae8 ldr r2, [r1, #0x20]
0x6faaec ldr r3, [r3, #-0xc]
0x6faaf0 str r2, [r4, r3]
0x6faaf4 bl #0x30e2e0
0x6faaf8 str r0, [r4, #8]
0x6faafc ldr r3, [r7]
0x6fab00 mov r0, r4
0x6fab04 str r3, [r4, #0xc]
0x6fab08 ldr r3, [r7, #4]
0x6fab0c str r3, [r4, #0x10]
0x6fab10 ldr r3, [r7, #8]
0x6fab14 str r3, [r4, #0x14]
0x6fab18 pop {r4, r5, r6, r7, r8, pc}

FUNCTION 0x6fab1c _ZN6glitch5scene24CParticleGravityAffectorC1ERKNS_4core8vector3dIfEEj
0x6fab1c ldr ip, [pc, #0xe0]
0x6fab20 ldr r3, [pc, #0xe0]
0x6fab24 push {r4, r5, r6, r7, r8, sb, sl, lr}
0x6fab28 add ip, pc, ip
0x6fab2c ldr lr, [pc, #0xd8]
0x6fab30 ldr r3, [ip, r3]
0x6fab34 mov r4, r0
0x6fab38 ldr lr, [ip, lr]
0x6fab3c ldr r0, [r3, #0x24]
0x6fab40 mov r6, #1
0x6fab44 add lr, lr, #8
0x6fab48 str r0, [r4]
0x6fab4c str lr, [r4, #0x18]
0x6fab50 str r6, [r4, #0x1c]
0x6fab54 ldr r7, [r0, #-0xc]
0x6fab58 ldr lr, [r3, #8]
0x6fab5c ldr r8, [r3, #0x28]
0x6fab60 mov r0, #0
0x6fab64 ldr r5, [r3, #0xc]
0x6fab68 str r8, [r4, r7]
0x6fab6c str lr, [r4]
0x6fab70 str r0, [r4, #4]
0x6fab74 str r0, [r4, #0xc]
0x6fab78 str r0, [r4, #8]
0x6fab7c ldr lr, [lr, #-0x1c]
0x6fab80 ldr r0, [r3, #4]
0x6fab84 ldr sl, [r3, #0x10]
0x6fab88 str r5, [r4, lr]
0x6fab8c ldr r5, [r4]
0x6fab90 ldr r7, [r3, #0x14]
0x6fab94 ldr lr, [pc, #0x74]
0x6fab98 ldr r8, [r5, #-0xc]
0x6fab9c ldr r5, [r3, #0x18]
0x6faba0 ldr lr, [ip, lr]
0x6faba4 str sl, [r4, r8]
0x6faba8 str r0, [r4]
0x6fabac strb r6, [r4, #4]
0x6fabb0 ldr r6, [r0, #-0x1c]
0x6fabb4 mov r0, r2
0x6fabb8 add r3, lr, #0x60
0x6fabbc str r7, [r4, r6]
0x6fabc0 ldr r2, [r4]
0x6fabc4 add lr, lr, #0x1c
0x6fabc8 mov r6, r1
0x6fabcc ldr r2, [r2, #-0xc]
0x6fabd0 str r5, [r4, r2]
0x6fabd4 str lr, [r4]
0x6fabd8 str r3, [r4, #0x18]
0x6fabdc bl #0x30e2e0
0x6fabe0 str r0, [r4, #8]
0x6fabe4 ldr r3, [r6]
0x6fabe8 mov r0, r4
0x6fabec str r3, [r4, #0xc]
0x6fabf0 ldr r3, [r6, #4]
0x6fabf4 str r3, [r4, #0x10]
0x6fabf8 ldr r3, [r6, #8]
0x6fabfc str r3, [r4, #0x14]
0x6fac00 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0x6fac04 eoreq sb, sb, r8, ror #30
0x6fac08 andeq r4, r0, r8, lsr #12
0x6fac0c andeq r2, r0, r4, asr #22
0x6fac10 andeq r1, r0, ip, asr #30

FUNCTION 0x6fac14 _ZN6glitch5scene24CParticleGravityAffector6affectEjPNS0_9SParticleEj
0x6fac14 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0x6fac18 mov r4, r0
0x6fac1c ldrb r0, [r0, #4]
0x6fac20 sub sp, sp, #0xc
0x6fac24 str r1, [sp, #4]
0x6fac28 cmp r0, #0
0x6fac2c mov fp, r3
0x6fac30 beq #0x6fad34
0x6fac34 cmp r3, #0
0x6fac38 beq #0x6fad34
0x6fac3c mov r5, r2
0x6fac40 mov r7, #0
0x6fac44 ldr r3, [sp, #4]
0x6fac48 ldr r0, [r5, #0x18]
0x6fac4c rsb r0, r0, r3
0x6fac50 bl #0x30e2e0
0x6fac54 ldr r1, [r4, #8]
0x6fac58 bl #0x30ec94
0x6fac5c mov r1, #0x3f800000
0x6fac60 mov r6, r0
0x6fac64 bl #0x30e2f8
0x6fac68 cmp r0, #0
0x6fac6c mov r1, #0
0x6fac70 mov r0, r6
0x6fac74 movne r6, #0x3f800000
0x6fac78 bne #0x6fac88
0x6fac7c bl #0x30e70c
0x6fac80 cmp r0, #0
0x6fac84 movne r6, #0
0x6fac88 mov r1, r6
0x6fac8c mov r0, #0x3f800000
0x6fac90 bl #0x30e3ac
0x6fac94 ldr r8, [r5, #0x2c]
0x6fac98 mov r6, r0
0x6fac9c ldr r0, [r4, #0x10]
0x6faca0 mov r1, r8
0x6faca4 bl #0x30e3ac
0x6faca8 mov r1, r0
0x6facac mov r0, r6
0x6facb0 bl #0x30ed6c
0x6facb4 mov r1, r0
0x6facb8 mov r0, r8
0x6facbc bl #0x30eba4
0x6facc0 ldr sl, [r5, #0x30]
0x6facc4 mov sb, r0
0x6facc8 ldr r0, [r4, #0x14]
0x6faccc mov r1, sl
0x6facd0 bl #0x30e3ac
0x6facd4 mov r1, r0
0x6facd8 mov r0, r6
0x6facdc bl #0x30ed6c
0x6face0 mov r1, r0
0x6face4 mov r0, sl
0x6face8 bl #0x30eba4
0x6facec ldr r8, [r5, #0x28]
0x6facf0 mov sl, r0
0x6facf4 ldr r0, [r4, #0xc]
0x6facf8 mov r1, r8
0x6facfc bl #0x30e3ac
0x6fad00 mov r1, r0
0x6fad04 mov r0, r6
0x6fad08 bl #0x30ed6c
0x6fad0c mov r1, r0
0x6fad10 mov r0, r8
0x6fad14 bl #0x30eba4
0x6fad18 add r7, r7, #1
0x6fad1c cmp r7, fp
0x6fad20 str r0, [r5, #0xc]
0x6fad24 str sb, [r5, #0x10]
0x6fad28 str sl, [r5, #0x14]
0x6fad2c add r5, r5, #0x44
0x6fad30 bne #0x6fac44
0x6fad34 add sp, sp, #0xc
0x6fad38 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

FUNCTION 0x6fad3c _ZNK6glitch5scene24CParticleGravityAffector19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
0x6fad3c push {r4, r5, r6, lr}
0x6fad40 mov r4, r1
0x6fad44 ldr r1, [pc, #0x40]
0x6fad48 mov r5, r0
0x6fad4c add r2, r5, #0xc
0x6fad50 mov r0, r4
0x6fad54 ldr ip, [r4]
0x6fad58 add r1, pc, r1
0x6fad5c mov r3, #0
0x6fad60 mov lr, pc
0x6fad64 ldr pc, [ip, #0x1a8]
0x6fad68 ldr r1, [pc, #0x20]
0x6fad6c mov r0, r4
0x6fad70 ldr r2, [r5, #8]
0x6fad74 add r1, pc, r1
0x6fad78 ldr ip, [r4]
0x6fad7c mov r3, #0
0x6fad80 mov lr, pc
0x6fad84 ldr pc, [ip, #0x64]
0x6fad88 pop {r4, r5, r6, pc}
0x6fad8c andseq r0, pc, r8, ror r6
0x6fad90 andseq r7, pc, ip, lsl r0

FUNCTION 0x6fad94 _ZN6glitch5scene24CParticleGravityAffectorD1Ev
0x6fad94 bx lr

FUNCTION 0x6fadd8 _ZN6glitch5scene24CParticleGravityAffector21deserializeAttributesEiPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
0x6fadd8 push {r4, r5, r6, lr}
0x6faddc mov r6, r0
0x6fade0 sub sp, sp, #0x10
0x6fade4 ldr r3, [r2]
0x6fade8 mov r0, r2
0x6fadec mov r4, r2
0x6fadf0 mov r5, r1
0x6fadf4 mov lr, pc
0x6fadf8 ldr pc, [r3, #0x10]
0x6fadfc cmp r0, #0
0x6fae00 beq #0x6fae18
0x6fae04 ldr r1, [pc, #0x9c]
0x6fae08 add r1, pc, r1
0x6fae0c bl #0x30e31c
0x6fae10 cmp r0, #0
0x6fae14 beq #0x6fae24
0x6fae18 mov r0, r5
0x6fae1c add sp, sp, #0x10
0x6fae20 pop {r4, r5, r6, pc}
0x6fae24 mov r2, r5
0x6fae28 add r0, sp, #4
0x6fae2c mov r1, r4
0x6fae30 ldr r3, [r4]
0x6fae34 mov lr, pc
0x6fae38 ldr pc, [r3, #0x1b8]
0x6fae3c ldr r3, [sp, #0xc]
0x6fae40 ldr r1, [sp, #4]
0x6fae44 ldr r2, [sp, #8]
0x6fae48 add r5, r5, #1
0x6fae4c str r1, [r6, #0xc]
0x6fae50 str r2, [r6, #0x10]
0x6fae54 str r3, [r6, #0x14]
0x6fae58 ldr r3, [r4]
0x6fae5c mov r0, r4
0x6fae60 mov r1, r5
0x6fae64 mov lr, pc
0x6fae68 ldr pc, [r3, #0x10]
0x6fae6c cmp r0, #0
0x6fae70 beq #0x6fae18
0x6fae74 ldr r1, [pc, #0x30]
0x6fae78 add r1, pc, r1
0x6fae7c bl #0x30e31c
0x6fae80 cmp r0, #0
0x6fae84 bne #0x6fae18
0x6fae88 mov r1, r5
0x6fae8c mov r0, r4
0x6fae90 ldr r3, [r4]
0x6fae94 mov lr, pc
0x6fae98 ldr pc, [r3, #0x74]
0x6fae9c add r5, r5, #1
0x6faea0 str r0, [r6, #8]
0x6faea4 b #0x6fae18
0x6faea8 andseq r0, pc, r8, asr #11
0x6faeac andseq r6, pc, r8, lsl pc

FUNCTION 0x6faeb0 _ZN6glitch5scene24IParticleGravityAffectorD0Ev
0x6faeb0 ldr r3, [pc, #0x40]
0x6faeb4 ldr r1, [pc, #0x40]
0x6faeb8 ldr r2, [pc, #0x40]
0x6faebc add r3, pc, r3
0x6faec0 ldr r1, [r3, r1]
0x6faec4 push {r4, lr}
0x6faec8 ldr r2, [r3, r2]
0x6faecc ldr ip, [r1, #4]
0x6faed0 ldr r1, [r1, #8]
0x6faed4 add r2, r2, #0x60
0x6faed8 str ip, [r0]
0x6faedc str r2, [r0, #8]
0x6faee0 ldr r2, [ip, #-0x1c]
0x6faee4 mov r4, r0
0x6faee8 str r1, [r0, r2]
0x6faeec bl #0x30e2b0
0x6faef0 mov r0, r4
0x6faef4 pop {r4, pc}

FUNCTION 0x6faf24 _ZN6glitch5scene24CParticleGravityAffectorD0Ev
0x6faf24 ldr r3, [pc, #0x64]
0x6faf28 ldr r2, [pc, #0x64]
0x6faf2c ldr r1, [pc, #0x64]
0x6faf30 add r3, pc, r3
0x6faf34 ldr r2, [r3, r2]
0x6faf38 push {r4, r5, r6, lr}
0x6faf3c ldr r1, [r3, r1]
0x6faf40 ldr ip, [r2, #4]
0x6faf44 ldr r5, [r2, #0x14]
0x6faf48 add r1, r1, #0x60
0x6faf4c str ip, [r0]
0x6faf50 str r1, [r0, #0x18]
0x6faf54 ldr lr, [ip, #-0x1c]
0x6faf58 ldr r1, [r2, #8]
0x6faf5c ldr ip, [r2, #0x18]
0x6faf60 str r5, [r0, lr]
0x6faf64 ldr lr, [r0]
0x6faf68 ldr r2, [r2, #0xc]
0x6faf6c mov r4, r0
0x6faf70 ldr r3, [lr, #-0xc]
0x6faf74 str ip, [r0, r3]
0x6faf78 str r1, [r0]
0x6faf7c ldr r3, [r1, #-0x1c]
0x6faf80 str r2, [r0, r3]
0x6faf84 bl #0x30e2b0
0x6faf88 mov r0, r4
0x6faf8c pop {r4, r5, r6, pc}
0x6faf90 eoreq sb, sb, r0, ror #22
0x6faf94 andeq r4, r0, r8, lsr #12
0x6faf98 andeq r1, r0, ip, asr #30

FUNCTION 0x97a9a8 _ZTCN6glitch7collada15particle_system24CDeflectorForceSceneNodeE0_NS1_15CForceSceneNodeE
0x97a9a8 andeq r0, r0, r0
0x97a9ac andeq r0, r0, r0
0x97a9b0 andeq r0, r0, r0
0x97a9b4 andeq r0, r0, r0
0x97a9b8 andeq r0, r0, r0, ror #2
0x97a9bc andeq r0, r0, r0
0x97a9c0 andeq r0, r0, r0
0x97a9c4 subseq r7, sb, r4, lsr #5
0x97a9c8 subseq r8, sb, r8, asr r0
0x97a9cc rsbeq r0, r3, r8, ror #14
0x97a9d0 rsbeq r0, r3, r0, ror #15
0x97a9d4 subseq r6, sb, r8, lsl #26
0x97a9d8 subseq r6, sb, ip, ror #26
0x97a9dc subseq r6, sb, r0, lsl sp

FUNCTION 0x97ad80 _ZTTN6glitch7collada15particle_system15CForceSceneNodeE
0x97ad80 addseq sl, r7, r4, ror #24
0x97ad84 ldrheq sl, [r7], ip
0x97ad88 ldrheq sl, [r7], ip
0x97ad8c addseq sl, r7, r0, asr #29
0x97ad90 addseq sl, r7, r4, ror #24
0x97ad94 addseq sl, r7, r0, ror sp
0x97ad98 ldrsbeq sl, [r7], ip
0x97ad9c ldrsheq sl, [r7], ip

FUNCTION 0x97ada0 _ZTCN6glitch7collada15particle_system15CForceSceneNodeE0_NS_5scene10ISceneNodeE
0x97ada0 andeq r0, r0, r0
0x97ada4 andeq r0, r0, r0
0x97ada8 andeq r0, r0, r0
0x97adac andeq r0, r0, r0
0x97adb0 andeq r0, r0, r0, asr #2
0x97adb4 andeq r0, r0, r0
0x97adb8 andeq r0, r0, r0
0x97adbc subseq r7, sb, r4, lsr #5
0x97adc0 subseq r8, sb, r8, asr r0
0x97adc4 subseq r8, sb, r4, lsl #23
0x97adc8 subseq r8, sb, r0, lsr #25
0x97adcc subseq r6, sb, r8, lsl #26
0x97add0 subseq r6, sb, ip, ror #26
0x97add4 subseq r6, sb, r0, lsl sp
0x97add8 andeq r0, r0, r0
0x97addc subseq r6, sb, r4, lsr #28
0x97ade0 subseq r6, sb, ip, lsr #28
0x97ade4 subseq r8, sb, r4, lsl #20
0x97ade8 subseq r7, sb, r0, asr pc
0x97adec andeq r0, r0, r0
0x97adf0 subseq r7, sb, ip, lsl #14
0x97adf4 eorseq sl, r5, r0, ror #17
0x97adf8 subseq sb, sb, r4, asr #8
0x97adfc subseq r8, sb, r8, lsl #18
0x97ae00 subseq sb, sb, ip, lsl r0
0x97ae04 subseq r6, sb, r4, asr #29
0x97ae08 subseq r6, sb, ip, asr pc
0x97ae0c subseq r6, sb, r4, ror #30
0x97ae10 subseq r6, sb, r4, ror #31
0x97ae14 ldrsheq r6, [sb], #-0xf4
0x97ae18 subseq r8, sb, r4, ror #16
0x97ae1c subseq r7, sb, r4
0x97ae20 subseq r8, sb, r8, ror #15
0x97ae24 subseq r7, sb, ip, rrx
0x97ae28 subseq r8, sb, r0, ror r7
0x97ae2c subseq r8, sb, r8, ror #13
0x97ae30 subseq r8, sb, r8, asr r6
0x97ae34 subseq r7, sb, r8, asr sp
0x97ae38 subseq r7, sb, r4, ror #31
0x97ae3c subseq r7, sb, r8, ror #30
0x97ae40 subseq r7, sb, r4, lsr #1
0x97ae44 ldrheq r7, [sb], #-0
0x97ae48 ldrheq r7, [sb], #-8
0x97ae4c ldrheq r7, [sb], #-0xc
0x97ae50 subseq r7, sb, r4, asr #1
0x97ae54 subseq r7, sb, ip, ror #1
0x97ae58 ldrsheq r7, [sb], #-4
0x97ae5c subseq r7, sb, r4, lsr #2
0x97ae60 subseq r7, sb, ip, lsr #2
0x97ae64 ldrsbeq r7, [sb], #-0x10
0x97ae68 ldrsbeq r7, [sb], #-0x18
0x97ae6c subseq r7, sb, r4, asr r2
0x97ae70 subseq r7, sb, ip, asr r2
0x97ae74 subseq r7, sb, r0, ror #24

FUNCTION 0x97aed0 _ZTCN6glitch7collada15particle_system15CForceSceneNodeE0_NS_2io26IAttributeExchangingObjectE
0x97aed0 andeq r0, r0, r0, asr #2
0x97aed4 andeq r0, r0, r0
0x97aed8 andeq r0, r0, r0
0x97aedc eorseq r0, r5, ip, ror ip
0x97aee0 eorseq r0, r5, r0, lsl #25
0x97aee4 eorseq r0, r5, r4, lsl #25
0x97aee8 eorseq r1, r5, ip, asr #13
0x97aeec andeq r0, r0, r0

FUNCTION 0x97b0d0 _ZTCN6glitch7collada15particle_system19CWindForceSceneNodeE0_NS1_15CForceSceneNodeE
0x97b0d0 andeq r0, r0, r0
0x97b0d4 andeq r0, r0, r0
0x97b0d8 andeq r0, r0, r0
0x97b0dc andeq r0, r0, r0
0x97b0e0 andeq r0, r0, ip, asr r1
0x97b0e4 andeq r0, r0, r0
0x97b0e8 andeq r0, r0, r0
0x97b0ec subseq r7, sb, r4, lsr #5
0x97b0f0 subseq r8, sb, r8, asr r0
0x97b0f4 rsbeq r0, r3, r8, ror #14
0x97b0f8 rsbeq r0, r3, r0, ror #15
0x97b0fc subseq r6, sb, r8, lsl #26
0x97b100 subseq r6, sb, ip, ror #26
0x97b104 subseq r6, sb, r0, lsl sp

FUNCTION 0x97b3a0 _ZTTN6glitch7collada15particle_system22CGravityForceSceneNodeE
0x97b3a0 addseq fp, r7, ip, ror #7
0x97b3a4 addseq fp, r7, r4, lsr #10
0x97b3a8 addseq fp, r7, ip, asr r6
0x97b3ac addseq fp, r7, ip, asr r6
0x97b3b0 addseq fp, r7, r0, ror #14
0x97b3b4 addseq fp, r7, r4, lsr #10
0x97b3b8 addseq fp, r7, r0, lsr r6
0x97b3bc addseq fp, r7, ip, ror #7
0x97b3c0 ldrsheq fp, [r7], r8
0x97b3c4 addseq fp, r7, ip, ror r7
0x97b3c8 umullseq fp, r7, ip, r7

FUNCTION 0x97b508 _ZTCN6glitch7collada15particle_system22CGravityForceSceneNodeE0_NS1_15CForceSceneNodeE
0x97b508 andeq r0, r0, r0
0x97b50c andeq r0, r0, r0
0x97b510 andeq r0, r0, r0
0x97b514 andeq r0, r0, r0
0x97b518 andeq r0, r0, r0, asr r1
0x97b51c andeq r0, r0, r0
0x97b520 andeq r0, r0, r0
0x97b524 subseq r7, sb, r4, lsr #5
0x97b528 subseq r8, sb, r8, asr r0
0x97b52c rsbeq r0, r3, r8, ror #14
0x97b530 rsbeq r0, r3, r0, ror #15
0x97b534 subseq r6, sb, r8, lsl #26
0x97b538 subseq r6, sb, ip, ror #26
0x97b53c subseq r6, sb, r0, lsl sp

FUNCTION 0x97b640 _ZTCN6glitch7collada15particle_system22CGravityForceSceneNodeE0_NS_5scene10ISceneNodeE
0x97b640 andeq r0, r0, r0
0x97b644 andeq r0, r0, r0
0x97b648 andeq r0, r0, r0
0x97b64c andeq r0, r0, r0
0x97b650 andeq r0, r0, r0, asr r1
0x97b654 andeq r0, r0, r0
0x97b658 andeq r0, r0, r0
0x97b65c subseq r7, sb, r4, lsr #5
0x97b660 subseq r8, sb, r8, asr r0
0x97b664 subseq r8, sb, r4, lsl #23
0x97b668 subseq r8, sb, r0, lsr #25
0x97b66c subseq r6, sb, r8, lsl #26
0x97b670 subseq r6, sb, ip, ror #26
0x97b674 subseq r6, sb, r0, lsl sp
0x97b678 andeq r0, r0, r0
0x97b67c subseq r6, sb, r4, lsr #28
0x97b680 subseq r6, sb, ip, lsr #28
0x97b684 subseq r8, sb, r4, lsl #20
0x97b688 subseq r7, sb, r0, asr pc
0x97b68c andeq r0, r0, r0
0x97b690 subseq r7, sb, ip, lsl #14
0x97b694 eorseq sl, r5, r0, ror #17
0x97b698 subseq sb, sb, r4, asr #8
0x97b69c subseq r8, sb, r8, lsl #18
0x97b6a0 subseq sb, sb, ip, lsl r0
0x97b6a4 subseq r6, sb, r4, asr #29
0x97b6a8 subseq r6, sb, ip, asr pc
0x97b6ac subseq r6, sb, r4, ror #30
0x97b6b0 subseq r6, sb, r4, ror #31
0x97b6b4 ldrsheq r6, [sb], #-0xf4
0x97b6b8 subseq r8, sb, r4, ror #16
0x97b6bc subseq r7, sb, r4
0x97b6c0 subseq r8, sb, r8, ror #15
0x97b6c4 subseq r7, sb, ip, rrx
0x97b6c8 subseq r8, sb, r0, ror r7
0x97b6cc subseq r8, sb, r8, ror #13
0x97b6d0 subseq r8, sb, r8, asr r6
0x97b6d4 subseq r7, sb, r8, asr sp
0x97b6d8 subseq r7, sb, r4, ror #31
0x97b6dc subseq r7, sb, r8, ror #30
0x97b6e0 subseq r7, sb, r4, lsr #1
0x97b6e4 ldrheq r7, [sb], #-0
0x97b6e8 ldrheq r7, [sb], #-8
0x97b6ec ldrheq r7, [sb], #-0xc
0x97b6f0 subseq r7, sb, r4, asr #1
0x97b6f4 subseq r7, sb, ip, ror #1
0x97b6f8 ldrsheq r7, [sb], #-4
0x97b6fc subseq r7, sb, r4, lsr #2
0x97b700 subseq r7, sb, ip, lsr #2
0x97b704 ldrsbeq r7, [sb], #-0x10
0x97b708 ldrsbeq r7, [sb], #-0x18
0x97b70c subseq r7, sb, r4, asr r2
0x97b710 subseq r7, sb, ip, asr r2
0x97b714 subseq r7, sb, r0, ror #24

FUNCTION 0x97b770 _ZTCN6glitch7collada15particle_system22CGravityForceSceneNodeE0_NS_2io26IAttributeExchangingObjectE
0x97b770 andeq r0, r0, r0, asr r1
0x97b774 andeq r0, r0, r0
0x97b778 andeq r0, r0, r0
0x97b77c eorseq r0, r5, ip, ror ip
0x97b780 eorseq r0, r5, r0, lsl #25
0x97b784 eorseq r0, r5, r4, lsl #25
0x97b788 eorseq r1, r5, ip, asr #13
0x97b78c andeq r0, r0, r0

FUNCTION 0x981240 _ZTTN6glitch2ps12PForcesModelINS0_9SParticleEEE
0x981240 ldrsheq r1, [r8], r4
0x981244 addseq r1, r8, ip, lsr #3

FUNCTION 0x98d8f8 _ZTTN6glitch5scene24IParticleGravityAffectorE
0x98d8f8 addseq sp, r8, r4, lsr #17
0x98d8fc addseq sp, r8, r4, lsr sb
0x98d900 addseq sp, r8, r4, lsr sb
0x98d904 addseq sp, r8, r8, ror #18
0x98d908 addseq sp, r8, r4, lsr #17
0x98d90c addseq sp, r8, r8, ror #17
0x98d910 addseq sp, r8, r4, lsl #19
0x98d914 addseq sp, r8, r4, lsr #19

FUNCTION 0x98d918 _ZTCN6glitch5scene24IParticleGravityAffectorE0_NS0_17IParticleAffectorE
0x98d918 andeq r0, r0, r0
0x98d91c andeq r0, r0, r0
0x98d920 andeq r0, r0, r0
0x98d924 andeq r0, r0, r0
0x98d928 andeq r0, r0, r8
0x98d92c andeq r0, r0, r0
0x98d930 andeq r0, r0, r0
0x98d934 rsbeq r8, pc, ip, ror r3
0x98d938 eorseq r0, r5, r0, lsl #25
0x98d93c rsbeq r8, pc, r4, lsl r4
0x98d940 rsbeq r8, pc, r8, ror r6
0x98d944 andeq r0, r0, r0
0x98d948 rsbeq r8, pc, ip, ror #6
0x98d94c rsbeq r8, pc, r4, ror r3
0x98d950 mlseq pc, r0, r3, r8
0x98d954 andeq r0, r0, r0
0x98d958 andeq r0, r0, r0

FUNCTION 0x98d978 _ZTCN6glitch5scene24IParticleGravityAffectorE0_NS_2io26IAttributeExchangingObjectE
0x98d978 andeq r0, r0, r8
0x98d97c andeq r0, r0, r0
0x98d980 andeq r0, r0, r0
0x98d984 eorseq r0, r5, ip, ror ip
0x98d988 eorseq r0, r5, r0, lsl #25
0x98d98c eorseq r0, r5, r4, lsl #25
0x98d990 eorseq r1, r5, ip, asr #13
0x98d994 andeq r0, r0, r0

FUNCTION 0x98da20 _ZTTN6glitch5scene24CParticleGravityAffectorE
0x98da20 addseq sp, r8, ip, asr #19
0x98da24 addseq sp, r8, ip, ror #20
0x98da28 ldrsbeq sp, [r8], ip
0x98da2c ldrsbeq sp, [r8], ip
0x98da30 addseq sp, r8, r0, lsl fp
0x98da34 addseq sp, r8, ip, ror #20
0x98da38 ldrheq sp, [r8], r0
0x98da3c addseq sp, r8, ip, asr #19
0x98da40 addseq sp, r8, r0, lsl sl
0x98da44 addseq sp, r8, ip, lsr #22
0x98da48 addseq sp, r8, ip, asr #22

FUNCTION 0x98da50 _ZTCN6glitch5scene24CParticleGravityAffectorE0_NS0_24IParticleGravityAffectorE
0x98da50 andeq r0, r0, r0
0x98da54 andeq r0, r0, r0
0x98da58 andeq r0, r0, r0
0x98da5c andeq r0, r0, r0
0x98da60 andeq r0, r0, r8, lsl r0
0x98da64 andeq r0, r0, r0
0x98da68 andeq r0, r0, r0
0x98da6c rsbeq r8, pc, ip, ror r3
0x98da70 eorseq r0, r5, r0, lsl #25
0x98da74 rsbeq sl, pc, r0, lsr sl
0x98da78 strhteq sl, [pc], #-0xe0
0x98da7c andeq r0, r0, r0
0x98da80 rsbeq r8, pc, ip, ror #6
0x98da84 rsbeq r8, pc, r4, ror r3
0x98da88 mlseq pc, r0, r3, r8

FUNCTION 0x98dac0 _ZTCN6glitch5scene24CParticleGravityAffectorE0_NS0_17IParticleAffectorE
0x98dac0 andeq r0, r0, r0
0x98dac4 andeq r0, r0, r0
0x98dac8 andeq r0, r0, r0
0x98dacc andeq r0, r0, r0
0x98dad0 andeq r0, r0, r8, lsl r0
0x98dad4 andeq r0, r0, r0
0x98dad8 andeq r0, r0, r0
0x98dadc rsbeq r8, pc, ip, ror r3
0x98dae0 eorseq r0, r5, r0, lsl #25
0x98dae4 rsbeq r8, pc, r4, lsl r4
0x98dae8 rsbeq r8, pc, r8, ror r6
0x98daec andeq r0, r0, r0
0x98daf0 rsbeq r8, pc, ip, ror #6
0x98daf4 rsbeq r8, pc, r4, ror r3
0x98daf8 mlseq pc, r0, r3, r8
0x98dafc andeq r0, r0, r0
0x98db00 andeq r0, r0, r0

FUNCTION 0x98db20 _ZTCN6glitch5scene24CParticleGravityAffectorE0_NS_2io26IAttributeExchangingObjectE
0x98db20 andeq r0, r0, r8, lsl r0
0x98db24 andeq r0, r0, r0
0x98db28 andeq r0, r0, r0
0x98db2c eorseq r0, r5, ip, ror ip
0x98db30 eorseq r0, r5, r0, lsl #25
0x98db34 eorseq r0, r5, r4, lsl #25
0x98db38 eorseq r1, r5, ip, asr #13
0x98db3c andeq r0, r0, r0

FUNCTION 0x99f694 _ZGVZNK6glitch7collada15particle_system15CForceSceneNode14getBoundingBoxEvE5empty
0x99f694 andeq r0, r0, r0

FUNCTION 0x9f6f34 _ZZNK6glitch7collada15particle_system15CForceSceneNode14getBoundingBoxEvE5empty
0x9f6f34 andeq r0, r0, r0
0x9f6f38 andeq r0, r0, r0
0x9f6f3c andeq r0, r0, r0
0x9f6f40 andeq r0, r0, r0
0x9f6f44 andeq r0, r0, r0
0x9f6f48 andeq r0, r0, r0
