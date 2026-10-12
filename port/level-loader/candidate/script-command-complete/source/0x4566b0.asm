004566b0 10402de9 push {r4, lr}
004566b4 0010a0e3 mov r1, #0
004566b8 0c00a0e3 mov r0, #0xc
004566bc abe7faeb bl #0x310570
004566c0 20409fe5 ldr r4, [pc, #0x20]
004566c4 20209fe5 ldr r2, [pc, #0x20]
004566c8 0010a0e3 mov r1, #0
004566cc 04408fe0 add r4, pc, r4
004566d0 022094e7 ldr r2, [r4, r2]
004566d4 081080e5 str r1, [r0, #8]
004566d8 041080e5 str r1, [r0, #4]
004566dc 082082e2 add r2, r2, #8
004566e0 002080e5 str r2, [r0]
004566e4 1080bde8 pop {r4, pc}
004566e8 c4e35300 subseq lr, r3, r4, asr #7
004566ec dc140000 ldrdeq r1, r2, [r0], -ip
