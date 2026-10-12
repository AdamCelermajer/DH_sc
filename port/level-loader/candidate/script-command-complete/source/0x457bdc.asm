00457bdc 10402de9 push {r4, lr}
00457be0 0010a0e3 mov r1, #0
00457be4 1000a0e3 mov r0, #0x10
00457be8 60e2faeb bl #0x310570
00457bec 28409fe5 ldr r4, [pc, #0x28]
00457bf0 28209fe5 ldr r2, [pc, #0x28]
00457bf4 0010a0e3 mov r1, #0
00457bf8 04408fe0 add r4, pc, r4
00457bfc 022094e7 ldr r2, [r4, r2]
00457c00 0c1080e5 str r1, [r0, #0xc]
00457c04 0410c0e5 strb r1, [r0, #4]
00457c08 082082e2 add r2, r2, #8
00457c0c 002080e5 str r2, [r0]
00457c10 0020e0e3 mvn r2, #0
00457c14 082080e5 str r2, [r0, #8]
00457c18 1080bde8 pop {r4, pc}
