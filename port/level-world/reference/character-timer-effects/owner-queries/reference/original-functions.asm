
# _ZNK10ObjectBase17IsRemotelyUpdatedEv
0033dd10: ldr      r3, [r0, #0x110]
0033dd14: cmn      r3, #1
0033dd18: movne    r0, #1
0033dd1c: ldrbeq   r0, [r0, #0x118]
0033dd20: bx       lr

# _ZNK9Character6IsDeadEv
003a2ed4: movw     r3, #0x1449
003a2ed8: ldrb     r0, [r0, r3]
003a2edc: bx       lr
