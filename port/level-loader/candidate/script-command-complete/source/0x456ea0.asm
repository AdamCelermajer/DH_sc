00456ea0 10402de9 push {r4, lr}
00456ea4 0010a0e3 mov r1, #0
00456ea8 0c00a0e3 mov r0, #0xc
00456eac afe5faeb bl #0x310570
00456eb0 20409fe5 ldr r4, [pc, #0x20]
00456eb4 20209fe5 ldr r2, [pc, #0x20]
00456eb8 0010a0e3 mov r1, #0
00456ebc 04408fe0 add r4, pc, r4
00456ec0 022094e7 ldr r2, [r4, r2]
00456ec4 0810c0e5 strb r1, [r0, #8]
00456ec8 041080e5 str r1, [r0, #4]
00456ecc 082082e2 add r2, r2, #8
00456ed0 002080e5 str r2, [r0]
00456ed4 1080bde8 pop {r4, pc}
00456ed8 d4db5300 ldrsbeq sp, [r3], #-0xb4
00456edc 50220000 andeq r2, r0, r0, asr r2
