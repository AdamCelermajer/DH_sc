# _ZN12TiXmlPrinterC2Ev 0x485aa0 size=148
00485aa0: ldr r3, [pc, #0x7c]
00485aa4: ldr r1, [pc, #0x7c]
00485aa8: push {r4, r5, lr}
00485aac: add r3, pc, r3
00485ab0: ldr r1, [r3, r1]
00485ab4: mov r4, r0
00485ab8: add r2, r0, #0xc
00485abc: mov r5, #0
00485ac0: add r1, r1, #8
00485ac4: str r1, [r0]
00485ac8: sub sp, sp, #0xc
00485acc: mov r0, r2
00485ad0: str r2, [r4, #0x1c]
00485ad4: str r2, [r4, #0x20]
00485ad8: str r5, [r4, #4]
00485adc: strb r5, [r4, #8]
00485ae0: mov r1, #0x10
00485ae4: bl #0x31167c ; _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
00485ae8: ldr r3, [r4, #0x1c]
00485aec: ldr r1, [pc, #0x38]
00485af0: add r2, sp, #4
00485af4: strb r5, [r3]
00485af8: add r1, pc, r1
00485afc: add r0, r4, #0x24
00485b00: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00485b04: ldr r1, [pc, #0x24]
00485b08: add r0, r4, #0x3c
00485b0c: mov r2, sp
00485b10: add r1, pc, r1
00485b14: bl #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00485b18: mov r0, r4
00485b1c: add sp, sp, #0xc
00485b20: pop {r4, r5, pc}
00485b24: subseq lr, r0, r4, ror #31
00485b28: andeq r3, r0, r4, asr #24
00485b2c: subeq pc, r4, r0, ror #1
00485b30: subeq r5, r4, r0, ror #29
