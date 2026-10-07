Original ELF 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80

AnimatedFX::HasCompleted 004924e0
004924e0 push {r4, lr}
004924e4 ldr r3, [pc, #0x5c]
004924e8 ldr r1, [pc, #0x5c]
004924ec ldr r2, [r0, #0x2c]
004924f0 add r3, pc, r3
004924f4 ldr r0, [r3, r1]
004924f8 movw r1, #0x6164
004924fc movt r1, #0x7065
00492500 ldr r0, [r0, #0x10]
00492504 ldr r2, [r2, #8]
00492508 ldr r3, [r0, #0x1c]
0049250c mov r0, r3
00492510 ldr r3, [r3]
00492514 mov lr, pc
00492518 ldr pc, [r3, #0x1c]
0049251c subs r3, r0, #0
00492520 beq #0x492540
00492524 ldr r3, [r3]
00492528 mov lr, pc
0049252c ldr pc, [r3, #0xf8]
00492530 cmp r0, #0
00492534 movgt r0, #0
00492538 movle r0, #1
0049253c pop {r4, pc}
00492540 mov r0, #1
00492544 pop {r4, pc}
00492548 subseq r2, r0, r0, lsr #11
0049254c strdeq r3, r4, [r0], -r4

CParticleSystemSceneNode::getParticleCount<SParticle100> 0064bb90
0064bb90 ldr r2, [r0, #0x178]
0064bb94 movw r3, #0x5c29
0064bb98 movt r3, #0xc28f
0064bb9c ldr r1, [r2]
0064bba0 ldr r1, [r1, #-0xc]
0064bba4 add r2, r2, r1
0064bba8 ldr r1, [r2, #0x24]
0064bbac ldr r0, [r2, #0x28]
0064bbb0 rsb r0, r1, r0
0064bbb4 asr r0, r0, #2
0064bbb8 mul r0, r3, r0
0064bbbc bx lr

CGlitchNewParticleSystemSceneNode::getParticleCount<GNPSParticle> 00637b48
00637b48 ldr r2, [r0, #0x178]
00637b4c movw r3, #0x6f97
00637b50 movt r3, #0x96f9
00637b54 ldr r1, [r2]
00637b58 ldr r1, [r1, #-0xc]
00637b5c add r2, r2, r1
00637b60 ldr r1, [r2, #0x24]
00637b64 ldr r0, [r2, #0x28]
00637b68 rsb r0, r1, r0
00637b6c asr r0, r0, #2
00637b70 mul r0, r3, r0
00637b74 bx lr
