
# _ZN12CharAnimatorC1Ev
003c906c: ldr      r1, [pc, #0x68]
003c9070: ldr      ip, [pc, #0x68]
003c9074: mov      r2, #0
003c9078: add      r1, pc, r1
003c907c: ldr      ip, [r1, ip]
003c9080: push     {r4, r5}
003c9084: add      ip, ip, #8
003c9088: mov      r5, #0x3f800000
003c908c: mvn      r4, #0
003c9090: str      ip, [r0]
003c9094: mov      ip, #1
003c9098: strb     r2, [r0, #0x5c]
003c909c: str      r5, [r0, #0x40]
003c90a0: strb     ip, [r0, #0x48]
003c90a4: str      r4, [r0, #0x50]
003c90a8: str      r2, [r0, #4]
003c90ac: str      r2, [r0, #0x2c]
003c90b0: strb     r2, [r0, #0x30]
003c90b4: str      r5, [r0, #0x34]
003c90b8: strb     r2, [r0, #0x38]
003c90bc: str      r4, [r0, #0x3c]
003c90c0: str      r2, [r0, #0x44]
003c90c4: strb     r2, [r0, #0x49]
003c90c8: strb     r2, [r0, #0x4a]
003c90cc: strb     r2, [r0, #0x54]
003c90d0: str      r2, [r0, #0x58]
003c90d4: pop      {r4, r5}
003c90d8: bx       lr
003c90dc: subseq   fp, ip, r8, lsl sl
003c90e0: andeq    r1, r0, r4, lsl #14
