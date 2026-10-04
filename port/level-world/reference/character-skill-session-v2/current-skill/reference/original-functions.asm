
# _ZN9Character20_GetCurrentSkillInfoERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b8f9c: push     {r4, r5, r6, r7, r8, lr}
003b8fa0: ldr      r3, [r0, #4]
003b8fa4: mov      r4, r0
003b8fa8: mov      r6, r2
003b8fac: ldm      r3, {r0, r2}
003b8fb0: mov      r5, r1
003b8fb4: rsb      r3, r0, r2
003b8fb8: asr      r3, r3, #4
003b8fbc: add      r2, r3, r3, lsl #3
003b8fc0: add      r2, r2, r2, lsl #6
003b8fc4: add      r2, r3, r2, lsl #3
003b8fc8: add      r2, r2, r2, lsl #15
003b8fcc: add      r3, r3, r2, lsl #3
003b8fd0: cmp      r3, #0
003b8fd4: bne      #0x3b8fdc
003b8fd8: pop      {r4, r5, r6, r7, r8, pc}
003b8fdc: ldr      r3, [r0, #4]
003b8fe0: cmp      r3, #3
003b8fe4: beq      #0x3b904c
003b8fe8: bl       #0x31bbf0
003b8fec: mov      r8, r0
003b8ff0: mov      r0, r6
003b8ff4: bl       #0x3bc5fc
003b8ff8: mov      r7, r0
003b8ffc: mov      r0, r8
003b9000: bl       #0x8be2a0
003b9004: ldr      r3, [r7, #4]
003b9008: cmp      r3, r0
003b900c: bls      #0x3b8fd8
003b9010: ldr      r4, [r4, #4]
003b9014: ldm      r4, {r0, r3}
003b9018: rsb      r3, r0, r3
003b901c: asr      r3, r3, #4
003b9020: add      r2, r3, r3, lsl #3
003b9024: add      r2, r2, r2, lsl #6
003b9028: add      r2, r3, r2, lsl #3
003b902c: add      r2, r2, r2, lsl #15
003b9030: add      r3, r3, r2, lsl #3
003b9034: cmp      r3, #0
003b9038: bne      #0x3b904c
003b903c: ldr      r0, [pc, #0x3c]
003b9040: add      r0, pc, r0
003b9044: bl       #0x708eb0
003b9048: ldr      r0, [r4]
003b904c: bl       #0x31bbf0
003b9050: bl       #0x30e4cc
003b9054: mov      r4, r0
003b9058: mov      r1, r4
003b905c: mov      r0, r6
003b9060: bl       #0x3bc784
003b9064: mov      r1, r4
003b9068: mov      r0, r6
003b906c: bl       #0x3bbed0
003b9070: mov      r1, r0
003b9074: mov      r0, r5
003b9078: pop      {r4, r5, r6, r7, r8, lr}
003b907c: b        #0x37cb24
003b9080: subseq   r5, r0, r8, lsr #8
