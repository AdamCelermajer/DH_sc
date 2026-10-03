
# _ZNK9Character9GetCharAIEv
003a3024: ldr      r3, [pc, #0x20]
003a3028: ldr      r2, [pc, #0x20]
003a302c: push     {r4, lr}
003a3030: add      r3, pc, r3
003a3034: ldr      r2, [r3, r2]
003a3038: ldr      r4, [r2]
003a303c: bl       #0x3a2fec
003a3040: mov      r3, #0x44
003a3044: mla      r0, r3, r0, r4
003a3048: pop      {r4, pc}
003a304c: subseq   r1, pc, r0, ror #20
003a3050: andeq    r0, r0, r8, asr r7
